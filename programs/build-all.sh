#! /bin/bash

set -xe

cd "$(dirname "$0")"

echo -n '' > programs

for dir in */
do
	echo -n "$(dirname "$0")/$dir$(basename "$dir") " >> programs
	make -C $dir $1
done