/*
 * DAIMOS-only console backend for GCC PDP-10 memory formatter v2.
 *
 * The monitor's WRITECHAR syscall accepts (fd, character) in AC1/AC2
 * and reports zero for a successful write.  No KCC variadic entry
 * points are used.  This module must only be linked with DAIMOS's
 * actual dsys_writechar veneer; the portable formatter does not refer
 * to the monitor and remains usable without an operating system.
 *
 * C89; no heap, allocation, stdout buffer or writable global data.
 */
#include <stdarg.h>

#define P10STDIO_DAIMOS_EOF_V1 (-1)
#define P10STDIO_DAIMOS_STDOUT_V1 1

extern int dsys_writechar(int descriptor, int character);
extern int __p10stdio_vformat_v2(const char *format, va_list arguments,
                                int (*emit)(void *, int), void *context);

static int
p10stdio_daimos_emit_v1(void *context, int character)
{
    (void)context;
    return dsys_writechar(P10STDIO_DAIMOS_STDOUT_V1, character);
}

int
putchar(int character)
{
    int ch = (unsigned char)character;
    if (dsys_writechar(P10STDIO_DAIMOS_STDOUT_V1, ch) != 0)
        return P10STDIO_DAIMOS_EOF_V1;
    return ch;
}

int
puts(const char *text)
{
    if (text == (const char *)0)
        return P10STDIO_DAIMOS_EOF_V1;
    while (*text != 0) {
        if (putchar((unsigned char)*text++) == P10STDIO_DAIMOS_EOF_V1)
            return P10STDIO_DAIMOS_EOF_V1;
    }
    return putchar('\n') == P10STDIO_DAIMOS_EOF_V1 ?
           P10STDIO_DAIMOS_EOF_V1 : 0;
}

int
vprintf(const char *format, va_list arguments)
{
    return __p10stdio_vformat_v2(format, arguments,
                                 p10stdio_daimos_emit_v1, (void *)0);
}

int
printf(const char *format, ...)
{
    va_list args;
    int count;

    va_start(args, format);
    count = vprintf(format, args);
    va_end(args);
    return count;
}
