#!/usr/bin/env python2

import os
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import rc
#from scipy.optimize import curve_fit

from jackknife import jackknife

rc('text', usetex=True)
rc('font', **{'family': 'serif', 'serif':['Computer Modern']}) 

if not os.path.exists('plots/moebius/'): os.makedirs('plots/moebius/')

dataDir = 'data/moebius/charge/phys/'
betas = []
ratios = []
dratios = []
for file in os.listdir(dataDir):
    f = open(dataDir+file, 'r')
    for i in range(2):
        exec(f.readline()[1:]) #Read from file the value of beta and N
    f.close()
    data = np.loadtxt(dataDir+file, dtype=int)
    QsqMean, dQsqMean = jackknife(data)
    ratio = QsqMean/N**2
    dratio = dQsqMean/N**2
    ratios.append(ratio)
    dratios.append(dratio)

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
    betas.append(beta)
    plt.errorbar(1./(N*N), ratio, yerr=dratio, fmt='o', ms=3, label=label)


#Evolution plot:
plt.figure(1)
#plt.ylim(ymax=5)
plt.title('Topological charge evolution at constant physics')
plt.xlabel('Iterations')
plt.ylabel('$Q$')
plt.legend()
plt.savefig('./plots/moebius/plotPhysEvo.pdf')
plt.close()
#Histogram:
plt.figure(2)
#plt.xlim([-20,20])
plt.title('PDF of topological charge at constant physics')
plt.xlabel('$Q$')
plt.ylabel('$P(Q)$')
plt.legend()
plt.savefig('./plots/moebius/plotPhysHisto.pdf')
plt.close()
#Fit <Q^2>/N^2 and plot:

#def f(beta, A, k): return A*np.exp(-beta*k)
#popt, pcov = curve_fit(f, betas, ratios, absolute_sigma=False)

plt.figure(3)
#plt.xscale('log')
#plt.yscale('log')

#betapoints = np.linspace(0,8,50)
#plt.plot(betapoints, f(betapoints, popt[0], popt[1]), label=r'$A \cdot e^{-\beta k}$')
plt.title('Topological susceptibility divided by $N^2$ at constant physics')
#plt.text(0.2, 0.03, 'Fit result: $k=%.3f\\pm%.3f$'%(popt[1],np.sqrt(pcov[1][1])), horizontalalignment='left', fontsize='large')
plt.xlabel(r'$\frac{1}{N^2}$')
plt.ylim(ymin=-0.003)
plt.ylabel(r'$\frac{\left<Q^2\right>}{N^2}$')
plt.legend(numpoints=1)
plt.savefig('./plots/moebius/plotPhysRatio.pdf')
plt.close()

