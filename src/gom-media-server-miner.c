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

#include "gom-media-server-miner.h"

#define MINER_IDENTIFIER "gd:media-server:miner:a4a47a3e-eb55-11e3-b983-14feb59cfa0e"

G_DEFINE_TYPE (GomMediaServerMiner, gom_media_server_miner, GOM_TYPE_MINER)

static void
query_media_server (GomAccountMinerJob *job,
                    GError **error)
{
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
}

static void
gom_media_server_miner_class_init (GomMediaServerMinerClass *klass)
{
  GomMinerClass *miner_class = GOM_MINER_CLASS (klass);

  miner_class->goa_provider_type = "media_server";
  miner_class->miner_identifier = MINER_IDENTIFIER;
  miner_class->version = 1;

  miner_class->create_services = create_services;
  miner_class->query = query_media_server;
}