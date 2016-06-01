#ifndef _COLLECTION_H
#define _COLLECTION_H
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
template<typename T>
class IEnumerator : public virtual Interface{
	///��ö�����͵Ļ���������Clone,Current,Index,Next,Reset,Evalated
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
#endif