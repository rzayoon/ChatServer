#include <algorithm>


#include "ChatLogic.h"
#include "LockFreeQueue.h"

using std::pair;

#ifdef AUTO_PACKET
LockFreeQueue<pair<SID, PacketPtr>> g_JobQueue;
#else
LockFreeQueue<pair<SID, CPacket*>> g_JobQueue;
#endif

unsigned __stdcall SingleUpdate(void* param)
{




	return 0;
}