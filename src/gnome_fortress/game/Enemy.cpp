#include "gnome_fortress/game/Enemy.h"

namespace gnome_fortress {
namespace game {


Enemy::Enemy(const model::MeshGroup *meshGroup, renderer::BasicMeshNodeTechnique *technique)
    : model::BasicMeshGroupNode(meshGroup, technique) {
    //Nothing yet
}


void Enemy::onUpdateSelf(float dt) {

}

float Enemy::GetBoundingRadius() {
    return boundingRadius;
}

const char* Enemy::GetAttackSound() {
    return attackSoundByte;
}

void Enemy::DoDamage(float damage) {
    health -= damage;
}

float Enemy::GetHealth() {
    return health;
}

void Enemy::PlayAttackSound() {
    glm::vec3 pos = getPosition();
    irrklang::ISound *effect = soundEngine->play3D(attackSoundByte, irrklang::vec3df(pos.x, pos.y, pos.z), GL_FALSE, GL_TRUE);
    if (effect) {
        effect->setMinDistance(1.5);
        effect->setVolume(1000.0);
        effect->setIsPaused(false);
    }
}

}
}