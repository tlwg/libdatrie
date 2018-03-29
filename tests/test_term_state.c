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
 * test_term_state.c - Test data retrieval from terminal state
 * Created: 2018-03-29
 * Author:  Theppitak Karoonboonyanan <theppitak@gmail.com>
 */

#include <datrie/trie.h>
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Test trie
 *
 * (1) -a-> (2) -b-> (3) -#-> [4] {data=1}
 *                    |
 *                    +---c-> (5) -#-> [6] {data=2}
 *
 */
int
main ()
{
    Trie         *test_trie;
    TrieState    *trie_state;
    TrieData      data;
    Bool          is_failed;

    msg_step ("Preparing trie");
    test_trie = en_trie_new ();
    if (!test_trie) {
        printf ("Fail to create test trie\n");
        goto err_trie_not_created;
    }

    /* populate trie */
    msg_step ("Populating trie with test set");
    if (!trie_store (test_trie, (AlphaChar *)L"ab", 1)) {
        printf ("Failed to add key 'ab', data 1.\n");
        goto err_trie_created;
    }
    if (!trie_store (test_trie, (AlphaChar *)L"abc", 2)) {
        printf ("Failed to add key 'abc', data 2.\n");
        goto err_trie_created;
    }

    is_failed = FALSE;

    /* try retrieving data */
    msg_step ("Preparing root state");
    trie_state = trie_root (test_trie);
    if (!trie_state) {
        printf ("Failed to get trie root state\n");
        goto err_trie_created;
    }

    msg_step ("Try walking from root with 'a'");
    if (!trie_state_walk (trie_state, (AlphaChar)L'a')) {
        printf ("Failed to walk from root with 'a'.\n");
        is_failed = TRUE;
    }

    data = trie_state_get_data (trie_state);
    if (data != TRIE_DATA_ERROR) {
        printf ("Retrieved data at 'a' is %d, not %d.\n",
                data, TRIE_DATA_ERROR);
        is_failed = TRUE;
    }

    msg_step ("Try walking further with 'b'");
    if (!trie_state_walk (trie_state, (AlphaChar)L'b')) {
        printf ("Failed to continue walking with 'b'.\n");
        is_failed = TRUE;
    }

    data = trie_state_get_data (trie_state);
    if (data != 1) {
        printf ("Retrieved data for key 'ab' is %d, not 1.\n", data);
        is_failed = TRUE;
    }

    msg_step ("Try walking further with 'c'");
    if (!trie_state_walk (trie_state, (AlphaChar)L'c')) {
        printf ("Failed to continue walking with 'c'.\n");
        is_failed = TRUE;
    }

    data = trie_state_get_data (trie_state);
    if (data != 2) {
        printf ("Retrieved data for key 'abc' is %d, not 2.\n", data);
        is_failed = TRUE;
    }

    trie_state_free (trie_state);

    if (is_failed) {
        printf ("Errors found in terminal state data retrieval.\n");
        goto err_trie_created;
    }

    trie_free (test_trie);
    return 0;

err_trie_created:
    trie_free (test_trie);
err_trie_not_created:
    return 1;
}

/*
vi:ts=4:ai:expandtab
*/
