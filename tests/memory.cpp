#include <bleak/memory.hpp>

#include <cassert>
#include <cstdlib>

int main(int argc, char* argv[]) {
    using namespace bleak;

    cenotaph<i32> memory_bank{};
    assert(memory_bank.is_empty());

    
    return EXIT_SUCCESS;
}