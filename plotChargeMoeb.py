#!/usr/bin/env python2

import numpy as np
import os

dir = 'data/moebius/charge/fixed/'
for file in os.listdir(dir):
    data = np.loadtxt(dir+file)
    print "File: ", file, ' ', 'ratio: ', np.sum(abs(data)<0.1)

dir = 'data/moebius/charge/phys/'
for file in os.listdir(dir):
    data = np.loadtxt(dir+file)
    print "File: ", file, ' ', 'ratio: ', np.sum(abs(data)<0.1)
