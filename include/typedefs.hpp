#pragma once  // include at most once
#include <ncurses.h>
#include <iostream>
#include <vector>

typedef enum
{
    WEREWOLVES, VAMPIRES, AVATAR, LAKE, TREE, POTION, NONE
}
entity_types;

// https://i.ytimg.com/vi/0vuCP0C_kms/maxresdefault.jpg
typedef enum
{
    NORTH, NORTH_EAST, EAST, SOUTH_EAST, SOUTH, SOUTH_WEST, WEST, NORTH_WEST
}
move_types;

typedef enum
{
    DAY, NIGHT
}
time_types;

struct point
{
    int x, y;
    
    point() : x(0), y(0) {}
    point(int x, int y) : x(x), y(y) {}
    point operator+(const point& other) const {
        return point(x + other.x, y + other.y);
    }
    
    point operator-(const point& other) const {
        return point(x - other.x, y - other.y);
    }
};

#define AANG_S 'A'
#define WEREWOLF_S 'W'
#define VAMPIRE_S 'V'
#define LAKE_S 'L'
#define TREE_S 'T'
#define POTION_S 'O'
