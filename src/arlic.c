#include "arlic.h"

Binary Binary_CreateNewBinary()
{
	Binary bin;

	FILE *file = fopen("untiled", "wb");
	
	if(file == NULL)
	{
		printf("ERROR, can't created the file\n");
		exit(1);	
	}
	
	bin.data = malloc(1);
	
	if(bin.data == NULL)
	{
		printf("ERROR, can't allocate  memory for the file\n");	
	}
	
	bin.data[0] = '\0';
	fclose(file);
	
	bin.size = 1;
	bin.position = 0x0;
	bin.endianness = ENDIANNESS_LITTLE;
	
	return bin;
}

Binary Binary_LoadFromFile(const char *filePath)
{
	Binary bin;
	u64 size;
	
	FILE *file = fopen(filePath, "rb");
	
	if(file == NULL)
	{
		printf("ERROR, can't open %s", filePath);
		exit(1);	
	}
	
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);
	bin.data = malloc(size);
	fread(bin.data, 1, size, file);
	fclose(file);
	
	bin.size = size;
	bin.path = filePath;
	bin.position = 0x0;
	bin.endianness = ENDIANNESS_LITTLE;
	
	return bin;
}

u64 Endianness_Change(u64 data, u8 size)
{	
	u64 result;
	
	switch(size)
	{
		case 0x2:
					
			result = (data << 8) | (data >> 8);
			
			break;
			
		case 0x4:
			
			
			result = ((data & 0x000000FFU) << 24) | ((data & 0x0000FF00U) << 8) | 
			         ((data & 0x00FF0000U) >> 8)  | ((data & 0xFF000000U) >> 24);
			
			break;
			
		default:
			printf("ERROR, incorrect size -> in func: Endianness_Change");
			result = data;
	}
	
	return result;		
	
}

u8 Binary_Read_UInt8(Binary *bin)
{	

	if(bin == NULL)
	{	
		printf("ERROR, this binary don't have data\n");
		return 0;	
	}

	if(bin->position + 1 > bin->size)
	{
		printf("ERROR, you can't read out the buffer\n");
		return 0;
	}
	
	u8 readValue;
	
	memcpy(&readValue, bin->data + (bin->position), 1);
	
	bin->position = bin->position + 1;
	
	return readValue;
}

u16 Binary_Read_UInt16(Binary *bin)
{	
	if(bin == NULL)
	{	
		printf("ERROR, this binary don't have data\n");
		return 0;	
	}

	if(bin->position + 2 > bin->size)
	{
		printf("ERROR, you can't read out the buffer\n");
		return 0;
	}
	
	u16 readValue;
	
	memcpy(&readValue, bin->data + (bin->position), 2);
	
	if(bin->endianness == ENDIANNESS_BIG)
	{
		readValue = (u16)Endianness_Change(readValue, 2);
	}
	
	bin->position = bin->position + 2;
	
	return readValue;
}
 
u32 Binary_Read_UInt32(Binary *bin)
{	
	if(bin == NULL)
	{	
		printf("ERROR, this binary don't have data\n");
		return 0;	
	}

	if(bin->position + 4 > bin->size)
	{
		printf("ERROR, you can't read out the buffer\n");
		return 0;
	}

	u32 readValue;
	
	memcpy(&readValue, bin->data + (bin->position), 4);
	
	if(bin->endianness == ENDIANNESS_BIG)
	{
		readValue = (u32)Endianness_Change(readValue, 4);
	}
	
	bin->position = bin->position + 4;
	
	return readValue;
}

s8 Binary_Read_Int8(Binary *bin)
{	

	if(bin == NULL)
	{	
		printf("ERROR, this binary don't have data\n");
		return 0;	
	}

	if(bin->position + 1 > bin->size)
	{
		printf("ERROR, you can't read out the buffer\n");
		return 0;
	}
	
	s8 readValue;
	
	memcpy(&readValue, bin->data + (bin->position), 1);
	
	bin->position = bin->position + 1;
	
	return readValue;
}

s16 Binary_Read_Int16(Binary *bin)
{	

	if(bin == NULL)
	{	
		printf("ERROR, this binary don't have data\n");
		return 0;	
	}

	if(bin->position + 2 > bin->size)
	{
		printf("ERROR, you can't read out the buffer\n");
		return 0;
	}
	
	s16 readValue;
	
	memcpy(&readValue, bin->data + (bin->position), 2);
	
	if(bin->endianness == ENDIANNESS_BIG)
	{
		readValue = (u16)Endianness_Change(readValue, 2);
	}
	
	bin->position = bin->position + 2;
	
	return readValue;
}
 
s32 Binary_Read_Int32(Binary *bin)
{	

	if(bin == NULL)
	{	
		printf("ERROR, this binary don't have data\n");
		return 0;	
	}

	if(bin->position + 4 > bin->size)
	{
		printf("ERROR, you can't read out the buffer\n");
		return 0;
	}

	s32 readValue;
	
	memcpy(&readValue, bin->data + (bin->position), 4);
	
	if(bin->endianness == ENDIANNESS_BIG)
	{
		readValue = (u32)Endianness_Change(readValue, 4);
	}
	
	bin->position = bin->position + 4;
	
	return readValue;
}

void Binary_Write_UInt8(Binary *bin, u8 value)
{
	if(bin == NULL)
	{	
		printf("ERROR, this binary don't have data\n");
		return;	
	}
	
	char *tempInput;
	size_t diference;
	
	if(bin->size < bin->position + 1)
	{		
		diference = (bin->position + 1) - bin->size;						
		tempInput = realloc(bin->data, (bin->size) + diference);
		
		if(tempInput != NULL)
		{
			bin->size = bin->size + diference;			
			bin->data = tempInput;
		}
		else
		{
			printf("ERROR, can't write the file\n");
			return;
		}
	}
	
	bin->data[bin->position] = value;
		
	bin->position += 1;
}

void Binary_Write_UInt16(Binary *bin, u16 value)
{
	if(bin == NULL)
	{	
		printf("ERROR, this binary don't have data\n");
		return;	
	}
	
	char *tempInput;
	size_t diference;
	
	if(bin->size < bin->position + 2)
	{		
		diference = (bin->position + 2) - bin->size;						
		tempInput = realloc(bin->data, (bin->size) + diference);
		
		if(tempInput != NULL)
		{
			bin->size = bin->size + diference;			
			bin->data = tempInput;
		}
		else
		{
			printf("ERROR, can't write the file\n");
			return;
		}
	}
		
	if(bin->endianness == ENDIANNESS_BIG)
	{
		value = Endianness_Change(value, 2);	
	}	
	
	memcpy(bin->data + (bin->position), &value, 2);
		
	bin->position += 2;
}



void Binary_Write_UInt32(Binary *bin, u32 value)
{
	if(bin == NULL)
	{	
		printf("ERROR, this binary doesn't have data\n");
		return;	
	}
	
	char *tempInput;
	size_t diference;
	
	if(bin->size < bin->position + 4)
	{		
		diference = (bin->position + 4) - bin->size;						
		tempInput = realloc(bin->data, (bin->size) + diference);
		
		if(tempInput != NULL)
		{
			bin->size = bin->size + diference;			
			bin->data = tempInput;
		}
		else
		{
			printf("ERROR, can't write the file\n");
			return;
		}
	}
		
	if(bin->endianness == ENDIANNESS_BIG)
	{
		value = Endianness_Change(value, 4);	
	}	
	
	memcpy(bin->data + (bin->position), &value, 4);
	
	bin->position += 4;
}

void Binary_Write_Int8(Binary *bin, s8 value)
{
	
	if(bin == NULL)
	{	
		printf("ERROR, this binary doesn't have data\n");
		return;	
	}
	
	char *tempInput;
	size_t diference;
	
	if(bin->size < bin->position + 1)
	{		
		diference = (bin->position + 1) - bin->size;						
		tempInput = realloc(bin->data, (bin->size) + diference);
		
		if(tempInput != NULL)
		{
			bin->size = bin->size + diference;			
			bin->data = tempInput;
		}
		else
		{
			printf("ERROR, can't write the file\n");
			return;
		}
	}
	
	bin->data[bin->position] = value;
		
	bin->position += 1;
}

void Binary_Write_Int16(Binary *bin, s16 value)
{
	if(bin == NULL)
	{	
		printf("ERROR, this binary don't have data\n");
		return;	
	}
	
	char *tempInput;
	size_t diference;
	
	if(bin->size < bin->position + 2)
	{		
		diference = (bin->position + 2) - bin->size;						
		tempInput = realloc(bin->data, (bin->size) + diference);
		
		if(tempInput != NULL)
		{
			bin->size = bin->size + diference;			
			bin->data = tempInput;
		}
		else
		{
			printf("ERROR, can't write the file\n");
			return;
		}
	}
		
	if(bin->endianness == ENDIANNESS_BIG)
	{
		value = Endianness_Change(value, 2);	
	}	
	
	memcpy(bin->data + (bin->position), &value, 2);
		
	bin->position += 2;
}



void Binary_Write_Int32(Binary *bin, s32 value)
{
	
	if(bin == NULL)
	{	
		printf("ERROR, this binary don't have data\n");
		return;	
	}
	
	char *tempInput;
	size_t diference;
	
	if(bin->size < bin->position + 4)
	{		
		diference = (bin->position + 4) - bin->size;						
		tempInput = realloc(bin->data, (bin->size) + diference);
		
		if(tempInput != NULL)
		{
			bin->size = bin->size + diference;			
			bin->data = tempInput;
		}
		else
		{
			printf("ERROR, can't write the file\n");
			return;
		}
	}
		
	if(bin->endianness == ENDIANNESS_BIG)
	{
		value = Endianness_Change(value, 4);	
	}	
	
	memcpy(bin->data + (bin->position), &value, 4);
	
	bin->position += 4;
}

void Binary_Destroy(Binary *bin)
{
	free(bin->data);
}

u8 Binary_SafeBinaryInFile(Binary *bin, char *filePath)
{		
	if(bin == NULL || bin->data == NULL)
	{
		printf("ERROR, this binary don't have data\n");
		return;	
	}
	
	FILE* binaryFile = fopen(filePath, "wb");
	
	if(binaryFile == NULL)
	{
		printf("ERROR, can't create file\n");
		return;
	}	
	
	size_t bytesCopied = fwrite(bin->data, 1, bin->size, binaryFile);
	
	fclose(binaryFile);
		
	if(bytesCopied != bin->size)
	{
		printf("ERROR, can't save the file or incomplete writing\n");
		return 0;
	}
		
	return 1;
}

void Get_BaseDirectory(char* dir) //This only works in Windows
{
	char* lastBar;
	
	GetModuleFileName(NULL, dir, 256);
	lastBar = strrchr(dir, '\\');
	*(lastBar + 1) = '\0';
}
