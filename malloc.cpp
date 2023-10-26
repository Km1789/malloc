#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <stack>

using namespace std;

#define NMAX 10000
#define LMAX 100000

char p[NMAX][LMAX];

int np[NMAX] = {0}; // inittializing with zeroes

char used[NMAX] = {0};

char sf[NMAX][128] = {0};

int il[NMAX];

static int SetFirst = 1;

void CheckMem(void);

void *malloc(size_t n)
{
    if (SetFirst)
    {
        atexit(CheckMem);
        SetFirst = 0;
    }
    int i = 0;
    while(used[i])
    {
        i++;
    }
    np[i] = n;
    used[i] = 1;
    return p[i];
}

void *malloc(size_t n, const char *sf, int il)
{
    if (SetFirst)
    {
        atexit(CheckMem);
        SetFirst = 0;
    }
    int i = 0;
    while(used[i])
    {
        i++;
    }
    np[i] = n;
    used[i] = 1;
    strcpy(::sf[i], sf);
    ::il[i] = il;
    return p[i];
}

void free(void * r)
{
    size_t i = ((char *)r - (char *)p) / LMAX;
    if (i >= NMAX)
    {
        if(0)
        {
            printf("Can't free\n");
        }
    }
    else if (!used[i])
    {
        printf("Free double\n");
    }
    else
    {
        np[i] = 0;
        used[i] = 0;
    }
}

//------

void *operator new(size_t n)
{
    printf("new(%d)\n", (int)n);
    return malloc(n);
}

void *operator new[](size_t n)
{
    printf("new[](%d)\n", (int)n);
    return malloc(n);
}

void *operator new(size_t n, size_t sz)
{
    printf("new(%d, %d)\n", (int)n, (int)sz);
    return malloc(n);
}

void *operator new[](size_t n, size_t sz)
{
    printf("new[](%d, %d)\n", (int)n, (int)sz);
    return malloc(n);
}

//------

void *operator new(size_t n, const char *sf, int il)
{
    printf("new(%d)\n", (int)n);
    return malloc(n, sf, il);
}

void *operator new[](size_t n, const char *sf, int il)
{
    printf("new[](%d)\n", (int)n);
    return malloc(n, sf, il);
}

void *operator new(size_t n, size_t sz, const char *sf, int il)
{
    printf("new(%d, %d)\n", (int)n, (int)sz);
    return malloc(n, sf, il);
}

void *operator new[](size_t n, size_t sz, const char *sf, int il)
{
    printf("new[](%d, %d)\n", (int)n, (int)sz);
    return malloc(n, sf, il);
}

//------

void operator delete(void *p)
{
    printf("delete\n");
    free(p);
}

void operator delete(void *p, size_t sz)
{
    printf("delete(%d)\n", (int)sz);
    free(p);
}

void operator delete[](void *p)
{
    printf("delete[]\n");
    free(p);
}

void operator delete[](void *p, size_t sz)
{
    printf("deletep[](%d)\n", (int)sz);
    free(p);
}

//-----
#define new new(__FILE__, __LINE__)
//-----

void CheckMem(void)
{
    int bad = 0;
    #ifdef __GNUC__
    for (int i = 2; i < NMAX; i++)
    {
        if (used[i])
        {
            bad = 1;
            printf("memory leak: <%s> #%d : %d bytes\n", sf[i], il[i], np[i]);
        }
    }
    #else
    for (int i = 0; i < NMAX; i++)
    {
        if (used[i])
        {
            bad = 1;
            printf("memory leak: <%s> #%d : %d bytes\n", sf[i], il[i], np[i]);
        }
    }
    #endif
    if (!bad)
    {
        printf("No memory leaks\n");
    }
}
