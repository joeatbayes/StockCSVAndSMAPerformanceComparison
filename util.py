import time

def curr_ms():
  return time.time() * 1000.0

def elap(msg, start, end):
  print "\n",msg, " elap=", end - start
