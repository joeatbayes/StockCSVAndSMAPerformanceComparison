# Doesn't work need to finish so can test
# but it took over 2 minutes to load the CSV
# compared to the simple load which wass only 13 seconds
# would be interesting to see how they new python compilers fare with 
# speeding up the loop.
# http://stackoverflow.com/questions/4315506/load-csv-into-2d-matrix-with-numpy-for-plotting
# http://docs.scipy.org/doc/numpy/user/basics.io.genfromtxt.html
# http://pythonprogramming.net/advanced-matplotlib-graphing-charting-tutorial/
# http://www.lfd.uci.edu/~gohlke/pythonlibs/#scikit-image
# http://sourceforge.net/projects/winpython/?source=typ_redirect
# http://www.jmlr.org/papers/volume12/pedregosa11a/pedregosa11a.pdf

import time
import numpy as np


def elap(msg, start, end):
  print ("\n",msg, " elap=", end - start)

def movingaverage(values,window):
    weigths = np.repeat(1.0, window)/window
    smas = np.convolve(values, weigths, 'valid')
    return smas # as a numpy array


begreadms = time.time()
data = np.genfromtxt("c:\\calgo-data\\exp-EURUSD-Tick-rsi14-stddev100-bars.csv", dtype=None, delimiter=',', names=True)
elap("Finished genFromText", begreadms, time.time())
begma = time.time()
colum3 = data[3]
print("colmn3 len=", len(colum3))
print("colmn3[1:5]=", colum3[1:5])
tout = movingaverage(colum3, 1000)
elap("finished ma=", begma, time.time())
elap("total run=", begreadms, time.time())

#def simpAvg(nptr, startNdx,  endNdx):
#  numEle = (endNdx - startNdx) + 1
#  if numEle < 1:
#    return 0
#  total = 0.0
#  ndx = startNdx
#  while ndx <= endNdx:
#    total += nptr[ndx]
#    ndx += 1
#  return total / numEle
#
#def sma(nptr,  startNdx,  endNdx,  numPer, tout):
#   total = 0.0
#   ndx = startNdx
#   while ndx <= endNdx:
#      tbeg  = max(1, ndx - numPer)
#      tout[ndx] =  simpAvg(nptr, tbeg, ndx)
#      ndx += 1
#
#
#startms = curr_ms()
#f = open("c:\\calgo-data\\exp-EURUSD-Tick-rsi14-stddev100-bars.csv")
##a = f.readlines()
##endreadms = curr_ms()
##elap("finished read lines", startms, endreadms)
#
#begsplit = curr_ms()
#header = f.readline()
#tarr = []
#for aline in f:
#    flds = aline.split(",")
#    tarr.append(float(flds[3]))
#endsplit = curr_ms()
#elap("finished split lines to flds" , begsplit, endsplit)
#print "# rows=", len(tarr)
#startsma = curr_ms()
#startsmaclone = startsma
#smaout = list(tarr)
#elap("allocate sma memory", startsmaclone, curr_ms())
#numRec = len(tarr)
#sma(tarr, 0, numRec-1, 1000, smaout)
#elap("finished sma", startsma, curr_ms())







