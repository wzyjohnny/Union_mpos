#ifndef SYS_TIMER_H_
#define SYS_TIMER_H_

#include <stdint.h>

/************************************************************************
 * Software Timer Interface
 ************************************************************************/
typedef void (*timer_func_t)(void);
typedef struct _timer_t {
    struct _timer_t * next;
    uint32_t halt;
    uint32_t timeout;
    uint32_t curr;
    timer_func_t handler;
}timer_t;
void timer_add(timer_t * tm, uint32_t timeout, timer_func_t handler);
void timer_start(timer_t * tm);
void timer_stop(timer_t *tm);
void timer_del(timer_t * tm);

#endif
