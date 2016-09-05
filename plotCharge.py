import pylab
import os

pylab.rc('text', usetex=True)
pylab.rc('font', **{'family': 'serif', 'serif':['Computer Modern']})

if not os.path.exists('plots'): os.makedirs('plots')

dir = './data/betas/'
for file in os.listdir(dir):
    beta = float(file[10:13])
    n = int(file[14:16])
    data = pylab.loadtxt(dir+file, dtype=int)
    label = '$\\beta=%.1f$'%(beta)
    pylab.plot(data, label=label)

pylab.title('Topological charge evolution with $N=%i$'%(n))
pylab.xlabel('Iterations')
pylab.ylabel('$Q$')
pylab.legend()
pylab.savefig('./plots/plotBetasEvo.pdf')
pylab.clf()

for file in reversed(os.listdir(dir)):
    beta = float(file[10:13])
    n = int(file[14:16])
    data = pylab.loadtxt(dir+file, dtype=int)
    label = '$\\beta=%.1f$'%(beta)
    bins = pylab.arange(data.min(),data.max()+2) - 0.5
    pylab.hist(data, bins=bins, normed=True, label=label)

pylab.xlim([-15,15])
pylab.title('PDF of topological charge with $N=%i$'%(n))
pylab.xlabel('$Q$')
pylab.ylabel('$P(Q)$')
pylab.legend()
pylab.savefig('./plots/plotBetasHisto.pdf')
pylab.clf()

dir = './data/ns/'
for file in reversed(os.listdir(dir)):
    beta = float(file[10:13])
    n = int(file[14:16])
    data = pylab.loadtxt(dir+file, dtype=int)
    label = '$N=%i$'%(n)
    pylab.plot(data, label=label)

pylab.title('Topological charge evolution with $\\beta=%.1f$'%(beta))
pylab.xlabel('Iterations')
pylab.ylabel('$Q$')
pylab.legend()
pylab.savefig('./plots/plotNsEvo.pdf')
pylab.clf()

for file in os.listdir(dir):
    beta = float(file[10:13])
    n = int(file[14:16])
    data = pylab.loadtxt(dir+file, dtype=int)
    label = '$N=%i$'%(n)
    bins = pylab.arange(data.min(),data.max()+2) - 0.5
    pylab.hist(data, bins=bins, normed=True, label=label)

pylab.xlim([-15,15])
pylab.title('PDF of topological charge with $\\beta=%.1f$'%(beta))
pylab.xlabel('$Q$')
pylab.ylabel('$P(Q)$')
pylab.legend()
pylab.savefig('./plots/plotNsHisto.pdf')
pylab.close()

