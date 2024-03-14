#include "util.h"

u16 UpdateCRC16(u16 crc_in, u8 byte)
{
    u32 crc = crc_in;
    u32 in = byte | 0x100;
    do{
		crc <<= 1;
		in <<= 1;
		if(in & 0x100)
		  ++crc;
		if(crc & 0x10000)
		  crc ^= 0x1021;
    }
    while(!(in & 0x10000));
    return crc & 0xffffu;
}

u16 Cal_CRC16(const u8* p_data, u32 size)
{
    u16 crc = 0;
    const u8* dataEnd = p_data+size;
    while(p_data < dataEnd)
    	crc = UpdateCRC16(crc, *p_data++);
    crc = UpdateCRC16(crc, 0);
  	crc = UpdateCRC16(crc, 0);
  	return crc&0xffffu;
}

//  0x21000 -> 32 31 30 30 30 00 ("21000")
unsigned char hex_to_ascii(int hex_value,char *ascii_str) {
	return sprintf(ascii_str, "%X", hex_value);
}

int tlkapi_wcharStrlen(uint08 *pStr, uint16 maxLen)
{
	uint16 index;
	uint16 count = maxLen/2;
	uint16 *pTemp = (uint16*)pStr;
	for(index=0; index<count; index++){
		if(pTemp[index] == 0) break;
	}
	return (index*2);
}

/*-------------------------------------------------------------------------
    Function    :  tlkapi_ascii_char2val             xxx
    Return: -1=error
    Description:
        'a' -> 0xa  'A' -> 0xa
-------------------------------------------------------------------------*/
char tlkapi_ascii_char2val(char c)
{
    if(c>='0' && c<='9')
        return c-'0';
    if((c>='a' && c<='f') || (c>='A' && c<='F'))
        return (c&0x7)+9;

    return (char)(-1);
}

/*-------------------------------------------------------------------------
    Function    :  tlkapi_ascii_str2val             xxx
    Return: -1=error
    Description:
        str = "123" bas = 10 return 123
        str = "123" bas = 16 return 0x123
-------------------------------------------------------------------------*/
int tlkapi_ascii_str2val(const char str[], char base)
{
    int val = 0;
    signed char v;
    while(*str != 0){
        v = tlkapi_ascii_char2val(*str);
        if (v == -1 || v >= base)
        {
            if(val == 0) //to filter abormal beginning and ending
            {
                str ++;
                continue;
            }
            else
            {
                break;
            }
        }
        val = val*base + v;
        str++;
    }
    return val;
}


/******************************************************************************
 * Function: tlkapi_value_to_str
 * Descript: Converts a numeric value to a string in decimal format.
 * Params:
 *     @str[OUT]--Converts the value of the input buffer.
 *     @dec[IN]--Decimal value.
 *     @num[IN]--The number of conversions. (num<=10)
 *     @isDropZero[IN]--Whether to discard zero or not.
 * Return: The length of the converted string.Negative means failure.
 * Others: None.
*******************************************************************************/
const uint32 sTlkApiPow10Array[] = {1, 10, 100, 1000, 10000, 100000, 1000000,
	10000000, 100000000, 1000000000};
int tlkapi_decToStr(char *str, uint32 dec, uint num, bool isDropZero)
{
	uint08 temp;
	uint32 value;
	char *pTemp;

	if(str == nullptr || num == 0) return 0;
	if(num > 10) num = 10;

	pTemp = str;
	while(num--){
		value = dec/sTlkApiPow10Array[num];
		temp = value%10;
		if(isDropZero && temp == 0) continue;
		if(isDropZero) isDropZero = false;
		*pTemp++ = '0'+temp;
	}
	if(pTemp == str) *pTemp++ = '0';

	return (pTemp-str);
}
/******************************************************************************
 * Function: tlkapi_value_to_str
 * Descript: Converts a numeric value to a string in hex format.
 * Params:
 *     @str[OUT]--Converts the value of the input buffer.
 *     @hex[IN]--Hexadecimal value.
 *     @num[IN]--The number of conversions.
 *     @isDropZero[IN]--Whether to discard zero or not.
 * Return: The length of the converted string. Negative means failure.
 * Others: None.
*******************************************************************************/
int tlkapi_hexToStr(char *str, uint32 hex, uint num, bool isDropZero)
{
	char *pTemp;
	uint08 temp;
	uint08 offset;

	if(str == nullptr || num == 0) return 0;

	pTemp = str;
	if(num > 8) num = 8;
	while(num--){
		offset = num<<2;
		temp = (hex & (0xFUL<<offset))>>offset;
		if(isDropZero && temp == 0) continue;
		if(isDropZero) isDropZero = false;
		if(temp < 10) *pTemp++ = '0'+temp;
		else *pTemp++ = 'A'+(temp-10);
	}
	if(pTemp == str){
		*pTemp++ = '0';
		*pTemp++ = '0';
	}

	return (pTemp-str);
}


/******************************************************************************
 * Function: tlkapi_str_to_int32
 * Descript: Converts a string to an int32 value.
 * Params:
 *     @str[IN]--String to be Converted.
 *     @strLen[IN]--The length of the string to be Converted.
 *     @pValue[OUT]--The converted value.
 * Return: Conversion results. Negative means failure.
 * Others: None.
*******************************************************************************/
int tlkapi_strToInt32(char *str, int strLen, sint32 *pValue)
{
	sint32 value;
	uint08 isNeg;

	if(str == nullptr || strLen == 0 || pValue == nullptr){
		return -TLK_EPARAM;
	}

	while(strLen != 0 && str[0] == ' '){
		str ++;
		strLen --;
	}
	if(strLen == 0 || str[0] < '0' || str[0] > '9') return -TLK_EFAIL;

	value = 0;
	isNeg = false;
	if(str[0] == '-' || str[0] == '+'){
		if(str[0] == '-') isNeg = true;
		strLen --;
		str ++;
	}
	if(strLen == 0 || strLen > 10) return -TLK_EPARAM;

	while(strLen != 0){
		if(str[0] < '0' || str[0] > '9') break;
		value *= 10;
		value += (str[0]-'0');
		str ++;
		strLen --;
	}

	if(isNeg) value *= -1;
	*pValue = value;

	return TLK_ENONE;
}
int tlkapi_strToInt08(char *str, int strLen, sint08 *pValue)
{
	int ret;
	sint32 value;

	ret = tlkapi_strToInt32(str, strLen, &value);
	if(ret != TLK_ENONE) return ret;

	if(value < -255 || value > 255) return -TLK_EOVERFLOW;
	*pValue = value;

	return TLK_ENONE;
}
int tlkapi_strToInt16(char *str, int strLen, sint16 *pValue)
{
	int ret;
	sint32 value;

	ret = tlkapi_strToInt32(str, strLen, &value);
	if(ret != TLK_ENONE) return ret;

	if(value < -32767 || value > 32767) return -TLK_EOVERFLOW;
	*pValue = value;

	return TLK_ENONE;
}

/******************************************************************************
 * Function: tlkapi_str_to_uint32
 * Descript: Converts a string to an uint32 value.
 * Params:
 *     @str[IN]--String to be Converted.
 *     @strLen[IN]--The length of the string to be Converted.
 *     @pValue[OUT]--The converted value.
 * Return: Conversion results. Negative means failure.
 * Others: None.
*******************************************************************************/
int tlkapi_strToUint32(char *str, int strLen, uint32 *pValue)
{
	uint32 value;

	if(str == nullptr || strLen == 0 || strLen > 10 || pValue == nullptr){
		return -TLK_EPARAM;
	}

	while(strLen != 0 && str[0] == ' '){
		str ++;
		strLen --;
	}
	if(strLen == 0 || str[0] < '0' || str[0] > '9') return -TLK_EFORMAT;

	value = 0;
	while(strLen != 0){
		if(str[0] < '0' || str[0] > '9') break;
		value *= 10;
		value += (str[0]-'0');
		str ++;
		strLen --;
	}

	*pValue = value;

	return TLK_ENONE;
}
int tlkapi_strToUint08(char *str, int strLen, uint08 *pValue)
{
	int ret;
	uint32 value;

	ret = tlkapi_strToUint32(str, strLen, &value);
	if(ret != TLK_ENONE) return ret;

	if(value > 0xFF) return -TLK_EOVERFLOW;
	*pValue = value;

	return TLK_ENONE;
}
int tlkapi_strToUint16(char *str, int strLen, uint16 *pValue)
{
	int ret;
	uint32 value;

	ret = tlkapi_strToUint32(str, strLen, &value);
	if(ret != TLK_ENONE) return ret;

	if(value > 0xFFFF) return -TLK_EOVERFLOW;
	*pValue = value;

	return TLK_ENONE;
}


/******************************************************************************
 * Function: tlkapi_str_to_array
 * Descript: Converts a string to a hexadecimal array.
 * Params:
 *     @str[IN]--String to be Converted.
 *     @strLen[IN]--The length of the string to be Converted.
 *     @pHex[OUT]--The converted array.
 *     @hexLen[IN]--The length of the converted array.
 * Return: The length of the converted array. Negative means failure.
 * Others: None.
*******************************************************************************/
int tlkapi_strToArray(char *str, int strLen, uint08 *pHex, int hexLen)
{
	uint08 temp;
	uint08 flag0;
	uint08 flag1;
	uint08 length;

	if(str == nullptr || strLen == 0 || pHex == nullptr || hexLen == 0){
		return -TLK_EPARAM;
	}

	temp = 0;
	flag0 = 0;
	flag1 = 0;
	length = 0;
	while(strLen != 0){
		if(str[0] >= 'A' && str[0] <= 'F') temp |= (str[0]-'A')+10;
		else if(str[0] >= 'a' && str[0] <= 'f') temp |= (str[0]-'a')+10;
		else if(str[0] >= '0' && str[0] <= '9') temp |= (str[0]-'0');
		else if(str[0] == ' ') flag0 = 1;
		else return -TLK_EFORMAT;

		if(flag0 == 1){
			if(flag1 == 1){
				flag1 = 2;
				temp >>= 4;
			}
			flag0 = 0;
		}else{
			flag1 ++;
			temp <<= 4;
		}

		if(flag1 == 2){
			pHex[length++] = temp;
			temp = 0;
			if(length == hexLen) break;
		}

		str ++;
		strLen --;
	}

	return length;
}
/******************************************************************************
 * Function: tlkapi_array_to_str
 * Descript: Converts a array to a string.
 * Params:
 *     @pHex[IN]--Array to be Converted.
 *     @hexLen[IN]--The length of the array to be Converted.
 *     @str[OUT]--The converted string.
 *     @strLen[IN]--The length of the converted string.
 *     @split[IN]--The delimiter to add. Zero means no.
 * Return: The length of the converted string. Negative means failure.
 * Others:
 *     1. The last split will not be added.
*******************************************************************************/
#if (TLK_USB_UDB_ENABLE)
_attribute_retention_code_
#endif
int tlkapi_arrayToStr(uint08 *pHex, int hexLen, char *str, int strLen, char split)
{
	char *pStr;
	uint08 temp;
	uint08 temp0;
	int length;

	if(str == nullptr || strLen == 0 || pHex == nullptr || hexLen == 0){
		return -TLK_EPARAM;
	}

	temp = 0;
	length = 0;
	pStr = str;
	while(hexLen--){
		if(length+2 > strLen) break;
		length += 2;
		temp = *pHex;
		temp0 = (temp & 0xF0)>>4;
		temp &= 0x0F;
		if(temp0 < 10) *pStr++ = '0'+temp0;
		else *pStr++ = 'A'+(temp0-10);
		if(temp < 10) *pStr++ = '0'+temp;
		else *pStr++ = 'A'+(temp-10);
		if(split != 0 && hexLen != 0){
			if(length+1 > strLen) break;
			*pStr++ = split;
			length += 1;
		}
		pHex ++;
	}

	return (pStr-str);
}


char *tlkapi_str_findChar(char *pSrc, uint16 srcLen, char dst, uint16 *pOffset)
{
	uint16 offset;
	if(pSrc == NULL || srcLen == 0) return NULL;
	offset = 0;
	while(offset < srcLen){
		if(pSrc[offset] == dst) break;
		offset ++;
	}
	if(offset == srcLen) return NULL;
	if(pOffset != NULL) *pOffset = offset;
	return &pSrc[offset];
}
char *tlkapi_str_findStr(char *pSrc, uint16 srcLen, char *pDst, uint16 dstLen, uint16 *pOffset)
{
	if(pSrc == NULL || srcLen < dstLen || pDst == NULL || dstLen == 0) return NULL;
	if(dstLen == 1){
		return tlkapi_str_findChar(pSrc, srcLen, pDst[0], pOffset);
	}else{
		uint16 index = 0;
		uint16 offset = 0;
		while(index < srcLen){
			if(index+dstLen > srcLen) break;

			offset = 0;
			while(offset < dstLen && pSrc[index+offset] == pDst[offset]){
				offset ++;
			}
			if(offset == dstLen) break;

			index ++;
		}
		if(index+dstLen > srcLen) return NULL;
		if(pOffset != NULL) *pOffset = index;
		return &pSrc[index];
	}
}





