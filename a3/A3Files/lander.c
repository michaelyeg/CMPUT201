#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <math.h>

void drawlandscape(FILE * landscape, char * filename, FILE * sketch);
void drawship (FILE * sketch);
void drawthrust (FILE * sketch);

struct pos{
    double x;
    double y;
};

struct pos shuttle[4];
struct pos thurst[2];

int main(int argc, char * argv[])
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
            drawship(sketch);
            drawthrust(sketch);
            fprintf (sketch,"end");
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
}/* drawlandscape */

void drawship (FILE * sketch){
    shuttle[0].x=320;
    shuttle[0].y=0;
    shuttle[1].x=340;
    shuttle[1].y=0;
    shuttle[2].x=350;
    shuttle[2].y=15;
    shuttle[3].x=310;
    shuttle[3].y=15;
    fprintf(sketch,"drawSegment %ld %ld %ld %ld\n",lround(shuttle[0].x),lround(shuttle[0].y),lround(shuttle[1].x),lround(shuttle[1].y));
    fprintf(sketch,"drawSegment %ld %ld %ld %ld\n",lround(shuttle[1].x),lround(shuttle[1].y),lround(shuttle[2].x),lround(shuttle[2].y));
    fprintf(sketch,"drawSegment %ld %ld %ld %ld\n",lround(shuttle[2].x),lround(shuttle[2].y),lround(shuttle[3].x),lround(shuttle[3].y));
    fprintf(sketch,"drawSegment %ld %ld %ld %ld\n",lround(shuttle[3].x),lround(shuttle[3].y),lround(shuttle[0].x),lround(shuttle[0].y));
    
}/* drawship */

void drawthrust (FILE * sketch){
    thurst[0].x=shuttle[3].x+20;
    thurst[0].y=shuttle[3].y+10;
    thurst[1].x=thurst[0].x;
    thurst[1].y=thurst[0].y+20;
    fprintf(sketch,"drawSegment %ld %ld %ld %ld\n",lround(shuttle[3].x),lround(shuttle[3].y),lround(thurst[0].x),lround(thurst[0].y));
    fprintf(sketch,"drawSegment %ld %ld %ld %ld\n",lround(shuttle[2].x),lround(shuttle[2].y),lround(thurst[0].x),lround(thurst[0].y));
    fprintf(sketch, "pause 1\n");
    fprintf(sketch,"eraseSegment %ld %ld %ld %ld\n",lround(shuttle[3].x),lround(shuttle[3].y),lround(thurst[0].x),lround(thurst[0].y));
    fprintf(sketch,"eraseSegment %ld %ld %ld %ld\n",lround(shuttle[2].x),lround(shuttle[2].y),lround(thurst[0].x),lround(thurst[0].y));
    fprintf(sketch,"drawSegment %ld %ld %ld %ld\n",lround(shuttle[3].x),lround(shuttle[3].y),lround(thurst[1].x),lround(thurst[1].y));
    fprintf(sketch,"drawSegment %ld %ld %ld %ld\n",lround(shuttle[2].x),lround(shuttle[2].y),lround(thurst[1].x),lround(thurst[1].y));
    fprintf(sketch, "pause 1\n");
}/* drawthrust */