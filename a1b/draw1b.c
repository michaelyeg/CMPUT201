#include <stdio.h> 
#include <string.h> /* For strncmp(), etc. */ 
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "draw1b.h"

int main( int argc, char * argv[]) 
{ 
  int i; 
  FILE * fp; 
  FILE* sketch; 
  fprintf(stdout,"draw1b started on ");
  fflush(stdout);
  /*print out system date*/
  system("date");
  sketch=popen("java -jar Sketchpad.jar","w");
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
	parseFile( fp, argv[ i ],sketch);
	fclose( fp ); 
	pclose(sketch);
      } 
  } 
  return( 0 ); 
} /* main */ 
 
void parseFile( FILE * fp, char * fname, FILE * sketch) 
{ 
  int i=0,j=0;
  char image[6],lineto[7],print[6],child[6],draw[5],translate[10];
  char line[100];
  char p_name[1],d_name[1],t_name[1];
  char cmd[50];
  struct image i_name[16];
  int image_num=0;
  while( fgets(line, 100, fp) !=NULL) 
    { 
      //Image command: Create a struct "image" and record its name
      if (strncmp(line,"Image",strlen("Image"))==0){
	sscanf (line,"%s %s %lf %lf",image,&i_name[i].name,&i_name[i].x[j],&i_name[i].y[j]);
	i_name[i].size=0;
	image_num+=1;
      }
      //LineTo command: Store all the edge points of the lines in the struct "image"
	else if (strncmp(line,"lineTo",strlen("lineTo"))==0){
	  sscanf(line,"%s %lf %lf",lineto, &delta_x, &delta_y);
	  i_name[i].x[j+1]=i_name[i].x[j]+delta_x;
	  i_name[i].y[j+1]=i_name[i].y[j]+delta_y;
	  i_name[i].size+=1;
	  j+=1;
	}
	else if (strncmp(line,"End",strlen("End"))==0){
	  j=0;
	  i++;
	}
      //Draw command: send drawSegment command to sketchpad
	else if (strncmp(line,"draw",4)==0){
	  sscanf(line,"%s %s",draw,d_name);
	  int k=0;
	  while (k<image_num){
	    if ((strncmp(&i_name[k].name,d_name,1))==0){
	      for (int l=0;l<(i_name[k].size);l++){
		x1=i_name[k].x[l];
		y1=i_name[k].y[l];
		x2=i_name[k].x[l+1];
		y2=i_name[k].y[l+1];
		fprintf(sketch,"drawSegment %ld %ld %ld %ld\n",lround(x1),lround(y1),lround(x2),lround(y2));
		fflush(sketch);
	      }
	    }
	    k+=1;
	  }
	}
      //Print command: print all the coordinates to stdout
	else if (strncmp(line,"print",strlen("print"))==0){
	  sscanf(line,"%s %s",print,p_name);
	  printf("Print Image %s\n",p_name);
	  int k=0;
	  while (k<image_num){
	    if ((strncmp(&i_name[k].name,p_name,1))==0){
	      for (int l=0;l<(i_name[k].size+1);l++){
		printf("%i %i\n",(int) i_name[k].x[l],(int) i_name[k].y[l]);
		}
	      printf("End Image %s\n",&i_name[k].name);
	    }
	    k++;
	  }
	}
      //Translate command: move x and y coordinates accordingly and update them in the struct
	else if (strncmp(line,"translate",strlen("translate"))==0){
	  sscanf(line,"%s %s %lf %lf",translate,t_name,&tx,&ty);
	  int k=0;
	  while (k<image_num){
	    if ((strncmp(&i_name[k].name,t_name,1))==0){
	      for (int l=0;l<(i_name[k].size+1);l++){
		i_name[k].x[l]+=tx;
		i_name[k].y[l]+=ty;
	      }
	    }
	    k+=1;
	  }
	}
	else if (strncmp(line,"child",strlen("child"))==0){
	  sscanf(line,"%s %[^-]",child,cmd);
	  fprintf(sketch, "%s\n", cmd);
	}
      //Comment
	else if (strncmp(line,"#",1)==0){
	  printf("%s",line);
	}
      } /* while */ 
} /* parseFile */ 
