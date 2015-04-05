#define MIN_BUF 256 
#define MAX_BUF 2048 
#define PI acos(-1.0)


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
