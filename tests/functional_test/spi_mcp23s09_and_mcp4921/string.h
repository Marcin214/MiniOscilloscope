#define NIBBLE 0x0000000F
#define NULL 0

typedef enum { OK, ERRORR} Result;
typedef enum { DIFFERENT , EQUAL } CompResult;

/*----------------------------------------
		 Lancuchy znakowe - operacje proste
 *----------------------------------------*/
void CopyString(char pcSource[], char pcDestination[]);
CompResult eCompareString(char pcStr1[], char pcStr2[]);
void AppendString (char pcSourceStr[],char pcDestinationStr[]);
void ReplaceCharactersInString(char pcString[],char cOldChar,char cNewChar);

/*----------------------------------------
		 Lancuchy znakowe - konwersje
 *----------------------------------------*/
void UIntToHexStr (unsigned int uiValue, char pcStr[]);
Result eHexStringToUInt(char pcStr[],unsigned int *puiValue);
void AppendUIntToString (unsigned int uiValue, char pcDestinationStr[]);

