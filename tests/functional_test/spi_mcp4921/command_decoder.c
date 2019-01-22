#include "command_decoder.h"
#include "string.h"

#define SPACE 			0x00000020
#define TERMINATOR 	0x0000000D 

Token asToken[MAX_TOKEN_NR];
unsigned char ucTokenNr;
Keyword asKeywordList[MAX_KEYWORD_NR]= 
{
	{DACSET,"dacset"},
	{ID,"id"}
};

/*----------------------------------------
		   Dekodowanie komunikatów
 *----------------------------------------*/
	unsigned char ucFindTokensInString (char *pcString) {
	
	unsigned int uiLength;

  TokenStatus eState = DELIMITER;
	ucTokenNr = 0;

    for (uiLength=0;; uiLength++) {
			switch (eState) {
	    	case TOKEN:
					if((pcString[uiLength] == NULL)||(ucTokenNr == MAX_TOKEN_NR))
		    			return ucTokenNr;
					else if(pcString[uiLength] == SPACE)
		    			eState=DELIMITER;
					else
						eState = TOKEN;
					break;
	    		
				case DELIMITER:
					if (pcString[uiLength] == NULL)
		    			return ucTokenNr;
					else if (pcString[uiLength] == SPACE) 
						eState = DELIMITER;
					else{
		    			eState = TOKEN;
		    			asToken[ucTokenNr].uValue.pcString = pcString + uiLength;
							ucTokenNr++;
					}
					break;
			}
    }
    return 0;
}

//_____________________________________________________________
Result eStringToKeyword (char cStr[],KeywordCode *peKeyword) {

	unsigned char ucKeywordCtr;

	for (ucKeywordCtr = 0; ucKeywordCtr < MAX_KEYWORD_NR; ucKeywordCtr ++) {
		if (eCompareString (cStr, asKeywordList[ucKeywordCtr].cString) == EQUAL) {
			*peKeyword = asKeywordList[ucKeywordCtr].eCode;
			return OK;
		}
	}
	return ERRORR;
}
//_____________________________________________________________
void DecodeTokens(void) {

	unsigned char ucTokenCtr;

	for (ucTokenCtr=0; ucTokenCtr < ucTokenNr ; ucTokenCtr ++ ) {
		KeywordCode eKeywordCode;
		unsigned int uiNumberValue;
		Token * pasToken;
		pasToken = &asToken[ucTokenCtr];

		if (eStringToKeyword(pasToken ->uValue.pcString, &eKeywordCode) == OK) {
			pasToken->eType = KEYWORD;
			pasToken->uValue.eKeyword = eKeywordCode;
		}
		else if (eHexStringToUInt(pasToken ->uValue.pcString, &uiNumberValue) == OK) {
			pasToken->eType = NUMBER;
			pasToken->uValue.uiNumber = uiNumberValue;
		}
		else {
			pasToken ->eType = STRING;
		}
	}
}
//_____________________________________________________________
void DecodeMsg(char *cStr) {
ucFindTokensInString(cStr);
	ReplaceCharactersInString(cStr,SPACE,NULL);
	DecodeTokens();
}


