import time

def curr_ms():
  return time.time() * 1000.0

def elap(msg, start, end):
  print "\n",msg, " elap=", end - start

def simpAvg(nptr, startNdx,  endNdx):
  numEle = (endNdx - startNdx) + 1
  if numEle < 1:
    return 0
  total = 0.0
  ndx = startNdx
  while ndx <= endNdx:
    total += nptr[ndx]
    ndx += 1
  return total / numEle

def sma(nptr,  startNdx,  endNdx,  numPer, tout):
   total = 0.0
   ndx = startNdx
   while ndx <= endNdx:
      tbeg  = max(1, ndx - numPer)
      tout[ndx] =  simpAvg(nptr, tbeg, ndx)
      ndx += 1


startms = curr_ms()
f = open("c:\\calgo-data\\exp-EURUSD-Tick-rsi14-stddev100-bars.csv")
#a = f.readlines()
#endreadms = curr_ms()
#elap("finished read lines", startms, endreadms)

begsplit = curr_ms()
header = f.readline()
tarr = []
for aline in f:
	flds = aline.split(",")
	tarr.append(float(flds[3]))
endsplit = curr_ms()
elap("finished split lines to flds" , begsplit, endsplit)
print "# rows=", len(tarr)
startsma = curr_ms()
startsmaclone = startsma
smaout = list(tarr)
elap("allocate sma memory", startsmaclone, curr_ms())
numRec = len(tarr)
sma(tarr, 0, numRec-1, 1000, smaout)
elap("finished sma", startsma, curr_ms())







