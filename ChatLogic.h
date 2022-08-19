#pragma once
#include <unordered_map>
using std::unordered_map;

#include "CPacket.h"
#include "JOB.h"
#include "User.h"
#include "Tracer.h"


 


extern MemoryPoolTls<JOB> g_JobPool;
extern LockFreeQueue<JOB*> g_JobQueue;
extern unordered_map<SS_ID, User*> g_UserMap;
extern unsigned int g_connect_cnt;
extern unsigned int g_login_cnt;
extern ULONG g_job_proc;

extern Tracer g_Tracer;


unsigned __stdcall SingleUpdate(void* param);

bool FindUser(SS_ID s_id, User** user);
void CreateUser(SS_ID s_id);
void DeleteUser(SS_ID s_id);

void SendMessageUni(CPacket* packet, SS_ID sid);
void SendMessageSector(CPacket* packet, int sector_x, int sector_y);
void SendMessageAround(CPacket* packet, SS_ID sid);