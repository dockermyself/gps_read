#!/bin/bash
echo "start program read gps"
sleep 10
cd /root/gps_g60/build && ./read_gps &
cd - &
