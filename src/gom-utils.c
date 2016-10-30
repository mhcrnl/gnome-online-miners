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

#include <string.h>

#include "gom-utils.h"

static const char *
gom_filename_get_extension_offset (const char *filename)
{
	char *end, *end2;

	end = strrchr (filename, '.');

	if (end && end != filename) {
		if (strcmp (end, ".gz") == 0 ||
		    strcmp (end, ".bz2") == 0 ||
		    strcmp (end, ".sit") == 0 ||
		    strcmp (end, ".Z") == 0) {
			end2 = end - 1;
			while (end2 > filename &&
			       *end2 != '.') {
				end2--;
			}
			if (end2 != filename) {
				end = end2;
			}
		}
	}

	return end;
}

gchar *
gom_filename_strip_extension (const gchar *filename_with_extension)
{
  gchar *end;
  gchar *filename;

  if (filename_with_extension == NULL)
    return NULL;

  filename = g_strdup (filename_with_extension);
  end = (gchar *) gom_filename_get_extension_offset (filename);

  if (end != NULL && end != filename)
    *end = '\0';

  return filename;
}

const gchar *
gom_filename_to_rdf_type (const gchar *filename_with_extension)
{
  const gchar *extension;
  const gchar *type = NULL;

  g_return_val_if_fail (filename_with_extension != NULL, NULL);

  extension = gom_filename_get_extension_offset (filename_with_extension);

  if (g_strcmp0 (extension, ".txt") == 0)
    type = "nfo:HtmlDocument";

  else if (g_strcmp0 (extension, ".doc") == 0
      || g_strcmp0 (extension, ".docm") == 0
      || g_strcmp0 (extension, ".docx") == 0
      || g_strcmp0 (extension, ".dot") == 0
      || g_strcmp0 (extension, ".dotx") == 0
      || g_strcmp0 (extension, ".odt") == 0
      || g_strcmp0 (extension, ".pdf") == 0)
    type = "nfo:PaginatedTextDocument";

  else if (g_strcmp0 (extension, ".epub") == 0
           || g_strcmp0 (extension, ".djv") == 0
           || g_strcmp0 (extension, ".djvu") == 0
           || g_strcmp0 (extension, ".cbr") == 0
           || g_strcmp0 (extension, ".cbz") == 0
           || g_strcmp0 (extension, ".cbt") == 0
           || g_strcmp0 (extension, ".cb7") == 0
           || g_strcmp0 (extension, ".fb2") == 0
           || g_strcmp0 (extension, ".fb2.zip") == 0
           || g_strcmp0 (extension, ".mobi") == 0
           || g_strcmp0 (extension, ".prc") == 0)
    type = "nfo:EBook";

  else if (g_strcmp0 (extension, ".odp") == 0
           || g_strcmp0 (extension, ".pot") == 0
           || g_strcmp0 (extension, ".potm") == 0
           || g_strcmp0 (extension, ".potx") == 0
           || g_strcmp0 (extension, ".pps") == 0
           || g_strcmp0 (extension, ".ppsm") == 0
           || g_strcmp0 (extension, ".ppsx") == 0
           || g_strcmp0 (extension, ".ppt") == 0
           || g_strcmp0 (extension, ".pptm") == 0
           || g_strcmp0 (extension, ".pptx") == 0)
    type = "nfo:Presentation";

  else if (g_strcmp0 (extension, ".txt") == 0)
    type = "nfo:PlainTextDocument";

  else if (g_strcmp0 (extension, ".ods") == 0
           || g_strcmp0 (extension, ".xls") == 0
           || g_strcmp0 (extension, ".xlsb") == 0
           || g_strcmp0 (extension, ".xlsm") == 0
           || g_strcmp0 (extension, ".xlsx") == 0)
    type = "nfo:Spreadsheet";

  return type;
}

gchar *
gom_iso8601_from_timestamp (gint64 timestamp)
{
  GTimeVal tv;

  tv.tv_sec = timestamp;
  tv.tv_usec = 0;
  return g_time_val_to_iso8601 (&tv);
}
