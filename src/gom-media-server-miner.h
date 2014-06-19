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

#ifndef __GOM_MEDIA_SERVER_MINER_H__
#define __GOM_MEDIA_SERVER_MINER_H__

#include <gio/gio.h>
#include "gom-miner.h"

G_BEGIN_DECLS

#define GOM_TYPE_MEDIA_SERVER_MINER gom_media_server_miner_get_type()

#define GOM_MEDIA_SERVER_MINER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
   GOM_TYPE_MEDIA_SERVER_MINER, GomMediaServerMiner))

#define GOM_MEDIA_SERVER_MINER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
   GOM_TYPE_MEDIA_SERVER_MINER, GomMediaServerMinerClass))

#define GOM_IS_MEDIA_SERVER_MINER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
   GOM_TYPE_MEDIA_SERVER_MINER))

#define GOM_IS_MEDIA_SERVER_MINER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
   GOM_TYPE_MEDIA_SERVER_MINER))

#define GOM_MEDIA_SERVER_MINER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
   GOM_TYPE_MEDIA_SERVER_MINER, GomMediaServerMinerClass))

typedef struct _GomMediaServerMiner GomMediaServerMiner;
typedef struct _GomMediaServerMinerClass GomMediaServerMinerClass;
typedef struct _GomMediaServerMinerPrivate GomMediaServerMinerPrivate;

struct _GomMediaServerMiner {
  GomMiner parent;

  GomMediaServerMinerPrivate *priv;
};

struct _GomMediaServerMinerClass {
  GomMinerClass parent_class;
};

GType gom_media_server_miner_get_type(void);

G_END_DECLS

#endif /* __GOM_MEDIA_SERVER_MINER_H__ */
