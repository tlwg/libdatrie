/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * libdatrie - Double-Array Trie Library
 * Copyright (C) 2018  Theppitak Karoonboonyanan <theppitak@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/*
 * test_byte_list.c - Test byte trie enumeration
 * Created: 2018-11-20
 * Author:  Theppitak Karoonboonyanan <theppitak@gmail.com>
 *          Based on test case in issue #9
 *          https://github.com/tlwg/libdatrie/issues/9
 */

#include <datrie/trie.h>
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct _DictEntry DictEntry;
struct _DictEntry {
    AlphaChar   key[4];
    TrieData    data;
    int         is_checked;
};

#define N_ELM(v) (sizeof(v)/sizeof(v[0]))

/* Dictionary source */
static DictEntry Source[] = {
    { { '1', '2', 0 },      1, 0 },
    { { '1', '2', '3', 0 }, 2, 0 },
};

static void
dump_key_data (const AlphaChar *key, TrieData data)
{
    const AlphaChar *p;

    printf ("[");
    for (p = key; *p; ++p) {
        if (p != key) {
            printf (", ");
        }
        printf ("%04x", *p);
    }
    printf ("] : %d\n", data);
}

static void
dump_entry (const TrieIterator *iter)
{
    AlphaChar *key = trie_iterator_get_key (iter);
    dump_key_data (key, trie_iterator_get_data (iter));
    free (key);
}

/*
 * Check if the trie entry referenced by iter match any Source[] element
 * and mark the matched element as checked.
 * Return: 1 if matched, 0 otherwise
 */
static int
validate_entry (const TrieIterator *iter)
{
    AlphaChar *key = trie_iterator_get_key (iter);
    TrieData   data = trie_iterator_get_data (iter);
    int i;

    for (i = 0; i < N_ELM (Source); i++) {
        if (alpha_char_strcmp (Source[i].key, key) == 0
            && Source[i].data == data)
        {
            Source[i].is_checked = 1;
            free (key);
            return 1;
        }
    }
    free (key);
    return 0;
}

/*
 * Check if all Source[] elements are checked and reported unchecked one.
 * Return: 1 if all are checked, 0 otherwise.
 */
static int
is_all_checked()
{
    int i;
    int ret = 1;

    for (i = 0; i < N_ELM (Source); i++) {
        if (!Source[i].is_checked) {
            printf ("Not visited Source entry: ");
            dump_key_data (Source[i].key, Source[i].data);
            ret = 0;
        }
    }

    return ret;
}

int
main (void)
{
    AlphaMap     *alpha_map;
    Trie         *test_trie;
    int           i;
    TrieState    *root;
    TrieIterator *iter;
    int           ret = 0;

    msg_step ("Preparing alpha map");
    alpha_map = alpha_map_new ();
    if (!alpha_map) {
        printf ("Fail to allocate alpha map\n");
        goto err_alpha_map_not_created;
    }
    if (alpha_map_add_range (alpha_map, 0x00, 0xff) != 0) {
        printf ("Fail to add full alpha map range\n");
        goto err_alpha_map_created;
    }

    msg_step ("Preparing trie");
    test_trie = trie_new (alpha_map);
    alpha_map_free (alpha_map);
    if (!test_trie) {
        printf ("Fail to create test trie\n");
        goto err_alpha_map_created;
    }

    msg_step ("Storing entries to test trie");
    for (i = 0; i < N_ELM (Source); i++) {
        if (!trie_store (test_trie, Source[i].key, Source[i].data)) {
            printf ("Fail to store entry %d to test trie:\n", i);
            dump_key_data (Source[i].key, Source[i].data);
            goto err_trie_created;
        }
    }

    msg_step ("Iterating trie");
    root = trie_root (test_trie);
    iter = trie_iterator_new (root);
    while (trie_iterator_next (iter)) {
        if (!validate_entry (iter)) {
            printf ("Fail to validate trie entry:\n");
            dump_entry (iter);
            ret = 1;
        }
    }
    if (!is_all_checked()) {
        ret = 1;
    }
    trie_iterator_free (iter);
    trie_state_free (root);

    msg_step ("Freeing test trie");
    trie_free (test_trie);
    return ret;

err_trie_created:
    trie_free (test_trie);
err_alpha_map_created:
    alpha_map_free (alpha_map);
err_alpha_map_not_created:
    return 1;
}

/*
vi:ts=4:ai:expandtab
*/
