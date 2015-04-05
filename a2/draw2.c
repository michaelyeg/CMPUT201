#include <stdio.h> 
#include <string.h> /* For strncmp(), etc. */ 
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include "draw2.h"
#include "memwatch.h"

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
            parseFile(fp, argv[ i ],sketch);
            fclose( fp );
            pclose(sketch);
        }
    }
    return( 0 );
} /* main */

char* trimwhitespace (char* str){
    char * end;
    while (strncmp(str," ",1)==0){
        str++;
    }
    end=str+strlen(str)-1;
    while (end>str && strncmp(end," ",1)==0) {
        end--;
    }
    *(end+1)=0;
    return str;
} /* trimwhitespace */
/*Reference: http://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way*/

void error_handling(int num){
    printf ("draw2: %i, error\n",num);
    exit(EXIT_FAILURE);
}

void parseFile( FILE * fp, char * fname, FILE * sketch)
{ 
    int i=0,j=0,linum=0;
    char * cmdname,*cmdname_ext, * cmdtarget, * childcmd, * trimline;
    char * line;
    double *delta_x, *delta_y, *X1, *X2, *Y1, *Y2, *tx, *ty, *a_d, *a_r,*rotated_x,*rotated_y;
    struct image * i_name;
    bool found=false;
    i_name=(struct image *)malloc(100*sizeof(struct image));
    int image_num=0;
    while(fgets(line=(char*)malloc(100*sizeof(char)),100,fp)!=NULL){
        linum++;
        if (strncmp(&line[0]," ",1)==0){	    
            trimline=trimwhitespace(line);
        }
        else{
            trimline=line;
        }
        /*Skip blank lines*/
        if (trimline[0]!='\n'){
            //Image command: Create a struct "image" and record its name
            if (strncmp(trimline,"Image",strlen("Image"))==0){
                cmdname=malloc(sizeof(char*));
                sscanf (trimline,"%s %c %lf %lf\n",cmdname,&i_name[i].name,&i_name[i].x[j],&i_name[i].y[j]);
                i_name[i].size=0;
                image_num+=1;
                free (cmdname);
          }
            //LineTo command: Store all sthe edge points of the lines in the struct "image"
            else if (strncmp(trimline,"lineTo",strlen("lineTo"))==0){
                cmdname=malloc(sizeof(char*));
                delta_x=(double*)malloc(sizeof(double));
                delta_y=(double*)malloc(sizeof(double));
                sscanf(trimline,"%s %lf %lf\n",cmdname, delta_x, delta_y);
                i_name[i].x[j+1]=i_name[i].x[j]+delta_x[0];
                i_name[i].y[j+1]=i_name[i].y[j]+delta_y[0];
                i_name[i].size+=1;
                free(delta_x);
                free(delta_y);
                free(cmdname);
                j+=1;
            }
            else if (strncmp(trimline,"End",strlen("End"))==0){
                cmdname=malloc(sizeof(char*));
                cmdname_ext=malloc(sizeof(char*));
                cmdtarget=malloc(sizeof(char*));
                sscanf (trimline,"%s %s %c\n",cmdname,cmdname_ext,cmdtarget);
                //Error Handling
                if (strncmp(cmdtarget,&i_name[i].name,1)!=0) {
                    error_handling(linum);
                }
                j=0;
                i++;
                free (cmdname);
                free (cmdname_ext);
                free (cmdtarget);
            }
            //Draw command: send drawSegment command to sketchpad
            else if (strncmp(trimline,"draw",4)==0){
                cmdname=malloc(sizeof(char*));
                cmdtarget=malloc(sizeof(char*));
                sscanf(trimline,"%s %s\n",cmdname,cmdtarget);
                int k=0;
                while (k<image_num){
                    if ((strncmp(&i_name[k].name,cmdtarget,1))==0){
                        found=true;
                        for (int l=0;l<(i_name[k].size);l++){
                            X1=(double*)malloc(sizeof(double));
                            Y1=(double*)malloc(sizeof(double));
                            X2=(double*)malloc(sizeof(double));
                            Y2=(double*)malloc(sizeof(double));
                            X1[0]=i_name[k].x[l];
                            Y1[0]=i_name[k].y[l];
                            X2[0]=i_name[k].x[l+1];
                            Y2[0]=i_name[k].y[l+1];
                            fprintf(sketch,"drawSegment %ld %ld %ld %ld\n",lround(*X1),lround(*Y1),lround(*X2),lround(*Y2));
                            fflush(sketch);
                            free(X1);
                            free(Y1);
                            free(X2);
                            free(Y2);
                        }
                    }
                    k+=1;
                }
                if (!found){
                    error_handling(linum);
                }
                free(cmdname);
                free(cmdtarget);
            }
            //Print command: print all the coordinates to stdout
            else if (strncmp(trimline,"print",strlen("print"))==0){
                cmdname=malloc(sizeof(char*));
                cmdtarget=malloc(sizeof(char*));
                sscanf(trimline,"%s %s\n",cmdname,cmdtarget);
                printf("Print Image %s\n",cmdtarget);
                int k=0;
                while (k<image_num){
                    if ((strncmp(&i_name[k].name,cmdtarget,1))==0){
                        found=true;
                        for (int l=0;l<(i_name[k].size+1);l++){
                            printf("%g %g\n",i_name[k].x[l],i_name[k].y[l]);
                        }
                        printf("End Image %s\n",cmdtarget);
                    }
                    k++;
                }
                if (!found){
                    error_handling(linum);
                }
                free(cmdname);
                free(cmdtarget);
            }
            //Translate command: move x and y coordinates accordingly and update them in the struct
            else if (strncmp(trimline,"translate",strlen("translate"))==0){
	        cmdname=(char*)malloc(10*sizeof(char));
	        cmdtarget=malloc(sizeof(char*));
                tx=(double*)malloc(sizeof(double));
                ty=(double*)malloc(sizeof(double));
                sscanf(trimline,"%s %s %lf %lf\n",cmdname,cmdtarget,tx,ty);
                free(cmdname);
                int k=0;
                while (k<image_num){
                    if ((strncmp(&i_name[k].name,cmdtarget,1))==0){
                        found=true;
                        for (int l=0;l<(i_name[k].size+1);l++){
                            i_name[k].x[l]+=tx[0];
                            i_name[k].y[l]+=ty[0];
                        }
                    }
                    k+=1;
                }
                if (!found){
                    error_handling(linum);
                }
                free(cmdtarget);
                free(tx);
                free(ty);
            }
            //Child command
            else if (strncmp(trimline,"child",strlen("child"))==0){
                cmdname=malloc(sizeof(char*));
                childcmd=malloc(50*sizeof(char*));
                sscanf(trimline,"%s %[^-]\n",cmdname,childcmd);
                fprintf(sketch, "%s\n",childcmd);
                free(childcmd);
                free(cmdname);
            }
            //Comment: print the entire comment line to stdout with the whitespaces
            else if (strncmp(trimline,"#",1)==0){
                printf("%s\n",line);
            }
	    //Rotate command
            else if (strncmp(trimline,"rotate",6)==0){
                cmdname=malloc(sizeof(char*));
                cmdtarget=malloc(sizeof(char*));
                a_d=malloc(sizeof(double*));
                a_r=malloc(sizeof(double*));
                int k=0;
                sscanf(trimline,"%s %s %lf\n",cmdname,cmdtarget,a_d);
                *a_r=*a_d * PI /180.0;
                while (k<image_num){
                    if ((strncmp(&i_name[k].name,cmdtarget,1))==0){
                        found=true;
                        for (int l=0;l<(i_name[k].size+1);l++){
                                rotated_x=(double*)malloc(sizeof(double));
                                rotated_y=(double*)malloc(sizeof(double));
                                *rotated_x=i_name[k].x[l]*cos(*a_r)-i_name[k].y[l]*sin(*a_r);
                                *rotated_y=i_name[k].x[l]*sin(*a_r)+i_name[k].y[l]*cos(*a_r);
                                i_name[k].x[l]=*rotated_x;
                                i_name[k].y[l]=*rotated_y;
                                free (rotated_x);
                                free (rotated_y);
                        }
                    }
                    k+=1;
                }
                if (!found){
                    error_handling(linum);
                }
                free (cmdname);
                free (cmdtarget);
                free (a_d);
                free (a_r);
            }
            else{
                error_handling(linum);
            }/* if */    
        }
	free (line);	
    } /* while */
    //Free the struct pointer.
    free (i_name);
    free (line);
} /* parseFile */
