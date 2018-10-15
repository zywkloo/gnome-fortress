#include "gnome_fortress/model/SceneNode.h"

#define GLEW_STATIC
#include <GL/glew.h>

namespace gnome_fortress {
namespace model {

// === PUBLIC ===

SceneNode::SceneNode()
    : position(),
      rotation(),
      scale_vec(1, 1, 1),
      parent(nullptr),
      children() {

}


void SceneNode::update(float delta_time) {
    onUpdateSelf(delta_time);
    onUpdateChildren(delta_time);
}

void SceneNode::draw(const glm::mat4 &parent_transform) const {
    onDrawSelf(parent_transform);
    onDrawChildren(parent_transform);
}

// === LOCAL TRANSFORMATIONS ===

glm::vec3 SceneNode::getPosition() const {
    return position;
}

void SceneNode::setPosition(const glm::vec3 &new_position) {
    position = new_position;
}

void SceneNode::setPosition(float x, float y, float z) {
    position = glm::vec3(x, y, z);
}

void SceneNode::setX(float x) {
    position.x = x;
}

void SceneNode::setY(float y) {
    position.y = y;
}

void SceneNode::setZ(float z) {
    position.z = z;
}

void SceneNode::translate(const glm::vec3 &translation) {
    position += translation;
}

void SceneNode::translate(float tx, float ty, float tz) {
    position += glm::vec3(tx, ty, tz);
}

void SceneNode::translateX(float tx) {
    position.x += tx;
}

void SceneNode::translateY(float ty) {
    position.y += ty;
}

void SceneNode::translateZ(float tz) {
    position.z += tz;
}

glm::quat SceneNode::getRotation() const {
    return rotation;
}

glm::mat4 SceneNode::getRotationMatrix() const {
    return glm::mat4_cast(rotation);
}

void SceneNode::setRotation(const glm::quat &new_rotation) {
    rotation = new_rotation;
}

void SceneNode::setRotation(float angle, const glm::vec3 &axis) {
    rotation = glm::angleAxis(angle, axis);
}

void SceneNode::rotate(const glm::quat &in_rotation) {
    rotation = glm::normalize(rotation * in_rotation);
}

void SceneNode::rotate(float angle, const glm::vec3 &axis) {
    rotate(glm::angleAxis(angle, axis));
}

void SceneNode::setOrbit(const glm::quat &in_rotation, const glm::vec3 &anchor, const glm::vec3 &init_position) {
    setRotation(in_rotation);

    position = init_position - anchor;
    position = glm::vec3(rotation * glm::vec4(position, 0));
    position += anchor;
}

void SceneNode::setOrbit(float angle, const glm::vec3 &axis, const glm::vec3 &anchor, const glm::vec3 &init_position) {
    setOrbit(glm::angleAxis(angle, axis), anchor, init_position);
}

void SceneNode::orbit(const glm::quat &in_rotation, const glm::vec3 &anchor) {
    rotate(in_rotation);

    position -= anchor;
    position = glm::vec3(in_rotation * glm::vec4(position, 0));
    position += anchor;
}

void SceneNode::orbit(float angle, const glm::vec3 &axis, const glm::vec3 &anchor) {
    orbit(glm::angleAxis(angle, axis), anchor);
}

glm::vec3 SceneNode::getScale() const {
    return scale_vec;
}

void SceneNode::setScale(const glm::vec3 &new_scale) {
    scale_vec = new_scale;
}

void SceneNode::setScale(float sx, float sy, float sz) {
    scale_vec = glm::vec3(sx, sy, sz);
}

void SceneNode::setScaleX(float sx) {
    scale_vec.x = sx;
}

void SceneNode::setScaleY(float sy) {
    scale_vec.y = sy;
}

void SceneNode::setScaleZ(float sz) {
    scale_vec.z = sz;
}

void SceneNode::scale(const glm::vec3 &scale) {
    scale_vec *= scale;
}

void SceneNode::scale(float sx, float sy, float sz) {
    scale_vec *= glm::vec3(sx, sy, sz);
}

void SceneNode::scaleX(float sx) {
    scale_vec.x *= sx;
}

void SceneNode::scaleY(float sy) {
    scale_vec.y *= sy;
}

void SceneNode::scaleZ(float sz) {
    scale_vec.z *= sz;
}

glm::mat4 SceneNode::getTransformMatrix() const {
    const glm::vec3 &t = position;
    const glm::vec3 &s = scale_vec;

    glm::mat4 local_translation_mat(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        t.x, t.y, t.z, 1
    );

    glm::mat4 local_scale_mat(
        s.x, 0, 0, 0,
        0, s.y, 0, 0,
        0, 0, s.z, 0,
        0, 0, 0, 1
    );

    return local_translation_mat * getRotationMatrix() * local_scale_mat;
}

void SceneNode::resetTransform() {
    position = glm::vec3();
    rotation = glm::quat();
    scale_vec = glm::vec3(1, 1, 1);
}

// === ANCESTRY ===

SceneNode *SceneNode::getParent() const {
    return parent;
}

glm::mat4 SceneNode::getGlobalTransform() const {
    if (parent) {
        return parent->getGlobalTransform() * getTransformMatrix();
    } else {
        return getTransformMatrix();
    }
}

glm::mat4 SceneNode::getGlobalInverseTransform() const {
    if (parent) {
        return glm::inverse(getTransformMatrix()) * parent->getGlobalInverseTransform();
    }
    else {
        return glm::inverse(getTransformMatrix());
    }
}

// === CHILD MANAGEMENT ===

int SceneNode::getNumChildren() const {
    return children.size();
}

int SceneNode::indexOf(SceneNode *child) const {
    for (int i = 0; i < children.size(); i++) {
        if (children[i] == child) {
            return i;
        }
    }
    return -1;
}

SceneNode *SceneNode::getChild(int index) const {
    if (index < 0 || index >= children.size()) {
        throw std::exception("Failed to get child. Index out of bounds.");
    }

    return children[index];
}

void SceneNode::forEachChild(const std::function<void(SceneNode *)> &callback) const {
    for (SceneNode *child : children) {
        callback(child);
    }
}

void SceneNode::insertChild(SceneNode *child, int index) {
    if (index < 0 || index > children.size()) {
        throw std::exception("Failed to insert child. Index out of bounds.");
    }
    if (child->parent) {
        throw std::exception("Failed to insert child. Given node already has a parent.");
    }

    children.insert(children.begin() + index, child);
    child->parent = this;
}

void SceneNode::appendChild(SceneNode *child) {
    if (child->parent) {
        throw std::exception("Failed to append child. Given node already has a parent.");
    }

    children.push_back(child);
    child->parent = this;
}

void SceneNode::replaceChild(SceneNode *child, int index) {
    if (index < 0 || index >= children.size()) {
        throw std::exception("Failed to replace child. Index out of bounds.");
    }
    if (child->parent) {
        throw std::exception("Failed to replace child. Given node already has a parent.");
    }

    children[index]->parent = nullptr;
    children[index] = child;
    child->parent = this;
}

void SceneNode::removeChild(int index) {
    if (index < 0 || index >= children.size()) {
        throw std::exception("Failed to remove child. Index out of bounds.");
    }

    children[index]->parent = nullptr;
    children.erase(children.begin() + index);
}

void SceneNode::clearChildren() {
    for (SceneNode *child : children) {
        child->parent = nullptr;
    }
    children.clear();
}

void SceneNode::removeFromParent() {
    if (parent) {
        parent->removeChild(parent->indexOf(this));
    }
}

// === PROTECTED ===

void SceneNode::applyTransform() const {
    // get angle and axis from rotation quaternion
    float angle = glm::angle(rotation);
    glm::vec3 axis = glm::axis(rotation);

    // SRT transform
    glTranslatef(position.x, position.y, position.z);
    glRotatef(glm::degrees(angle), axis.x, axis.y, axis.z);
    glScalef(scale_vec.x, scale_vec.y, scale_vec.z);
}

void SceneNode::onUpdateSelf(float delta_time) {
    // override to add update behaviour
}

void SceneNode::onUpdateChildren(float delta_time) {
    for (SceneNode *child : children) {
        child->update(delta_time);
    }
}

void SceneNode::onDrawSelf(const glm::mat4 &parent_transform) const {
    // override to add draw behaviour
}

void SceneNode::onDrawChildren(const glm::mat4 &parent_transform) const {
    for (SceneNode *child : children) {
        child->draw(parent_transform * getTransformMatrix());
    }
}

}
}