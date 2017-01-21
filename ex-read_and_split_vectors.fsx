#light
open System
let nowms() : double = 
  double(System.DateTime.Now.Ticks)
let elap (msg : string,  start : double) = 
  let elap = (nowms() - start) / double(10000.0)
  System.Console.WriteLine( msg + " elap=" + string(elap)) |> ignore
let elapx (msg : string,  start : double, stop : double) = 
  let elap = (stop - start) / double(10000.0)
  System.Console.WriteLine( msg + " elap=" + string(elap)) |> ignore
let mutable lc = 0;
let countLinesBytes(fiName : string) =
  let bytes = System.IO.File.ReadAllBytes(fiName)
  lc <- 0
  for bb in bytes do
    if bb = (byte) '\n' then lc <- lc + 1
  lc
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
  (vdtime, vday, vopen, vclose, vhigh, vlow, vvol)
let fiName = "2014.M1.csv"
let aRes = load_bars_vectors(fiName)
