#pragma once

#include "typedefs.hpp"

// General class for all entities
class Entities
{
    protected:
        point coords;
        entity_types entity_type;
        
    public:
        // constructor
        Entities(int coord_x, int coord_y, entity_types entity);

        // returns coordinates of the object
        point get_coords(void) const;

        // sets coordinates
        void set_coords(point);

        // debug
        void print_info();
};

// -AVATAR-
class Avatar: public Entities
{
    private:
        // the current number of potions Avatar has available (0/1/2)
        int num_of_potions;
    public:
        // constructor
        Avatar(int avatar_x, int avatar_y);

        // number of potions remaining
        bool can_use_potion() const;

        // use potion
        void use_potion();

        // pick up potion
        void pick_up_potion();

        // returns the current number of potions
        int potion_num() const ;
};

// General class for warewolfs and vampires
class Human: public Entities
{
    protected:
        // character stats
        int attack, defense, health, original_health, num_bandages;

        // increases health
        void increase_health(void);

        // use bandage
        void use_bandage(void);

    public:
        // constructor
        Human(int human_x, int human_y, entity_types entity);

        // is full_health
        bool is_full_health(void) const;

        // reset its health back to its orignal one
        void reset_health(void);
        
        // increases damage
        void increase_damage(void);

        // attack
        int get_attack(void) const;

        // is attacked
        bool attacked(int);

        // returns true/ false depending on whether the human has bandages or not
        bool has_bandages(void) const;
};

class Vampire;

// -WEREWOLVES-
class Werewolf: public Human
{
    public:
        // constructor
        Werewolf(int werewolf_x, int werewolf_y);

        friend bool wolf_attack(Werewolf* werewold, Vampire* vampire);
        friend bool vampire_attack(Werewolf* werewold, Vampire* vampire);

        friend void heal_wolf(Werewolf* wolf, Werewolf* were_to_heal);
};

// -VAMPIRES-
class Vampire: public Human
{
    public:
        // constructor
        Vampire(int vampire_x, int vampire_y);
    
        friend bool wolf_attack(Werewolf* werewold, Vampire* vampire);
        friend bool vampire_attack(Werewolf* werewold, Vampire* vampire);

        friend void heal_vamp(Vampire* wolf, Vampire* were_to_heal);
};
