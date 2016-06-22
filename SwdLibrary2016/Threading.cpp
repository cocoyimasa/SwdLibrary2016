#include "Threading.h"
#include "Collection.h"
#include <Windows.h>

using namespace threading_internal;
/*
编程恰如写文章，非饱读之士妙手不能得精致之文。
观vczh之代码，严谨优美，井井有条，秩序俨然，如排兵布阵，处处得章法。
结构巧妙，技巧纯熟。
妙不可言。
*/
///
namespace threading_internal
{
	struct WaitableData
	{
		HANDLE handle;
		WaitableData(HANDLE _handle)
			:handle(_handle)
		{
		}
	};
}
WaitableObject::WaitableObject()
	:waitableData(0)
{
}
void WaitableObject::SetData(threading_internal::WaitableData* data)
{
	waitableData = data;
}
bool WaitableObject::isCreated()
{
	return waitableData != 0;
}
bool WaitableObject::Wait()
{
	return WaitForTime(INFINITE);
}
bool WaitableObject::WaitForTime(int ms)
{
	if (isCreated())
	{
		if (WaitForSingleObject(waitableData->handle, (DWORD)ms) == WAIT_OBJECT_0)
		{
			return true;
		}
	}
	return false;
}
bool WaitableObject::WaitAll(WaitableObject** objects, int count)
{
	Array<HANDLE> handles(count);
	for (int i = 0; i < count; i++)
	{
		handles[i] = objects[i]->waitableData->handle;
	}
	DWORD result = WaitForMultipleObjects((DWORD)count, &handles[0], true, INFINITE);
	return result == WAIT_OBJECT_0 || result == WAIT_ABANDONED_0;
}
bool WaitableObject::WaitAllForTime(WaitableObject** objects, int count, int ms)
{
	Array<HANDLE> handles(count);
	for (int i = 0; i < count; i++)
	{
		handles[i] = objects[i]->waitableData->handle;
	}
	DWORD result = WaitForMultipleObjects((DWORD)count, &handles[0], true, (DWORD)ms);
	return result == WAIT_OBJECT_0 || result == WAIT_ABANDONED_0;
}
int WaitableObject::WaitAny(WaitableObject** objects, int count, bool* abandoned)
{
	Array<HANDLE> handles(count);
	for (int i = 0; i < count; i++)
	{
		handles[i] = objects[i]->waitableData->handle;
	}
	DWORD result = WaitForMultipleObjects((DWORD)count, &handles[0], FALSE, INFINITE);
	if (WAIT_OBJECT_0 <= result && result < WAIT_OBJECT_0 + count)
	{
		*abandoned = false;
		return result - WAIT_OBJECT_0;
	}
	else if (WAIT_ABANDONED_0 <= result && result < WAIT_ABANDONED_0 + count)
	{
		*abandoned = true;
		return result - WAIT_ABANDONED_0;
	}
	else
	{
		return -1;
	}
}
int WaitableObject::WaitAnyForTime(WaitableObject** objects, int count, int ms, bool* abandoned)
{
	Array<HANDLE> handles(count);
	for (int i = 0; i<count; i++)
	{
		handles[i] = objects[i]->waitableData->handle;
	}
	DWORD result = WaitForMultipleObjects((DWORD)count, &handles[0], FALSE, (DWORD)ms);
	if (WAIT_OBJECT_0 <= result && result<WAIT_OBJECT_0 + count)
	{
		*abandoned = false;
		return result - WAIT_OBJECT_0;
	}
	else if (WAIT_ABANDONED_0 <= result && result<WAIT_ABANDONED_0 + count)
	{
		*abandoned = true;
		return result - WAIT_ABANDONED_0;
	}
	else
	{
		return -1;
	}
}
namespace threading_internal
{
	struct ThreadData : public WaitableData
	{
		DWORD id;
		ThreadData()
			:WaitableData(NULL)
		{
			id = -1;
		}
	};
	class ProceduredThread : public Thread
	{

	};
}