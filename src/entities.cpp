#include "../include/gameplay.hpp"

Entities::Entities(int coord_x, int coord_y, entity_types entity)
{
    // load coordinates
    coords.x = coord_x;
    coords.y = coord_y;

    // load entity
    entity_type = entity;
}

point Entities:: get_coords(void) const
{
    return coords;
}

void Entities::set_coords(point p)
{
    this->coords=p;
}

void Entities:: print_info()
{
    std::cout << coords.x << " " << coords.y << std::endl;
}

Human::Human(int human_x, int human_y, entity_types entity): Entities(human_x, human_y, entity)
{
    health = (rand() % 3)+1;  // arbitary - range [1, 4]
    original_health = health;
    defense = rand() % 3;
    attack = rand() % 4;
    num_bandages = rand() % 3;
}

void Human::reset_health(void)
{
    health = original_health;
}

void Human::increase_damage(void)
{
    this->attack++;
}

bool Human::is_full_health(void) const
{
    return (original_health == health);
}

void Human::increase_health(void)
{
    health++;
}

bool Human::attacked(int off)
{
    int diff = off-defense;
    if(diff < 0)
        return false;

    else
    {
        diff++;
        health=health-diff;
    }
    return health<=0;
}

bool Human::has_bandages(void) const
{
    return num_bandages > 0;
}

int Human::get_attack(void) const
{
    return attack;
}

void Human::use_bandage(void)
{
    num_bandages--;
}

// -AVATAR-
Avatar::Avatar(int avatar_x, int avatar_y): Entities(avatar_x, avatar_y, AVATAR)
{
    num_of_potions = 1;  // avatar begins with one potion
}

bool Avatar::can_use_potion() const
{
    return num_of_potions > 0;
}

void Avatar::use_potion()
{
    this->num_of_potions--;
}

int Avatar::potion_num() const
{
    return num_of_potions;
}

void Avatar::pick_up_potion()
{
    num_of_potions++;
}

// -WEREWOLVES-
Werewolf::Werewolf(int werewolf_x, int werewolf_y): Human(werewolf_x, werewolf_y, WEREWOLVES) {}

// -VAMPIRES-
Vampire::Vampire(int vampire_x = 0, int vampire_y = 0): Human(vampire_x, vampire_y, VAMPIRES) {}

bool wolf_attack(Werewolf* werewolf, Vampire* vampire)
{
    if (werewolf->attack >= vampire->attack)
        if (vampire->attacked(werewolf->attack))
            return true;

    return false;
}

bool vampire_attack(Werewolf* werewolf, Vampire* vampire)
{
    if (vampire->attack >= werewolf->attack)
        if (werewolf->attacked(vampire->attack))
            return true;

    return false;
}

void heal_vamp(Vampire* vamp, Vampire* vampire_to_heal)
{
    vampire_to_heal->increase_health();
    vamp->use_bandage();
}

void heal_wolf(Werewolf* wolf, Werewolf* were_to_heal)
{
    were_to_heal->increase_health();
    wolf->use_bandage();
}
