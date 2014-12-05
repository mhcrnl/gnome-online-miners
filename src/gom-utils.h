/*
 * GNOME Online Miners - crawls through your online content
 * Copyright (c) 2011, 2012 Red Hat, Inc.
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
 * Author: Cosimo Cecchi <cosimoc@redhat.com>
 *
 */

#ifndef __GOM_UTILS_H__
#define __GOM_UTILS_H__

#include <glib.h>

G_BEGIN_DECLS

const gchar *gom_filename_to_rdf_type (const gchar *filename_with_extension);

gchar *gom_filename_strip_extension (const gchar *filename_with_extension);

gchar *gom_iso8601_from_timestamp (gint64 timestamp);

G_END_DECLS

#endif /* __GOM_UTILS_H__ */
