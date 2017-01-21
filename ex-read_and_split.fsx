// read a list of Media id and corbis ID from CSV file 
// and generate output file containing a generated
// set of URI in format to send to freedom oss tool
#light
open System

type BarRec = string * string * float * float * float * float * int   

let nowms() : double = 
  double(System.DateTime.Now.Ticks)

let elap (msg : string,  start : double) = 
  let elap = (nowms() - start) / double(10000.0)
  System.Console.WriteLine( msg + " elap=" + string(elap)) |> ignore

let elapx (msg : string,  start : double, stop : double) = 
  let elap = (stop - start) / double(10000.0)
  System.Console.WriteLine( msg + " elap=" + string(elap)) |> ignore

let cavg (arr : BarRec[]) =
  let mutable tot = 0.0
  for date, day, topen, tclose, thigh, tlow, tvol in arr do 
    tot <- tot + tclose
  tot / float(arr.Length)

let makeCloseArr(arr : BarRec[]) = [|
    for date, day, topen, tclose, thigh, tlow, tvol in arr do
      yield tclose |]

let mutable lc = 0;
let load_barsX2(fiName : string) = 
  let tsBegin = nowms()
  let iterator = System.IO.File.ReadLines(fiName, System.Text.Encoding.ASCII)
  //let header = iterator.MoveNext();
  let tsLoad = nowms()
  lc <- 0
  let tout = [|  
    for line in iterator do
      lc <- lc + 1
      if lc > 1 then
        let tarr = line.Split(',')
        yield (tarr.[0], tarr.[1], float(tarr.[2]), float(tarr.[3]), float(tarr.[4]), float(tarr.[5]), int(tarr.[6])) |]


  elap("Parse and Convert", tsBegin);
  System.Console.WriteLine ( "Number of Lines= " + string(tout.Length)) 
  let tsBuilt = nowms()
  let closeArr = makeCloseArr(tout)
  elap("Make Close Arrary", tsBuilt);
  let begAvg = nowms()
  let closeAvg = Array.average  closeArr
  elap("Calc Average = " + string(closeAvg) + " from Array vector", begAvg)
  let avgb1 = nowms()
  let avg1 = cavg(tout)
  elap("Calc average from struct", avgb1)

let countLines(fiName : string) =  
  let tsbegi2 = nowms()
  let iterator2 = System.IO.File.ReadLines(fiName, System.Text.Encoding.ASCII)
  let tsLoad = nowms()
  lc <- 0
  for line in iterator2 do
    lc <- lc + 1
  elap ("count " + string(lc) + " lines", tsbegi2);
  lc

let countLinesBytes(fiName : string) =
  let tsBeginb = nowms()
  let bytes = System.IO.File.ReadAllBytes("c:/JOE/stock/JTDATA/symbols/SPY/2012.M1.csv")
  let tsLoadb = nowms()
  lc <- 0
  for bb in bytes do
    if bb = (byte) '\n' then lc <- lc + 1
  let tsEndb = nowms()
  elap("parse bytes to count " + string(lc) + " lines", tsLoadb) 
  elapx( "read file as bytes =", tsBeginb, tsLoadb)
  elapx( "Count Lines Bytes All =", tsBeginb, tsEndb)
  lc


//let mutable lc = 0;
let load_bars_vectors(fiName : string) = 
  let tsBegin = nowms()
  let numRec  = countLinesBytes(fiName)
  let tdouble = -1.0
  let tlong = -1
  let vdtime = Array.create numRec ""
  let vday = Array.create numRec ""
  let vopen = Array.create numRec tdouble
  let vclose = Array.create numRec tdouble
  let vhigh = Array.create numRec tdouble
  let vlow = Array.create numRec tdouble
  let vvol = Array.create numRec tlong

  let iterator = System.IO.File.ReadLines(fiName, System.Text.Encoding.ASCII)
  //let header = iterator.MoveNext();
  let tsLoad = nowms()
  lc <- -2
  for line in iterator do
    lc <- lc + 1
    //System.Console.Write(" " + string(lc))      
    if lc > -1 then
      if line.Length > 0 then
        //System.Console.WriteLine("line=" + line);
        let tarr = line.Split(',')
        vdtime.[lc] <- tarr.[0]
        vday.[lc] <- tarr.[1]
        vopen.[lc] <- float(tarr.[2])
        vclose.[lc] <- float(tarr.[3])
        vhigh.[lc] <- float(tarr.[4])
        vlow.[lc] <- float(tarr.[5])
        vvol.[lc] <- int(tarr.[6])
      

  elap("Parse and Convert", tsBegin);
  System.Console.WriteLine ( "Number of Lines= " + string(numRec)) 
  let tsBuilt = nowms()
  let begAvg = nowms()
  let closeAvg = Array.average vclose
  elap("Calc Average = " + string(closeAvg) + " from Array vector", begAvg)
  

  let begmavg = nowms()
  let a1 = [
    for xx = 1 to 1000 do
      let xavg = Array.average vclose
      yield xavg ]    
  let endmavg = nowms()
  let mavgelap = (((endmavg - begmavg) / 10000.0) / 1000.0)
  System.Console.WriteLine("Average of 1,000 avg runs=" + string(mavgelap))

  (vdtime, vday, vopen, vclose, vhigh, vlow, vvol)

let x = 2
let fiName = "c:/JOE/stock/JTDATA/symbols/SPY/2012.M1.csv"
if x = 1 then 
  let tout = load_barsX2(fiName)
  let lines = countLines(fiName)
  let linesx = countLinesBytes(fiName)
  let startmem = nowms()
  let tdouble = double(-1.0)
  let tlong = 0
  let vdtime = Array.create linesx ""
  let vday = Array.create linesx ""
  let vopen = Array.create linesx tdouble
  let vclose = Array.create linesx tdouble
  let vhigh = Array.create linesx tdouble
  let vlow = Array.create linesx tdouble
  let vvol = Array.create linesx tlong
  elap("alloc vectors", startmem)

if x = 2 then
  let aRes = load_bars_vectors(fiName)
  System.Console.WriteLine("done")

