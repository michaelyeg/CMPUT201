#include <stdio.h> 
#include <string.h> /* For strncmp(), etc. */ 
#include <stdlib.h>
#define MIN_BUF 256 
#define MAX_BUF 2048 
char Buffer[ MAX_BUF ]; 
char word[ MIN_BUF ]; 
void parseFile( FILE * fp, char * fname ); 
 
int main( int argc, char * argv[] ) 
{ 
  int i; 
  FILE * fp; 
  fprintf(stdout,"draw1a started on ");
  fflush(stdout);
  /*print out system date*/
  system("date");
  for( i = 1; i < argc; i++ ) { 
    fp=fopen(argv[i],"r");
    if( fp == NULL ) 
      { 
	printf( "Could not open file %s\n", argv[ i ] ); 
	exit( -1 ); 
      } 
    else 
      { 
	printf("Inuput File: %s\n", argv[i]);
	parseFile( fp, argv[ i ] );
	fclose( fp ); 
      } 
  } 
  return( 0 ); 
} /* main */ 
 
void parseFile( FILE * fp, char * fname ) 
{ 
  /*create an array with 8 0s*/
  int num[8]={0};
  /*Assign 8 pointers to the addess of each num,which will later used to count the command lines.*/
  int *image=&num[0], *lineto=&num[1], *end=&num[2], *print=&num[3], *draw=&num[4], *tran=&num[5], *child=&num[6], *com=&num[7]; 
  int rval;
  /* Read first word */ 
  if ((rval=fscanf( fp, "%s%*[^\n]", word))==1){
    while( ! feof( fp ) ) 
      { 
	if (strncmp(word,"Image",strlen("Image"))==0){
	  *image+=1;
	}
	else if (strncmp(word,"lineTo",strlen("lineTo"))==0){
	  *lineto+=1;
	}
	else if (strncmp(word,"End",strlen("End"))==0){
	  *end+=1;
	}
	else if (strncmp(word,"print",strlen("print"))==0){
	  *print+=1;
	}
	else if (strncmp(word,"draw",strlen("draw"))==0){
	  *draw+=1;
	}
	else if (strncmp(word,"translate",strlen("translate"))==0){
	  *tran+=1;
	}
	else if (strncmp(word,"child",strlen("child"))==0){
	  *child+=1;
	}
	else if (strncmp(word,"#",strlen("#"))==0){
	  *com+=1;
	}
	fscanf( fp, "%s%*[^\n]", word );
      } /* while */ 
      printf ("%i Image definition(s)\n",*image);
      printf ("%i lineTo command(s) within Figures\n",*lineto);
      printf ("%i End command(s)\n",*end);
      printf ("%i print command(s)\n",*print);
      printf ("%i draw command(s)\n",*draw);
      printf ("%i translate command(s)\n",*tran);
      printf ("%i child command(s)\n",*child);
      printf ("%i comment(s)\n",*com);
  }
  else{
    printf("Empty File, Exit.\n");
    return;
  }
} /* parseFile */ 
