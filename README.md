# KASSERT - Kukos's assertion


### Getting started:
Before you use this code I suggest to see, compile and play with examples which can be found in this repo (example directory)

To compile examples type:

````
$make all
````

or

````
$make example
````

## Features
* Good Verbose mode during assertions which can print every primitive variables using proper printf format.
* Getting useful information before exit program, like ThreadID, StackTrace. Lack of those informations is a main drawback of normal assert in C. Please note that full stacktrace can be printed only if program is compiled with **-rdynamic** flag.
* Macros family with trivial relations (=, !=, <, <=, >, >=) and also macro for general condition like normal assert.
* Checking types of variables in compile time. When code will compile you are sure that assertion has no type side effect like promotion.
* Checking functions returned value in the same way as normal variable. You don't need to create temporary variable to pass to macro. Macro will analyze type of returned value and work on this as on normal variable.
* Define to disable assertion. You can disable KAassert using NDEBUG like in normal assert
* Define to use KAssert instead of normal assert. KASSERT_EVERYWHERE does this.

## Platforms
For now KAssert has been tested only on Linux.

## Requirements
* Compiler with GnuC dialect and at least C11 standard
* Makefile

## How to build

You can type make help to get list of all targets:
````
KAssert Makefile

Targets:
    all               - build kassert and examples
    lib               - build only kassert library
    examples          - examples
    install[P = Path] - install kassert to path P or default Path

Makefile supports Verbose mode when V=1
To check default compiler (gcc) change CC variable (i.e export CC=clang)
````
## How to install
To install KAssert on your computer you can use

````
make install [P=path]
````
or you can execute script directly from

````
scripts/install_kassert.sh [P=path]
````

Let's assume that you have your project MyProject and directories as follows:
```
.
├── app
├── external
├── inc
├── src
└── tests
````

You want to install KAssert in external/Kassert directory.

````
1. Download this repo
2. $make install P=/home/$user/MyProject/external/Kassert
3. Now you need to link include files and libkassert.a file, you can add to your compile this options
-I/home/$user/MyProject/external/Kassert/inc -L/home/$user/MyProject/external/Kassert -lkassert
4. You can pass to compiler -rdynamic to get better Stacktrace prints.
5. You can pass to compiler some defines using -D option. -DNDEBUG disables assertions like in normal assert, -DKASSERT_EVERYWHERE will change normal assert for KASSERT in your code
6. In your files you need include main header: #include <kassert/kassert.h>
7. Write your code, add assertions and enjoy! Please see examples for details.
````

## How to use
````c
/*
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
````

## Example
````c
#include <stdio.h>
#include <assert.h>

/* We want to use KASSERT(cond) instead of assert */
#define KASSERT_EVERYWHERE
#include <kassert/kassert.h>

static int get41(void);

static void example1(void);
static void example2(void);

/* To shows that we can use functions like variables in KASSERT */
static int get41(void)
{
    return 41;
}

/* Please note, that KASSERT terminates program, so comment assert to see another */
static void example1(void)
{
    double a = 5.0;
    double b = 4.0;

    /* You can pass 2 variables with the same type */
    KASSERT_EQ(a, b);

    int c = 100;
    int d = 100;
    KASSERT_NEQ(c, d);

    /* You can pass variable with normal value but be aware of promotion is C */
    char e = 'E';
    KASSERT_GT(e, (char)'K');

    /* You can pass variable and function which returns the same type */
    int f = 100;
    KASSERT_GEQ(get41(), f);

    KASSERT_LT(200, get41());

    /* You can pass expressions but type must be the same */
    KASSERT_LEQ(get41() + 1, get41());

    /*
        true / false is not a bool :(, that's why type checking is disabling a little bit for booleans
        When one of the variable is bool then other variable have to be bool
        When one of the variable is bool then constant value can be 1 (true) or 0 (false)
    */
    bool b_val = false;
    KASSERT_EQ(b_val, true);
    /* KASSERT_EQ(b_val, 100); */
}

/* Please note, that KASSERT terminates program, so comment assert to see another */
static void example2(void)
{
    int* ptr = NULL;
    /* this assert is in fact KASSERT due to KASSERT_EVERYWHERE */
    assert(ptr != NULL);

    /* You can pass here all you want, like to normal assert */
    KASSERT(ptr != NULL && *ptr != 0);

    /* Pointers have disabled type checking. Pointer is a pointer :) */
    KASSERT_EQ(ptr, NULL);

    /* Cannot compare pointer with non-pointer types */
    /* KASSERT_EQ(t, 0xdead); */

    KASSERT_PTR_NOT_NULL(ptr);
    KASSERT_PTR_NULL(ptr);

    /* You can also check pointer to function as well */
    typedef void (*func_f)(int a, int b);
    func_f func = NULL;
    KASSERT_PTR_NULL(func);
}

int main(void)
{
    example1();
    example2();

    return 0;
}
````

## Full Example
````c
/* main.c */
#include <kassert/kassert.h>

void f(int n);
void g(int n);
void h(int n);

void h(int n)
{
	KASSERT_EQ(n, 10);
}

void g(int n)
{
	h(n);
}

void f(int n)
{
	g(n);
}

int main(void)
{
	f(100);

	return 0;
}

/*
    Command to compile (you can use clang)
        gcc -std=gnu17 -Lpath/to/kassert -Ipath/to/kassert/inc main.c -lkassert -rdynamic -o main.out

    Output from ./main.out
        main.c:9: h: Assertion 'n == 10' failed. (100 == 10)
        ThreadID: 737185
        Stacktrace:
        ./main(+0x13c0) [0x55ddf40fa3c0]
        ./main(__kassert_print_and_exit+0xe0) [0x55ddf40fa540]
        ./main(h+0x91) [0x55ddf40fa31a]
        ./main(g+0x19) [0x55ddf40fa34a]
        ./main(f+0x19) [0x55ddf40fa366]
        ./main(main+0x12) [0x55ddf40fa37b]
        /lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf3) [0x7fe5b45f30b3]
        ./main(_start+0x2e) [0x55ddf40fa1ce]
*/
````

#### Contact
email: michalkukowski10@gmail.com
