from util import *

def average(marr):
  tot = 0.0
  for aFloat in marr:
    tot += aFloat
  return tot / len(marr)



class csv_bar:
  def __init__(self):
    self.vdateTime = []
    self.vday = []
    self.vopen = []
    self.vclose =[]
    self.vhigh = []
    self.vlow = []
    self.vvolume = []

  def load(self,fname):
    f = open(fname)
    header = f.readline()
    vdateTime = self.vdateTime # copy to local varibles because faster
    vday = self.vday
    vopen = self.vopen
    vclose = self.vclose
    vhigh = self.vhigh
    vlow = self.vlow
    vvolume = self.vvolume
    while 1:
      aLine = f.readline()
      if not aLine:
        break;
      if len(aLine) < 5:
        continue
      ra = aLine.split(",")
      vdateTime.append(ra[0])
      vday.append(ra[1])
      vopen.append(float(ra[2]))
      vclose.append(float(ra[3]))
      vhigh.append(float(ra[4]))
      vlow.append(float(ra[5]))
      vvolume.append(int(ra[6]))
    f.close();

def main():
  run_start = curr_ms()
  abars = csv_bar()
  abars.load("c:/JOE/stock/JTDATA/symbols/SPY/2012.M1.csv")
  elap("Total Load time ", run_start, curr_ms())
  print "out contains", len(abars.vopen), " records"

  beg_time = curr_ms()
  tavg = average(abars.vclose)
  elap("Average close elap", beg_time, curr_ms())
  print "close avg=", tavg

if __name__ == "__main__":
  main()
