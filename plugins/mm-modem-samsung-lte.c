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

#include <config.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "mm-modem-samsung-lte.h"
#include "mm-errors.h"
#include "mm-modem-simple.h"
#include "mm-callback-info.h"
#include "mm-modem-helpers.h"

static void modem_init (MMModem *modem_class);
static void modem_simple_init (MMModemSimple *class);

G_DEFINE_TYPE_EXTENDED (MMModemSamsungLte, mm_modem_samsung_lte, MM_TYPE_GENERIC_GSM, 0,
                        G_IMPLEMENT_INTERFACE (MM_TYPE_MODEM, modem_init)
                        G_IMPLEMENT_INTERFACE (MM_TYPE_MODEM_SIMPLE, modem_simple_init))

#define MM_MODEM_SAMSUNG_LTE_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), MM_TYPE_MODEM_SAMSUNG_LTE, MMModemSamsungLtePrivate))

typedef struct {
    guint enable_wait_id;
    char *username;
    char *password;
} MMModemSamsungLtePrivate;

MMModem *
mm_modem_samsung_lte_new (const char *device,
                         const char *driver,
                         const char *plugin,
                         guint32 vendor,
                         guint32 product)
{
    g_return_val_if_fail (device != NULL, NULL);
    g_return_val_if_fail (driver != NULL, NULL);
    g_return_val_if_fail (plugin != NULL, NULL);

    return MM_MODEM (g_object_new (MM_TYPE_MODEM_SAMSUNG_LTE,
                                   MM_MODEM_MASTER_DEVICE, device,
                                   MM_MODEM_DRIVER, driver,
                                   MM_MODEM_PLUGIN, plugin,
                                   MM_MODEM_IP_METHOD, MM_MODEM_IP_METHOD_DHCP,
                                   MM_MODEM_HW_VID, vendor,
                                   MM_MODEM_HW_PID, product,
                                   NULL));
}

/*****************************************************************************/


/*****************************************************************************/
/*    Modem class override functions                                         */
/*****************************************************************************/

static gboolean
grab_port (MMModem *modem,
           const char *subsys,
           const char *name,
           MMPortType suggested_type,
           gpointer user_data,
           GError **error)
{
    MMGenericGsm *gsm = MM_GENERIC_GSM (modem);
    MMPortType ptype = MM_PORT_TYPE_IGNORED;
    MMPort *port;

    if (!strcmp (subsys, "tty")) {
        if (suggested_type == MM_PORT_TYPE_UNKNOWN) {
            if (!mm_generic_gsm_get_at_port (gsm, MM_PORT_TYPE_PRIMARY))
                ptype = MM_PORT_TYPE_PRIMARY;
            else if (!mm_generic_gsm_get_at_port (gsm, MM_PORT_TYPE_SECONDARY))
                ptype = MM_PORT_TYPE_SECONDARY;
        } else
            ptype = suggested_type;
    }

    port = mm_generic_gsm_grab_port (gsm, subsys, name, ptype, error);

    if (port && MM_IS_AT_SERIAL_PORT (port)) {
        //GRegex *regex;

        // g_object_set (G_OBJECT (port), MM_PORT_CARRIER_DETECT, FALSE, NULL);

        //            regex = g_regex_new ("\\r\\n\\+PACSP0\\r\\n", G_REGEX_RAW | G_REGEX_OPTIMIZE, 0, NULL);
        //mm_at_serial_port_add_unsolicited_msg_handler (MM_AT_SERIAL_PORT (port), regex, NULL, NULL, NULL);
        //g_regex_unref (regex);
    }

    return !!port;
}

static void
ps_attach_done (MMAtSerialPort *port,
                GString *response,
                GError *error,
                gpointer user_data)
{
    MMCallbackInfo *info = (MMCallbackInfo *) user_data;

    if (error) {
        info->error = g_error_copy (error);
        mm_callback_info_schedule (info);
        return;
    }
    else {
        mm_generic_gsm_connect_complete (MM_GENERIC_GSM (info->modem), error, info);
    }
}

static void
do_connect (MMModem *modem,
            const char *number,
            MMModemFn callback,
            gpointer user_data)
{
    MMCallbackInfo *info;
    MMAtSerialPort *port;

    mm_modem_set_state (modem, MM_MODEM_STATE_CONNECTING, MM_MODEM_STATE_REASON_NONE);

    info = mm_callback_info_new (modem, callback, user_data);

    port = mm_generic_gsm_get_best_at_port (MM_GENERIC_GSM (modem), &info->error);
    if (!port) {
        mm_callback_info_schedule (info);
        return;
    }

    mm_at_serial_port_queue_command (port, "+CGATT=1", 10, ps_attach_done, info);
}

static void
clear_user_pass (MMModemSamsungLte *self)
{
    MMModemSamsungLtePrivate *priv = MM_MODEM_SAMSUNG_LTE_GET_PRIVATE (self);

    g_free (priv->username);
    priv->username = NULL;
    g_free (priv->password);
    priv->username = NULL;
}

static void
do_disconnect (MMGenericGsm *gsm,
               gint cid,
               MMModemFn callback,
               gpointer user_data)
{
    MMAtSerialPort *primary;

    clear_user_pass (MM_MODEM_SAMSUNG_LTE (gsm));

    primary = mm_generic_gsm_get_at_port (gsm, MM_PORT_TYPE_PRIMARY);
    g_assert (primary);

    /* Deactivate net interface */
    mm_at_serial_port_queue_command (primary, "+CGATT=0", 3, NULL, NULL);

    MM_GENERIC_GSM_CLASS (mm_modem_samsung_lte_parent_class)->do_disconnect (gsm, cid, callback, user_data);
}

/*****************************************************************************/
/*    Simple Modem class override functions                                  */
/*****************************************************************************/

static char *
simple_dup_string_property (GHashTable *properties, const char *name, GError **error)
{
    GValue *value;

    value = (GValue *) g_hash_table_lookup (properties, name);
    if (!value)
        return NULL;

    if (G_VALUE_HOLDS_STRING (value))
        return g_value_dup_string (value);

    g_set_error (error, MM_MODEM_ERROR, MM_MODEM_ERROR_GENERAL,
                 "Invalid property type for '%s': %s (string expected)",
                 name, G_VALUE_TYPE_NAME (value));

    return NULL;
}

static void
simple_connect (MMModemSimple *simple,
                GHashTable *properties,
                MMModemFn callback,
                gpointer user_data)
{
    MMModemSamsungLtePrivate *priv = MM_MODEM_SAMSUNG_LTE_GET_PRIVATE (simple);
    MMCallbackInfo *info = (MMCallbackInfo *) user_data;
    MMModemSimple *parent_iface;

    clear_user_pass (MM_MODEM_SAMSUNG_LTE (simple));
    priv->username = simple_dup_string_property (properties, "username", &info->error);
    priv->password = simple_dup_string_property (properties, "password", &info->error);

    parent_iface = g_type_interface_peek_parent (MM_MODEM_SIMPLE_GET_INTERFACE (simple));
    parent_iface->connect (MM_MODEM_SIMPLE (simple), properties, callback, info);
}

static void
get_allowed_mode_done (MMAtSerialPort *port,
                       GString *response,
                       GError *error,
                       gpointer user_data)
{
    MMCallbackInfo *info = (MMCallbackInfo *) user_data;
    GRegex *r = NULL;
    GMatchInfo *match_info;

    info->error = mm_modem_check_removed (info->modem, error);
    if (info->error)
        goto done;

    r = g_regex_new ("+MODESELECT:(\\d+)$", 0, 0, NULL);
    if (!r) {
        info->error = g_error_new_literal (MM_MODEM_ERROR,
                                           MM_MODEM_ERROR_GENERAL,
                                           "Failed to parse the allowed mode response");
        goto done;
    }

    if (g_regex_match_full (r, response->str, response->len, 0, 0, &match_info, &info->error)) {
        MMModemGsmAllowedMode mode = MM_MODEM_GSM_ALLOWED_MODE_ANY;
        char *str;

        str = g_match_info_fetch (match_info, 1);
        switch (atoi (str)) {
        case 0:
            // mode = MM_MODEM_GSM_ALLOWED_MODE_3G_PREFERRED;
        case 1:
            // mode = MM_MODEM_GSM_ALLOWED_MODE_3G_ONLY;
        case 2:
            // mode = MM_MODEM_GSM_ALLOWED_MODE_2G_ONLY;
        case 3:
        case 4:
            //            mode = MM_MODEM_GSM_ALLOWED_MODE_2G_PREFERRED;
            mode = MM_MODEM_GSM_ALLOWED_MODE_ANY;
            break;

        default:
            info->error = g_error_new (MM_MODEM_ERROR,
                                       MM_MODEM_ERROR_GENERAL,
                                       "Failed to parse the allowed mode response: '%s'",
                                       response->str);
            break;
        }
        g_free (str);

        mm_callback_info_set_result (info, GUINT_TO_POINTER (mode), NULL);
    }

done:
    if (r)
        g_regex_unref (r);
    mm_callback_info_schedule (info);
}

static void
get_allowed_mode (MMGenericGsm *gsm,
                  MMModemUIntFn callback,
                  gpointer user_data)
{
    MMCallbackInfo *info;
    MMAtSerialPort *primary;

    info = mm_callback_info_uint_new (MM_MODEM (gsm), callback, user_data);

    primary = mm_generic_gsm_get_at_port (gsm, MM_PORT_TYPE_PRIMARY);

    mm_at_serial_port_queue_command (primary, "+MODESELECT?", 3, get_allowed_mode_done, info);
}

static void
set_allowed_mode_done (MMAtSerialPort *port,
                       GString *response,
                       GError *error,
                       gpointer user_data)
{
    MMCallbackInfo *info = (MMCallbackInfo *) user_data;

    if (error)
        info->error = g_error_copy (error);

   mm_callback_info_schedule (info);
}

static void
set_allowed_mode (MMGenericGsm *gsm,
                  MMModemGsmAllowedMode mode,
                  MMModemFn callback,
                  gpointer user_data)
{
    MMCallbackInfo *info;
    MMAtSerialPort *primary;
    char *command;
    int idx = 0;

    info = mm_callback_info_new (MM_MODEM (gsm), callback, user_data);

    primary = mm_generic_gsm_get_at_port (gsm, MM_PORT_TYPE_PRIMARY);
    if (mm_port_get_connected (MM_PORT (primary))) {
        g_set_error_literal (&info->error, MM_MODEM_ERROR, MM_MODEM_ERROR_CONNECTED,
                             "Cannot perform this operation while connected");
        mm_callback_info_schedule (info);
        return;
    }

    switch (mode) {
    case MM_MODEM_GSM_ALLOWED_MODE_2G_ONLY:
    case MM_MODEM_GSM_ALLOWED_MODE_3G_ONLY:
    case MM_MODEM_GSM_ALLOWED_MODE_2G_PREFERRED:
    case MM_MODEM_GSM_ALLOWED_MODE_3G_PREFERRED:
    case MM_MODEM_GSM_ALLOWED_MODE_ANY:
    default:
        idx = 3;
        break;
    }

    command = g_strdup_printf ("+MODESELECT=%d", idx);
    mm_at_serial_port_queue_command (primary, command, 3, set_allowed_mode_done, info);
    g_free (command);
}


/*****************************************************************************/

static void
modem_init (MMModem *modem_class)
{
    modem_class->grab_port = grab_port;
    modem_class->connect = do_connect;
}

static void
modem_simple_init (MMModemSimple *class)
{
    class->connect = simple_connect;
}

static void
mm_modem_samsung_lte_init (MMModemSamsungLte *self)
{
}

static void
dispose (GObject *object)
{
    clear_user_pass (MM_MODEM_SAMSUNG_LTE (object));
}

static void
set_property (GObject *object, guint prop_id,
              const GValue *value, GParamSpec *pspec)
{
    /* All props hardcoded in get_property */
}

static void
get_property (GObject *object, guint prop_id,
              GValue *value, GParamSpec *pspec)
{
    switch (prop_id) {
    /* This modem does not support "ATZ" command while in CFUN=0 mode */
    case MM_GENERIC_GSM_PROP_INIT_CMD:
        g_value_set_string (value, "E0");
        break;

        /*
    case MM_GENERIC_GSM_PROP_INIT_CMD_OPTIONAL:
        g_value_set_string (value, "+CFUN=5");
        break;
        */

    case MM_GENERIC_GSM_PROP_POWER_UP_CMD:
        g_value_set_string (value, "+CFUN=5");
        break;

    default:
        break;
    }
}

static void
mm_modem_samsung_lte_class_init (MMModemSamsungLteClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    MMGenericGsmClass *gsm_class = MM_GENERIC_GSM_CLASS (klass);

    mm_modem_samsung_lte_parent_class = g_type_class_peek_parent (klass);
    g_type_class_add_private (object_class, sizeof (MMModemSamsungLtePrivate));

    object_class->dispose = dispose;
    object_class->get_property = get_property;
    object_class->set_property = set_property;
    gsm_class->set_allowed_mode = set_allowed_mode;
    gsm_class->get_allowed_mode = get_allowed_mode;
    //gsm_class->do_enable_power_up_done = real_do_enable_power_up_done;
    // gsm_class->set_allowed_mode = set_allowed_mode;
    // gsm_class->get_allowed_mode = get_allowed_mode;
    // gsm_class->get_access_technology = get_access_technology;
    // gsm_class->get_sim_iccid = get_sim_iccid;
    gsm_class->do_disconnect = do_disconnect;

    g_object_class_override_property (object_class,
                                      MM_GENERIC_GSM_PROP_INIT_CMD,
                                      MM_GENERIC_GSM_INIT_CMD);
    /*
    g_object_class_override_property (object_class,
                                      MM_GENERIC_GSM_PROP_INIT_CMD_OPTIONAL,
                                      MM_GENERIC_GSM_INIT_CMD_OPTIONAL);
    */
    g_object_class_override_property (object_class,
                                      MM_GENERIC_GSM_PROP_POWER_UP_CMD,
                                      MM_GENERIC_GSM_POWER_UP_CMD);
}
