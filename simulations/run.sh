#!/bin/bash

#cluster02:
../pp-nbody/a.out cluster02.txt cluster02v3 1000 0.01 0.0005 1000 1 

cluster02_0.01:
../pp-nbody/a.out cluster02.txt cluster02v3 1000 0.01 ? 10 1

cluster02_0.001:
../pp-nbody/a.out cluster02.txt cluster02v3 1000 0.001 ? 10 1
