#include "fileio.h"

uint32 fsize(FILE* fp) {
  uint32 prev = ftell(fp);
  fseek(fp, 0L, SEEK_END);
  uint32 sz = ftell(fp);
  fseek(fp, prev, SEEK_SET);

  return sz;
}

struct File* readFile(byte* fname) {
  FILE* fp = fopen(fname, "rb");
  uint32 fsz = fsize(fp);

  byte* buffer = (byte*)malloc(fsz+1);
  fread(buffer, fsz, 1, fp);

  struct File* newFile = (struct File*)malloc(sizeof(struct File));
  memset(newFile,0,sizeof(struct File));

  newFile->data = buffer;
  newFile->size = fsz;
  
  return newFile;
}

void writeFile(byte* fname, struct File* fileData) {
  FILE* fp = fopen(fname, "wb");

  if(fp == (FILE*)0x0) {
	  return;
  }

  fwrite(fileData->data, sizeof(byte), fileData->size, fp);

  fclose(fp);
  
  return;
}