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
  var dataOut = [];
  for (var lineNum in DataLines)
  {
     var aLine = DataLines[lineNum].trim()
     if (aLine.length > 3)
     {
       FldArr = aLine.split(",");	 
       tEle = [FldArr[0], FldArr[1], parseFloat(FldArr[2]), parseFloat(FldArr[3]), parseFloat(FldArr[4]), parseFloat(FldArr[5]), parseInt(FldArr[6])];
       dataOut.push(tEle);
    }
  }
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
// Callback for data once available
function ParseData(err,data)
{  
  var DataLines = data.split("\n");    
  var HeaderLine = DataLines.shift();  
  var ElapReadAndSplit = Elapsed();   
  var StockData  = ParseStockCSV(DataLines); 
  var ElapBuildOut = Elapsed();
  var tAvg = CalcAverageOpen(StockData);
  var ElapCalcAvg = Elapsed();
  var ElapTot = ElapReadAndSplit + ElapBuildOut + ElapCalcAvg;
  // Generate Timing  
  console.log("number of lines=" + DataLines.length);
  console.log("Elapsed Read and Split=" + ElapReadAndSplit);
  console.log("BuildParsedArray=" + ElapBuildOut);
  console.log("calcAvgOpen=" + ElapCalcAvg);
  console.log("total Run= " + ElapTot);
  console.log("calculated average open=" + tAvg);
  return StockData;
}
fs.readFile('2014.M1.csv', "ascii", ParseData);