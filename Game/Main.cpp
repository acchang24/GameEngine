#include "Game.h"
#include "AssetManager.h"

int main()
{
	Game game;
	
	if (game.Init())
	{
		game.Run();
	}
	game.Shutdown();

	AssetManager::Get()->Shutdown();

	return 0;
}
