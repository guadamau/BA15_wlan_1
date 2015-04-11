#!/bin/sh
sudo ./shck.py -d 00:1E:4F:E5:BA:E8 -f ./testbin -i enp0s20u3 -n 30 -P -s RANDOM -t ETH
#sudo ./shck.py -d 160.85.23.245 -f ./testbin -i enp0s20u3 -n 0 -P -s MAX -t UDP
