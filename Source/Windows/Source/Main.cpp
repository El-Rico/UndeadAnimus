#include <iostream>
#include <GitVersion.hpp>

int main( int p_Argc, char **p_ppArgv )
{
	std::cout << "Undead Animus" << std::endl;
	std::cout << "[Revision:   " << GIT_COMMITHASH << "]" << std::endl;
	std::cout << "[Build date: " << GIT_COMMITTERDATE << "]" << std::endl;
	return 0;
}

