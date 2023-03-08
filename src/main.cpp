#include <iostream>
#include"../include/gameLoop.hpp"

int main(int argc, char** argv)
{
    if (argc != 4 || !is_number(argv[1]) || !is_number(argv[2]))
    {
        perror("Error! Usage: ./Wnv <x> <y> <team>");
        system("reset");
        return 1;
    }

    if (!game_loop(atoi(argv[1]), atoi(argv[2]),argv[3]))
        return 2;
    
    return 0;
}
