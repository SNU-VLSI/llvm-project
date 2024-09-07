//===-- IMCECallingConv.td - IMCE Calling Conventions ------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This describes the calling conventions for the IMCE architectures.
// Only the OpenBSD variant is supported.
//
//===----------------------------------------------------------------------===//

// IMCE 32-bit ELF C Calling convention.
def CC_IMCE : CallingConv<[
  CCIfType<[i8, i16, i32], CCPromoteToType<v16i16>>,
  CCIfSRet<CCIfType<[i32], CCAssignToReg<[V12]>>>,
  CCIfType<[i32,i64,f32,f64, v16i16], CCAssignToReg<[V2, V3, V4, V5, V6, V7, V8, V9]>>,
  CCAssignToStack<4, 4>
]>;

// IMCE 32-bit ELF C return-value convention.
def RetCC_IMCE : CallingConv<[
  // 32-bit values are returned in R2.
  CCIfType<[i32, v16i16], CCAssignToReg<[V2]>>,
]>;

// IMCE 32-bit ELF C callee saved registers.
def CSR_IMCE : CalleeSavedRegs<(add V1, V30, (sequence "V%d", 25, 14))>;
