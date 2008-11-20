/* 
 * File:   Depricated.h
 * Author: jhoelscher
 *
 * Created on 17. November 2008, 12:18
 */

#ifndef _DEPRICATED_H
#define	_DEPRICATED_H


#ifdef __GNUC__
#define DEPRECATED(func) func __attribute__ ((__deprecated__))
#elif defined(_MSC_VER)
#define DEPRECATED(func) __declspec(deprecated) func
#else
#warning unknown compiler for depricated Macro
#define DEPRECATED(func) func
#endif
#endif	/* _DEPRICATED_H */

