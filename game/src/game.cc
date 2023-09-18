#include "game.hh"

#include <common/file_system/file.hh>

Game::Game(Platform &platform)
	: platform_(platform)
{ }

bool Game::init()
{
	Platform &platform = platform_;
	File file(platform.file_system);
	if (file.init("Makefile", "rb")) {
		std::size_t size;
		if (file.try_get_size(&size)) {
			platform.logger.log(LogLevel::INFO, "%zu", size);
			return true;
		}
	}
	return false;
}

void Game::tick()
{ }
