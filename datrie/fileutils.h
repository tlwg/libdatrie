/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * fileutils.h - File utility functions
 * Created: 2006-08-14
 * Author:  Theppitak Karoonboonyanan <thep@linux.thai.net>
 */

#ifndef __FILEUTILS_H
#define __FILEUTILS_H

#include <stdio.h>

#include "triedefs.h"

FILE * file_open (const char *path, const char *name, const char *ext,
                  TrieIOMode mode);

long   file_length (FILE *file);

int16  file_read_int16 (FILE *file);
Bool   file_write_int16 (FILE *file, int16 val);

#endif /* __FILEUTILS_H */

/*
vi:ts=4:ai:expandtab
*/
