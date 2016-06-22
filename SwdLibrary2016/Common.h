#ifndef _COMMON_H
#define _COMMON_H
#include <cstdio>
#include <cstdlib>
#include <iostream>
class Object
{
public:
	virtual ~Object(){}
};
class NotCopyable{
private:
	NotCopyable(const NotCopyable&){}
	NotCopyable& operator=(const NotCopyable&){ return *this; }
public:
	NotCopyable(){}
};

class Interface :private NotCopyable{
public:
	virtual ~Interface()
	{}
};
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