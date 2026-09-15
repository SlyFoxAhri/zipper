#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <threads.h>
#include "main.h"

int main(int nums, char* name[])
{
  u8 isOK = 0;
  u8 id[2];
  u8 cm = 0;
  u8 flg = 0;
  bool flgs[8]; //iterate to see which flags are set 
  int bshift = 0;
  bool isText = false;
  bool isFhcrc = false;
  bool isExtra = false;
  bool isName = false;
  bool isComment = false;
  char ognlName[256];
  u16 extLen = 0;
  u32 mTime = 0;
  u8 xfl = 0;
  u8 os = 255; //unknown
  
  char* myFile = name[1];
  FILE* filepointer = fopen(myFile, "rb");
  
  if(!filepointer)
  {
    puts("No file provided");
    goto error;
  }

  fread(id, 1, 2, filepointer);
  if(id[0] == 31 && id[1] == 139)
  {
    puts("This indeed is a gzip file...");
  }
  else
  {
    goto error;
  }

  fread(&cm, 1, 1, filepointer);
  if(cm == 8)
  {
    puts("Happens to be comressed using the deflate method...");
  }
  else if(cm > 0 && cm < 8)
  {
    puts("Reserved bits are set, you are not supposed to do that...");
    goto error;
  }
  else
  {
    goto error;
  }

  fread(&flg, 1, 1, filepointer);
  for(int bshift = 0; bshift < sizeof flgs; bshift++)
  {
    flgs[bshift] = isBitSet(flg, bshift);
  }
  
  if(flgs[0] == 1)
  {
    isText = true;
  }
  if(flgs[1] == 1)
  {
    isFhcrc = true;
  }
  if(flgs[2] == 1)
  {
    isExtra = true;
  }
  if(flgs[3] == 1)
  {
    isName = true;
  }
  if(flgs[4] == 1)
  {
    isComment = true;
  }
  if(flgs[5] == 1)
  {
    goto error;
  }
  if(flgs[6] == 1)
  {
    goto error;
  }
  if(flgs[7] == 1)
  {
    goto error;
  }

  fread(&mTime, sizeof mTime, 1, filepointer);

  fread(&xfl, sizeof xfl, 1, filepointer); //2 or 4 determines compression algorithm

  fread(&os, sizeof os, 1, filepointer);
  
  if(isExtra)
  {
    fread(&extLen, sizeof extLen, 1, filepointer);
    //skip extra fied
    fseek(filepointer, extLen, SEEK_CUR); 
  }

  if(isName)
  {
    int ch = 1;
    int i = 0;
    while(ch != 0)
    {
      ch = fgetc(filepointer);
      ognlName[i] = (char)ch;
      i++;
    }
  }

  if(isComment)
  {
    int ch = fgetc(filepointer);
    while(ch != 0)
    {
      ch = fgetc(filepointer);
    }
  }

  if(isFhcrc)
  {
    fseek(filepointer, 2, SEEK_CUR);
  }

  //decompression now
  u8 eof = 1;
  bool isEnd = false;
  u8 firstByte;
  u8 firstBit;
  u8 cmpType = 0x03;
  u16 nc_len = 0;
  char* read_buff;

  FILE* newFilepointer = fopen(ognlName, "wb");
  if(!newFilepointer)
  {
    goto error;
  }
  
  while(!isEnd)
  {
  //READ IN BITS NOT BYTES!!!
    fread(&firstByte, sizeof firstByte, 1, filepointer);
    //make sum function 
    firstBit = firstByte << 7;
    isEnd = firstBit >> 7;
    firstByte = firstByte >> 1;
    cmpType = firstByte >> 6;
    switch(cmpType)
    {
      case NO_COMPRESSION:
        fread(&nc_len, sizeof nc_len, 1, filepointer);
        fseek(filepointer, 2, SEEK_CUR);
        
        read_buff = malloc(nc_len);
        fread(read_buff, nc_len, 1, filepointer);
        
        fputs(read_buff, newFilepointer);
        break;
      case FIX_HUFFMAN:
        //do decode
        break;
      case DYNAMIC_HUFFMAN:
        //do decode
        break;
      case NOT_VALID:
        goto error;
        break;
    }
  }
  
  fclose(newFilepointer);

  goto leave;
  
  error:
  isOK = 1;
  puts("ERROR");
  
  leave:  
  fclose(filepointer);
  return isOK;
}

bool isBitSet(u8 value, u8 bitshift)
{
  bool resoult = (value & (1 << bitshift)) != 0;

  return resoult;
}

