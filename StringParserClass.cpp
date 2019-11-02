/*
 * StringParserClass.cpp
 *
 *  Created on: Oct 8, 2017
 *      Author: keith
 */

#include <string>
#include <string.h>

#include<iostream>
#include "../327_proj3_test/includes/constants.h"
#include "../327_proj3_test/includes/StringParserClass.h"
using namespace std;

//dont forget to initialize member variables
KP_StringParserClass::StringParserClass::StringParserClass(void){
	pStartTag = new char(0);
	pEndTag = new char(0);
	areTagsSet = false;
}
//call cleanup to release any allocated memory
KP_StringParserClass::StringParserClass::~StringParserClass(void) {
	delete pStartTag;
	delete pEndTag;
	pEndTag = NULL;
	pStartTag = pEndTag;
}
//these are the start tag and the end tags that we want to find,
//presumably the data of interest is between them, please make a
//COPY of what pStartTag and pEndTag point to.  In other words
//DO NOT SET pStartTag = pStart
//returns:
//SUCCESS
//ERROR_TAGS_NULL if either pStart or pEnd is null
int KP_StringParserClass::StringParserClass::setTags(const char *pStart, const char *pEnd) {
	if (pStart == NULL && pEnd == NULL) {
		return ERROR_TAGS_NULL;
	}
	else {
		pStartTag = strdup(pStart);
		pEndTag = strdup(pEnd);
		areTagsSet = true;
		return SUCCESS;
	}
}

//First clears myVector
//going to search thru pDataToSearchThru, looking for info bracketed by
//pStartTag and pEndTag, will add that data to myStrings
//returns
//SUCCESS  finished searching for data between tags, results in myStrings (0 or more entries)
//ERROR_TAGS_NULL if either pStart or pEnd is null
//ERROR_DATA_NULL pDataToSearchThru is null
int KP_StringParserClass::StringParserClass::getDataBetweenTags(char *pDataToSearchThru, std::vector<std::string> &myVector) {
	myVector.clear();
	if (!areTagsSet) {
		return ERROR_TAGS_NULL;
	}
	if (pDataToSearchThru == NULL) {
		return ERROR_DATA_NULL;
	}
	char* p;
	int x;
	while (findTag(pStartTag, pDataToSearchThru, p) == SUCCESS) {
		x = findTag(pStartTag, pDataToSearchThru, p);
		pDataToSearchThru = *(&p);
		string str;
		char *pEnd;
		char* pStart = *(&p);
		char *pNonceStart = 0;
		char *pNonceEnd = 0;
		if (x == SUCCESS) {
			pNonceStart = pDataToSearchThru;
			pStart = pNonceStart;
			x = findTag(pEndTag, pStart, pEnd);
			switch (x) {
			case SUCCESS:
				pNonceEnd = pStart;
				str.assign(pNonceStart, *(&pNonceEnd) - pNonceStart);
				myVector.push_back(str);
				pStart = pEnd;
				pDataToSearchThru = pEnd;
				break;
			}
		}
	}
	return SUCCESS;
}
//Searches a string starting at pStart for pTagToLookFor
//returns:
//SUCCESS  found pTagToLookFor, pStart points to beginning of tag and pEnd points to end of tag
//FAIL did not find pTagToLookFor and pEnd points to 0
//ERROR_TAGS_NULL if either pStart or pEnd is null
int KP_StringParserClass::StringParserClass::findTag(char *pTagToLookFor, char *&pStart, char *&pEnd) {
	if (!areTagsSet) {
		return ERROR_TAGS_NULL;
	}
	if (pTagToLookFor == 0 || pStart == 0) {
		return FAIL;
	}
	pEnd = 0;
	int iLen = strlen(pStart);
	int lenTag = strlen(pTagToLookFor);
	for (int i = 0; i < iLen; ++i) {
		if (*(pStart) == *pTagToLookFor) {
			if (strncmp((pStart), pTagToLookFor, lenTag) == 0) {
				pEnd = pStart + lenTag;
				return SUCCESS;
			}
		}
		pStart++;
	}
	return FAIL;
}
