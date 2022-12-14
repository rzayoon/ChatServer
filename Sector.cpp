

#include "ChatLogic.h"
#include "Sector.h"
#include "User.h"


list<User*> g_SectorList[SECTOR_MAX_Y][SECTOR_MAX_X];


void Sector_AddUser(User* user)
{
	g_SectorList[user->sector_y][user->sector_x].push_back(user);

	return;
}

void Sector_RemoveUser(User* user)
{
	int sector_y = user->sector_y;
	int sector_x = user->sector_x;

	list<User*>& sector = g_SectorList[sector_y][sector_x];

	for (auto iter = sector.begin(); iter != sector.end(); ++iter)
	{
		if ((*iter) == user)
		{
			sector.erase(iter);
			break;
		}


	}

	return;
}

void GetSectorAround(int sector_x, int sector_y, SectorAround* sector_around)
{
	int cnt_x, cnt_y;

	sector_x--;
	sector_y--;

	sector_around->count = 0;

	for (cnt_y = 0; cnt_y < 3; cnt_y++)
	{
		int temp_y = sector_y + cnt_y;
		if (temp_y < 0 || temp_y >= SECTOR_MAX_Y)
			continue;

		for (cnt_x = 0; cnt_x < 3; cnt_x++)
		{
			int temp_x = sector_x + cnt_x;
			if (temp_x < 0 || temp_x >= SECTOR_MAX_X)
				continue;

			sector_around->around[sector_around->count].x = temp_x;
			sector_around->around[sector_around->count].y = temp_y;
			sector_around->count++;
		}
		

	}

	return;

}