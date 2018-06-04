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
 * test_byte_alpha.c - Test byte stream (full-range 0..255) alpha map
 * Created: 2018-04-21
 * Author:  Theppitak Karoonboonyanan <theppitak@gmail.com>
 *          Based on test case in issue #6
 *          https://github.com/tlwg/libdatrie/issues/6
 */

#include <datrie/trie.h>
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

#define TEST_DATA 255
int
main (void)
{
    AlphaMap    *alpha_map;
    Trie        *test_trie;
    AlphaChar    key[3];
    TrieData     data;

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

    msg_step ("Storing key to test trie");
    key[0] = 0xff;
    key[1] = 0xff;
    key[2] = 0;
    if (!trie_store (test_trie, key, TEST_DATA)) {
        printf ("Fail to store key to test trie\n");
        goto err_trie_created;
    }

    msg_step ("Retrieving data from test trie");
    if (!trie_retrieve (test_trie, key, &data)) {
        printf ("Fail to retrieve key from test trie\n");
        goto err_trie_created;
    }
    if (TEST_DATA != data) {
        printf ("Retrieved data = %d, not %d\n", data, TEST_DATA);
        goto err_trie_created;
    }

    msg_step ("Freeing test trie");
    trie_free (test_trie);
    return 0;

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
