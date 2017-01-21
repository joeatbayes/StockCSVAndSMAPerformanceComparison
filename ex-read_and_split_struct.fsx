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
  let avgb1 = nowms()
  let avg1 = cavg(tout)
  elap("Calc average from struct", avgb1)
let fiName = "c:/JOE/stock/JTDATA/symbols/SPY/2012.M1.csv"
let tout = load_barsX2(fiName)
