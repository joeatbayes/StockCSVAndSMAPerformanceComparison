# Parsing Speed of a CSV File with SMA Computation

License:  MIT
Author: Joseph Ellsworth
Version 1.0

When developing a system for machine learning I found that if we measured the speed a language could parse relatively large CSV files and then compute a SMA was a reasonably good indicator of how a language / runtime would perform in our larger project to predict stock price movements.      In my linked-in article   [c++ faster than C# depends on the coding idiom](https://www.linkedin.com/pulse/c-faster-than-depends-coding-idom-joe-ellsworth) Several people requested links to the source code.   This repository 

### Single Performance Factor###

The files here represent source code I wrote to test various approaches in the different languages.  The  single largest difference in peformance was generally delivered by using tricks to minimize the number of small memory allocations but this does not improve the performance under node.js or Python due to the way they manage memory and compute locations for array elements.   

I did not try using the NumPy arrays in python that could emulate the FFI arrays I used in Lua but there is a chance that Python with PyPy could improve in relative performance if this was implemented. 

#### Summary Analysis 

The C code and the Go code really where not much different  with the only real difference that the C code had to manually free of the arrays when no longer used while the GO code does that part automatic.     The interesting thing about the trading / prediction engine is that they don't really seem to generate a lot of garbage we need to automatically clean.  In fact most of the memory is consumed in long lived data structures that should be incrementally updated.    We also want control over what is cleaned up and when it will be cleaned for performance reasons.

I am still surprised how well the GO code performed in comparison to well written C code   This may indicate the GO code which may indicate that the GO code is doing less work but could not see where in a quick audit of the code.  

On the surface I have to say the pypy approach is attractive since it was only 37% the code of the of C or GO Ultimately it seems like I am generally managing our own data struture and memory management anyway for performance reasons such as creating large vectors with enough extra space to allow new bars to be added without reallocating and that in doing so I am fighting the runtime enviornment garbage collectors.  They techniques can improve the dynamic enviornments but if you are really wanting that level of control why fight them.  The main issue is prevalence in C++ of data structures that are fine grained memory allocators which can make things slower than desired or cause undesirable pauses.  As such why not just pay the price and write the code in C where I have the control in the first place.   It used to be that the C compilers where slow enough that build time was a problem but they seem nearly instant even with the optimizer enabled now days.

## Comparative Results

Please note these tests were generated with a much larger file.    You will need to enlarge the supplied CSV file by about 100X to get similar results.  

Lua                -   48.8  sec -  medium  Load / medium compute
Python             -  609.4  sec - Fast Load / slower compute
gcc bulk           -  19.13  sec - Faster  Load / Fast compute
gcc lines          -  21.112 sec - fastest lost /fastest compute
cl                 -  17.60  sec - fastest load / fastest compute
pypy               -  25.34  sec - Fast Load / Fast compute
GO1.4.2            - 21.915  seconds - Fastest / Fast
gcc 64 bit lines   - 13.483s
gcc 64 bit bulk    - 21.873s





## Test Results May-01-2015##

> > Note:  It is recommended that you replace the supplied 2014.M1.CSV with one that is 100 times larger to get accurate measurements of real world conditions. 

#### Lua Jit

time luajit read_file_line_by_line.lua
elap sec time=467.112 numRec=7619217
Elap sma time=2.261

real    7m49.508s
user    0m0.015s
sys     0m0.000s


time python read_file_line_by_line.py
finished split lines to flds  elap= 9396.0

rows= 7619217

allocate sma memory  elap= 43.0
finished sma  elap= 598764.0
real    10m9.420s
user    0m0.000s
sys     0m0.015s

#### Ansi C

time ./test.exe  (ANSI C)
sh: .test.exe: command not found
sh-4.1$ time ./test.exe
CLOCKS_PER_MS=1.000000  CLOCKS_PER_SEC=1000.000000
Read 815254390 bytes from c:/calgo-data/exp-EURUSD-Tick-rsi14-stddev100-bars.csv
found 7619218 lines
real    0m37.954s
user    0m0.000s
sys     0m0.000s

#### LuaJit

sh-4.1$ time luajit read_file_line_by_line.lua
Count Lines numRec=7619218 in 6.949
load array in 27.003
elap sec time=33.952 numRec=7619218
Elap sma time=14.909
num rec=7619218 avg=1.3152492779415

real    0m48.899s
user    0m0.000s
sys     0m0.000s



#### Python PyPy 32Bit


sh-4.1$ time c:/pypy32/pypy read_file_line_by_line.py
finished split lines to flds  elap= 11118.0

rows= 7619217

allocate sma memory  elap= 29.0
finished sma  elap= 10182.0

real    0m21.763s
user    0m0.000s
sys     0m0.000s
sh-4.1$



#### Google Go Lang

sh-4.1$ time go run read_file_line_by_line_go.go

fiName=c:\calgo-data\exp-EURUSD-Tick-rsi14-stddev100-bars.csv
ELAP File Read  =      487.951 ms
ELAP Convert bytes to string  =      305.273 ms
ELAP Split String to rows  =     1969.962 ms
ELAP Allocate Vectors =       20.516 ms
ELAP Make Records  =     4601.621 ms
ELAP total File Load  =     7386.808 ms

 numRec=7619218
Run 100 avg() = 0.00  each=0.000
ELAP CompleteSMA =     7547.167 ms
ELAP Complete Run =    14934.464 ms
average of sma=   1.3152

real    0m15.474s
user    0m0.000s
sys     0m0.015s

## Analysis


NOTE: One of the more surprising findings was that fully optimized gcc  64 bit compiled code slightly out performed microsoft CL 64 bit fully

 optimized compiled code.  I expect the oposite.  I was also quite surprised  at how much GO has improved so it is only 1.073 times slower than the best C code.   I was also plesantly surprised to find python written for pypy to only be 1.508 times slower than the well written C code.

#### Lua and Lua Jit

The lua version quite surprisingly was among the slowest of the set when using internal Lua tables taking over 8 minutes to run the complete set of load and average  once we changed it to use a pre-allocated FFI array it dropped to a total of 48 seconds even with a 6 second penalty to read through the array the first time to count lines.    This sample file is 800 meg with 7,619,218   The fastest C was only 14.426 seconds and the fastest GO was 15.5 seconds.  Even when using FFI the luajit was slower than GO, C and pypy and was only faster than Nim or cpython.

#### NIM

The Nim code was affected by the poor nim garbage collector so tight loops
ended up creating too much garbage. It was amonth the the slowest for loading
the data and no option tired seemed to help.  There were soem things like
analgous to pointers that could make Nim greate for functional proramming but
horrible for high performance code that can not afford to pass by reference.

#### Code Size Comparison

The GO code was	126 lines.   The python 52 lines,  The lua with the FII was 101 lines
and the C was 110 lines.   Of these the Luajit was the largest struggle. to get  working fast but I didn't try with the python sma functions in cPython.  The python without pypy over consumed over  600 seconds almost almost all the time for cpython was of it was in the sma calulations which is unfortunate since the python was among the fastest load time for both cpython and pypy.

#### Python with PyPy compiler 32 bit

On the surface PyPy may appear to the be best solution.  It offers a 50% code reduction compared to C and performed only 1.5 times slower.  In addtion there are pre-built bindings for scikit learn and numpy (not tested with pypy) that provide a lot of code for statsitical functions I would otherwise have to write and the scikit and numpy matrix functions seem to have a strong performance  repuations.  	The main concern with pypy is only available built for 32 bit on windows and I suspect that more complex structures will struggle with lots of oportunity for degradation.	  This will be the 4th language rewrite trying  to get some consiseness and library advantages from the language but in every case we end up bumping heads against the enviornment lmits to the point where we have to consider a re-write.   Pypy could easily reproduce this issue but if it worked it could be a great solution.

#### Using FFI with Lua to support larger files

> > See Article [LuaJIT Access 20 Gig or More of Memory](http://bayesanalytic.com/access-extra-memory-from-lua-jit/)

It would be possible to use the FFI to build a library to do the loading of the CSV files for Lua but the Luajit community seems risky with one guy who is rather caustic as the primary engineer and we have hit enough failure scenarios that it seems more rational to just use the native C and consider Lua again if we want to add scripting but since  we are dealing with memory models large enough be a issue for luajit  every time we turn around the poor GC will be an issue.	 By the time we pay the overhead of C memory managment sometimes and not others it the code volume was only a little smaller and lack of true objects that support queues, lists and others make it more difficult.  In addition lua is inherantly not good at multi-tasking.

> > > Updated: Jan-2017  I chose to implement a lot of code in LuaJit but found a number of edge cases where it was not quite reliable.    About 4 months after I started that work the primary author of LuaJit dropped out  and it was unclear what the long term life would be because some features had diverged quite dramatically from the mainstream lua community.    I started porting it all to ANSII C which went amazingly easy and then had to take a contract and was diverted fro the next couple years.   I recently released a new open source [Quantized Machine learning classifier written in GO](https://bitbucket.org/joexdobs/ml-classifier-gesture-recognition)  based on what I learned after having thought about what I learned from observing the Lua version operating in with real life stock data in near real time.     I probably would have chosen to implement it in Rust but I wanted to surface the engine as a HTTP service and GO has a particularly strong HTTP/HTTPS server built in while RUST has several none of which seem as clean as the GO version.

