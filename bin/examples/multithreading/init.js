// Init penetration depth to earth for items
setItemPenetrationDepth("SWORD021", 3);
setItemPenetrationDepth("SPEAR010", 8);
setItemPenetrationDepth("SPEAR013", 8);
setItemPenetrationDepth("SPEAR014", 8);
setItemPenetrationDepth("MACE011", 8);
setItemPenetrationDepth("MACE008", 8);
setItemPenetrationDepth("CANNON", 6);
setItemPenetrationDepth("FIST", 3); 
setItemPenetrationDepth("ARMOUR", 9);
setItemPenetrationDepth("SHOES", 9); 
setItemPenetrationDepth("ICE", 3);
setItemPenetrationDepth("THUNDER", 3);
setItemPenetrationDepth("FIRE", 4);
setItemPenetrationDepth("LIGHT", 5);
setItemPenetrationDepth("MACE", 6);
setItemPenetrationDepth("DAGGER", 4);
setItemPenetrationDepth("KEY", 4);
setItemPenetrationDepth("AXE", 4);
setItemPenetrationDepth("TORCH", 4);
setItemPenetrationDepth("GUN", 4);
setItemPenetrationDepth("THROW", 4);
setItemPenetrationDepth("EYE", 3);
setItemPenetrationDepth("BATWING", 4);
setItemPenetrationDepth("CARROT", 3);
setItemPenetrationDepth("SPEAR", 8);
setItemPenetrationDepth("SWORD", 8);
setItemPenetrationDepth("BOOK", 9);

setDroppedItemIcon("icons_list/W_Mace004ng");

var synonyms = {
// Swords
    "Simple spear": "icons_list/W_Spear010ng",
    "Crystal sword": "icons_list/W_Spear014ng",
    "Large axe": "icons_list/W_Axe003ng",
    "Mace of Destruction": "icons_list/W_Mace004ng"
};

// Swords
addItemDefinition({
    "icon" : "icons_list/W_Spear010ng",
    "name" : "Simple spear",
    "description": "A simple spear combined with axe.\nMakes 1 damage, when equipped\nand adds 1 passive damage\nwhen in inventory. Also, last picked\nweapon item is equipped by\ndefault",
    "on_added" : function(item, actor)  {
        print("Started adding");
        print("Added " + item.title());
        actor.incrementAttackModifier(1);
        var weapon = new Weapon();
        weapon.setShootingInterval(200);
        weapon.setAmountOfProjectiles(1);
        weapon.setDelay(200);
        weapon.setBaseDamage(0);
        weapon.setMinAngleDelta(0);
        weapon.setMaxAngleDelta(0);
        var settings = new SwingSettings();
        settings.IconName = "icons_list/W_Spear010ng";
        settings.ScaleFactor = 2.0;
        settings.DecayTime = 200;
        settings.SoundName = "swing";
        weapon.setSettings(settings);
        item.setGivenWeapon(weapon);
        actor.pushWeapon(weapon);
    },
    "on_removed": function(item, actor) {
        actor.decrementAttackModifier(1);
        item.removeGivenWeaponFrom(actor);
        print("Removed " + item.title());
    },
    "on_apply" : function(item, actor) {
        
    }
});

addItemDefinition({
    "icon" : "icons_list/W_Spear014ng",
    "name" : "Crystal sword",
    "description": "A sword, made of crystal.\nMakes 1 real damage, when equipped\nand adds 1 passive damage\nwhen in inventory.",
    "on_added" : function(item, actor)  {
        print("Started adding");
        print("Added " + item.title());
        actor.incrementAttackModifier(1);
        var weapon = new Weapon();
        weapon.setShootingInterval(400);
        weapon.setAmountOfProjectiles(1);
        weapon.setDelay(500);
        weapon.setBaseDamage(1);
        weapon.setMinAngleDelta(0);
        weapon.setMaxAngleDelta(0);
        var settings = new SwingSettings();
        settings.IconName = "icons_list/W_Spear014ng";
        settings.ScaleFactor = 3.0;
        settings.DecayTime = 200;
        settings.SoundName = "swing";
        weapon.setSettings(settings);
        item.setGivenWeapon(weapon);
        actor.pushWeapon(weapon);
    },
    "on_removed": function(item, actor) {
        actor.decrementAttackModifier(1);
        item.removeGivenWeaponFrom(actor);
        print("Removed " + item.title());
    },
    "on_apply" : function(item, actor) {
        
    }
});



addItemDefinition({
    "icon" : "icons_list/W_Axe003ng",
    "name" : "Large axe",
    "description": "A large axe.\nMakes 10 real damage, when\nequipped and adds 1 passive\ndamage when in inventory.\nReally slow to cut something.",
    "on_added" : function(item, actor)  {
        print("Started adding");
        print("Added " + item.title());
        actor.incrementAttackModifier(1);
        var weapon = new Weapon();
        weapon.setShootingInterval(1000);
        weapon.setAmountOfProjectiles(1);
        weapon.setDelay(500);
        weapon.setBaseDamage(10);
        weapon.setMinAngleDelta(0);
        weapon.setMaxAngleDelta(0);
        var settings = new SwingSettings();
        settings.IconName = "icons_list/W_Axe003ng";
        settings.ScaleFactor = 3.0;
        settings.DecayTime = 800;
        settings.SoundName = "swing";
        weapon.setSettings(settings);
        item.setGivenWeapon(weapon);
        actor.pushWeapon(weapon);
    },
    "on_removed": function(item, actor) {
        actor.decrementAttackModifier(1);
        item.removeGivenWeaponFrom(actor);
        print("Removed " + item.title());
    },
    "on_apply" : function(item, actor) {
        
    }
});



addItemDefinition({
    "icon" : "icons_list/W_Mace004ng",
    "name" : "Mace of Destruction",
    "description": "A large mace.\nMakes 3 real damage, when\nequipped and adds 2 passive\ndamage when in inventory.\nFaster than axe,\nbut makes lesss damage.",
    "on_added" : function(item, actor)  {
        print("Started adding");
        print("Added " + item.title());
        actor.incrementAttackModifier(5);
        var weapon = new Weapon();
        weapon.setShootingInterval(500);
        weapon.setAmountOfProjectiles(1);
        weapon.setDelay(500);
        weapon.setBaseDamage(5);
        weapon.setMinAngleDelta(0);
        weapon.setMaxAngleDelta(0);
        var settings = new SwingSettings();
        settings.IconName = "icons_list/W_Mace004ng";
        settings.ScaleFactor = 4.5;
        settings.DecayTime = 800;
        settings.SoundName = "swing";
        weapon.setSettings(settings);
        item.setGivenWeapon(weapon);
        actor.pushWeapon(weapon);
    },
    "on_removed": function(item, actor) {
        actor.decrementAttackModifier(5);
        item.removeGivenWeaponFrom(actor);
        print("Removed " + item.title());
    },
    "on_apply" : function(item, actor) {
        
    }
});

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
    player().pushWeapon(weapon);
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
    actor.pushWeapon(weapon);
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