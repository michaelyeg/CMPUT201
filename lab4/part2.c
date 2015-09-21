#include <stdio.h>
#include <stdlib.h>

int main (int argc, char * argv[])
{
  FILE * fp;
  long * number;
  int j=0,size;
  char line[256];
  fp=fopen(argv[1],"r");
  if( fp == NULL ) 
    { 
      printf( "Could not open file %s\n", argv[1] ); 
      exit( -1 ); 
    } 
  else{
    fgets(line,256,fp);
    sscanf(line,"%i",&size);
    number=(long*) malloc (size*sizeof(long));
    while (fgets(line,256,fp)!=NULL){
      sscanf(line,"%ld",&number[j]);
      j++;
    }
  } 
  j=j-1;
  while (j>=0){
    printf("%ld\n",number[j]);
    j=j-1;
  } 
  free (number);
  return (0);
}
