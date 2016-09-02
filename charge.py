import pylab
import os

if not os.path.exists('plots'):
    os.makedirs('plots')

data = pylab.loadtxt('charge.dat', dtype=int)

pylab.plot(data)
pylab.savefig('plots/chargeEvolution.pdf')
pylab.clf()


bins = pylab.arange(data.min(),data.max()+2)-.5
pylab.hist(data, bins=bins)
pylab.xticks(range(data.min(),data.max()+1))
pylab.savefig('plots/chargeHistogram.pdf')
pylab.clf()

