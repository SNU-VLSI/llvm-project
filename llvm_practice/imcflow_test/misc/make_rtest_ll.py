names = ['ADD', 'SUB', 'AND', 'OR', 'XOR', 'SRL', 'SLL', 'SRA', 'MAX', 'MIN', 'MULTL', 'MULTH']

for i, name in enumerate(names):
  func_str = ""
  func_str += f"define <16 x i16> @f{i}(<16 x i16> %a, <16 x i16> %b)" + "{\n" 
  func_str += f"  %ret = call <16 x i16> @llvm.IMCE.{name}(<16 x i16> %a, <16 x i16> %b, i16 10)\n"
  func_str += f"  ret <16 x i16> %ret" + "\n}"

  print(func_str)