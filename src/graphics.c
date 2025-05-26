#include "./graphics.h"
#include <stdio.h>
#include <stdlib.h>

void *loadbmpimg(const char *path)
{
    long size;
    void *image;
    FILE *file = fopen(path, "r");

    if (!file)
    {
        printf("Could not open file %s\n", path);
        return 0;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);

    image = malloc(size);
    fgets(image, sizeof(image), file);

    fclose(file);

    if (!image)
    {
        printf("Could not read from file\n");
        return 0;
    }

    return image;
}
