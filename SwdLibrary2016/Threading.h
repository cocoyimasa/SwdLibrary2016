#ifndef _THREADING_H
#define _THREADING_H

#include "Common.h"
#include "Function.h"
using namespace vl;
namespace threading_internal
{
	struct WaitableData;
	struct ThreadData;
	struct MutexData;
	struct SemaphoreData;
	struct EventData;
	struct CriticalSectionData;
	struct ConditionVariableData;
}

class WaitableObject : public Object, public NotCopyable
{
private:
	threading_internal::WaitableData* waitableData;
protected:
	WaitableObject();
	void SetData(threading_internal::WaitableData* data);
public:
	bool isCreated();
	bool Wait();
	bool WaitForTime(int ms);
	static bool WaitAll(WaitableObject** objects, int count);
	static bool WaitAllForTime(WaitableObject** objects, int count, int ms);
	static int WaitAny(WaitableObject** objects, int count, bool* abandoned);
	static int WaitAnyForTime(WaitableObject** objects, int count, int ms, bool* abandoned);
};

class Thread : public WaitableObject
{
	friend void InternalThreadProc(Thread* thread);
public:
	enum ThreadState
	{
		NotStarted,
		Running,
		Stopped
	};
	typedef void(*ThreadProcedure)(Thread*, void*);
protected:
	threading_internal::ThreadData* internalData;
	volatile ThreadState threadState;
	virtual void Run() = 0;
	Thread();
public:
	~Thread();
	static Thread* CreateAndStart(ThreadProcedure procedure, void* argument = 0, bool deleteAfterStopped = true);
	static Thread* CreateAndStart(const Func<void()>& procedure, bool deleteAfterStopped = true);
	static void Sleep(int ms);
	static int GetCPUCount();
	bool Start();
	bool Stop();
	ThreadState GetState();
	void SetCPU(int index);
};
#endif