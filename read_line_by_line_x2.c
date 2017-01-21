// test1.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

double CLOCKS_PER_MS = (double) CLOCKS_PER_SEC / (double) 1000.0;

double elap(const char *msg, double pstart, double pstop)
{
	double telap = pstop - pstart;
	double tmp  = telap /  (double) CLOCKS_PER_SEC;
	printf("elapsec=%lf\n", tmp);
	printf ("%s elapclock=%lf  elapsec=%f start=%lf  stop=%lf\n", msg, telap, tmp, pstart, pstop);
	return tmp;
}


inline double simpAvg(double *nptr,  int startNdx,  int endNdx)
{
	int numEle = (endNdx - startNdx) + 1;
	if (numEle < 1) {
		return 0.0;
	}

	double total = 0.0;
	for (int ndx=startNdx; ndx <= endNdx; ndx++)
	{
		total += nptr[ndx];
	}
	return total / numEle;
}


void sma(double *nptr,  int startNdx, int endNdx, int numPer,  double *tout)
{	
	for (int ndx=startNdx; ndx <= endNdx; ndx++)
	{
		int tbeg  = ndx - numPer;
		if (tbeg < 1) tbeg  = 1;
		tout[ndx] = simpAvg(nptr, tbeg, ndx);
	}
}


size_t  countLines(FILE *fp) {
	size_t numRec = 0;
	char bbuf[50003];
	while (1)
	{
		size_t numRead = fread(bbuf, 1, 50000, fp);
		if (numRead == 0)
			break;
		bbuf[numRead+1] = 0;
		char *ptr = (char *) &bbuf;
		for (; *ptr != 0; ptr++)
		{
			if (*ptr == '\n')
				numRec++;
		}
	}
	numRec--; // take one away for header.
	fseek(fp, 0, SEEK_SET);
	return numRec;
}

long countMatchChar(char *buff, char match) {
	long cnt = 0;
	for(; ; buff++) {
		char cc = *buff;
		if (cc == 0)
			break;
		if (cc == match) {
			cnt++;
		}
	}
	return cnt;
}



/* copy the characters into buffer until EOS
or delim is found.  Returns pointer to next 
available char or ptr to null if has reached EOS 
this is a little faster than strtok bcause it
only has to look at one possible delim */
char * copyNext(char *dest, char *src, char delim) 
{
  while ((*src != 0) && (*src != delim))
  {
     *dest = *src;
     dest++;
     src++;
  }
  *dest = 0;
  if (*src != 0)
    src++;
  return src;
}


void nextStr(char *dest,  char **src) {
   *src = copyNext(dest, *src, ','); // high   	   
}


double nextDouble(char **src) {
   char *endtodptr;
   double tout = strtod(*src,&endtodptr);   
   *src = endtodptr += 2; // advance past the comma
   return tout;
}

double nextLong(char **src) {
   char *endtodptr;
   long tout = strtol(*src,&endtodptr,10);   
   *src = endtodptr += 2; // advance past the comma
   return tout;
}


double  *TestLoadByLine(char *fname,  size_t *numRec) {
	char header[5000];
	char line[5000];
	double startParse = clock();
	int j = 0;
	FILE *fp = fopen(fname, "r");
	if (!fp) {
		printf("Couldn't open file for reading\n");
		return 0;
	}
	*numRec = countLines(fp);
	printf("numRec in File=%Iu\n", *numRec);
	
	fgets(header, 4999, fp);
	double *varr = (double *) malloc(sizeof(double) * (*numRec + 5));
	printf("allocated memory for %Iu double\n", *numRec);
	int recCnt = 0;
	char *token = (char *) malloc(sizeof(char) * 5000);		
	while(fgets(line, 4999, fp) != NULL){
		//printf("read %s  ", line);		
		
		char *next = copyNext(token, line, ',');
		next = copyNext(token, next, ','); // datetime
		next = copyNext(token, next, ','); // open
		varr[recCnt] = nextDouble(&next);  // close
		next = copyNext(token, next, ','); // high
		next = copyNext(token, next, ','); // low
		next = copyNext(token, next, ','); // volume
		next = copyNext(token, next, ','); // weighted_val
		next = copyNext(token, next, ','); // rsi
		next = copyNext(token, next, ','); // stddev
		next = copyNext(token, next, ','); // spread		
		printf("fld3=%8.5lf\n", varr[recCnt] );
		
		next = copyNext(token, next, ',');
		next = copyNext(token, next, ',');
		recCnt ++;
	}
	double endParse = clock();
	printf("%d\n",j);
	elap("Finish parsing loop", startParse, endParse);	
	return varr;
}

int main() {
	size_t numRec;
	char *fname = "c:/calgo-data/exp-EURUSD-Tick-rsi14-stddev100-bars.csv";
	printf("CLOCKS_PER_MS=%lf  CLOCKS_PER_SEC=%lf\n", (double) CLOCKS_PER_MS, (double) CLOCKS_PER_SEC);

	//double*tarr = TestLoadBulk(fname, &numRec);
	double *tarr = TestLoadByLine(fname, &numRec);
	double *smaarr = (double *) malloc(sizeof(double) * numRec + 1);
	sma(tarr, 1, numRec, 1000, smaarr);

	free(tarr);
	free(smaarr);
	return 0;

}


