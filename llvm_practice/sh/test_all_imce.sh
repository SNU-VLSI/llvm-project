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

  # compile with debug flag if -d is set
  if [ "$debug" = 'true' ]; then
    echo -n "Testing with -d: $fn"
    $script_dir/compile_imce_debug.sh $fn 2>&1 | tee -a ./logs/$(basename $fn).debug.log
  else
    echo -n "Testing: $fn"
    $script_dir/compile_imce.sh $fn 2> ./logs/$(basename $fn).log
  fi

  # check if the last command was successful
  if [ $? -ne 0 ]; then
    echo " ... Fail"
  else
    echo " ... Success"
  fi
done < $script_dir/testfiles.txt
