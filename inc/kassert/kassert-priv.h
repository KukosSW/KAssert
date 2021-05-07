#ifndef KASSERT_PRIV_H
#define KASSERT_PRIV_H

/*
    This is the private header for the KAssert.
    Do not include it directly

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com
    LICENCE: GPL3
*/

#ifndef KASSERT_H
#error "Never include <kassert/kassert-priv.h> directly, use <kassert/kassert.h> instead."
#endif

#ifndef __GNUC__
#error "Gnu extension is required to compile code with KAssert!"
#endif

/* C11 has value 201112L */
#if __STDC_VERSION__ < 201112L
#error "At least C11 is required to compile code with KAssert!"
#endif

#include "kassert-primitive-fmt.h"
#include "kassert-diag.h"

#include <stdbool.h>

#ifndef TOSTRING
#define KASSERT_TOSTRING(x) #x
#define TOSTRING(x) KASSERT_TOSTRING(x)
#endif

/* Main assert function: prints all stats like format, threadID, stacktrace and calls exit(1) */
void __attribute__ ((noreturn)) __kassert_print_and_exit(bool need_free, const char* fmt, ...);

/* Returns pointer to char[] with printf format, buffer must be freed */
const char* __kassert_create_print_fmt(const char* label,
                                       const char* val1_fmt,
                                       const char* op,
                                       const char* val2_fmt);

#define KASSERT_PRIV_COND_BODY(cond) \
    do { \
        if (!(cond)) \
        { \
            __kassert_print_and_exit(false, \
                                     "%s:%d: %s: Assertion \'%s\' failed.\n", \
                                     __FILE__, \
                                     __LINE__, \
                                     __func__, \
                                     TOSTRING(cond)); \
        } \
    } while (0)

#define KASSERT_PRIV_CREATE_LABEL(val1, val2, op) \
    TOSTRING(val1) " " TOSTRING(op) " " TOSTRING(val2)


/*
    Macro is using strict type checking with some exceptions:
    1. If both types are pointers then pointer are compatible. Because for comparision we can use just void*
    2. If one of the variable is a bool variable then
        2.1 Second val is a variable, then make type checking
        2.2 Second val is a const value, then check if can be converted to bool (is equal 0 or 1)
        2.3 First value is a const value and second value is a const value, they cannot by bool, because of promotion to int

    Plese note that _Static_assert need all of branches be compile expressions.
    That's why I use val1 / val2 instead of _ktest_val. But no worries. __typeof__ / sizeof are safe
    ++i, i++ will be ignored under those compiler "operators"
*/
#define KASSERT_PRIV_OP(val1, val2, op) \
    do { \
        KASSERT_DIAG_PUSH() \
        KASSERT_DIAG_IGNORE("-Wfloat-equal") \
        KASSERT_DIAG_IGNORE("-Wint-to-pointer-cast") \
        const __typeof__(val1) _kassert_val1 = (val1); \
        const __typeof__(val2) _kassert_val2 = (val2); \
        _Static_assert(__builtin_choose_expr(KASSERT_PRIMITIVES_PROBABLY_POINTER(_kassert_val1) && KASSERT_PRIMITIVES_PROBABLY_POINTER(_kassert_val2), \
                                             1, \
                                             __builtin_choose_expr(KASSERT_PRIMITIVE_GET_TYPE(_kassert_val1) == KASSERT_PRIMITIVES_BOOL || KASSERT_PRIMITIVE_GET_TYPE(_kassert_val2) == KASSERT_PRIMITIVES_BOOL, \
                                                                   1, \
                                                                   __builtin_types_compatible_p(__typeof__(val1), __typeof__(val2)))), \
                       "Uncompatible types"); \
        _Static_assert(__builtin_choose_expr(KASSERT_PRIMITIVE_GET_TYPE(_kassert_val1) == KASSERT_PRIMITIVES_BOOL || KASSERT_PRIMITIVE_GET_TYPE(_kassert_val2) == KASSERT_PRIMITIVES_BOOL, \
                                             __builtin_choose_expr(__builtin_constant_p(val1), \
                                                                   val1 == (__typeof__(val1))false || val1 == (__typeof__(val1))true, \
                                                                   __builtin_choose_expr(__builtin_constant_p(val2), \
                                                                                         val2 == (__typeof__(val2))false || val2 == (__typeof__(val2))true, \
                                                                                         1) \
                                                                  ), \
                                             1), \
                           "Implicit convertion to bool"); \
        if (!((__builtin_choose_expr(KASSERT_PRIMITIVES_PROBABLY_POINTER(_kassert_val1), (void *)(long)_kassert_val1, _kassert_val1)) op (__builtin_choose_expr(KASSERT_PRIMITIVES_PROBABLY_POINTER(_kassert_val2), (void *)(long)_kassert_val2, _kassert_val2)))) \
        { \
            const char* const fmt = \
                __kassert_create_print_fmt(KASSERT_PRIV_CREATE_LABEL(val1, val2, op), \
                                           KASSERT_PRIMTIVE_GET_FMT(_kassert_val1), \
                                           TOSTRING(op), \
                                           KASSERT_PRIMTIVE_GET_FMT(_kassert_val2)); \
            __kassert_print_and_exit(true, \
                                     fmt, \
                                     __FILE__, \
                                     __LINE__, \
                                     __func__, \
                                     _kassert_val1, \
                                     _kassert_val2); \
        } \
        KASSERT_DIAG_POP() \
    } while (0)

#define KASSERT_PRIV_EQ(val1, val2)  KASSERT_PRIV_OP(val1, val2, ==)
#define KASSERT_PRIV_NEQ(val1, val2) KASSERT_PRIV_OP(val1, val2, !=)
#define KASSERT_PRIV_LT(val1, val2)  KASSERT_PRIV_OP(val1, val2, <)
#define KASSERT_PRIV_LEQ(val1, val2) KASSERT_PRIV_OP(val1, val2, <=)
#define KASSERT_PRIV_GT(val1, val2)  KASSERT_PRIV_OP(val1, val2, >)
#define KASSERT_PRIV_GEQ(val1, val2) KASSERT_PRIV_OP(val1, val2, >=)

#define KASSERT_PRIV_COND(cond)      KASSERT_PRIV_COND_BODY(cond)

#endif
