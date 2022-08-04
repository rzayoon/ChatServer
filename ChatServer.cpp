#include <thread>

#include "ChatServer.h"
#include "ChatLogic.h"



ChatServer::ChatServer()
{
	m_hSingleThread = (HANDLE)_beginthreadex(nullptr, 0,SingleLogic , nullptr, 0, nullptr);

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

}
#else
void ChatServer::OnRecv(unsigned long long session_id, CPacket* packet);
{

}
#endif

void ChatServer::OnSend(unsigned long long session_id, int send_size)
{

}


void ChatServer::OnWorkerThreadBegin()
{

}

void ChatServer::OnWorkerThreadEnd()
{

}

void ChatServer::OnError(int errorcode, const wchar_t* msg)
{

}

