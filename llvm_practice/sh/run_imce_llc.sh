basename=$(basename $1)
filename_no_ext=$(echo "$basename" | sed 's/\(.*\)\..*/\1/')

# llc --march=IMCE $1 -o ./output/$filename_no_ext.s -debug -force-hardware-loops -force-nested-hardware-loop
llc --march=IMCE $1 -o ./output/$filename_no_ext.s \
  -force-hardware-loops \
  -force-nested-hardware-loop \
  -debug-only=isel

llc --march=IMCE $1 -o ./output/$filename_no_ext.o \
  -force-hardware-loops \
  -force-nested-hardware-loop \
  -filetype=obj \
  -debug-only=isel

llvm-objdump -dr --triple=IMCE output/$filename_no_ext.o > output/$filename_no_ext.dump