#include<stdio.h>
#include <stdlib.h> 
#include <time.h>
typedef char TimeStr[24];
typedef char DayStr[5];
double CLOCKS_PER_MS = CLOCKS_PER_SEC / 1000.0;
double nowms() {
  return (((double) clock()) / CLOCKS_PER_MS);
  //return ((double) clock() /);
}
double elap(const char *msg, double start, double stop) {
  double tmp = stop - start;
  printf ("%s elap = %9.2f\n", msg, tmp);
}
char * copyNext(char *dest, char *src, char delim) {
  while ((*src != 0) && (*src != delim))  {
     *dest = *src;
     dest++;
     src++;
  }
  *dest = 0;
  if (*src != 0)
    src++;
  return src;
}
double avg(double *nptr,  int numCnt) {
  double total = 0.0;
  for (int ndx=0; ndx < numCnt; ndx++)
  {
     total += nptr[ndx];
  }
  return total / numCnt;
}
int main() {
  double startRun = nowms();
  FILE *ptr_file;
  char buf[1000];
  char bbuf[50003];
  ptr_file =fopen("c:/JOE/stock/JTDATA/symbols/SPY/2012.M1.csv","r");
  if (!ptr_file)
     return 1;
  double startCount = nowms(); 
  int bcnt = 0;
  int numRec = 0;
  while (1)  {
    int numRead = fread(bbuf,1, 50000, ptr_file);
    if (numRead == 0)
      break;
    bbuf[numRead+1] = 0;
    char *ptr = (char *) &bbuf;
    for (; *ptr != 0; ptr++) {
      if (*ptr == '\n')
        numRec++;
    }
    bcnt++;
  }
  numRec--; // take one away for header.
  double stopCount = nowms(); 
  int lca = numRec + 1;
  double startAlloc = nowms(); 
  TimeStr *vtimes = (TimeStr *) malloc(sizeof(TimeStr) * lca);
  DayStr *vdays = (DayStr *) malloc(sizeof(DayStr) * lca);
  double *vopen = (double *) malloc(sizeof(double) * lca);
  double *vclose =(double *) malloc(sizeof(double) * lca);
  double *vhigh = (double *) malloc(sizeof(double) * lca);
  double *vlow =  (double *) malloc(sizeof(double) * lca);
  int *vvolume =  (int *) malloc(sizeof(int) * lca);
  double stopAlloc = nowms();
  double startBuild = nowms(); 
  fseek(ptr_file, 0, SEEK_SET);
  int lc = 0;
  char * tkn;
  char tkbuf[80];
  fgets(buf,1000, ptr_file); // read first line
  while (fgets(buf,1000, ptr_file)!=NULL) {    
    double tnum;
    tkn = (char *) &buf;
    tkn = copyNext(vtimes[lc], tkn, ','); // dateTime
    tkn = copyNext(vdays[lc], tkn, ','); // day
    tkn = copyNext(tkbuf, tkn, ','); // open
    vopen[lc] = atof((char *) &tkbuf);
    tkn = copyNext(tkbuf, tkn, ','); // close
    vclose[lc] = atof((char *) &tkbuf);
    tkn = copyNext(tkbuf, tkn, ','); // high
    vhigh[lc] = atof((char *) &tkbuf);
    tkn = copyNext(tkbuf, tkn, ','); // low
    vlow[lc] = atof((char *) &tkbuf);
    tkn = copyNext(tkbuf, tkn, ','); // volume
    vvolume[lc] = atol((char *) &tkbuf);
    lc++;
  }
  lc--; // one to high at end.
  double stopBuild = nowms(); 
  double stopRun = stopBuild;
  double startAvg = nowms(); 
  double tavg = avg(vclose, lc);
  double stopAvg = nowms(); 
  double start100Avg = nowms();
  for (int x=0; x<=100; x++)
    tavg = avg(vclose, lc);
  double stop100Avg = nowms();
  double startCleanup = nowms();
  free(vopen);
  free(vclose);
  free(vhigh);
  free(vlow);
  fclose(ptr_file);
  double stopCleanup = nowms();
  elap("Read in 50K blocks", startCount, stopCount);
  printf("Read %d of 50k blocks\n", bcnt);
  printf("Read %d lines\n", numRec);
  elap("allocate vector array", startAlloc, stopAlloc);  
  elap("build Recs", startBuild, stopBuild);
  printf("Read %d numRec\n", lc);
  elap("Total Load ", startRun, stopRun);
  elap("calc average", startAvg, stopAvg);
  double telap = stop100Avg - start100Avg;
  printf("calc average 100 times %11.2f  each=%11.2f\n", telap, (telap / 100));
  elap("cleanup", startCleanup, stopCleanup);
  return 0;
}