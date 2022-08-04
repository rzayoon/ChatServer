#pragma once
#include "CPacket.h"


typedef unsigned long long SID;

struct JOB
{
	SID id;
#ifdef AUTO_PACKET
	PacketPtr packet;
#else
	CPacket* packet;
#endif
};
 


extern MemoryPoolTls<JOB> g_JobPool;
extern LockFreeQueue<JOB*> g_JobQueue;


unsigned __stdcall SingleUpdate(void* param);