#pragma once

#include "types.h"

namespace ui {

/**
 * @brief Keys meant to be maintained to be activated.
 */
enum class KeyToggleIndex {
    // Movement
    Forward = 0,
    Backward,
    Left,
    Right,
    Up,
    Down,
    Speed,

    Count
};
constexpr u32 TOGGLE_INDEX_COUNT = (u32)KeyToggleIndex::Count;

/**
 * @brief Keys meant to be switched on/off.
 */
enum class KeySwitchIndex {
    DisableTime,
    MouseEnable,

    Count
};
constexpr u32 SWITCH_INDEX_COUNT = (u32)KeySwitchIndex::Count;

/**
 * @brief Keys meant to be pressed a couple time to select a value.
 */
enum class KeyValueIndex {
    DisplayDebug,

    Count
};
constexpr u32 VALUE_INDEX_COUNT = (u32)KeyValueIndex::Count;

class KeyHandler {
public:
    /* ====================================================================== */
    /*                                 METHODS                                */
    /* ====================================================================== */

    static void    toggleKey(const KeyToggleIndex index) noexcept;
    static void    untoggleKey(const KeyToggleIndex index) noexcept;
    static void    switchKey(const KeySwitchIndex index) noexcept;
    static void    setKeyValue(const KeyValueIndex index, const u32 value) noexcept;

    static bool    isKeyToggled(const KeyToggleIndex index) noexcept { return m_toggledKeys[(u32)index]; }
    static bool    isKeyOn(const KeySwitchIndex index) noexcept { return m_switchedKeys[(u32)index]; }
    static u32     getKeyValue(const KeyValueIndex index) noexcept { return m_keyValue[(u32)index]; }

private:
    /* ====================================================================== */
    /*                                  DATA                                  */
    /* ====================================================================== */

    static bool    m_toggledKeys[TOGGLE_INDEX_COUNT];
    static bool    m_switchedKeys[SWITCH_INDEX_COUNT];
    static u32     m_keyValue[VALUE_INDEX_COUNT];

}; // class KeyHandler

} // namespace ui