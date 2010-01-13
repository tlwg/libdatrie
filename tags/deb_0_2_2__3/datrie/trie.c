/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * trie.c - Trie data type and functions
 * Created: 2006-08-11
 * Author:  Theppitak Karoonboonyanan <thep@linux.thai.net>
 */

#include <stdlib.h>
#include <string.h>

#include "trie.h"
#include "fileutils.h"
#include "alpha-map.h"
#include "alpha-map-private.h"
#include "darray.h"
#include "tail.h"

/**
 * @brief Trie structure
 */
struct _Trie {
    AlphaMap   *alpha_map;
    DArray     *da;
    Tail       *tail;

    Bool        is_dirty;
};

/**
 * @brief TrieState structure
 */
struct _TrieState {
    const Trie *trie;       /**< the corresponding trie */
    TrieIndex   index;      /**< index in double-array/tail structures */
    short       suffix_idx; /**< suffix character offset, if in suffix */
    short       is_suffix;  /**< whether it is currently in suffix part */
};

/*------------------------*
 *   INTERNAL FUNCTIONS   *
 *------------------------*/

#define trie_da_is_separate(da,s)      (da_get_base ((da), (s)) < 0)
#define trie_da_get_tail_index(da,s)   (-da_get_base ((da), (s)))
#define trie_da_set_tail_index(da,s,v) (da_set_base ((da), (s), -(v)))

static TrieState * trie_state_new (const Trie *trie,
                                   TrieIndex   index,
                                   short       suffix_idx,
                                   short       is_suffix);

static Bool        trie_branch_in_branch (Trie           *trie,
                                          TrieIndex       sep_node,
                                          const TrieChar *suffix,
                                          TrieData        data);

static Bool        trie_branch_in_tail   (Trie           *trie,
                                          TrieIndex       sep_node,
                                          const TrieChar *suffix,
                                          TrieData        data);

/*-----------------------*
 *   GENERAL FUNCTIONS   *
 *-----------------------*/

Trie *
trie_new (const AlphaMap *alpha_map)
{
    Trie *trie;

    trie = (Trie *) malloc (sizeof (Trie));
    if (!trie)
        return NULL;

    trie->alpha_map = alpha_map_clone (alpha_map);
    if (!trie->alpha_map)
        goto exit_trie_created;

    trie->da = da_new ();
    if (!trie->da)
        goto exit_alpha_map_created;

    trie->tail = tail_new ();
    if (!trie->tail)
        goto exit_da_created;
 
    trie->is_dirty = TRUE;
    return trie;

exit_da_created:
    da_free (trie->da);
exit_alpha_map_created:
    alpha_map_free (trie->alpha_map);
exit_trie_created:
    free (trie);
    return NULL;
}

Trie *
trie_new_from_file (const char *path)
{
    Trie       *trie;
    FILE       *trie_file;

    trie_file = fopen (path, "r");
    if (!trie_file)
        return NULL;

    trie = (Trie *) malloc (sizeof (Trie));
    if (!trie)
        goto exit_file_openned;

    if (NULL == (trie->alpha_map = alpha_map_read_bin (trie_file)))
        goto exit_trie_created;
    if (NULL == (trie->da   = da_read (trie_file)))
        goto exit_alpha_map_created;
    if (NULL == (trie->tail = tail_read (trie_file)))
        goto exit_da_created;

    fclose (trie_file);
    trie->is_dirty = FALSE;
    return trie;

exit_da_created:
    da_free (trie->da);
exit_alpha_map_created:
    alpha_map_free (trie->alpha_map);
exit_trie_created:
    free (trie);
exit_file_openned:
    fclose (trie_file);
    return NULL;
}

void
trie_free (Trie *trie)
{
    alpha_map_free (trie->alpha_map);
    da_free (trie->da);
    tail_free (trie->tail);
    free (trie);
}

int
trie_save (Trie *trie, const char *path)
{
    FILE *file;
    int   res = 0;

    file = fopen (path, "w+");
    if (!file)
        return -1;

    if (alpha_map_write_bin (trie->alpha_map, file) != 0) {
        res = -1;
        goto exit_file_openned;
    }

    if (da_write (trie->da, file) != 0) {
        res = -1;
        goto exit_file_openned;
    }

    if (tail_write (trie->tail, file) != 0) {
        res = -1;
        goto exit_file_openned;
    }

    trie->is_dirty = FALSE;
    return 0;

exit_file_openned:
    fclose (file);
    return res;
}

Bool
trie_is_dirty (const Trie *trie)
{
    return trie->is_dirty;
}


/*------------------------------*
 *   GENERAL QUERY OPERATIONS   *
 *------------------------------*/

Bool
trie_retrieve (const Trie *trie, const AlphaChar *key, TrieData *o_data)
{
    TrieIndex        s;
    short            suffix_idx;
    const AlphaChar *p;

    /* walk through branches */
    s = da_get_root (trie->da);
    for (p = key; !trie_da_is_separate (trie->da, s); p++) {
        if (!da_walk (trie->da, &s,
                      alpha_map_char_to_trie (trie->alpha_map, *p)))
        {
            return FALSE;
        }
        if (0 == *p)
            break;
    }

    /* walk through tail */
    s = trie_da_get_tail_index (trie->da, s);
    suffix_idx = 0;
    for ( ; ; p++) {
        if (!tail_walk_char (trie->tail, s, &suffix_idx,
                             alpha_map_char_to_trie (trie->alpha_map, *p)))
        {
            return FALSE;
        }
        if (0 == *p)
            break;
    }

    /* found, set the val and return */
    if (o_data)
        *o_data = tail_get_data (trie->tail, s);
    return TRUE;
}

Bool
trie_store (Trie *trie, const AlphaChar *key, TrieData data)
{
    TrieIndex        s, t;
    short            suffix_idx;
    const AlphaChar *p, *sep;

    /* walk through branches */
    s = da_get_root (trie->da);
    for (p = key; !trie_da_is_separate (trie->da, s); p++) {
        if (!da_walk (trie->da, &s,
                      alpha_map_char_to_trie (trie->alpha_map, *p)))
        {
            TrieChar *key_str;
            Bool      res;

            key_str = alpha_map_char_to_trie_str (trie->alpha_map, p);
            res = trie_branch_in_branch (trie, s, key_str, data);
            free (key_str);

            return res;
        }
        if (0 == *p)
            break;
    }

    /* walk through tail */
    sep = p;
    t = trie_da_get_tail_index (trie->da, s);
    suffix_idx = 0;
    for ( ; ; p++) {
        if (!tail_walk_char (trie->tail, t, &suffix_idx,
                             alpha_map_char_to_trie (trie->alpha_map, *p)))
        {
            TrieChar *tail_str;
            Bool      res;

            tail_str = alpha_map_char_to_trie_str (trie->alpha_map, sep);
            res = trie_branch_in_tail (trie, s, tail_str, data);
            free (tail_str);

            return res;
        }
        if (0 == *p)
            break;
    }

    /* duplicated key, overwrite val */
    tail_set_data (trie->tail, t, data);
    trie->is_dirty = TRUE;
    return TRUE;
}

static Bool
trie_branch_in_branch (Trie           *trie,
                       TrieIndex       sep_node,
                       const TrieChar *suffix,
                       TrieData        data)
{
    TrieIndex new_da, new_tail;

    new_da = da_insert_branch (trie->da, sep_node, *suffix);
    if (TRIE_INDEX_ERROR == new_da)
        return FALSE;

    if ('\0' != *suffix)
        ++suffix;

    new_tail = tail_add_suffix (trie->tail, suffix);
    tail_set_data (trie->tail, new_tail, data);
    trie_da_set_tail_index (trie->da, new_da, new_tail);

    trie->is_dirty = TRUE;
    return TRUE;
}

static Bool
trie_branch_in_tail   (Trie           *trie,
                       TrieIndex       sep_node,
                       const TrieChar *suffix,
                       TrieData        data)
{
    TrieIndex       old_tail, old_da, s;
    const TrieChar *old_suffix, *p;

    /* adjust separate point in old path */
    old_tail = trie_da_get_tail_index (trie->da, sep_node);
    old_suffix = tail_get_suffix (trie->tail, old_tail);
    if (!old_suffix)
        return FALSE;

    for (p = old_suffix, s = sep_node; *p == *suffix; p++, suffix++) {
        TrieIndex t = da_insert_branch (trie->da, s, *p);
        if (TRIE_INDEX_ERROR == t)
            goto fail;
        s = t;
    }

    old_da = da_insert_branch (trie->da, s, *p);
    if (TRIE_INDEX_ERROR == old_da)
        goto fail;

    if ('\0' != *p)
        ++p;
    tail_set_suffix (trie->tail, old_tail, p);
    trie_da_set_tail_index (trie->da, old_da, old_tail);

    /* insert the new branch at the new separate point */
    return trie_branch_in_branch (trie, s, suffix, data);

fail:
    /* failed, undo previous insertions and return error */
    da_prune_upto (trie->da, sep_node, s);
    trie_da_set_tail_index (trie->da, sep_node, old_tail);
    return FALSE;
}

Bool
trie_delete (Trie *trie, const AlphaChar *key)
{
    TrieIndex        s, t;
    short            suffix_idx;
    const AlphaChar *p;

    /* walk through branches */
    s = da_get_root (trie->da);
    for (p = key; !trie_da_is_separate (trie->da, s); p++) {
        if (!da_walk (trie->da, &s,
                      alpha_map_char_to_trie (trie->alpha_map, *p)))
        {
            return FALSE;
        }
        if (0 == *p)
            break;
    }

    /* walk through tail */
    t = trie_da_get_tail_index (trie->da, s);
    suffix_idx = 0;
    for ( ; ; p++) {
        if (!tail_walk_char (trie->tail, t, &suffix_idx,
                             alpha_map_char_to_trie (trie->alpha_map, *p)))
        {
            return FALSE;
        }
        if (0 == *p)
            break;
    }

    tail_delete (trie->tail, t);
    da_set_base (trie->da, s, TRIE_INDEX_ERROR);
    da_prune (trie->da, s);

    trie->is_dirty = TRUE;
    return TRUE;
}

typedef struct {
    const Trie     *trie;
    TrieEnumFunc    enum_func;
    void           *user_data;
} _TrieEnumData;

static Bool
trie_da_enum_func (const TrieChar *key, TrieIndex sep_node, void *user_data)
{
    _TrieEnumData  *enum_data;
    TrieIndex       t;
    const TrieChar *suffix;
    AlphaChar      *full_key, *p;
    Bool            ret;

    enum_data = (_TrieEnumData *) user_data;

    t = trie_da_get_tail_index (enum_data->trie->da, sep_node);
    suffix = tail_get_suffix (enum_data->trie->tail, t);

    full_key = (AlphaChar *) malloc ((strlen ((const char *)key)
                                      + strlen ((const char *)suffix) + 1)
                                     * sizeof (AlphaChar));
    for (p = full_key; *key; p++, key++) {
        *p = alpha_map_trie_to_char (enum_data->trie->alpha_map, *key);
    }
    for ( ; *suffix; p++, suffix++) {
        *p = alpha_map_trie_to_char (enum_data->trie->alpha_map, *suffix);
    }
    *p = 0;

    ret = (*enum_data->enum_func) (full_key,
                                   tail_get_data (enum_data->trie->tail, t),
                                   enum_data->user_data);

    free (full_key);
    return ret;
}

Bool
trie_enumerate (const Trie *trie, TrieEnumFunc enum_func, void *user_data)
{
    _TrieEnumData   enum_data;

    enum_data.trie      = trie;
    enum_data.enum_func = enum_func;
    enum_data.user_data = user_data;

    return da_enumerate (trie->da, trie_da_enum_func, &enum_data);
}


/*-------------------------------*
 *   STEPWISE QUERY OPERATIONS   *
 *-------------------------------*/

TrieState *
trie_root (const Trie *trie)
{
    return trie_state_new (trie, da_get_root (trie->da), 0, FALSE);
}

/*----------------*
 *   TRIE STATE   *
 *----------------*/

static TrieState *
trie_state_new (const Trie *trie,
                TrieIndex   index,
                short       suffix_idx,
                short       is_suffix)
{
    TrieState *s;

    s = (TrieState *) malloc (sizeof (TrieState));
    if (!s)
        return NULL;

    s->trie       = trie;
    s->index      = index;
    s->suffix_idx = suffix_idx;
    s->is_suffix  = is_suffix;

    return s;
}

void
trie_state_copy (TrieState *dst, const TrieState *src)
{
    /* May be deep copy if necessary, not the case for now */
    *dst = *src;
}

TrieState *
trie_state_clone (const TrieState *s)
{
    return trie_state_new (s->trie, s->index, s->suffix_idx, s->is_suffix);
}

void
trie_state_free (TrieState *s)
{
    free (s);
}

void
trie_state_rewind (TrieState *s)
{
    s->index      = da_get_root (s->trie->da);
    s->is_suffix  = FALSE;
}

Bool
trie_state_walk (TrieState *s, AlphaChar c)
{
    TrieChar tc = alpha_map_char_to_trie (s->trie->alpha_map, c);

    if (!s->is_suffix) {
        Bool ret;

        ret = da_walk (s->trie->da, &s->index, tc);

        if (ret && trie_da_is_separate (s->trie->da, s->index)) {
            s->index = trie_da_get_tail_index (s->trie->da, s->index);
            s->suffix_idx = 0;
            s->is_suffix = TRUE;
        }

        return ret;
    } else {
        return tail_walk_char (s->trie->tail, s->index, &s->suffix_idx, tc);
    }
}

Bool
trie_state_is_walkable (const TrieState *s, AlphaChar c)
{
    TrieChar tc = alpha_map_char_to_trie (s->trie->alpha_map, c);

    if (!s->is_suffix)
        return da_is_walkable (s->trie->da, s->index, tc);
    else 
        return tail_is_walkable_char (s->trie->tail, s->index, s->suffix_idx,
                                      tc);
}

Bool
trie_state_is_single (const TrieState *s)
{
    return s->is_suffix;
}

TrieData
trie_state_get_data (const TrieState *s)
{
    return s->is_suffix ? tail_get_data (s->trie->tail, s->index)
                        : TRIE_DATA_ERROR;
}

/*
vi:ts=4:ai:expandtab
*/
