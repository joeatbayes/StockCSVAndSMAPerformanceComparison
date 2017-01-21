import os.path
import os
import time
import sys
def curr_ms():
  return time.time() * 1000.0
def elap(msg, start, end):
  print "\n",msg, " elap=", end - start
def average(marr):
  tot = 0.0
  for aFloat in marr:
    tot += aFloat
  return tot / len(marr)
def do_main_test(fi_name):
  beg_time = curr_ms()
  run_start = beg_time
  f = open(fi_name)
  marr = f.readlines()  
  elap("time to read pre-split ", beg_time, curr_ms())  
  beg_time = curr_ms()
  beg_time = curr_ms() 
  header = marr[0]
  marr = marr[1:]
  elap("Slice Marr to just data ", beg_time, curr_ms())
  beg_time = curr_ms() 
  line_cnt = 0
  vdateTime = []
  vday = []
  vopen = []
  vclose =[]
  vhigh = []
  vlow = []
  vvolume = []
  for aLine in marr:
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
  print "out contains", len(vopen), " records"
  elap("ieterate array and split ", beg_time, curr_ms())
  elap("Total Run time ", run_start, curr_ms())
  beg_time = curr_ms() 
  tavg = average(vclose)
  elap("Average close elap", beg_time, curr_ms())
  print "close avg=", tavg
do_main_test("c:/JOE/stock/JTDATA/symbols/SPY/2012.M1.csv")