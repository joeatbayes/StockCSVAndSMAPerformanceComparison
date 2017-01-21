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
	long nread = fread(buff, 1, fsize, fp);
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

double *TestLoadBulk(char *fiName,  long *numRec) {
	char *buff =  readAllText(fiName);
	long numRow = countMatchChar(buff, '\n');
	printf("found %d lines", numRow);
	double *varr = malloc(sizeof(double) * numRow);
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
		  *curr - '\n'; // replace token delimiter
			prev = curr + 1;
		 	rowcnt++;
		 }
  }
	free(buff);
	*numRec = numRow;
	return varr;
}

int main() {
	 long numRec;
	 char *fname = "2014.M1.csv";
  printf("CLOCKS_PER_MS=%lf  CLOCKS_PER_SEC=%lf\n", (double) CLOCKS_PER_MS, (double) CLOCKS_PER_SEC);

  double*tarr = TestLoadBulk(fname, &numRec);
  double *smaarr = (double *) malloc(sizeof(double) * numRec + 1);

	//double*tarr = TestLoadByLine(fname);
  sma(tarr, 1, numRec, 1000, smaarr);

	free(tarr);
	free(smaarr);
	return 0;
}
