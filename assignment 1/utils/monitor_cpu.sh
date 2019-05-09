#!/bin/bash

interval=0.05
iterations=200

if [ -z "$1" ]
  then
    echo "No argument supplied to define process id"
    exit 1
fi

echo "=== CPU USAGE MONITOR ==="
echo
echo "Checking the process with pid $1 ..."

for iteration in `seq $iterations`
do
  ps -p $1 -o %cpu
  sleep $interval
done

echo
echo "=== DONE ==="
