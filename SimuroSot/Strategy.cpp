// MyStrategy.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "Strategy.h"
#include "./Core/tester.hpp"
#include "./HAL/comm.h"
#include <math.h>
#include "winDebugger/Client.h"

int invert_field = teamColor == Simulator::BLUE_TEAM ? 0 : 1;
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}



char myMessage[200]; //big enough???
//void Velocity ( Robot *robot, int vl, int vr );
MyStrategy::BeliefState state;
 Tester t(state);
 Kalman kFilter;

  

extern "C" STRATEGY_API void Create ( Environment *env )
{
	Comm::getInstance()->envi=env;
}

extern "C" STRATEGY_API void Destroy ( Environment *env )
{
	// free any user data created in Create ( Environment * )

	// eg. if ( env->userData != NULL ) delete ( MyVariables * ) env->userData;
}


extern "C" STRATEGY_API void Strategy ( Environment *env )
{
	static int flag = 0;
	Comm::getInstance()->envi=env;
	kFilter.addInfo(env,invert_field);
	kFilter.update(state);
	state.update();
	t.run();
}
