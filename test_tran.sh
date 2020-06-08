#!/bin/bash

set -e

g++ -std=c++11 simulate.cpp -o simulator

./simulator < tran_test.in

echo "Success!"
