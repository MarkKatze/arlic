//Romhacking library by Marlon J Mesa (Mark Katze) 2026
//ARLIC: Another Romhacking Lib In C
//Inspired in Yarhl by Pleonex
//You can do whatever you want with this library, it has no license.

#ifndef ARLIC
#define ARLIC


#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include <stdint.h>

#define ARLIC_ENDIANNESS_LITTLE 0x0010
#define ARLIC_ENDIANNESS_BIG 0x0011

#define ARLIC_OK 0x0001
#define ARLIC_ERROR 0x0000

#ifdef ARLIC_DEBUG
	#define ARLIC_LOG(...) fprintf(stderr, __VA_ARGS__)
#else
	#define ARLIC_LOG(...) ((void)0)
	
#endif

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;



typedef struct ArlicBinary ArlicBinary;
//TODO: Create a struct to create and handle PO files

/*Creates a new Binary File, you can put initial
params with 0, and it puts this default params:
*size = 0x1;
*endianness = ARLIC_ENDIANNESS_LITTLE;*/
u16 ArlicBinary_Create(ArlicBinary **bin, size_t initialSize, u16 endianness);

/*Load an exising file, set in position 0, if endianness is 0:
endianness = ARLIC_ENDIANNESS_LITTLE*/
u16 ArlicBinary_LoadFromFile(ArlicBinary **bin, u16 endianess, const char *filePath);

u16 ArlicBinary_Resize(ArlicBinary *bin, size_t newSize);

u64 ArlicEndianness_Change(u64 data, u8 size);

u16 ArlicBinary_SetPosition(ArlicBinary *bin, size_t position);
size_t ArlicBinary_GetPosition(ArlicBinary *bin);
u16 ArlicBinary_GetPosition_s(ArlicBinary *bin, size_t *position); //safer version
size_t ArlicBinary_GetSize(ArlicBinary *bin);

u16 ArlicBinary_Read_UInt8(ArlicBinary *bin, u8 *readValue);
u16 ArlicBinary_Read_UInt16(ArlicBinary *bin, u16 *readValue);
u16 ArlicBinary_Read_UInt32(ArlicBinary *bin, u32 *readValue);

u16 ArlicBinary_Read_Int8(ArlicBinary *bin, s8 *readValue);
u16 ArlicBinary_Read_Int16(ArlicBinary *bin, s16 *readValue);
u16 ArlicBinary_Read_Int32(ArlicBinary *bin, s32 *readValue);

u16 ArlicBinary_Write_UInt8(ArlicBinary *bin, u8 value);
u16 ArlicBinary_Write_UInt16(ArlicBinary *bin, u16 value);
u16 ArlicBinary_Write_UInt32(ArlicBinary *bin, u32 value);

u16 ArlicBinary_Write_Int8(ArlicBinary *bin, s8 value);
u16 ArlicBinary_Write_Int16(ArlicBinary *bin, s16 value);
u16 ArlicBinary_Write_Int32(ArlicBinary *bin, s32 value);

/*Destroys the Binary pointer and frees his data*/
void ArlicBinary_Destroy(ArlicBinary *bin);

/*Safes the binary data in a file*/
u16 ArlicBinary_SaveBinaryInFile(ArlicBinary *bin, const char *filePath);

/*This only works in windows*/
void Get_BaseDirectory(char dir[]);

#endif
