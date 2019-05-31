#ifndef CARVEFILE_H
#define CARVEFILE_H

#include <stdlib.h>
#include <string.h>
#include "common.h"

#define KILOBYTE 1 << 10

/* A Carved File Data Structure. */
struct carved_file {
  uint32 size;
  uint32 offset;
  uint32 endOffset;
  byte* data;
};

/**
 * Creates a Carved File.
 * @param data - data of carved file.
 * @param size - size of carved file.
 * @param offset - offset of carved file in binary image.
 * @param endOffset - the end of file offset in binary image.
 * @return a carved file with data set to parameters.
 */
static struct carved_file* createCarvedFile(byte*data, uint32 size, uint32 offset, uint32 endOffset);

/** 
 * Carves File From Binary Image Buffer.
 * @param buffer - the buffer containing the binary image.
 * @param size - the size of the buffer.
 * @param endMagic - the end Magic to search for that marks end of the file to carve.
 * @param endMagicSz - the size of the end magic.
 * @param offset - offset to begin searching for end magic.
 * @return a carved file; null if end magic wasn't found.
 */
static struct carved_file* carveFileFromBuffer(byte* buffer, uint32 size, byte* endMagic,
					       uint32 endMagicSz, uint32 offset);

/**
 * Finds the next file to carve in a binary image.
 * @param magic - Beginning of File Magic to Search For.
 * @param magicSz - The Magic Size
 * @param endMagic - The End of File Magic to Search For.
 * @param endMagicSz - The End Magic Size
 * @param buffer - The Buffer of the Binary Image.
 * @param size - the Size of the Buffer containing The Binary Image.
 * @param offset - The offset into the buffer to begin searching.
 * @return a carved file data structure; null if file was not found.
 */
struct carved_file* findFile(byte* magic, uint32 magicSz, byte* endMagic, uint32 endMagicSz,
			     byte* buffer, uint32 size, uint32 offset);
  
#endif
