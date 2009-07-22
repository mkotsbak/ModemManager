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
 * Copyright (C) 2008 - 2009 Novell, Inc.
 * Copyright (C) 2009 Red Hat, Inc.
 */

#include <string.h>
#include <dbus/dbus-glib.h>
#include "mm-modem.h"
#include "mm-errors.h"
#include "mm-callback-info.h"

static void impl_modem_enable (MMModem *modem, gboolean enable, DBusGMethodInvocation *context);
static void impl_modem_connect (MMModem *modem, const char *number, DBusGMethodInvocation *context);
static void impl_modem_disconnect (MMModem *modem, DBusGMethodInvocation *context);
static void impl_modem_get_ip4_config (MMModem *modem, DBusGMethodInvocation *context);
static void impl_modem_get_info (MMModem *modem, DBusGMethodInvocation *context);

#include "mm-modem-glue.h"

static void
async_op_not_supported (MMModem *self,
                        MMModemFn callback,
                        gpointer user_data)
{
    MMCallbackInfo *info;

    info = mm_callback_info_new (self, callback, user_data);
    info->error = g_error_new_literal (MM_MODEM_ERROR, MM_MODEM_ERROR_OPERATION_NOT_SUPPORTED,
                                       "Operation not supported");
    mm_callback_info_schedule (info);
}

static void
async_call_done (MMModem *modem, GError *error, gpointer user_data)
{
    DBusGMethodInvocation *context = (DBusGMethodInvocation *) user_data;

    if (error)
        dbus_g_method_return_error (context, error);
    else
        dbus_g_method_return (context);
}

void
mm_modem_enable (MMModem *self,
                 gboolean enable,
                 MMModemFn callback,
                 gpointer user_data)
{
    g_return_if_fail (MM_IS_MODEM (self));
    g_return_if_fail (callback != NULL);

    if (MM_MODEM_GET_INTERFACE (self)->enable)
        MM_MODEM_GET_INTERFACE (self)->enable (self, enable, callback, user_data);
    else
        async_op_not_supported (self, callback, user_data);
}

static void
impl_modem_enable (MMModem *modem,
                   gboolean enable,
                   DBusGMethodInvocation *context)
{
    mm_modem_enable (modem, enable, async_call_done, context);
}

void
mm_modem_connect (MMModem *self,
                  const char *number,
                  MMModemFn callback,
                  gpointer user_data)
{
    g_return_if_fail (MM_IS_MODEM (self));
    g_return_if_fail (callback != NULL);
    g_return_if_fail (number != NULL);

    if (MM_MODEM_GET_INTERFACE (self)->connect)
        MM_MODEM_GET_INTERFACE (self)->connect (self, number, callback, user_data);
    else
        async_op_not_supported (self, callback, user_data);
}

static void
impl_modem_connect (MMModem *modem,
                    const char *number,
                    DBusGMethodInvocation *context)
{
    mm_modem_connect (modem, number, async_call_done, context);
}

static void
get_ip4_invoke (MMCallbackInfo *info)
{
    MMModemIp4Fn callback = (MMModemIp4Fn) info->callback;

    callback (info->modem,
              GPOINTER_TO_UINT (mm_callback_info_get_data (info, "ip4-address")),
              (GArray *) mm_callback_info_get_data (info, "ip4-dns"),
              info->error, info->user_data);
}

void
mm_modem_get_ip4_config (MMModem *self,
                         MMModemIp4Fn callback,
                         gpointer user_data)
{
    g_return_if_fail (MM_IS_MODEM (self));
    g_return_if_fail (callback != NULL);

    if (MM_MODEM_GET_INTERFACE (self)->get_ip4_config)
        MM_MODEM_GET_INTERFACE (self)->get_ip4_config (self, callback, user_data);
    else {
        MMCallbackInfo *info;

        info = mm_callback_info_new_full (self,
                                          get_ip4_invoke,
                                          G_CALLBACK (callback),
                                          user_data);

        info->error = g_error_new_literal (MM_MODEM_ERROR, MM_MODEM_ERROR_OPERATION_NOT_SUPPORTED,
                                           "Operation not supported");
        mm_callback_info_schedule (info);
    }
}

static void
value_array_add_uint (GValueArray *array, guint32 i)
{
    GValue value = { 0, };

    g_value_init (&value, G_TYPE_UINT);
    g_value_set_uint (&value, i);
    g_value_array_append (array, &value);
    g_value_unset (&value);
}

static void
get_ip4_done (MMModem *modem,
              guint32 address,
              GArray *dns,
              GError *error,
              gpointer user_data)
{
    DBusGMethodInvocation *context = (DBusGMethodInvocation *) user_data;

    if (error)
        dbus_g_method_return_error (context, error);
    else {
        GValueArray *array;
        guint32 dns1 = 0;
        guint32 dns2 = 0;
        guint32 dns3 = 0;

        array = g_value_array_new (4);

        if (dns) {
            if (dns->len > 0)

                dns1 = g_array_index (dns, guint32, 0);
            if (dns->len > 1)
                dns2 = g_array_index (dns, guint32, 1);
            if (dns->len > 2)
                dns3 = g_array_index (dns, guint32, 2);
        }

        value_array_add_uint (array, address);
        value_array_add_uint (array, dns1);
        value_array_add_uint (array, dns2);
        value_array_add_uint (array, dns3);

        dbus_g_method_return (context, array);
    }
}

static void
impl_modem_get_ip4_config (MMModem *modem,
                           DBusGMethodInvocation *context)
{
    mm_modem_get_ip4_config (modem, get_ip4_done, context);
}

void
mm_modem_disconnect (MMModem *self,
                     MMModemFn callback,
                     gpointer user_data)
{
    g_return_if_fail (MM_IS_MODEM (self));
    g_return_if_fail (callback != NULL);

    if (MM_MODEM_GET_INTERFACE (self)->disconnect)
        MM_MODEM_GET_INTERFACE (self)->disconnect (self, callback, user_data);
    else
        async_op_not_supported (self, callback, user_data);
}

static void
impl_modem_disconnect (MMModem *modem,
                       DBusGMethodInvocation *context)
{
    mm_modem_disconnect (modem, async_call_done, context);
}

static void
info_call_done (MMModem *self,
                const char *manufacturer,
                const char *model,
                const char *version,
                GError *error,
                gpointer user_data)
{
    DBusGMethodInvocation *context = (DBusGMethodInvocation *) user_data;

    if (error)
        dbus_g_method_return_error (context, error);
    else {
        GValueArray *array;
        GValue value = { 0, };

        array = g_value_array_new (3);

        /* Manufacturer */
        g_value_init (&value, G_TYPE_STRING);
        g_value_set_string (&value, manufacturer);
        g_value_array_append (array, &value);
        g_value_unset (&value);

        /* Model */
        g_value_init (&value, G_TYPE_STRING);
        g_value_set_string (&value, model);
        g_value_array_append (array, &value);
        g_value_unset (&value);

        /* Version */
        g_value_init (&value, G_TYPE_STRING);
        g_value_set_string (&value, version);
        g_value_array_append (array, &value);
        g_value_unset (&value);

        dbus_g_method_return (context, array);
    }
}

static void
info_invoke (MMCallbackInfo *info)
{
    MMModemInfoFn callback = (MMModemInfoFn) info->callback;

    callback (info->modem, NULL, NULL, NULL, info->error, info->user_data);
}

static void
info_call_not_supported (MMModem *self,
                         MMModemInfoFn callback,
                         gpointer user_data)
{
    MMCallbackInfo *info;

    info = mm_callback_info_new_full (MM_MODEM (self), info_invoke, G_CALLBACK (callback), user_data);
    info->error = g_error_new_literal (MM_MODEM_ERROR, MM_MODEM_ERROR_OPERATION_NOT_SUPPORTED,
                                       "Operation not supported");

    mm_callback_info_schedule (info);
}

void
mm_modem_get_info (MMModem *self,
                   MMModemInfoFn callback,
                   gpointer user_data)
{
    g_return_if_fail (MM_IS_MODEM (self));
    g_return_if_fail (callback != NULL);

    if (MM_MODEM_GET_INTERFACE (self)->get_info)
        MM_MODEM_GET_INTERFACE (self)->get_info (self, callback, user_data);
    else
        info_call_not_supported (self, callback, user_data);
}

static void
impl_modem_get_info (MMModem *modem,
                     DBusGMethodInvocation *context)
{
    mm_modem_get_info (modem, info_call_done, context);
}

/*****************************************************************************/

gboolean
mm_modem_owns_port (MMModem *self,
                    const char *subsys,
                    const char *name)
{
    g_return_val_if_fail (self != NULL, FALSE);
    g_return_val_if_fail (MM_IS_MODEM (self), FALSE);
    g_return_val_if_fail (subsys, FALSE);
    g_return_val_if_fail (name, FALSE);

    g_assert (MM_MODEM_GET_INTERFACE (self)->owns_port);
    return MM_MODEM_GET_INTERFACE (self)->owns_port (self, subsys, name);
}

gboolean
mm_modem_grab_port (MMModem *self,
                    const char *subsys,
                    const char *name,
                    MMPortType suggested_type,
                    gpointer user_data,
                    GError **error)
{
    g_return_val_if_fail (self != NULL, FALSE);
    g_return_val_if_fail (MM_IS_MODEM (self), FALSE);
    g_return_val_if_fail (subsys, FALSE);
    g_return_val_if_fail (name, FALSE);

    g_assert (MM_MODEM_GET_INTERFACE (self)->grab_port);
    return MM_MODEM_GET_INTERFACE (self)->grab_port (self, subsys, name, suggested_type, user_data, error);
}

void
mm_modem_release_port (MMModem *self,
                       const char *subsys,
                       const char *name)
{
    g_return_if_fail (self != NULL);
    g_return_if_fail (MM_IS_MODEM (self));
    g_return_if_fail (subsys);
    g_return_if_fail (name);

    g_assert (MM_MODEM_GET_INTERFACE (self)->release_port);
    MM_MODEM_GET_INTERFACE (self)->release_port (self, subsys, name);
}

gboolean
mm_modem_get_valid (MMModem *self)
{
    gboolean valid = FALSE;

    g_return_val_if_fail (self != NULL, FALSE);
    g_return_val_if_fail (MM_IS_MODEM (self), FALSE);

    g_object_get (G_OBJECT (self), MM_MODEM_VALID, &valid, NULL);
    return valid;
}

char *
mm_modem_get_device (MMModem *self)
{
    char *device;

    g_return_val_if_fail (self != NULL, NULL);
    g_return_val_if_fail (MM_IS_MODEM (self), NULL);

    g_object_get (G_OBJECT (self), MM_MODEM_MASTER_DEVICE, &device, NULL);
    return device;
}

/*****************************************************************************/

static void
mm_modem_init (gpointer g_iface)
{
    static gboolean initialized = FALSE;

    if (initialized)
        return;

    /* Properties */
    g_object_interface_install_property
        (g_iface,
         g_param_spec_string (MM_MODEM_DATA_DEVICE,
                              "Device",
                              "Data device",
                              NULL,
                              G_PARAM_READWRITE));

    g_object_interface_install_property
        (g_iface,
         g_param_spec_string (MM_MODEM_MASTER_DEVICE,
                              "MasterDevice",
                              "Master modem parent device of all the modem's ports",
                              NULL,
                              G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

    g_object_interface_install_property
        (g_iface,
         g_param_spec_string (MM_MODEM_DRIVER,
                              "Driver",
                              "Driver",
                              NULL,
                              G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

    g_object_interface_install_property
        (g_iface,
         g_param_spec_string (MM_MODEM_PLUGIN,
                              "Plugin",
                              "Plugin name",
                              NULL,
                              G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

    g_object_interface_install_property
        (g_iface,
         g_param_spec_uint (MM_MODEM_TYPE,
                            "Type",
                            "Type",
                            0, G_MAXUINT32, 0,
                            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

    g_object_interface_install_property
        (g_iface,
         g_param_spec_uint (MM_MODEM_IP_METHOD,
                            "IP method",
                            "IP configuration method",
                            MM_MODEM_IP_METHOD_PPP,
                            MM_MODEM_IP_METHOD_DHCP,
                            MM_MODEM_IP_METHOD_PPP,
                            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

    g_object_interface_install_property
        (g_iface,
         g_param_spec_boolean (MM_MODEM_VALID,
                               "Valid",
                               "Modem is valid",
                               FALSE,
                               G_PARAM_READABLE));

    initialized = TRUE;
}

GType
mm_modem_get_type (void)
{
    static GType modem_type = 0;

    if (!G_UNLIKELY (modem_type)) {
        const GTypeInfo modem_info = {
            sizeof (MMModem), /* class_size */
            mm_modem_init,   /* base_init */
            NULL,       /* base_finalize */
            NULL,
            NULL,       /* class_finalize */
            NULL,       /* class_data */
            0,
            0,              /* n_preallocs */
            NULL
        };

        modem_type = g_type_register_static (G_TYPE_INTERFACE,
                                             "MMModem",
                                             &modem_info, 0);

        g_type_interface_add_prerequisite (modem_type, G_TYPE_OBJECT);

        dbus_g_object_type_install_info (modem_type, &dbus_glib_mm_modem_object_info);
    }

    return modem_type;
}
