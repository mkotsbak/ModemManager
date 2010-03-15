/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details:
 *
 * Copyright (C) 2010 Red Hat, Inc.
 */

#ifndef MM_CHARSETS_H
#define MM_CHARSETS_H

#include <glib.h>

typedef enum {
    MM_MODEM_CHARSET_UNKNOWN = 0x00000000,
    MM_MODEM_CHARSET_GSM     = 0x00000001,
    MM_MODEM_CHARSET_IRA     = 0x00000002,
    MM_MODEM_CHARSET_8859_1  = 0x00000004,
    MM_MODEM_CHARSET_UTF8    = 0x00000008,
    MM_MODEM_CHARSET_UCS2    = 0x00000010,
    MM_MODEM_CHARSET_PCCP437 = 0x00000020,
    MM_MODEM_CHARSET_PCDN    = 0x00000040,
    MM_MODEM_CHARSET_HEX     = 0x00000080
} MMModemCharset;

const char *mm_modem_charset_to_string (MMModemCharset charset);

MMModemCharset mm_modem_charset_from_string (const char *string);

#endif /* MM_CHARSETS_H */
