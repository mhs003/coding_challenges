#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FLAGS 50
#define MAX_FILES 50

// signatures
bool hasPrefix(const char *pre, const char *str);
bool array_contains_str(char *arr[], char *target);
void readFile(char *name, char *flags[]);
// ----

int main(int argc, char *argv[])
{
    char *flags[MAX_FLAGS];
    int flagpos = 0;
    char *files[MAX_FILES];
    int filepos = 0;

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

    for (int i = 0; files[i] != NULL; i++)
    {
        if (strlen(files[i]) == 0)
            continue;
        readFile(files[i], flags);
    }

    return 0;
}

// function implementations
bool hasPrefix(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}

bool array_contains_str(char *arr[], char *target)
{
    for (int i = 0; arr[i] != NULL; i++)
    {
        if (arr[i] == target)
        {
            return true;
        }
    }
    return false;
}

void readFile(char *name, char *flags[])
{
    FILE *fptr;

    fptr = fopen(name, "r");
    if (fptr == NULL)
    {
        printf("Unable to open file '%s'.\n", name);
        exit(EXIT_FAILURE);
    }

    size_t buff_size = 4096;

    char buffer[buff_size];

    int line = 1;
    while (fgets(buffer, sizeof buffer, fptr))
    {
        if (array_contains_str(flags, "-n"))
        {
            char out[buff_size + 32];
            snprintf(out, sizeof out, "%6d  %s", line, buffer);
            fputs(out, stdout);
        }
        else
        {
            fputs(buffer, stdout);
        }
        line++;
    }

    fclose(fptr);
}
