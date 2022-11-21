#include <utility>


#include "ChatServer.h"
#include "MemoryPoolTls.h"
#include "ObjectPool.h"
#include "ChatLogic.h"
#include "LockFreeQueue.h"
#include "JOB.h"
#include "User.h"
#include "CommonProtocol.h"
#include "ProcPacket.h"
#include "Sector.h"

// 모니터링 대상
MemoryPoolTls<JOB> g_JobPool;
LockFreeQueue<JOB*> g_JobQueue(MAX_JOB_QUEUE, false);


ObjectPool<User> g_UserPool(20000, true);

Tracer g_Tracer;

// session key -> account no 중복 검사 시 순회 필요 ( login )
// account no key -> ?? session key 중복 검사?? join - leave 시 검색해서 찾아야 함.
unordered_map<SS_ID, User*> g_UserMap;
unsigned int g_connect_cnt;
unsigned int g_login_cnt;
alignas(64) ULONG g_job_proc;

// polling 먼저 하고 차후에 event driven 구조로
// time out 처리 프레임 필요
unsigned __stdcall SingleUpdate(void* param)
{
	JOB* job;
	bool ret_deq;
	ULONGLONG oldTick = GetTickCount64();
	while (true)
	{
		// time out 
		ULONGLONG nowTick = GetTickCount64();
		ULONGLONG diffTick = nowTick - oldTick;
		if (diffTick >= 1000)
		{

			for (auto& iter : g_UserMap)
			{
				SS_ID sid = iter.first;
				User* user = iter.second;
				if (GetTickCount64() - user->GetLastRecvTime() >= 40000) {
					user->last_recv_time = GetTickCount64();
					g_server.DisconnectSession((unsigned long long)sid);
					g_Tracer.trace(00, user);
				}
			}
			
			oldTick = nowTick - diffTick;
		}

		ret_deq = g_JobQueue.Dequeue(&job);
		if (!ret_deq) continue;

		// job 처리
		SS_ID sid = job->id;
		unsigned char job_type = job->type;

		switch (job_type)
		{
		case JOB::ON_RECV:
		{
			CPacket* packet = job->packet;
			WORD packet_type;
			(*packet) >> packet_type;
			
			User* user = g_UserMap[sid];


			bool result_proc = false;
			// Packet Proc
			switch (packet_type)
			{
			case en_PACKET_CS_CHAT_REQ_LOGIN:
			{
				g_Tracer.trace(10, (PVOID)user->session_id);
				result_proc = ProcChatLogin(user, packet);
				break;
			}
			case en_PACKET_CS_CHAT_REQ_SECTOR_MOVE:
			{
				g_Tracer.trace(11, (PVOID)user->session_id);
				result_proc = ProcChatSectorMove(user, packet);
				break;
			}
			case en_PACKET_CS_CHAT_REQ_MESSAGE:
			{
				g_Tracer.trace(12, (PVOID)user->session_id);
				result_proc = ProcChatMessage(user, packet);
				break;
			}
			case en_PACKET_CS_CHAT_REQ_HEARTBEAT:
			{
				//  필요 없음
				break;
			}
			default: // undefined protocol
			{
				// 예외 처리


			}

			}

			if (!result_proc) g_server.DisconnectSession(user->session_id);

			packet->SubRef();

			break;
		}
		case JOB::ON_CLI_JOIN:
		{
			CreateUser(sid);
			break;
		}
		case JOB::ON_CLI_LEAVE:
		{
			DeleteUser(sid);
			break;
		}
		default:
		{
			// undefined job

		}


		}

		g_JobPool.Free(job);
		InterlockedIncrement(&g_job_proc);
	}



	return 0;
}

bool FindUser(SS_ID s_id, User** user)
{
	auto iter = g_UserMap.find(s_id);
	if (iter == g_UserMap.end())
	{
		return false;
	}

	*user = iter->second;

	return true;
}

void CreateUser(SS_ID s_id)
{
	User* user = g_UserPool.Alloc();
	user->session_id = s_id;

	user->last_recv_time = GetTickCount64();

	

	g_Tracer.trace(1, (PVOID)user->session_id);

	// 추가
	g_UserMap[s_id] = user;
	g_connect_cnt++;

}

void DeleteUser(SS_ID s_id)
{
	User* user;

	if (!FindUser(s_id, &user))
	{
		g_Tracer.trace(3, (PVOID)s_id);
		return;
	}

	if (user->is_in_sector)
	{
		Sector_RemoveUser(user);
	}

	g_Tracer.trace(2, user);

	if (user->is_login)
		g_login_cnt--;
	else
		g_connect_cnt--;

	g_UserMap.erase(s_id);

	g_UserPool.Free(user);

	return;

}

void SendMessageUni(CPacket* packet, SS_ID sid)
{
	User* user;
	if (!FindUser(sid, &user))
	{

		return;
	}

	g_server.SendPacket(user->session_id, packet);

	return;
}

void SendMessageSector(CPacket* packet, int sector_x, int sector_y)
{
	auto& sector = g_SectorList[sector_y][sector_x];

	for (auto& user : sector)
	{
		SendMessageUni(packet, user->session_id);

	}

	return;
}


void SendMessageAround(CPacket* packet, SS_ID sid)
{
	SectorAround sect_around;
	int cnt;
	User* user;
	
	if (!FindUser(sid, &user))
	{


		return;
	}


	GetSectorAround(user->sector_x, user->sector_y, &sect_around);


	for (cnt = 0; cnt < sect_around.count; cnt++)
	{
		SendMessageSector(packet, sect_around.around[cnt].x, sect_around.around[cnt].y);
	}

	return;
}