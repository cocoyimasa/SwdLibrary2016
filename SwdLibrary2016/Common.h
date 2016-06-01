#ifndef _COMMON_H
#define _COMMON_H
#include <cstdio>
#include <cstdlib>
#include <iostream>

class Error{
protected:
	const wchar_t* description;
public:
	Error(const wchar_t* _description){
		description = _description;
	}
	const wchar_t* Description(){
		return description;
	}
};

#define CHECK_ERROR(CONDITION,DESCRIPTION) do{if(!(CONDITION))throw Error(DESCRIPTION);}while(0)
#endif