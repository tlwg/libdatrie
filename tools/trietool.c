/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * trietool.c - Trie manipulation tool
 * Created: 2006-08-15
 * Author:  Theppitak Karoonboonyanan <thep@linux.thai.net>
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <config.h>
#include <datrie/trie.h>

typedef struct {
    const char *path;
    const char *trie_name;
    Trie       *trie;
} ProgEnv;

static int  decode_switch       (int argc, char *argv[], ProgEnv *env);
static int  decode_command      (int argc, char *argv[], ProgEnv *env);

static int  command_add         (int argc, char *argv[], ProgEnv *env);
static int  command_add_list    (int argc, char *argv[], ProgEnv *env);
static int  command_delete      (int argc, char *argv[], ProgEnv *env);
static int  command_delete_list (int argc, char *argv[], ProgEnv *env);
static int  command_query       (int argc, char *argv[], ProgEnv *env);
static int  command_list        (int argc, char *argv[], ProgEnv *env);

static void usage               (const char *prog_name, int exit_status);

int
main (int argc, char *argv[])
{
    int     i;
    ProgEnv env;
    int     ret;

    env.path = ".";

    i = decode_switch (argc, argv, &env);
    if (i == argc)
        usage (argv[0], EXIT_FAILURE);

    env.trie_name = argv[i++];
    env.trie = trie_open (env.path, env.trie_name,
                          TRIE_IO_READ | TRIE_IO_WRITE | TRIE_IO_CREATE);
    if (!env.trie) {
        fprintf (stderr, "Cannot open trie '%s' at '%s'\n",
                 env.trie_name, env.path);
        exit (EXIT_FAILURE);
    }

    ret = decode_command (argc - i, argv + i, &env);

    trie_close (env.trie);

    return ret;
}

static int
decode_switch (int argc, char *argv[], ProgEnv *env)
{
    int opt_idx;

    for (opt_idx = 1; opt_idx < argc && *argv[opt_idx] == '-'; opt_idx++) {
        if (strcmp (argv[opt_idx], "-h") == 0 ||
            strcmp (argv[opt_idx], "--help") == 0)
        {
            usage (argv[0], EXIT_FAILURE);
        } else if (strcmp (argv[opt_idx], "-V") == 0 ||
                   strcmp (argv[opt_idx], "--version") == 0)
        {
            printf ("%s\n", VERSION);
            exit (EXIT_FAILURE);
        } else if (strcmp (argv[opt_idx], "-p") == 0 ||
                   strcmp (argv[opt_idx], "--path") == 0)
        {
            env->path = argv[++opt_idx];
        } else if (strcmp (argv[opt_idx], "--") == 0) {
            ++opt_idx;
            break;
        } else {
            fprintf (stderr, "Unknown option: %s\n", argv[opt_idx]);
            exit (EXIT_FAILURE);
        }
    }

    return opt_idx;
}

static int
decode_command (int argc, char *argv[], ProgEnv *env)
{
    int opt_idx;

    for (opt_idx = 0; opt_idx < argc; opt_idx++) {
        if (strcmp (argv[opt_idx], "add") == 0) {
            ++opt_idx;
            opt_idx += command_add (argc - opt_idx, argv + opt_idx, env);
        } else if (strcmp (argv[opt_idx], "add-list") == 0) {
            ++opt_idx;
            opt_idx += command_add_list (argc - opt_idx, argv + opt_idx, env);
        } else if (strcmp (argv[opt_idx], "delete") == 0) {
            ++opt_idx;
            opt_idx += command_delete (argc - opt_idx, argv + opt_idx, env);
        } else if (strcmp (argv[opt_idx], "delete-list") == 0) {
            ++opt_idx;
            opt_idx += command_delete_list (argc - opt_idx, argv + opt_idx, env);
        } else if (strcmp (argv[opt_idx], "query") == 0) {
            ++opt_idx;
            opt_idx += command_query (argc - opt_idx, argv + opt_idx, env);
        } else if (strcmp (argv[opt_idx], "list") == 0) {
            ++opt_idx;
            opt_idx += command_list (argc - opt_idx, argv + opt_idx, env);
        } else {
            fprintf (stderr, "Unknown command: %s\n", argv[opt_idx]);
            exit (EXIT_FAILURE);
        }
    }
}

static int
command_add (int argc, char *argv[], ProgEnv *env)
{
    int opt_idx;

    opt_idx = 0;
    while (opt_idx < argc) {
        const TrieChar *key;
        TrieData        data;

        key = (const TrieChar *) argv[opt_idx++];
        data = (opt_idx < argc) ? atoi (argv[opt_idx++]) : TRIE_DATA_ERROR;

        if (!trie_store (env->trie, key, data)) {
            fprintf (stderr, "Failed to add entry '%s' with data %d\n",
                     key, data);
        }
    }

    return opt_idx;
}

static int
command_add_list (int argc, char *argv[], ProgEnv *env)
{
    /* TODO: implement it */
    fprintf (stderr, "add-list: Function not implemented.\n");

    return 1;
}

static int
command_delete (int argc, char *argv[], ProgEnv *env)
{
    int opt_idx;

    for (opt_idx = 0; opt_idx < argc; opt_idx++)
        if (!trie_delete (env->trie, (const TrieChar *) argv[opt_idx]))
            fprintf (stderr, "No entry '%s'. Not deleted.\n", argv[opt_idx]);

    return opt_idx;
}

static int
command_delete_list (int argc, char *argv[], ProgEnv *env)
{
    /* TODO: implement it */
    fprintf (stderr, "delete-list: Function not implemented.\n");

    return 1;
}

static int
command_query (int argc, char *argv[], ProgEnv *env)
{
    TrieData    data;

    if (argc == 0) {
        fprintf (stderr, "query: No key specified.\n");
        return 0;
    }

    if (trie_retrieve (env->trie, (const TrieChar *) argv[0], &data)) {
        printf ("%d\n", data);
    } else {
        fprintf (stderr, "query: Key '%s' not found.\n", argv[0]);
    }

    return 1;
}

static Bool
list_enum_func (const TrieChar *key, TrieData key_data, void *user_data)
{
    printf ("%s\t%6d\n", key, key_data);
}

static int
command_list (int argc, char *argv[], ProgEnv *env)
{
    trie_enumerate (env->trie, list_enum_func, (void *) 0);
    return 0;
}


static void
usage (const char *prog_name, int exit_status)
{
    printf ("%s - double-array trie manipulator\n", prog_name);
    printf ("Usage: %s [OPTION]... TRIE CMD ARG ...\n", prog_name);
    printf (
        "Options:\n"
        "  -p, --path DIR           set trie directory to DIR [default=.]\n"
        "  -h, --help               display this help and exit\n"
        "  -V, --version            output version information and exit\n"
        "\n"
        "Commands:\n"
        "  add  WORD DATA ...       add WORD with DATA to trie\n"
        "  add-list LISTFILE        add WORD and DATA from LISTFILE to trie\n"
        "  delete WORD ...          delete WORD from trie\n"
        "  delete-list LISTFILE     delete words listed in LISTFILE from trie\n"
        "  query WORD               query WORD data from trie\n"
        "  list                     list all words in trie\n"
    );

    exit (exit_status);
}

/*
vi:ts=4:ai:expandtab
*/
