import times
import strutils

var fname = "c:\\calgo-data\\exp-EURUSD-Tick-rsi14-stddev100-bars.csv"
var i = open(fname)

var startLoad = cpuTime()
var tcol = newSeq[string](1)
for line in i.lines:
   tcol.add(line)
i.close()
var elap = cpuTime() - startLoad
echo "completed read file line by line"
echo elap

startLoad = cpuTime()
for line in lines(fname):
  var flds = split(line, ',')
  var afld = flds[3]

elap = cpuTime() - startLoad
echo "complete entire file read using lines split each line on comma"
echo  elap


startLoad = cpuTime()
var allbytes = readFile(fname)
elap = cpuTime() - startLoad
echo "complete entire file entire file as bytes "
echo  elap
var startsplit = cpuTime()
var startline = 0
var endline = 0
# TODO:  Add function to parse out the
# the lines by detecting \n and then split
# each of those lines into an array of fields
var numLine = 0
var lineBeg = 0
var lineEnd = 0
var currNdx = 0
var tnums = newSeq[float](1)
for tc in allbytes:
  if tc ==  '\L':
    lineEnd = currNdx
		if  numLine > 0:
      var aline  = allbytes[lineBeg .. lineEnd]
      var fields = aline.split(',')
      var fldval = fields[3]
      try:
        var anum = parseFloat(fldval)
        tnums.add(anum)
      except ValueError:
        echo "value error parsing float"
        echo aline
        echo fldval
    numLine += 1
    lineBeg = lineEnd + 1
  currNdx += 1

elap = cpuTime() - startsplit
echo "finished split preloaded block"
echo elap