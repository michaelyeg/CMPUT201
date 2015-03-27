#define MIN_BUF 256 
#define MAX_BUF 2048 
double delta_x, delta_y;
double x1,x2,y1,y2;
double tx,ty;

char Buffer[ MAX_BUF ]; 
char word[ MIN_BUF ]; 

struct image{
  char name;
  double x[100],y[100];
  int size;
};



void parseFile( FILE * fp, char * fname, FILE * sketch); 
FILE *popen(char *command, const char *type);
int pclose(FILE *stream);
