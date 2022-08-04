#pragma once
#include "CPacket.h"


typedef unsigned long long SID;

#ifdef AUTO_PACKET
extern LockFreeQueue<pair<SID, PacketPtr>> g_JobQueue;
#else
extern LockFreeQueue<pair<SID, CPacket*>> g_JobQueue;
#endif

unsigned __stdcall SingleUpdate(void* param);