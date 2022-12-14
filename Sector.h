#pragma once
#include <list>
using std::list;

#include "User.h"

#define SECTOR_MAX_X 50
#define SECTOR_MAX_Y 50


struct SectorPos
{
	DWORD x;
	DWORD y;
};

struct SectorAround
{
	int count;
	SectorPos around[9];

};

void Sector_AddUser(User* user);

void Sector_RemoveUser(User* user);

void GetSectorAround(int sector_x, int sector_y, SectorAround* sector_around);


extern list<User*> g_SectorList[SECTOR_MAX_Y][SECTOR_MAX_X];