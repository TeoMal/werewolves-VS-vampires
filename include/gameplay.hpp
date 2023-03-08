#pragma once  // include at most once

#include "typedefs.hpp"
#include "entities.hpp"

void initialize_colors();

// gameState contains all the info needed to represent the current game
class gameState
{
    private:
        point potion_coords;

        point map_coords;  // the size of the map

        int num_of_vampires, num_of_werewolves;  // the number of enemy entities

        entity_types team;  // the team the avatar supports (WEREWOLVES/ VAMPIRES)

        entity_types** map;  // the map containing all the objects

        std::vector <Werewolf> wolves;  // vector containing the werewolves

        std::vector <Vampire> vamps;  // vector containing the vampires

        Avatar* game_avatar;  // the player

        time_types time_type;  // the time (day/night) - changes every [10 - 13] seconds

        point* directional_vector;  // utility array containing used to create new coords

        // gets random point
        point getRandomPoint();

        // returns true if the coordinates do not go out of bound
        bool are_coords_valid(point a);

        // heal players
        void werewolves_healing();
        void vampires_healing();

        // fights
        void werewolves_fights(WINDOW *);
        void vampires_fights(WINDOW *);

        // shuffles entities
        void shuffle_vamps(WINDOW *);
        void shuffle_were(WINDOW *);

        // helper intializer
        void create_directional_vector();

        bool turns;
    public:
        // constructor
        gameState(int x, int y);

        // destructor
        ~gameState();

        void set_team(entity_types t);
        void Healing();
        void won(WINDOW *);
        void Fights(WINDOW *);

        // draws the board in its current form
        void draw_board(WINDOW *,int,int,int,int);

        // draw stats
        void draw_pause(WINDOW *);

        // moves avatar
        void move_avatar(move_types move, WINDOW *win);

        // alternates between night and day
        void alternate_day_night(void);

        void Shuffle(WINDOW *win);

        // returns true/ false depending on whether at least one team has no entities or not
        bool isEndState(void);

        // use potion
        void use_potion();
};
