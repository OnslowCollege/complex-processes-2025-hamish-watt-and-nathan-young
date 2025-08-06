#include "./utils.h"
#include <stdlib.h>
#include <string.h>

VEC createvec(int capacity)
{
    VEC v;
    v.idx = 0;
    v.capacity = capacity;
    v.elems = malloc(capacity * sizeof(void *));

    return v;
}

void pushvec(VEC *v, void *element)
{
    if ((v->idx / sizeof(void *)) > v->capacity)
    {
        void *new_mem = malloc(v->capacity * 2 * sizeof(void *));
        v->capacity *= 2;
        memcpy(v->elems, new_mem, v->capacity);
        free(v->elems);

        v->elems = new_mem;
    }

    v->elems[v->idx] = element;
    v->idx += sizeof(void *);
}

void *vecget(VEC *v, int idx)
{
    return v->elems[idx * sizeof(void *)];
}

void rmvec(VEC *v, int idx)
{
    memmove(v->elems + idx * sizeof(void *), v->elems + idx * sizeof(void *) + sizeof(void *),
            veclength(v) * sizeof(void *) - idx * sizeof(void *) - sizeof(void *));
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
