#ifndef PDP10_FREESTANDING_STRING_V1_H
#define PDP10_FREESTANDING_STRING_V1_H

/* ISO C freestanding byte and string operations for the shared PDP-10 ABI.
 * size_t comes from GCC's own target-specific stddef.h; do not borrow
 * KCC's size_t typedef or preprocessor feature macros.
 */
#include <stddef.h>

void *memcpy(void *restrict dst, const void *restrict src, size_t count);
void *memmove(void *dst, const void *src, size_t count);
void *memset(void *dst, int value, size_t count);
int memcmp(const void *a, const void *b, size_t count);
size_t strlen(const char *s);
int strcmp(const char *a, const char *b);
int strncmp(const char *a, const char *b, size_t count);
char *strcpy(char *restrict dst, const char *restrict src);
char *strncpy(char *restrict dst, const char *restrict src, size_t count);
char *strcat(char *restrict dst, const char *restrict src);
char *strncat(char *restrict dst, const char *restrict src, size_t count);
char *strchr(const char *s, int c);
char *strrchr(const char *s, int c);
size_t strcspn(const char *s, const char *reject);
char *strtok(char *restrict s, const char *restrict delim);

#endif
