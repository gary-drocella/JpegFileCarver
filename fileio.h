#ifndef FILEIO_H
#define FILEIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

struct File {
  uint32 size;
  byte* data;
};

/* Reads a file into a buffer. */
struct File* readFile(byte* fname);

/* Determines the size of a file. */
uint32 fsize(FILE* fp);

void writeFile(byte* fname, struct File* file);

#endif
