/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * tail.h - trie tail for keeping suffixes
 * Created: 2006-08-12
 * Author:  Theppitak Karoonboonyanan <thep@linux.thai.net>
 */

#ifndef __TAIL_H
#define __TAIL_H

#include "triedefs.h"

/**
 * @file tail.h
 * @brief trie tail for keeping suffixes
 */

/**
 * @brief Double-array structure type
 */
typedef struct _Tail  Tail;

Tail *   tail_new ();

Tail *   tail_read (FILE *file);

void     tail_free (Tail *t);

int      tail_write (const Tail *t, FILE *file);


const TrieChar *    tail_get_suffix (const Tail *t, TrieIndex index);

Bool     tail_set_suffix (Tail *t, TrieIndex index, const TrieChar *suffix);

TrieIndex tail_add_suffix (Tail *t, const TrieChar *suffix);

TrieData tail_get_data (const Tail *t, TrieIndex index);

Bool     tail_set_data (Tail *t, TrieIndex index, TrieData data);

void     tail_delete (Tail *t, TrieIndex index);

int      tail_walk_str  (const Tail      *t,
                         TrieIndex        s,
                         short           *suffix_idx,
                         const TrieChar  *str,
                         int              len);

Bool     tail_walk_char (const Tail      *t,
                         TrieIndex        s,
                         short           *suffix_idx,
                         TrieChar         c);

/**
 * @brief Test walkability in tail with a character
 *
 * @param t          : the tail data
 * @param s          : the tail data index
 * @param suffix_idx : current character index in suffix
 * @param c          : the character to test walkability
 *
 * @return boolean indicating walkability
 *
 * Test if the character @a c can be used to walk from given character 
 * position @a suffix_idx of entry @a s of the tail data @a t.
 */
/*
Bool     tail_is_walkable_char (Tail            *t,
                                TrieIndex        s,
                                short            suffix_idx,
                                const TrieChar   c);
*/
#define  tail_is_walkable_char(t,s,suffix_idx,c) \
    (tail_get_suffix ((t), (s)) [suffix_idx] == (c))

#endif  /* __TAIL_H */

/*
vi:ts=4:ai:expandtab
*/
