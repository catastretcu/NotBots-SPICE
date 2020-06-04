#!/bin/bash

set -e

g++ -std=c++11 simulate.cpp -o simulator

./simulator < op_test.in

echo "Success!"
