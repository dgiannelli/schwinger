#!/usr/bin/env python2

import os
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import rc
from scipy.optimize import curve_fit

from jackknife import jackknife

rc('text', usetex=True)
rc('font', **{'family': 'serif', 'serif':['Computer Modern']}) 

if not os.path.exists('plots'): os.makedirs('plots')

dataDir = 'data/torus/charge/phys/'
#count = 0
for file in os.listdir(dataDir):
    f = open(dataDir+file, 'r')
    for i in range(2):
        exec(f.readline()[1:]) #Read from file the value of beta and N
    f.close()
    data = np.loadtxt(dataDir+file, dtype=int)
    QsqMean, dQsqMean = jackknife(data)
    ratio = QsqMean/N**2
    dratio = dQsqMean/N**2

    label='$\\beta=%.1f, N=%i$'%(beta,N)
    if N > 16: 
        #Evolution plot:
        plt.figure(1)
        plt.plot(data, zorder=N, label=label)
        #Histogram:
        plt.figure(2)
        bins = np.arange(data.min(),data.max()+2) - 0.5
        plt.hist(data, bins=bins, normed=True, histtype='step', zorder=-N, label=label)
    #<Q^2>/N^2:
    plt.figure(3)
    #count += 1
    plt.errorbar(1, ratio, yerr=dratio, fmt='o', ms=3, label=label)


#Evolution plot:
plt.figure(1)
plt.ylim(ymax=5)
plt.title('Topological charge evolution at constant physics')
plt.xlabel('Iterations')
plt.ylabel('$Q$')
plt.legend()
plt.savefig('./plots/plotPhysEvo.pdf')
plt.close()
#Histogram:
plt.figure(2)
#plt.xlim([-20,20])
plt.title('PDF of topological charge at constant physics')
plt.xlabel('$Q$')
plt.ylabel('$P(Q)$')
plt.legend()
plt.savefig('./plots/plotPhysHisto.pdf')
plt.close()
#<Q^2>/N^2:
plt.figure(3)
plt.ylim(ymin=-0.003)
plt.title('Topological susceptibility divided by $N^2$ at constant physics')
plt.ylabel(r'$\frac{\left<Q^2\right>}{N^2}$')
plt.xticks([])
plt.legend(numpoints=1)
plt.savefig('./plots/plotPhysRatio.pdf')
plt.close()

