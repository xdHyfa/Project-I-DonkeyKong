#include "Entities/EntityCollision.hpp"

bool EntityCollision(Entity& entity1, Entity& entity2)
{
    if (CheckCollisionRecs(entity1.getHitbox(), entity2.getHitbox()))
    {
        return true;
    }

    return false;
}