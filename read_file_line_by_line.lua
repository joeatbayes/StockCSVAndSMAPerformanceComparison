local ffi= require("ffi")
local size_double = ffi.sizeof("double")
local size_char    = ffi.sizeof("char")
local size_float    = ffi.sizeof("float")
--ffi.cdef "double* atof (char *ptr);"
ffi.cdef"void* malloc (size_t size);"
ffi.cdef"void free (void* ptr);"
local DArr = ffi.metatype(
        --               size,               array
        "struct{uint32_t size; double* a;}",
        {__gc = function(self) ffi.C.free(self.a) end}
    )

function simpAvg(nptr, startNdx,  endNdx)
  local numEle = (endNdx - startNdx) + 1
  if numEle < 1 then
    return 0
	end
  local total = 0.0
  local ndx = startNdx
  while ndx <= endNdx do
    total = total + nptr[ndx]
    ndx = ndx + 1
	end
  return total / numEle
end

function sma(nptr,  startNdx,  endNdx,  numPer, tout)
   local total = 0.0
   local ndx = startNdx
   while ndx <= endNdx do
      local tbeg  = math.max(1, ndx - numPer)
      tout[ndx] =  simpAvg(nptr, tbeg, ndx)
      ndx = ndx + 1
	  end
end


function split(aStr, sub)
  local out = {}
  local i = 0
  local pat = "[^" .. sub .. "]+"
  for val in string.gmatch(aStr, pat) do
    out[i] = val
    i = i + 1
  end
  return out
end

function countLines(fp)
  local num_rec = 0
	for line in fp:lines() do
		   num_rec = num_rec + 1
	end
	fp:seek("set", 0)
	return num_rec
end

function makeArray(numEle)
		local size_in_bytes = size_double * (numEle  + 2)
    local ptr = ffi.C.malloc(size_in_bytes)
    if ptr == nil then
      return nil
    end
		local dbls = DArr( numEle,  ptr)
    return dbls
end

function dorun(fname)
    local file = assert(io.open(fname, "r"))
		local startcnt = os.clock()
		local numrec = countLines(file)
		print( "Count Lines numRec=" .. numrec .. " in "  .. (os.clock() - startcnt))
		local dbls = makeArray(numrec)
    local arrout  = dbls.a
    local startread = os.clock()
		local recNum = 0
		headers = file:read("*line")
		for line in file:lines() do
		   local i=0
			 tarr = split(line, ",")
			 arrout[recNum] = tonumber(tarr[3])
			recNum = recNum + 1
		end
		file:close()
		print("load array in " .. (os.clock() - startread))
		return dbls
end

beg_time = os.clock()
local trecs = dorun("c:\\calgo-data\\exp-EURUSD-Tick-rsi14-stddev100-bars.csv")
end_time = os.clock()
elap = end_time - beg_time
print("elap sec time=" .. elap .. "numRec=" .. trecs.size )
beg_sma = os.clock()
tsmaout = makeArray(trecs.size)
sma(trecs.a, 1, trecs.size - 5, 1000, tsmaout.a)
elap = os.clock() - beg_sma
print("Elap sma time=" .. elap)
tavg = simpAvg(tsmaout.a, 1,  trecs.size -5)
print ("num rec=" ..  trecs.size .. " avg=" .. tavg)





