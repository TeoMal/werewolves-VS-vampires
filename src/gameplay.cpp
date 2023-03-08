#include <iostream>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "../include/gameplay.hpp"

#define ABS(x) ((x) < 0 ? -(x) : (x))
using namespace std;
#define move_prob 6

// map scarcity of obstacles
#define OBSTACLE_SCARCITY 20
#define PLAYER_SCARCITY 15

#define GET_NUM_OF_PLAYERS(x, y) (x * y / PLAYER_SCARCITY)

gameState::gameState(int x = 10, int y = 10)
{
    // seed rand using the current time
    srand((unsigned)time(NULL));
    // load map coords
    map_coords.x = x;
    map_coords.y = y;
    turns = true;
    point random_point;
    int num_of_players = GET_NUM_OF_PLAYERS(x, y);
    num_of_werewolves = num_of_vampires = num_of_players;
    int random_number;
    try
    {
        map = new entity_types *[x];
        if (map == NULL)
            throw "Memory allocation failure";
    } catch (const char *message)  { cerr << message << endl; }

    create_directional_vector();
    
    for (int i = 0; i < map_coords.x; i++)
    {
        try
        {
            map[i] = new entity_types[y];
            if (map[i] == NULL)
                throw "Memory allocation failure";
        }
        catch (const char *message)  { cerr << message << endl; }


        // initialize array
        for (int j = 0; j < map_coords.y; j++)
            map[i][j] = NONE;
    }

    // obstacle generation
    random_number = rand() % (x * y / OBSTACLE_SCARCITY);
    for (int i = 0; i < random_number; i++)
    {
        // get random place for a lake
        random_point = getRandomPoint();

        map[random_point.x][random_point.y] = LAKE;
    }
    random_number = rand() % (x * y / OBSTACLE_SCARCITY);
    for (int i = 0; i < random_number; i++)
    {
        // get random place for a tree
        random_point = getRandomPoint();

        map[random_point.x][random_point.y] = TREE;
    }
    
    // werewolf/vampire generation
    for (int i = 0; i < num_of_players; i++)
    {
        // get random place for a werewolf
        random_point = getRandomPoint();

        wolves.push_back(Werewolf(random_point.x, random_point.y));
        map[random_point.x][random_point.y] = WEREWOLVES;

        // get random place for vampire
        random_point = getRandomPoint();

        vamps.push_back(Vampire(random_point.x, random_point.y));
        map[random_point.x][random_point.y] = VAMPIRES;
    }

    // avatar generation
    random_point = getRandomPoint();
    game_avatar = new Avatar(random_point.x, random_point.y);
    map[random_point.x][random_point.y] = AVATAR;  // place avatar

    // potion generation
    potion_coords = getRandomPoint();
    map[potion_coords.x][potion_coords.y] = POTION;

    // define day time
    time_type = ((rand() % 2) == 0) ? DAY : NIGHT;
}

void gameState::set_team(entity_types t)  { team=t; }

void gameState:: create_directional_vector()
{
    try
    {
        directional_vector = new point[8];
        if (directional_vector == NULL)
            throw "Memory allocation failure";
    } catch (const char *message)  { cerr << message << endl; }
    
    /*

    Directional vector contains the difference between distance of the neighbour coordinates
    A visualization:

    -------------------------------
    |(i-1,j-1)| (i,j-1) |(i+1,j-1)|
    |   NW    |    N    |   NE    |
    |-----------------------------|
    | (i-1,j) |  (i,j)  | (i+1,j) |
    |    W    |         |   E     |
    |-----------------------------|
    |(i-1,j+1)| (i,j+1) |(i+1,j+1)|
    |   SW    |    S    |   SE    |
    -------------------------------

    */
    directional_vector[NORTH].x = 0;       directional_vector[NORTH].y = -1;
    directional_vector[NORTH_EAST].x = 1;  directional_vector[NORTH_EAST].y = -1;
    directional_vector[EAST].x = 1;        directional_vector[EAST].y = 0;
    directional_vector[SOUTH_EAST].x = 1;  directional_vector[SOUTH_EAST].y = 1;
    directional_vector[SOUTH].x = 0;       directional_vector[SOUTH].y = 1;
    directional_vector[SOUTH_WEST].x = -1; directional_vector[SOUTH_WEST].y = 1;
    directional_vector[WEST].x = -1;       directional_vector[WEST].y = 0;
    directional_vector[NORTH_WEST].x = -1; directional_vector[NORTH_WEST].y = -1;
}

void gameState::move_avatar(move_types move, WINDOW *win)
{
    point old_pos = game_avatar->get_coords();
    point new_pos=old_pos+directional_vector[move];
    if (!(new_pos.x<0 || new_pos.y<0 || new_pos.x>=map_coords.x || new_pos.y>=map_coords.y))
    {
        if (this->map[new_pos.x][new_pos.y] == NONE) // move to an empty title
        {
            mvwaddch(win, old_pos.y + 1, old_pos.x + 1, ' ');
            this->map[old_pos.x][old_pos.y] = NONE;
            this->map[new_pos.x][new_pos.y] = AVATAR;
            this->game_avatar->set_coords(new_pos);
        }
        else if (this->map[new_pos.x][new_pos.y] == POTION) // pick up potion
        {
            mvwaddch(win, old_pos.y + 1, old_pos.x + 1, ' ');
            this->map[old_pos.x][old_pos.y] = NONE;
            this->map[new_pos.x][new_pos.y] = AVATAR;
            this->game_avatar->set_coords(new_pos);
            game_avatar->pick_up_potion();
        }
    }
}

void gameState::draw_pause(WINDOW *win)
{
    mvwprintw(win, (map_coords.y + 2) / 2, (map_coords.x + 2) / 2, "Paused\nWerewolves: %d\nVampires: %d\nPotions: %d\n", 
    num_of_werewolves, num_of_vampires, game_avatar->potion_num());
}

void gameState::vampires_fights(WINDOW *win)
{
    for (auto vamp = vamps.begin(); vamp != vamps.end(); vamp++)
    {
        point vamp_coords = vamp->get_coords();
        int i=0;
        for (auto wolf = wolves.begin(); wolf != wolves.end(); wolf++,i++)
        {
            point wolf_coords = wolf->get_coords();

            int x__dist = vamp_coords.x- wolf_coords.x;
            int y__dist = vamp_coords.y - wolf_coords.y;
            if (ABS(x__dist) >= 2 || ABS(y__dist) >= 2)
                continue;

            Vampire V = *vamp; Werewolf W = *wolf;

            if (vampire_attack(&W, &V))
            {
                map[wolf_coords.x][wolf_coords.y]=NONE;
                mvwaddch(win,wolf_coords.y+1,wolf_coords.x+1,' ');
                wolves.erase(wolves.begin()+i);
                num_of_werewolves--;
                break;
            }
            else
            {
                point new_vamp_coords;
                new_vamp_coords.x=x__dist+vamp_coords.x;
                new_vamp_coords.y=y__dist+vamp_coords.y;

                if (!are_coords_valid(new_vamp_coords)) continue;

                vamp->set_coords(new_vamp_coords);
                map[vamp_coords.x][vamp_coords.y]=NONE;
                mvwaddch(win,vamp_coords.y+1,vamp_coords.x+1,' ');
                map[new_vamp_coords.x][new_vamp_coords.y]=VAMPIRES;
                break;
            }
        }
    }
}

void gameState::werewolves_fights(WINDOW *win)
{
    for (auto wolf = wolves.begin(); wolf != wolves.end(); wolf++)
    {
        point wolf_coords = wolf->get_coords();
        int i=0;
        for (auto vamp = vamps.begin(); vamp != vamps.end(); vamp++, i++)
        {
            point vamp_coords = vamp->get_coords();

            int x__dist = (wolf_coords.x- vamp_coords.x);
            int y__dist = (wolf_coords.y - vamp_coords.y);
            
            // not neighbours
            if (ABS(x__dist) > 1 || ABS(y__dist) > 1)
                continue;

            Vampire V = *vamp; Werewolf W = *wolf;

            if (wolf_attack(&W, &V))
            {
                map[vamp_coords.x][vamp_coords.y]=NONE;
                mvwaddch(win,vamp_coords.y+1,vamp_coords.x+1,' ');
                vamps.erase(vamps.begin()+i);
                num_of_vampires--;
                break;
            }
            else
            {
                point new_wolf_coords;
                new_wolf_coords.x=wolf_coords.x+x__dist;
                new_wolf_coords.y=wolf_coords.y+y__dist;

                if (!are_coords_valid(new_wolf_coords)) continue;
                    
                wolf->set_coords(new_wolf_coords);
                map[wolf_coords.x][wolf_coords.y]=NONE;
                mvwaddch(win,wolf_coords.y+1,wolf_coords.x+1,' ');
                map[new_wolf_coords.x][new_wolf_coords.y]=WEREWOLVES;
                break;
            }
        }
    }
}

void gameState::Fights(WINDOW *win)
{
    if(turns)
    {
        vampires_fights(win);
        werewolves_fights(win);
    }
    else
    {
        werewolves_fights(win);
        vampires_fights(win);
    }
    turns=!turns; 
}

void gameState::Shuffle(WINDOW *win)
{
    shuffle_were(win);
    shuffle_vamps(win);
}

void gameState::shuffle_were(WINDOW *win)
{
    for (auto woof = wolves.begin(); woof != wolves.end(); woof++)
    {
        if(rand() % 10 >= move_prob)
            continue;

        // werewolf has half as many moves - can not move diagonally (check move_types)
        int rand_move = 2 * (rand() % 4);

        point new_coords, curr = woof->get_coords();
        new_coords=directional_vector[rand_move]+curr;

        if (!are_coords_valid(new_coords)) continue;

        map[curr.x][curr.y] = NONE;
        map[new_coords.x][new_coords.y] = WEREWOLVES;
        mvwaddch(win, curr.y + 1, curr.x + 1, ' ');
        woof->set_coords(new_coords);
    }
}

void gameState::shuffle_vamps(WINDOW *win)
{
    for (auto vamp = vamps.begin(); vamp != vamps.end(); vamp++)
    {
        if(rand() % 10 >= move_prob)
            continue;

        int rand_move = rand() % 8;

        point new_coords, curr = vamp->get_coords();
        new_coords=directional_vector[rand_move]+curr;

        // invalid coords
        if (!are_coords_valid(new_coords)) continue;

        map[curr.x][curr.y] = NONE;
        map[new_coords.x][new_coords.y] = VAMPIRES;
        mvwaddch(win, curr.y + 1, curr.x + 1, ' ');
        vamp->set_coords(new_coords);
    }
}

void gameState::werewolves_healing()
{
    // check healing for werewolves
    for (auto wolf = wolves.begin(), end = wolves.end(); wolf != end; wolf++)
    {
        if (!(wolf->has_bandages())) continue;
            
        point wolf_coords = wolf->get_coords();
        for (auto neighbor_wolf = wolves.begin(), end = wolves.end(); neighbor_wolf != end; neighbor_wolf++)
        {
            if (neighbor_wolf == wolf || neighbor_wolf->is_full_health())
                continue;
            
            point neighbor_wolf_coords = neighbor_wolf->get_coords();

            int x_dist = ABS(neighbor_wolf_coords.x - wolf_coords.x);
            int y_dist = ABS(neighbor_wolf_coords.y - wolf_coords.y);

            if (x_dist < 2 && y_dist < 2)
            {
                // there is a 50% chance it will heal
                if (rand() % 2) continue;
                
                Werewolf A = *wolf, B = *neighbor_wolf;
                heal_wolf(&A, &B);
            }
        }
    }
}

void gameState::vampires_healing()
{
    // check healing for vampires
    for (auto vamp = vamps.begin(), end = vamps.end(); vamp != end; vamp++)
    {
        if (!(vamp->has_bandages())) continue;
        
        point vamp_coords = vamp->get_coords();
        
        for (auto neighbor_vamp = vamps.begin(), end = vamps.end(); neighbor_vamp != end; neighbor_vamp++)
        {
            if (neighbor_vamp == vamp || neighbor_vamp->is_full_health())
                continue;

            point neighbor_vamp_coords = neighbor_vamp->get_coords();

            int x_dist = ABS(neighbor_vamp_coords.x - vamp_coords.x);
            int y_dist = ABS(neighbor_vamp_coords.y - vamp_coords.y);

            if (x_dist < 2 && y_dist < 2)
            {
                // there is a 50% chance it will heal
                if (rand() % 2) continue;

                Vampire A = *vamp, B = *neighbor_vamp;
                heal_vamp(&A, &B);
            }
        }
    }
}

void gameState::Healing()
{
    werewolves_healing();
    vampires_healing();
}

void gameState:: won(WINDOW *win)
{
    wclear(win);
    nodelay(win,TRUE);
    int x,y;
    getmaxyx(win,y,x);
    if (num_of_vampires==0)
        wattron(win,COLOR_PAIR(1));
    else
        wattron(win,COLOR_PAIR(2));
    while (true)
    {
        int c=wgetch(win);
        if (c=='q' || c=='Q')
            break;
        if (num_of_vampires==0)
            mvwprintw(win,y/2, (x/ 2)-10, "Werewolves Won!(Press Q to Exit)");
        else
            mvwprintw(win,y/2, (x/2)-10, "Vampires Won!(Press Q to Exit)");

    }
    if (num_of_vampires==0)
        wattroff(win,COLOR_PAIR(1));
    else
        wattroff(win,COLOR_PAIR(2));
}

void gameState::draw_board(WINDOW *win,int xbig,int ybig,int xlil,int ylil)
{
    box(win, 0, 0);
    if (time_type==DAY)
    {
        mvwprintw(stdscr,(ybig/2)-(ylil/2)-2,(xbig/2), "     ");
        wattron(stdscr,COLOR_PAIR(6));
        mvwprintw(stdscr,(ybig/2)-(ylil/2)-2,(xbig/2), "Day");
        wattroff(stdscr,COLOR_PAIR(6));
    }
    else
    {
        mvwprintw(stdscr,(ybig/2)-(ylil/2)-2,(xbig/2), "   ");
        wattron(stdscr,COLOR_PAIR(5));
        mvwprintw(stdscr,(ybig/2)-(ylil/2)-2,(xbig/2), "Night");
        wattroff(stdscr,COLOR_PAIR(5));
    }
    if (team==VAMPIRES)
    {
        wattron(win,COLOR_PAIR(2));
        mvwprintw(win,0,map_coords.x-10, "Vampires");
        wattroff(win,COLOR_PAIR(2));
    }
    else
    {
        wattron(win,COLOR_PAIR(1));
        mvwprintw(win,0,map_coords.x-10, "Werewolves");
        wattroff(win,COLOR_PAIR(1));
    }

    for (int i = 0; i < map_coords.x; i++)
    {
        for (int j = 0; j < map_coords.y; j++)
        {
            if (map[i][j] == AVATAR)
                if(team==WEREWOLVES)
                    mvwaddch(win, j + 1, i + 1, WEREWOLF_S);
                else
                    mvwaddch(win, j + 1, i + 1, VAMPIRE_S);
            else if (map[i][j] == VAMPIRES)
                mvwaddch(win, j + 1, i + 1, VAMPIRE_S | COLOR_PAIR(2));
            else if (map[i][j] == WEREWOLVES)
                mvwaddch(win, j + 1, i + 1, WEREWOLF_S | COLOR_PAIR(1));
            else if (map[i][j] == LAKE)
                mvwaddch(win, j + 1, i + 1, LAKE_S | COLOR_PAIR(4));
            else if (map[i][j] == TREE)
                mvwaddch(win, j + 1, i + 1, TREE_S | COLOR_PAIR(3));
            else if (map[i][j] == POTION)
                mvwaddch(win, j + 1, i + 1, POTION_S);
        }
    }
}

void gameState::use_potion()
{
    if (game_avatar->can_use_potion()) // avatar has available potions remaining
    {
        if (time_type == NIGHT && team == VAMPIRES)
        {
            game_avatar->use_potion();

            for (auto vamp = vamps.begin(); vamp != vamps.end(); vamp++){
                vamp->reset_health();
                vamp->increase_damage();
            }
        }
        else if (time_type == DAY && team == WEREWOLVES)
        {
            game_avatar->use_potion();
            for (auto wolf = wolves.begin(); wolf != wolves.end(); wolf++){
                wolf->reset_health();
                wolf->increase_damage();
            }
        }
    }
}

void gameState::alternate_day_night(void)
{
    time_type = (time_type == DAY)? NIGHT: DAY;
}

bool gameState::isEndState(void)
{
    // if all players are killed, the game ends
    return (num_of_vampires == 0 || num_of_werewolves == 0);
}

gameState::~gameState()
{
    // destroy map
    for (int i = 0; i < map_coords.x; i++)
        delete[] map[i];
    delete[] map;

    delete game_avatar;
    delete directional_vector;
}
