#/bin/sh

for f in *.h *.cpp *.y *.l
do
    expand -t 4 < $f > $f.tmp
    mv $f.tmp $f
done

