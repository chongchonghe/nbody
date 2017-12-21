#!/bin/bash

# pp-nbody run of small step size
#../../pp-nbody/a.out test.txt test4 3 0.0000001 0.0001 80000 800

# pp-nbody run of large step size
#../../pp-nbody/a.out test.txt test_large_step_size 3 0.0000001 0.01 800 8

# pythag01
rm -rf pythag01 pythag01_fig
../../pp-nbody/main pythag01.txt pythag01 3 0.00001 .00001 5000000 1000
# python movie2d.py pythag01 3.4 1
# python ../makemovie.py pythag01_fig
