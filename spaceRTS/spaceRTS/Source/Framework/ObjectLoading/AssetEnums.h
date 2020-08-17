#pragma once
namespace MYENGINE
{
    enum Asset
    {
        Triangle,
        Square,
        OGCommander
    };

    enum ObjectType
    {
        Nothing = 0,
        Unit = 1,           //pawns are ...something
        Commander = 2,        //the ground!
        Building = 4,      //characters are creatures of some sort
        Actor = 8,          //actors are objects which can be interacted with
        Item = 16,          //pick up items
        Projectile = 32,    //arrows, flying rocks, etc
        Ethereal = 64,      //stuff can go through this, not affected by most forces such as gravity
        Building2 = 128,     //houses, gates, walls, etc.
        Obstacle = 256,     //trees, rocks, and other static impassables
        SOLID = Nothing | Unit | Commander | Building | Actor | Item | Projectile | Building2 | Obstacle,
        ALL = 511
    };

    enum WeaponType
    {
        Laser,
        Bullet,
        AOEProjectile
    };
}