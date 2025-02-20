#pragma once

#include "game_decl.h"
#include "types.h"
#include "vector.h"

namespace ui {
class Controller;
}

namespace game {

struct Camera {
    /* ====================================================================== */
    /*                              HELPER CLASS                              */
    /* ====================================================================== */

    struct Settings {
        const f32     nearPlane = DEFAULT_NEAR_PLANE;
        const f32     farPlane = DEFAULT_FAR_PLANE;

        f32     fov = CAM_FOV;
        f32     cameraSpeed = 0.15f;
        f32     brightness = 100.0f; // in percent
    };

    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static constexpr f32    DEFAULT_NEAR_PLANE = 0.01f;
    static constexpr f32    DEFAULT_FAR_PLANE = 500.0f;

    static constexpr f32    ASPECT_RATIO = WINDOW_WIDTH / WINDOW_HEIGHT;

    static constexpr f32    NORMAL_SPEED = 0.15f;
    static constexpr f32    HIGH_SPEED = 0.8f;

    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    void init(const math::vec3& position, const math::vec3& front);
    void update(const ui::Controller& controller);

    const math::ivec2&      getChunkPosition() const noexcept { return m_chunkPosition; }
    static const Settings&  getSettings() noexcept { return ms_cameraSettings; }

    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    math::vec3      m_position;
    math::ivec2     m_chunkPosition = { 0, 0 };
    struct {
        math::vec3 front;
        math::vec3 right;
        math::vec3 up;
    }               m_directions;

    bool            m_updateChunk = false;

private:
    /* ====================================================================== */
    /*                             STATIC MEMBERS                             */
    /* ====================================================================== */

    static Settings ms_cameraSettings;

}; // class Camera

} // namespace game