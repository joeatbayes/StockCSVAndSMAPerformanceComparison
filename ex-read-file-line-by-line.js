var readline = require('readline');
var fs = require('fs');
var util = require('util');

function memory_usage() {
  pm = process.memoryUsage();
  return pm["heapTotal"] + pm["heapUsed"];
}

function print_memory_usage() 
{
  console.log("memory usage=" + (memory_usage() / 1000000.0) + " Meg");
}

function curr_ms()
{
   return (new Date()).getMilliseconds();
}

var beg_time = curr_ms();
print_memory_usage();

var file_name = "2014.M1.csv";
var flags = {flags: 'r',  encoding: 'utf-8',  fd: null,  mode: 0666, bufferSize: 64 * 1024 }
var tstream = fs.createReadStream(file_name, flags);
var head_arr = null;
var tout = {}
 
function on_line(line)
{
   row_obj = {};
   //console.log('Line: ' + line);
   var tarr = line.split("\t");
   if (head_arr === null) 
     head_arr = tarr
   for (var ndx in head_arr)
   {
     var fld_name = head_arr[ndx];     
     var fld_val = tarr[ndx].replace(/\s+/g, " ").replace(/\"/g, "").trim()
     if (fld_val.length > 0)
       row_obj[fld_name] = fld_val;
   }
   var pid = row_obj["PHOTO_NUMBER"];
   tout[pid] = row_obj;
}

function on_file_done(parms)
{
  console.log("file done");
  //console.log(tout);
  var end_time = curr_ms();
  var elap = end_time - beg_time;
  console.log(" elap ms=" + elap + " seconds=" +  (elap / 1000.0));
  print_memory_usage();
  console.log("num rec=" + Object.keys(tout).length);
}

require('readline').createInterface({input: tstream,  terminal: false}).on('line', on_line).on('close', on_file_done);
