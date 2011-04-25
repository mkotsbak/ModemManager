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
 * Copyright (C) 2011 - Marius Bjoernstad Kotsbak <marius@kotsbak.com>
 */

#ifndef MM_MODEM_SAMSUNG_LTE_H
#define MM_MODEM_SAMSUNG_LTE_H

#include "mm-generic-gsm.h"

#define MM_TYPE_MODEM_SAMSUNG_LTE            (mm_modem_samsung_lte_get_type ())
#define MM_MODEM_SAMSUNG_LTE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), MM_TYPE_MODEM_SAMSUNG_LTE, MMModemSamsungLte))
#define MM_MODEM_SAMSUNG_LTE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  MM_TYPE_MODEM_SAMSUNG_LTE, MMModemSamsungLteClass))
#define MM_IS_MODEM_SAMSUNG_LTE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MM_TYPE_MODEM_SAMSUNG_LTE))
#define MM_IS_MODEM_SAMSUNG_LTE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  MM_TYPE_MODEM_SAMSUNG_LTE))
#define MM_MODEM_SAMSUNG_LTE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  MM_TYPE_MODEM_SAMSUNG_LTE, MMModemSamsungLteClass))

typedef struct {
    MMGenericGsm parent;
} MMModemSamsungLte;

typedef struct {
    MMGenericGsmClass parent;
} MMModemSamsungLteClass;

GType mm_modem_samsung_lte_get_type (void);

MMModem *mm_modem_samsung_lte_new (const char *device,
                                  const char *driver,
                                  const char *plugin_name,
                                  guint32 vendor,
                                  guint32 product);

#endif /* MM_MODEM_SAMSUNG_LTE_H */
