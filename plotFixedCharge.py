#!/usr/bin/env python2

import os
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import rc
from scipy.optimize import curve_fit

from jackknife import Jackknife

rc('text', usetex=True)
rc('font', **{'family': 'serif', 'serif':['Computer Modern']}) 

if not os.path.exists('plots'): os.makedirs('plots')

dataDir = 'data/torus/charge/fixed'
Ns = []
QsqMeans = []
dQsqMeand = []
for file in os.listdir(dataDir):
    f = open(file, 'r')
    for i in range(2):
        exec(f.readline()[1:]) #Read from file the value of beta and N
    f.close()
    data = np.loadtxt(file, dtype=int)
    QsqMean, dQsqMean = Jackknife(data)
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
    plt.hist(data, bins=bins, normed=True, zorder=N, label=label)

#Evolution plot:
plt.figure(1)
plt.title('Topological charge evolution with $\\beta=%.1f$'%(beta))
plt.xlabel('Iterations')
plt.ylabel('$Q$')
plt.legend()
plt.savefig('./plots/plotFixedEvo.pdf')
plt.close()
#Histogram:
plt.figure(2)
plt.title('PDF of topological charge with $\\beta=%.1f$'%(beta))
plt.xlabel('$Q$')
plt.ylabel('$P(Q)$')
plt.legend()
plt.savefig('./plots/plotFixedHisto.pdf')
plt.close()

#Fit and plot of: <Q^2> = x*log(N) + A:

def f(logN, x, A): return x*logN + A

logNs = np.log(np.asarray(Ns))
popt, pcov = curve_fit(f, logNs, QsqMeans, sigma=dQsqMeans, absolute_sigma=True)

plt.errorbar(logNs, QsqMeans, yerr=dQsqMeans, 'o')
plt.plot(logNs, f(logNs), label=r'$\left<Q^2\right>=A+x\log N$')
plt.title('$\\left<Q^2\\right> at fixed $\\beta=%.1f: x=%.2f\pm%.2f'%(beta,popt[1],np.sqrt(pcov[0][0])))
plt.xlabel(r'$\log N$')
plt.ylabel(r'$\left<Q^2\right>$')
plt.legend()
plt.savefig('.plots/plotFixedSucep.pdf')
plt.close()
    
