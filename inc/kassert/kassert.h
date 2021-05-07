#ifndef KASSERT_H
#define KASSERT_H

/*
    This is the main header of the KAssert, you shall include only this header.

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com
    LICENCE: GPL3

    KAssert = Kukos's assertion developed in C for better alerts during assertsions.
    The main drawbacks of assert from assert.h are:
    -ugly print like assertions ptr != NULL failed
    -no info about threadID
    -no info about stack, it will be better to see stacktrace like in gdb
    -no type checking in compile time

    This Library fixes all those drawbacks.
    By introducing a set of macros instead of one macro library can deduct which type is used
    and choose proper printf format. That's why I recommend to use KASSERT_NEQ(ptr, NULL)
    instead of KASSERT(ptr != NULL). Of course I understand that sometime we want to protect funtion
    using single assertion. OK, you can still pass single multistatemented condition to KASEERT.

    Like assert from assert.h KAssert also have define to disable code for release versions.
    You can use the same define: NDEBUG. When NDEBUG is defined KAssert does nothing.
    Also you can introduce KAssert to your project that have already assertions.
    Just pass to compile define: KASSERT_EVERYWHERE with this define all assert will be replaced by KASSERT.

    Please note that performance of assert from assert.h and KASSERT is the same.
    All needed data are fetched and processed when asserion fails. This means that in correct situation
    Both libraries evaluate only condition and do nothing more.
    When condition fails we are going to close program, so performance is no critical here.

    Please note that KAssert uses exit(1) instead of abort. So this library lets program to clean itself
    before closing.

    Remember. To get full info from StackTrace linker need to add some special debug symbols.
    To see not only address of functions but names also you need to add to compiler -rdynamic (gcc)
*/

#include "kassert-priv.h"

/**
 * Like normal assert from assert.h. When NDEBUG is defined then assertions are empty
*/
#ifndef NDEBUG

/**
 * Those set of macros are recommended! They can deduct types of variables (and functions)
 * and use proper printf format. The good side effect is also a type checking in compile time.
 * When variable will be promoted to another type, you will see an error :)
 *
 * Use this macros to check relation between 2 values
 * Relations:
 * EQ  -> val1 == val2
 * NEQ -> val1 != val2
 * LT  -> val1 <  val2
 * LEQ -> val1 <= val2
 * GT  -> val1 >  val2
 * GEQ -> val1 >= val2
 *
 * If condition (relation) is evaluated to false then library will produce
 * and error message on stderr and will terminate program using exit(1) command.
 *
 * The example of output can be like this (with -rdynamic):
 * main.c:9: h: Assertion 'n == 10' failed. (100 == 10)
 * ThreadID: 739210
 * Stacktrace:
 * ./main(+0x13c0) [0x55f99f4573c0]
 * ./main(__kassert_print_and_exit+0xe0) [0x55f99f457540]
 * ./main(h+0x91) [0x55f99f45731a]
 * ./main(g+0x19) [0x55f99f45734a]
 * ./main(f+0x19) [0x55f99f457366]
 * ./main(main+0x12) [0x55f99f45737b]
 * /lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf3) [0x7f9ca0f7d0b3]
 * ./main(_start+0x2e) [0x55f99f4571ce
 */
#define KASSERT_EQ(val1, val2)    KASSERT_PRIV_EQ(val1, val2)
#define KASSERT_NEQ(val1, val2)   KASSERT_PRIV_NEQ(val1, val2)
#define KASSERT_GT(val1, val2)    KASSERT_PRIV_GT(val1, val2)
#define KASSERT_GEQ(val1, val2)   KASSERT_PRIV_GEQ(val1, val2)
#define KASSERT_LT(val1, val2)    KASSERT_PRIV_LT(val1, val2)
#define KASSERT_LEQ(val1, val2)   KASSERT_PRIV_LEQ(val1, val2)

/**
 * This works in the same way as normal assert from assert.h
 * It does not enable pretty prints, but still will print ThreadId and StackTrace.
 *
 * If condition (relation) is evaluated to false then library will produce
 * and error message on stderr and will terminate program using exit(1) command.
 *
 * The example of output can be like this (with -rdynamic):
 * main.c:9: h: Assertion 'n == 10' failed.
 * ThreadID: 739272
 * Stacktrace:
 * ./main(+0x13f0) [0x5643089a63f0]
 * ./main(__kassert_print_and_exit+0xe0) [0x5643089a6570]
 * ./main(h+0x54) [0x5643089a62dd]
 * ./main(g+0x19) [0x5643089a6380]
 * ./main(f+0x19) [0x5643089a639c]
 * ./main(main+0x12) [0x5643089a63b1]
 * /lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf3) [0x7f16d28890b3]
 * ./main(_start+0x2e) [0x5643089a61ce]
 */
#define KASSERT(cond)             KASSERT_PRIV_COND(cond)

/**
 * Use this macro to check if pointer is not null
 */
#define KASSERT_PTR_NOT_NULL(ptr) KASSERT_NEQ(ptr, (void *)0)

/**
 * Use this macro to check if pointer is null
 */
#define KASSERT_PTR_NULL(ptr)     KASSERT_EQ(ptr, (void *)0)

/**
 * When you have assert in your code and want to use KASSERT instead of, pass this define to compiler
 */
#ifdef KASSERT_EVERYWHERE

/*
    include assert.h to get assert defined and also get include guard defined.
    So when you have assert.h included before.
    This include does nothing and we can undef assert and define as KASSERT

    But if you have not assert.h included before. Then my aseert can be replaced
    by include assert.h. That's why i include it here, include guard will be included
    and any other includes will do nothing.
*/
#include <assert.h>
#undef assert
#define assert(cond) KASSERT(cond)

#endif /* ifdef KASSERT_EVERYWHERE  */

#else /* ifndef NDEBUG */

/* NDEBUG defined do nothing */
#define KASSERT_EQ(val1, val2)
#define KASSERT_NEQ(val1, val2)
#define KASSERT_GT(val1, val2)
#define KASSERT_GEQ(val1, val2)
#define KASSERT_LT(val1, val2)
#define KASSERT_LEQ(val1, val2)

#define KASSERT(cond)

#define KASSERT_PTR_NOT_NULL(ptr)

#define KASSERT_PTR_NULL(ptr)

#endif /* ifndef NDEBUG */

#endif /* include guard */
