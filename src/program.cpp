#include <necrowarp/game.hpp>

int main(int argc, char* argv[]) {
	using namespace necrowarp;

	const int exit_code{ Game::run() };

	if (exit_code == EXIT_FAILURE) {
		message_log.flush_to_file();
		error_log.flush_to_file();
	}

	return exit_code;
}
