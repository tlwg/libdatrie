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
 * @brief Open double-array from file
 *
 * @param path : the path that stores the double-array files
 * @param name : the name of the double-array (not actual file name)
 * @param mode : openning mode, read or write
 *
 * @return a pointer to the openned double-array, NULL on failure
 *
 * Open a double-array structure of given name. Note that @a name here does 
 * not mean the actual file name. Rather, the file name will be inferred by 
 * the name.
 */
DArray * da_open (const char *path, const char *name, TrieIOMode mode);

/**
 * @brief Close double-array data
 *
 * @param d : the double-array data
 *
 * @return 0 on success, non-zero on failure
 *
 * Close the given double-array data. If @a d was openned for writing, all 
 * pending changes will be saved to file.
 */
int      da_close (DArray *d);

/**
 * @brief Save double-array data
 *
 * @param d : the double-array data
 *
 * @return 0 on success, non-zero on failure
 *
 * If @a double-array data was openned for writing, save all pending changes 
 * to file.
 */
int      da_save (DArray *d);


/**
 * @brief Get root state
 *
 * @param d     : the double-array data
 *
 * @return root state of the @a index set, or TRIE_INDEX_ERROR on failure
 *
 * Get root state for stepwise walking.
 */
TrieIndex  da_get_root (const DArray *d);


/**
 * @brief Get BASE cell
 *
 * @param d : the double-array data
 * @param s : the double-array state to get data
 *
 * @return the BASE cell value for the given state
 *
 * Get BASE cell value for the given state.
 */
TrieIndex  da_get_base (const DArray *d, TrieIndex s);

/**
 * @brief Get CHECK cell
 *
 * @param d : the double-array data
 * @param s : the double-array state to get data
 *
 * @return the CHECK cell value for the given state
 *
 * Get CHECK cell value for the given state.
 */
TrieIndex  da_get_check (const DArray *d, TrieIndex s);


/**
 * @brief Set BASE cell
 *
 * @param d   : the double-array data
 * @param s   : the double-array state to get data
 * @param val : the value to set
 *
 * Set BASE cell for the given state to the given value.
 */
void       da_set_base (DArray *d, TrieIndex s, TrieIndex val);

/**
 * @brief Set CHECK cell
 *
 * @param d   : the double-array data
 * @param s   : the double-array state to get data
 * @param val : the value to set
 *
 * Set CHECK cell for the given state to the given value.
 */
void       da_set_check (DArray *d, TrieIndex s, TrieIndex val);

/**
 * @brief Walk in double-array structure
 *
 * @param d : the double-array structure
 * @param s : current state
 * @param c : the input character
 *
 * @return boolean indicating success
 *
 * Walk the double-array trie from state @a *s, using input character @a c.
 * If there exists an edge from @a *s with arc labeled @a c, this function
 * returns TRUE and @a *s is updated to the new state. Otherwise, it returns
 * FALSE and @a *s is left unchanged.
 */
Bool       da_walk (DArray *d, TrieIndex *s, TrieChar c);

/**
 * @brief Insert a branch from trie node
 *
 * @param d : the double-array structure
 * @param s : the state to add branch to
 * @param c : the character for the branch label
 *
 * @return the index of the new node
 *
 * Insert a new arc labelled with character @a c from the trie node 
 * represented by index @a s in double-array structure @a d.
 * Note that it assumes that no such arc exists before inserting.
 */
TrieIndex  da_insert_branch (DArray *d, TrieIndex s, TrieChar c);

#endif  /* __DARRAY_H */

/*
vi:ts=4:ai:expandtab
*/
