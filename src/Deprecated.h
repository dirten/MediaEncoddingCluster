#ifndef DEPRECATED_H
#define DEPRECATED_H

#ifdef __GNUC__
#define DEPRECATED(func) func __attribute__ ((__deprecated__))
#elif defined(_MSC_VER)
#define DEPRECATED(func) __declspec(deprecated) func
#else
#warning unknown compiler for depricated Macro
#define DEPRECATED(func) func
#endif

#endif

