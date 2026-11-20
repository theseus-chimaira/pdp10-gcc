#ifndef PDP10_CTYPE_C_LOCALE_V1_H
#define PDP10_CTYPE_C_LOCALE_V1_H

/*
 * PDP-10 freestanding C-locale character classification, version 1.
 *
 * The narrow C char has nine bits (0..0777).  These real functions
 * accept EOF (-1) or a value representable as unsigned char.  C-locale
 * printable and space characters are ASCII 040..0176 and the usual
 * whitespace controls, respectively.  Values 0200..0777 are not
 * classified as printable or whitespace in the C locale.
 *
 * Other C locale or UTF-9 character properties are not implemented
 * here.  Do not substitute an ASCII-only implementation for UTF-9.
 */
int isprint(int);
int isspace(int);

#endif
