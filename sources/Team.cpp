#include "Team.hpp"
#include <iostream>
#include <string>
#include <cmath>
using namespace ariel;
using namespace std;

Point::Point(double x, double y)
{
    (*this).x = x;
    (*this).y = y;
}
Point::Point()
{
    (*this).x = 0.0;
    (*this).y = 0.0;
}
double Point::distance(Point other)
{
    return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2));
}
void Point::print()
{
    cout << x << ", " << y;
}
void Point::updateLocation(double x, double y)
{
    this->x = x;
    this->y = y;
}
void Point::updateLocation(Point other)
{
    this->x = other.x;
    this->y = other.y;
}
bool Point::updateBoolean(Point src, Point dest, double coord, int isX)
{ // isX = 1 when using x coordinate, isX = 0 when using y coordinate
    if (isX == 1)
    {
        if (src.x > dest.x)
        {
            return coord < src.x;
        }
        else
        {
            return coord > src.x;
        }
    }
    else
    {
        if (src.y > dest.y)
        {
            return coord < src.y;
        }
        else
        {
            return coord > src.y;
        }
    }
}
Point Point::moveTowards(Point src, Point dest, double dist)
{
    if (dist < 0)
    {
        throw &dist;
    }
    if (src.distance(dest) <= dist)
    {
        return dest;
    }
    // We will now preform a min function to find the point closest to dest:
    Point res(src.x, src.y);
    double resX, resY, incX, incY; // Declaring output x and y and their increments.
    bool condX, condY;             // Declaring the conditions used in the for loops.
    // Assigning values for X, Y.
    if (src.x > dest.x)
    {
        resX = src.x - dist / 2;
        incX = dist / 100; // dist/100 is arbitrary; the division could be larger at the cost of run time.
        condX = resX < src.x;
    }
    else
    {
        resX = src.x + dist / 2;
        incX = -dist / 100;
        condX = resX > src.x;
    }
    if (src.y > dest.y)
    {
        resY = src.y - dist / 2;
        incY = dist / 100;
        condY = resY < src.y;
    }
    else
    {
        resY = src.y + dist / 2;
        incY = -dist / 100;
        condY = resY > src.y;
    }
    for (; condX; resX += incX)
    {
        condX = updateBoolean(src, dest, resX, 1);
        for (; condY; resY += incY)
        {
            condY = updateBoolean(src, dest, resY, 0);
            double tempDist = dest.distance(Point(resX, resY));
            if (tempDist < dest.distance(res) && src.distance(Point(resX, resY)) <= dist)
            {
                res.updateLocation(resX, resY);
            }
        }
    }
    return res;
}
string Point::printStr()
{
    return to_string(x) + ", " + to_string(y);
}
bool Point::operator==(const Point &other) const
{
    return x == other.x && y == other.y;
}

Character::Character(string name, Point location, int hp, string type)
{
    this->name = name;
    this->location = location;
    this->hp = hp;
    this->type = type;
    this->inTeam = false;
}
Character::Character()
{
    this->name = "No name";
    this->location = Point();
    this->hp = 0;
    this->inTeam = false;
}
Character::~Character()
{
}
bool Character::isAlive()
{
    return hp > 0;
}
double Character::distance(Character *other)
{
    return location.distance(other->location);
}
void Character::hit(int damage)
{
    if (!isAlive() || damage < 0)
    {
        throw this;
    }
    if (hp - damage < 0)
    {
        hp = 0;
    }
    else
    {
        hp = hp - damage;
    }
}
string Character::getName()
{
    return name;
}
Point *Character::getLocation()
{
    return &location;
}
string Character::print()
{
    return "Name: " + name + ", Character Type: " + type + ", location: " + location.printStr() + ", HP: " + to_string(hp);
}
int Character::getHp()
{
    return hp;
}
int Character::getType()
{
    if (type == "cowboy")
    {
        return 1;
    }
    if (type == "young ninja" || type == "trained ninja" || type == "old ninja")
    {
        return 2;
    }
    return -1;
}
void Character::attack(Character *enemy)
{
    // Empty function as it will allways be overridden
}
bool Character::operator==(Character other)
{
    return name == other.name && type == other.type && location == *(other.getLocation());
}

Ninja::Ninja(string name, Point location, int hp, int speed, string type) : Character(name, location, hp, type + "ninja")
{
    this->speed = speed;
}
Ninja::~Ninja() // Empty as it calls Character's destructor
{
}
int Ninja::getSpeed()
{
    return speed;
}
void Ninja::move(Character *enemy)
{
    if (!isAlive() || enemy == this)
    {
        throw this;
    }
    Point res = getLocation()->moveTowards(*getLocation(), *(enemy->getLocation()), speed);
    getLocation()->updateLocation(res);
}
void Ninja::slash(Character *enemy)
{
    if (!enemy->isAlive() || enemy == this)
    {
        throw enemy;
    }
    if(!isAlive()){
        throw this;
    }
    if (distance(enemy) <= 1)
    {
        enemy->hit(40);
    }
}
void Ninja::attack(Character *enemy)
{
    if (enemy == nullptr || enemy == this)
    {
        throw enemy;
    }
    if (!isAlive())
    {
        throw this;
    }
    if (getLocation()->x == MAXFLOAT)
    {
        return;
    }
    cout << getName() << " is attacking " << enemy->getName();
    if (distance(enemy) > 1)
    {
        move(enemy);
        cout << ", "
             << getName() << " has moved to " << getLocation()->printStr() << endl;
        return;
    }
    slash(enemy);
    cout << ", They now have " << enemy->getHp() << " hp left" << endl;
}

YoungNinja::YoungNinja(string name, Point location) : Ninja(name, location, 100, 14, "young ")
{
}
YoungNinja::~YoungNinja()
{
}

TrainedNinja::TrainedNinja(string name, Point location) : Ninja(name, location, 120, 12, "trained ")
{
}
TrainedNinja::~TrainedNinja()
{
}

OldNinja::OldNinja(string name, Point location) : Ninja(name, location, 150, 8, "old ")
{
}
OldNinja::~OldNinja()
{
}

Cowboy::Cowboy(string name, Point location) : Character(name, location, 110, "cowboy")
{
    num_of_bullets = 6;
}
Cowboy::~Cowboy()
{
}
bool Cowboy::hasboolets()
{
    return num_of_bullets > 0;
}
void Cowboy::reload()
{
    if(!isAlive()){
        throw this;
    }
    num_of_bullets = 6;
}
void Cowboy::shoot(Character *enemy)
{
    if (!enemy->isAlive() || enemy == this)
    {
        throw enemy;
    }
    if (!isAlive())
    {
        throw this;
    }
    if (hasboolets())
    {
        num_of_bullets--;
        enemy->hit(10);
    }
}
void Cowboy::attack(Character *enemy)
{
    if (enemy == nullptr || enemy == this)
    {
        throw enemy;
    }
    if (!isAlive())
    {
        throw this;
    }
    cout << getName() << " is attacking " << enemy->getName();
    if (!hasboolets())
    {
        reload();
        return;
    }
    shoot(enemy);
    cout << ", They now have " << enemy->getHp() << " hp left" << endl;
}

Team::Team(Character *leader)
{
    if(leader->inTeam){
        throw leader;
    }
    this->leader = leader;
    leader->inTeam = true;
    leader->placeInTeam = 0;
    members.push_back(leader);
    if (leader->getType() == 1)
    {
        cowboys.push_back(static_cast<Cowboy *>(leader));
    }
    else if (leader->getType() == 2)
    {
        ninjas.push_back(static_cast<Ninja *>(leader));
    }
    else
    {
        throw leader;
    }
}
Team::~Team()
{
}
Character *Team::getLeader()
{
    return leader;
}
void Team::add(Character *member)
{
    if (member->inTeam == true || member == nullptr)
    {
        throw member;
    }
    for (auto x : members)
    {
        if (x->getName() == member->getName() && x->getType() == member->getType())
        {
            throw member;
        }
    }
    if (members.size() < 10)
    {
        members.push_back(member);
        member->inTeam = true;
        member->placeInTeam = members.size() - 1;
        if (member->getType() == 1)
        {
            cowboys.push_back(dynamic_cast<Cowboy *>(member));
        }
        else if (member->getType() == 2)
        {
            ninjas.push_back(dynamic_cast<Ninja *>(member));
        }
        else
        {
            cout << "An unexpected error has occurred while adding character " << member->getName() << endl;
        }
    }
}
int Team::stillAlive()
{
    int res = 0;
    for (Character *x : members)
    {
        if (x->isAlive())
        {
            res++;
        }
    }
    return res;
}
std::vector<Character *> Team::getMembers()
{
    return members;
}
Character *Team::closestTo(Character *member)
{
    // Simple min function.
    Character *res = members.front();
    for (Character *x : members)
    { // We start by assigning res to be the first living member
        if (x->isAlive())
        {
            res = x;
            break;
        }
    }
    for (Character *x : members)
    {
        if (x->isAlive() && x->distance(member) < res->distance(member))
        {
            res = x;
        }
    }
    if (!res->isAlive())
    {
        res = NULL;
    }
    return res;
}
void Team::attack(Team *other)
{
    if (other == nullptr || other == this)
    {
        throw other;
    }
    if (stillAlive() <= 0 || other->stillAlive() <= 0)
    {
        return;
    }
    Character *activeLeader = leader;
    if (!activeLeader->isAlive())
    {
        activeLeader = closestTo(leader); // cannot be null as stillAlive() > 0
    }
    Character *currentVictim = other->closestTo(activeLeader); // cannot be null as other->stillAlive() > 0
    while (stillAlive() > 0 && other->stillAlive() > 0)
    {
        // Cowboys attacking
        for (Cowboy *currCboy : cowboys)
        {
            if (currCboy->isAlive())
            {
                currCboy->attack(currentVictim);
            }
            // Update activeLeader and currentVictim:
            if (!activeLeader->isAlive())
            {
                activeLeader = closestTo(activeLeader);
                if (activeLeader == NULL)
                {
                    return;
                }
            }
            if (!currentVictim->isAlive())
            {
                currentVictim = other->closestTo(activeLeader);
                if (currentVictim == NULL)
                {
                    return;
                }
            }
        }
        // Ninjas attacking
        for (Ninja *currNin : ninjas)
        {
            if (currNin->isAlive())
            {
                currNin->attack(currentVictim);
            }
            // Update activeLeader and currentVictim:
            if (!activeLeader->isAlive())
            {
                activeLeader = closestTo(activeLeader);
                if (activeLeader == NULL)
                {
                    return;
                }
            }
            if (!currentVictim->isAlive())
            {
                currentVictim = other->closestTo(activeLeader);
                if (currentVictim == NULL)
                {
                    return;
                }
            }
        }
    }
}
void Team::print()
{
    cout << "Printing members of team " << leader->getName() << ":" << endl;
    // Print cowboys:
    for (Cowboy *x : cowboys)
    {
        cout << x->print() << endl;
    }
    // Print ninjas:
    for (Ninja *x : ninjas)
    {
        cout << x->print() << endl;
    }
}

Team2::Team2(Character *leader) : Team(leader)
{ // Empty as it is the same as Team(leader)
}
Team2::~Team2()
{
}
void Team2::attack(Team *other)
{
    if (other == nullptr || other == this)
    {
        throw other;
    }
    if (stillAlive() <= 0 || other->stillAlive() <= 0)
    {
        return;
    }
    Character *activeLeader = getLeader();
    while (stillAlive() > 0 && other->stillAlive() > 0)
    {
        if (!activeLeader->isAlive())
        {
            activeLeader = closestTo(getLeader()); // cannot be null as stillAlive() > 0
        }
        Character *currentVictim = other->closestTo(activeLeader);
        for (Character *currNin : getMembers())
        {
            if (currNin->isAlive())
            {
                currNin->attack(currentVictim);
            }
            // Update activeLeader and currentVictim:
            if (!activeLeader->isAlive())
            {
                activeLeader = closestTo(activeLeader);
                if (activeLeader == NULL)
                {
                    return;
                }
            }
            if (!currentVictim->isAlive())
            {
                currentVictim = other->closestTo(activeLeader);
                if (currentVictim == NULL)
                {
                    return;
                }
            }
        }
    }
}
void Team2::print()
{
}

SmartTeam::SmartTeam(Character *leader) : Team(leader)
{
}
SmartTeam::~SmartTeam()
{
}
Character *SmartTeam::leastHealth(Team *other, int *exclude, size_t length)
{
    Character *output = other->getLeader();
    for (Character *x : other->getMembers())
    {
        bool isExcluded = false;
        if (exclude != nullptr)
        {
            for (int i = 0; i < length; i++)
            {
                if (exclude[i] == x->placeInTeam)
                {
                    isExcluded = true;
                    break;
                }
            }
        }
        if (isExcluded)
        {
            continue;
        }
        if (x->isAlive() && x->getHp() < output->getHp())
        {
            output = x;
        }
    }
    return output;
}
Character *SmartTeam::ninjaVictim(Team *other, Ninja *ninja, int *exclude, size_t length)
{
    // Assign values to potential victims according to their distance from currChar, their health, and their likelyhood to kill currChar before they die.
    // If the potential victim is a cowboy : Divide distance by speed to know how many turns it would take to reach the victim.
    // Multiply the result by ten to see how much damage we will take before striking back.
    // Finally, divide the victim's health by 40 to see how many more times they would attack us before they die.
    // That is the potential damage we will receive. Now we need to calculate the potential damage we will cause:
    // Remove the value we found about how much damage we will take whilst moving from out current health (0 if the victim is also a ninja).
    // Divide currChar's health by the damage the victim can cause to see how many times we can strike before we die.
    // Now we use those values to pick our victim: if the potential damage we receive is greater than our health, we pass on the potential victim.
    // (Edge case: in the case where all the potential damage we may take is more than our health, simply pick the victim we will hurt the most)
    // Going on, we subtract potential received damage from potential caused damage and choose the victim with the highest value.
    Character *output = other->getLeader();
    int damageReceived[other->getMembers().size()];
    int damageCaused[other->getMembers().size()];
    int passedOn[other->getMembers().size()]; // This list will tell us if we passed on the character at that index because they will kill us first.
    for (int i = 0; i < other->getMembers().size(); i++)
    {
        damageCaused[i] = 0;
        damageReceived[i] = 0;
        passedOn[i] = 0;
    }
    for (Character *potVictim : other->getMembers())
    {
        bool isExcluded = false;
        if (exclude != nullptr)
        {
            for (int i = 0; i < length; i++)
            {
                if (exclude[i] == potVictim->placeInTeam)
                {
                    isExcluded = true;
                    break;
                }
            }
        }
        if (!potVictim->isAlive() || isExcluded)
        {
            passedOn[potVictim->placeInTeam] = 1; // assign 1 so we will not accidentally consider this character
            continue;
        }
        if (potVictim->getType() == 1)
        {                                                                                                                 // Cowboy case
            damageReceived[potVictim->placeInTeam] += ((int)(ninja->distance(potVictim) + 0.5) / ninja->getSpeed()) * 10; // Rounding up the distance
            damageCaused[potVictim->placeInTeam] += ((ninja->getHp() - damageReceived[potVictim->placeInTeam]) / 10) * 40;
            damageReceived[potVictim->placeInTeam] += (potVictim->getHp() / 40) * 10;
        }
        if (potVictim->getType() == 2)
        {
            damageCaused[potVictim->placeInTeam] += ((ninja->getHp()) / 40) * 40;
            damageReceived[potVictim->placeInTeam] += (potVictim->getHp() / 40) * 40;
        }
        if (damageReceived[potVictim->placeInTeam] > ninja->getHp())
        {
            passedOn[potVictim->placeInTeam] = 1;
            continue;
        }
        if (damageCaused[potVictim->placeInTeam] - damageReceived[potVictim->placeInTeam] > damageCaused[output->placeInTeam] - damageReceived[output->placeInTeam])
        {
            output = potVictim;
        }
    }
    // Check if we passed all potential victims:
    int multresult = 1;
    for (int i = 0; i < other->getMembers().size(); i++)
    {
        multresult *= passedOn[i];
    }
    if (multresult == 1)
    {
        for (Character *potVictim : other->getMembers())
        {
            if (potVictim->isAlive() && damageCaused[potVictim->placeInTeam] - damageReceived[potVictim->placeInTeam] > damageCaused[output->placeInTeam] - damageReceived[output->placeInTeam])
            {
                output = potVictim;
            }
        }
    }
    return output;
}
void SmartTeam::attack(Team *other)
{
    // Make a new array that covers all the members of the other team and compares the other team's character stats to the character attacking them.
    int otherArr[other->getMembers().size()]; // This array keeps track of the attacking force against each character in the other team.
    size_t lastVictim[getMembers().size()];      // This array keeps track of the current victim of all our team's members
    for (int i = 0; i < other->getMembers().size(); i++)
    {
        otherArr[i] = 0;
    }
    for (int i = 0; i < getMembers().size(); i++)
    {
        lastVictim[i] = 11; // Max num is 9 as only 10 members can be at each team.
    }
    while (stillAlive() > 0 && other->stillAlive() > 0)
    {
        for (Character *x : other->getMembers())
        {
            if (!x->isAlive())
            {
                otherArr[x->placeInTeam] = 999999; // Setting this so we dont accidentally target a dead character
            }
        }
        for (Character *currChar : getMembers())
        {
            // We start with the case of cowboys as it is much simpler
            if (currChar->getType() == 1)
            {
                if (other->stillAlive() == 0)
                { // This is the loop's condition, but may be changed before we finish a complete iteration.
                    return;
                }
                // Find character with least health and shoot them
                Character *victim = leastHealth(other);
                int *exclude = (int *)malloc(sizeof(int));
                *exclude = victim->placeInTeam;
                size_t length = 1;
                while (otherArr[victim->placeInTeam] >= 1 && length < other->getMembers().size())
                {
                    length += 1;
                    victim = leastHealth(other, exclude, length);
                    exclude = (int *)realloc(exclude, length * sizeof(int));
                    *(exclude + length) = victim->placeInTeam;
                }
                free(exclude);
                currChar->attack(victim);
                if(lastVictim[currChar->placeInTeam] == 11){
                    lastVictim[currChar->placeInTeam] = (size_t)victim->placeInTeam;
                }
                if (lastVictim[currChar->placeInTeam] != (size_t)victim->placeInTeam)
                {
                    if (other->getMembers().at(lastVictim[currChar->placeInTeam])->getType() == 2)
                    {
                        otherArr[lastVictim[currChar->placeInTeam]] -= (currChar->getHp() + 10) / (other->getMembers().at(lastVictim[currChar->placeInTeam])->getHp() + 40);
                    }
                    else if (victim->getType() == 1)
                    {
                        otherArr[lastVictim[currChar->placeInTeam]] -= (currChar->getHp() + 10) / (other->getMembers().at(lastVictim[currChar->placeInTeam])->getHp() + 10);
                    }
                    lastVictim[currChar->placeInTeam] = (size_t)victim->placeInTeam;
                }
                if (victim->getType() == 2)
                {
                    otherArr[victim->placeInTeam] += (currChar->getHp() + 10) / (victim->getHp() + 40);
                }
                else if (victim->getType() == 1)
                {
                    otherArr[victim->placeInTeam] += (currChar->getHp() + 10) / (victim->getHp() + 10);
                }
                else
                {
                    throw victim;
                }
            }
            if (currChar->getType() == 2)
            {
                if (other->stillAlive() == 0)
                {
                    return;
                }
                cout << other->stillAlive() << endl;
                Character *victim = ninjaVictim(other, static_cast<Ninja *>(currChar));
                int *exclude = (int *)malloc(sizeof(int));
                *exclude = victim->placeInTeam;
                size_t length = 1;
                while (otherArr[victim->placeInTeam] >= 1 && length < other->getMembers().size())
                {
                    length += 1;
                    victim = ninjaVictim(other, static_cast<Ninja *>(currChar), exclude, length);
                    exclude = (int *)realloc(exclude, length * sizeof(int));
                    *(exclude + length) = victim->placeInTeam;
                }
                free(exclude);
                currChar->attack(victim);
                if(lastVictim[currChar->placeInTeam] == 11){
                    lastVictim[currChar->placeInTeam] = (size_t)victim->placeInTeam;
                }
                if (lastVictim[currChar->placeInTeam] != (size_t)victim->placeInTeam)
                {
                    if (other->getMembers().at(lastVictim[currChar->placeInTeam])->getType() == 2)
                    {
                        otherArr[lastVictim[currChar->placeInTeam]] -= (currChar->getHp() + 40) / (other->getMembers().at(lastVictim[currChar->placeInTeam])->getHp() + 40);
                    }
                    else if (victim->getType() == 1)
                    {
                        otherArr[lastVictim[currChar->placeInTeam]] -= (currChar->getHp() + 40) / (other->getMembers().at(lastVictim[currChar->placeInTeam])->getHp() + (other->getMembers().at(lastVictim[currChar->placeInTeam])->distance(currChar) / static_cast<Ninja *>(currChar)->getSpeed()) * 10);
                    }
                    lastVictim[currChar->placeInTeam] = (size_t)victim->placeInTeam;
                }
                if (victim->getType() == 2)
                {
                    otherArr[victim->placeInTeam] += (currChar->getHp() + 40) / (victim->getHp() + 40);
                }
                else if (victim->getType() == 1)
                {
                    otherArr[victim->placeInTeam] += (currChar->getHp() + 40) / (victim->getHp() + (victim->distance(currChar) / static_cast<Ninja *>(currChar)->getSpeed()) * 10);
                }
                else
                {
                    throw victim;
                }
            }
        }
    }
}

void SmartTeam::print()
{
    // print alphabetically.
}