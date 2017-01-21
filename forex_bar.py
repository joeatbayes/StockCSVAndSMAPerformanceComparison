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

class forexBars:
  def __init__(self):
    self.dtime = []
    self.open = []
    self.close = []
    self.high = []
    self.low = []
    self.volume = []


startms = curr_ms()
f = open("2014.M1.csv")
#a = f.readlines()
#endreadms = curr_ms()
#elap("finished read lines", startms, endreadms)

begsplit = curr_ms()
header = f.readline()
tbars = forexBars()

for aline in f:
	flds = aline.split(",")
	tbars.dtime.append(flds[0])
	tbars.open.append(float(flds[1]))
	tbars.close.append(float(flds[2]))
	tbars.high.append(float(flds[3]))
	tbars.low.append(float(flds[4]))
	tbars.volume.append(long(float(flds[5])))
	
endsplit = curr_ms()
numRec = len(tbars.open)
elap("finished split lines to flds" , begsplit, endsplit)
print "# rows=", numRec

startsmaclone = curr_ms()
smaopen = list(tbars.open)
smaclose = list(tbars.open)                                                       
elap("allocate sma memory", startsmaclone, curr_ms())

startsma = curr_ms()
sma(tbars.open, 0, numRec-1, 1000, smaopen)
sma(tbars.close, 0, numRec-1, 1000, smaclose)
elap("finished sma", startsma, curr_ms())







