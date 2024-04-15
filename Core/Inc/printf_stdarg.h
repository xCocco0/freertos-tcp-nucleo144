/*
 *  Copyright 2001, 2002 Georges Menie (www.menie.org)
 *  stdarg version contributed by Christian Ettinger
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Changes for the FreeRTOS ports:
 *
 *  - The dot in "%-8.8s"
 *  - The specifiers 'l' (long) and 'L' (int64_t)
 *  - The specifier 'u' for unsigned
 *  - Dot notation for IP addresses:
 *    sprintf("IP = %xip\n", 0xC0A80164);
 *    will produce "IP = 192.168.1.100\n"
 *    sprintf("IP = %pip\n", pxIPv6_Address);
 */

#ifndef PRINTF_STDARG_H
#define PRINTF_STDARG_H

int tiny_printf( const char * format,
                 ... );

/* Defined here: write a large amount as GB, MB, KB or bytes */
const char * mkSize( uint64_t aSize,
                     char * apBuf,
                     int aLen );

int snprintf( char * apBuf,
              size_t aMaxLen,
              const char * apFmt,
              ... );

int sprintf( char * apBuf,
             const char * apFmt,
             ... );

#endif /*PRINTF_STDARG_H*/
