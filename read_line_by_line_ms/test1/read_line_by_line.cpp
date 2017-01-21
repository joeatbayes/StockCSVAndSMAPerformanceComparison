// test1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

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
	double total = 0.0;
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
	return numRec;
}

char *readAllText(char *fiName) {
	FILE *fp = fopen(fiName, "rb");
	if (!fp) {
		printf("Couldn't open file for reading %s\n", fiName);
		return NULL;
	}
	setbuf(fp, NULL);
	fseek(fp, 0L, SEEK_END);
	long fsize = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	char *buff = (char *) malloc(sizeof(char) * fsize + 1);
	long nread = (long) fread(buff, 1, fsize, fp);
	if (nread <= 0) {
		printf ("failed read got=%d  expected=%d in %s ", nread, fsize, fiName);
		free(buff);
		return NULL;
	}
	else {
		printf("Read %d bytes from %s", fsize, fiName);
	}
	buff[fsize] = 0;
	return buff;
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

//double *parseArray(char *buff) {
//	return 0.0;
//}

double *TestLoadBulk(char *fiName,  long *numRec) {

	char *buff =  readAllText(fiName);
	long numRow = countMatchChar(buff, '\n');
	printf("found %d lines", numRow);
	double *varr = (double *) malloc(sizeof(double) * (numRow + 5));
	char *curr = buff;
	char *prev = buff;
	char *token;
	long rowcnt = 0;
	const char delim[2] = ",";
	char *endtodptr;
	for(; *curr != 0; curr++) {
		if (*curr == '\n') {
			if (rowcnt > 0) {
				// parse the fields for the line just ended.
				// pull out a the 3rd field
				// create convert to a float and save
				*curr = 0;
				int fldnum = 0;
				token = strtok(prev, delim);
				while (token != NULL) {
					fldnum++;
					//printf("token=%s", token);
					if (token == NULL)
						break;
					if (fldnum == 3) {
						varr[rowcnt] = strtod(token,&endtodptr);
					}
					token = strtok(NULL, delim);
				}
			}
			*curr = '\n'; // replace token delimiter
			prev = curr + 1;
			rowcnt++;
		}
	}
	free(buff);
	*numRec = numRow;
	return varr;
}

// test loading in 100K chunks that are tokenized
// as they are loaded from the string requires extra
// logic to reassemble chunks when the line crosses
// the chunk boundary or to seek back to beginning
// of  partial line to re-read.
//void *TestLoadByChunks(char *fname) {
//
//}

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
	printf("numRec in File=%d\n", *numRec);
	fseek(fp, 0, SEEK_SET);
	fgets(header, 4999, fp);

	char *token;
	const char delim[2] = ",";
	char *endtodptr;
	double *varr = (double *) malloc(sizeof(double) * (*numRec + 5));
	printf("allocated memory\n");

	int recCnt = 0;
	while(fgets(line, 4999, fp) != NULL){
		//printf("read %s  ", line);
		int fldnum = 0;
		token = strtok(line, delim);
		while (token != NULL) {
			fldnum++;
			//printf("token=%s", token);
			if (token == NULL)
				break;
			if (fldnum == 3) {
				varr[recCnt] = strtod(token,&endtodptr);
			}
			token = strtok(NULL, delim);
		}
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

//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

