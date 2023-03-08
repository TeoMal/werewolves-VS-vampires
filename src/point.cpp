#include"../include/gameplay.hpp"
#include"../include/typedefs.hpp"

point gameState::getRandomPoint()
{
    point a;
    do
    {
        a.x = rand() % (map_coords.x - 1);
        a.y = rand() % map_coords.y;
    }
    // keep generating random coordinates until an empty square is found
    while (map[a.x][a.y] != NONE);

    return a;
}

bool gameState::are_coords_valid(point a)
{
    if (a.x < 0 || a.y < 0 || a.x >= map_coords.x || a.y >= map_coords.y || map[a.x][a.y] != NONE)
        return false;
    
    return true;
}
