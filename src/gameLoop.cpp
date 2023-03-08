#include <iostream>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include"../include/gameplay.hpp"

bool game_loop(int lines, int cols,const char *team)
{
    srand((unsigned)time(NULL));
    
    initscr();
    clear();
    noecho();
    curs_set(0);
    cbreak();
    initialize_colors();
    int TICK_SPEED = 50;
    WINDOW *win;
    int ymax,xmax;
    getmaxyx(stdscr,ymax,xmax);

    // coordinates with this size can not fit the terminal
    if (lines+2>xmax || cols+2>ymax)
    {
        perror("Error! Map requested is too big to fit in the terminal!");
        echo();
        curs_set(1);
        nocbreak();
        endwin();
        system("reset");
        return false;
    }

    int c, r;
    int framecounter = 0;
    int daycounter=0;
    win = newwin(cols+2, lines+2, (ymax/2)-cols/2, (xmax/2)-lines/2);
    keypad(win, TRUE);
    nodelay(win, TRUE);
    
    // create game state
    gameState game_state(lines, cols);
    if(strncasecmp(team,"werewolves",12)==0)
       game_state.set_team(WEREWOLVES);     
    else if(strncasecmp(team,"vampires",10)==0)
        game_state.set_team(VAMPIRES);
    else
    {
        perror("Error at selecting team");
        
        exit(4);
    }
    // random change between night & day
    int tick_sp = rand() % 30;

    while (!game_state.isEndState())
    {
        usleep(1000);
        c = wgetch(win);
        switch (c)
        {
        // Move
        case KEY_UP:
        case 'w':
        case 'W':
            game_state.move_avatar(NORTH,win);
            game_state.draw_board(win,xmax,ymax,lines,cols);
            break;
        case KEY_DOWN:
        case 's':
        case 'S':
            game_state.move_avatar(SOUTH,win);
            game_state.draw_board(win,xmax,ymax,lines,cols);
            break;
        case KEY_RIGHT:
        case 'd':
        case 'D':
            game_state.move_avatar(EAST,win);
            game_state.draw_board(win,xmax,ymax,lines,cols);
            break;
        case KEY_LEFT:
        case 'a':
        case 'A':
            game_state.move_avatar(WEST,win);
            game_state.draw_board(win,xmax,ymax,lines,cols);
            break;
        // Pause
        case 'P':
        case 'p':
            wclear(win);
            while(true)
            {
                game_state.draw_pause(win);
                wrefresh(win);
                int input=wgetch(win);
                if (input=='p' || input=='P')
                    break;
                else if (input=='q' || input=='Q')
                {
                    c=input;
                    break;
                }
            }
            wclear(win);
            break;
        // Use Potion
        case 'F':
        case 'f':
            game_state.use_potion();
            break;
        default:
            break;
        }
        // Quit - break out of game loop and exit
        if (c == 'q' || c == 'Q')
            break;
        // Framecounter so that the pieces move every TICK_SPEED loops
        if(framecounter==TICK_SPEED)
        {
            daycounter++;
            game_state.Shuffle(win);
            game_state.Healing();
            game_state.Fights(win);
            framecounter=0;
            game_state.draw_board(win,xmax,ymax,lines,cols);
        }
        else
            framecounter++;
        // alternate between day/night
        if(daycounter==tick_sp)
        {
            game_state.alternate_day_night();
            game_state.draw_board(win,xmax,ymax,lines,cols);
            daycounter=0;
            tick_sp = rand() % 30;
        }
        refresh();
    }
    delwin(win);
    clear();
    //If the game ended and wasn't interrupted from user print Winning team
    if(game_state.isEndState())
        game_state.won(stdscr);
    endwin();
    return true;
}

bool is_number(char* num)
{
    for (unsigned int i = 0; num[i] < '\0'; i++)
        if (!isdigit(num[i]))
            return false;
        
    return true;
}
