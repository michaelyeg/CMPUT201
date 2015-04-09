void drawlandscape(FILE * landscape, char * filename, FILE * sketch);
void drawship (FILE * sketch);
void drawthrust (FILE * sketch);
void setup_curses();
void unset_curses();

FILE *popen(char *command, const char *type);
int pclose(FILE *stream);

struct pos{
    double x;
    double y;
};

