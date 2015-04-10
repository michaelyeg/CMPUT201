#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <math.h>
#include <curses.h>
#include <signal.h>
#include "lander.h"

struct pos shuttle[4];
struct pos thrust[2];
double angle=90;
double angle_rad;

int main(int argc, char * argv[])
{
    FILE * landscape;
    FILE * sketch;

    sketch=popen("java -jar Sketchpad.jar","w");
    setup_curses();
    
    /*consturcting the ship*/
    shuttle[0].x=320;
    shuttle[0].y=0;
    shuttle[1].x=340;
    shuttle[1].y=0;
    shuttle[2].x=350;
    shuttle[2].y=15;
    shuttle[3].x=310;
    shuttle[3].y=15;
    /*constructing the initial thrust*/
    thrust[0].x=shuttle[3].x+20;
    thrust[0].y=shuttle[3].y+10;
    thrust[1].x=thrust[0].x;
    thrust[1].y=thrust[0].y+20;

    move(5, 10);
    printw("Press any key to start.\n");
    refresh();
    int c = getch();

    nodelay(stdscr, true);
    erase();

    move(5, 10);
    printw("left arrow key rotates counter-clockwise, right for clockwise, space for thrust, q to quit.\n");
    refresh();
    
    for (int i=1; i<argc; i++){
        landscape=fopen(argv[i],"r");
        if (landscape==NULL){
            printf("Could not draw landscape\n");
            exit (-1);
        }
        else
        {
            drawlandscape(landscape, argv[i], sketch);
	    fflush(sketch);
            drawship(sketch);
	    fflush(sketch);
        }
    }/* for */
    c = getch();
    while (1){
      if (c!=ERR){
	erase();
	move (5,10);
	printw("left arrow key rotates counter-clockwise, right for clockwise, space for thrust, q to quit.");
	move (6,10);
	if (c==' '){
	  drawthrust(sketch);
	  fflush(sketch);
	}
	else if (c==KEY_LEFT){
	  rotate_left(sketch);
	  fflush(sketch);
	}
	else if (c==KEY_RIGHT){
	  rotate_right(sketch);
	  fflush(sketch);
	}
	else if (c=='q'){
	  fprintf (sketch,"end");
	  break;
	}
	refresh();
      }
      c=getch();
    }
    unset_curses();
    exit(EXIT_SUCCESS);
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
    fprintf(sketch,"drawSegment %ld %ld %ld %ld\n",lround(shuttle[0].x),lround(shuttle[0].y),lround(shuttle[1].x),lround(shuttle[1].y));
    fprintf(sketch,"drawSegment %ld %ld %ld %ld\n",lround(shuttle[1].x),lround(shuttle[1].y),lround(shuttle[2].x),lround(shuttle[2].y));
    fprintf(sketch,"drawSegment %ld %ld %ld %ld\n",lround(shuttle[2].x),lround(shuttle[2].y),lround(shuttle[3].x),lround(shuttle[3].y));
    fprintf(sketch,"drawSegment %ld %ld %ld %ld\n",lround(shuttle[3].x),lround(shuttle[3].y),lround(shuttle[0].x),lround(shuttle[0].y));
    update_midpoint();
}/* drawship */

void drawthrust (FILE * sketch){
    fprintf(sketch,"drawSegment %ld %ld %ld %ld\n",lround(shuttle[3].x),lround(shuttle[3].y),lround(thrust[0].x),lround(thrust[0].y));
    fprintf(sketch,"drawSegment %ld %ld %ld %ld\n",lround(shuttle[2].x),lround(shuttle[2].y),lround(thrust[0].x),lround(thrust[0].y));
    fprintf(sketch, "pause 1\n");
    fprintf(sketch,"eraseSegment %ld %ld %ld %ld\n",lround(shuttle[3].x),lround(shuttle[3].y),lround(thrust[0].x),lround(thrust[0].y));
    fprintf(sketch,"eraseSegment %ld %ld %ld %ld\n",lround(shuttle[2].x),lround(shuttle[2].y),lround(thrust[0].x),lround(thrust[0].y));
    fprintf(sketch,"drawSegment %ld %ld %ld %ld\n",lround(shuttle[3].x),lround(shuttle[3].y),lround(thrust[1].x),lround(thrust[1].y));
    fprintf(sketch,"drawSegment %ld %ld %ld %ld\n",lround(shuttle[2].x),lround(shuttle[2].y),lround(thrust[1].x),lround(thrust[1].y));
    fprintf(sketch, "pause 1\n");
    fprintf(sketch,"eraseSegment %ld %ld %ld %ld\n",lround(shuttle[3].x),lround(shuttle[3].y),lround(thrust[1].x),lround(thrust[1].y));
    fprintf(sketch,"eraseSegment %ld %ld %ld %ld\n",lround(shuttle[2].x),lround(shuttle[2].y),lround(thrust[1].x),lround(thrust[1].y));
}/* drawthrust */

void setup_curses()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
}/* setup_curses */

void unset_curses()
{
    keypad(stdscr, false);
    nodelay(stdscr, false);
    nocbreak();
    echo();
    endwin();
}/* unset_curses */

void update_midpoint(){
    x_min=min(shuttle[0].x,shuttle[1].x,shuttle[2].x,shuttle[3].x);
    x_max=max(shuttle[0].x,shuttle[1].x,shuttle[2].x,shuttle[3].x);
    y_min=min(shuttle[0].y,shuttle[1].y,shuttle[2].y,shuttle[3].y);
    y_max=max(shuttle[0].y,shuttle[1].y,shuttle[2].y,shuttle[3].y);
    x_mid=0.5*(x_min+x_max);
    y_mid=0.5*(y_min+y_max);
}/* update_midpoint */

void rotate_left(FILE * sketch){
    double x_temp,y_temp;
    update_midpoint();
    /*erase the original ship*/
    fprintf(sketch,"eraseSegment %ld %ld %ld %ld\n",lround(shuttle[0].x),lround(shuttle[0].y),lround(shuttle[1].x),lround(shuttle[1].y));
    fprintf(sketch,"eraseSegment %ld %ld %ld %ld\n",lround(shuttle[1].x),lround(shuttle[1].y),lround(shuttle[2].x),lround(shuttle[2].y));
    fprintf(sketch,"eraseSegment %ld %ld %ld %ld\n",lround(shuttle[2].x),lround(shuttle[2].y),lround(shuttle[3].x),lround(shuttle[3].y));
    fprintf(sketch,"eraseSegment %ld %ld %ld %ld\n",lround(shuttle[3].x),lround(shuttle[3].y),lround(shuttle[0].x),lround(shuttle[0].y));
    /* update angle of the space ship */
    angle-=10.0;
    angle_rad=angle*PI/180.0;
    for (int i=0;i<4;i++){
        x_temp=shuttle[i].x-x_mid;
	y_temp=shuttle[i].y-y_mid;
	shuttle[i].x=x_temp*cos(-10.0/180.0)-y_temp*sin(-10.0/180.0)+x_mid;
	shuttle[i].y=x_temp*sin(-10.0/180.0)+y_temp*cos(-10.0/180.0)+y_mid;
    }
    drawship(sketch);
    for (int j=0;j<2;j++){
        x_temp=thrust[j].x-x_mid;
	y_temp=thrust[j].y-y_mid;
	thrust[j].x=x_temp*cos(-10.0/180.0)-y_temp*sin(-10.0/180.0)+x_mid;
	thrust[j].y=x_temp*sin(-10.0/180.0)+y_temp*cos(-10.0/180.0)+y_mid;
    }
}/* rotate_left */

void rotate_right(FILE * sketch){
    double x_temp,y_temp;
    update_midpoint();
    /*erase the original ship*/
    fprintf(sketch,"eraseSegment %ld %ld %ld %ld\n",lround(shuttle[0].x),lround(shuttle[0].y),lround(shuttle[1].x),lround(shuttle[1].y));
    fprintf(sketch,"eraseSegment %ld %ld %ld %ld\n",lround(shuttle[1].x),lround(shuttle[1].y),lround(shuttle[2].x),lround(shuttle[2].y));
    fprintf(sketch,"eraseSegment %ld %ld %ld %ld\n",lround(shuttle[2].x),lround(shuttle[2].y),lround(shuttle[3].x),lround(shuttle[3].y));
    fprintf(sketch,"eraseSegment %ld %ld %ld %ld\n",lround(shuttle[3].x),lround(shuttle[3].y),lround(shuttle[0].x),lround(shuttle[0].y));
    /* update angle of the space ship */
    angle+=10.0;
    angle_rad=angle*PI/180.0;
    for (int i=0;i<4;i++){
        x_temp=shuttle[i].x-x_mid;
	y_temp=shuttle[i].y-y_mid;
	shuttle[i].x=x_temp*cos(10.0/180.0)-y_temp*sin(10.0/180.0)+x_mid;
	shuttle[i].y=x_temp*sin(10.0/180.0)+y_temp*cos(10.0/180.0)+y_mid;
    }
    drawship(sketch);
    for (int j=0;j<2;j++){
        x_temp=thrust[j].x-x_mid;
	y_temp=thrust[j].y-y_mid;
	thrust[j].x=x_temp*cos(10.0/180.0)-y_temp*sin(10.0/180.0)+x_mid;
	thrust[j].y=x_temp*sin(10.0/180.0)+y_temp*cos(10.0/180.0)+y_mid;
    }
}/* rotate_right */
