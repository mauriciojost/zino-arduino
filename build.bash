#!/bin/bash

set -e

curr_dir=$(dirname $0)
root_dir=$(readlink -e $curr_dir)
generate_test_runner_script="$root_dir/libs/unity/auto/generate_test_runner.rb"
test_runner_dir="$root_dir/test_runner/"
general_flags="-D UNIT_TEST -D SUBCYCLES_2 -D LOG_LEVEL=0"


function run_test() {

  export main_src="$1"
  export main_src_name=$(basename $main_src)
  export main_src_runner="$test_runner_dir"/"$main_src_name"

  export flags="$2"

  export headers=$(find * -name "*.h" | xargs -I% dirname % | uniq | grep -v test | grep -v unity | xargs -I% echo "-I %")
  export src_c=$(find * -name "*.c" | grep -v test | grep -v unity | xargs -I% echo " %")
  export src_cpp=$(find * -name "*.cpp" | grep -v test | grep -v unity | xargs -I% echo " %")

  export headers_unity="-I $root_dir/libs/unity/src "
  export src_unity="$root_dir/libs/unity/src/*.c"

  ruby "$generate_test_runner_script" "$main_src" "$main_src_runner"

  rm -f "$root_dir"/simulator.bin

  g++ -o "$root_dir"/simulator.bin $flags "$main_src_runner" "$main_src" $src_c $src_cpp $src_unity $headers $headers_unity

  "$root_dir"/simulator.bin

  echo "EXIT CODE: $?"

  rm "$root_dir"/simulator.bin

}

platformio run

rm -fr "$test_runner_dir"
mkdir -p "$test_runner_dir"

for f in $(find test -name *.cpp)
do
  echo ""
  echo ""
  echo "### Running $f"
  echo ""
  echo ""
  run_test "$f" "$general_flags"
done
