#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <execinfo.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdint.h>

#include <kassert/kassert.h>

#define EXIT()             exit(1)
#define CALLSTACK_SIZE_MAX 256

static void __kassert_print_backtrace(void);
static void __kassert_print_threadid(void);

static void __kassert_print_backtrace(void)
{
    void* callstack[CALLSTACK_SIZE_MAX];
    int frames = backtrace(callstack, CALLSTACK_SIZE_MAX);
    if (frames < 0)
        return;

    char** strs = backtrace_symbols(callstack, frames);
    if (strs == NULL)
        return;

    fprintf(stderr, "Stacktrace:\n");
    for (int i = 0; i < frames; ++i)
        fprintf(stderr, "%s\n", strs[i]);

    free(strs);
}

static void __kassert_print_threadid(void)
{
    pid_t id = (pid_t)syscall(__NR_gettid);

    fprintf(stderr, "ThreadID: %d\n", id);
}

/***** GLOBAL FUNCTIONS *****/
void __attribute__(( format(printf, 2, 3) )) __attribute__ ((noreturn))
__kassert_print_and_exit(bool need_free, const char* fmt, ...)
{
    /* PRINT assertion */
    va_list args;
    va_start(args, fmt);

    vfprintf(stderr, fmt, args);
    va_end(args);

    /* fmt is in heap not in rodata / text section */
    if (need_free)
        free((void *)(uintptr_t)fmt); /* bad cast, but I want to keep const char* fmt like in printf */

    /* PRINT ThreadID */
    __kassert_print_threadid();

    /* PRINT backtrace */
    __kassert_print_backtrace();

    /* exit instead of abort to clean program properly */
    EXIT();
}

/* We need to create printf fmt from "dynamic" partial format */
const char* __kassert_create_print_fmt(const char* label,
                                       const char* val1_fmt,
                                       const char* op,
                                       const char* val2_fmt)
{
    /* Calculate how many chars we need to write as fmt */
    size_t len = (size_t)snprintf(NULL,
                                  0,
                                  "%%s:%%d: %%s: Assertion \'%s\' failed. (%s %s %s)\\n",
                                  label,
                                  val1_fmt,
                                  op,
                                  val2_fmt);

    /* Len calculated so alloc it, it will protect us against multithreading (static buffor is a bad idea) */
    char* buf = malloc(len);
    snprintf(buf,
             len,
             "%%s:%%d: %%s: Assertion \'%s\' failed. (%s %s %s)\n",
             label,
             val1_fmt,
             op,
             val2_fmt);

    return buf;
}
