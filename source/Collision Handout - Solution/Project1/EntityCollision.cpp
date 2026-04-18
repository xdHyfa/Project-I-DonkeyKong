#include "Entities/EntityCollision.hpp"
#include "Core/constants.h"

bool EntityCollision(Entity& entity1, Entity& entity2)
{
    Rectangle hb1 = entity1.getHitbox();
    Rectangle hb2 = entity2.getHitbox();

    // Reducimos el hitbox solo para esta comprobación
    int margin = 2;
    Rectangle shrunk1 = { hb1.x + margin, hb1.y + margin, hb1.width - margin*2, hb1.height - margin*2 };
    Rectangle shrunk2 = { hb2.x + margin, hb2.y + margin, hb2.width - margin*2, hb2.height - margin*2 };

    return CheckCollisionRecs(shrunk1, shrunk2);
}