makePlatformGoOnWay("MovingPlatform4", "Way1");
// Just a simple JS trigger
addTriggerOnce(400, function() {
	gamePrint("Player has reached point of " + player().middle().x  + "," + player().middle().y + " which is more than 400\n");
	player().tryStartGoingUp();
	spawnEnemyWalkerAt("player", new sad.Point2D(400, 500), function(actor, state) {
		if (actor.isResting()) {
			if (Math.random() > 2) {
				actor.tryStartGoingUp();
			}
			if (state["left"] == 1) {
				if (actor.middle().x < 60) {
					return {"left": 0};
				} else {
					actor.tryStopGoingRight();
					actor.tryStartGoingLeft();
					return state;
				}
			} else {
				if (actor.middle().x > 500) {
					return {"left": 1};
				} else {
					actor.tryStopGoingLeft();
					actor.tryStartGoingRight();
					return state;
				}
			}
		} else {
			return state;
		}
	}, {'left': 1});
});