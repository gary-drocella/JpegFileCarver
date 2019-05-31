#include <stdio.h>
#include <windows.h>
#include "md5.h"
#include "carvefile.h"
#include "fileio.h"
#include "magic.h"
#include "common.h"

#define MD5_DIGEST_SZ 16
#define MD5_STRING_SZ 32


/**
 * Create a Directory.
 * @param dirName - name of directory to create.
 * @return nothing.
 */
static void createDirectory(const byte* dirName) {
	CreateDirectory(dirName,(void*)0);
	return;
}

/**
 * Converts a digest to a string.
 * @param str - string to copy digest into.
 * @param digest - the md5 digest.
 * @return nothing.
 */
static void digestToString(byte* str, const byte* digest) {
	for(int i = 0; i < MD5_DIGEST_SZ; i++) {
		sprintf(str, "%s%02x", str, digest[i]);
	}
	
	return;
}

/**
 * Computes the MD5 Hash
 * @param digest - buffer to copy digest into
 * @param data - data to compute MD5 hash
 * @param size - size of data.
 * @return nothing.
 */
static void md5(byte* digest, const byte* data, uint32 size) {
	MD5_CTX md5Context;
	
	MD5_Init(&md5Context);
	MD5_Update(&md5Context, data, size);
	MD5_Final(digest, &md5Context);
	
}

/**
 * Extracts and Repairs The Jpegs
 * @param kdbFile - the kdb file to get magic bytes from.
 * @param inputFile - the binary image. 
 * @return nothing.
 */
static void extractAndRepairJpegs(byte* inputFile) {
  uint32 currOffset = 0;
  struct File* file = readFile(inputFile);
  struct carved_file* jpegFile = 0;
  byte repairedFileName[KILOBYTE];
  byte directoryName[KILOBYTE];
  byte digest[MD5_DIGEST_SZ+1];
  byte digestStr[MD5_STRING_SZ+1];
  
  sprintf(directoryName,"%s_Repaired", inputFile);
  createDirectory(directoryName);
  
  while(1) {
    jpegFile = findFile(JPEG_MAGIC, JPEG_MAGIC_SZ, JPEG_MAGIC_END, JPEG_MAGIC_END_SZ,
					  file->data, file->size, currOffset);
    if(jpegFile == 0) {
      break;
    }
	
	sprintf(repairedFileName, "./%s/%d.jpeg", directoryName,jpegFile->offset);
	
	printf("Detected JPEG At Offset: %08x\n", jpegFile->offset);
    printf("Detected File Size: %d\n", jpegFile->size);
	
    memset(digest, 0, MD5_DIGEST_SZ+1);
    memset(digestStr, 0, MD5_STRING_SZ+1);
	
    md5(digest, jpegFile->data, jpegFile->size);
    digestToString(digestStr, digest);

    printf("JPEG MD5: %s\n", digestStr);
    printf("Repaired File Location: %s\n", repairedFileName);
	
	struct File* newFile = malloc(sizeof(struct File));
	newFile->size = jpegFile->size;
	newFile->data = jpegFile->data;
	
    writeFile(repairedFileName, newFile);
	
	memset(repairedFileName, 0, KILOBYTE);
    currOffset = jpegFile->endOffset;

	free(newFile);
    free(jpegFile->data);
    free(jpegFile);

    jpegFile = 0;
  }
  
  
  free(file);
  
}

int main(int argc, char* argv[]) {
  byte* inputFile = argv[1];

  extractAndRepairJpegs(inputFile);
  
  return 0;
}
