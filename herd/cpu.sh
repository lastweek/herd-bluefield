#!/bin/sh

END=23
for ((i=0;i<=END;i++)); do
    #echo 1200000 > /sys/devices/system/cpu/cpu$i/cpufreq/scaling_max_freq
    echo 3200000 > /sys/devices/system/cpu/cpu$i/cpufreq/scaling_max_freq
    #echo $i
    cat /sys/devices/system/cpu/cpu$i/cpufreq/scaling_max_freq
    done
#echo 3200000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq
