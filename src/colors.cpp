#include"../include/gameplay.hpp"

void initialize_colors()
{
    start_color();
    init_color(COLOR_YELLOW, 500, 340, 245);    // werewolves
    init_color(COLOR_RED, 925, 95,180);         // vampires
    init_color(COLOR_BLUE, 0,370 , 720);        // lake
    init_color(COLOR_GREEN,62 ,200,40);         // trees
    init_color(COLOR_BLACK,0,0,0);              // black
    init_color(COLOR_MAGENTA,700,75,990);          
    init_color(COLOR_CYAN,956,500,215);
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5,COLOR_MAGENTA,COLOR_BLACK);
    init_pair(6,COLOR_CYAN,COLOR_BLACK);
}
