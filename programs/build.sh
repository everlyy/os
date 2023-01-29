#! /bin/bash

set -xe

cd "$(dirname "$0")"

echo -n '' > programs

for dir in */
do
	make -C $dir $1
	echo -n "$(dirname "$0")/$dir$(basename "$dir") " >> programs
done