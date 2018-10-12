makePlatformGoOnWay("MovingPlatform4", "Way1");
// Just a simple JS trigger
addTriggerOnce(400, function() {
	gamePrint("Player has reached point of " + player().middle().x  + "," + player().middle().y + " which is more than 400\n");
	player().tryStartGoingUp();
	spawnEnemyWalkerAt("player", new sad.Point2D(400, 500), "random_60_500");
	//spawnAnimatedFloater("animated_floater_1", new sad.Point2D(500, 500), 50, 750, 40);
	//spawnAnimatedFloater("animated_floater_2", new sad.Point2D(600, 500), 50, 750, 40);
	spawnFollowPlayerFloater("red_disc", new sad.Point2D(500, 500));
	spawnFollowPlayerFloater("green_floater", new sad.Point2D(600, 500));
	spawnFollowPlayerFloater("enemy_floater_1", new sad.Point2D(200, 500));
});