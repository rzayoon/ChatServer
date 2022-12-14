#pragma once
#include <Windows.h>
#include "CPacket.h"
#include "RingBuffer.h"
#include "LockFreeQueue.h"
#include "Tracer.h"

#define STACK_INDEX
//#define TRACE_SESSION

#define MAX_SENDQ 50


#ifdef TRACE_SESSION
enum
{
	enDisconnect = 0,
	enSendFailed = 1,
	enSendAsync = 2,
	enSendSync = 3,
	enSendPacket = 7,
	enAccept = 8,
	enCancelIO = 9,
	enLeave = 10,
	enRecvFailed = 11,
	enRecvAsync = 12,
	enRecvSync = 13,
	enRelease = 99

};
#endif

class alignas(64) Session
{
public:

	Session();
	~Session();

	void Lock();
	void Unlock();

#ifndef STACK_INDEX
	bool used;
#endif

	alignas(64) unsigned int session_id;
	unsigned short session_index;

	LARGE_INTEGER send_time;

	OVERLAPPED recv_overlapped;
	SOCKET recv_sock;
	DWORD recvbytes;
	OVERLAPPED send_overlapped;
	SOCKET send_sock;
	DWORD sendbytes;
	RingBuffer recv_q = RingBuffer(2000);
#ifdef AUTO_PACKET
	LockFreeQueue<PacketPtr> send_q = LockFreeQueue<PacketPtr>(MAX_SENDQ, false);
#else
	LockFreeQueue<CPacket*> send_q = LockFreeQueue<CPacket*>(MAX_SENDQ, false);
#endif

	// interlock
	alignas(64) SOCKET sock;
	alignas(64) int io_count; //(session ref count) 경계에만 세우고 뒤에 다른 변수 들어올 수 있음.
	int release_flag;
	alignas(64) int pend_count; // CancelIO 타이밍 잡기
	int disconnect;
	alignas(64) int leave_flag;
	alignas(64) int send_flag;
	alignas(64) int send_packet_cnt;  // Send에 넣은 Packet 객체 삭제에 필요
	alignas(64) DWORD ref_time;
	ULONGLONG send_packet_time;

	wchar_t ip[16];
	unsigned short port;
	CRITICAL_SECTION session_cs;

#ifdef TRACE_SESSION
	MiniTracer pending_tracer;
#endif

	// 락프리 스택쓰기?
#ifdef AUTO_PACKET
	PacketPtr temp_packet[200];
#else
	CPacket* temp_packet[200];
#endif

};
