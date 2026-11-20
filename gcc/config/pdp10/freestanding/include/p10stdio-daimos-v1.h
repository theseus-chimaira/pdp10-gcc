#ifndef PDP10_STDIO_DAIMOS_V1_H
#define PDP10_STDIO_DAIMOS_V1_H

/* DAIMOS-only real WRITECHAR console API for GCC.
 *
 * Include the ordinary freestanding stdio.h for the independent
 * buffer-formatting functions.  Link the explicit DAIMOS console
 * archive and the v2 formatter archive, together with the genuine
 * DAIMOS dsys_writechar syscall veneer.
 */
#include <stdio.h>

int putchar(int character);
int puts(const char *text);
int printf(const char *format, ...);
int vprintf(const char *format, __gnuc_va_list arguments);

#endif
