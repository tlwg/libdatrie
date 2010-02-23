/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * darray.h - Double-array trie structure
 * Created: 2006-08-11
 * Author:  Theppitak Karoonboonyanan <thep@linux.thai.net>
 */

#ifndef __DARRAY_H
#define __DARRAY_H

#include "triedefs.h"

/**
 * @file darray.h
 * @brief Double-array trie structure
 */

/**
 * @brief Double-array structure type
 */
typedef struct _DArray  DArray;

/**
 * @brief Double-array entry enumeration function
 *
 * @param key       : the key of the entry, up to @a sep_node
 * @param sep_node  : the separate node of the entry
 * @param user_data : user-supplied data
 *
 * @return TRUE to continue enumeration, FALSE to stop
 */
typedef Bool (*DAEnumFunc) (const TrieChar   *key,
                            TrieIndex         sep_node,
                            void             *user_data);


DArray * da_new ();

DArray * da_read (FILE *file);

void     da_free (DArray *d);

int      da_write (const DArray *d, FILE *file);


TrieIndex  da_get_root (const DArray *d);


TrieIndex  da_get_base (const DArray *d, TrieIndex s);

TrieIndex  da_get_check (const DArray *d, TrieIndex s);


void       da_set_base (DArray *d, TrieIndex s, TrieIndex val);

void       da_set_check (DArray *d, TrieIndex s, TrieIndex val);

Bool       da_walk (const DArray *d, TrieIndex *s, TrieChar c);

/**
 * @brief Test walkability in double-array structure
 *
 * @param d : the double-array structure
 * @param s : current state
 * @param c : the input character
 *
 * @return boolean indicating walkability
 *
 * Test if there is a transition from state @a s with input character @a c.
 */
/*
Bool       da_is_walkable (DArray *d, TrieIndex s, TrieChar c);
*/
#define    da_is_walkable(d,s,c) \
    (da_get_check ((d), da_get_base ((d), (s)) + (c)) == (s))

TrieIndex  da_insert_branch (DArray *d, TrieIndex s, TrieChar c);

void       da_prune (DArray *d, TrieIndex s);

void       da_prune_upto (DArray *d, TrieIndex p, TrieIndex s);

Bool    da_enumerate (const DArray *d, DAEnumFunc enum_func, void *user_data);

#endif  /* __DARRAY_H */

/*
vi:ts=4:ai:expandtab
*/
