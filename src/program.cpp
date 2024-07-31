#include <necrowarp/game.hpp>

#define main SDL_main

int main(int argc, char* argv[]) {
	using namespace necrowarp;

	return Game::run();
}
