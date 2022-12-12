#!/bin/bash
while true; do
  radamsa test.json > input.txt
  jq . input.txt > /dev/null 2>&1
  if [ $? -gt 127 ]; then
    cp input.txt crash-`date +s%.%N`.txt
    echo "Crash found!"
  fi
done
