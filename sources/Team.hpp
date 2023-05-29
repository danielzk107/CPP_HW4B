#include <list>
#include <vector>
#include <string>
using namespace std;

namespace ariel
{

    class Point
    {
    public:
        double x, y; // Variables not in private as they are very frequently used.

        Point(double x, double y);
        Point();
        double distance(Point other);
        void updateLocation(Point other);
        void updateLocation(double x, double y);
        Point moveTowards(Point src, Point dest, double dist);
        void print();
        string printStr();
        bool updateBoolean(Point src, Point dest, double coord, int isX); // Function that is used by moveTowards.
        bool operator==(const Point &other) const;
    };
    class Character
    {
    private:
        Point location;
        int hp;
        string name;
        string type;

    public:
        bool inTeam;
        int placeInTeam; // Used by smartTeam - index of this character in their team's list of members
        Character();
        Character(string name, Point location, int hp, string type);
        ~Character();
        int getHp();
        bool isAlive();
        double distance(Character *other);
        void hit(int damage); // Function that reduces the hitpoints of the character
        string getName();
        Point *getLocation();
        virtual void attack(Character *enemy);
        string print();
        int getType(); // Returns 1 if cowboy, 2 if ninja
        bool operator==(Character other);
    };
    class Ninja : public Character
    {
    private:
        int speed;

    public:
        Ninja(string name, Point location, int hp, int speed, string type);
        ~Ninja();
        void move(Character *enemy);
        int getSpeed();
        void slash(Character *enemy);
        void attack(Character *enemy);
    };
    class YoungNinja : public Ninja
    {
    public:
        YoungNinja(string name, Point location);
        ~YoungNinja();
    };
    class TrainedNinja : public Ninja
    {
    public:
        TrainedNinja(string name, Point location);
        ~TrainedNinja();
    };
    class OldNinja : public Ninja
    {
    public:
        OldNinja(string name, Point location);
        ~OldNinja();
    };
    class Cowboy : public Character
    {
    private:
        int num_of_bullets;

    public:
        Cowboy(string name, Point location);
        ~Cowboy();
        bool hasboolets();
        void reload();
        void shoot(Character *enemy);
        void attack(Character *enemy);
    };
    class Team
    {
    private:
        Character *leader;
        std::vector<Character *> members; // Max 10 members
        std::vector<Cowboy *> cowboys;
        std::vector<Ninja *> ninjas;

    public:
        Team(Character *leader);
        ~Team();
        Character *getLeader();
        std::vector<Character *> getMembers();
        Character *closestTo(Character *member);
        void add(Character *member);
        int stillAlive();
        void attack(Team *other);
        void print();
    };
    class Team2 : public Team
    {
    public:
        Team2(Character *leader);
        ~Team2();
        void attack(Team *other);
        void print();
    };
    class SmartTeam : public Team
    {
    public:
        SmartTeam(Character *leader);
        ~SmartTeam();
        Character * leastHealth(Team *other, int *exclude = nullptr, size_t length = 0);
        Character * ninjaVictim(Team *other, Ninja *ninja, int *exclude = nullptr, size_t length = 0);
        void attack(Team *other);
        void print();
    };

};
