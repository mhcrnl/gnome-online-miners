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

#include <goa/goa.h>

#include "gom-dlna-server.h"
#include "gom-dlna-servers-manager.h"
#include "gom-media-server-miner.h"

#define MINER_IDENTIFIER "gd:media-server:miner:a4a47a3e-eb55-11e3-b983-14feb59cfa0e"

struct _GomMediaServerMinerPrivate {
  GomDlnaServersManager *mngr;
};

G_DEFINE_TYPE_WITH_PRIVATE (GomMediaServerMiner, gom_media_server_miner, GOM_TYPE_MINER)

typedef struct {
  gchar *name;
  gchar *mimetype;
  gchar *path;
  gchar *url;
} PhotoItem;

static PhotoItem *
photo_item_new (GVariant *var)
{
  GVariant *tmp;
  PhotoItem *photo;
  const gchar *str;

  photo = g_slice_new0 (PhotoItem);

  g_variant_lookup (var, "DisplayName", "&s", &str);
  photo->name = g_strdup (str);

  g_variant_lookup (var, "MIMEType", "&s", &str);
  photo->mimetype = g_strdup (str);

  g_variant_lookup (var, "Path", "&o", &str);
  photo->path = g_strdup (str);

  g_variant_lookup (var, "URLs", "@as", &tmp);
  g_variant_get_child (tmp, 0, "&s", &str);
  photo->url = g_strdup (str);
  g_variant_unref (tmp);

  return photo;
}

static void
photo_item_free (PhotoItem *photo)
{
  g_free (photo->name);
  g_free (photo->mimetype);
  g_free (photo->path);
  g_free (photo->url);
  g_slice_free (PhotoItem, photo);
}

static gboolean
account_miner_job_process_photo (GomAccountMinerJob *job,
                                 PhotoItem *photo,
                                 GError **error)
{
  const gchar *photo_id;
  gchar *identifier;
  const gchar *class = "nmm:Photo";
  gchar *resource = NULL;
  gboolean resource_exists;
  gchar **tmp_arr;

  tmp_arr = g_strsplit_set (photo->path, "/", -1);
  photo_id = tmp_arr[g_strv_length (tmp_arr) - 1];
  identifier = g_strdup_printf ("media-server:%s", photo_id);

  /* remove from the list of the previous resources */
  g_hash_table_remove (job->previous_resources, identifier);

  resource = gom_tracker_sparql_connection_ensure_resource
    (job->connection,
     job->cancellable, error,
     &resource_exists,
     job->datasource_urn, identifier,
     "nfo:RemoteDataObject", class, NULL);

  if (*error != NULL)
    goto out;

  gom_tracker_update_datasource (job->connection, job->datasource_urn,
                                 resource_exists, identifier, resource,
                                 job->cancellable, error);
  if (*error != NULL)
    goto out;

  /* the resource changed - just set all the properties again */
  gom_tracker_sparql_connection_insert_or_replace_triple
    (job->connection,
     job->cancellable, error,
     job->datasource_urn, resource,
     "nie:url", photo->url);

  if (*error != NULL)
    goto out;

  gom_tracker_sparql_connection_insert_or_replace_triple
    (job->connection,
     job->cancellable, error,
     job->datasource_urn, resource,
     "nie:mimeType", photo->mimetype);

  if (*error != NULL)
    goto out;

  gom_tracker_sparql_connection_insert_or_replace_triple
    (job->connection,
     job->cancellable, error,
     job->datasource_urn, resource,
     "nie:title", photo->name);

  if (*error != NULL)
    goto out;

 out:
  g_free (resource);
  g_free (identifier);
  g_strfreev (tmp_arr);

  if (*error != NULL)
    return FALSE;

  return TRUE;
}

static GList *
get_photos (GomMediaServerMiner *self,
            const gchar *udn)
{
  GomDlnaServer *server;
  GList *photos_list = NULL;
  GError *error = NULL;
  GVariant *out, *var;
  GVariantIter *iter = NULL;
  PhotoItem *photo;

  server = gom_dlna_servers_manager_get_server (self->priv->mngr, udn);
  if (server == NULL)
    return NULL; /* Server is offline. */

  if (gom_dlna_server_get_searchable (server))
    {
      out = gom_dlna_server_search_objects (server, &error);
      if (error != NULL)
        {
          g_warning ("Unable to search objects on server : %s",
                     error->message);
          g_error_free (error);
          return NULL;
        }

      g_variant_get (out, "aa{sv}", &iter);
      while (g_variant_iter_loop (iter, "@a{sv}", &var))
        {
          photo = photo_item_new (var);
          photos_list = g_list_prepend (photos_list, photo);
        }

      g_variant_iter_free (iter);
    }
  else
    {
      /* TODO: Implement an algo here for !searchable devices. */
    }

  return photos_list;
}

static void
query_media_server (GomAccountMinerJob *job,
                    GError **error)
{
  GomMediaServerMiner *self = GOM_MEDIA_SERVER_MINER (job->miner);
  GomMediaServerMinerPrivate *priv = self->priv;
  GError *local_error = NULL;
  GoaMediaServer *server;
  GList *l;
  GList *photos_list;
  GoaObject *object;
  const gchar *udn;

  object = GOA_OBJECT (g_hash_table_lookup (job->services, "photos"));
  if (object == NULL)
    {
      /* FIXME: use proper #defines and enumerated types */
      g_set_error (error,
                   g_quark_from_static_string ("gom-error"),
                   0,
                   "Can not query without a service");
      return;
    }

  server = goa_object_get_media_server (object);
  udn = goa_media_server_get_udn (server);

  photos_list = get_photos (self, udn);
  for (l = photos_list; l != NULL; l = l->next)
    {
      PhotoItem *photo = (PhotoItem *) l->data;

      account_miner_job_process_photo (job, photo, &local_error);
      if (local_error != NULL)
        {
          g_warning ("Unable to process photo: %s", local_error->message);
          g_clear_error (&local_error);
        }
    }

  g_list_free_full (photos_list, (GDestroyNotify) photo_item_free);
  g_object_unref (server);
}

static GHashTable *
create_services (GomMiner *self,
                 GoaObject *object)
{
  GHashTable *services;

  services = g_hash_table_new_full (g_str_hash, g_str_equal,
                                    NULL, (GDestroyNotify) g_object_unref);

  if (gom_miner_supports_type (self, "photos"))
    g_hash_table_insert (services, "photos", g_object_ref (object));

  return services;
}

static void
gom_media_server_miner_init (GomMediaServerMiner *miner)
{
  miner->priv = gom_media_server_miner_get_instance_private (miner);
  miner->priv->mngr = gom_dlna_servers_manager_dup_singleton ();
}

static void
gom_media_server_miner_class_init (GomMediaServerMinerClass *klass)
{
  GomMinerClass *miner_class = GOM_MINER_CLASS (klass);

  miner_class->goa_provider_type = "media-server";
  miner_class->miner_identifier = MINER_IDENTIFIER;
  miner_class->version = 1;

  miner_class->create_services = create_services;
  miner_class->query = query_media_server;
}
