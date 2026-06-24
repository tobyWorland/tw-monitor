#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#define BIT(N)                   (1 << N)
#define ARR_LEN(ARR)             (sizeof(ARR) / sizeof(ARR[0]))
#define MIN(A, B)                ((A) < (B) ? (A) : (B))
#define OFFSET_OF(STRUCT, FIELD) ((unsigned)&((STRUCT*)NULL)->FIELD)
#define __BKPT(IMM)              __asm("bkpt " #IMM)

#define MHz(X)                   ((X) * 1000000UL)

struct size {
    unsigned width;
    unsigned height;
};

struct position {
    unsigned x;
    unsigned y;
};

#endif
