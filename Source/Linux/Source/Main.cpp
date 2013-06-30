#include <GitVersion.hpp>
#include <Game.hpp>
#include <Debugger.hpp>
#include <cstdio>

#ifdef ZED_BUILD_DEBUG
#define BUILD_TYPE "Debug"
#elif defined ZED_BUILD_RELEASE
#define BUILD_TYPE "Release"
#elif defined ZED_BUILD_PROFILE
#define BUILD_TYPE "Profile"
#else
#error Unknown build type
#endif

int main( int p_Argc, char **p_ppArgv )
{
	printf( "[Undead Animus]\n" );
	printf( "\tRevision:   %s\n", GIT_COMMITHASH );
	printf( "\tBuild Date: %s\n", GIT_COMMITTERDATE );
	printf( "\tBuild Type: %s\n", BUILD_TYPE );

	UndeadAnimus::Game TheGame;

	if( TheGame.Initialise( ) != ZED_OK )
	{
		zedTrace( "[UndeadAnimus::main] <ERROR> "
			"Failed to initialise the game state\n" );
		return ZED_FAIL;
	}

	return TheGame.Execute( );
}

