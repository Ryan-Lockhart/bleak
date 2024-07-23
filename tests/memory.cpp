#include <bleak/memory.hpp>

#include <cassert>
#include <cstdlib>

int main(int argc, char* argv[]) {
    bleak::cenotaph<int> memory_bank{ 0 };

    memory_bank.memorialize(1);
    memory_bank.memorialize(2);
    memory_bank.memorialize(3);
    memory_bank.memorialize(4);
    memory_bank.memorialize(5, 6, 7);

    
    return EXIT_SUCCESS;
}