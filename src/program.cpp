#include <necrowarp/game.hpp>

int main(int argc, char* argv[]) {
	using namespace necrowarp;

	const int exit_code{ Game::run() };

	if (exit_code == EXIT_FAILURE) {
#if defined(BLEAK_DEBUG)
		message_log.flush_to_console(std::cout);
		error_log.flush_to_console(std::cerr);
#elif defined(BLEAK_RELEASE)
		message_log.flush_to_file();
		error_log.flush_to_file();
#endif
	}

	return exit_code;
}
