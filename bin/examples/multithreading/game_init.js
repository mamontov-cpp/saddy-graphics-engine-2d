// Init in-game state machines

// Init common state machine
state_machine.addState("starting_screen", new sad.hfsm.State());
state_machine.addState("options", new sad.hfsm.State());
state_machine.addState("playing", new sad.hfsm.State());
state_machine.addState("lose_screen", new sad.hfsm.State());
state_machine.addState("win_screen", new sad.hfsm.State());

state_machine.addTransition("starting_screen", "options", new sad.hfsm.Transition());
state_machine.addTransition("options", "starting_screen", new sad.hfsm.Transition());

state_machine.addTransition("starting_screen", "playing", new sad.hfsm.Transition());
state_machine.addTransition("playing", "starting_screen", new sad.hfsm.Transition());

state_machine.addTransition("playing", "lose_screen", new sad.hfsm.Transition());
state_machine.addTransition("lose_screen", "starting_screen", new sad.hfsm.Transition());

state_machine.addTransition("playing", "win_screen", new sad.hfsm.Transition());
state_machine.addTransition("win_screen", "starting_screen", new sad.hfsm.Transition());

state_machine.enterState("starting_screen");

// Init state machine for handling pause
paused_state_machine.addState("playing", new sad.hfsm.State());
// This third stated indicates, that we are transitioning between multiple scenes
paused_state_machine.addState("transitioning", new sad.hfsm.State());
paused_state_machine.addState("paused", new sad.hfsm.State());

paused_state_machine.addTransition("playing", "paused", new sad.hfsm.Transition());
paused_state_machine.addTransition("playing", "transitioning", new sad.hfsm.Transition());
paused_state_machine.addTransition("transitioning", "playing", new sad.hfsm.Transition());

paused_state_machine.addTransition("paused", "playing", new sad.hfsm.Transition());
paused_state_machine.addTransition("paused", "transitioning", new sad.hfsm.Transition());
paused_state_machine.addTransition("transitioning", "paused", new sad.hfsm.Transition());

paused_state_machine.enterState("playing");

// Init player options
var playerOptions = new ActorOptions();
playerOptions.IsFloater = false;
playerOptions.CanEmitSound = true;

playerOptions.WalkerHorizontalVelocity = 200;
playerOptions.WalkerVerticalVelocity = 400;

playerOptions.FloaterHorizontalVelocity = 200;
playerOptions.FloaterVerticalVelocity = 200;

playerOptions.WalkingAnimationOptions =  ["enemies_list/playerRed_walk1ng", "enemies_list/playerRed_walk2ng", "enemies_list/playerRed_walk3ng"];
playerOptions.WalkingAnimationTime = 200;

playerOptions.JumpingAnimationOptions =  ["enemies_list/playerRed_up1ng", "enemies_list/playerRed_up2ng"];
playerOptions.JumpingAnimationTime = 500;

playerOptions.StandingSprite = "enemies_list/playerRed_standng";
playerOptions.WalkingSprite = "enemies_list/playerRed_walk1ng";
playerOptions.WalkingSprite = "enemies_list/playerRed_walk1ng";
playerOptions.JumpingSprite = "enemies_list/playerRed_up2ng";
playerOptions.FallingSprite = "enemies_list/playerRed_fallng";
playerOptions.DuckingSprite = "enemies_list/playerRed_duckng";
playerOptions.FloaterSprite = "enemies_list/playerRed_rollng";

addActorOptions("player", playerOptions);

var animatedFloaterOptions = new ActorOptions();
animatedFloaterOptions.IsFloater = true;
animatedFloaterOptions.CanEmitSound = false;

animatedFloaterOptions.WalkerHorizontalVelocity = 200;
animatedFloaterOptions.WalkerVerticalVelocity = 400;

animatedFloaterOptions.FloaterHorizontalVelocity = 70;
animatedFloaterOptions.FloaterVerticalVelocity = 70;

animatedFloaterOptions.WalkingAnimationTime = 200;
animatedFloaterOptions.JumpingAnimationTime = 500;

animatedFloaterOptions.StandingSprite = "enemies_list/enemyFlying_2ng";
animatedFloaterOptions.WalkingSprite = "enemies_list/enemyFlying_2ng";
animatedFloaterOptions.WalkingSprite = "enemies_list/enemyFlying_2ng";
animatedFloaterOptions.JumpingSprite = "enemies_list/enemyFlying_2ng";
animatedFloaterOptions.FallingSprite = "enemies_list/enemyFlying_2ng";
animatedFloaterOptions.DuckingSprite = "enemies_list/enemyFlying_2ng";
animatedFloaterOptions.FloaterSprite = "enemies_list/enemyFlying_2ng";
animatedFloaterOptions.FloaterFlyAnimationOptions = ["enemies_list/enemyFlying_1ng", "enemies_list/enemyFlying_2ng"/*, "enemies_list/enemyFlying_3ng", "enemies_list/enemyFlying_4ng"*/];
animatedFloaterOptions.WalkingAnimationOptions = animatedFloaterOptions.FloaterFlyAnimationOptions;
animatedFloaterOptions.JumpingAnimationOptions =  animatedFloaterOptions.FloaterFlyAnimationOptions;

addActorOptions("animated_floater_1", animatedFloaterOptions);

animatedFloaterOptions = new ActorOptions();
animatedFloaterOptions.IsFloater = true;
animatedFloaterOptions.CanEmitSound = false;

animatedFloaterOptions.WalkerHorizontalVelocity = 200;
animatedFloaterOptions.WalkerVerticalVelocity = 400;

animatedFloaterOptions.FloaterHorizontalVelocity = 70;
animatedFloaterOptions.FloaterVerticalVelocity = 70;

animatedFloaterOptions.WalkingAnimationTime = 200;
animatedFloaterOptions.JumpingAnimationTime = 500;

animatedFloaterOptions.StandingSprite = "enemies_list/enemyFlyingAlt_2ng";
animatedFloaterOptions.WalkingSprite = "enemies_list/enemyFlyingAlt_2ng";
animatedFloaterOptions.WalkingSprite = "enemies_list/enemyFlyingAlt_2ng";
animatedFloaterOptions.JumpingSprite = "enemies_list/enemyFlyingAlt_2ng";
animatedFloaterOptions.FallingSprite = "enemies_list/enemyFlyingAlt_2ng";
animatedFloaterOptions.DuckingSprite = "enemies_list/enemyFlyingAlt_2ng";
animatedFloaterOptions.FloaterSprite = "enemies_list/enemyFlyingAlt_2ng";
animatedFloaterOptions.FloaterFlyAnimationOptions = ["enemies_list/enemyFlyingAlt_1ng", "enemies_list/enemyFlyingAlt_2ng"/*, "enemies_list/enemyFlyingAlt_3ng", "enemies_list/enemyFlyingAlt_4ng"*/];
animatedFloaterOptions.WalkingAnimationOptions = animatedFloaterOptions.FloaterFlyAnimationOptions;
animatedFloaterOptions.JumpingAnimationOptions =  animatedFloaterOptions.FloaterFlyAnimationOptions;

addActorOptions("animated_floater_2", animatedFloaterOptions);

var makeFloaterOptions = function(tile_name) {
    var animatedFloaterOptions = new ActorOptions();
    animatedFloaterOptions.IsFloater = true;
    animatedFloaterOptions.CanEmitSound = false;

    animatedFloaterOptions.WalkerHorizontalVelocity = 200;
    animatedFloaterOptions.WalkerVerticalVelocity = 400;

    animatedFloaterOptions.FloaterHorizontalVelocity = 20;
    animatedFloaterOptions.FloaterVerticalVelocity = 20;

    animatedFloaterOptions.WalkingAnimationTime = 200;
    animatedFloaterOptions.JumpingAnimationTime = 500;

    animatedFloaterOptions.StandingSprite = tile_name;
    animatedFloaterOptions.WalkingSprite = animatedFloaterOptions.StandingSprite;
    animatedFloaterOptions.WalkingSprite = animatedFloaterOptions.StandingSprite;
    animatedFloaterOptions.JumpingSprite = animatedFloaterOptions.StandingSprite;
    animatedFloaterOptions.FallingSprite = animatedFloaterOptions.StandingSprite;
    animatedFloaterOptions.DuckingSprite = animatedFloaterOptions.StandingSprite;
    animatedFloaterOptions.FloaterSprite = animatedFloaterOptions.StandingSprite;
    animatedFloaterOptions.FloaterFlyAnimationOptions = [tile_name];
    animatedFloaterOptions.WalkingAnimationOptions = animatedFloaterOptions.FloaterFlyAnimationOptions;
    animatedFloaterOptions.JumpingAnimationOptions =  animatedFloaterOptions.FloaterFlyAnimationOptions;
 
    return animatedFloaterOptions;
};


addActorOptions("red_disc", makeFloaterOptions("enemies_list/discRedng"));
addActorOptions("green_disc", makeFloaterOptions("enemies_list/discGreenng"));
addActorOptions("enemy_floater_4", makeFloaterOptions("enemies_list/enemyFloating_4ng"));
addActorOptions("enemy_floater_3", makeFloaterOptions("enemies_list/enemyFloating_3ng"));
addActorOptions("enemy_floater_2", makeFloaterOptions("enemies_list/enemyFloating_2ng"));
addActorOptions("enemy_floater_1", makeFloaterOptions("enemies_list/enemyFloating_1ng"));
addActorOptions("green_floater", makeFloaterOptions("enemies_list/playerGreen_rollng"));


var playerLikeOptions = new ActorOptions();
playerLikeOptions.IsFloater = false;
playerLikeOptions.CanEmitSound = false;

playerLikeOptions.WalkerHorizontalVelocity = 200;
playerLikeOptions.WalkerVerticalVelocity = 400;

playerLikeOptions.FloaterHorizontalVelocity = 200;
playerLikeOptions.FloaterVerticalVelocity = 200;

playerLikeOptions.WalkingAnimationOptions =  ["enemies_list/playerRed_walk1ng", "enemies_list/playerRed_walk2ng", "enemies_list/playerRed_walk3ng"];
playerLikeOptions.WalkingAnimationTime = 200;

playerLikeOptions.JumpingAnimationOptions =  ["enemies_list/playerRed_up1ng", "enemies_list/playerRed_up2ng"];
playerLikeOptions.JumpingAnimationTime = 500;

playerLikeOptions.StandingSprite = "enemies_list/playerRed_standng";
playerLikeOptions.WalkingSprite = "enemies_list/playerRed_walk1ng";
playerLikeOptions.WalkingSprite = "enemies_list/playerRed_walk1ng";
playerLikeOptions.JumpingSprite = "enemies_list/playerRed_up2ng";
playerLikeOptions.FallingSprite = "enemies_list/playerRed_fallng";
playerLikeOptions.DuckingSprite = "enemies_list/playerRed_duckng";
playerLikeOptions.FloaterSprite = "enemies_list/playerRed_rollng";

addActorOptions("player_like_enemy", playerLikeOptions);


var greenPlayerOptions = new ActorOptions();
greenPlayerOptions.IsFloater = false;
greenPlayerOptions.CanEmitSound = false;

greenPlayerOptions.WalkerHorizontalVelocity = 100;
greenPlayerOptions.WalkerVerticalVelocity = 300;

greenPlayerOptions.FloaterHorizontalVelocity = 100;
greenPlayerOptions.FloaterVerticalVelocity = 100;

greenPlayerOptions.WalkingAnimationOptions =  ["enemies_list/playerGreen_walk1ng", "enemies_list/playerGreen_walk2ng", "enemies_list/playerGreen_walk3ng"];
greenPlayerOptions.WalkingAnimationTime = 200;

greenPlayerOptions.JumpingAnimationOptions =  ["enemies_list/playerGreen_up1ng", "enemies_list/playerGreen_up2ng"];
greenPlayerOptions.JumpingAnimationTime = 500;

greenPlayerOptions.StandingSprite = "enemies_list/playerGreen_standng";
greenPlayerOptions.WalkingSprite = "enemies_list/playerGreen_walk1ng";
greenPlayerOptions.WalkingSprite = "enemies_list/playerGreen_walk1ng";
greenPlayerOptions.JumpingSprite = "enemies_list/playerGreen_up2ng";
greenPlayerOptions.FallingSprite = "enemies_list/playerGreen_fallng";
greenPlayerOptions.DuckingSprite = "enemies_list/playerGreen_duckng";
greenPlayerOptions.FloaterSprite = "enemies_list/playerGreen_rollng";

addActorOptions("green_walker", greenPlayerOptions);


var enemyOptions = new ActorOptions();
enemyOptions.IsFloater = false;
enemyOptions.CanEmitSound = false;

enemyOptions.WalkerHorizontalVelocity = 100;
enemyOptions.WalkerVerticalVelocity = 300;

enemyOptions.FloaterHorizontalVelocity = 100;
enemyOptions.FloaterVerticalVelocity = 100;

enemyOptions.WalkingAnimationOptions =  ["enemies_list/enemyWalking_1ng", "enemies_list/enemyWalking_2ng", "enemies_list/enemyWalking_3ng"];
enemyOptions.WalkingAnimationTime = 200;

enemyOptions.JumpingAnimationOptions =  ["enemies_list/enemyWalking_1ng", "enemies_list/enemyWalking_2ng"];
enemyOptions.JumpingAnimationTime = 500;

enemyOptions.StandingSprite = "enemies_list/enemyWalking_1ng";
enemyOptions.WalkingSprite = "enemies_list/enemyWalking_1ng";
enemyOptions.JumpingSprite = "enemies_list/enemyWalking_2ng";
enemyOptions.FallingSprite = "enemies_list/enemyWalking_2ng";
enemyOptions.DuckingSprite = "enemies_list/enemySpikey_2ng";
enemyOptions.FloaterSprite = "enemies_list/enemyFloating_2ng";

addActorOptions("enemy_walker", enemyOptions);

/** Typesafe version of adding item into player's inventory
 *  @param {Object} item An object. Must contain three fields - "icon", "name", "description"
 *  @return {boolean} whether item has been added into inventory
 */
function addItemToPlayerInventory(item) {
    if ((typeof item != "object") || (item === null)) {
        throw new Error("An item must be valid object");
    }
    if (!("icon" in item)) {
        throw new Error("An item must have a string \"icon\" property, which represents a name of icon resource in spritesheets. See icons.json in multithreading folder for a list of valid icons");
    }
    if (!("name" in item)) {
        throw new Error("An item must have a string \"name\" property, which represents a name of item. Note, that this name will be used when looking for item script.");
    }
    if (!("description" in item)) {
        throw new Error("An item must have a string \"description\" property, which represents a description of item.");
    }
    if (typeof (item["icon"])  !== "string") {
        throw new Error("An \"icon\" property must be a string. Note, that \"String\" objects are not supported.  See icons.json in multithreading folder for a list of valid icons.");
    }
    if (typeof (item["name"])  !== "string") {
        throw new Error("A \"name\" property must be a string. Note, that \"String\" objects are not supported.  This name will be used when looking for item script.");
    }
    if (typeof (item["description"])  !== "string") {
        throw new Error("A \"description\" property must be a string. Note, that \"String\" objects are not supported. ");
    }
    return _addItemToPlayerInventory(item["icon"], item["name"], item["description"]);
}

/** Tries to move item from ground into player's inventory. Called from Game::initPhysics callbacks and receives an item's actor
 *  To work properly, requires itemFactory function to be defined, which should return an object with props "icon", "name", "description"
 *  by string icon name for item. Also, if it returns null, item won't be taken.
 *  @param {Actor} icon item's icon from ground
 */
function tryMoveItemFromGroundIntoPlayersInventory(actor) {
    try {
        var iconName = actor.sprite().optionsName();
        var errorMessage = null;
        if (typeof itemFactory == "function") {
            var item = itemFactory(iconName);
            if (item !== null) {
                if (addItemToPlayerInventory(item)) {
                    _sheduleKillActorByBody(actor);
                } else {
                    errorMessage = "Failed to add item to inventory";
                }
            } else {
                errorMessage = "Factory failed to create item";
            }
        } else {
            errorMessage = "No factory set, disabled picking item from ground";
        }
        if (errorMessage !== null) {
            print(errorMessage);
            _makeItemUnpickable(actor);
        }
    } catch(e) {
        print("Exception: " + e.message + ". Stacktrace: " + e.stack);
    }
}

