/*
 * GNOME Online Miners - crawls through your online content
 * Copyright (c) 2014 Pranav Kant
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * Author: Pranav Kant <pranav913@gmail.com>
 *
 */

#include "config.h"

#include "gom-dleyna-server-media-device.h"
#include "gom-upnp-media-container2.h"
#include "gom-dlna-server.h"

struct _GomDlnaServerPrivate
{
  DleynaServerMediaDevice *device;
  UpnpMediaContainer2 *container;
  GBusType bus_type;
  GDBusProxyFlags flags;
  gchar *object_path;
  gchar *well_known_name;
};


enum{
  PROP_0,
  PROP_BUS_TYPE,
  PROP_FLAGS,
  PROP_OBJECT_PATH,
  PROP_WELL_KNOWN_NAME,
};

static void gom_dlna_server_initable_iface_init (GInitableIface *iface);

G_DEFINE_TYPE_WITH_CODE (GomDlnaServer, gom_dlna_server, G_TYPE_OBJECT,
                         G_ADD_PRIVATE (GomDlnaServer)
                         G_IMPLEMENT_INTERFACE (G_TYPE_INITABLE,
                                                gom_dlna_server_initable_iface_init));


static void
gom_dlna_server_dispose (GObject *object)
{
  GomDlnaServer *self = GOM_DLNA_SERVER (object);
  GomDlnaServerPrivate *priv = self->priv;

  g_clear_object (&priv->device);
  g_clear_object (&priv->container);

  G_OBJECT_CLASS (gom_dlna_server_parent_class)->dispose (object);
}


static void
gom_dlna_server_finalize (GObject *object)
{
  GomDlnaServer *self = GOM_DLNA_SERVER (object);
  GomDlnaServerPrivate *priv = self->priv;

  g_free (priv->well_known_name);
  g_free (priv->object_path);

  G_OBJECT_CLASS (gom_dlna_server_parent_class)->finalize (object);
}


static void
gom_dlna_server_get_property (GObject *object,
                              guint prop_id,
                              GValue *value,
                              GParamSpec *pspec)
{
  GomDlnaServer *self = GOM_DLNA_SERVER (object);

  switch (prop_id)
    {
    case PROP_OBJECT_PATH:
      g_value_set_string (value, gom_dlna_server_get_object_path (self));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}


static void
gom_dlna_server_set_property (GObject *object,
                              guint prop_id,
                              const GValue *value,
                              GParamSpec *pspec)
{
  GomDlnaServer *self = GOM_DLNA_SERVER (object);
  GomDlnaServerPrivate *priv = self->priv;

  switch (prop_id)
    {
    case PROP_BUS_TYPE:
      priv->bus_type = g_value_get_enum (value);
      break;

    case PROP_FLAGS:
      priv->flags = g_value_get_flags (value);
      break;

    case PROP_OBJECT_PATH:
      priv->object_path = g_value_dup_string (value);
      break;

    case PROP_WELL_KNOWN_NAME:
      priv->well_known_name = g_value_dup_string (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}


static void
gom_dlna_server_init (GomDlnaServer *self)
{
  self->priv = gom_dlna_server_get_instance_private (self);
}


static void
gom_dlna_server_class_init (GomDlnaServerClass *class)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (class);

  gobject_class->dispose = gom_dlna_server_dispose;
  gobject_class->finalize = gom_dlna_server_finalize;
  gobject_class->set_property = gom_dlna_server_set_property;
  gobject_class->get_property = gom_dlna_server_get_property;

  g_object_class_install_property (gobject_class,
                                   PROP_BUS_TYPE,
                                   g_param_spec_enum ("bus-type",
                                                      "Bus Type",
                                                      "The bus to connect to, defaults to the session one",
                                                      G_TYPE_BUS_TYPE,
                                                      G_BUS_TYPE_SESSION,
                                                      G_PARAM_WRITABLE |
                                                      G_PARAM_CONSTRUCT_ONLY |
                                                      G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_class,
                                   PROP_FLAGS,
                                   g_param_spec_flags ("flags",
                                                       "Flags",
                                                       "Proxy flags",
                                                       G_TYPE_DBUS_PROXY_FLAGS,
                                                       G_DBUS_PROXY_FLAGS_NONE,
                                                       G_PARAM_READWRITE |
                                                       G_PARAM_CONSTRUCT_ONLY |
                                                       G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (gobject_class,
                                   PROP_OBJECT_PATH,
                                   g_param_spec_string ("object-path",
                                                        "Object Path",
                                                        "The object path the proxy is for",
                                                        NULL,
                                                        G_PARAM_READWRITE |
                                                        G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_STATIC_STRINGS));


  g_object_class_install_property (gobject_class,
                                   PROP_WELL_KNOWN_NAME,
                                   g_param_spec_string ("well-known-name",
                                                        "Well-Known Name",
                                                        "The well-known name of the service",
                                                        NULL,
                                                        G_PARAM_READWRITE |
                                                        G_PARAM_CONSTRUCT_ONLY |
                                                        G_PARAM_STATIC_STRINGS));

}


GomDlnaServer *
gom_dlna_server_new_for_bus (GBusType bus_type,
                             GDBusProxyFlags flags,
                             const gchar *well_known_name,
                             const gchar *object_path,
                             GCancellable *cancellable,
                             GError **error)
{
  return g_initable_new (GOM_TYPE_DLNA_SERVER,
                         cancellable,
                         error,
                         "bus-type", bus_type,
                         "flags", flags,
                         "object-path", object_path,
                         "well-known-name", well_known_name,
                         NULL);
}


static gboolean
gom_dlna_server_initable_init (GInitable *initable,
                               GCancellable *cancellable,
                               GError **error)
{
  GomDlnaServer *self = GOM_DLNA_SERVER (initable);
  GomDlnaServerPrivate *priv = self->priv;
  gboolean ret_val = TRUE;

  priv->device =
    dleyna_server_media_device_proxy_new_for_bus_sync (priv->bus_type,
                                                       G_DBUS_PROXY_FLAGS_NONE,
                                                       priv->well_known_name,
                                                       priv->object_path,
                                                       NULL,
                                                       error);
  if (*error != NULL)
    {
      ret_val = FALSE;
      goto out;
    }

  priv->container =
    upnp_media_container2_proxy_new_for_bus_sync (priv->bus_type,
                                                  G_DBUS_PROXY_FLAGS_NONE,
                                                  priv->well_known_name,
                                                  priv->object_path,
                                                  NULL,
                                                  error);
  if (*error != NULL)
    {
      ret_val = FALSE;
      goto out;
    }

 out:
  return ret_val;
}


static void
gom_dlna_server_initable_iface_init (GInitableIface *iface)
{
  iface->init = gom_dlna_server_initable_init;
}


const gchar *
gom_dlna_server_get_object_path (GomDlnaServer *self)
{
  return self->priv->object_path;
}


const gchar *
gom_dlna_server_get_friendly_name (GomDlnaServer *self)
{
  GomDlnaServerPrivate *priv = self->priv;

  return dleyna_server_media_device_get_friendly_name (priv->device);
}


GVariant *
gom_dlna_server_search_objects (GomDlnaServer *self, GError **error)
{
  GomDlnaServerPrivate *priv = self->priv;
  GVariant *out = NULL;
  gchar *query = g_strdup_printf ("Type = \"image.photo\"");
  const gchar const *filter[] = {"DisplayName", "URLs", "Path", "MIMEType", NULL};

  upnp_media_container2_call_search_objects_sync (priv->container,
                                                  query,
                                                  0,
                                                  0,
                                                  filter,
                                                  &out,
                                                  NULL,
                                                  error);


  g_free (query);
  return out;
}


const gchar *
gom_dlna_server_get_udn (GomDlnaServer *self)
{
  GomDlnaServerPrivate *priv = self->priv;

  return dleyna_server_media_device_get_udn (priv->device);
}


gboolean
gom_dlna_server_get_searchable (GomDlnaServer *self)
{
  GomDlnaServerPrivate *priv = self->priv;
  return upnp_media_container2_get_searchable (priv->container);
}
