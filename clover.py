import pylab

data = pylab.loadtxt('clover.dat', unpack=True)

pylab.plot(data)
pylab.savefig('clover.pdf')
pylab.clf()

pylab.plot(data[-20:-1])
pylab.savefig('cloverZoom.pdf')
pylab.clf()
