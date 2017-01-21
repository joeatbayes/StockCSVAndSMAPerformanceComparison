package main

import "fmt"
import "io/ioutil"
import "strings"
import "time"
import "strconv"
//import "math"

func Nowms() float64 {
	nn := time.Now()
	return float64(nn.UnixNano()) / float64(time.Millisecond)
}
func Elap(msg string, beg_time float64, end_time float64) float64 {
	elap := end_time - beg_time
	fmt.Printf("ELAP %s = %12.3f ms\n", msg, elap)
	return elap
}

const BadFloatParse = -0.00005
const BadIntParse = -1

type Floats []float64
type Ints []int
type Strings []string

func maxi(num1 int32,  num2 int32) int32 {
  if num1 > num2 {
   return num1
  }
  return num2
}

func parseFloat(aStr string) float64 {
	amount, err := strconv.ParseFloat(strings.Trim(aStr, " "), 10)
	if err != nil {
		fmt.Println("error parseFloat in=", aStr)
		panic(err)
		return BadFloatParse
	}
	return amount
}
func parseInt(aStr string) int {
	amount, err := strconv.ParseInt(strings.Trim(aStr, " "), 10, 32)
	if err != nil {
		fmt.Println("error parseInt in=", aStr)
		panic(err)
		return BadIntParse
	}
	return int(amount)
}
func Avg(s Floats) float64 {
	tot := float64(0.0)
	for _, val := range s {
		tot += val
	}
	return tot / float64(len(s))
}

func SimpAvg(s Floats, begNdx int32, endNdx int32) float64 {
	tot := float64(0.0)
	numEle := endNdx - begNdx
	if numEle < 1 {
	  return 0
	}
	for ndx :=begNdx; ndx<=endNdx; ndx++ {
	   tot += s[ndx];	
	}
	return tot / float64(numEle)
}

func smaAvg(s Floats, begNdx int32, endNdx int32, numEle int32, tout Floats) {	
	for ndx := begNdx; ndx<=endNdx; ndx++ {
	   tBeg := maxi(1, ndx - numEle)
	   tout[ndx] = SimpAvg(s, tBeg, ndx)
	}
}

func LoadBarsCSVFile(fname string) {
	fmt.Printf("\nfiName=%s\n", fname)
	begRun:= Nowms()
	start := Nowms()
	tload := start
	tBytes, err := ioutil.ReadFile(fname)
	if err != nil {
		fmt.Printf("Error in ReadFile=%s\n", fname)
		panic(err)
	}
	Elap("File Read ", start, Nowms())
	start = Nowms()
	full_str := string(tBytes)
	Elap("Convert bytes to string ", start, Nowms())
	start = Nowms()
	full_arr := strings.Split(full_str, "\n")
	Elap("Split String to rows ", start, Nowms())
	full_str = ""                // free up the memory
	num_rec := len(full_arr) - 1 // -1 because deduct one for header
	start = Nowms()
	vopen := make(Floats, num_rec)
	Elap("Allocate Vectors", start, Nowms())
	start = Nowms()
	for line_cnt := 0; line_cnt < (num_rec - 1); line_cnt++ {
		rowStr := strings.TrimRight(full_arr[line_cnt+1], " \t\n\r")
		if len(rowStr) < 5 {
			fmt.Println("skipped empyt row")
			continue
		}
		rowa := strings.SplitN(rowStr, ",", 8)
		vopen[line_cnt] = parseFloat(rowa[3])
	}
	Elap("Make Records ", start, Nowms())
	Elap("total File Load ", tload, Nowms())
	fmt.Printf("\n numRec=%d\n", num_rec)	
	startsma := Nowms()
	smatmp := make(Floats, num_rec)
	smaAvg(vopen, 1, int32(num_rec - 1), 1000, smatmp)
	Elap("CompleteSMA", startsma, Nowms())
	Elap("Complete Run", begRun, Nowms())
	tAvg := Avg(vopen)
	fmt.Printf("average of sma=%9.4f\n", tAvg)
}

func main() {
	LoadBarsCSVFile("c:\\calgo-data\\exp-EURUSD-Tick-rsi14-stddev100-bars.csv")
}
