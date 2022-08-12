#include <thread>
#include <utility>

#include "MemoryPoolTls.h"
#include "ChatServer.h"
#include "ChatLogic.h"


ChatServer g_server;

ChatServer::ChatServer()
{
	m_hSingleThread = (HANDLE)_beginthreadex(nullptr, 0, SingleUpdate, nullptr, 0, nullptr);

}


ChatServer::~ChatServer()
{
	

}

bool ChatServer::OnConnectionRequest(wchar_t* ip, unsigned short port)
{
	if (g_login_cnt > max_user) // ´À½¼ÇÏ°Ô
	{
		return false;
	}

	return true;
}

void ChatServer::OnClientJoin(unsigned long long session_id)
{
	JOB* job = g_JobPool.Alloc();
	job->id = session_id;
	job->type = JOB::ON_CLI_JOIN;

	g_JobQueue.Enqueue(job);

	return;
}

void ChatServer::OnClientLeave(unsigned long long session_id)
{
	JOB* job = g_JobPool.Alloc();
	job->id = session_id;
	job->type = JOB::ON_CLI_LEAVE;
	
	g_JobQueue.Enqueue(job);

	return;
}

void ChatServer::OnRecv(unsigned long long session_id, CPacket* packet)
{
	JOB* job = g_JobPool.Alloc();
	
	job->type = JOB::ON_RECV;
	job->id = session_id;

	packet->AddRef();
	job->packet = packet;
	
	g_JobQueue.Enqueue(job);

	return;
}



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

