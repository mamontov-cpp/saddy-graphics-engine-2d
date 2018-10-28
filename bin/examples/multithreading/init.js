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
	settings.MaxBounceCount = 0;
	settings.ApplyGravity = true;
	settings.GravityValue = new sad.Point2D(0, 300);
	settings.BounceResilienceCoefficient = 0.9;
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
	spawnEnemyInDirection("green_walker", new sad.Point2D(200, 300), HDir.Right, VDir.None);
	spawnPlatformPatrol("enemy_walker", new sad.Point2D(220, 300));
});

addTriggerOnce(550, function() {
	triggerWinGame();
});