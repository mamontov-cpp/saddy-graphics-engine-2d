// Enum-ish direction
var HDir = { "Right": 0, "Left": 1, "None" : 2};
var VDir = { "Up": 0, "Down": 1,  "None": 2};

makePlatformGoOnWay("MovingPlatform4", "Way1");
// Just a simple JS trigger
addTriggerOnce(400, function() {
	gamePrint("Player has reached point of " + player().middle().x  + "," + player().middle().y + " which is more than 400\n");
	player().tryStartGoingUp();
	spawnEnemyWalkerAt("player", new sad.Point2D(400, 500), "random_60_500");
	//spawnAnimatedFloater("animated_floater_1", new sad.Point2D(500, 500), 50, 750, 40);
	//spawnAnimatedFloater("animated_floater_2", new sad.Point2D(600, 500), 50, 750, 40);
	spawnEnemyInDirection("red_disc", new sad.Point2D(500, 500), HDir.Right, VDir.Down);
	spawnEnemyInDirection("green_floater", new sad.Point2D(600, 500), HDir.Right, VDir.Down);
	spawnEnemyInDirection("green_walker", new sad.Point2D(200, 300), HDir.Right, VDir.None);
	spawnEnemyInDirection("enemy_walker", new sad.Point2D(100, 300), HDir.Right, VDir.None);
});