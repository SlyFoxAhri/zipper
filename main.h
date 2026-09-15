#include <complex.h>
#include <stdint.h>
#include <stdbool.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef enum
{
  NO_COMPRESSION = 0X00,
  FIX_HUFFMAN = 0X01,
  DYNAMIC_HUFFMAN = 0X02,
  NOT_VALID = 0X03
} Compression_Type;

bool isBitSet(u8 val, u8 bshift);
