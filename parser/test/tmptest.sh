#!/bin/bash

TESTFILES=*.p

for f in $TESTFILES
do
	../a.out < $f &> output
	if [ -s "output" ]; then
		echo $f FAILED
	else
		# echo $f PASSED
		echo -n
	fi
done

# rm output
