#!/usr/bin/env python2.7
import random
values = []
while len(values) < 1000000:
    value = int(random.gauss(420,521))
    if 63 < value < 1515:
        values.append(value)
        print str(value)
