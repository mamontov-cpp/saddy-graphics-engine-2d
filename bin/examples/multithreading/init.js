makePlatformGoOnWay("MovingPlatform4", "Way1");
// Just a simple JS trigger
addTriggerOnce(400, function() {
	gamePrint("Player has reached point of " + player().middle().x  + "," + player().middle().y + " which is more than 400\n");
	player().tryStartGoingUp();
	spawnEnemyWalkerAt("player", new sad.Point2D(400, 500), "random_60_500");
	spawnEnemyWalkerAt("player", new sad.Point2D(300, 500), "random_60_500");
	spawnEnemyWalkerAt("player", new sad.Point2D(200, 500), "random_60_500");
});