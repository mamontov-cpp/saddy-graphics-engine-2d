// Init in-game state machines

// Init common state machine
state_machine.addState("starting_screen", new sad.hfsm.State());
state_machine.addState("options", new sad.hfsm.State());
state_machine.addState("playing", new sad.hfsm.State());

state_machine.addTransition("starting_screen", "options", new sad.hfsm.Transition());
state_machine.addTransition("options", "starting_screen", new sad.hfsm.Transition());

state_machine.addTransition("starting_screen", "playing", new sad.hfsm.Transition());
state_machine.addTransition("playing", "starting_screen", new sad.hfsm.Transition());

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

animatedFloaterOptions.FloaterHorizontalVelocity = 400;
animatedFloaterOptions.FloaterVerticalVelocity = 400;

animatedFloaterOptions.WalkingAnimationTime = 200;
animatedFloaterOptions.JumpingAnimationTime = 500;

animatedFloaterOptions.StandingSprite = "enemies_list/enemyFlying_2ng";
animatedFloaterOptions.WalkingSprite = "enemies_list/enemyFlying_2ng";
animatedFloaterOptions.WalkingSprite = "enemies_list/enemyFlying_2ng";
animatedFloaterOptions.JumpingSprite = "enemies_list/enemyFlying_2ng";
animatedFloaterOptions.FallingSprite = "enemies_list/enemyFlying_2ng";
animatedFloaterOptions.DuckingSprite = "enemies_list/enemyFlying_2ng";
animatedFloaterOptions.FloaterSprite = "enemies_list/enemyFlying_2ng";
animatedFloaterOptions.FloaterFlyAnimationOptions = ["enemies_list/enemyFlying_1ng", "enemies_list/enemyFlying_2ng", "enemies_list/enemyFlying_3ng", "enemies_list/enemyFlying_4ng"];
animatedFloaterOptions.WalkingAnimationOptions = animatedFloaterOptions.FloaterFlyAnimationOptions;
animatedFloaterOptions.JumpingAnimationOptions =  animatedFloaterOptions.FloaterFlyAnimationOptions;

addActorOptions("animated_floater_1", animatedFloaterOptions);

animatedFloaterOptions = new ActorOptions();
animatedFloaterOptions.IsFloater = true;
animatedFloaterOptions.CanEmitSound = false;

animatedFloaterOptions.WalkerHorizontalVelocity = 200;
animatedFloaterOptions.WalkerVerticalVelocity = 400;

animatedFloaterOptions.FloaterHorizontalVelocity = 400;
animatedFloaterOptions.FloaterVerticalVelocity = 400;

animatedFloaterOptions.WalkingAnimationTime = 200;
animatedFloaterOptions.JumpingAnimationTime = 500;

animatedFloaterOptions.StandingSprite = "enemies_list/enemyFlyingAlt_2ng";
animatedFloaterOptions.WalkingSprite = "enemies_list/enemyFlyingAlt_2ng";
animatedFloaterOptions.WalkingSprite = "enemies_list/enemyFlyingAlt_2ng";
animatedFloaterOptions.JumpingSprite = "enemies_list/enemyFlyingAlt_2ng";
animatedFloaterOptions.FallingSprite = "enemies_list/enemyFlyingAlt_2ng";
animatedFloaterOptions.DuckingSprite = "enemies_list/enemyFlyingAlt_2ng";
animatedFloaterOptions.FloaterSprite = "enemies_list/enemyFlyingAlt_2ng";
animatedFloaterOptions.FloaterFlyAnimationOptions = ["enemies_list/enemyFlyingAlt_1ng", "enemies_list/enemyFlyingAlt_2ng", "enemies_list/enemyFlyingAlt_3ng", "enemies_list/enemyFlyingAlt_4ng"];
animatedFloaterOptions.WalkingAnimationOptions = animatedFloaterOptions.FloaterFlyAnimationOptions;
animatedFloaterOptions.JumpingAnimationOptions =  animatedFloaterOptions.FloaterFlyAnimationOptions;


addActorOptions("animated_floater_2", animatedFloaterOptions);
