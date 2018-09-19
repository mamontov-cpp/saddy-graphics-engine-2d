makePlatformGoOnWay("MovingPlatform4", "Way1");
// Just a simple JS trigger
addTriggerOnce(400, function() {
	print("Player has reached point of " + player().middle().x  + "," + player().middle().y + " which is more than 400\n");
	player().tryStartGoingUp();
});