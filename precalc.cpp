#include <iostream>

int main(int argc, char* argv[]) {
    
    for (int rank = 8; rank >= 1; rank--){
        for (int file = 'a'; file <= 'a'+7; file++){
            std::cout << (char)file << rank << ',';
        }
        std::cout << std::endl;
    }

    

    return 0;
}