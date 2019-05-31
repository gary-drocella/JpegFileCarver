#include "carvefile.h"
#include <stdio.h>

static struct carved_file* createCarvedFile(byte* data, uint32 size, uint32 offset, uint32 endOffset) {
  struct carved_file* newCarvedFile = (struct carved_file*)malloc(sizeof(struct carved_file));
  memset(newCarvedFile, 0, sizeof(struct carved_file));
  newCarvedFile->data = data;
  newCarvedFile->size = size;
  newCarvedFile->offset = offset;
  newCarvedFile->endOffset = endOffset;
  return newCarvedFile;
}


static struct carved_file* carveFileFromBuffer(byte* buffer, uint32 size, byte* endMagic,
					       uint32 endMagicSz, uint32 offset) {
  uint32 endOffset = offset;
  uint32 currFileBufferSz = 4*KILOBYTE;
  uint32 i = 0;
  byte* fileBuffer = (byte*)malloc(sizeof(byte)*currFileBufferSz);
  struct carved_file* carvedFile = (struct carved_file*)0;
  uint32 endMagicOffset=0;
  uint32 notMagic = 0;
  
  /* Search For End Magic Until End of File Is Reached. */
  while(endOffset < size) {
    fileBuffer[i] = *(buffer + offset + i);
    
	/* Determine If First Byte of End Magic Has Been Found. */
    if(endMagic[0] == *(buffer+offset+i)) {
      /* Loop Through Loop Magic Sz */
      while(endMagicOffset < endMagicSz) {
        /*  */
        if(endMagic[endMagicOffset] != *(buffer+offset+i+endMagicOffset)) {
          notMagic = 1;
          break;
        }
	    endMagicOffset++;
      }

	  /* If End Magic is Found */
      if(!notMagic) {
        /* Allocate more memory if necessary for Buffer. */
        if(i+endMagicSz > currFileBufferSz) {
          currFileBufferSz += i+endMagicSz;
          fileBuffer = (byte*)realloc(fileBuffer,currFileBufferSz);
	    }
		
	    uint32 endMagicOffset = i+endMagicSz;
        /* Copy The Magic End Offset to the Buffer. */
        while(i < endMagicOffset) {
          fileBuffer[i] = *(buffer + offset + i);
          i++;
	    }
        /* Create a Carved File Data Structure. */
	    endOffset += 2;
        uint32 carvedFileSz = endOffset-offset;
        carvedFile = createCarvedFile(fileBuffer, carvedFileSz, offset, endOffset);
        break;
      }
    }
    
    i++;
	/* If end of File Buffer is reached, then Twice its Size. */
    if(i >= currFileBufferSz) {
      currFileBufferSz *= 2;
      fileBuffer = (byte*)realloc(fileBuffer, currFileBufferSz);
    }

    endMagicOffset = 0;
    notMagic = 0;
    endOffset++;
  }

  return carvedFile;
}

struct carved_file* findFile(byte* magic, uint32 magicSz, byte* endMagic, uint32 endMagicSz, byte* buffer, uint32 size, uint32 fileOffset) {
  uint32 offset = fileOffset;
  uint32 magicOffset = 0;
  uint32 notMagic = 0;
  struct carved_file* file = 0;
    
  while(offset < size) {
    if(magic[magicOffset] == *(buffer+offset)) {
      while(magicOffset < magicSz) {
        if(magic[magicOffset] != *(buffer+offset+magicOffset)) {
          notMagic = 1;
          break;
	    }
	    magicOffset++;
      }

      if(!notMagic) {
	    file = carveFileFromBuffer(buffer, size, endMagic, endMagicSz, offset);
        break;
      } 
    }
    
    notMagic = 0;
    magicOffset = 0;
    offset++;
  }

  return file;
}
