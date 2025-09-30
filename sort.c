#include "types.h"
#include "user.h"
#include "stat.h"

#define MAXLINES 512
#define MAXLEN 128

char lines[MAXLINES][MAXLEN];
char *lineptr[MAXLINES];
int nlines=0;

int str_cmp(const char *a,const char *b){
return strcmp(a,b);
}

int num_cmp(const char *a,const char *b){
    int na=atoi(a); int nb=atoi(b);
    if(na<nb) return -1;
    if(na>nb) return 1;
    return strcmp(a,b);
}

void swap(char **a,char **b){
    char *t=*a;*a=*b;*b=t;
}

void sort_lines(int numeric){
int i,j;
for(i=0;i<nlines-1;i++)
 for(j=0;j<nlines-i-1;j++)
 {
   int c=numeric?num_cmp(lineptr[j],lineptr[j+1]):str_cmp(lineptr[j],lineptr[j+1]);
   if(c>0) swap(&lineptr[j],&lineptr[j+1]);
 }
}

int main(int argc,char *argv[])
{
int numeric=0,reverse=0,unique=0;
char buf[512];
int fd=0,argi=1;
while(argi<argc&&argv[argi][0]=='-')
{ 
 if(strcmp(argv[argi],"-n")==0) numeric=1;
 else if(strcmp(argv[argi],"-r")==0) reverse=1;
 else if(strcmp(argv[argi],"-u")==0) unique=1;
 else { printf(2,"Unknown flag:%s\n",argv[argi]); exit(); }
 argi++;
}
if(argi<argc)
{ fd=open(argv[argi],0);
  if(fd<0){ printf(2,"sort: cannot open %s\n",argv[argi]); exit(); } 
}

int n,l=0,i,idx=0;
while((n=read(fd,buf,sizeof(buf)))>0){
 for(i=0;i<n;i++){
  if(buf[i]=='\n'){ lines[nlines][l]=0; lineptr[nlines]=lines[nlines]; nlines++; l=0; }
  else if(l<MAXLEN-1) lines[nlines][l++]=buf[i];
 }
}

sort_lines(numeric);

if(reverse){
for(i=nlines-1;i>=0;i--){
 if(unique && i<nlines-1 && strcmp(lineptr[i],lineptr[i+1])==0) continue;
 printf(1,"%s\n",lineptr[i]);
}
}else{
for(i=0;i<nlines;i++){
 if(unique && i>0 && strcmp(lineptr[i],lineptr[i-1])==0) continue;
 printf(1,"%s\n",lineptr[i]);
}
}

if(fd!=0) close(fd);
exit();
}
