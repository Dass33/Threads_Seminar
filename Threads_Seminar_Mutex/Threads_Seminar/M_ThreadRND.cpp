#include <iostream>
#include <Windows.h>



//lokalni promene jsou pro thready neviditelne 
// (vidi jen ty sve)


int pintRND_number;
HANDLE pmMutex01;


DWORD WINAPI RND_generator(LPVOID lpParm)
{
	DWORD ldwRetVal;

	while (true)
	{
		ldwRetVal = WaitForSingleObject(pmMutex01, 2000);

		if (ldwRetVal == WAIT_OBJECT_0)
		{
			__try 
			{
			pintRND_number++;
			if (pintRND_number > 6) pintRND_number = 1;

			}
			__finally
			{
				ReleaseMutex(pmMutex01);
			}
		}

	}

	return 0;
}


DWORD WINAPI RND_reader(LPVOID lpParm)
{
	DWORD ldwRetVal;

	for (int i = 0; i < 20; i++)
	{
		ldwRetVal = WaitForSingleObject(pmMutex01, 2000);

		if (ldwRetVal == WAIT_OBJECT_0)
		{
			__try 
			{
				printf("%i, \n", pintRND_number);
			}
			__finally
			{
				ReleaseMutex(pmMutex01);
			}
		}
			Sleep(200);
	}

	return 0;
}




int main()
{
	DWORD ldwThread1Id, ldwThread2Id;
	HANDLE lhThread2d;

	//vytvoreni mutexu
	pmMutex01 = CreateMutex(NULL, FALSE, NULL);
	//pujcime si mutex od jadra, ktere zajistuje ze dokud ho mamme tak ho nema nikdo jiny
	//pokud mutex nepojmenujeme muzeme sdilet jen pomoci duplicate HANDLE

	if (pmMutex01 == NULL)
	{
		printf("Mutex creation error");
		return 1;
	}

	//init promene
	pintRND_number = 1;

	//spusteni RND threadu
	CreateThread(NULL,0,RND_generator, NULL,0, &ldwThread1Id);

	lhThread2d = CreateThread(NULL,0,RND_reader, NULL,0, &ldwThread2Id);
	
	// cekani az cteci thread skonci
	WaitForSingleObject(lhThread2d, 2000);
	std::cout << "Hi\n";
	
}

