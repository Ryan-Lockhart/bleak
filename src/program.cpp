#include <necrowarp/game.hpp>

int main(int argc, char* argv[]) {
	using namespace necrowarp;

	const int exited_successfully{ Game::run() };

	if (!exited_successfully) {
		message_log.flush_to_file();			
		error_log.flush_to_file();
	}

	return exited_successfully;
}
