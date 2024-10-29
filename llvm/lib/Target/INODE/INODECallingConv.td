//===-- INODECallingConv.td - INODE Calling Conventions ------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This describes the calling conventions for the INODE architectures.
// Only the OpenBSD variant is supported.
//
//===----------------------------------------------------------------------===//

// INODE 32-bit ELF C Calling convention.
def CC_INODE : CallingConv<[
  CCIfType<[i8, i16], CCPromoteToType<i32>>,
  CCIfType<[i32], CCAssignToReg<[SReg2, SReg3, SReg4, SReg5]>>,
  CCAssignToStack<4, 4>
]>;

// INODE 32-bit ELF C return-value convention.
def RetCC_INODE : CallingConv<[
  CCIfType<[i32], CCAssignToReg<[SReg2]>>
]>;

// INODE 32-bit ELF C callee saved registers.
def CSR_INODE : CalleeSavedRegs<(add SReg1, SReg2)>;