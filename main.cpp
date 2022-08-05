
#pragma comment(lib, "ws2_32")
#pragma comment(lib, "winmm")
#include <WinSock2.h>
#include <WS2tcpip.h>

#include <Windows.h>

#include <stdio.h>
#include <conio.h>

#include "ChatServer.h"
#include "TextParser.h"



#define SERVERPORT 6000

ChatServer server;

int main()
{
	
	timeBeginPeriod(1);

	char ip[16];
	short port;
	int worker;
	int max_worker;
	int max_user;
	int max_session;
	unsigned char packet_code;
	unsigned char packet_key;

	TextParser parser;
	if (!parser.LoadFile("Config.ini")) return 1;

	parser.GetStringValue("ServerBindIP", ip, 16);
	parser.GetValue("ServerBindPort", (int*)&port);
	parser.GetValue("IOCPWorkerThread", &worker);
	parser.GetValue("IOCPActiveThread", &max_worker);
	parser.GetValue("MaxUser", &max_user);
	parser.GetValue("MaxSession", &max_session);
	parser.GetValue("PacketCode", (int*)&packet_code);
	parser.GetValue("PacketKey", (int*)&packet_key);
	// packet code, fix_key



	wchar_t wip[16];

	MultiByteToWideChar(CP_ACP, 0, ip, 16, wip, 16);

	server.Start(wip, port, worker, max_worker, max_session, max_user, packet_key, packet_code);

	DWORD oldTick = timeGetTime();
	while (1)
	{
		if (_kbhit())
		{
			wchar_t input;
			input = _getwch();

			if (input == L'q' || input == L'Q')
			{

				server.Stop();

				break;
			}
		}

		DWORD term = timeGetTime() - oldTick;
		Sleep(1000 - term);
		oldTick = timeGetTime();

		server.Show();

	}

	timeEndPeriod(1);

	wprintf(L"Fine Closing\n");
	Sleep(5000);


	return 0;
}