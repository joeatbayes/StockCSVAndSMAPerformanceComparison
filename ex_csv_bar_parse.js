var readline = require('readline');
var fs = require('fs');
var util = require('util');

function curr_ms()
{
   return (new Date()).getMilliseconds();
}
var fs = require('fs');
var LastTime = new Date().getTime();

function Elapsed()
{
   var tStamp = new Date().getTime()
   var tElap = tStamp - LastTime;
   LastTime = tStamp;
   return tElap;
}


// Convert the simple array of lines into a array
// of pre-parsed float and int values to allow rapid
// multiple ieteration with indicators that need fast
// access to values as numbers.  
function ParseStockCSV(DataLines)
{
  // inputFormat = DateTime,day, open, close, high, low, volume
    return dataOut;
}

function CalcAverage(DataLines, columnNum)
{
  var vTotal = 0.0;
  for (aLineNum in DataLines)
  {
    vTotal = vTotal + DataLines[aLineNum][columnNum];
  }
  var vAvg = vTotal / DataLines.length; 
  return vAvg;
}

function CalcAverageOpen(DataLines)
{
  return CalcAverage(DataLines, 2);
}

function calcAverageX(tarr)
{
    var vtotal = 0.0;
    var tlen =  tarr.length;
    for (var ndx = 0; ndx < tlen; ndx++) 
    {      
      vtotal += tarr[ndx];
    }
    var tavg = vtotal / tlen;
    return tavg
}  


// Callback for data once available
function ParseDataPreAlloc(err,data)
{  
  var CloseColumn = 3;  
  var ElapRead = Elapsed();
  var DataLines = data.split("\n");     
  var HeaderLine = DataLines.shift();  
  var ElapSplit = Elapsed();
  var numRows = DataLines.Length;
  
  var dateTime = new Array(numRows);
  var day = new Array(numRows);
  var open = new Array(numRows);
  var close = new Array(numRows);
  var high =  new Array(numRows);
  var low  =  new Array(numRows);
  var volume = new Array(numRows);
  var ElapPreAllocArray  = Elapsed();
  for (var lineNum in DataLines)
  {
    var aLine = DataLines[lineNum].trim()
    if (aLine.length < 3) continue;
    
      var FldArr = aLine.split(",");
      dateTime[lineNum] = FldArr[0];
      day[lineNum] = FldArr[1];
      open[lineNum] = parseFloat(FldArr[2]);
      close[lineNum] = parseFloat(FldArr[3]);
      high[lineNum] = parseFloat(FldArr[4]);
      low[lineNum] = parseFloat(FldArr[5]);
      volume[lineNum] = parseInt(FldArr[6]);    
  }
      
  var ElapBuildOut = Elapsed();
  var tAvg = calcAverageX(close);
  var ElapCalcAvg = Elapsed();
  var ElapTot = ElapRead + ElapSplit + ElapBuildOut; // average explicitly ignored
  
  // Generate Timing    
  console.log("number of lines=" + DataLines.length);
  console.log("Elapsed Read =" + ElapRead);
  console.log("Elapsed Split=" + ElapSplit);
  console.log("pre-alloc array=" + ElapPreAllocArray);
  console.log("BuildParsedArray=" + ElapBuildOut);
  console.log("calcAvgClose=" + tAvg);
  console.log("total Run= " + ElapTot);
  
  return [dateTime, day, open, close, high, low, volume];
}



// Callback for data once available
function ParseData(err,data)
{  
  
  var ElapRead = Elapsed();
  var DataLines = data.split("\n");     
  var HeaderLine = DataLines.shift();  
  var ElapSplit = Elapsed();
    
  var StockData = [];
  for (var lineNum in DataLines)
  {
    var aLine = DataLines[lineNum].trim()
    if (aLine.length > 3)
    {
      FldArr = aLine.split(",");	 
      tEle = [FldArr[0], FldArr[1], parseFloat(FldArr[2]), parseFloat(FldArr[3]), parseFloat(FldArr[4]), parseFloat(FldArr[5]), parseInt(FldArr[6])];
      StockData.push(tEle);
    }
  }
  
  var ElapBuildOut = Elapsed();
  var tAvg = CalcAverageOpen(StockData);
  var ElapCalcAvg = Elapsed();
  var ElapTot = ElapRead + ElapSplit + ElapBuildOut; // average explicitly ignored
  
  // Generate Timing  
  console.log("number of lines=" + DataLines.length);
  console.log("Elapsed Read =" + ElapRead);
  console.log("Elapsed Split=" + ElapSplit);
  console.log("BuildParsedArray=" + ElapBuildOut);
  console.log("calcAvgOpen=" + ElapCalcAvg);
  console.log("total Run= " + ElapTot);
  console.log("calculated average open=" + tAvg);
  
  return StockData;
}

start = Elapsed();  // start the clock after all the libraries are loaded and parsed

fs.readFile('2014.M1.csv', "ascii", ParseData);

start = Elapsed();  // start the clock after all the libraries are loaded and parsed
//fs.readFile('c:/JOE/stock/JTDATA/symbols/SPY/2012.M1.csv', "ascii", ParseDataPreAlloc);
