#!/bin/bash

for file in "$@"
do
  echo -e "\n[Checking $file]"
  diff --suppress-common-lines --color=always -u1 <(cat $file) <(clang-format $file)
done
exit 0
