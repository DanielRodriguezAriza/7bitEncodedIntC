#ifndef DRA_7BIT_ENCODED_INT_H
#define DRA_7BIT_ENCODED_INT_H

#include <stdio.h>

/* TODO : Implement custom BE and LE functions and 64 bit version because why not. */

typedef struct {
	int value;
	unsigned char bytes;
} i32_7bit_encoded_t;

int read_7_bit_encoded_i32_le(FILE *file, i32_7bit_encoded_t *value)
{
	if(file == NULL)
		return 1;
	
	int ans = 0;
	unsigned char check = (((unsigned char)(1)) << 7);
	unsigned char mask = ~check;
	
	for(int i = 0; i < 5; ++i)
	{
		unsigned char byte = 0;
		fread(&byte, 1, 1, file);
		
		int current = ((int)(byte & mask)) << (7 * i);
		ans |= current;
		
		if((check & byte) == 0)
		{
			if(value != NULL)
			{
				value->value = ans;
				value->bytes = i + 1;
			}
			
			return 0;
		}
	}

	return 2;
}

int write_7_bit_encoded_i32_le(FILE *file, i32_7bit_encoded_t *value)
{
	if(file == NULL)
		return 1;
	
	unsigned int v = *((unsigned int *)(&(value->value)));
	unsigned char mask = 127;
	
	for(int i = 0; i < 5; ++i)
	{
		unsigned char lb = v & mask;
		
		v = v >> 7;
		
		if(v != 0)
		{
			lb |= 1 << 7;
			fwrite(&lb, 1, 1, file);
		}
		else
		{
			fwrite(&lb, 1, 1, file);
			if(value)
			{
				value->bytes = i + 1;
			}
			break;
		}
	}
	
	return 0;
}

#endif /* DRA_7BIT_ENCODED_INT_H */
