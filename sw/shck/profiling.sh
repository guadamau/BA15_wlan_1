#!/bin/sh
python -m cProfile -o ./profile.prof ./shck.py -d 192.168.0.2 -f ./testpayload -i prp1 -n 100 -P -s MAX -t UDP
mv ./profile.txt /results/shckprofile.txt
