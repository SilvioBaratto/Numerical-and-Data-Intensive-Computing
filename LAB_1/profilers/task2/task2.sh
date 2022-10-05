#!/bin/bash

make clean

make

sudo perf stat -e cycles:u -e instructions:u \
                -e L1-dcache-loads:u -e L1-dcache-load-misses:u \
                -e L1-dcache-stores:u -e L1-dcache-store-misses:u \
                -e LLC-loads:u -e LLC-load-misses:u \
                -e LLC-stores:u -e LLC-store-misses:u ./task2 
