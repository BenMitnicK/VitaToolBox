#include "../vitaident.h"

int fileExists(const char* path)
{
	SceUID dir = sceIoOpen(path, SCE_O_RDONLY, 0777);
	if (dir >= 0)
	{
		sceIoClose(dir);
		return 1;
	}
	else
	{
		return 0;
	}
}

int dirExists(const char* path)
{
	SceUID dir = sceIoDopen(path);
	if (dir >= 0)
	{
		sceIoDclose(dir);
		return 1;
	}
	else 
	{
		return 0;
	}
}

SceOff getPartitionInfo(int storage, const char * partition)
{
	SceOff maxSize = 0;
	SceOff freeSize = 0;

	SceIoStat stat;
	memset(&stat, 0, sizeof(SceIoStat));
	
	if (sceIoGetstat(partition, &stat) >= 0) 
	{		
		SceIoDevInfo info;
		memset(&info, 0, sizeof(SceIoDevInfo));
		int res = sceIoDevctl(partition, 0x3001, 0, 0, &info, sizeof(SceIoDevInfo));
		if (res >= 0) 
		{
			freeSize = info.free_size;
			maxSize = info.max_size;
		}
	}
	
	if (storage == 0)
		return freeSize;
	else
		return maxSize;
}

char * getVitaModel()
{	
	SceOff maxSize = getPartitionInfo(1, "ur0:");
	
	static char string[16];
	getSizeString(string, maxSize);
	
	if (strcmp(string, "1.50 GB") == 0)
		return "2000";
	else 
		return "1000";
}
