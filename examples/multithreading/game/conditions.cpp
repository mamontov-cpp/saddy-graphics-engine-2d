#include "conditions.h"

// ============================================ game::Conditions::ConditionsForRenderer ============================================

void game::Conditions::ConditionsForRenderer::init(bool is_inventory_thread)
{
    // Unused on starting screen and on paused screen
    LeftKeyConditions[game::Conditions::State::CS_START_SCREEN] = nullptr;
    LeftKeyConditions[game::Conditions::State::CS_OPTIONS_SCREEN] = new sad::KeyHoldCondition(sad::KeyboardKey::KeyLeft);
    LeftKeyConditions[game::Conditions::State::CS_PLAYGAME_PLAYING_PRESSED] = new sad::KeyHoldCondition(sad::KeyboardKey::KeyLeft);
    if (is_inventory_thread)
    {
         LeftKeyConditions[game::Conditions::State::CS_PLAYGAME_PLAYING_RELEASED] = nullptr;
         LeftKeyConditions[game::Conditions::State::CS_PLAYGAME_PAUSED_RELEASED] = nullptr;
    }
    else
    {
        LeftKeyConditions[game::Conditions::State::CS_PLAYGAME_PLAYING_RELEASED] = new sad::KeyHoldCondition(sad::KeyboardKey::KeyRight);
        LeftKeyConditions[game::Conditions::State::CS_PLAYGAME_PAUSED_RELEASED] = new sad::KeyHoldCondition(sad::KeyboardKey::KeyRight);
    }
    LeftKeyConditions[game::Conditions::State::CS_PLAYGAME_PAUSED_PRESSED] = nullptr;

    // Unused on starting screen and on  paused screen
    RightKeyConditions[game::Conditions::State::CS_START_SCREEN] = nullptr;
    RightKeyConditions[game::Conditions::State::CS_OPTIONS_SCREEN] = new sad::KeyHoldCondition(sad::KeyboardKey::KeyRight);
    RightKeyConditions[game::Conditions::State::CS_PLAYGAME_PLAYING_PRESSED] = new sad::KeyHoldCondition(sad::KeyboardKey::KeyRight);
    if (is_inventory_thread)
    {
        RightKeyConditions[game::Conditions::State::CS_PLAYGAME_PLAYING_RELEASED]= nullptr;
        RightKeyConditions[game::Conditions::State::CS_PLAYGAME_PAUSED_RELEASED] = nullptr;
    }
    else
    {
        RightKeyConditions[game::Conditions::State::CS_PLAYGAME_PLAYING_RELEASED] = new sad::KeyHoldCondition(sad::KeyboardKey::KeyRight);
        RightKeyConditions[game::Conditions::State::CS_PLAYGAME_PAUSED_RELEASED] = new sad::KeyHoldCondition(sad::KeyboardKey::KeyRight);
    }
    RightKeyConditions[game::Conditions::State::CS_PLAYGAME_PAUSED_PRESSED] = nullptr;

    if (is_inventory_thread)
    {
        UpKeyConditions[game::Conditions::State::CS_START_SCREEN] = nullptr;
    }
    else
    { 
        UpKeyConditions[game::Conditions::State::CS_START_SCREEN] = new sad::KeyHoldCondition(sad::KeyboardKey::KeyUp);
    }
    UpKeyConditions[game::Conditions::State::CS_OPTIONS_SCREEN] = new sad::KeyHoldCondition(sad::KeyboardKey::KeyUp);
    UpKeyConditions[game::Conditions::State::CS_PLAYGAME_PLAYING_PRESSED] = new sad::KeyHoldCondition(sad::KeyboardKey::KeyUp);
    if (is_inventory_thread)
    {
        UpKeyConditions[game::Conditions::State::CS_PLAYGAME_PLAYING_RELEASED] = nullptr;
        UpKeyConditions[game::Conditions::State::CS_PLAYGAME_PAUSED_RELEASED] = nullptr;
    }
    else
    {
        UpKeyConditions[game::Conditions::State::CS_PLAYGAME_PLAYING_RELEASED] = new sad::KeyHoldCondition(sad::KeyboardKey::KeyUp);
        UpKeyConditions[game::Conditions::State::CS_PLAYGAME_PAUSED_RELEASED] = new sad::KeyHoldCondition(sad::KeyboardKey::KeyUp);
    }
    UpKeyConditions[game::Conditions::State::CS_PLAYGAME_PAUSED_PRESSED] = new sad::KeyHoldCondition(sad::KeyboardKey::KeyUp);


    if (is_inventory_thread)
    {
        DownKeyConditions[game::Conditions::State::CS_START_SCREEN] = nullptr;
    }
    else
    {
        DownKeyConditions[game::Conditions::State::CS_START_SCREEN] = new sad::KeyHoldCondition(sad::KeyboardKey::KeyDown);
    }
    DownKeyConditions[game::Conditions::State::CS_OPTIONS_SCREEN] = new sad::KeyHoldCondition(sad::KeyboardKey::KeyDown);
    DownKeyConditions[game::Conditions::State::CS_PLAYGAME_PLAYING_PRESSED] = new sad::KeyHoldCondition(sad::KeyboardKey::KeyDown);
    if (is_inventory_thread)
    {
        DownKeyConditions[game::Conditions::State::CS_PLAYGAME_PLAYING_RELEASED] = nullptr;
        DownKeyConditions[game::Conditions::State::CS_PLAYGAME_PAUSED_RELEASED] = nullptr;
    }
    else
    { 
        DownKeyConditions[game::Conditions::State::CS_PLAYGAME_PLAYING_RELEASED] = new sad::KeyHoldCondition(sad::KeyboardKey::KeyDown);
        DownKeyConditions[game::Conditions::State::CS_PLAYGAME_PAUSED_RELEASED] = new sad::KeyHoldCondition(sad::KeyboardKey::KeyDown);
    }
    DownKeyConditions[game::Conditions::State::CS_PLAYGAME_PAUSED_PRESSED] = new sad::KeyHoldCondition(sad::KeyboardKey::KeyDown);

    if (is_inventory_thread)
    {
        JumpActionConditions[game::Conditions::State::CS_START_SCREEN] = nullptr;
    }
    else 
    {
        JumpActionConditions[game::Conditions::State::CS_START_SCREEN] = new sad::KeyHoldCondition(sad::KeyboardKey::Space);
    }

    JumpActionConditions[game::Conditions::State::CS_OPTIONS_SCREEN] = new sad::KeyHoldCondition(sad::KeyboardKey::Space);
    JumpActionConditions[game::Conditions::State::CS_PLAYGAME_PLAYING_PRESSED] = new sad::KeyHoldCondition(sad::KeyboardKey::Space);
    JumpActionConditions[game::Conditions::State::CS_PLAYGAME_PLAYING_RELEASED] = nullptr;
    JumpActionConditions[game::Conditions::State::CS_PLAYGAME_PAUSED_PRESSED] = new sad::KeyHoldCondition(sad::KeyboardKey::Space);
    JumpActionConditions[game::Conditions::State::CS_PLAYGAME_PAUSED_RELEASED] = nullptr;

    PauseCondition = new sad::KeyHoldCondition(sad::KeyboardKey::P);
    PauseConditionWhenPaused = new sad::KeyHoldCondition(sad::KeyboardKey::P);
}


void game::Conditions::ConditionsForRenderer::setLeftKey(sad::KeyboardKey key)
{
    game::Conditions::ConditionsForRenderer::setKeyForConditions(LeftKeyConditions, key);
}

void game::Conditions::ConditionsForRenderer::setRightKey(sad::KeyboardKey key)
{
    game::Conditions::ConditionsForRenderer::setKeyForConditions(RightKeyConditions, key);
}

void game::Conditions::ConditionsForRenderer::setUpKey(sad::KeyboardKey key)
{
    game::Conditions::ConditionsForRenderer::setKeyForConditions(UpKeyConditions, key);
}

void game::Conditions::ConditionsForRenderer::setDownKey(sad::KeyboardKey key)
{
    game::Conditions::ConditionsForRenderer::setKeyForConditions(DownKeyConditions, key);
}

void game::Conditions::ConditionsForRenderer::setJumpActionKey(sad::KeyboardKey key)
{
    game::Conditions::ConditionsForRenderer::setKeyForConditions(JumpActionConditions, key);
}

void game::Conditions::ConditionsForRenderer::setPauseKey(sad::KeyboardKey key) const
{
    PauseCondition->setKey(key);
    PauseConditionWhenPaused->setKey(key);
}

void game::Conditions::ConditionsForRenderer::setKeyForConditions(sad::KeyHoldCondition* c[], sad::KeyboardKey key,size_t length)
{
    for(size_t i = 0; i < length; i++)
    {
        if (c[i])
        {
            c[i]->setKey(key);
        }
    }
}


// ============================================ game::Conditions ============================================

game::Conditions::Conditions()  // NOLINT(cppcoreguidelines-pro-type-member-init)
{
    ConditionsForMainRenderer.init(false);
    ConditionsForInventoryRenderer.init(true);
}

void game::Conditions::setLeftKey(sad::KeyboardKey key)
{
    ConditionsForMainRenderer.setLeftKey(key);
    ConditionsForInventoryRenderer.setLeftKey(key);
}

void game::Conditions::setRightKey(sad::KeyboardKey key)
{
    ConditionsForMainRenderer.setRightKey(key);
    ConditionsForInventoryRenderer.setRightKey(key);
}

void game::Conditions::setUpKey(sad::KeyboardKey key)
{
    ConditionsForMainRenderer.setUpKey(key);
    ConditionsForInventoryRenderer.setUpKey(key);
}

void game::Conditions::setDownKey(sad::KeyboardKey key)
{
    ConditionsForMainRenderer.setDownKey(key);
    ConditionsForInventoryRenderer.setDownKey(key);
}

void game::Conditions::setJumpActionKey(sad::KeyboardKey key)
{
    ConditionsForMainRenderer.setJumpActionKey(key);
    ConditionsForInventoryRenderer.setJumpActionKey(key);
}

void game::Conditions::setPauseKey(sad::KeyboardKey key) const
{
    ConditionsForMainRenderer.setPauseKey(key);
    ConditionsForInventoryRenderer.setPauseKey(key);
}

void game::Conditions::apply(const game::Options& opts)
{
    setLeftKey(opts.LeftKey);
    setRightKey(opts.RightKey);
    setUpKey(opts.UpKey);
    setDownKey(opts.DownKey);
    setJumpActionKey(opts.JumpActionKey);
    setPauseKey(opts.PauseKey);
}