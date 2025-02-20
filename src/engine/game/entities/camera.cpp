#include "camera.h"
#include "controller.h"
#include "key_handler.h"
#include "maths.h"
#include "world.h"

namespace game {

/* ========================================================================== */
/*                                   PUBLIC                                   */
/* ========================================================================== */

Camera::Settings Camera::ms_cameraSettings;

void Camera::init(const math::vec3& position, const math::vec3& front) {
    m_position = position;
    m_directions.front = front;
    m_directions.right = math::normalize(math::cross(m_directions.front, WORLD_Y));
    m_directions.up = math::cross(m_directions.right, m_directions.front);
}

void Camera::update(const ui::Controller& controller) {
    // Camera angles
    const float oldYaw = math::radians(controller.getYaw());
    const float oldPitch = math::radians(controller.getPitch());
    const float cosYaw = std::cos(oldYaw);
    const float sinYaw = std::sin(oldYaw);
    const float cosPitch = std::cos(oldPitch);
    const float sinPitch = std::sin(oldPitch);

    m_directions.front = { cosYaw * cosPitch, sinPitch, sinYaw * cosPitch };
    m_directions.right = math::normalize(math::cross(m_directions.front, WORLD_Y));
    m_directions.up = math::cross(m_directions.right, m_directions.front);

    // Position
    const float moveSpeed = ui::KeyHandler::isKeyToggled(ui::KeyToggleIndex::Speed) ? HIGH_SPEED : NORMAL_SPEED;

    if (ui::KeyHandler::isKeyToggled(ui::KeyToggleIndex::Forward))
        m_position += math::vec3(cosYaw, 0.0f, sinYaw) * moveSpeed;
    else if (ui::KeyHandler::isKeyToggled(ui::KeyToggleIndex::Backward))
        m_position -= math::vec3(cosYaw, 0.0f, sinYaw) * moveSpeed;

    if (ui::KeyHandler::isKeyToggled(ui::KeyToggleIndex::Left))
        m_position += math::vec3(sinYaw, 0.0f, -cosYaw) * moveSpeed;
    else if (ui::KeyHandler::isKeyToggled(ui::KeyToggleIndex::Right))
        m_position -= math::vec3(sinYaw, 0.0f, -cosYaw) * moveSpeed;

    if (ui::KeyHandler::isKeyToggled(ui::KeyToggleIndex::Up))
        m_position.y += moveSpeed;
    else if (ui::KeyHandler::isKeyToggled(ui::KeyToggleIndex::Down))
        m_position.y -= moveSpeed;

    m_chunkPosition.x = ((i32)m_position.x / (i32)Chunk::SIZE) + World::SIZE;
    m_chunkPosition.y = ((i32)m_position.z / (i32)Chunk::SIZE) + World::SIZE;
}

} // namespace game