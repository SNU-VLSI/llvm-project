declare <16 x i16> @llvm.IMCE.ADD(<16 x i16>, <16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.SUB(<16 x i16>, <16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.AND(<16 x i16>, <16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.OR(<16 x i16>, <16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.XOR(<16 x i16>, <16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.SRL(<16 x i16>, <16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.SLL(<16 x i16>, <16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.SRA(<16 x i16>, <16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.MAX(<16 x i16>, <16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.MIN(<16 x i16>, <16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.MULTL(<16 x i16>, <16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.MULTH(<16 x i16>, <16 x i16>, i16);

declare <16 x i16> @llvm.IMCE.ADDI(<16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.SUBI(<16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.ANDI(<16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.ORI(<16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.XORI(<16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.SRLI(<16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.SLLI(<16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.SRAI(<16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.MAXI(<16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.MINI(<16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.MULTLI(<16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.MULTHI(<16 x i16>, i16);

declare <16 x i16> @llvm.IMCE.DWCONV(<16 x i16>, i16, i16, i16, i16);

declare void @llvm.IMCE.BNE(i16, <16 x i16>, i16);


declare void @llvm.IMCE.SEND(i16, <16 x i16>, i16, i16);
declare void @llvm.IMCE.RECV(i16, <16 x i16>, i16, i16);
declare void @llvm.IMCE.SET_FLAG(i16, <16 x i16>, i16, i16);
declare void @llvm.IMCE.STANDBY(i16, <16 x i16>, i16, i16);

declare <16 x i16> @llvm.IMCE.MAXPOOL(i16, i16, i16);
declare <16 x i16> @llvm.IMCE.AVGPOOL(i16, i16, i16);

declare <16 x i16> @llvm.IMCE.ADDQ(<16 x i16>, <16 x i16>, i16, i16);
declare <16 x i16> @llvm.IMCE.SUBQ(<16 x i16>, <16 x i16>, i16, i16);
declare <16 x i16> @llvm.IMCE.MULTLQ(<16 x i16>, <16 x i16>, i16, i16);
declare <16 x i16> @llvm.IMCE.MULTHQ(<16 x i16>, <16 x i16>, i16, i16);
declare <16 x i16> @llvm.IMCE.NU_QUANT(<16 x i16>, <16 x i16>, i16, i16);
declare <16 x i16> @llvm.IMCE.MM_QUANT(<16 x i16>, <16 x i16>, i16, i16);

declare void @llvm.IMCE.STEP();
declare void @llvm.IMCE.NOP();
declare void @llvm.IMCE.STOP();

declare void @llvm.IMCE.JUMP(i32);

declare <16 x i16> @llvm.IMCE.SCAN_RW(<16 x i16>);