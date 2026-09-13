#include <stdio.h>
#include "main.h"

int main(int nums, char* name[])
{
  u8 isOK;
  u8 id[2];
  u8 cm;
  u8 flg;
  bool flgs[8]; //iterate to see which flags are set 
  int val = 1; //00000001
  int bshift = 0;
  bool isText;
  bool isFhcrc;
  bool isExtra;
  bool isName;
  bool isComment;
  u16 extLen;
  
  char* myFile = name[1];
  FILE* filepointer = fopen(myFile, "rb");
  
  if(!filepointer)
  {
    puts("No file provided");
    isOK = 1;
    goto leave;
  }

  fread(id, 1, 2, filepointer);
  if(id[0] == 31 && id[1] == 139)
  {
    puts("This indeed is a gzip file...");
  }
  else
  {
    isOK = 1;
    goto leave;
  }

  fread(&cm, 1, 1, filepointer);
  if(cm == 8)
  {
    puts("Happens to be comressed using the deflate method...");
  }
  else
  {
    isOK = 1;
    goto leave;
  }

  fread(&flg, 1, 1, filepointer);
  for(int bshift = 0; bshift < sizeof flgs; bshift++)
  {
    flgs[bshift] = isBitSet(val, bshift);
    val *= 2;
  }
  
  if(flgs[0])
  {
    isText = true;
  }
  if(flgs[1])
  {
    isFhcrc = true;
  }
  if(flgs[2])
  {
    isExtra = true;
  }
  if(flgs[3])
  {
    isName = true;
  }
  if(flgs[4])
  {
    isComment = true;
  }
  if(flgs[5])
  {
    isOK = 1;
    goto leave;
  }
  if(flgs[6])
  {
    isOK = 1;
    goto leave;
  }
  if(flgs[7])
  {
    isOK = 1;
    goto leave;
  }

  if(isExtra)
  {
    //first 2 bytes is size;
    fread(&extLen, sizeof extLen, 1, filepointer);
    //skip extra fied
    // not sure if offdet is right
    fseek(filepointer, extLen, SEEK_CUR); 
  }

  if(isName)
  {
    //skip till first null term
  }

  if(isComment)
  {
    //skip till first null term
  }
  

  isOK = 0;
  leave:  
  fclose(filepointer);
  
  return isOK;
}

bool isBitSet(u8 value, u8 bitshift)
{
  bool resoult = (value & (1 << bitshift)) != 0;

  return resoult;
}

