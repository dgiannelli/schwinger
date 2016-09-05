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

    pylab.figure(1)
    pylab.plot(data, label=label)

    pylab.figure(2)
    bins = pylab.arange(data.min(),data.max()+2) - 0.5
    pylab.hist(data, bins=bins, normed=True, alpha=0.2, label=label)

pylab.figure(1)
pylab.title('Topological charge evolution with $N=%i$'%(n))
pylab.xlabel('Iterations')
pylab.ylabel('$Q$')
pylab.legend()
pylab.savefig('./plots/plotBetasEvo.pdf');
pylab.close()

pylab.figure(2)
pylab.xlim([-15,15])
pylab.title('PDF of topological charge with $N=%i$'%(n))
pylab.xlabel('$Q$');
pylab.ylabel('$P(Q)$')
pylab.legend()
pylab.savefig('./plots/plotBetasHisto.pdf');
pylab.close()

#pylab.plot(data)
#pylab.savefig('plots/chargeEvolution.pdf')
#pylab.clf()
#
#
#bins = pylab.arange(data.min(),data.max()+2)-.5
#pylab.hist(data, bins=bins)
#pylab.xticks(range(data.min(),data.max()+1))
#pylab.savefig('plots/chargeHistogram.pdf')
#pylab.clf()

