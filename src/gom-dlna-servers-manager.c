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

#include <gio/gio.h>

#include "gom-dleyna-server-manager.h"
#include "gom-dlna-servers-manager.h"
#include "gom-dlna-server.h"

struct _GomDlnaServersManagerPrivate
{
  DleynaServerManager *proxy;
  GHashTable *servers;
  GHashTable *udn_to_server;
  GError *error;
};

enum
{
  SERVER_FOUND,
  SERVER_LOST,
  LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };


G_DEFINE_TYPE_WITH_PRIVATE (GomDlnaServersManager, gom_dlna_servers_manager, G_TYPE_OBJECT)


static GObject *gom_dlna_servers_manager_singleton = NULL;


static void
gom_dlna_servers_manager_dispose (GObject *object)
{
  GomDlnaServersManager *self = GOM_DLNA_SERVERS_MANAGER (object);
  GomDlnaServersManagerPrivate *priv = self->priv;

  g_clear_object (&priv->proxy);
  g_clear_pointer (&priv->servers, g_hash_table_unref);
  g_clear_pointer (&priv->udn_to_server, g_hash_table_unref);
  g_clear_error (&priv->error);

  G_OBJECT_CLASS (gom_dlna_servers_manager_parent_class)->dispose (object);
}

static void
gom_dlna_servers_manager_server_found_cb (GomDlnaServersManager *self,
                                          const gchar           *object_path,
                                          gpointer              *data)
{
  GomDlnaServersManagerPrivate *priv = self->priv;
  GError *error = NULL;
  GomDlnaServer *server;
  const gchar *udn;

  server = gom_dlna_server_new_for_bus (G_BUS_TYPE_SESSION,
                                        G_DBUS_PROXY_FLAGS_NONE,
                                        "com.intel.dleyna-server",
                                        object_path,
                                        NULL, /* GCancellable */
                                        &error);

  if (error != NULL)
    {
      g_warning ("Error initializing new Server : %s",
                 error->message);
      g_error_free (error);
      return;
    }

  udn = gom_dlna_server_get_udn (server);
  g_debug ("%s '%s' %s %s",
           G_STRFUNC,
           gom_dlna_server_get_friendly_name (server),
           udn,
           object_path);
  g_hash_table_insert (priv->servers, (gpointer) object_path, server);
  g_hash_table_insert (priv->udn_to_server, (gpointer) udn, server);
  g_signal_emit (self, signals[SERVER_FOUND], 0, server);
}


static void
gom_dlna_servers_manager_server_lost_cb (GomDlnaServersManager *self,
                                         const gchar           *object_path,
                                         gpointer              *data)
{
  GomDlnaServersManagerPrivate *priv = self->priv;
  GomDlnaServer *server;
  const gchar *udn;

  server = GOM_DLNA_SERVER (g_hash_table_lookup (priv->servers, object_path));
  g_return_if_fail (server != NULL);

  udn = gom_dlna_server_get_udn (server);
  /* By using g_hash_table_steal instead of remove, we delay deallocation
     of 'server' until all of its associations from all
     hashtables are removed. */
  g_hash_table_steal (priv->udn_to_server, udn);
  g_hash_table_steal (priv->servers, object_path);

  g_signal_emit (self, signals[SERVER_LOST], 0, server);
  /* Server is deallocated now after destroying all its associations
     from all hashtables */
  g_object_unref (server);
}


static void
gom_dlna_servers_manager_proxy_get_servers_cb (GObject      *source_object,
                                               GAsyncResult *res,
                                               gpointer      user_data)
{
  GomDlnaServersManager *self = user_data;
  GomDlnaServersManagerPrivate *priv = self->priv;
  gchar **object_paths = NULL;
  GError *error = NULL;
  guint i;

  dleyna_server_manager_call_get_servers_finish (priv->proxy, &object_paths, res, &error);
  if (error != NULL)
    {
      g_warning ("Unable to Call GetServers : %s",
                 error->message);
      g_error_free (error);
      g_propagate_error (&priv->error, error);
      goto out;
    }

  for (i = 0; object_paths[i] != NULL; i++)
    gom_dlna_servers_manager_server_found_cb (self, object_paths[i], NULL);

 out:
  g_strfreev (object_paths);
  g_object_unref (self);
}


static void
gom_dlna_servers_manager_proxy_new_cb (GObject      *source_object,
                                       GAsyncResult *res,
                                       gpointer      user_data)
{
  GomDlnaServersManager *self = user_data;
  GomDlnaServersManagerPrivate *priv = self->priv;
  GError *error = NULL;

  priv->proxy = dleyna_server_manager_proxy_new_for_bus_finish (res, &error);
  if (error != NULL)
    {
      g_warning ("Unable to connect to the dlnaservers.Manager DBus object: %s",
                 error->message);
      g_error_free (error);
      g_propagate_error (&priv->error, error);
      goto out;
    }

  g_debug ("%s DLNA Servers Manager initialized", G_STRFUNC);

  g_signal_connect_swapped (priv->proxy,
                            "found-server",
                            G_CALLBACK (gom_dlna_servers_manager_server_found_cb),
                            self);

  g_signal_connect_swapped (priv->proxy,
                            "lost-server",
                            G_CALLBACK (gom_dlna_servers_manager_server_lost_cb),
                            self);

  dleyna_server_manager_call_get_servers (priv->proxy, NULL,
                                          gom_dlna_servers_manager_proxy_get_servers_cb,
                                          g_object_ref (self));

 out:
  g_object_unref (self);
}


static GObject *
gom_dlna_servers_manager_constructor (GType                  type,
                                      guint                  n_construct_params,
                                      GObjectConstructParam *construct_params)
{
  if (gom_dlna_servers_manager_singleton == NULL)
    {
      gom_dlna_servers_manager_singleton
        = G_OBJECT_CLASS (gom_dlna_servers_manager_parent_class)->constructor (type,
                                                                               n_construct_params,
                                                                               construct_params);
      g_object_add_weak_pointer (gom_dlna_servers_manager_singleton,
                                 (gpointer) &gom_dlna_servers_manager_singleton);
      return gom_dlna_servers_manager_singleton;
    }

  return g_object_ref (gom_dlna_servers_manager_singleton);
}


static void
gom_dlna_servers_manager_init (GomDlnaServersManager *self)
{
  GomDlnaServersManagerPrivate *priv;

  self->priv = priv = gom_dlna_servers_manager_get_instance_private (self);

  dleyna_server_manager_proxy_new_for_bus (G_BUS_TYPE_SESSION,
                                           G_DBUS_PROXY_FLAGS_NONE,
                                           "com.intel.dleyna-server",
                                           "/com/intel/dLeynaServer",
                                           NULL,
                                           gom_dlna_servers_manager_proxy_new_cb,
                                           g_object_ref (self));

  priv->servers = g_hash_table_new (g_str_hash, g_str_equal);
  priv->udn_to_server = g_hash_table_new (g_str_hash, g_str_equal);
}


static void
gom_dlna_servers_manager_class_init (GomDlnaServersManagerClass *class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (class);

  object_class->constructor = gom_dlna_servers_manager_constructor;
  object_class->dispose = gom_dlna_servers_manager_dispose;

  signals[SERVER_FOUND] = g_signal_new ("server-found",
                                        G_TYPE_FROM_CLASS (class),
                                        G_SIGNAL_RUN_LAST,
                                        0,
                                        NULL,
                                        NULL,
                                        g_cclosure_marshal_VOID__OBJECT,
                                        G_TYPE_NONE,
                                        1,
                                        GOM_TYPE_DLNA_SERVER);

  signals[SERVER_LOST] = g_signal_new ("server-lost",
                                       G_TYPE_FROM_CLASS (class),
                                       G_SIGNAL_RUN_LAST,
                                       0,
                                       NULL,
                                       NULL,
                                       g_cclosure_marshal_VOID__OBJECT,
                                       G_TYPE_NONE,
                                       1,
                                       GOM_TYPE_DLNA_SERVER);
}


GomDlnaServer *
gom_dlna_servers_manager_get_server (GomDlnaServersManager *self,
                                     const gchar           *udn)
{
  return g_hash_table_lookup (self->priv->udn_to_server, udn);
}


GomDlnaServersManager *
gom_dlna_servers_manager_dup_singleton (void)
{
  return g_object_new (GOM_TYPE_DLNA_SERVERS_MANAGER, NULL);
}
