/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * alpha-map.h - map between character codes and trie alphabet
 * Created: 2006-08-19
 * Author:  Theppitak Karoonboonyanan <thep@linux.thai.net>
 */

#ifndef __ALPHA_MAP_H
#define __ALPHA_MAP_H

#include <stdio.h>

#include "typedefs.h"
#include "triedefs.h"

/**
 * @file alpha-map.h
 * @brief AlphaMap data type and functions
 */

/**
 * @brief AlphaMap data type
 */
typedef struct _AlphaMap    AlphaMap;

/**
 * @brief Create new alphabet map
 *
 * @return a pointer to the newly created alphabet map, NULL on failure
 *
 * Create a new empty alphabet map. The map contents can then be added with
 * alpha_map_add_range().
 *
 *  The created object must be freed with alpha_map_free().
 */
AlphaMap *  alpha_map_new ();

/**
 * @brief Create a clone of alphabet map
 *
 * @param a_map : the source alphabet map to clone
 *
 * @return a pointer to the alphabet map clone, NULL on failure
 *
 *  The created object must be freed with alpha_map_free().
 */
AlphaMap *  alpha_map_clone (const AlphaMap *a_map);

/**
 * @brief Free an alphabet map object
 *
 * @param alpha_map : the alphabet map object to free
 *
 * Destruct the @a alpha_map and free its allocated memory.
 */
void        alpha_map_free (AlphaMap *alpha_map);

/**
 * @brief Add a range to alphabet map
 *
 * @param alpha_map : the alphabet map object
 * @param begin     : the first character of the range
 * @param end       : the last character of the range
 *
 * Add a range of character codes from @a begin to @a end to the
 * alphabet set.
 */
int         alpha_map_add_range (AlphaMap  *alpha_map,
                                 AlphaChar  begin,
                                 AlphaChar  end);

/**
 * @brief Alphabet string length
 *
 * @param str   : the array of null-terminated AlphaChar string to measure
 *
 * @return the total characters in @a str.
 */
int         alpha_char_strlen (const AlphaChar *str);

#endif /* __ALPHA_MAP_H */


/*
vi:ts=4:ai:expandtab
*/
