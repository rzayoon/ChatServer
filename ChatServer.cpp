#include <thread>
#include <utility>

#include "MemoryPoolTls.h"
#include "ChatServer.h"
#include "ChatLogic.h"


ChatServer::ChatServer()
{
	m_hSingleThread = (HANDLE)_beginthreadex(nullptr, 0, SingleUpdate, nullptr, 0, nullptr);

}


ChatServer::~ChatServer()
{
	

}

bool ChatServer::OnConnectionRequest(wchar_t* ip, unsigned short port)
{
	return true;
}

void ChatServer::OnClientJoin(unsigned long long session_id)
{
	// session id ¾Ë¸°´Ù?

}

#ifdef AUTO_PACKET
void ChatServer::OnRecv(unsigned long long session_id, PacketPtr packet)
{
	JOB* job = g_JobPool.Alloc();
	
	job->packet = packet;

	g_JobQueue.Enqueue(job);

	return;
}
#else
void ChatServer::OnRecv(unsigned long long session_id, CPacket* packet)
{
	JOB* job = g_JobPool.Alloc();
	
	packet->AddRef();
	job->packet = packet;
	
	g_JobQueue.Enqueue(job);

	return;
}
#endif

void ChatServer::OnSend(unsigned long long session_id, int send_size)
{

	return;
}


void ChatServer::OnWorkerThreadBegin()
{
	return;
}

void ChatServer::OnWorkerThreadEnd()
{
	return;
}

void ChatServer::OnError(int errorcode, const wchar_t* msg)
{

	return;
}

