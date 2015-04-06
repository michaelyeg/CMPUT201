#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <math.h>

void drawlandscape(FILE * landscape, char * filename, FILE * sketch);

int main(int argc, char*argv[])
{
    FILE * landscape;
    FILE * sketch;
    sketch=popen("java -jar Sketchpad.jar","w");
    for (int i=1; i<argc; i++){
        landscape=fopen(argv[i],"r");
        if (landscape==NULL){
            printf("Could not draw landscape\n");
            exit (-1);
        }
        else
        {
            drawlandscape(landscape, argv[i], sketch);
            fclose (landscape);
            pclose (sketch);
        }
    }
    return (0);
}

void drawlandscape(FILE * landscape, char * filename, FILE * sketch){
    char * line;
    long coordinate[12];
    int i=0;
    while (fgets(line=(char*)malloc(100*sizeof(char)),100,landscape)!=NULL){
        sscanf(line,"%ld %ld\n",&coordinate[i],&coordinate[i+1]);
        i+=2;
        free (line);
    }/* while */
    free (line);
    
    for (int j=0; j<5; j+=1){
        fprintf(sketch, "drawSegment %ld %ld %ld %ld\n",lround(coordinate[2*j]),lround(coordinate[2*j+1]),lround(coordinate[2*j+2]),lround(coordinate[2*j+3]));
    }
    fprintf(sketch, "pause 5\n");
}/* drawlandscape */