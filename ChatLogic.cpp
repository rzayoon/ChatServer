#include <utility>
#include <unordered_map>
using std::unordered_map;

#include "MemoryPoolTls.h"
#include "ChatLogic.h"
#include "LockFreeQueue.h"


MemoryPoolTls<JOB> g_JobPool;
LockFreeQueue<JOB*> g_JobQueue;

unsigned __stdcall SingleUpdate(void* param)
{




	return 0;
}