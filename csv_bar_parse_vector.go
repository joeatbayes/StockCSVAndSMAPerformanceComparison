package main

import "fmt"
import "io/ioutil"
import "strings"
import "time"
import "strconv"

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
func LoadBarsCSVFile(fname string) {
	fmt.Printf("\nfiName=%s\n", fname)
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
	Elap("Convert string to bytes ", start, Nowms())
	start = Nowms()
	full_arr := strings.Split(full_str, "\n")
	Elap("Split String to rows ", start, Nowms())
	full_str = ""                // free up the memory
	num_rec := len(full_arr) - 1 // -1 because deduct one for header
	//fmt.Printf("num_rec=%d\n", num_rec)
	start = Nowms()
	vtimes := make(Strings, num_rec)
	vday := make(Strings, num_rec)
	vopen := make(Floats, num_rec)
	vclose := make(Floats, num_rec)
	vhigh := make(Floats, num_rec)
	vlow := make(Floats, num_rec)
	vvolume := make(Ints, num_rec)
	Elap("Allocate Vectors", start, Nowms())
	start = Nowms()
	for line_cnt := 0; line_cnt < (num_rec - 1); line_cnt++ {
		rowStr := strings.TrimRight(full_arr[line_cnt+1], " \t\n\r")
		if len(rowStr) < 5 {
			fmt.Println("skipped empyt row")
			continue
		}
		rowa := strings.SplitN(rowStr, ",", 9)
		vtimes[line_cnt] = rowa[0]
		vday[line_cnt] = rowa[1]
		vopen[line_cnt] = parseFloat(rowa[2])
		vclose[line_cnt] = parseFloat(rowa[3])
		vhigh[line_cnt] = parseFloat(rowa[4])
		vlow[line_cnt] = parseFloat(rowa[5])
		vvolume[line_cnt] = parseInt(rowa[6])
	}
	Elap("Make Records ", start, Nowms())
	Elap("total File Load ", tload, Nowms())
	start = Nowms()
	tAvg := Avg(vclose)
	Elap("Calculate Avg() ", start, Nowms())
	start = Nowms()
	fmt.Printf("Avg = %9.2f\n", tAvg)
	for xx := 0; xx < 100; xx++ {
		tAvg := Avg(vclose)
		tAvg -= 0.01
	}
	elapa100 := Nowms() - start
	fmt.Printf("Run 100 avg() =%5.2f  each=%5.3f\n", elapa100, (elapa100 / 100.0))
}
func main() {
	LoadBarsCSVFile("c:/JOE/stock/JTDATA/symbols/SPY/2012.M1.csv")
}
