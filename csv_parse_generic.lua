-- Provides a CSV parse similar to the generic table parse
-- in R and julia where each column winds up as a vector
-- of columns so we can easily apply statistical functions
-- to the columns.   
require "os"

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

function trim (s)
  return (string.gsub(s, "^%s*(.-)%s*$", "%1"))
end


function curr_sec()
  return os.clock()
end

function print_arr(arr) 
  local k, v
  for k,v in pairs(arr) do print(" " .. k .. " = " .. v) end
end

-- NOTE: To Be valid as a comparison to DataFrame parser must create
--   an array of columns that can be accessed by Name and
--   has been parsed into a numeric type when it is numeric.
--   colmns must be accessible as single vectors to pass into
--   stats functions
--
-- Reads a CSV File as set of columns.  If the column
-- can be treated as numeric then all values will be parsed
-- as numeric.   Columns can be indexed as out["names"]["close"]
-- where close is the column names or accessed as out["cols"][3]
-- to obtain the column by position in the file. 
function read_csv(fiName)
  local file = assert(io.open(fiName, "r"))
  local tout = {}
  local headline = file:read("*l")
  local headarr = split(headline, ",")
  
  -- Build structure to contain our output
  -- columns.   
  local cols = {}
  local names = {}
  tout["names"] = names
  tout["columns"] = cols
  for fldNum,fldName in pairs(headarr) do    
    fldName = trim(fldName)
    --print(" " .. k .. " = " .. tv)
    avector = {}
    -- since objects are by reference both the
    -- the cols and names return pointer to same
    -- array.
    cols[fldNum] = avector
    names[fldName] = avector
  end
  
  local i = 0;  
  for line in file:lines() do 
    local arr = split(line, ",")
    for fldNum,fldName in pairs(headarr) do 
      acol = cols[fldNum]
      fldval = arr[fldNum]
      nval = tonumber(fldval)
      if nval == nil then
        acol[i] = fldval
      else
        acol[i] = nval
      end
    end -- for each field
    i = i + 1
  end  -- for each line
  file:close()
  print("num rec=" .. i)
  return tout
end -- func()

-- Note: There are faster ways to compute SMA but this
--  is closest to the method shown in basic tutorials
--  and is a valid test of a tight loop that spends a 
--  a lot of time indexing into an array.  
function sma(arr, numPer)
  local tout = {}
  adjdiv = numPer + 1
  for ndx,tval in pairs(arr) do 
    local begndx = math.max(0, ndx - numPer)
    local tsum = 0.0
    for slicendx = begndx, ndx, 1 do
      tsum = tsum +  arr[slicendx]
    end
    if ndx > adjdiv then
      tout[ndx] = tsum / adjdiv
    else
     tout[ndx] = tsum / ndx
    end
  end  
  return tout
end


 
beg_time = curr_sec()
freemem = collectgarbage('count')
print("megabytes in use=" .. freemem/1024)
dtbl = read_csv("2014.M1.csv")
end_time = curr_sec()
elap = end_time - beg_time
print("elap sec time=" .. elap)
--print("num records=" .. # tout)
freemem = collectgarbage('count')
print("megabytes in use=" .. freemem/1024)

print("Column Names")
names = dtbl["names"]
print(names)
for k,v in pairs(names) do 
  print(" " .. k )
end

beg_time = curr_sec()
closeVect = names["close"]
sma14 = sma(closeVect, 14)
end_time = curr_sec()
elap = end_time - beg_time
freemem = collectgarbage('count')
print("sma(14) elap=" .. elap .. " num sma=" .. # sma14 .. " megabytes in use=" .. freemem/1024)


beg_time = curr_sec()
sma600 = sma(closeVect, 600)
end_time = curr_sec()
elap = end_time - beg_time
freemem = collectgarbage('count')
print("sma(600) elap=" .. elap .. " num sma=" .. # sma600 .. " megabytes in use=" .. freemem/1024)
