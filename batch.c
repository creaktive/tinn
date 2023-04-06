#include "Tinn.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

// Returns the number of lines in a file.
static int lns(FILE* const file)
{
    int ch = EOF;
    int lines = 0;
    int pc = '\n';
    while((ch = getc(file)) != EOF)
    {
        if(ch == '\n')
            lines++;
        pc = ch;
    }
    if(pc != '\n')
        lines++;
    rewind(file);
    return lines;
}

// Reads a line from a file.
static char* readln(FILE* const file)
{
    int ch = EOF;
    int reads = 0;
    int size = 2048;
    char* line = (char*) malloc((size) * sizeof(char));
    while((ch = getc(file)) != '\n' && ch != EOF)
    {
        line[reads++] = ch;
        if(reads + 1 == size)
            line = (char*) realloc((line), (size *= 2) * sizeof(char));
    }
    line[reads] = '\0';
    return line;
}

// Gets one row of inputs and outputs from a string.
static void parse(float* data, char* line, const int cols)
{
    for(int col = 0; col < cols; col++)
    {
        const float val = atof(strtok(col == 0 ? line : NULL, " "));
        data[col] = val;
    }
}

// Learns and predicts hand written digits with 98% accuracy.
int main(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("Usage: %s file.dat\n", argv[0]);
        exit(0);
    }

    sranddev();

    const Tinn loaded = xtload("saved.tinn");

    const char* path = argv[1];
    float* in = (float*) malloc(loaded.nips * sizeof(float));
    FILE* file = fopen(path, "r");
    if(file == NULL)
    {
        printf("Could not open %s\n", path);
        exit(1);
    }
    const int rows = lns(file);

    printf("P2\n%d\n%d\n255\n", loaded.nips + loaded.nops, rows);
    for(int row = 0; row < rows; row++)
    {
        char* line = readln(file);
        parse(in, line, loaded.nips);
        const float* const pd = xtpredict(loaded, in);
        for(int i = 0; i < loaded.nips; i++)
            printf("%d ", (unsigned char) (255 * in[i]));
        for(int i = 0; i < loaded.nops; i++)
            printf("%d ", (unsigned char) (255 * pd[i]));
        printf("\n");
        free(line);
    }
    fclose(file);

    // All done. Let's clean up.
    xtfree(loaded);
    free(in);
    return 0;
}
