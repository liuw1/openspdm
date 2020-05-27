/** @file
  Root include file of C runtime library to support building the third-party
  cryptographic library.

Copyright (c) 2010 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __CRT_LIB_SUPPORT_H__
#define __CRT_LIB_SUPPORT_H__

#include <Base.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>

#define OPENSSLDIR ""
#define ENGINESDIR ""

#define MAX_STRING_SIZE  0x1000

//
// We already have "no-ui" in out Configure invocation.
// but the code still fails to compile.
// Ref:  https://github.com/openssl/openssl/issues/8904
//
// This is defined in CRT library(stdio.h).
//
#ifndef BUFSIZ
#define BUFSIZ  8192
#endif

//
// OpenSSL relies on explicit configuration for word size in crypto/bn,
// but we want it to be automatically inferred from the target. So we
// bypass what's in <openssl/opensslconf.h> for OPENSSL_SYS_UEFI, and
// define our own here.
//
#ifdef CONFIG_HEADER_BN_H
#error CONFIG_HEADER_BN_H already defined
#endif

#define CONFIG_HEADER_BN_H

#if defined(MDE_CPU_X64) || defined(MDE_CPU_AARCH64) || defined(MDE_CPU_IA64)
//
// With GCC we would normally use SIXTY_FOUR_BIT_LONG, but MSVC needs
// SIXTY_FOUR_BIT, because 'long' is 32-bit and only 'long long' is
// 64-bit. Since using 'long long' works fine on GCC too, just do that.
//
#define SIXTY_FOUR_BIT
#elif defined(MDE_CPU_IA32) || defined(MDE_CPU_ARM) || defined(MDE_CPU_EBC)
#define THIRTY_TWO_BIT
#else
#error Unknown target architecture
#endif

//
// Map all va_xxxx elements to VA_xxx defined in MdePkg/Include/Base.h
//
#if !defined(__CC_ARM) // if va_list is not already defined
#define va_list   VA_LIST
#define va_arg    VA_ARG
#define va_start  VA_START
#define va_end    VA_END
#else // __CC_ARM
#define va_start(Marker, Parameter)   __va_start(Marker, Parameter)
#define va_arg(Marker, TYPE)          __va_arg(Marker, TYPE)
#define va_end(Marker)                ((void)0)
#endif

//
// Definitions for global constants used by CRT library routines
//
#define EINVAL       22               /* Invalid argument */
#define EAFNOSUPPORT 47               /* Address family not supported by protocol family */
#define INT_MAX      0x7FFFFFFF       /* Maximum (signed) int value */
#define LONG_MAX     0X7FFFFFFFL      /* max value for a long */
#define LONG_MIN     (-LONG_MAX-1)    /* min value for a long */
#define ULONG_MAX    0xFFFFFFFF       /* Maximum unsigned long value */
#define CHAR_BIT     8                /* Number of bits in a char */

//
// Address families.
//
#define AF_INET   2     /* internetwork: UDP, TCP, etc. */
#define AF_INET6  24    /* IP version 6 */

//
// Define constants based on RFC0883, RFC1034, RFC 1035
//
#define NS_INT16SZ    2   /*%< #/bytes of data in a u_int16_t */
#define NS_INADDRSZ   4   /*%< IPv4 T_A */
#define NS_IN6ADDRSZ  16  /*%< IPv6 T_AAAA */

//
// Basic types mapping
//
typedef UINTN          size_t;
typedef UINTN          u_int;
typedef INTN           ssize_t;
typedef INT32          time_t;
typedef UINT8          __uint8_t;
typedef UINT8          sa_family_t;
typedef UINT8          u_char;
typedef UINT32         uid_t;
typedef UINT32         gid_t;

//
// File operations are not required for EFI building,
// so FILE is mapped to VOID * to pass build
//
typedef VOID  *FILE;

//
// Structures Definitions
//
struct tm {
  int   tm_sec;     /* seconds after the minute [0-60] */
  int   tm_min;     /* minutes after the hour [0-59] */
  int   tm_hour;    /* hours since midnight [0-23] */
  int   tm_mday;    /* day of the month [1-31] */
  int   tm_mon;     /* months since January [0-11] */
  int   tm_year;    /* years since 1900 */
  int   tm_wday;    /* days since Sunday [0-6] */
  int   tm_yday;    /* days since January 1 [0-365] */
  int   tm_isdst;   /* Daylight Savings Time flag */
  long  tm_gmtoff;  /* offset from CUT in seconds */
  char  *tm_zone;   /* timezone abbreviation */
};

struct timeval {
  long tv_sec;      /* time value, in seconds */
  long tv_usec;     /* time value, in microseconds */
};

struct sockaddr {
  __uint8_t    sa_len;       /* total length */
  sa_family_t  sa_family;    /* address family */
  char         sa_data[14];  /* actually longer; address value */
};

//
// Global variables
//
extern int  errno;
extern FILE *stderr;

//
// Function prototypes of CRT Library routines
//
void           *malloc     (size_t);
void           *realloc    (void *, size_t);
void           free        (void *);
void           *memset     (void *, int, size_t);
int            memcmp      (const void *, const void *, size_t);
int            isdigit     (int);
int            isspace     (int);
int            isxdigit    (int);
int            isalnum     (int);
int            isupper     (int);
int            tolower     (int);
int            strcmp      (const char *, const char *);
int            strncasecmp (const char *, const char *, size_t);
char           *strchr     (const char *, int);
char           *strrchr    (const char *, int);
unsigned long  strtoul     (const char *, char **, int);
long           strtol      (const char *, char **, int);
char           *strerror   (int);
size_t         strspn      (const char *, const char *);
size_t         strcspn     (const char *, const char *);
int            printf      (const char *, ...);
int            sscanf      (const char *, const char *, ...);
FILE           *fopen      (const char *, const char *);
size_t         fread       (void *, size_t, size_t, FILE *);
size_t         fwrite      (const void *, size_t, size_t, FILE *);
int            fclose      (FILE *);
int            fprintf     (FILE *, const char *, ...);
time_t         time        (time_t *);
struct tm      *gmtime     (const time_t *);
uid_t          getuid      (void);
uid_t          geteuid     (void);
gid_t          getgid      (void);
gid_t          getegid     (void);
int            issetugid   (void);
void           qsort       (void *, size_t, size_t, int (*)(const void *, const void *));
char           *getenv     (const char *);
char           *secure_getenv (const char *);
#if defined(__GNUC__) && (__GNUC__ >= 2)
void           abort       (void) __attribute__((__noreturn__));
#else
void           abort       (void);
#endif
int            inet_pton   (int, const char *, void *);

void *memcpy(void *destin, const void *source, size_t n);
void *memset(void *s, int ch, size_t n);
void *memchr(const void *buf, int ch, size_t count);
int memcmp(const void *str1, const void *str2, size_t n);
void *memmove( void* dest, const void* src, size_t count );
size_t strlen(const char *string);
char *strcpy(char* dest, const char *src);
char *strncpy(char *destinin, const char *source, int maxlen);
char *strcat(char *dest, const char *src);
int strncmp(const char *str1, const char *str2, size_t n);
int strcasecmp (const char *s1, const char *s2);
int sprintf(char *string, char *format, ...);
#define localtime(timer)                  NULL
#define assert(expression)
#define offsetof(type,member)             OFFSET_OF(type,member)
int atoi(const char *nptr);
#define gettimeofday(tvp,tz)              do { (tvp)->tv_sec = time(NULL); (tvp)->tv_usec = 0; } while (0)

#endif