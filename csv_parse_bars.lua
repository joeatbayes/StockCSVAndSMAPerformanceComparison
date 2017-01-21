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

function trim_quote (s)
  return s:gsub("^\"*", ""):gsub("\"*$","")  
end

function curr_sec()
  return os.clock()
end

function print_arr(arr) 
  local k, v
  for k,v in pairs(arr) do print(" " .. k .. " = " .. v) end
end
  
function parse_file(fiName)
  file = assert(io.open(fiName, "r"))
  local out = {}
  local i = 0;
  local headline = file:read("*l")
  local headarr = split(headline, ",")
  --print(headarr)
  for line in file:lines() do 
    local arr = split(line, ",")
    local dtime = arr[0]
    local day  =  arr[1]
    local open = tonumber(arr[2])
    local close= tonumber(arr[3])
    local high = tonumber(arr[4])
    local low = tonumber(arr[5])
    local vol = tonumber(arr[6])    
    local tobj = {dtime, day, open, close, high, low, vol}
    out[dtime] = tobj
    i = i + 1
  end  
  file:close()
  print("num rec=" .. i)
  return out
end
 
beg_time = curr_sec()
freemem = collectgarbage('count')
print("megabytes in use=" .. freemem/1024)
tout = parse_file("2014.M1.csv")
end_time = curr_sec()
elap = end_time - beg_time
print("elap sec time=" .. elap)
--print("num records=" .. # tout)
freemem = collectgarbage('count')
print("megabytes in use=" .. freemem/1024)