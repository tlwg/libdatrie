/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * libdatrie - Double-Array Trie Library
 * Copyright (C) 2013  Theppitak Karoonboonyanan <theppitak@gmail.com>
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
 * test_serialization.c - Test for datrie file and in-memory blob operations
 * Created: 2019-11-11
 * Author:  Theppitak Karoonboonyanan <theppitak@gmail.com> and KOLANICH <KOLANICH@users.noreply.github.com>
 */

#include <datrie/trie.h>
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define TRIE_FILENAME "test.tri"

int
main (void)
{
    Trie    *test_trie;
    DictRec *dict_p;

    msg_step ("Preparing trie");
    test_trie = en_trie_new ();
    if (!test_trie) {
        printf ("Failed to allocate test trie.\n");
        goto err_trie_not_created;
    }

    /* add/remove some words */
    for (dict_p = dict_src; dict_p->key; dict_p++) {
        if (!trie_store (test_trie, dict_p->key, dict_p->data)) {
            printf ("Failed to add key '%ls', data %d.\n",
                    (wchar_t *)dict_p->key, dict_p->data);
            goto err_trie_created;
        }
    }

    /* save & close */
    msg_step ("Saving trie to file");
    remove (TRIE_FILENAME);  /* error ignored */
    if (trie_save (test_trie, TRIE_FILENAME) != 0) {
        printf ("Failed to save trie to file '%s'.\n", TRIE_FILENAME);
        goto err_trie_created;
    }

    msg_step ("Getting serialized trie size");
    size_t size = trie_get_serialized_size (test_trie);
    printf ("serialized trie size %lu\n", size);
    msg_step ("Allocating");
    uint8 *trieSerializedData = malloc (size);
    if (!trieSerializedData) {
        printf ("Failed to allocate trieSerializedData.\n");
        goto err_trie_saved;
    }
    printf ("allocated %p\n", trieSerializedData);
    msg_step ("Serializing");
    trie_serialize (test_trie, trieSerializedData);
    msg_step ("Serialized");

    FILE *f = fopen (TRIE_FILENAME, "rb");
    if (!f) {
        printf ("Failed to reopen trie file " TRIE_FILENAME ".\n");
        goto err_serial_data_allocated;
    }
    fseek (f, 0, SEEK_END);
    size_t file_size = ftell (f);
    fseek (f, 0, SEEK_SET);

    if (size != file_size) {
        printf ("Trie serialized data doesn't match size of the file.\n");
        goto err_file_reopened;
    }

    unsigned char *trieFileData = malloc (size);
    if (!trieFileData) {
        printf ("Failed to allocate trieFileData.\n");
        goto err_file_reopened;
    }
    fread (trieFileData, 1, size, f);
    if (memcmp (trieSerializedData, trieFileData, size) != 0) {
        printf ("Trie serialized data doesn't match contents of the file.\n");
        goto err_file_data_allocated;
    }
    printf ("PASS!\n");

    free (trieFileData);
    fclose (f);
    free (trieSerializedData);
    remove (TRIE_FILENAME);
    trie_free (test_trie);
    return 0;

err_file_data_allocated:
    free (trieFileData);
err_file_reopened:
    fclose (f);
err_serial_data_allocated:
    free (trieSerializedData);
err_trie_saved:
    remove (TRIE_FILENAME);
err_trie_created:
    trie_free (test_trie);
err_trie_not_created:
    return 1;
}

/*
vi:ts=4:ai:expandtab
*/
