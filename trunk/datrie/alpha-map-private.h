/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * alpha-map-private.h - private APIs for alpha-map
 * Created: 2008-12-04
 * Author:  Theppitak Karoonboonyanan <thep@linux.thai.net>
 */

#ifndef __ALPHA_MAP_PRIVATE_H
#define __ALPHA_MAP_PRIVATE_H

#include <stdio.h>
#include "alpha-map.h"

AlphaMap *  alpha_map_read_bin (FILE *file);

int         alpha_map_write_bin (const AlphaMap *alpha_map, FILE *file);

TrieChar    alpha_map_char_to_trie (const AlphaMap *alpha_map,
                                    AlphaChar       ac);

AlphaChar   alpha_map_trie_to_char (const AlphaMap *alpha_map,
                                    TrieChar        tc);

TrieChar *  alpha_map_char_to_trie_str (const AlphaMap  *alpha_map,
                                        const AlphaChar *str);

AlphaChar * alpha_map_trie_to_char_str (const AlphaMap  *alpha_map,
                                        const TrieChar  *str);


#endif /* __ALPHA_MAP_PRIVATE_H */


/*
vi:ts=4:ai:expandtab
*/

