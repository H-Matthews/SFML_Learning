#ifndef CATEGORY_H
#define CATEGORY_H

// Purpose: Define categories to ensure correct delivery for our message delivery system
// Description: Each category except for None is initialized with an integer that has one bit set to 1, and the 
//              rest are set to 0
// Why: It gives us the ability to bitwise OR different categories together in order to send to multiple groups
// Example: It gives us the ability to do this: anyAircraft = Category::PlayerAircraft | Category::AlliedAircraft
namespace Category
{
    enum Type
    {
        None = 0,
        Scene = 1 << 0,
        PlayerAircraft = 1 << 1,
        AlliedAircraft = 1 << 2,
        EnemyAircraft = 1 << 3,
    };
}

#endif