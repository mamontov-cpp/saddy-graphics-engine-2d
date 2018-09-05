#include "conditions.h"

// ============================================ game::Conditions::ConditionsForRenderer ============================================

void game::Conditions::ConditionsForRenderer::init(bool is_inventory_thread)
{
    // Unused on starting screen and on paused screen
    LeftKeyConditions[game::Conditions::CS_START_SCREEN] = NULL;
    LeftKeyConditions[game::Conditions::CS_OPTIONS_SCREEN] = new sad::KeyHoldCondition(sad::KeyLeft);
    LeftKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING_PRESSED] = new sad::KeyHoldCondition(sad::KeyLeft);
    if (is_inventory_thread)
    {
         LeftKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING_RELEASED] = NULL;
    }
    else
    {
        LeftKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING_RELEASED] = new sad::KeyHoldCondition(sad::KeyRight);
    }
    LeftKeyConditions[game::Conditions::CS_PLAYGAME_PAUSED] = NULL;

    // Unused on starting screen and on  paused screen
    RightKeyConditions[game::Conditions::CS_START_SCREEN] = NULL;
    RightKeyConditions[game::Conditions::CS_OPTIONS_SCREEN] = new sad::KeyHoldCondition(sad::KeyRight);
    RightKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING_PRESSED] = new sad::KeyHoldCondition(sad::KeyRight);
    if (is_inventory_thread)
    {
        RightKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING_RELEASED]= NULL;
    }
    else
    {
        RightKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING_RELEASED] = new sad::KeyHoldCondition(sad::KeyRight);
    }
    RightKeyConditions[game::Conditions::CS_PLAYGAME_PAUSED] = NULL;

    if (is_inventory_thread)
    {
        UpKeyConditions[game::Conditions::CS_START_SCREEN] = NULL;
    }
    else
    { 
        UpKeyConditions[game::Conditions::CS_START_SCREEN] = new sad::KeyHoldCondition(sad::KeyUp);
    }
    UpKeyConditions[game::Conditions::CS_OPTIONS_SCREEN] = new sad::KeyHoldCondition(sad::KeyUp);
    UpKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING_PRESSED] = new sad::KeyHoldCondition(sad::KeyUp);
    UpKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING_RELEASED] = NULL; 
    UpKeyConditions[game::Conditions::CS_PLAYGAME_PAUSED] = new sad::KeyHoldCondition(sad::KeyUp);


    if (is_inventory_thread)
    {
        DownKeyConditions[game::Conditions::CS_START_SCREEN] = NULL;
    }
    else
    {
        DownKeyConditions[game::Conditions::CS_START_SCREEN] = new sad::KeyHoldCondition(sad::KeyDown);
    }
    DownKeyConditions[game::Conditions::CS_OPTIONS_SCREEN] = new sad::KeyHoldCondition(sad::KeyDown);
    DownKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING_PRESSED] = new sad::KeyHoldCondition(sad::KeyDown);
    if (is_inventory_thread)
    {
        DownKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING_RELEASED] = NULL;
    }
    else
    { 
        DownKeyConditions[game::Conditions::CS_PLAYGAME_PLAYING_RELEASED] = new sad::KeyHoldCondition(sad::KeyDown);
    }
    DownKeyConditions[game::Conditions::CS_PLAYGAME_PAUSED] = new sad::KeyHoldCondition(sad::KeyDown);

    if (is_inventory_thread)
    {
        JumpActionConditions[game::Conditions::CS_START_SCREEN] = NULL;
    }
    else 
    {
        JumpActionConditions[game::Conditions::CS_START_SCREEN] = new sad::KeyHoldCondition(sad::Space);
    }

    JumpActionConditions[game::Conditions::CS_OPTIONS_SCREEN] = new sad::KeyHoldCondition(sad::Space);
    JumpActionConditions[game::Conditions::CS_PLAYGAME_PLAYING_PRESSED] = new sad::KeyHoldCondition(sad::Space);
    JumpActionConditions[game::Conditions::CS_PLAYGAME_PLAYING_RELEASED] = NULL;
    JumpActionConditions[game::Conditions::CS_PLAYGAME_PAUSED] = new sad::KeyHoldCondition(sad::Space);

    PauseCondition = new sad::KeyHoldCondition(sad::P);
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
}

void game::Conditions::ConditionsForRenderer::setKeyForConditions(sad::KeyHoldCondition* c[], sad::KeyboardKey key,size_t length)
{
    for(int i = 0; i < length; i++)
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