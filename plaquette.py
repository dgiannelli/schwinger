import pylab

data = pylab.loadtxt('plaquette.dat', unpack=True)

pylab.plot(data)
pylab.savefig('plaquette.pdf')
pylab.clf()

pylab.plot(data[-20:-1])
pylab.savefig('plaquetteZoom.pdf')
pylab.clf()
