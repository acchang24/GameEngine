#include "Game.h"

int main()
{
	Game game;
	
	if (game.Init())
	{
		game.Run();
	}
	game.Shutdown();

	return 0;
}
