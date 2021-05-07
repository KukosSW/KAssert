#ifndef KASSERT_DIAG_H
#define KASSERT_DIAG_H

/*
    This is a private header for kassert.
    Do not include it directly

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL3
*/

#ifndef KASSERT_H
#error "Never include <kassert/kassert-diag.h> directly, use <kassert/kassert.h> instead."
#endif

#ifndef TOSTRING
#define KASSERT_TOSTRING(x) #x
#define TOSTRING(x) KASSERT_TOSTRING(x)
#endif

#ifdef __clang__

#define KASSERT_DIAG_CLANG(msg)       _Pragma(TOSTRING(clang diagnostic msg))
#define KASSERT_DIAG_PUSH()           KASSERT_DIAG_CLANG(push)
#define KASSERT_DIAG_POP()            KASSERT_DIAG_CLANG(pop)
#define KASSERT_DIAG_WARNING(str)     KASSERT_DIAG_CLANG(warning str)
#define KASSERT_DIAG_ERROR(str)       KASSERT_DIAG_CLANG(error str)
#define KASSERT_DIAG_IGNORE(str)      KASSERT_DIAG_CLANG(ignored str)

#elif defined(__GNUC__)

#define KASSERT_DIAG_GCC(msg)         _Pragma(TOSTRING(GCC diagnostic msg))
#define KASSERT_DIAG_PUSH()           KASSERT_DIAG_GCC(push)
#define KASSERT_DIAG_POP()            KASSERT_DIAG_GCC(pop)
#define KASSERT_DIAG_WARNING(str)     KASSERT_DIAG_GCC(warning str)
#define KASSERT_DIAG_ERROR(str)       KASSERT_DIAG_GCC(error str)
#define KASSERT_DIAG_IGNORE(str)      KASSERT_DIAG_GCC(ignored str)

#else

#define KASSERT_DIAG_PUSH()
#define KASSERT_DIAG_POP()
#define KASSERT_DIAG_WARNING(str)
#define KASSERT_DIAG_ERROR(str)
#define KASSERT_DIAG_IGNORE(str)

#endif

#endif
