#!/bin/sh
../a.out < $1 > tmp.cpp
cat tmp.cpp
g++ tmp.cpp
