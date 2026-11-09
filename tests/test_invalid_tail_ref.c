/* Reject a serialized double-array node pointing past the tail blocks. */
#include <datrie/trie.h>
#include <stdio.h>

#define TEST_FILE "test_invalid_tail_ref.tri"

int
main (void)
{
    const AlphaChar key[] = { 'a', 'b', 0 };
    const unsigned char bad_base[] = { 0xff, 0xff, 0xff, 0x9c };
    unsigned char base[4];
    AlphaMap *map = NULL;
    Trie *trie = NULL;
    FILE *file = NULL;
    TrieData data;
    int failed = 1;

    map = alpha_map_new ();
    if (!map || alpha_map_add_range (map, 'a', 'z') != 0)
        goto done;
    trie = trie_new (map);
    if (!trie || !trie_store (trie, key, 7) || trie_save (trie, TEST_FILE) != 0)
        goto done;
    trie_free (trie);
    trie = trie_new_from_file (TEST_FILE);
    if (!trie || !trie_retrieve (trie, key, &data) || data != 7)
        goto done;
    trie_free (trie);
    trie = NULL;

    file = fopen (TEST_FILE, "r+b");
    if (!file || fseek (file, 48, SEEK_SET) != 0 ||
        fread (base, 1, sizeof base, file) != sizeof base ||
        base[0] != 0xff || base[1] != 0xff ||
        base[2] != 0xff || base[3] != 0xff ||
        fseek (file, 48, SEEK_SET) != 0 ||
        fwrite (bad_base, 1, sizeof bad_base, file) != sizeof bad_base)
        goto done;
    fclose (file);
    file = NULL;

    trie = trie_new_from_file (TEST_FILE);
    if (trie)
        goto done;
    file = fopen (TEST_FILE, "rb");
    if (!file)
        goto done;
    trie = trie_fread (file);
    if (trie)
        goto done;
    failed = 0;

done:
    if (file)
        fclose (file);
    if (trie)
        trie_free (trie);
    if (map)
        alpha_map_free (map);
    remove (TEST_FILE);
    return failed;
}
