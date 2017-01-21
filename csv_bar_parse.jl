# Julia function to load CSV and compute
# a couple of SMA.  written to be identical
# to lua version.
using DataFrames


# Note: There are faster ways to compute SMA but this
#  is closest to the method shown in basic tutorials
#  and is a valid test of a tight loop that spends a
#  a lot of time indexing into an array.   We show it
#  as a nested loop instead of slice
function sma(avect, numPer)
    print(length(avect), " numper=", numPer)
    numEle = length(avect)
    tout = Array(Float32, numEle)
    for ndx = 1:numEle
      tsum = 0.0
      begndx = max(1, ndx - numPer)
      for slicendx = begndx:ndx
        tsum += avect[slicendx]
      end
      tout[ndx] = tsum / float32(numPer)
    end
    return tout
end

## Showing the more common Julia version of
## slicing out what we need and applying builtin
## operator mean instead of manual sub loop.
function sma_slice(avect, numPer)
    print(length(avect), " numper=", numPer)
    numEle = length(avect)
    tout = Array(Float32, numEle)
    for ndx = 1:numEle
      begndx = max(1, ndx - numPer)
      tout[ndx] = mean(avect[begndx:ndx])
    end
    return tout
end



function runTest()
   print ("read table")
   tic()
   dta = readtable("2014.M1.csv")
   toc()
   println("finished read table")

   closeVect = dta[:close]
   println("compute sma(14)")
   tic()
   sma14 = sma(closeVect, 14)
   toc()
   println("finished sma(14)")

   println("compute sma (600)")
   tic()
   sma600 = sma(closeVect, 600)
   toc()
   println("finished sma(600)")


   closeVect = dta[:close]
   println("compute sma_slice(14)")
   tic()
   sma14 = sma_slice(closeVect, 14)
   toc()
   println("finished sma_slice(14)")

   println("compute sma_slice(600)")
   tic()
   sma600 = sma_slice(closeVect, 600)
   toc()
   println("finished sma_slice(600)")


   println("\n\n Convert to Typed vector and try again \n\n")
   tic()
   tlen = length(closeVect)
   tvect = Array(Float32, tlen)
   for ndx = 1:tlen
     tvect[ndx] = closeVect[ndx]
   end
   toc()
   println("vector convertion complete")

   println("compute sma(14)")
   tic()
   sma14 = sma(tvect, 14)
   toc()
   println("finished sma(14)")

   println("compute sma (600)")
   tic()
   sma600 = sma(tvect, 600)
   toc()
   println("finished sma(600)")


   closeVect = dta[:close]
   println("compute sma_slice(14)")
   tic()
   sma14 = sma_slice(tvect, 14)
   toc()
   println("finished sma_slice(14)")

   println("compute sma_slice(600)")
   tic()
   sma600 = sma_slice(tvect, 600)
   toc()
   println("finished sma_slice(600)")



end

runTest()

