/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * trie.c - Trie data type and functions
 * Created: 2006-08-11
 * Author:  Theppitak Karoonboonyanan <thep@linux.thai.net>
 */

#include <stdlib.h>
#include <string.h>

#include "trie.h"
#include "darray.h"
#include "tail.h"

/**
 * @brief Trie structure
 */
struct _Trie {
    DArray  *da;
    Tail    *tail;
};

/**
 * @brief TrieState structure
 */
struct _TrieState {
    Trie      *trie;        /**< the corresponding trie */
    TrieIndex  index;       /**< index in double-array/tail structures */
    short      suffix_idx;  /**< suffix character offset, if in suffix */
    short      is_suffix;   /**< whether it is currently in suffix part */
};

/*------------------------*
 *   INTERNAL FUNCTIONS   *
 *------------------------*/

#define trie_da_is_separate(da,s)      (da_get_base ((da), (s)) < 0)
#define trie_da_get_tail_index(da,s)   (-da_get_base ((da), (s)))
#define trie_da_set_tail_index(da,s,v) (da_set_base ((da), (s), -(v)))

static TrieState * trie_state_new (Trie      *trie,
                                   TrieIndex  index,
                                   short      suffix_idx,
                                   short      is_suffix);

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
trie_open (const char *path, const char *name, TrieIOMode mode)
{
    Trie *trie;

    trie = (Trie *) malloc (sizeof (Trie));
    if (!trie)
        return NULL;

    trie->da   = da_open (path, name, mode);
    trie->tail = tail_open (path, name, mode);

    return trie;
}

int
trie_close (Trie *trie)
{
    if (!trie)
        return -1;

    if (da_close (trie->da) != 0)
        return -1;

    if (tail_close (trie->tail) != 0)
        return -1;

    free (trie);

    return 0;
}

int
trie_save (Trie *trie)
{
    if (!trie)
        return -1;

    if (da_save (trie->da) != 0)
        return -1;

    if (tail_save (trie->tail) != 0)
        return -1;

    return 0;
}


/*------------------------------*
 *   GENERAL QUERY OPERATIONS   *
 *------------------------------*/

Bool
trie_retrieve (Trie *trie, const TrieChar *key, TrieData *o_data)
{
    TrieIndex        s;
    short            suffix_idx;
    const TrieChar  *p;
    size_t           len;

    /* walk through branches */
    s = da_get_root (trie->da);
    for (p = key; !trie_da_is_separate (trie->da, s); p++) {
        if (!da_walk (trie->da, &s, *p))
            return FALSE;
        if ('\0' == *p)
            break;
    }

    /* walk through tail */
    s = trie_da_get_tail_index (trie->da, s);
    if ('\0' != *p) {
        suffix_idx = 0;
        len = strlen ((const char *) p) + 1;    /* including null-terminator */
        if (tail_walk_str (trie->tail, s, &suffix_idx, p, len) != len)
            return FALSE;
    }

    /* found, set the val and return */
    if (o_data)
        *o_data = tail_get_data (trie->tail, s);
    return TRUE;
}

Bool
trie_store (Trie *trie, const TrieChar *key, TrieData data)
{
    TrieIndex        s, t;
    short            suffix_idx;
    const TrieChar  *p;
    size_t           len;

    /* walk through branches */
    s = da_get_root (trie->da);
    for (p = key; !trie_da_is_separate (trie->da, s); p++) {
        if (!da_walk (trie->da, &s, *p))
            return trie_branch_in_branch (trie, s, p, data);
        if ('\0' == *p)
            break;
    }

    /* walk through tail */
    t = trie_da_get_tail_index (trie->da, s);
    if ('\0' != *p) {
        suffix_idx = 0;
        len = strlen ((const char *) p) + 1;    /* including null-terminator */
        if (tail_walk_str (trie->tail, t, &suffix_idx, p, len) != len)
            return trie_branch_in_tail (trie, s, p, data);
    }

    /* duplicated key, overwrite val */
    tail_set_data (trie->tail, t, data);
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
    if ('\0' != *suffix)
        ++suffix;

    new_tail = tail_add_suffix (trie->tail, suffix);
    trie_da_set_tail_index (trie->da, new_da, new_tail);

    return TRUE;
}

static Bool
trie_branch_in_tail   (Trie           *trie,
                       TrieIndex       sep_node,
                       const TrieChar *suffix,
                       TrieData        data)
{
    TrieIndex   old_tail, old_da;
    TrieChar   *old_suffix, *p;

    /* adjust separate point in old path */
    old_tail = trie_da_get_tail_index (trie->da, sep_node);
    old_suffix = tail_get_suffix (trie->tail, old_tail);

    for (p = old_suffix; *p == *suffix; p++, suffix++)
        sep_node = da_insert_branch (trie->da, sep_node, *p);

    old_da = da_insert_branch (trie->da, sep_node, *p);
    if ('\0' != *p)
        ++p;
    tail_set_suffix (trie->tail, old_tail, p);
    trie_da_set_tail_index (trie->da, old_da, old_tail);

    free (old_suffix);

    /* insert the new branch at the new separate point */
    return trie_branch_in_branch (trie, sep_node, suffix, data);
}

/*-------------------------------*
 *   STEPWISE QUERY OPERATIONS   *
 *-------------------------------*/

TrieState *
trie_root (Trie *trie)
{
    return trie_state_new (trie, da_get_root (trie->da), 0, FALSE);
}

/*----------------*
 *   TRIE STATE   *
 *----------------*/

static TrieState *
trie_state_new (Trie *trie, TrieIndex index, short suffix_idx, short is_suffix)
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

Bool
trie_state_walk (TrieState *s, const TrieChar c)
{
    if (!s->is_suffix) {
        Bool ret;

        ret = da_walk (s->trie->da, &s->index, c);

        if (ret && trie_da_is_separate (s->trie->da, s->index)) {
            s->index = trie_da_get_tail_index (s->trie->da, s->index);
            s->suffix_idx = 0;
            s->is_suffix = TRUE;
        }

        return ret;
    } else {
        return tail_walk_char (s->trie->tail, s->index, &s->suffix_idx, c);
    }
}

Bool
trie_state_is_leaf (const TrieState *s)
{
    TrieState *t;
    Bool       ret;

    t = trie_state_clone (s);

    ret = trie_state_walk (t, TRIE_CHAR_TERM);

    trie_state_free (t);

    return ret;
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
