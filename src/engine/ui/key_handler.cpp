#include "key_handler.h"

namespace ui {

bool    KeyHandler::m_toggledKeys[TOGGLE_INDEX_COUNT] = { false };
bool    KeyHandler::m_switchedKeys[SWITCH_INDEX_COUNT] = { false };
u32     KeyHandler::m_keyValue[VALUE_INDEX_COUNT] = { 0 };

/* ========================================================================== */

void KeyHandler::toggleKey(const KeyToggleIndex index) noexcept {
    if (isKeyToggled(index))
        return;
    m_toggledKeys[(u32)index] = true;
}

void KeyHandler::untoggleKey(const KeyToggleIndex index) noexcept {
    if (!isKeyToggled(index))
        return;
    m_toggledKeys[(u32)index] = false;
}

void KeyHandler::switchKey(const KeySwitchIndex index) noexcept {
    m_switchedKeys[(u32)index] = !m_switchedKeys[(u32)index];
}

void KeyHandler::setKeyValue(const KeyValueIndex index, const u32 value) noexcept {
    m_keyValue[(u32)index] = value;
}

} // namespace ui