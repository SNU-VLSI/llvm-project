#!/bin/bash

mkdir -p ./logs
script_dir=$(dirname "$0")
debug=''


print_usage() {
  printf "Usage: test_all_imce.sh [-d]\n"
}

while getopts 'd' flag; do
  case "${flag}" in
    d) debug='true' ;;
    *) print_usage
       exit 1 ;;
  esac
done

# read lines from "./testfiles" and compile the files
echo "Reading from $script_dir/testfiles.txt"
while IFS= read -r fn || [[ -n "$fn" ]]; do
  # get filename without ext
  fn_base=$(basename $fn)
  fn_no_ext=$(echo "$fn_base" | sed 's/\(.*\)\..*/\1/')

  # compile with debug flag if -d is set
  if [ "$debug" = 'true' ]; then
    echo "Testing with -d: $fn"
    $script_dir/compile_imce_debug.sh $fn 2> ./logs/$fn_no_ext.debug.log
  else
    echo "Testing: $fn"
    $script_dir/compile_imce.sh $fn 2> ./logs/$fn_no_ext.log
  fi

done < $script_dir/testfiles.txt
