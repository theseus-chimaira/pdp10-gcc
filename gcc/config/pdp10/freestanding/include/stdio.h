#ifndef PDP10_FREESTANDING_STDIO_V1_H
#define PDP10_FREESTANDING_STDIO_V1_H

/*
 * PDP-10 freestanding buffered formatting (v1).
 *
 * This is NOT a hosted ISO C stdio implementation.  No OS-independent
 * FILE, stdin, stdout, stderr, file descriptors or console I/O exist
 * for pdp10-dec-none yet.  Declare only functions backed by the real
 * libp10stdio-v1.a formatter; do not pretend that stream I/O works.
 *
 * GCC's stdarg.h defines __gnuc_va_list without exporting va_list
 * merely because a translation unit includes stdio.h.
 */
#define __need___va_list
#include <stdarg.h>
#include <stddef.h>

#define EOF (-1)

int sprintf(char *destination, const char *format, ...);
int vsprintf(char *destination, const char *format, __gnuc_va_list arguments);
int snprintf(char *destination, size_t size, const char *format, ...);
int vsnprintf(char *destination, size_t size, const char *format,
              __gnuc_va_list arguments);

#endif
