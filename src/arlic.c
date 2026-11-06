#include "arlic.h"

struct ArlicBinary
{
	u8 *data;
    size_t position;
	size_t size;
	u8 endianness;

};


u16 ArlicBinary_Create(ArlicBinary **bin, size_t initialSize, u16 endianness)
{
	if(bin == NULL)
	{
		
		ARLIC_LOG("ERROR IN: ArlicBinary_Create(), ArlicBinary **bin can't be NULL.... WTF, why you do that!!??\n");
		return ARLIC_ERROR;	
	}
	
	*bin = malloc(sizeof(ArlicBinary));
	
	if(*bin == NULL)
	{
		ARLIC_LOG("ERROR IN: ArlicBinary_Create(), ArlicBinary *bin can't be created\n");
		return ARLIC_ERROR;
	}
	
	if(initialSize == 0)
	{
		initialSize = 1;
	}
		
	(*bin)->data = calloc(initialSize, 1);	
	
	if((*bin)->data == NULL)
	{
		ARLIC_LOG("ERROR IN: ArlicBinary_Create(), can't allocate memory for the binary data\n");
		free(*bin);
		*bin = NULL;
		return ARLIC_ERROR;
	}
	
	(*bin)->data[initialSize - 1] = '\0';
	(*bin)->size = initialSize;
	(*bin)->position = 0x0;
	
	
	if(endianness == ARLIC_ENDIANNESS_BIG)
	{
		(*bin)->endianness = ARLIC_ENDIANNESS_BIG;		
	}
	else
	{
		(*bin)->endianness = ARLIC_ENDIANNESS_LITTLE;	
	}

	return ARLIC_OK;
}


u16 ArlicBinary_LoadFromFile(ArlicBinary **bin, u16 endianness, const char *filePath)
{
	if(bin == NULL)
	{
		
		ARLIC_LOG("ERROR IN: ArlicBinary_LoadFromFile(), ArlicBinary **bin can't be NULL.... WTF, why you do that!!??\n");
		return ARLIC_ERROR;	
	}
	
	if (filePath == NULL)
	{
		ARLIC_LOG("ERROR IN: ArlicBinary_LoadFromFile(), const char *filePath can't be NULL\n");
		return ARLIC_ERROR;
	}

	*bin = malloc(sizeof(ArlicBinary));
	
	if(*bin == NULL)
	{
		ARLIC_LOG("ERROR IN: ArlicBinary_LoadFromFile(), ArlicBinary *bin can't be created\n");
		return ARLIC_ERROR;
	}
	
	FILE *file = fopen(filePath, "rb");
	
	if(file == NULL)
	{
		ARLIC_LOG("ERROR IN: ArlicBinary_LoadFromFile(), can't load the File\n");
  free(*bin);
  *bin = NULL;
		return ARLIC_ERROR;
	}
	
	fseek(file, 0, SEEK_END);
	size_t size = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	(*bin)->data = malloc(size);
	
	if((*bin)->data == NULL)
	{
		ARLIC_LOG("ERROR IN: ArlicBinary_LoadFromFile(), can't allocate memory for the binary data\n");
		free(*bin);
		*bin = NULL;
		fclose(file);
		return ARLIC_ERROR;
	}
	
	fread((*bin)->data, 1, size, file);
	fclose(file);
	
	(*bin)->size = size;
	(*bin)->position = 0x0;
	
	
	if(endianness == ARLIC_ENDIANNESS_BIG)
	{
		(*bin)->endianness = ARLIC_ENDIANNESS_BIG;		
	}
	else
	{
		(*bin)->endianness = ARLIC_ENDIANNESS_LITTLE;	
	}

	return ARLIC_OK;
}

u16 ArlicBinary_Resize(ArlicBinary *bin, size_t newSize)
{
	if (newSize == 0) 
	{
		ARLIC_LOG("ERROR IN: ArlicBinary_Resize(), you can't resize a buffer to 0\n");
		return ARLIC_ERROR;
	}

	if(bin == NULL)
	{
		ARLIC_LOG("ERROR IN: ArlicBinary_Resize(), ArlicBinary *bin can't be NULL\n");
		return ARLIC_ERROR;
	}
	
	if(bin->data == NULL)
	{
		ARLIC_LOG("ERROR IN: ArlicBinary_Resize(), The Binary has not been created\n");
		return ARLIC_ERROR;
	}
	
	u8* tempDir;
	
	tempDir = realloc(bin->data, newSize);
	
	size_t originalSize = bin->size;
	
	if(tempDir == NULL)
	{
		ARLIC_LOG("ERROR IN: ArlicBinary_Resize(), ArlicBinary *bin can't be resized\n");
		return ARLIC_ERROR;	
	}
	else
	{	
		bin->data = tempDir;
		bin->size = newSize;
		
		if(bin->size <= bin->position)
		{
			bin->position = bin->size - 1;			
		}	
	}
	
	if(bin->size > originalSize)
	{
		memset(bin->data + originalSize, 0, bin->size - originalSize);	
	}
	
	return ARLIC_OK;
	
}

u64 ArlicEndianness_Change(u64 data, u8 size)
{	
	u64 result;
	
	switch(size)
	{
		case 0x2:
					
			result = ((data & 0x00FFU) << 8) | ((data >> 8) & 0x00FFU);
			
			break;
			
		case 0x4:
			
			
			result = ((data & 0x000000FFU) << 24) | ((data & 0x0000FF00U) << 8) | 
			         ((data & 0x00FF0000U) >> 8)  | ((data & 0xFF000000U) >> 24);
			
			break;
		//TODO: ADD 8 bytes and 16 bytes changes 
			
		default:
			ARLIC_LOG("ERROR IN: ArlicEndianness_Change(), invalid u8 size value");
			result = data;
	}
	
	return result;		
	
}


u16 ArlicBinary_SetPosition(ArlicBinary *bin, size_t position)
{
	if(bin == NULL)
	{	
		ARLIC_LOG("ERROR IN: ArlicBinary_SetPosition(), ArlicBinary *bin can't be NULL\n");
		return ARLIC_ERROR;
	}
	
	if(position > bin->size)
	{
		ARLIC_LOG("ERROR IN: ArlicBinary_SetPosition(), position can't be greater than buffer size\n");
		return ARLIC_ERROR;	
	}
	
	bin->position = position;
	
	return ARLIC_OK;
}

size_t ArlicBinary_GetPosition(ArlicBinary *bin)
{
	return bin->position;
}

u16 ArlicBinary_GetPosition_s(ArlicBinary *bin, size_t *position)
{
	if(bin == NULL)
	{	
	    ARLIC_LOG("ERROR IN: ArlicBinary_SetPosition(), ArlicBinary *bin can't be NULL\n");
		return ARLIC_ERROR;
	}
	
	*position = bin->position;
	
	return ARLIC_OK;
}


size_t ArlicBinary_GetSize(ArlicBinary *bin)
{
	return bin->size;	
}

/*******************
** READ FUNCTIONS **
********************/


u16 ArlicBinary_Read_UInt8(ArlicBinary *bin, u8 *readValue)
{	
	if(bin == NULL)
	{	
		ARLIC_LOG("ERROR IN: ArlicBinary_Read_UInt8(), ArlicBinary *bin can't be NULL\n");
		return ARLIC_ERROR;
	}

	if(readValue == NULL)
	{	
		ARLIC_LOG("ERROR IN: ArlicBinary_Read_UInt8(), u8 *readValue can't be NULL\n");
		return ARLIC_ERROR;
	}
	
	if(bin->position + 1 > bin->size)
	{
		ARLIC_LOG("ERROR IN: ArlicBinary_Read_UInt8(), you can't read out the buffer\n");
		return ARLIC_ERROR;
	}
	
	memcpy(readValue, bin->data + (bin->position), 1);
	
	bin->position = bin->position + 1;
	
	return ARLIC_OK;
}

u16 ArlicBinary_Read_UInt16(ArlicBinary *bin, u16 *readValue)
{	
	if(bin == NULL)
	{	
		ARLIC_LOG("ERROR IN: ArlicBinary_Read_UInt16(), ArlicBinary *bin can't be NULL\n");	
		return ARLIC_ERROR;
	}
	
	if(readValue == NULL)
	{	
		ARLIC_LOG("ERROR IN: ArlicBinary_Read_UInt16(), u16 *readValue can't be NULL\n");	
		return ARLIC_ERROR;
	}

	if(bin->position + 2 > bin->size)
	{
		ARLIC_LOG("ERROR IN: ArlicBinary_Read_UInt16(), you can't read out the buffer\n");
		return ARLIC_ERROR;
	}
	
	memcpy(readValue, bin->data + (bin->position), 2);
	
	if(bin->endianness == ARLIC_ENDIANNESS_BIG)
	{
		*readValue = (u16)ArlicEndianness_Change(*readValue, 2);
	}
	
	bin->position = bin->position + 2;
	
	return ARLIC_OK;
}
 
u16 ArlicBinary_Read_UInt32(ArlicBinary *bin, u32 *readValue)
{	
	if(bin == NULL)
	{	
		ARLIC_LOG("ERROR IN: ArlicBinary_Read_UInt32(), ArlicBinary *bin can't be NULL\n");	
		return ARLIC_ERROR;
	}
	
	if(readValue == NULL)
	{	
		ARLIC_LOG("ERROR IN: ArlicBinary_Read_UInt32(), u32 *readValue can't be NULL\n");
		return ARLIC_ERROR;
	}

	if(bin->position + 4 > bin->size)
	{
		ARLIC_LOG("ERROR IN: ArlicBinary_Read_UInt32(), you can't read out the buffer\n");
		return ARLIC_ERROR;
	}
	
	memcpy(readValue, bin->data + (bin->position), 4);
	
	if(bin->endianness == ARLIC_ENDIANNESS_BIG)
	{
		*readValue = (u32)ArlicEndianness_Change((u64)*readValue, 4);
	}
	
	bin->position = bin->position + 4;
	
	return ARLIC_OK;
}



u16 ArlicBinary_Read_Int8(ArlicBinary *bin, s8 *readValue)
{	
	if(bin == NULL)
	{	
		ARLIC_LOG("ERROR IN: ArlicBinary_Read_Int8(), ArlicBinary *bin can't be NULL\n");
		return ARLIC_ERROR;
	}
	
	if(readValue == NULL)
	{	
		ARLIC_LOG("ERROR IN: ArlicBinary_Read_Int8(), s8 *readValue can't be NULL\n");
		return ARLIC_ERROR;
	}
	
	if(bin->position + 1 > bin->size)
	{
		ARLIC_LOG("ERROR IN: ArlicBinary_Read_Int8(), you can't read out the buffer\n");
		return ARLIC_ERROR;
	}
	
	memcpy(readValue, bin->data + (bin->position), 1);
	
	bin->position = bin->position + 1;
	
	return ARLIC_OK;
}

u16 ArlicBinary_Read_Int16(ArlicBinary *bin, s16 *readValue)
{	
	if(bin == NULL)
	{	
		ARLIC_LOG("ERROR IN: ArlicBinary_Read_Int16(), ArlicBinary *bin can't be NULL\n");	
		return ARLIC_ERROR;
	}
	
	if(readValue == NULL)
	{	
		ARLIC_LOG("ERROR IN: ArlicBinary_Read_Int16(), s16 *readValue can't be NULL\n");
		return ARLIC_ERROR;
	}

	if(bin->position + 2 > bin->size)
	{
		ARLIC_LOG("ERROR IN: ArlicBinary_Read_Int16(), you can't read out the buffer\n");
		return ARLIC_ERROR;
	}
	
	memcpy(readValue, bin->data + (bin->position), 2);
	
	if(bin->endianness == ARLIC_ENDIANNESS_BIG)
	{
		*readValue = (s16)ArlicEndianness_Change((u64)*readValue, 2);
	}
	
	bin->position = bin->position + 2;
	
	return ARLIC_OK;
	
}
 
u16 ArlicBinary_Read_Int32(ArlicBinary *bin, s32 *readValue)
{	
	if(bin == NULL)
	{	
		ARLIC_LOG("ERROR IN: ArlicBinary_Read_Int32(), ArlicBinary *bin can't be NULL\n");
		return ARLIC_ERROR;
	}
	
	if(readValue == NULL)
	{	
		ARLIC_LOG("ERROR IN: ArlicBinary_Read_Int32(), s32 *readValue can't be NULL\n");	
		return ARLIC_ERROR;
	}

	if(bin->position + 4 > bin->size)
	{
		ARLIC_LOG("ERROR IN: ArlicBinary_Read_Int32(), you can't read out the buffer\n");
		return ARLIC_ERROR;
	}
	
	memcpy(readValue, bin->data + (bin->position), 4);
	
	if(bin->endianness == ARLIC_ENDIANNESS_BIG)
	{
		*readValue = (s32)ArlicEndianness_Change((u64)*readValue, 4);
	}
	
	bin->position = bin->position + 4;
	
	return ARLIC_OK;
	
}


/********************
** WRITE FUNCTIONS **
*********************/


u16 ArlicBinary_Write_UInt8(ArlicBinary *bin, u8 value)
{
	if(bin == NULL)
	{	
		ARLIC_LOG("ERROR IN: ArlicBinary_Write_UInt8(), ArlicBinary *bin can't be NULL\n");
		return ARLIC_ERROR;
	}
	
	u8 *tempInput;
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
			
			ARLIC_LOG("ERROR IN: ArlicBinary_Write_UInt8(), can't reallocate binary data\n");	
			return ARLIC_ERROR;
		}
	}
	
	bin->data[bin->position] = value;
		
	bin->position += 1;
	
	return ARLIC_OK;
}

u16 ArlicBinary_Write_UInt16(ArlicBinary *bin, u16 value)
{
	if(bin == NULL)
	{	
		ARLIC_LOG("ERROR IN: ArlicBinary_Write_UInt16(), ArlicBinary *bin can't be NULL\n");	
		return ARLIC_ERROR;
	}
	
	u8 *tempInput;
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
			ARLIC_LOG("ERROR IN: ArlicBinary_Write_UInt16(), can't reallocate binary data\n");	
			return ARLIC_ERROR;
		}
	}
		
	if(bin->endianness == ARLIC_ENDIANNESS_BIG)
	{
		value = ArlicEndianness_Change(value, 2);	
	}	
	
	memcpy(bin->data + (bin->position), &value, 2);
		
	bin->position += 2;
	
	return ARLIC_OK;
}

u16 ArlicBinary_Write_UInt32(ArlicBinary *bin, u32 value)
{
	if(bin == NULL)
	{	
		ARLIC_LOG("ERROR IN: ArlicBinary_Write_UInt32(), ArlicBinary *bin can't be NULL\n");	
		return ARLIC_ERROR;
	}
	
	u8 *tempInput;
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
			ARLIC_LOG("ERROR IN: ArlicBinary_Write_UInt32(), can't reallocate binary data\n");	
			return ARLIC_ERROR;
		}
	}
		
	if(bin->endianness == ARLIC_ENDIANNESS_BIG)
	{
		value = ArlicEndianness_Change(value, 4);	
	}	
	
	memcpy(bin->data + (bin->position), &value, 4);
	
	bin->position += 4;
	
	return ARLIC_OK;
}



u16 ArlicBinary_Write_Int8(ArlicBinary *bin, s8 value)
{
	if(bin == NULL)
	{	
		ARLIC_LOG("ERROR IN: ArlicBinary_Write_Int8(), ArlicBinary *bin can't be NULL\n");
		return ARLIC_ERROR;
	}
	
	u8 *tempInput;
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
			ARLIC_LOG("ERROR IN: ArlicBinary_Write_Int8(), can't reallocate binary data\n");	
			return ARLIC_ERROR;
		}
	}
	
	bin->data[bin->position] = value;
		
	bin->position += 1;
	
	return ARLIC_OK;
	
}

u16 ArlicBinary_Write_Int16(ArlicBinary *bin, s16 value)
{
	if(bin == NULL)
	{	
		ARLIC_LOG("ERROR IN: ArlicBinary_Write_Int16(), ArlicBinary *bin can't be NULL\n");
		return ARLIC_ERROR;
	}
	
	u8 *tempInput;
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
			ARLIC_LOG("ERROR IN: ArlicBinary_Write_Int16(), can't reallocate binary data\n");
			return ARLIC_ERROR;
		}
	}
		
	if(bin->endianness == ARLIC_ENDIANNESS_BIG)
	{
		value = ArlicEndianness_Change(value, 2);	
	}	
	
	memcpy(bin->data + (bin->position), &value, 2);
		
	bin->position += 2;
	
	return ARLIC_OK;
}



u16 ArlicBinary_Write_Int32(ArlicBinary *bin, s32 value)
{
	if(bin == NULL)
	{	
		ARLIC_LOG("ERROR IN: ArlicBinary_Write_Int32(), ArlicBinary *bin can't be NULL\n");	
		return ARLIC_ERROR;
	}
	
	u8 *tempInput;
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
			ARLIC_LOG("ERROR IN: ArlicBinary_Write_Int32(), can't reallocate binary data\n");	
			return ARLIC_ERROR;
		}
	}
		
	if(bin->endianness == ARLIC_ENDIANNESS_BIG)
	{
		value = ArlicEndianness_Change(value, 4);	
	}	
	
	memcpy(bin->data + (bin->position), &value, 4);
	
	bin->position += 4;
	
	return ARLIC_OK;
	
}

void ArlicBinary_Destroy(ArlicBinary *bin)
{
	free(bin->data);
	free(bin);
}

u16 ArlicBinary_SaveBinaryInFile(ArlicBinary *bin, const char *filePath)
{		
	if(bin == NULL)
	{	
		ARLIC_LOG("ERROR IN: ArlicBinary_SaveBinaryInFile(), ArlicBinary *bin can't be NULL\n");	
		return ARLIC_ERROR;
	}
	
	if(bin->data == NULL)
	{	
		ARLIC_LOG("ERROR IN: ArlicBinary_SaveBinaryInFile(), this binary don't have data\n");	
		return ARLIC_ERROR;
	}
	
	FILE* binaryFile = fopen(filePath, "wb");
	
	if(binaryFile == NULL)
	{
		ARLIC_LOG("ERROR IN: ArlicBinary_SaveBinaryInFile(), can't create File\n");	
		return ARLIC_ERROR;
	}	
	
	size_t bytesCopied = fwrite(bin->data, 1, bin->size, binaryFile);
	
	fclose(binaryFile);
		
	if(bytesCopied != bin->size)
	{
		ARLIC_LOG("ERROR IN: ArlicBinary_SaveBinaryInFile(), can't save the file or incomplete writing\n");	
		return ARLIC_ERROR;
	}
		
	return ARLIC_OK;
}

void Get_BaseDirectory(char dir[]) //This only works in Windows
{
	char* lastBar;
	
	GetModuleFileName(NULL, dir, 1024);
	
	lastBar = strrchr(dir, '\\');
	*(lastBar + 1) = '\0';
}
