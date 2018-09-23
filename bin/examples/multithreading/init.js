makePlatformGoOnWay("MovingPlatform4", "Way1");
// Just a simple JS trigger
addTriggerOnce(400, function() {
	gamePrint("Player has reached point of " + player().middle().x  + "," + player().middle().y + " which is more than 400\n");
	/*player().tryStartGoingUp();
	spawnEnemyWalkerAt("player", new sad.Point2D(400, 500), function(actor, state) {
		if (actor.isResting()) {
			if (Math.random() * 4.0 > 3.8) {
				actor.tryStartGoingUp();
			}
			if (state["left"]) {
				if (actor.middle().x < 60) {
					return {"left": false};
				} else {
					actor.tryStopGoingRight();
					actor.tryStartGoingLeft();
					return state;
				}
			} else {
				if (actor.middle().x > 500) {
					return {"left": true};
				} else {
					actor.tryStopGoingLeft();
					actor.tryStartGoingRight();
					return state;
				}
			}
		} else {
			return state;
		}
	}, {'left': true});*/
});