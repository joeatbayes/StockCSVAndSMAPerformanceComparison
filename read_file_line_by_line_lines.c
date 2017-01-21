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
}


double simpAvg(double *nptr,  int startNdx,  int endNdx)
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
		int tbeg  = fmax(1, ndx - numPer);
     tout[ndx] =  simpAvg(nptr, tbeg, ndx);
  }
}

int countLines(FILE *fp) {
	int numRec = 0;
	char bbuf[50003];
	while (1)
  {
     int numRead = fread(bbuf,1, 50000, fp);
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


double  *TestLoadByLine(char *fname, long *numRec) {
	char data1[70];
  char data2[2];
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
	double *varr = malloc(sizeof(double) *  *numRec);
	printf("allocated memory for %d records \n", *numRec);
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
  long numRec;
	char *fname = "c:/calgo-data/exp-EURUSD-Tick-rsi14-stddev100-bars.csv";
  double *tarr = TestLoadByLine(fname, &numRec);
  printf("CLOCKS_PER_MS=%lf  CLOCKS_PER_SEC=%lf\n", (double) CLOCKS_PER_MS, (double) CLOCKS_PER_SEC);
  double startsma = clock();
  double *smaarr = (double *) malloc(sizeof(double) * numRec + 1);
  sma(tarr, 1, numRec, 1000, smaarr);
  double endsma = clock();
  elap("finished sma", startsma, endsma);
  free(tarr);
  free(smaarr);
  return 0;
}
