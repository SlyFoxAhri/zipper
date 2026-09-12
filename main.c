#include <stdio.h>
#include "main.h"

u8 id[2];

int main(int nums, char* name[])
{
  char* myFile = name[1];
  FILE* filepointer = fopen(myFile, "rb");
  
  if(!filepointer)
  {
    puts("No file provided");
    return 1;
  }

  fread(id, 1, 2, filepointer);
  if(id[0] == 31 && id[1] == 139)
  {
    printf("this indeed is a gzip file...\n");
  }
  
  for(int i = 0; i <= 1 ; i++)
  {
    printf("%d\n", id[i]);
  }

  fclose(filepointer);
  return 0;
}

u8 binaryToDecimal(u8* binary)
{
  u8 decimal;
  u8 base = 1;
  //this changed irs endianness
  // what is endianness of gzip fread??
  
  u8 binary_num = binary[0] + (binary[1] << 1) + (binary[2] << 2) + (binary[3] << 3) + (binary[4] << 4) + (binary[5] << 5) + (binary[6] << 6) + (binary[7] << 7);

  printf("%d\n", binary_num);
  
  while(binary_num)
  {
    u8 last_digit = binary_num % 10;
  }

  

  return decimal;
}
