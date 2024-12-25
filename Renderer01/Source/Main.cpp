#include "Game.h"

int main(int argc, char* args[])
{
	Game game;

	if (game.Init())
	{
		game.Run();
	}
	game.Shutdown();

	return 0;
}
