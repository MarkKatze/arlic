//Library for romHacking Marlon J Mesa 2026
//ARLIC: Another Romhacking Lib In C
//Inspired in Yarhl

#ifndef ARLIC
#define ARLIC

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include <stdint.h>

#define ENDIANNESS_BIG 0
#define ENDIANNESS_LITTLE 1

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef struct
{
	u8 *data;
	const char *path;
    size_t position;
	size_t size;
	u8 endianness;

}Binary;

/*Creates a new Binary File with this default params:
*position 0x0
*size = 0x1;
*endianness = ENDIANNESS_LITTLE;*/
Binary Binary_CreateNewBinary();

/*Load an exising file, set in position 0 and in little endian*/
Binary Binary_LoadFromFile(const char *filePath);

u64 Endianness_Change(u64 data, u8 size);

u8 Binary_Read_UInt8(Binary *bin);
u16 Binary_Read_UInt16(Binary *bin);
u32 Binary_Read_UInt32(Binary *bin);

s8 Binary_Read_Int8(Binary *bin);
s16 Binary_Read_Int16(Binary *bin);
s32 Binary_Read_Int32(Binary *bin);

void Binary_Write_UInt8(Binary* bin, u8 value);
void Binary_Write_UInt16(Binary *bin, u16 value);
void Binary_Write_UInt32(Binary *bin, u32 value);

void Binary_Write_Int8(Binary* bin, s8 value);
void Binary_Write_Int16(Binary *bin, s16 value);
void Binary_Write_Int32(Binary *bin, s32 value);

/*Frees data*/
void Binary_Destroy(Binary *bin);

/*Safes the binary data in a file*/
u8 Binary_SafeBinaryInFile(Binary *bin, char* filePath);


/*This only works in windows*/
void Get_BaseDirectory(char* dir);

#endif
