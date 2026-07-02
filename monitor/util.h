#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#define BIT(N)                   (1 << N)
#define ARR_LEN(ARR)             (sizeof(ARR) / sizeof(ARR[0]))
#define MIN(A, B)                ((A) < (B) ? (A) : (B))
#define OFFSET_OF(STRUCT, FIELD) ((unsigned)&((STRUCT*)NULL)->FIELD)
#define __BKPT(IMM)              __asm("bkpt " #IMM) ///< Breakpoint instruction
#define __WFI()                  __asm("wfi") ///< Wait for interrupts

#define MHz(X)                   ((X) * 1000000UL)

struct size {
    unsigned width;
    unsigned height;
};

struct position {
    unsigned x;
    unsigned y;
};

/**
 * @brief Enter critical section without interrupts
 *
 * Can be nested
 *
 * @see util_leave_critical
 */
void util_enter_critical(void);
/**
 * @brief Leave critical section
 *
 * @see util_enter_critical
 */
void util_leave_critical(void);

#endif
