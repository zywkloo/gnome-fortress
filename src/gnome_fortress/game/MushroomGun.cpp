#include "gnome_fortress/game/MushroomGun.h"
#include "gnome_fortress/game/Resources.h"
#include "gnome_fortress/game/Spore.h"

namespace gnome_fortress {
namespace game {
MushroomGun::MushroomGun(
    resource::ResourceManager &resourceManager,
    renderer::BasicMeshNodeTechnique *technique,
    Player *player)
    : game::Weapon(
        resourceManager.getOrLoadMeshGroup(resources::models::mushroom_gun),
        resourceManager.getOrLoadMeshGroup(resources::models::rock5),
        technique,
        player
    ) {

}
  
void MushroomGun::onUpdateSelf(float delta_time) {
    cooldown -= delta_time;
}

Projectile* MushroomGun::fireBullet(glm::vec3 position) {
    glm::vec3 vel = glm::normalize(glm::vec3(getGlobalTransform() * glm::vec4(0, 0, -1, 0)));
    vel.x *= 5.0; //FIRING_VELOCITY;
    vel.y *= 5.0; //FIRING_VELOCITY;
    vel.z *= 5.0; //FIRING_VELOCITY;
    Projectile *p = new Spore(bulletMeshGroup, getTechnique(),
        glm::vec3(getGlobalTransform() * glm::vec4(0.03, 0.5, -0.25, 1)), vel);
    // Projectile *p = new Projectile(bulletMeshGroup, getTechnique(),
    //     glm::vec3(getGlobalTransform() * glm::vec4(0.03, 0.5, -0.25, 1)), vel);
    return p;
}

}
}