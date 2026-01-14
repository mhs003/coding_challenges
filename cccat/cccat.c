#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FLAGS 50
#define MAX_FILES 50

// signatures
bool hasPrefix(const char *pre, const char *str);
bool array_contains_str(char *arr[], char *target);
void readFile(int *line, char *name, char *flags[]);
void stdReader(int *line, FILE *fptr, char *flags[]);
// ----

int main(int argc, char *argv[])
{
    char *flags[MAX_FLAGS];
    int flagpos = 0;
    char *files[MAX_FILES];
    int filepos = 0;
    int line = 1;

    for (int i = 0; i < argc; i++)
    {
        if (hasPrefix("-", argv[i]))
        {
            if (!argv[i])
                continue;
            if (flagpos >= MAX_FLAGS - 1)
            {
                fprintf(stderr, "Too many flags\n");
                exit(EXIT_FAILURE);
            }

            flags[flagpos] = argv[i];
            flagpos++;
        }
        else
        {
            if (i == 0)
                continue;
            if (filepos >= MAX_FILES - 1)
            {
                fprintf(stderr, "Too many files\n");
                exit(EXIT_FAILURE);
            }

            files[filepos] = argv[i];
            filepos++;
        }
    }

    files[filepos] = NULL;
    flags[flagpos] = NULL;

    if (filepos > 0)
    {
        for (int i = 0; files[i] != NULL; i++)
        {
            if (strlen(files[i]) == 0)

                continue;
            readFile(&line, files[i], flags);
        }
    }
    else
    {
        stdReader(&line, stdin, flags);
    }

    return 0;
}

// signature implementations
bool hasPrefix(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}

bool array_contains_str(char *arr[], char *target)
{
    int i = 0;
    while (arr[i] != NULL)
    {
        if (strcmp(arr[i], target) == 0)
        {
            return true;
        }
        i++;
    }
    return false;
}

void readFile(int *line, char *name, char *flags[])
{
    FILE *fptr;

    fptr = fopen(name, "r");
    if (fptr == NULL)
    {
        printf("Unable to open file '%s'.\n", name);
        exit(EXIT_FAILURE);
    }

    stdReader(line, fptr, flags);
}

void stdReader(int *line, FILE *fptr, char *flags[])
{
    size_t buff_size = 4096;

    char buffer[buff_size];

    bool linenum_enabled = false;
    bool number_nonblank = false;

    if (array_contains_str(flags, "-n"))
    {
        buff_size = buff_size + 32;
        linenum_enabled = true;
    }
    if (array_contains_str(flags, "-b"))
    {
        number_nonblank = true;
    }

    int iline = *line;

    while (fgets(buffer, sizeof buffer, fptr))
    {
        char lout[buff_size];
        if (linenum_enabled || number_nonblank)
        {
            char out[buff_size];
            if (number_nonblank && buffer[0] == '\n')
            {
                strcpy(lout, buffer);
            }
            else
            {
                snprintf(out, sizeof out, "%6d  %s", iline, buffer);
                strcpy(lout, out);
            }
        }
        else
        {
            strcpy(lout, buffer);
        }
        fputs(lout, stdout);
        if (number_nonblank && lout[0] == '\n')
        {
            continue;
        }
        iline++;
    }

    *line = iline;

    fclose(fptr);
}
