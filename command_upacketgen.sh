#!/bin/bash
./upacketgen -i --saddr_start 192.168.0.1 --saddr_end 192.168.0.10 \
--sport_start 10000 --sport_num 4000 \
--daddr_start 10.2.109.10 --daddr_end 10.2.109.10 \
--dport_start 10000 --dport_num 200 \
--usleep_num 0
