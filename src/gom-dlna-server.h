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

#ifndef GOM_DLNA_SERVER_H
#define GOM_DLNA_SERVER_H

#include <gio/gio.h>

G_BEGIN_DECLS

#define GOM_TYPE_DLNA_SERVER (gom_dlna_server_get_type ())

#define GOM_DLNA_SERVER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
   GOM_TYPE_DLNA_SERVER, GomDlnaServer))

#define GOM_DLNA_SERVER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
   GOM_TYPE_DLNA_SERVER, GomDlnaServerClass))

#define GOM_IS_DLNA_SERVER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
   GOM_TYPE_DLNA_SERVER))

#define GOM_IS_DLNA_SERVER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
   GOM_TYPE_DLNA_SERVER))

#define GOM_DLNA_SERVER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
   GOM_TYPE_DLNA_SERVER, GomDlnaServerClass))

typedef struct _GomDlnaPhotoItem     GomDlnaPhotoItem;

struct _GomDlnaPhotoItem
{
  gchar *name;
  gchar *mimetype;
  gchar *path;
  gchar *url;
  gchar *type;
};

void                  gom_dlna_photo_item_free                  (GomDlnaPhotoItem *photo);

typedef struct _GomDlnaServer        GomDlnaServer;
typedef struct _GomDlnaServerClass   GomDlnaServerClass;
typedef struct _GomDlnaServerPrivate GomDlnaServerPrivate;

struct _GomDlnaServer
{
  GObject parent_instance;
  GomDlnaServerPrivate *priv;
};

struct _GomDlnaServerClass
{
  GObjectClass parent_class;
};

GType                 gom_dlna_server_get_type                  (void) G_GNUC_CONST;

GomDlnaServer        *gom_dlna_server_new_from_object_path      (const gchar *object_path);

GomDlnaServer        *gom_dlna_server_new_for_bus               (GBusType bus_type,
                                                                 GDBusProxyFlags flags,
                                                                 const gchar *well_known_name,
                                                                 const gchar *object_path,
                                                                 GCancellable *cancellable,
                                                                 GError **error);

const gchar          *gom_dlna_server_get_object_path           (GomDlnaServer  *server);

gboolean              gom_dlna_server_get_searchable            (GomDlnaServer  *server);

GVariant             *gom_dlna_server_search_objects            (GomDlnaServer  *device, GError **error);

const gchar          *gom_dlna_server_get_friendly_name         (GomDlnaServer  *self);

const gchar          *gom_dlna_server_get_udn                   (GomDlnaServer  *self);

GList                *gom_dlna_server_get_photos                (GomDlnaServer  *self);

G_END_DECLS

#endif /* GOM_DLNA_SERVER_H */
