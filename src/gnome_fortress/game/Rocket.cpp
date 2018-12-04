#include "gnome_fortress/game/Resources.h"
#include "gnome_fortress/game/Rocket.h"

namespace gnome_fortress {
namespace game {
Rocket::Rocket(
    const model::MeshGroup *bulletMeshGroup,
    renderer::BasicMeshNodeTechnique *technique,
    glm::vec3 pointOfOrigin,
    glm::vec3 velocity) : game::Projectile(
        bulletMeshGroup,
        technique,
        pointOfOrigin,
        velocity
    ), 
    acceleration(1.0f) {

    threshold = 10;
}

const float Rocket::DAMAGE = 20.0f;
const float Rocket::DAMAGE_RAD = 3.0f;
const float Rocket::EXPLOSION_DAMAGE = 2.0f;
const float Rocket::EXPLOSION_LIFESPAN = 1.0f;

float Rocket::GetDamage() {
    return DAMAGE;
}

float Rocket::GetDamageRad() {
    return DAMAGE_RAD;
}

void Rocket::onUpdateSelf(float dt) {
    lifespan += dt;

    //setPosition(getPosition() + (velocity * dt));
    setPosition(getPosition() + (velocity * dt * acceleration));

    if (lifespan > 1) {
        acceleration += 0.5f;
    }
    setPosition(getPosition() + (velocity * dt));
    lifespan += 1 * dt;
}
}
}