import numpy as np

def f(x): return sum(x**2)/float(x.size)

def jackknifemean(x):
    n = x.size
    idx = np.arange(n)
    return np.sum(f(x[idx!=i]) for i in range(n))/float(n)

def jackknife(x):
    n = x.size
    idx = np.arange(n)
    jMean = jackknifemean(x)
    return (jMean, (n-1)/float(n)*np.sum( (f(x[idx!=i])-jMean)** 2 for i in range(n) ))

