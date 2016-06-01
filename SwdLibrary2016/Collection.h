#ifndef _COLLECTION_H
#define _COLLECTION_H
#include <iostream>
#include "Common.h"

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
///=================������ʩ Basic facilities / Foundation=========================
/// Plain-Old-Data Type ������������ ����true��ʾ�ǻ�������
/// �˴����Գ�ֿ�����νģ��Ԫ�����ʵ�൱��if...else������˵ģʽƥ�䣬����ν���ͼ���
/// ��������������˹�Ȼ�����亦����ʱ�������Ӹ羫�µĴ��뷽��Ȼ����
template<typename T>
struct POD
{
	/// <summary>Returns true if the type is a Plain-Old-Data type.</summary>
	static const bool Result = false;
};

template<>struct POD<bool>{ static const bool Result = true; };
template<>struct POD<signed __int8>{ static const bool Result = true; };
template<>struct POD<unsigned __int8>{ static const bool Result = true; };
template<>struct POD<signed __int16>{ static const bool Result = true; };
template<>struct POD<unsigned __int16>{ static const bool Result = true; };
template<>struct POD<signed __int32>{ static const bool Result = true; };
template<>struct POD<unsigned __int32>{ static const bool Result = true; };
template<>struct POD<signed __int64>{ static const bool Result = true; };
template<>struct POD<unsigned __int64>{ static const bool Result = true; };
template<>struct POD<char>{ static const bool Result = true; };
template<>struct POD<wchar_t>{ static const bool Result = true; };
template<typename T>struct POD<T*>{ static const bool Result = true; };
template<typename T>struct POD<T&>{ static const bool Result = true; };
template<typename T, typename C>struct POD<T C::*>{ static const bool Result = true; };
template<typename T, int _Size>struct POD<T[_Size]>{ static const bool Result = POD<T>::Result; };
template<typename T>struct POD<const T>{ static const bool Result = POD<T>::Result; };
template<typename T>struct POD<volatile T>{ static const bool Result = POD<T>::Result; };
template<typename T>struct POD<const volatile T>{ static const bool Result = POD<T>::Result; };

///���ͼ���
///�Ƴ����������Լ��������δ�
template<typename T>
struct RemoveReference
{
	typedef T			Type;
};

template<typename T>
struct RemoveReference<T&>
{
	typedef T			Type;
};

template<typename T>
struct RemoveReference<T&&>
{
	typedef T			Type;
};

template<typename T>
struct RemoveConst
{
	typedef T			Type;
};

template<typename T>
struct RemoveConst<const T>
{
	typedef T			Type;
};

template<typename T>
struct RemoveVolatile
{
	typedef T			Type;
};

template<typename T>
struct RemoveVolatile<volatile T>
{
	typedef T			Type;
};

template<typename T>
struct RemoveCVR
{
	typedef T								Type;
};

template<typename T>
struct RemoveCVR<T&>
{
	typedef typename RemoveCVR<T>::Type		Type;
};

template<typename T>
struct RemoveCVR<T&&>
{
	typedef typename RemoveCVR<T>::Type		Type;
};

template<typename T>
struct RemoveCVR<const T>
{
	typedef typename RemoveCVR<T>::Type		Type;
};

template<typename T>
struct RemoveCVR<volatile T>
{
	typedef typename RemoveCVR<T>::Type		Type;
};
///int&&�������Ϊ��ֵ����
template<typename T>
typename RemoveReference<T>::Type&& MoveValue(T&& value)
{
	return (typename RemoveReference<T>::Type&&)value;
}
///================================
template<typename T>
struct KeyType {
public:
	typedef T Type;

	static T GetKeyValue(const T& value){
		return value;
	}
};
/// Why virtual inherits?
/// Because it's a interface,allow multiple inheritance
template<typename T>
class IEnumerator : public virtual Interface{
	///��ö�����͵Ļ���������Clone,Current,Index,Next,Reset,Evalated
public:
	typedef T ElementType;

	virtual IEnumerator<T>* Clone()const = 0;
	virtual const T& Current()const = 0;
	virtual int Index() const = 0;
	virtual bool Next()=0;
	virtual void Reset()=0;
	virtual bool Evalated()const{ return false; };
};

template<typename T> 
class IEnumerable : public virtual Interface{
public:
	typedef T ElementType;
	virtual IEnumerator<T>* CreateEnumerator()const = 0;

};

template<typename T,bool PODType>
class ListStore abstract : public Object{

};

template<typename T>
class ListStore<T, false> : public Object{
protected:
	static void CopyObjects(T* dest, const T* source, int count){
		///��ֹ�ڴ��ص�
		/// dst...........src ���dst��ǰ���ȸ���ǰ�ߣ���ֹǰ�߱�����
		/// src...........dst ���dst�ں��ȸ��ƺ�ߣ���ֹ��߱�����
		if (dest < src)
		{
			for (int i = 0; i < count; i++){
				dest[i] = MoveValue(source[i]);
			}
		}
		else if (dest > src)
		{
			for (int i = count - 1; i >= 0; i++){
				dest[i] = MoveValue(source[i]);
			}
		}
		//else equal , no need to copy
	}
	static void ClearObjects(T* dest, int count){
		for (int i = 0; i < count; i++){
			dest[i] = T();
		}
	}
};
template<typename T>
class ListStore < T, true > abstract:public Object{
protected:
	static void CopyObjects(T* dest, const T* source, int count){
		memmove(dest, source, sizeof(T)*count);
	}
	static void ClearObjects(T* dest, int count){
		
	}
};
template<typename T>
class ArrayBase abstract : public ListStore<T,POD<T>::Result>, public virtual IEnumerable<T>{
protected:
	class Enumerator : public Object, public virtual IEnumerator < T > {
	protected:
		const ArrayBase<T>* container;
		int index;
	public:
		Enumerator(const ArrayBase<T>* _container, int _index = -1){
			container = _container;
			index = _index;
		}
		IEnumerator<T>* Clone()const{
			return new Enumerator(container,index);
		}
		const T& Current()const{
			return container->Get(index);
		}
		int Index() const{
			return index;
		}
		bool Next(){
			index++;
			return index>=0 && index < container->Count();
		}
		void Reset(){
			index = -1;
		}

	};
	T* buffer;
	int count;
public:
	ArrayBase()
		:buffer(0), count(0)
	{}
	///CreateEnumerator,Count,Get,operator[]
	IEnumerator<T>* CreateEnumerator()
	{
		return;
	}
	int Count()const{
		return count;
	}
	const T& Get(int index){
		CHECK_ERROR(index >= 0 && index < count, L"ArrayBase::Get(int) - array index out of range");
		return buffer[index];
	}
	T& operator[](int index){
		CHECK_ERROR(index >= 0 && index < count, L"ArrayBase::operator[](int) - array index out of range");
		return buffer[index];
	}
};
/// Why two types needed?
template<typename T,typename K = typename KeyType<T>::Type>
class ListBase abstract : public ArrayBase <T>
{
protected:
	int capacity;
	bool lessMemoryMode;
	int CalculateCapacity(int expected)
	{
		int result = capacity;
		if (expected > capacity){
			result = capacity / 4 * 5 + 1;
		}
		return result;
	}
	void MakeRoom(int index, int _count){
		int newCount = ArrayBase<T>::count + _count;
		if (newCount > capacity){
			int newCapacity = CalculateCapacity(newCount);
			T* newBuffer = new T[newCapacity];
			ListStore<T, POD<T>::Result>::CopyObjects(newBuffer, ArrayBase<T>::buffer, index);
			ListStore<T, POD<T>::Result>::CopyObjects(newBuffer + index + _count, 
													  ArrayBase<T>::buffer+index, 
													  ArrayBase<T>::count-index);
			delete[] ArrayBase<T>::buffer;
			ArrayBase<T>::buffer = newBuffer;
			capacity = newCapacity;
		}
		else
		{
			ListStore<T, POD<T>::Result>::CopyObjects(ArrayBase<T>::buffer+index+_count, 
													  ArrayBase<T>::buffer, 
													  ArrayBase<T>::count-index);
		}
		ArrayBase<T>::count = newCount;
	}
	void ReleaseUnnecessaryBuffer(int previousCount){
		int _count = previousCount - ArrayBase<T>::count;
		if (ArrayBase<T>::buffer && _count > 0){
			ListStore<T, POD<T>::Result>::ClearObjects(ArrayBase<T>::buffer + ArrayBase<T>::count, 
													   _count);
		}
		if (lessMemoryMode && ArrayBase<T>::count <= previousCount / 2){
			int newCapacity = 5 / 8 * capacity;

			T* newBuffer = new T[newCapacity];
			ListStore<T, POD<T>::Result>::CopyObjects(newBuffer,
													  ArrayBase<T>::buffer,
													  ArrayBase<T>::count);
			delete[] ArrayBase<T>::buffer;
			capacity = newCapacity;
			ArrayBase<T>::buffer = newBuffer;
		}
	}
public:
	ListBase(){
		ArrayBase<T>::count = 0;
		ArrayBase<T>::buffer = 0;
		capacity = 0;
		lessMemoryMode = true;
	}
	~ListBase()
	{
		delete[] ArrayBase<T>::buffer;
	}
	void SetLessMemoryMode(bool mode){
		lessMemoryMode = mode;
	}
	bool RemoveAt(int index){
		CHECK_ERROR(index >= 0 && index < this->Count(), L"ListBase<T,K>::removeAt(int)-List index out of range");
		int previousCount = ArrayBase<T>::count;
		ListStore<T, POD<T>::Result>::CopyObjects(ArrayBase<T>::buffer+index,
												  ArrayBase<T>::buffer+index+1,
												  ArrayBase<T>::count-index-1
												  );
		ArrayBase<T>::count--;
		ReleaseUnnecessaryBuffer(previousCount);
		return true;
	}
	bool RemoveRange(int index, int _count){
		CHECK_ERROR(index>=0 && index<ArrayBase<T>::count,L"ListBase<T,K>::removeRange(int,int)-List index out of range")
		CHECK_ERROR(index+_count >= 0 && index + _count < ArrayBase<T>::count, L"ListBase<T,K>::removeRange(int,int)-index+_count out of range");
		int previousCount = ArrayBase<T>::count;
		ListStore<T, POD<T>::Result>::CopyObjects(ArrayBase<T>::buffer + index,
												  ArrayBase<T>::buffer + index + _count,
												  ArrayBase<T>::count - index -_count
												  );
		ArrayBase<T>::count -= _count;
		ReleaseUnnecessaryBuffer(previousCount);
		return true;
	}
	bool Clear(){
		int previousCount = ArrayBase<T>::count;
		ArrayBase<T>::count = 0;
		if (lessMemoryMode){
			delete[] ArrayBase<T>::buffer;
			ArrayBase<T>::buffer = 0;
		}
		else
		{
			ReleaseUnnecessaryBuffer(previousCount);
		}
		return true;
	}
};


#endif