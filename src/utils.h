#pragma once

#define VEC struct vec

struct vec
{
    int idx;
    void **elems;
    int capacity;
};

VEC createvec(int capacity);
void *vecget(VEC *v, int idx);
void pushvec(VEC *v, void *element);
void rmvec(VEC *v, int idx);
unsigned int veclength(VEC *v);
void clrvec(VEC *v);
