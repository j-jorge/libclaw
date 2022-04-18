#!/bin/sh

for easing in $(./ex-tween 2>&1 | tail -1 | cut -d: -f2- | tr -d ',')
do
    ./ex-tween $easing
    gnuplot mkpng.gp

    mv in.png $easing-in.png
    mv out.png $easing-out.png
    mv inout.png $easing-inout.png
done
