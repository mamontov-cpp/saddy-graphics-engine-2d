// Init penetration depth to earth for items
setItemPenetrationDepth("SWORD021", 3);
setItemPenetrationDepth("ICE", 3);
setItemPenetrationDepth("THUNDER", 3);
setItemPenetrationDepth("BOW", 4);
setItemPenetrationDepth("MACE", 4);
setItemPenetrationDepth("DAGGER", 4);
setItemPenetrationDepth("KEY", 4);
setItemPenetrationDepth("AXE", 4);
setItemPenetrationDepth("TORCH", 4);
setItemPenetrationDepth("GUN", 4);
setItemPenetrationDepth("THROW", 4);
setItemPenetrationDepth("EYE", 3);
setItemPenetrationDepth("BATWING", 4);
setItemPenetrationDepth("CARROT", 3);
setItemPenetrationDepth("SPEAR", 4);
setItemPenetrationDepth("SWORD", 4);

setDroppedItemIcon("icons_list/W_Spear010ng");


// Enum-ish direction
var HDir = { "Right": 0, "Left": 1, "None" : 2};
var VDir = { "Up": 0, "Down": 1,  "None": 2};

makePlatformGoOnWay("MovingPlatform4", "Way1");
// Just a simple JS trigger

addTriggerOnce(200, function() {
	gamePrint("Have a nice weapon!");
	var min_angle = Math.PI * (-0.25);
	var weapon = new Weapon();
	weapon.setShootingInterval(1000);
	weapon.setAmountOfProjectiles(6);
	weapon.setDelay(50);
	weapon.setBaseDamage(1);
	weapon.setMinAngleDelta(/*min_angle*/ 0);
	weapon.setMaxAngleDelta(/*min_angle * (-1)*/ 0);
	var settings = new BulletSettings();
	settings.IconName = "bullets/yellow/x_huge";
	settings.MaxBounceCount = 200;
	settings.ApplyGravity = false;
	settings.GravityValue = new sad.Point2D(0, -300);
	settings.RestitutionCoefficient = 0.9;
    settings.Speed = 300;
	weapon.setSettings(settings);
	player().setWeapon(weapon);
	gamePrint("Weapon is given");
});

addTriggerOnce(400, function() {
	gamePrint("Player has reached point of " + player().middle().x  + "," + player().middle().y + " which is more than 400\n");
	player().tryStartGoingUp();
	//spawnEnemyWalkerAt("player", new sad.Point2D(400, 500), "random_60_500");
	//spawnAnimatedFloater("animated_floater_1", new sad.Point2D(500, 500), 50, 750, 40);
	//spawnAnimatedFloater("animated_floater_2", new sad.Point2D(600, 500), 50, 750, 40);
	/*
	spawnEnemyInDirection("red_disc", new sad.Point2D(500, 500), HDir.Right, VDir.Down);
	spawnEnemyInDirection("green_floater", new sad.Point2D(600, 500), HDir.Right, VDir.Down);
	*/
	var actor = spawnEnemyInDirection("green_walker", new sad.Point2D(200, 300), HDir.Right, VDir.None);
	var weapon = new Weapon();
	weapon.setShootingInterval(100);
	weapon.setAmountOfProjectiles(1);
	weapon.setBaseDamage(1);
	var settings = new BulletSettings();
	settings.IconName = "bullets/yellow/x_huge";
	settings.MaxBounceCount = 0;
	settings.ApplyGravity = false;
	settings.RestitutionCoefficient = 0.9;
	weapon.setSettings(settings);
	actor.setWeapon(weapon);
	/*
	var strategy = new FixedAngleStrategy(Math.PI);
	var strategy = new TurningStrategy(2.0);
	var strategy = new PlayerLocatingStrategy();
	*/
	var strategy = new RandomStrategy();
	strategy.setInterval(100);
	
	actor.setShootingStrategy(strategy); 
	
	spawnPlatformPatrol("enemy_walker", new sad.Point2D(220, 300));
});

addTriggerOnce(750, function() {
	triggerWinGame();
});