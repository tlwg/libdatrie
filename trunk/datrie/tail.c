/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * tail.c - trie tail for keeping suffixes
 * Created: 2006-08-15
 * Author:  Theppitak Karoonboonyanan <thep@linux.thai.net>
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "tail.h"
#include "fileutils.h"

/*----------------------------------*
 *    INTERNAL TYPES DECLARATIONS   *
 *----------------------------------*/

/*-----------------------------------*
 *    PRIVATE METHODS DECLARATIONS   *
 *-----------------------------------*/

static TrieIndex    tail_alloc_block (Tail *t);
static void         tail_free_block (Tail *t, TrieIndex block);

/* ==================== BEGIN IMPLEMENTATION PART ====================  */

/*------------------------------------*
 *   INTERNAL TYPES IMPLEMENTATIONS   *
 *------------------------------------*/

/*------------------------------*
 *    PRIVATE DATA DEFINITONS   *
 *------------------------------*/

typedef struct {
    TrieIndex   next_free;
    TrieData    data;
    TrieChar   *suffix;
} TailBlock;

struct _Tail {
    TrieIndex   num_tails;
    TailBlock  *tails;
    TrieIndex   first_free;

    FILE       *file;
    Bool        is_dirty;
};

/*-----------------------------*
 *    METHODS IMPLEMENTAIONS   *
 *-----------------------------*/

#define TAIL_SIGNATURE  0xDFFD

Tail *
tail_open (const char *path, const char *name, TrieIOMode mode)
{
    Tail       *t;
    TrieIndex   i;

    t = (Tail *) malloc (sizeof (Tail));

    t->file = file_open (path, name, ".tl", mode);
    if (!t->file)
        goto exit1;

    if ((uint16) file_read_int16 (t->file) != TAIL_SIGNATURE)
        goto exit2;

    /* init tails data */
    if (file_length (t->file) == 0) {
        t->first_free = 0;
        t->num_tails  = 0;
        t->tails      = NULL;
        t->is_dirty   = TRUE;
    } else {
        t->first_free = file_read_int16 (t->file);
        t->num_tails = file_read_int16 (t->file);
        t->tails = (TailBlock *) malloc (t->num_tails * sizeof (TailBlock));
        if (!t->tails)
            goto exit2;
        for (i = 0; i < t->num_tails; i++) {
            int8    length;

            t->tails[i].next_free = file_read_int16 (t->file);
            t->tails[i].data      = file_read_int16 (t->file);

            length = file_read_int8 (t->file);
            t->tails[i].suffix    = (TrieChar *) malloc (length + 1);
            if (length > 0)
                file_read_chars (t->file, t->tails[i].suffix, length);
            t->tails[i].suffix[length] = '\0';
        }
        t->is_dirty = FALSE;
    }

    return t;

exit2:
    fclose (t->file);
exit1:
    free (t);
    return NULL;
}

int
tail_close (Tail *t)
{
    TrieIndex i;

    tail_save (t);
    fclose (t->file);
    if (t->tails) {
        for (i = 0; i < t->num_tails; i++)
            if (t->tails[i].suffix)
                free (t->tails[i].suffix);
        free (t->tails);
    }
    free (t);
}

int
tail_save (Tail *t)
{
    TrieIndex   i;

    if (!t->is_dirty)
        return 0;

    rewind (t->file);
    file_write_int16 (t->file, TAIL_SIGNATURE);
    file_write_int16 (t->file, t->first_free);
    file_write_int16 (t->file, t->num_tails);
    for (i = 0; i < t->num_tails; i++) {
        int8    length;

        file_write_int16 (t->file, t->tails[i].next_free);
        file_write_int16 (t->file, t->tails[i].data);

        length = strlen ((const char *) t->tails[i].suffix);
        file_write_int8 (t->file, length);
        if (length > 0)
            file_write_chars (t->file, t->tails[i].suffix, length);
    }
    t->is_dirty = FALSE;

    return 0;
}


TrieChar *
tail_get_suffix (const Tail *t, TrieIndex index)
{
    return (index < t->num_tails) ? t->tails[index].suffix : NULL;
}

Bool
tail_set_suffix (Tail *t, TrieIndex index, const TrieChar *suffix)
{
    if (index < t->num_tails) {
        t->tails[index].suffix = (TrieChar *) realloc (t->tails[index].suffix,
                                                       strlen (suffix) + 1);
        strcpy ((char *) t->tails[index].suffix, (const char *) suffix);
        t->is_dirty = TRUE;
        return TRUE;
    }
    return FALSE;
}

TrieIndex
tail_add_suffix (Tail *t, const TrieChar *suffix)
{
    TrieIndex   new_block;

    new_block = tail_alloc_block (t);
    tail_set_suffix (t, new_block, suffix);

    return new_block;
}

static TrieIndex
tail_alloc_block (Tail *t)
{
    TrieIndex   block;

    if (0 != t->first_free) {
        block = t->first_free;
        t->first_free = t->tails[block].next_free;
    } else {
        block = t->num_tails;
        t->tails = (TailBlock *) realloc (t->tails,
                                          ++t->num_tails * sizeof (TailBlock));
    }
    t->tails[block].next_free = -1;
    t->tails[block].data = TRIE_DATA_ERROR;
    t->tails[block].suffix = NULL;
    
    return block;
}

static void
tail_free_block (Tail *t, TrieIndex block)
{
    TrieIndex   i, j;

    if (block >= t->num_tails)
        return;

    t->tails[block].data = TRIE_DATA_ERROR;
    if (NULL != t->tails[block].suffix) {
        free (t->tails[block].suffix);
        t->tails[block].suffix = NULL;
    }

    /* find insertion point */
    j = 0;
    for (i = t->first_free; i != 0 && i < block; i = t->tails[i].next_free)
        j = i;

    /* insert free block between j and i */
    t->tails[block].next_free = i;
    if (0 != j)
        t->tails[j].next_free = block;
    else
        t->first_free = block;

    t->is_dirty = TRUE;
}

TrieData
tail_get_data (Tail *t, TrieIndex index)
{
    return (index < t->num_tails) ? t->tails[index].data : TRIE_DATA_ERROR;
}

Bool
tail_set_data (Tail *t, TrieIndex index, TrieData data)
{
    if (index < t->num_tails) {
        t->tails[index].data = data;
        t->is_dirty = TRUE;
        return TRUE;
    }
    return FALSE;
}

int
tail_walk_str  (Tail            *t,
                TrieIndex        s,
                short           *suffix_idx,
                const TrieChar  *str,
                int              len)
{
    const TrieChar *suffix;
    int             i;
    short           j;

    suffix = t->tails[s].suffix;
    i = 0; j = *suffix_idx;
    while (i < len) {
        if (str[i] != suffix[j])
            break;
        ++i;
        /* stop and stay at null-terminator */
        if (0 == suffix[j])
            break;
        ++j;
    }
    *suffix_idx = j;
    return i;
}

Bool
tail_walk_char (Tail            *t,
                TrieIndex        s,
                short           *suffix_idx,
                const TrieChar   c)
{
    TrieChar    suffix_char;

    suffix_char = t->tails[s].suffix[*suffix_idx];
    if (suffix_char == c) {
        if (0 != suffix_char)
            ++*suffix_idx;
        return TRUE;
    }
    return FALSE;
}

/*
vi:ts=4:ai:expandtab
*/
