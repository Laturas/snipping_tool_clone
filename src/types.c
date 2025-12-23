/**
* types.c implemented by Katelyn Doucette
* Utility code that defines some useful shorthand types for ease of use.
*/

#define DEFINITIONS
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;
typedef float f32;
typedef double f64;

#ifndef static_assert
    #include <assert.h>
    static_assert(sizeof(u8) == 1   , "Types are the wrong size from what was expected");
    static_assert(sizeof(u16) == 2  , "Types are the wrong size from what was expected");
    static_assert(sizeof(u32) == 4  , "Types are the wrong size from what was expected");
    static_assert(sizeof(u64) == 8  , "Types are the wrong size from what was expected");
    static_assert(sizeof(i8) == 1   , "Types are the wrong size from what was expected");
    static_assert(sizeof(i16) == 2  , "Types are the wrong size from what was expected");
    static_assert(sizeof(i32) == 4  , "Types are the wrong size from what was expected");
    static_assert(sizeof(i64) == 8  , "Types are the wrong size from what was expected");
    static_assert(sizeof(f32) == 4  , "Types are the wrong size from what was expected");
    static_assert(sizeof(f64) == 8  , "Types are the wrong size from what was expected");
#endif

#define true 1
#define false 0
#define bool char

typedef struct string {
    char* str;
    u64 length;
} String;

typedef struct slice {
    u8* bytes;
    u64 length;
} Slice;