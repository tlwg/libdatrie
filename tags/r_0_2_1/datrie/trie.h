/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * trie.h - Trie data type and functions
 * Created: 2006-08-11
 * Author:  Theppitak Karoonboonyanan <thep@linux.thai.net>
 */

#ifndef __TRIE_H
#define __TRIE_H

#include <datrie/triedefs.h>
#include <datrie/alpha-map.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file trie.h
 * @brief Trie data type and functions
 */

/**
 * @brief Trie data type
 */
typedef struct _Trie   Trie;

/**
 * @brief Trie enumeration function
 *
 * @param key  : the key of the entry
 * @param data : the data of the entry
 *
 * @return TRUE to continue enumeration, FALSE to stop
 */
typedef Bool (*TrieEnumFunc) (const AlphaChar  *key,
                              TrieData          key_data,
                              void             *user_data);

/**
 * @brief Trie walking state
 */
typedef struct _TrieState TrieState;

/*-----------------------*
 *   GENERAL FUNCTIONS   *
 *-----------------------*/

/**
 * @brief Create a new trie
 *
 * @param   alpha_map   : the alphabet set for the trie
 *
 * @return a pointer to the newly created trie, NULL on failure
 *
 * Create a new empty trie object based on the given @a alpha_map alphabet
 * set. The trie contents can then be added and deleted with trie_store() and
 * trie_delete() respectively.
 *
 * The created object must be freed with trie_free().
 */
Trie *  trie_new (const AlphaMap *alpha_map);

/**
 * @brief Create a new trie by loading from a file
 *
 * @param path  : the path to the file
 *
 * @return a pointer to the created trie, NULL on failure
 *
 * Create a new trie and initialize its contents by loading from the file at
 * given @a path.
 *
 * The created object must be freed with trie_free().
 */
Trie *  trie_new_from_file (const char *path);

/**
 * @brief Free a trie object
 *
 * @param trie  : the trie object to free
 *
 * Destruct the @a trie and free its allocated memory.
 */
void    trie_free (Trie *trie);

/**
 * @brief Save a trie to file
 *
 * @param trie  : the trie
 *
 * @param path  : the path to the file
 *
 * @return 0 on success, non-zero on failure
 *
 * Create a new file at the given @a path and write @a trie data to it.
 * If @a path already exists, its contents will be replaced.
 */
int     trie_save (Trie *trie, const char *path);

/**
 * @brief Check pending changes
 *
 * @param trie  : the trie object
 *
 * @return TRUE if there are pending changes, FALSE otherwise
 *
 * Check if the @a trie is dirty with some pending changes and needs saving
 * to synchronize with the file.
 */
Bool    trie_is_dirty (const Trie *trie);


/*------------------------------*
 *   GENERAL QUERY OPERATIONS   *
 *------------------------------*/

/**
 * @brief Retrieve an entry from trie
 *
 * @param trie   : the trie
 * @param key    : the key for the entry to retrieve
 * @param o_data : the storage for storing the entry data on return
 *
 * @return boolean value indicating the existence of the entry.
 *
 * Retrieve an entry for the given @a key from @a trie. On return,
 * if @a key is found and @a o_data is not NULL, @a *o_data is set
 * to the data associated to @a key.
 */
Bool    trie_retrieve (const Trie      *trie,
                       const AlphaChar *key,
                       TrieData        *o_data);

/**
 * @brief Store a value for  an entry to trie
 *
 * @param trie  : the trie
 * @param key   : the key for the entry to retrieve
 * @param data  : the data associated to the entry
 *
 * @return boolean value indicating the success of the process
 *
 * Store a @a data for the given @a key in @a trie. If @a key does not 
 * exist in @a trie, it will be appended. If it does, its current data will
 * be overwritten.
 */
Bool    trie_store (Trie *trie, const AlphaChar *key, TrieData data);

/**
 * @brief Delete an entry from trie
 *
 * @param trie  : the trie
 * @param key   : the key for the entry to delete
 *
 * @return boolean value indicating whether the key exists and is removed
 *
 * Delete an entry for the given @a key from @a trie.
 */
Bool    trie_delete (Trie *trie, const AlphaChar *key);

/**
 * @brief Enumerate entries in trie
 *
 * @param trie       : the trie
 * @param enum_func  : the callback function to be called on each key
 * @param user_data  : user-supplied data to send as an argument to @a enum_func
 *
 * @return boolean value indicating whether all the keys are visited
 *
 * Enumerate all entries in trie. For each entry, the user-supplied 
 * @a enum_func callback function is called, with the entry key and data.
 * Returning FALSE from such callback will stop enumeration and return FALSE.
 */
Bool    trie_enumerate (const Trie     *trie,
                        TrieEnumFunc    enum_func,
                        void           *user_data);


/*-------------------------------*
 *   STEPWISE QUERY OPERATIONS   *
 *-------------------------------*/

/**
 * @brief Get root state of a trie
 *
 * @param trie : the trie
 *
 * @return the root state of the trie
 *
 * Get root state of @a trie, for stepwise walking.
 *
 * The returned state is allocated and must be freed with trie_state_free()
 */
TrieState * trie_root (const Trie *trie);


/*----------------*
 *   TRIE STATE   *
 *----------------*/

/**
 * @brief Clone a trie state
 *
 * @param s    : the state to clone
 *
 * @return an duplicated instance of @a s
 *
 * Make a copy of trie state.
 *
 * The returned state is allocated and must be freed with trie_state_free()
 */
TrieState * trie_state_clone (const TrieState *s);

/**
 * @brief Copy trie state to another
 *
 * @param dst  : the destination state
 * @param src  : the source state
 *
 * Copy trie state data from @a src to @a dst. All existing data in @a dst
 * is overwritten.
 */
void        trie_state_copy (TrieState *dst, const TrieState *src);

/**
 * @brief Free a trie state
 *
 * @param s    : the state to free
 *
 * Free the trie state.
 */
void      trie_state_free (TrieState *s);

/**
 * @brief Rewind a trie state
 *
 * @param s    : the state to rewind
 *
 * Put the state at root.
 */
void      trie_state_rewind (TrieState *s);

/**
 * @brief Walk the trie from the state
 *
 * @param s    : current state
 * @param c    : key character for walking
 *
 * @return boolean value indicating the success of the walk
 *
 * Walk the trie stepwise, using a given character @a c.
 * On return, the state @a s is updated to the new state if successfully walked.
 */
Bool      trie_state_walk (TrieState *s, AlphaChar c);

/**
 * @brief Test walkability of character from state
 *
 * @param s    : the state to check
 * @param c    : the input character
 *
 * @return boolean indicating walkability
 *
 * Test if there is a transition from state @a s with input character @a c.
 */
Bool      trie_state_is_walkable (const TrieState *s, AlphaChar c);

/**
 * @brief Check for terminal state
 *
 * @param s    : the state to check
 *
 * @return boolean value indicating whether it is a terminal state
 *
 * Check if the given state is a terminal state. A terminal state is a trie
 * state that terminates a key, and stores a value associated with it.
 */
#define   trie_state_is_terminal(s) trie_state_is_walkable((s),TRIE_CHAR_TERM)

/**
 * @brief Check for single path
 *
 * @param s    : the state to check
 *
 * @return boolean value indicating whether it is in a single path
 *
 * Check if the given state is in a single path, that is, there is no other
 * branch from it to leaf.
 */
Bool      trie_state_is_single (const TrieState *s);

/**
 * @brief Check for leaf state
 *
 * @param s    : the state to check
 *
 * @return boolean value indicating whether it is a leaf state
 *
 * Check if the given state is a leaf state. A leaf state is a terminal state 
 * that has no other branch.
 */
#define   trie_state_is_leaf(s) \
    (trie_state_is_single(s) && trie_state_is_terminal(s))

/**
 * @brief Get data from leaf state
 *
 * @param s    : a leaf state
 *
 * @return the data associated with the leaf state @a s,
 *         or TRIE_DATA_ERROR if @a s is not a leaf state
 *
 * Get value from a leaf state of trie. Getting value from a non-leaf state
 * will result in TRIE_DATA_ERROR.
 */
TrieData trie_state_get_data (const TrieState *s);

#ifdef __cplusplus
}
#endif

#endif  /* __TRIE_H */

/*
vi:ts=4:ai:expandtab
*/
