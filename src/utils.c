#include "./utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

VEC createvec(int capacity)
{
    VEC v;
    v.idx = 0;
    v.capacity = capacity * sizeof(void *);
    v.elems = malloc(capacity * sizeof(void *));

    return v;
}

void pushvec(VEC *v, void *element)
{
    if (v->idx >= v->capacity)
    {
        v->capacity *= 2;
        v->elems = realloc(v->elems, v->capacity);

        if (v->elems == NULL)
        {
            fprintf(stderr, "Failed to reallocate vector\n");
        }
    }

    v->elems[v->idx / sizeof(void *)] = element;
    v->idx += sizeof(void *);
}

void *vecget(VEC *v, int idx)
{
    return v->elems[idx];
}

void rmvec(VEC *v, int idx)
{
    memmove(&v->elems[idx], &v->elems[idx + 1], veclength(v) * sizeof(void *) - idx * sizeof(void *) - sizeof(void *));
    v->idx -= sizeof(void *);
}

void clrvec(VEC *v)
{
    free(v->elems);
}

unsigned int veclength(VEC *v)
{
    return (v->idx / sizeof(void *));
}
