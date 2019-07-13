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

#define G_LOG_DOMAIN "sessiond"

#include "keyfile.h"
#include "xsource.h"

#include <glib-2.0/glib.h>

#define LOG_FORMAT "[%s] %s: %s"

static gboolean
handle_error(GError *err, const gchar *group, const gchar *key)
{
    gboolean ret = FALSE;
    if (err->code == G_KEY_FILE_ERROR_KEY_NOT_FOUND ||
        err->code == G_KEY_FILE_ERROR_GROUP_NOT_FOUND) {
        ret = TRUE;
        g_debug(LOG_FORMAT, group, key, err->message);
    } else {
        g_warning(LOG_FORMAT, group, key, err->message);
    }

    g_error_free(err);

    return ret;
}


gboolean
kf_load_bool(GKeyFile *kf, const gchar *g, const gchar *k, gboolean *opt)
{
    GError *err = NULL;
    gboolean boolean = g_key_file_get_boolean(kf, g, k, &err);
    if (err)
        return handle_error(err, g, k);

    *opt = boolean;

    return TRUE;
}

gboolean
kf_load_str(GKeyFile *kf, const gchar *g, const gchar *k, gchar **opt)
{
    GError *err = NULL;
    gchar *str = g_key_file_get_string(kf, g, k, &err);
    if (err)
        return handle_error(err, g, k);

    *opt = str;

    return TRUE;
}

gboolean
kf_load_int(GKeyFile *kf, const gchar *g, const gchar *k, gint *opt)
{
    GError *err = NULL;
    gint i = g_key_file_get_integer(kf, g, k, &err);
    if (err)
        return handle_error(err, g, k);

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

gboolean
kf_load_exec(GKeyFile *kf, const gchar *g, const gchar *k, gchar ***opt)
{
    gchar *str = NULL;
    if (!kf_load_str(kf, g, k, &str))
        return FALSE;

    if (!str)
        return TRUE;

    GError *err = NULL;
    gchar **argv;
    g_shell_parse_argv(str, NULL, &argv, &err);

    if (err) {
        g_warning(LOG_FORMAT, g, k, err->message);
        g_error_free(err);
        g_free(str);
        return FALSE;
    }

    g_free(str);
    *opt = argv;

    return TRUE;
}

gboolean
kf_load_input_mask(GKeyFile *kf, const gchar *g, const gchar *k, guint *opt)
{
    GError *err = NULL;
    gsize len;
    gchar **input = g_key_file_get_string_list(kf, g, k, &len, &err);

    if (err)
        return handle_error(err, g, k);

    guint mask = 0;

    for (guint i = 0; i < len; i++) {
        gchar *str = input[i];
#define X(t, n) \
        if (g_strcmp0(str, n) == 0) { \
            mask |= INPUT_TYPE_MASK(t); \
            continue; \
        }
        INPUT_TYPE_LIST
#undef X
    }

    g_strfreev(input);

    if (mask)
        *opt = mask;

    return TRUE;
}
