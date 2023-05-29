#include "doctest.h"
#include "sources/Team.hpp"
using namespace std;
using namespace ariel;

TEST_CASE("Point functions")
{
    Point p1(10, 9), p2(0.5, 6.8);
    CHECK(p1.distance(p2) == 9.7514101544341);
    CHECK(p1.distance(p2) == p2.distance(p1));
}

TEST_CASE("General character functions")
{
    Point p1(10, 9), p2(0.5, 6.8);
    YoungNinja ynin("Ninja name", p1);
    Cowboy cboy("Woody", p2);
    CHECK_EQ(cboy.getName(), "Woody");
    // CHECK_EQ(ynin.getLocation(), p1);

    ynin.hit(80);
    CHECK_EQ(ynin.getHp(), 20);

    CHECK(cboy.isAlive());
    cboy.hit(110);
    CHECK_FALSE(cboy.isAlive());
}

TEST_CASE("Ninja vs Cowboy")
{
    Point p1(10, 9), p2(0.5, 6.8);
    TrainedNinja *tnin = new TrainedNinja("Ninja name", p1);
    Cowboy *cboy = new Cowboy("Woody", p2);

    tnin->move(cboy);
    // CHECK_EQ(tnin->getLocation(), p2); // The ninja's speed is larger than the distance, so the ninja should be there.
    cboy->shoot(tnin);
    CHECK(tnin->getHp() == 110);
    tnin->slash(cboy);
    CHECK(cboy->getHp() == 70);
    CHECK(cboy->isAlive());

    CHECK(cboy->hasboolets());
    for (int i = 0; i < 5; i++)
    {
        cboy->shoot(tnin);
    }
    CHECK_FALSE(cboy->hasboolets());
    cboy->reload();
    CHECK(cboy->hasboolets());

    CHECK(tnin->isAlive());
    tnin->slash(cboy);
    tnin->slash(cboy); // Twice in order to kill the cowboy
    CHECK_FALSE(cboy->isAlive());

    delete cboy;
    delete tnin;
}

TEST_CASE("Team functions")
{
    Point p1(10, 9), p2(0.5, 6.8), p3(15, 14);
    TrainedNinja *tnin = new TrainedNinja("Ninja name", p1);
    Cowboy *cboy = new Cowboy("Woody", p2);
    OldNinja *onin = new OldNinja("Sensei", p3);
    Team t1(tnin), t2(cboy);
    CHECK(t1.getLeader() == tnin);

    t2.add(onin);
    CHECK(t2.stillAlive() == 2);
    while(tnin->getHp() >0){
        if(!cboy->hasboolets()){
            cboy->reload();
        }
        cboy->shoot(tnin);
    }
    CHECK_FALSE(tnin->isAlive());
    CHECK(t2.stillAlive() == 1);
    CHECK_EQ(t2.getLeader(), onin);

    t2.attack(&t1);
    CHECK(t1.stillAlive() == 0);
    CHECK(onin->getHp() < 150);
}

TEST_CASE("Smart team functionality"){
    Point p1(10, 9), p2(0.5, 6.8), p3(15, 14);
    TrainedNinja *tnin = new TrainedNinja("Ninja name", p1);
    Cowboy *cboy = new Cowboy("Woody", p2);
    Cowboy *kevin = new Cowboy("Kevin", p2);
    OldNinja *onin = new OldNinja("Sensei", p3);
    Team t1(tnin);
    SmartTeam sTeam(cboy);
    sTeam.add(onin);
    t1.add(kevin);

    sTeam.attack(&t1);
    CHECK(t1.stillAlive() == 0);
}