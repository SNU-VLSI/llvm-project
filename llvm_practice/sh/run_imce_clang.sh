basename=$(basename $1)
filename_no_ext=$(echo "$basename" | sed 's/\(.*\)\..*/\1/')
# clang -O3 --target=IMCE -S -emit-llvm $1 -I../test_cpp -o output/$filename_no_ext.ll

clang -O1 --target=IMCE -S -emit-llvm $1 -I../test_cpp -o output/$filename_no_ext.ll

llc --march=IMCE output/$filename_no_ext.ll -o ./output/$filename_no_ext.s \
  -force-hardware-loops \
  -force-nested-hardware-loop \
  -debug-only=isel
