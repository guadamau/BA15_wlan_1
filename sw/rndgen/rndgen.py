#!/usr/bin/env python2.7
import random
values = []
while len(values) < 1000000:
    value = int(random.gauss(791,121.166666667))
    if 63 < value < 1519:
        values.append(value)
        print str(value)
