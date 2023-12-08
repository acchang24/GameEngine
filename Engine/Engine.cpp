#include "Engine.h"
#include <iostream>

Engine::Engine(int test): test(test)
{
}

Engine::~Engine()
{

}

void Engine::Hello()
{
	std::cout << "Hello Engine " << test << std::endl;
}
