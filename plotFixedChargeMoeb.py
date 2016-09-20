#!/usr/bin/env python2

import os
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import rc
from scipy.optimize import curve_fit

from jackknife import jackknife

rc('text', usetex=True)
rc('font', **{'family': 'serif', 'serif':['Computer Modern']}) 

if not os.path.exists('plots/moebius/'): os.makedirs('plots/moebius/')

dataDir = 'data/moebius/charge/fixed/'
Ns = []
QsqMeans = []
dQsqMeans = []
for file in os.listdir(dataDir):
    f = open(dataDir+file, 'r')
    for i in range(2):
        exec(f.readline()[1:]) #Read from file the value of beta and N
    f.close()
    data = np.loadtxt(dataDir+file, dtype=int)
    QsqMean, dQsqMean = jackknife(data)
    Ns.append(N)
    QsqMeans.append(QsqMean)
    dQsqMeans.append(dQsqMean)
    label='$N=%i$'%(N)

    #Evolution plot:
    plt.figure(1)
    plt.plot(data, zorder=-N, label=label)
    #Histogram:
    plt.figure(2)
    bins = np.arange(data.min(),data.max()+2) - 0.5
    plt.hist(data, bins=bins, normed=True, histtype='stepfilled', alpha=0.4, zorder=N, label=label)

#Evolution plot:
plt.figure(1)
plt.title('Topological charge evolution at $\\beta=%.0f$'%(beta))
plt.xlabel('Iterations')
plt.ylabel('$Q$')
plt.legend()
plt.savefig('./plots/moebius/plotFixedEvo.pdf')
plt.close()
#Histogram:
plt.figure(2)
#plt.xlim([-20,20])
plt.title('PDF of topological charge at $\\beta=%.0f$'%(beta))
plt.xlabel('$Q$')
plt.ylabel('$P(Q)$')
plt.legend()
plt.savefig('./plots/moebius/plotFixedHisto.pdf')
plt.close()

#Fit and plot of: <Q^2> = x*log(N) + A:

def f(N, A, x): return A*N**x
popt, pcov = curve_fit(f, Ns, QsqMeans, sigma=dQsqMeans, absolute_sigma=True)

plt.xscale('log')
plt.yscale('log')
plt.errorbar(Ns, QsqMeans, yerr=dQsqMeans, fmt='o', ms=3)
NPoints = np.linspace(1,50)
plt.plot(NPoints, popt[0]*NPoints**popt[1], label=r'$\left<Q^2\right> = A \cdot N^x$')
plt.title('Topological susceptibility vs $N$ at fixed $\\beta=%.0f$'%(beta))
plt.text(1.1,20, 'Fit result: $x=%.3f\\pm%.3f$'%(popt[1],np.sqrt(pcov[1][1])), horizontalalignment='left', fontsize='large')
plt.xlabel(r'$N$')
plt.ylabel(r'$\left<Q^2\right>$')
plt.legend(loc=2)
#plt.tight_layout()
plt.savefig('./plots/moebius/plotFixedSucep.pdf')
plt.close()

print 'Fit result: $x=%.6f\\pm%.6f$'%(popt[1],np.sqrt(pcov[1][1]))

