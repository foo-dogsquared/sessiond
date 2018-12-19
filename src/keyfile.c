/*
sessiond - standalone X session manager
Copyright (C) 2018 James Reed

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "keyfile.h"

#include <glib-2.0/glib.h>

gboolean
kf_load_bool(GKeyFile *kf, const gchar *g, const gchar *k, gboolean *opt)
{
    GError *err = NULL;
    gboolean boolean = g_key_file_get_boolean(kf, g, k, &err);
    if (err) {
        gboolean ret = FALSE;
        if (err->code == G_KEY_FILE_ERROR_KEY_NOT_FOUND ||
            err->code == G_KEY_FILE_ERROR_GROUP_NOT_FOUND)
            ret = TRUE;
        else
            g_warning("%s", err->message);
        g_error_free(err);
        return ret;
    }

    *opt = boolean;

    return TRUE;
}

gboolean
kf_load_str(GKeyFile *kf, const gchar *g, const gchar *k, gchar **opt)
{
    GError *err = NULL;
    gchar *str = g_key_file_get_string(kf, g, k, &err);
    if (err) {
        gboolean ret = FALSE;
        if (err->code == G_KEY_FILE_ERROR_KEY_NOT_FOUND ||
            err->code == G_KEY_FILE_ERROR_GROUP_NOT_FOUND)
            ret = TRUE;
        else
            g_warning("%s", err->message);
        g_error_free(err);
        return ret;
    }

    *opt = str;

    return TRUE;
}

gboolean
kf_load_int(GKeyFile *kf, const gchar *g, const gchar *k, gint *opt)
{
    GError *err = NULL;
    gint i = g_key_file_get_integer(kf, g, k, &err);
    if (err) {
        gboolean ret = FALSE;
        if (err->code == G_KEY_FILE_ERROR_KEY_NOT_FOUND ||
            err->code == G_KEY_FILE_ERROR_GROUP_NOT_FOUND)
            ret = TRUE;
        else
            g_warning("%s", err->message);
        g_error_free(err);
        return ret;
    }

    *opt = i;

    return TRUE;
}

gboolean
kf_load_uint(GKeyFile *kf, const gchar *g, const gchar *k, guint *opt)
{
    gint i = -1;
    if (!kf_load_int(kf, g, k, &i))
        return FALSE;
    if (i > -1)
        *opt = i;
    return TRUE;
}
