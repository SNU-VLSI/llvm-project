//===- IMCECountedLoopMIRPass - MIR level targeting of hardware loops -===//
//    Copyright (c) 2023 Graphcore Ltd. All Rights Reserved.
//     Licensed under the Apache License, Version 2.0 (the "License");
//     you may not use this file except in compliance with the License.
//     You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//     Unless required by applicable law or agreed to in writing, software
//     distributed under the License is distributed on an "AS IS" BASIS,
//     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//     See the License for the specific language governing permissions and
//     limitations under the License.
// --- LLVM Exceptions to the Apache 2.0 License ----
//
// As an exception, if, as a result of your compiling your source code, portions
// of this Software are embedded into an Object form of such source code, you
// may redistribute such embedded portions in such Object form without complying
// with the conditions of Sections 4(a), 4(b) and 4(d) of the License.
//
// In addition, if you combine or link compiled forms of this Software with
// software that is licensed under the GPLv2 ("Combined Software") and if a
// court of competent jurisdiction determines that the patent provision (Section
// 3), the indemnity provision (Section 9) or other Section of the License
// conflicts with the conditions of the GPLv2, you may retroactively and
// prospectively choose to deem waived or otherwise exclude such Section(s) of
// the License, but only in their entirety and only with respect to the Combined
// Software.
//
//===----------------------------------------------------------------------===//
//
// This pass translates counted loop pseudo instructions into real instructions
// See HardwareLoops + IMCELoopConversion for the IR passes that introduces
// the intrinsics that IMCEISelLowering lowers to ISD nodes that
// IMCEInstrInfo.td lowers to these pseudo instructions.
//
//===----------------------------------------------------------------------===//

#include <algorithm>
#include <iterator>

#include "IMCE.h"
// #include "IMCECoissueUtil.h"
#include "IMCECountedLoopOptions.h"
#include "IMCESubtarget.h"
#include "MCTargetDesc/IMCEMCTargetDesc.h"

#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineDominators.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineLoopInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/InitializePasses.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/GenericDomTreeConstruction.h"

using namespace llvm;
#define DEBUG_TYPE "imce-counted-loop-mir"
#define PASS_NAME "IMCE Counted Loop MIR Pass"

namespace {
class IMCECountedLoopMIR : public MachineFunctionPass {
private:
  const MachineDominatorTree *MDT = nullptr;
  const MachineLoopInfo *MLI = nullptr;
  const TargetInstrInfo *TII = nullptr;

  // After software pipelining, it's hard to retrieve the original
  // pre-pipelining preheader required to lower the cloops (guard is added to
  // preheader invalidating the 'preheader'ness of the basic block). Add to that
  // the possibility that only half of the cloop pseudo instructions got emitted
  // due to failure to emit the other half in isellowering and it becomes really
  // hard to find the original preheader. We'll try to do an initial traversal
  // of the loops and populate the map if the original preheader with cloop
  // pseudos can be found.
  DenseMap<MachineLoop *, MachineBasicBlock *> PreheaderMap;

public:
  static char ID;
  explicit IMCECountedLoopMIR() : MachineFunctionPass(ID) {}

  bool runOnMachineFunction(MachineFunction &F) override;
  void gatherPreheaders(MachineLoop &L, MachineBasicBlock *ParentPreheader);
  MachineBasicBlock *getPreheader(MachineLoop &L, bool &isPipelined);
  bool traverseLoop(MachineLoop &L);
  bool cleanPrologs(MachineBasicBlock *Preheader,
                    MachineBasicBlock *EndValueBB);

  StringRef getPassName() const override { return PASS_NAME; }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<MachineDominatorTree>();
    AU.addRequired<MachineLoopInfo>();
    MachineFunctionPass::getAnalysisUsage(AU);
  }
};
} // namespace
char IMCECountedLoopMIR::ID = 0;
INITIALIZE_PASS_BEGIN(IMCECountedLoopMIR, DEBUG_TYPE, PASS_NAME, false, false)
INITIALIZE_PASS_DEPENDENCY(MachineDominatorTree)
INITIALIZE_PASS_DEPENDENCY(MachineLoopInfo)
INITIALIZE_PASS_END(IMCECountedLoopMIR, DEBUG_TYPE, PASS_NAME, false, false)
FunctionPass *llvm::createIMCECountedLoopMIRPass() {
  return new IMCECountedLoopMIR();
}

// This parse transforms counted loops expressed in terms of CLOOP_* pseudos
// into real instructions. This pass runs after register allocation in order to
// determine an accurate instruction count for rpt.

// A generalised input, removing coissue operands for clarity:
// bb.0.entry:
//   successors: %bb.1, %bb.X, ...
//   InstrG
//   CLOOP_GUARD_BRANCH $mi, %bb.X
//   InstrH
//   br %bb.1 # may be elided
//
// bb.1.for.body.preheader:
//   successors: %bb.2
//   InstrA
//   $mi = CLOOP_BEGIN_VALUE killed $mi, m # out reg == in reg
//   InstrB
//   CLOOP_BEGIN_TERMINATOR $mi, m
//   InstrC
//   br %bb.2 # may be elided
//
// bb.2.for.body:
// ; predecessors: %bb.1, %bb.2
//   successors: %bb.2, %bb.3
//   liveins: $mi, others
//   InstrD
//   $mi = CLOOP_END_VALUE killed $mi, m # out reg == in reg
//   InstrE
//   CLOOP_END_BRANCH $mi, %bb.2, m
//   InstrF
//   br %bb.3 # may be elided
//
// The unconditional branches at the end of the blocks may have been removed
// by analyzeBranch.
// InstrA represents whatever comes before this loop. It shouldn't contain
// another counted loop, as each body gets its own basic block.
// InstrB will generally not be empty.
// InstrC is ideally empty, but can contain copies introduced by phi elimination
// InstrD is the first part of the loop body. InstrE the second part.
// InstrF is ideally empty, but can contain copies introduced by phi
// elimination.
//
// InstrG represents whatever comes before the loop guard.
// InstrH should be empty, but the passes assume there could be instructions.
// CLOOP_GUARD_BRANCH may not always be present, such as hinting on
// BNE or if loop entry required a different comparison, eg `while (x < y)`.
// In this case a BRZ instruction may be present instead. This ideally should
// branch to loop exit (bb.3 above), but might be different. bb.0 ideally has 2
// successors, but could have 1 if no loop guard (known nonzero trip count) or
// more for complex cases. Any possible branch in InstrH would be considered a
// terminator instruction.

// The fallback transform, used when nothing better is available, transforms to:
// bb.0.entry:
//   successors: %bb.1
//   InstrG
//   brz $mi, %bb.X
//   InstrH
//   br %bb.1 # may be elided
//
// bb.1.for.body.preheader:
//   successors: %bb.2
//   InstrA
//   InstrB
//   InstrC
//   br %bb.2 # may be elided
//
// bb.2.for.body:
// ; predecessors: %bb.1, %bb.2
//   successors: %bb.2, %bb.3
//   liveins: $mi, others
//   InstrD
//   $mi = add $mi, -1
//   InstrE # InstrE may clobber $mi, as long as it is restored before the brnz
//   brnz $mi, %bb.2
//   InstrF
//   br %bb.3 # may be elided
//
// Note that this deletes both pseudos from the header and transformed both in
// the body. This means that it can be applied to reliably lower the pseudos,
// even when the basic blocks have become separated and each is lowered
// individually.
// This also applies to the loop guard pseudo, which is simply lowered to brz.

// The next most commonly applicable transform uses BNE. Unfortunately this
// requires a sub 1 in the header to provide the right trip count, so requires
// both blocks to be available without an intermediate block inserted inbetween.
// Loop guard converted to brz, similar to above.
// bb.1.for.body.preheader:
//   successors: %bb.2
//   InstrA
//   $mi = add $mi, -1
//   InstrB
//   CLOOP_BEGIN_TERMINATOR $mi, m
//   InstrC
//   br %bb.2 # may be elided
//
// bb.2.for.body:
// ; predecessors: %bb.1, %bb.2
//   successors: %bb.2, %bb.3
//   liveins: $mi, others
//   InstrD
//   InstrE
//   BNE $mi, %bb.2
//   InstrF # requires none of InstrF to clobber $mi
//   br %bb.3 # may be elided

// The simple rpt lowering is only valid if the maximum trip count < 4095
// This is indicated by metadata m == tripCountOKForRpt. There are also various
// constraints on the instructions in the InstrX sequences, e.g. InstrC must be
// empty at present. Some contraints may be relaxed with further testing.
//
// If the loop guard pseudo is safe to remove - loop guard branches to loop exit
// and InstrA, InstrB, InstrH dont modify any of the live registers in loop
// exit, as well as dont have side effects - then the loop pseudo is removed.
//
// bb.0.entry:
//   successors: %bb.1
//   InstrG
//   brz $mi, %bb.X
//   InstrH
//   br %bb.1 # may be elided
//
// bb.1.for.body.preheader:
//   successors: %bb.2
//   InstrA
//   InstrB
//   br %bb.2 # may be elided
//
// bb.2.for.body:
// ; predecessors: %bb.1, %bb.2
//   successors: %bb.2, %bb.3
//   liveins: $mi, others
//   rpt $mi, size_field
//   InstrD
//   InstrE
//   # end of rpt body
//   InstrF
//   br %bb.3 # may be elided

// Loop Guard Removal
// Lets assume we had the following code
// {
//   int sum = 42;
//
//   if (N == 0)
//       return 10;
//
//   for (int i=0; i<N; ++i)
//       arr[i] += something
//
//   return sum;
// }
//
// One way of expressing this in assembly, assuming a pass takes advantage of
// the loop guard:
//
// Entry:
// $m0 = 10;
// CLOOP_GUARD_BRANCH $m5, LoopExit
// $m0 = 42                           # $m0 is modified
// RPT
// { ...
// }
//
// LoopExit:                          # $m0 is live
// RTN
//
// In this case there is an instruction between CLOOP_GUARD_BRANCH and RPT
// (formerly CLOOP_BEGIN_TERMINATOR). This would not have been a problem, had it
// not mmodified $m0, which is live just after the loop. Hence
// CLOOP_GUARD_BRANCH cannot be safely removed as this could change the
// behaviour of the program. The same would apply had this been an instruction
// with a side effect such as a call or store.

// Software Pipelining
// Some exotic basic block orders/structures may occur when a loop is software
// pipelined by the MachinePipeliner pass.
// Originally a loop body basic block would be preceeded by a preheader basic
// block where the body has CLOOP_END_* instructions and the preheader has
// CLOOP_BEGIN_* instructions.
// To illustrate:
//
// Preheader -> loop body -> exit
//              ^        |
//              |        |
//               --------
//
// Now, in addition to the orignal loop basic blocks structure, the loop body
// may be split up in multiple basic blocks. Particularly, the loop body basic
// block may now be converted into 1 or more prologs basic blocks, 1 loop kernel
// basic block, and 1 or more epilog basic blocks. Within these additional
// blocks the amount and type of CLOOP_* instructions may differ.
// Prologs may have CLOOP_END_VALUE instructions which are transformed into a
// subtraction of 1.
// Kernels should be equivalent to the (previous) loop body in terms of
// CLOOP_END_* instructions.
// Epilogs should not have any of the CLOOP_* pseudo instructions.
// Additionally, the previous preheader may now not be a preheader llvm defines
// a preheader.
// To illustrate:
//
// "Preheader" -> Prolog -> Kernel -> Epilog -> exit
//            |         |  ^      |  ^         ^
//            |         |  |      |  |         |
//            |         |   -----    |         |
//            |          ------------          |
//             --------------------------------
//
// The "Preheader" and Kernel pair will be treated similar to the preheader and
// loop body before (to determine BNE).

namespace {
bool eliminatePseudos(MachineBasicBlock *BB, const TargetInstrInfo &TII) {
  DebugLoc dl;
  if (!BB)
    return false;

  // The fallback is to remove nodes from the header and
  // replace those in the footer with a subtract and a brnz
  bool changed = false;
  for (auto BBI = BB->instr_begin(), E = BB->instr_end(); BBI != E;) {
    unsigned opc = BBI->getOpcode();
    switch (opc) {
    default: {
      ++BBI;
      break;
    }
    case IMCEISD::CLOOP_BEGIN_VALUE: {
      assert(BBI->getOperand(0).getReg() == BBI->getOperand(1).getReg());
      BBI = BB->erase(BBI);
      changed = true;
      break;
    }
    case IMCEISD::CLOOP_BEGIN_TERMINATOR: {
      BBI = BB->erase(BBI);
      changed = true;
      break;
    }
    case IMCEISD::CLOOP_END_VALUE: {
      unsigned r = BBI->getOperand(0).getReg();
      assert(r == BBI->getOperand(1).getReg());
      assert(0 && "CLOOP_END_VALUE should have been eliminated by now");
      // This add could sometimes be eliminated by modifying existing
      // instructions, e.g. if the def is an add +1
      // BuildMI(*BB, BBI, dl, TII.get(IMCE::ADD_SI), r)
      //     .addReg(r)
      //     .addImm(-1)
      //     .addImm(0 /* coissue */);

      BBI = BB->erase(BBI);
      changed = true;
      break;
    }
    case IMCEISD::CLOOP_END_BRANCH: {
      assert(0 && "CLOOP_END_BRANCH should have been eliminated by now");
      // BuildMI(*BB, BBI, dl, TII.get(IMCE::BRNZ))
      //     .add(BBI->getOperand(0))
      //     .add(BBI->getOperand(1))
      //     .addImm(0 /* coissue */);
      BBI = BB->erase(BBI);
      changed = true;
      break;
    }
    case IMCEISD::CLOOP_GUARD_BRANCH: {
      assert(0 && "CLOOP_GUARD_BRANCH should have been eliminated by now");
      // BuildMI(*BB, BBI, dl, TII.get(IMCE::BRZ))
      //     .add(BBI->getOperand(0))
      //     .add(BBI->getOperand(1))
      //     .addImm(0 /* coissue */);
      BBI = BB->erase(BBI);
      changed = true;
      break;
    }
    }
  }
  return changed;
}

/// Lower CLOOP_GUARD_BRANCH into a fallback brz instruction.
bool eliminateLoopGuardPseudo(SmallVectorImpl<MachineInstr *> &loopGuards,
                              const TargetInstrInfo &TII) {
  bool changed = false;
  for (auto loopGuard : loopGuards) {
    if (eliminatePseudos(loopGuard->getParent(), TII))
      changed = true;
  }
  return changed;
}

MachineBasicBlock::instr_iterator
findBySearchingFromTerminator(MachineBasicBlock *BB, unsigned opcode) {
  MachineBasicBlock::instr_iterator fail = BB->instr_end();
  auto TI = BB->getFirstInstrTerminator();
  if (TI == fail) {
    return fail;
  }

  for (;; --TI) {
    if (TI->getOpcode() == opcode) {
      return TI;
    }
    if (TI == BB->instr_begin()) {
      return fail;
    }
  }
}

bool containsPseudos(MachineBasicBlock *BB, std::vector<unsigned> Pseudos,
                     bool ContainsAll = true) {
  if (!BB || Pseudos.empty())
    return false;

  enum PseudoType {
    BEGIN_VAL = 0x1,
    BEGIN_TERM = 0x2,
    END_VAL = 0x4,
    END_BR = 0x8,
    GUARD_BR = 0x16
  };

  auto MappedPseudo = [](unsigned opcode) -> unsigned {
    switch (opcode) {
    default:
      return 0;
    case IMCEISD::CLOOP_BEGIN_VALUE:
      return PseudoType::BEGIN_VAL;
    case IMCEISD::CLOOP_BEGIN_TERMINATOR:
      return PseudoType::BEGIN_TERM;
    case IMCEISD::CLOOP_END_VALUE:
      return PseudoType::END_VAL;
    case IMCEISD::CLOOP_END_BRANCH:
      return PseudoType::END_BR;
    case IMCEISD::CLOOP_GUARD_BRANCH:
      return PseudoType::GUARD_BR;
    }
  };

  unsigned AllPseudosInBB = 0;
  for (auto &I : *BB) {
    unsigned MP = MappedPseudo(I.getOpcode());
    if (AllPseudosInBB & MP)
      return false;
    AllPseudosInBB |= MP;
  }

  unsigned AllPseudosInArg = 0;
  for (auto &I : Pseudos) {
    unsigned MP = MappedPseudo(I);
    if (AllPseudosInArg & MP)
      return false;
    AllPseudosInArg |= MP;
  }

  AllPseudosInBB &= AllPseudosInArg;

  if (ContainsAll)
    return AllPseudosInBB == AllPseudosInArg;
  else
    return !!(AllPseudosInBB);
}

void findLoopGuardPseudos(MachineBasicBlock *header,
                          SmallVectorImpl<MachineInstr *> &loopGuards) {
  if (!header)
    return;

  // Search header's predecessors BB for loop guard pseudo.
  for (auto BBI = header->pred_begin(), BBE = header->pred_end(); BBI != BBE;
       ++BBI) {
    MachineBasicBlock *PredBB = *BBI;
    MachineBasicBlock::instr_iterator Inst;
    if ((Inst = findBySearchingFromTerminator(
             PredBB, IMCEISD::CLOOP_GUARD_BRANCH)) != PredBB->instr_end()) {
      loopGuards.push_back(&*Inst);
    }
  }
}

Error canLowerToBNE(MachineBasicBlock *header, MachineBasicBlock *body) {
  // Assumes that header contains begin pseudos that
  // arrange for the correct register to be live on entry

  auto bodyEndBranch = body->getFirstInstrTerminator();
  assert(bodyEndBranch->getOpcode() == IMCEISD::CLOOP_END_BRANCH);
  const unsigned reg = bodyEndBranch->getOperand(0).getReg();

  // Instructions inserted between CLOOP_END_VALUE and CLOOP_END_BRANCH
  // prevents the use of BNE if they use the HWLOOP register because they
  // assume decrement has happened but not the branch. That assumption is
  // invalid once BNE is inserted in place of CLOOP_END_BRANCH. Such
  // instructions can be inserted by phi elimination for instance.

  auto MI = bodyEndBranch;
  for (--MI; MI->getOpcode() != IMCEISD::CLOOP_END_VALUE; --MI) {
    for (auto &MO : MI->operands()) {
      if (MO.isReg() && MO.getReg() == reg) {
        std::string OrStr;
        raw_string_ostream OrStrStream(OrStr);
        MI->print(OrStrStream, /*IsStandalone=*/true, /*SkipOpers=*/false,
                  /*SkipDebugLoc=*/false, /*AddNewLine=*/false);
        return createStringError(
            std::errc::operation_not_permitted,
            ("BNE register referred to between decrement and branch by\n" +
             OrStrStream.str())
                .c_str());
      }
    }
    assert(MI != body->instr_begin() && "loop does not have CLOOP_END_VALUE");
  }

  assert(MI->getOperand(0).getReg() == reg);
  return Error::success();
}

void lowerToBNE(MachineBasicBlock *header, MachineBasicBlock *body,
                const TargetInstrInfo &TII) {
  DebugLoc dl;
  assert(!bool(canLowerToBNE(header, body)));

  auto headerBeginValue =
      findBySearchingFromTerminator(header, IMCEISD::CLOOP_BEGIN_VALUE);
  auto headerBeginTerminator = header->getFirstInstrTerminator();
  auto bodyEndValue =
      findBySearchingFromTerminator(body, IMCEISD::CLOOP_END_VALUE);
  auto bodyEndBranch = body->getFirstInstrTerminator();

  Register reg = headerBeginValue->getOperand(0).getReg();
  MachineInstr *InstrBeforeCLoopBegin = headerBeginValue->getPrevNode();

  // Check for instruction preceding CLOOP_BEGIN_VALUE that modifies the
  // induction counter and fold the decrement into it.
  // VINN: we don't apply fold decrement for now: check again if we implement
  // SETZI, ADD_SI, ADD_ZI, etc in front of CLOOP_BEGIN_VALUE
  // auto foldDecrementIntoInstr = [&]() { return false };

  // Prefer folding the decrement into existing instructions.
  // if (!foldDecrementIntoInstr())
  //   BuildMI(*header, headerBeginValue, dl, TII.get(IMCE::ADD_SI), reg)
  //       .add(headerBeginValue->getOperand(1))
  //       .addImm(-1)
  //       .addImm(0 /*coissue*/);

  // VINN: get the immediate value from instruction prior to headerBeginValue.
  unsigned opcode = InstrBeforeCLoopBegin->getOpcode();
  int64_t hw_loop_cnt = 100;
  // if (opcode == IMCE::SETZI) {
  //   assert(InstrBeforeCLoopBegin->getOperand(0).isReg() &&
  //          InstrBeforeCLoopBegin->getOperand(0).getReg() == reg);
  //   hw_loop_cnt = InstrBeforeCLoopBegin->getOperand(1).getImm();
  // } else if (opcode == IMCE::ADD_SI || opcode == IMCE::ADD_ZI) {
  //   assert(InstrBeforeCLoopBegin->getOperand(0).isReg() &&
  //          InstrBeforeCLoopBegin->getOperand(0).getReg() == reg);
  //   assert(InstrBeforeCLoopBegin->getOperand(1).isReg() &&
  //          InstrBeforeCLoopBegin->getOperand(1).getReg() == 0);
  //   hw_loop_cnt = InstrBeforeCLoopBegin->getOperand(2).getImm();
  // } else {
  //   assert(0 &&
  //          "Opcode before CLOOP_BEGIN_VALUE should be SETZI/ADD_SI/ADD_ZI");
  // }
  assert(hw_loop_cnt != 0);

  headerBeginValue->eraseFromParent();
  headerBeginTerminator->eraseFromParent();
  bodyEndValue->eraseFromParent();

  // VINN: get the immediate value from instruction prior to headerBeginValue?
  BuildMI(*body, bodyEndBranch, dl, TII.get(IMCE::IMCE_BNE_INST),
          bodyEndBranch->getOperand(0).getReg()) // $rs2 == $rs1
      .add(bodyEndBranch->getOperand(1))         // branch target (bb)
      .add(bodyEndBranch->getOperand(0))         // $rs1
      .addImm(hw_loop_cnt);                      // $imm: hw_loop_cnt_max
  bodyEndBranch->removeFromParent();
}

// Walks up the MDT to find the matching CLOOP_BEGIN_VALUE machineinstr (and its
// MBB).
MachineBasicBlock *findMatchingEndVal(MachineInstr *BeginTerm,
                                      const MachineDominatorTree *MDT) {
  assert(BeginTerm && BeginTerm->getOpcode() == IMCEISD::CLOOP_BEGIN_TERMINATOR &&
         "Arg can only be a cloop begin terminator");
  assert(MDT && "MachineDominatorTree must be populated");

  MachineDomTreeNode *BeginValDN = MDT->getNode(BeginTerm->getParent());
  for (; BeginValDN; BeginValDN = BeginValDN->getIDom()) {
    if (!containsPseudos(BeginValDN->getBlock(), {IMCEISD::CLOOP_BEGIN_VALUE}))
      continue;
    auto BeginValI = findBySearchingFromTerminator(BeginValDN->getBlock(),
                                                   IMCEISD::CLOOP_BEGIN_VALUE);
    // Confirm the def register in begin_val is the same as the use register of
    // begin_term.
    if (BeginValI->getOperand(0).getReg() ==
        BeginTerm->getOperand(0).getReg()) {
      return BeginValDN->getBlock();
    }
  }
  return nullptr;
}
} // namespace

// Clean up all prologs that occur between the Preheader and EndValueBB.
bool IMCECountedLoopMIR::cleanPrologs(MachineBasicBlock *Preheader,
                                      MachineBasicBlock *EndValueBB) {
  // If Preheader dominates EndValueBB, we should find Preheader on the path
  // from EndValueBB to entry.
  if (!MDT->properlyDominates(Preheader, EndValueBB))
    return false;
  bool changed = false;
  // Walk all the immediate dominators between EndValueBB and Preheader
  // (exclusive).
  MachineDomTreeNode *prolog = MDT->getNode(EndValueBB)->getIDom();
  while (prolog != MDT->getRootNode() && prolog->getBlock() != Preheader) {
    changed |= eliminatePseudos(prolog->getBlock(), *TII);
    prolog = prolog->getIDom();
  }
  return changed;
}

// Gather the "original" preheaders which may differ from the current preheaders
// depending on software pipelining. Note that this runs recursively through all
// loops much like traverseLoop (even if by processing the outer loops prior to
// the inner loops unlike traverseLoop).
void IMCECountedLoopMIR::gatherPreheaders(MachineLoop &L,
                                          MachineBasicBlock *ParentPreheader) {
  MachineBasicBlock *Preheader = L.getLoopPreheader();
  if (!containsPseudos(Preheader, {IMCEISD::CLOOP_BEGIN_TERMINATOR})) {
    Preheader = nullptr;
    if (L.getLoopPredecessor()) {
      MachineDomTreeNode *DomTreePreheader =
          MDT->getNode(L.getLoopPredecessor());
      while (DomTreePreheader &&
             DomTreePreheader->getBlock() != ParentPreheader &&
             !containsPseudos(DomTreePreheader->getBlock(),
                              {IMCEISD::CLOOP_BEGIN_TERMINATOR})) {
        DomTreePreheader = DomTreePreheader->getIDom();
      }
      Preheader =
          DomTreePreheader && DomTreePreheader->getBlock() != ParentPreheader
              ? DomTreePreheader->getBlock()
              : nullptr;
    }
  }

  if (Preheader)
    PreheaderMap.insert(std::make_pair(&L, Preheader));

  // Take care of the inner loops after the current loop has found its
  // preheader.
  for (auto &InnerLoop : L)
    gatherPreheaders(*InnerLoop, Preheader ? Preheader : MDT->getRoot());
}

// Retrieve the gathered original preheaders using the current loop as key for
// the map populated by gatherPreheaders.
MachineBasicBlock *IMCECountedLoopMIR::getPreheader(MachineLoop &L,
                                                    bool &isPipelined) {
  MachineBasicBlock *preheader = L.getLoopPreheader();
  if (!containsPseudos(preheader, {IMCEISD::CLOOP_BEGIN_TERMINATOR}))
    isPipelined = true;

  if (PreheaderMap.find(&L) == PreheaderMap.end())
    return nullptr;

  return PreheaderMap.find(&L)->second;
}

bool IMCECountedLoopMIR::traverseLoop(MachineLoop &L) {
  bool changed = false;
  bool isPipelined = false;

  // Innermost loop first.
  for (auto &InnerLoop : L)
    changed |= traverseLoop(*InnerLoop);

  LLVM_DEBUG(dbgs() << "Traversing loop: "; L.dump(););

  // Critical edge splitting can result in cloned instructions. In that case
  // there may be multiple backedges (i.e. loop latch basic blocks) with the
  // IMCE pseudos.
  MachineBasicBlock *EndBranchBB = nullptr;
  SmallVector<MachineBasicBlock *> Latches;
  L.getLoopLatches(Latches);
  unsigned numFoundBranchPseudos = 0;
  for (MachineBasicBlock *MBB : Latches) {
    if (containsPseudos(MBB, {IMCEISD::CLOOP_END_BRANCH})) {
      EndBranchBB = MBB;
      numFoundBranchPseudos++;
    }
  }

  // If there's no preheader present, there's a good possibility the loop is
  // software pipelined. We will have to manually find the MBB preceeding the
  // {prolog*,kernel,epilog} pipeline MBBs (even if it is technically not a
  // preheader in strict terms).
  MachineBasicBlock *Preheader = getPreheader(L, isPipelined);
  SmallVector<MachineInstr *, 4> loopGuards;
  findLoopGuardPseudos(Preheader, loopGuards);

  // If there are more CLOOP_END_BRANCH instructions found within the same
  // loop, bail out.
  if (numFoundBranchPseudos > 1) {
    // Bail out.
    changed |= eliminateLoopGuardPseudo(loopGuards, *TII);
    for (MachineBasicBlock *MBB : Latches) {
      changed |= eliminatePseudos(MBB, *TII);
    }
    changed |= eliminatePseudos(Preheader, *TII);
    return changed;
  }

  // Some pass(es) may cause the branch to exist in the header. If that's the
  // case, bail out.
  if (!EndBranchBB) {
    EndBranchBB = L.getHeader();
    if (!containsPseudos(EndBranchBB,
                         {IMCEISD::CLOOP_END_VALUE, IMCEISD::CLOOP_END_BRANCH})) {
      changed |= eliminateLoopGuardPseudo(loopGuards, *TII);
      changed |= eliminatePseudos(Preheader, *TII);
      changed |= eliminatePseudos(EndBranchBB, *TII);
      return changed;
    }
  }

  // If preheader doesn't exists, bail out.
  if (!Preheader) {
    changed |= eliminateLoopGuardPseudo(loopGuards, *TII);
    changed |= eliminatePseudos(EndBranchBB, *TII);
    return changed;
  }

  auto bodyEndValue =
      findBySearchingFromTerminator(EndBranchBB, IMCEISD::CLOOP_END_VALUE);

  if (!containsPseudos(
          Preheader, {IMCEISD::CLOOP_BEGIN_TERMINATOR, IMCEISD::CLOOP_BEGIN_VALUE}) ||
      Preheader->getFirstInstrTerminator() == Preheader->instr_end()) {
    // Some pass(es) may have split BEGIN_VALUE and BEGIN_TERMINATOR so find the
    // associated BEGIN_VALUE if we know BEGIN_TERMINATOR in Preheader and
    // delete the BEGIN_VALUE.
    auto BeginTerm =
        findBySearchingFromTerminator(Preheader, IMCEISD::CLOOP_BEGIN_TERMINATOR);
    if (BeginTerm != Preheader->instr_end())
      changed |= eliminatePseudos(findMatchingEndVal(&*BeginTerm, MDT), *TII);
    changed |= eliminateLoopGuardPseudo(loopGuards, *TII);
    changed |= eliminatePseudos(Preheader, *TII);
    changed |= eliminatePseudos(EndBranchBB, *TII);
    return changed;
  }

  assert(MDT->dominates(Preheader, EndBranchBB) &&
         "Preheader should dominate all parts of the loop.");

  auto headerBeginValue =
      findBySearchingFromTerminator(Preheader, IMCEISD::CLOOP_BEGIN_VALUE);
  if (headerBeginValue == Preheader->instr_end() ||
      bodyEndValue == EndBranchBB->instr_end()) {
    report_fatal_error("Malformed MIR. Missing pseudo for counted loop");
  }

  auto cloopValueRegistersMatch = [](MachineBasicBlock::instr_iterator i) {
    assert(i->getOpcode() == IMCEISD::CLOOP_BEGIN_VALUE ||
           i->getOpcode() == IMCEISD::CLOOP_END_VALUE);
    return i->getOperand(0).getReg() == i->getOperand(1).getReg();
  };

  if (!cloopValueRegistersMatch(headerBeginValue) ||
      !cloopValueRegistersMatch(bodyEndValue)) {
    report_fatal_error("Malformed MIR. Cloop value registers must match");
  }

  LLVM_DEBUG(dbgs() << "Analysing counted loop with body "
                    << EndBranchBB->getName() << "\n";);

  // TODO: Maybe we can remove the guard for BNE case if we know the trip
  // count.
  changed |= eliminateLoopGuardPseudo(loopGuards, *TII);

  if (Error BNELoweringError = canLowerToBNE(Preheader, EndBranchBB)) {
    LLVM_DEBUG(dbgs() << "Cannot use BNE: " << BNELoweringError << "\n";);
    consumeError(std::move(BNELoweringError));
  } else {
    lowerToBNE(Preheader, EndBranchBB, *TII);
    if (isPipelined)
      cleanPrologs(Preheader, EndBranchBB);
    LLVM_DEBUG(dbgs() << "Lowered to BNE. New loop body:\n";
               EndBranchBB->dump(); dbgs() << "\n";);
    return true;
  }

  LLVM_DEBUG(dbgs() << "Eliminating counted loop pseudos\n";);
  changed |= eliminatePseudos(EndBranchBB, *TII);
  changed |= eliminatePseudos(Preheader, *TII);
  if (L.getLoopPredecessor())
    changed |= eliminatePseudos(L.getLoopPredecessor(), *TII);
  LLVM_DEBUG(dbgs() << "Cleaned up loop body:\n"; EndBranchBB->dump();
             dbgs() << "\n";);

  return changed;
}

bool IMCECountedLoopMIR::runOnMachineFunction(MachineFunction &mf) {
  auto &ST = mf.getSubtarget<IMCESubtarget>();
  TII = ST.getInstrInfo();

  if (!IMCE::CountedLoop::EnableMIR)
    return false;

  LLVM_DEBUG(dbgs() << "IMCE counted loop mir on function " << mf.getName()
                    << "\n";);

  bool changed = false;
  PreheaderMap.clear();
  MDT = &getAnalysis<MachineDominatorTree>();
  MLI = &getAnalysis<MachineLoopInfo>();

  // Cache the "original" preheaders if found.
  for (auto &L : *MLI)
    gatherPreheaders(*L, nullptr);

  for (auto &L : *MLI)
    changed |= traverseLoop(*L);

  return changed;
}