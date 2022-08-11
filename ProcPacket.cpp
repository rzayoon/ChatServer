#include <unordered_map>
using std::unordered_map;

#include "ChatServer.h"
#include "ChatLogic.h"
#include "ProcPacket.h"
#include "MakePacket.h"
#include "Sector.h"
#include "CommonProtocol.h"


bool ProcChatLogin(User* user, CPacket* packet)
{
	__int64 account_no;
	wchar_t id[MAX_ID_SIZE];
	wchar_t nick[MAX_NICK_SIZE];

	(*packet) >> account_no;
	(*packet).GetData((char*)id, MAX_ID_SIZE * sizeof(wchar_t));
	(*packet).GetData((char*)nick, MAX_NICK_SIZE * sizeof(wchar_t));
	
	if (user->is_login == true)
	{
		// ���� ����id���� �ߺ� �α��� �޽���

		CPacket* send_packet = CPacket::Alloc();
		
		MakeChatLogin(send_packet, 0, account_no);

		SendMessageUni(send_packet, user->session_id);

		CPacket::Free(send_packet);

		g_server.DisconnectSession(user->session_id);
		

		return false;
	}


	for (auto& iter : g_UserMap)
	{
		User* temp_user = iter.second;
		if (temp_user->account_no == account_no)
		{
			// �ٸ� ���� id���� account no �ߺ� �α���
			CPacket* send_packet = CPacket::Alloc();

			MakeChatLogin(send_packet, 0, account_no);

			SendMessageUni(send_packet, user->session_id);

			CPacket::Free(send_packet);

			g_server.DisconnectSession(temp_user->session_id);

			return false;
		}
	}

	user->is_login = true;
	user->account_no = account_no;
	wcscpy_s(user->id, id);
	wcscpy_s(user->nickname, nick);

	CPacket* send_packet = CPacket::Alloc();

	MakeChatLogin(send_packet, 1, user->account_no);

	SendMessageUni(send_packet, user->session_id);

	CPacket::Free(send_packet);

	return true;


}

bool ProcChatSectorMove(User* user, CPacket* packet)
{
	__int64 account_no;
	WORD sector_x;
	WORD sector_y;

	(*packet) >> account_no >> sector_x >> sector_y;

	if (account_no != user->account_no)
	{
		// ����

		return false;
	}

	if (user->is_in_sector == true)
	{
		Sector_RemoveUser(user);
	}
	user->sector_x = sector_x; 
	user->sector_y = sector_y;

	Sector_AddUser(user);
	user->is_in_sector = true;

	CPacket* send_packet = CPacket::Alloc();

	MakeChatSectorMove(send_packet, user->account_no, user->sector_x, user->sector_y);

	SendMessageUni(send_packet, user->session_id);

	CPacket::Free(send_packet);

	return true;

}
bool ProcChatMessage(User* user, CPacket* packet)
{
	__int64 account_no;
	WORD message_len;

	wchar_t message[MAX_MESSAGE];

	(*packet) >> account_no >> message_len;
	(*packet).GetData((char*)message, message_len);

	CPacket* send_packet = CPacket::Alloc();

	MakeChatMessage(send_packet, account_no, user->id, user->nickname, message_len, message);

	SendMessageAround(send_packet, user->session_id);

	CPacket::Free(send_packet);

	return true;
}