package main

import "fmt"

//import "io/ioutil"
import "strings"
import "time"
import "strconv"
import "os"
import "log"
import "bufio"
import s "strings"

const BadFloatParse = -0.00005
const BadIntParse = -1

type Floats []float64
type Bar struct {
	DateTime string
	Day      string
	Open     float64
	Close    float64
	High     float64
	Low      float64
	Volume   int
}

func Nowms() float64 {
	nn := time.Now()
	return float64(nn.UnixNano()) / float64(time.Millisecond)
}
func Elap(msg string, beg_time float64, end_time float64) float64 {
	elap := end_time - beg_time
	fmt.Printf("ELAP %s = %12.3f ms\n", msg, elap)
	return elap
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
func Avg(s []*Bar) float64 {
	tot := float64(0.0)
	for _, bar := range s {
		tot += bar.Close
	}
	return tot / float64(len(s))
}
func LoadBarsCSVFile(fname string) []*Bar {
	fmt.Printf("\nfiName=%s\n", fname)
	start := Nowms()
	tload := start

	start = Nowms()
	fiIn, err := os.Open(fname)
	if err != nil {
		log.Fatal(err)
	}
	scanner := bufio.NewScanner(fiIn)
	defer fiIn.Close()

	scanner.Scan() // skip headers

	//headTxt := s.TrimSpace(scanner.Text())
	Elap("Open File ", start, Nowms())
	start = Nowms()
	bars := make([]*Bar, 0, 100000)
	Elap("Allocate Bars", start, Nowms())

	start = Nowms()
	for scanner.Scan() {
		rowStr := s.TrimSpace(scanner.Text())

		if err := scanner.Err(); err != nil {
			log.Fatal(err)
		}

		rowa := strings.SplitN(rowStr, ",", 9)
		lObj := new(Bar)
		lObj.DateTime = rowa[0]
		lObj.Day = rowa[1]
		lObj.Open = parseFloat(rowa[2])
		lObj.Close = parseFloat(rowa[3])
		lObj.High = parseFloat(rowa[4])
		lObj.Low = parseFloat(rowa[5])
		lObj.Volume = parseInt(rowa[6])
		bars = append(bars, lObj)
	}
	Elap("Make Records ", start, Nowms())
	Elap("total File Load ", tload, Nowms())
	start = Nowms()
	tAvg := Avg(bars)
	Elap("Calculate Avg() ", start, Nowms())
	fmt.Printf("Avg = %9.2f\n", tAvg)
	start = Nowms()
	fmt.Printf("Avg = %9.2f\n", tAvg)
	for xx := 0; xx < 100; xx++ {
		tAvg := Avg(bars)
		tAvg -= 0.01
	}
	elapa100 := Nowms() - start
	fmt.Printf("Run 100 avg() =%5.2f  each=%5.3f\n", elapa100, (elapa100 / 100.0))
	return bars
}
func main() {
	lb := LoadBarsCSVFile("2014.M1.csv")
	fmt.Printf("len(newBars)=%d\n", len(lb))
}
