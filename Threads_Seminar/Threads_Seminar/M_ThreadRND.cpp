#include <iostream>
#include <Windows.h>



//lokalni promene jsou pro thready neviditelne 
// (vidi jen ty sve)


int pintRND_number;

CRITICAL_SECTION pcsCriticalSection01;

DWORD WINAPI RND_generator(LPVOID lpParm)
{

	while (true)
	{
		EnterCriticalSection(&pcsCriticalSection01);
		//tread prestane mit pridelovany strojovi cas do te doby dokud neni kriticka sekce volna

		//vzdycky ostetrit chyby
		//osetrit opusteni funcke
		pintRND_number++;
		if (pintRND_number > 6) pintRND_number = 1;
	
		LeaveCriticalSection(&pcsCriticalSection01);
	}

	return 0;
}


DWORD WINAPI RND_reader(LPVOID lpParm)
{
	for (int i = 0; i < 20; i++)
	{
		EnterCriticalSection(&pcsCriticalSection01);

		printf("%i, \n", pintRND_number);
		LeaveCriticalSection(&pcsCriticalSection01);
		
		Sleep(200);
	}

	return 0;
}


int main()
{
	DWORD ldwThread1Id, ldwThread2Id;
	HANDLE lhThread2d;

	//init promene
	pintRND_number = 1;

	InitializeCriticalSection(&pcsCriticalSection01);
	//spusteni RND threadu
	CreateThread(NULL,0,RND_generator, NULL,0, &ldwThread1Id);

	lhThread2d = CreateThread(NULL,0,RND_reader, NULL,0, &ldwThread2Id);
	
	// cekani az cteci thread skonci
	WaitForSingleObject(lhThread2d, 2000);
	std::cout << "Hi\n";
	
}
