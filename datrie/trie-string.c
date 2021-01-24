/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * libdatrie - Double-Array Trie Library
 * Copyright (C) 2006  Theppitak Karoonboonyanan <theppitak@gmail.com>
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
 * trie-string.c - Dynamic string type for Trie alphabets
 * Created: 2012-08-02
 * Author:  Theppitak Karoonboonyanan <theppitak@gmail.com>
 */

#include "trie-string.h"
#include "dstring-private.h"
#include "triedefs.h"

#include <string.h>
#include <stdlib.h>

/*----------------------------*
 *   STATIC TRIECHAR STRING   *
 *----------------------------*/

size_t
trie_char_strlen (const TrieChar *str)
{
    size_t len = 0;
    while (*str++ != TRIE_CHAR_TERM) {
        ++len;
    }
    return len;
}

size_t
trie_char_strsize (const TrieChar *str)
{
    return trie_char_strlen (str) * sizeof (TrieChar);
}

TrieChar *
trie_char_strdup (const TrieChar *str)
{
    TrieChar *dup
        = (TrieChar *) malloc (sizeof (TrieChar) * (trie_char_strlen (str) + 1));
    TrieChar *p = dup;

    while (*str != TRIE_CHAR_TERM) {
        *p++ = *str++;
    }
    *p = TRIE_CHAR_TERM;

    return dup;
}

/*-----------------------------*
 *   DYNAMIC TRIECHAR STRING   *
 *-----------------------------*/

struct _TrieString {
    DString ds;
};


TrieString *
trie_string_new (int n_elm)
{
    return (TrieString *) dstring_new (sizeof (TrieChar), n_elm);
}

void
trie_string_free (TrieString *ts)
{
    dstring_free ((DString *)ts);
}

int
trie_string_length (const TrieString *ts)
{
    return dstring_length ((DString *)ts);
}

const void *
trie_string_get_val (const TrieString *ts)
{
    return dstring_get_val ((DString *)ts);
}

void *
trie_string_get_val_rw (TrieString *ts)
{
    return dstring_get_val_rw ((DString *)ts);
}

void
trie_string_clear (TrieString *ts)
{
    dstring_clear ((DString *)ts);
}

Bool
trie_string_copy (TrieString *dst, const TrieString *src)
{
    return dstring_copy ((DString *)dst, (const DString *)src);
}

Bool
trie_string_append (TrieString *dst, const TrieString *src)
{
    return dstring_append ((DString *)dst, (const DString *)src);
}

Bool
trie_string_append_string (TrieString *ts, const TrieChar *str)
{
    return dstring_append_string ((DString *)ts,
                                  str, strlen ((const char *)str));
}

Bool
trie_string_append_char (TrieString *ts, TrieChar tc)
{
    return dstring_append_char ((DString *)ts, &tc);
}

Bool
trie_string_terminate (TrieString *ts)
{
    static const TrieChar term = TRIE_CHAR_TERM;
    return dstring_append_char ((DString *)ts, &term);
}

Bool
trie_string_cut_last (TrieString *ts)
{
    return dstring_cut_last ((DString *)ts);
}

/*
vi:ts=4:ai:expandtab
*/
