void drawlandscape(FILE * landscape, char * filename, FILE * sketch);
void drawship (FILE * sketch);
void drawthrust (FILE * sketch);
void setup_curses();
void unset_curses();
void update_midpoint();

FILE *popen(char *command, const char *type);
int pclose(FILE *stream);

struct pos{
    double x;
    double y;
};

double x_min,y_min,x_max,y_max,x_mid,y_mid;

double min(double a, double b, double c, double d){
    double min;
    if (a<b){
      min=a;
    }
    else{min=b;}
    if (c<min){min=c;}
    if (d<min){min=d;}
    return min;
}

double max(double a, double b, double c, double d){
    double max;
    if (a<b){max=b;}
    else{max=a;}
    if (c>max){max=c;}
    if (d>max){max=d;}
    return max;
}
