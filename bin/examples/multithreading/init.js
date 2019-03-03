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


var synonyms = {
// Swords
    "Simple spear": "icons_list/W_Spear010ng",
    "Crystal sword": "icons_list/W_Spear014ng",
    "Large axe": "icons_list/W_Axe003ng",
    "Mace of Destruction": "icons_list/W_Mace004ng",
// Guns, magic
    "Blade of fire": "icons_list/S_Fire01ng",
    "Light blob": "icons_list/S_Light02ng",
    "Advanced fire spell": "icons_list/S_Fire03ng",
    "Advanced magic spell": "icons_list/S_Magic02ng",
// Tier 3 gun, should appear as last resort
    "Wand of death": "icons_list/W_Mace012ng",
// Laser
    "Laser pointer":  "icons_list/W_Mace011ng",
    "Magic wand":  "icons_list/W_Mace008ng",
    "Phandaal\'s Excellent Prismatic Spray": "icons_list/W_Mace014ng",
// Armour
    "Helmet": "icons_list/C_Elm04ng",
    "Gauntlets": "icons_list/W_Fist002ng",
    "Armour": "icons_list/A_Armour02ng",
    "Shoes": "icons_list/A_Shoes05ng",
// Potion
    "Red potion": "icons_list/P_Red01ng",
// Money
    "Gold coin": "icons_list/I_GoldCoinng",
    "An expensive necklace": "icons_list/Ac_Necklace03ng",
    "Medal": "icons_list/Ac_Medal04ng",
// Books
    "Book of Flight": "icons_list/I_Bookng",
    "Book of Permanent Flight": "icons_list/W_Book03ng"
};

var spawnItem = function(name, point) {
    if (name in synonyms) {
        return _spawnItem(synonyms[name], point);
    }
    return null;
};

var setLootForActor = function(actor, base_loot ) {
    var loot = {};
    for(var name in base_loot) {
        if (base_loot.hasOwnProperty(name)) {
            if (name in synonyms) {
                loot[synonyms[name]] = base_loot[name];
            }
        }
    }
    return _setLootForActor(actor, loot);
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

// Bullet stuff (Guns, Magic) 
addItemDefinition({
    "icon" : "icons_list/S_Fire01ng",
    "name" : "Blade of fire",
    "description": "Fires tiny fire bullets.\nMakes no real damage, when\nequipped and adds 1 passive\ndamage when in inventory.\nShoots very fast",
    "on_added" : function(item, actor)  {
        print("Started adding");
        print("Added " + item.title());
        actor.incrementAttackModifier(1);
        var weapon = new Weapon();
        weapon.setShootingInterval(100);
        weapon.setAmountOfProjectiles(1);
        weapon.setDelay(500);
        weapon.setBaseDamage(0);
        weapon.setMinAngleDelta(0);
        weapon.setMaxAngleDelta(0);
        var settings = new BulletSettings();
        settings.IconName = "bullets/red/normal";
        settings.Speed = 1000;
        settings.ApplyGravity = false;
        settings.SoundName = "shooting_1";
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
    "icon" : "icons_list/S_Light02ng",
    "name" : "Light blob",
    "description": "Slowly fires light bullets.\nMakes 10 real damage, when\nequipped. Shoots very fast",
    "on_added" : function(item, actor)  {
        print("Started adding");
        print("Added " + item.title());
        //actor.incrementAttackModifier(1);
        var weapon = new Weapon();
        weapon.setShootingInterval(500);
        weapon.setAmountOfProjectiles(3);
        weapon.setDelay(0);
        weapon.setBaseDamage(10);
        weapon.setMinAngleDelta(-Math.PI / 10);
        weapon.setMaxAngleDelta(Math.PI / 10);
        var settings = new BulletSettings();
        settings.IconName = "bullets/yellow/xxx_huge";
        settings.Speed = 100;
        settings.ApplyGravity = false;
        settings.SoundName = "shooting_1";
        weapon.setSettings(settings);
        item.setGivenWeapon(weapon);
        actor.pushWeapon(weapon);
    },
    "on_removed": function(item, actor) {
        //actor.decrementAttackModifier(1);
        item.removeGivenWeaponFrom(actor);
        print("Removed " + item.title());
    },
    "on_apply" : function(item, actor) {
        
    }
});

addItemDefinition({
    "icon" : "icons_list/S_Fire03ng",
    "name" : "Advanced fire spell",
    "description": "Fires large piercing fire\nbullets. Makes 2 real damage, when\nequipped and adds 3 passive\ndamage when in inventory.\nShoots very fast",
    "on_added" : function(item, actor)  {
        print("Started adding");
        print("Added " + item.title());
        actor.incrementAttackModifier(3);
        var weapon = new Weapon();
        weapon.setShootingInterval(300);
        weapon.setAmountOfProjectiles(1);
        weapon.setDelay(500);
        weapon.setBaseDamage(2);
        weapon.setMinAngleDelta(0);
        weapon.setMaxAngleDelta(0);
        var settings = new BulletSettings();
        settings.IconName = "bullets/red/x_huge";
        settings.Speed = 1000;
        settings.ApplyGravity = true;
        settings.IsPiercing = true;
        settings.IsGhost = true;
        settings.SoundName = "shooting_2";
        weapon.setSettings(settings);
        item.setGivenWeapon(weapon);
        actor.pushWeapon(weapon);
    },
    "on_removed": function(item, actor) {
        actor.decrementAttackModifier(3);
        item.removeGivenWeaponFrom(actor);
        print("Removed " + item.title());
    },
    "on_apply" : function(item, actor) {
        
    }
});

addItemDefinition({
    "icon" : "icons_list/S_Magic02ng",
    "name" : "Advanced magic spell",
    "description": "Fires several large piercing bullets.\nMakes 3 real damage, when\nequipped and adds 3 passive\ndamage when in inventory.\nShoots fast",
    "on_added" : function(item, actor)  {
        print("Started adding");
        print("Added " + item.title());
        actor.incrementAttackModifier(3);
        var weapon = new Weapon();
        weapon.setShootingInterval(1000);
        weapon.setAmountOfProjectiles(4);
        weapon.setDelay(0);
        weapon.setBaseDamage(3);
        weapon.setMinAngleDelta(-Math.PI / 30.0);
        weapon.setMaxAngleDelta(Math.PI/ 30.0);
        var settings = new BulletSettings();
        settings.IconName = "bullets/blue/huge";
        settings.Speed = 300;
        settings.ApplyGravity = true;
        settings.IsPiercing = true;
        settings.IsGhost = false;
        settings.MaxBounceCount = 3;
        settings.SoundName = "shooting_2";
        weapon.setSettings(settings);
        item.setGivenWeapon(weapon);
        actor.pushWeapon(weapon);
    },
    "on_removed": function(item, actor) {
        actor.decrementAttackModifier(3);
        item.removeGivenWeaponFrom(actor);
        print("Removed " + item.title());
    },
    "on_apply" : function(item, actor) {
        
    }
});

// Lasers
addItemDefinition({
    "icon" : "icons_list/W_Mace011ng",
    "name" : "Laser pointer",
    "description": "Fires small laser.\nMakes no real damage, when\nequipped and adds 2 passive\ndamage when in inventory.\nShoots very fast",
    "on_added" : function(item, actor)  {
        print("Started adding");
        print("Added " + item.title());
        actor.incrementAttackModifier(2);
        var weapon = new Weapon();
        weapon.setShootingInterval(100);
        weapon.setAmountOfProjectiles(1);
        weapon.setDelay(500);
        weapon.setBaseDamage(0);
        weapon.setMinAngleDelta(0);
        weapon.setMaxAngleDelta(0);
        var settings = new LaserSettings();
        settings.IconName = "bullets/blue/tiny_laser";
        settings.Width = 2;
        settings.Height = 1000;
        settings.Time = 200;
        settings.SoundName = "shooting_3";
        weapon.setSettings(settings);
        item.setGivenWeapon(weapon);
        actor.pushWeapon(weapon);
    },
    "on_removed": function(item, actor) {
        actor.decrementAttackModifier(2);
        item.removeGivenWeaponFrom(actor);
        print("Removed " + item.title());
    },
    "on_apply" : function(item, actor) {
        
    }
});

addItemDefinition({
    "icon" : "icons_list/W_Mace008ng",
    "name" : "Magic wand",
    "description": "Fires medium purple laser.\nMakes no real damage, when\nequipped and adds 3 passive\ndamage when in inventory.\nShoots very fast",
    "on_added" : function(item, actor)  {
        print("Started adding");
        print("Added " + item.title());
        actor.incrementAttackModifier(3);
        var weapon = new Weapon();
        weapon.setShootingInterval(400);
        weapon.setAmountOfProjectiles(1);
        weapon.setDelay(500);
        weapon.setBaseDamage(0);
        weapon.setMinAngleDelta(0);
        weapon.setMaxAngleDelta(0);
        var settings = new LaserSettings();
        settings.IconName = "bullets/bubble/5_laser";
        settings.Width = 8;
        settings.Height = 1000;
        settings.Time = 200;
        settings.SoundName = "shooting_3";
        weapon.setSettings(settings);
        item.setGivenWeapon(weapon);
        actor.pushWeapon(weapon);
    },
    "on_removed": function(item, actor) {
        actor.decrementAttackModifier(3);
        item.removeGivenWeaponFrom(actor);
        print("Removed " + item.title());
    },
    "on_apply" : function(item, actor) {
        
    }
});

// Exception: not really a laser but very powerful weapon
addItemDefinition({
    "icon" : "icons_list/W_Mace012ng",
    "name" : "Wand of death",
    "description": "Fires small fast projectiles.\nMakes 2 real damage, when\nequipped and adds 2 passive\ndamage when in inventory.\nShoots very fast",
    "on_added" : function(item, actor)  {
        print("Started adding");
        print("Added " + item.title());
        actor.incrementAttackModifier(2);
        var weapon = new Weapon();
        weapon.setShootingInterval(600);
        weapon.setAmountOfProjectiles(8);
        weapon.setDelay(50);
        weapon.setBaseDamage(2);
        weapon.setMinAngleDelta(0);
        weapon.setMaxAngleDelta(0);
        var settings = new BulletSettings();
        settings.IconName = "bullets/bubble/9";
        settings.Speed = 800;
        settings.ApplyGravity = false;
        settings.SoundName = "shooting_2";
        settings.Piercing = true;
        settings.MaxBounceCount = 1;
        weapon.setSettings(settings);
        item.setGivenWeapon(weapon);
        actor.pushWeapon(weapon);
    },
    "on_removed": function(item, actor) {
        actor.decrementAttackModifier(2);
        item.removeGivenWeaponFrom(actor);
        print("Removed " + item.title());
    },
    "on_apply" : function(item, actor) {
        
    }
});

// A final laser
addItemDefinition({
    "icon" : "icons_list/W_Mace014ng",
    "name" : "Phandaal\'s Excellent Prismatic Spray",
    "description": "Fires small laser.\nMakes 30 real damage.\nShoots only once.",
    "on_added" : function(item, actor)  {
        print("Started adding");
        print("Added " + item.title());
        var weapon = new Weapon();
        weapon.setShootingInterval(200);
        weapon.setAmountOfProjectiles(16);
        weapon.setDelay(0);
        weapon.setBaseDamage(30);
        weapon.setMinAngleDelta(0);
        weapon.setMaxAngleDelta(Math.PI * 2);
        weapon.toggleEraseAfterShoot(true);
        var settings = new LaserSettings();
        settings.IconName = "bullets/yellow/tiny";
        settings.Width = 2;
        settings.Height = 1000;
        settings.Time = 200;
        settings.SoundName = "shooting_3";
        weapon.setSettings(settings);
        item.setGivenWeapon(weapon);
        actor.pushWeapon(weapon);
    },
    "on_removed": function(item, actor) {
        item.removeGivenWeaponFrom(actor);
        print("Removed " + item.title());
    },
    "on_apply" : function(item, actor) {
        
    }
});

// An armor
addItemDefinition({
    "icon" : "icons_list/C_Elm04ng",
    "name" : "Helmet",
    "description": "Increments defense by 1",
    "on_added" : function(item, actor)  {
        print("Started adding");
        print("Added " + item.title());
        actor.incrementDefense(1);
    },
    "on_removed": function(item, actor) {
        actor.decrementDefense(1);
        print("Removed " + item.title());
    },
    "on_apply" : function(item, actor) {
        
    }
});

addItemDefinition({
    "icon" : "icons_list/W_Fist002ng",
    "name" : "Gauntlets",
    "description": "Increments defense by 1",
    "on_added" : function(item, actor)  {
        print("Started adding");
        print("Added " + item.title());
        actor.incrementDefense(1);
    },
    "on_removed": function(item, actor) {
        actor.decrementDefense(1);
        print("Removed " + item.title());
    },
    "on_apply" : function(item, actor) {
        
    }
});

addItemDefinition({
    "icon" : "icons_list/A_Armour02ng",
    "name" : "Armour",
    "description": "Increments defense by 2",
    "on_added" : function(item, actor)  {
        print("Started adding");
        print("Added " + item.title());
        actor.incrementDefense(1);
    },
    "on_removed": function(item, actor) {
        actor.decrementDefense(1);
        print("Removed " + item.title());
    },
    "on_apply" : function(item, actor) {
        
    }
});

addItemDefinition({
    "icon" : "icons_list/A_Shoes05ng",
    "name" : "Shoes",
    "description": "Increments defense by 1",
    "on_added" : function(item, actor)  {
        print("Started adding");
        print("Added " + item.title());
        actor.incrementDefense(1);
    },
    "on_removed": function(item, actor) {
        actor.decrementDefense(1);
        print("Removed " + item.title());
    },
    "on_apply" : function(item, actor) {
        
    }
});

// Potion
addItemDefinition({
    "icon" : "icons_list/P_Red01ng",
    "name" : "Red potion",
    "description": "Increments health by 1.\nTo apply, simply click on it.",
    "delete_after_apply": true,
    "on_added" : function(item, actor)  {
        print("Started adding");
        print("Added " + item.title());
    },
    "on_removed": function(item, actor) {
        print("Removed " + item.title());
    },
    "on_apply" : function(item, actor) {
        actor.incrementLives(1);
    }
});

// Money
addItemDefinition({
    "icon" : "icons_list/I_GoldCoinng",
    "name" : "Gold coin",
    "description": "Increments score by 5 when applied.\nTo apply, simply click on it.",
    "delete_after_apply": true,
    "on_added" : function(item, actor)  {
        print("Started adding");
        print("Added " + item.title());
    },
    "on_removed": function(item, actor) {
        print("Removed " + item.title());
    },
    "on_apply" : function(item, actor) {
        incrementScore(5);
    }
});

addItemDefinition({
    "icon" : "icons_list/Ac_Necklace03ng",
    "name" : "An expensive necklace",
    "description": "Increments score by 10 when applied.\nTo apply, simply click on it.",
    "delete_after_apply": true,
    "on_added" : function(item, actor)  {
        print("Started adding");
        print("Added " + item.title());
    },
    "on_removed": function(item, actor) {
        print("Removed " + item.title());
    },
    "on_apply" : function(item, actor) {
        incrementScore(10);
    }
});

addItemDefinition({
    "icon" : "icons_list/Ac_Medal04ng",
    "name" : "Medal",
    "description": "Increments score by 20 when applied.\nTo apply, simply click on it.",
    "delete_after_apply": true,
    "on_added" : function(item, actor)  {
        print("Started adding");
        print("Added " + item.title());
    },
    "on_removed": function(item, actor) {
        print("Removed " + item.title());
    },
    "on_apply" : function(item, actor) {
        incrementScore(20);
    }
});

// Books
addItemDefinition({
    "icon" : "icons_list/I_Bookng",
    "name" : "Book of Flight",
    "description": "Grants 30 seconds of flight.\nTo apply, simply click on it.",
    "delete_after_apply": true,
    "on_added" : function(item, actor)  {
        print("Started adding");
        print("Added " + item.title());
    },
    "on_removed": function(item, actor) {
        print("Removed " + item.title());
    },
    "on_apply" : function(item, actor) {
        actor.incrementFloaterStateCounter();
        decrementFloaterStateCounterDelayed(actor, 30000);
    }
});

addItemDefinition({
    "icon" : "icons_list/W_Book03ng",
    "name" : "Book of Permanent Flight",
    "description": "Grants ability to flight.",
    "on_added" : function(item, actor)  {
        print("Started adding");
        print("Added " + item.title());
        actor.incrementFloaterStateCounter();
    },
    "on_removed": function(item, actor) {
        print("Removed " + item.title());
        actor.decrementFloaterStateCounter();
    },
    "on_apply" : function(item, actor) {
    }
});


// Enum-ish direction
var HDir = { "Right": 0, "Left": 1, "None" : 2};
var VDir = { "Up": 0, "Down": 1,  "None": 2};

makePlatformGoOnWay("MovingPlatform4", "Way1");

var snow = snowParticles();
snow.setFallingSpeed(300);
snow.setSpeedupCoefficient(3.0);
snow.setSpawnParticleDelay(100);
snow.setSpawnParticleDelaySpan(50);
snow.setMinAmountOfParticlesSpawned(1);
snow.setMaxAmountOfParticlesSpawned(5);
snow.setParticleName("bullets/red/xx_huge");

cameraMovement().setMoveLeftBoundary(350);
cameraMovement().setMoveRightBoundary(450);
cameraMovement().setMaxShiftTime(2000);
cameraMovement().setArrowPosition(new sad.Point2D(775,300));

setMaxLevelX(3200);
setRightBound(3200);
resetEnemyCounter();

// Just a simple JS trigger

addTriggerOnce(200, function() {
    //setWindSpeed(-20);
    //setWindSpeed(60);
    //snow.start();
    //spawnItem("Book of Flight", new sad.Point2D(400, 500));
    //spawnItem("Red potion", new sad.Point2D(350, 500));
    spawnItem("Simple spear", new sad.Point2D(400, 500));
    //cameraMovement().showArrow();
    //setGlobalOffset(new sad.Point2D(-50, 0));
});

addTriggerOnce(400, function() {
    //setWindSpeed(0);
    //snow.stop();

    lockScreen();
    setEnemyCounter(2);
    onZeroEnemies(function() { unlockScreen(); });

    //player().tryStartGoingUp();
    //spawnEnemyWalkerAt("player", new sad.Point2D(400, 500), "random_60_500");
    //spawnAnimatedFloater("animated_floater_1", new sad.Point2D(500, 500), 50, 750, 40);
    //spawnAnimatedFloater("animated_floater_2", new sad.Point2D(600, 500), 50, 750, 40);
    /*
    spawnEnemyInDirection("red_disc", new sad.Point2D(500, 500), HDir.Right, VDir.Down);
    spawnEnemyInDirection("green_floater", new sad.Point2D(600, 500), HDir.Right, VDir.Down);
    */
    var actor = spawnEnemyInDirection("green_walker", new sad.Point2D(200, 300), HDir.Right, VDir.None);
    decrementCounterOnActorDeath(actor);

    
    var actor = spawnPlatformPatrol("enemy_walker", new sad.Point2D(220, 300));
    decrementCounterOnActorDeath(actor);

    setLootForActor(actor, {"Gold coin" : 34, "Helmet": 33, "Red potion": 33});
});

addTriggerOnce(810, function() {
    lockScreen();
    setEnemyCounter(2);
    onZeroEnemies(function() { unlockScreen(); });

    var actor = spawnPlatformPatrol("enemy_walker", new sad.Point2D(964, 275));
    setLootForActor(actor, {"Gold coin" : 50, "Red potion": 50});
    decrementCounterOnActorDeath(actor);

    var weapon = new Weapon();
    weapon.setShootingInterval(2000);
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
    var strategy = new PlayerLocatingStrategy();
    strategy.setInterval(200);
    
    actor.setShootingStrategy(strategy); 
    
    actor = spawnPlatformPatrol("enemy_walker", new sad.Point2D(1195, 275));
    setLootForActor(actor, {"Gold coin" : 50, "Red potion": 50});
    decrementCounterOnActorDeath(actor);
    actor.pushWeapon(weapon);
    var strategy = new PlayerLocatingStrategy();
    strategy.setInterval(200);
    
    actor.setShootingStrategy(strategy); 
});

addTriggerOnce(950, function() {
    //unlockScreen();
    //cameraMovement().showArrow();
    //triggerWinGame();
});

addTriggerOnce(3100, function() {
    triggerWinGame();
});

