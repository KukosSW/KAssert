#ifndef KASSERT_PRIMITIVE_FMT_H
#define KASSERT_PRIMITIVE_FMT_H

/*
    This is a private header for kassert.
    Do not include it directly

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL3
*/

#ifndef KASSERT_H
#error "Never include <kassert/kassert-primitive-fmt.h> directly, use <kassert/kassert.h> instead."
#endif

/**
 * This macro returning printf fmt for variable x
 */
#define KASSERT_PRIMTIVE_GET_FMT(x) \
    _Generic((x), \
             _Bool: "%u", \
             char: "%c", \
             signed char: "%c", \
             unsigned char: "%c", \
             short: "%hd", \
             unsigned short: "%hu", \
             int: "%d", \
             unsigned int: "%u", \
             long: "%ld", \
             unsigned long: "%lu", \
             long long: "%lld", \
             unsigned long long: "%llu", \
             float: "%f", \
             double: "%lf", \
             long double: "%Lf", \
             default: "%p" \
    )

typedef enum KASSERT_PRIMITIVES
{
    KASSERT_PRIMITIVES_BOOL,
    KASSERT_PRIMITIVES_CHAR,
    KASSERT_PRIMITIVES_SIGNED_CHAR,
    KASSERT_PRIMITIVES_UNSIGNED_CHAR,
    KASSERT_PRIMITIVES_SHORT,
    KASSERT_PRIMITIVES_UNSIGNED_SHORT,
    KASSERT_PRIMITIVES_INT,
    KASSERT_PRIMITIVES_UNSIGNED_INT,
    KASSERT_PRIMITIVES_LONG,
    KASSERT_PRIMITIVES_UNSIGNED_LONG,
    KASSERT_PRIMITIVES_LONG_LONG,
    KASSERT_PRIMITIVES_UNSIGNED_LONG_LONG,
    KASSERT_PRIMITIVES_FLOAT,
    KASSERT_PRIMITIVES_DOUBLE,
    KASSERT_PRIMITIVES_LONG_DOUBLE,
    KASSERT_PRIMITIVES_NON_PRIMITIVE
} KASSERT_PRIMITIVES;

#define KASSERT_PRIMITIVE_GET_TYPE(x) \
    _Generic((x), \
             _Bool: KASSERT_PRIMITIVES_BOOL, \
             char: KASSERT_PRIMITIVES_CHAR, \
             signed char: KASSERT_PRIMITIVES_SIGNED_CHAR, \
             unsigned char: KASSERT_PRIMITIVES_UNSIGNED_CHAR, \
             short: KASSERT_PRIMITIVES_SHORT, \
             unsigned short: KASSERT_PRIMITIVES_UNSIGNED_SHORT, \
             int: KASSERT_PRIMITIVES_INT, \
             unsigned int: KASSERT_PRIMITIVES_UNSIGNED_INT, \
             long: KASSERT_PRIMITIVES_LONG, \
             unsigned long: KASSERT_PRIMITIVES_UNSIGNED_LONG, \
             long long: KASSERT_PRIMITIVES_LONG_LONG, \
             unsigned long long: KASSERT_PRIMITIVES_UNSIGNED_LONG_LONG, \
             float: KASSERT_PRIMITIVES_FLOAT, \
             double: KASSERT_PRIMITIVES_DOUBLE, \
             long double: KASSERT_PRIMITIVES_LONG_DOUBLE, \
             default: KASSERT_PRIMITIVES_NON_PRIMITIVE \
    )

#define KASSERT_PRIMITIVES_PROBABLY_POINTER(x) (KASSERT_PRIMITIVE_GET_TYPE(x) == KASSERT_PRIMITIVES_NON_PRIMITIVE && sizeof(x) == sizeof(void *))


#endif
