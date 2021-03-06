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
        settings.ScaleFactor = 1.2;
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
        item.activateGivenWeapon(actor);
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
        item.activateGivenWeapon(actor);
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
        item.activateGivenWeapon(actor);
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
        item.activateGivenWeapon(actor);
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
        item.activateGivenWeapon(actor);
    }
});

addItemDefinition({
    "icon" : "icons_list/S_Light02ng",
    "name" : "Light blob",
    "description": "Slowly fires fast light bullets.\nMakes 10 real damage, when\nequipped. Shoots very fast",
    "on_added" : function(item, actor)  {
        print("Started adding");
        print("Added " + item.title());
        //actor.incrementAttackModifier(1);
        var weapon = new Weapon();
        weapon.setShootingInterval(1000);
        weapon.setAmountOfProjectiles(5);
        weapon.setDelay(100);
        weapon.setBaseDamage(10);
        weapon.setMinAngleDelta(0);
        weapon.setMaxAngleDelta(0);
        var settings = new BulletSettings();
        settings.IconName = "bullets/yellow/xxx_huge";
        settings.Speed = 3000;
        settings.IsGhost = true;
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
        item.activateGivenWeapon(actor);
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
        item.activateGivenWeapon(actor);
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
        item.activateGivenWeapon(actor);
    }
});

// Lasers
addItemDefinition({
    "icon" : "icons_list/W_Mace011ng",
    "name" : "Laser pointer",
    "description": "Fires small laser.\nMakes no real damage, when\nequipped.\nShoots fast",
    "on_added" : function(item, actor)  {
        print("Started adding");
        print("Added " + item.title());
        var weapon = new Weapon();
        //weapon.setShootingInterval(100);
        weapon.setShootingInterval(1500);
        weapon.setAmountOfProjectiles(1);
        weapon.setDelay(1500);
        weapon.setBaseDamage(0);
        weapon.setMinAngleDelta(0);
        weapon.setMaxAngleDelta(0);
        var settings = new LaserSettings();
        settings.IconName = "lasers/laser1";
        settings.SpriteWidth = 14;
        settings.PhysicalWidth = 4;
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
        item.activateGivenWeapon(actor);
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
        weapon.setShootingInterval(1000);
        weapon.setAmountOfProjectiles(1);
        weapon.setDelay(0);
        weapon.setBaseDamage(0);
        weapon.setMinAngleDelta(0);
        weapon.setMaxAngleDelta(0);

        var settings = new LaserSettings();
        settings.IconName = "lasers/laser2";
        settings.SpriteWidth = 40;
        settings.PhysicalWidth = 40;
        settings.Height = 1000;
        settings.Time = 800;
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
        item.activateGivenWeapon(actor);
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
        item.activateGivenWeapon(actor);
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
        settings.SpriteWidth = 2;
        settings.PhysicalWidth = 2;
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
        item.activateGivenWeapon(actor);
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

setMaxLevelX(13030);
setRightBound(13030);
resetEnemyCounter();

var playEnemySpawnSound = function() {
    playSound("item_drop");
};

var easyMode = false;

if (easyMode) {
    player().setLives(15);
} else {
    player().setLives(6);
}

var isWindEnabled = true;
var notFoundActors = [ null, null, null, null, null ];

// Just a simple JS trigger

addTriggerOnce(200, function() {
    //setWindSpeed(-20);
    //setWindSpeed(60);
    //snow.start();
    //spawnItem("Book of Flight", new sad.Point2D(400, 500));
    //spawnItem("Red potion", new sad.Point2D(350, 500));
    if (easyMode) {
        spawnItem("Wand of death", new sad.Point2D(400, 500));
    } else {
        spawnItem("Simple spear", new sad.Point2D(400, 500));
    }
    //cameraMovement().showArrow();
    //setGlobalOffset(new sad.Point2D(-50, 0));
});

if (!easyMode) {

addTriggerOnce(400, function() {
    //setWindSpeed(0);
    //snow.stop();

    lockScreen();
    setEnemyCounter(2);
    onZeroEnemies(function() { unlockScreen(); spawnItem("Red potion", new sad.Point2D(350, 500)); spawnItem("Red potion", new sad.Point2D(450, 500)); });
    playEnemySpawnSound();

    //player().tryStartGoingUp();
    //spawnEnemyWalkerAt("player", new sad.Point2D(400, 500), "random_60_500");
    //spawnAnimatedFloater("animated_floater_1", new sad.Point2D(500, 500), 50, 750, 40);
    //spawnAnimatedFloater("animated_floater_2", new sad.Point2D(600, 500), 50, 750, 40);
    //spawnEnemyInDirection("red_disc", new sad.Point2D(500, 500), HDir.Right, VDir.Down);
    //spawnEnemyInDirection("green_floater", new sad.Point2D(600, 500), HDir.Right, VDir.Down);
    
    var actor = spawnEnemyInDirection("green_walker", new sad.Point2D(200, 300), HDir.Right, VDir.None);
    decrementCounterOnActorDeath(actor);

    
    var actor = spawnPlatformPatrol("enemy_walker", new sad.Point2D(220, 300));
    decrementCounterOnActorDeath(actor);

    setLootForActor(actor, {"Helmet": 50, "Red potion": 33});
});

addTriggerOnce(1072, function() {
    startPlayingCameraLockAnimation(-635.0, 1400.0);
    setEnemyCounter(2);
    onZeroEnemies(function() { unlockScreen();  spawnItem("Laser pointer", new sad.Point2D(1285, 285)); });
    
    playEnemySpawnSound();
    var actor = spawnPlatformPatrol("enemy_walker", new sad.Point2D(964, 300));
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
    
    //var strategy = new FixedAngleStrategy(Math.PI);
    //var strategy = new TurningStrategy(2.0);
    //var strategy = new PlayerLocatingStrategy();
    
    var strategy = new PlayerLocatingStrategy();
    strategy.setInterval(200);
    
    actor.setShootingStrategy(strategy); 
    
    actor = spawnPlatformPatrol("enemy_walker", new sad.Point2D(1195, 300));
    setLootForActor(actor, {"Gold coin" : 50, "Red potion": 50});
    decrementCounterOnActorDeath(actor);
    weapon = new Weapon();
    weapon.setShootingInterval(2000);
    weapon.setAmountOfProjectiles(1);
    weapon.setBaseDamage(1);
    weapon.setSettings(settings);
    actor.pushWeapon(weapon);
    var strategy = new PlayerLocatingStrategy();
    strategy.setInterval(200);
    
    actor.setShootingStrategy(strategy); 
});

addTriggerOnce(1762, function() {
    startPlayingCameraLockAnimation(-1357.0, 1400.0);
    setEnemyCounter(6);
    onZeroEnemies(function() { unlockScreen(); });
    addDelayedTask(1500.0, function() {
        shakeCamera();
        addDelayedTask(500.0, function() {
            removePlatform("Floor5");
            removePlatform("Floor72");
        });
        addDelayedTask(600.0, function() {
            playEnemySpawnSound();
            
            var settings = new BulletSettings();
            settings.IconName = "bullets/green/xx_huge";
            settings.MaxBounceCount = 2;
            settings.Speed = 150;
            settings.ApplyGravity = false;
            settings.RestitutionCoefficient = 0.9;
    
            var actors = [
                spawnFollowPlayerFloater("animated_floater_1", new sad.Point2D(1558, 432)),
                spawnFollowPlayerFloater("animated_floater_1", new sad.Point2D(1754, 432))
            ];
            for(var i = 0; i < actors.length; i++)
            {
                var a = actors[i];
                a.setLives(2);
                decrementCounterOnActorDeath(a);
                
                weapon = new Weapon();
                weapon.setShootingInterval(3000);
                weapon.setAmountOfProjectiles(1);
                weapon.setBaseDamage(1);
                weapon.setSettings(settings);
                a.pushWeapon(weapon);
                
                var strategy = new RandomStrategy();
                strategy.setInterval(3000);
                a.setShootingStrategy(strategy); 
            }
            setLootForActor(actors[0], {"Gold coin" : 50, "Red potion": 50});
            setLootForActor(actors[1], {"Helmet" : 50, "Gauntlets": 50});
        });
        addDelayedTask(700.0, function() {
            playEnemySpawnSound();
            
            var settings = new BulletSettings();
            settings.IconName = "bullets/red/xx_huge";
            settings.Speed = 150;
            settings.MaxBounceCount = 2;
            settings.ApplyGravity = false;
            settings.RestitutionCoefficient = 0.9;
    
            var actors = [
                spawnFollowPlayerFloater("animated_floater_1", new sad.Point2D(1558, 456)),
                spawnFollowPlayerFloater("animated_floater_1", new sad.Point2D(1754, 456))
            ];
            for(var i = 0; i < actors.length; i++)
            {
                var a = actors[i];
                a.setLives(2);
                decrementCounterOnActorDeath(a);
                
                weapon = new Weapon();
                weapon.setShootingInterval(3000);
                weapon.setAmountOfProjectiles(1);
                weapon.setBaseDamage(1);
                weapon.setSettings(settings);
                a.pushWeapon(weapon);
                
                var strategy = new RandomStrategy();
                strategy.setInterval(3000);
                a.setShootingStrategy(strategy); 
            }
            setLootForActor(actors[0], {"Gold coin" : 75, "Book of Flight": 25});
            setLootForActor(actors[1], {"Crystal sword" : 70, "Large axe": 20, "Mace of Destruction": 10});
        });
        addDelayedTask(800.0, function() {
            playEnemySpawnSound();
            
            var settings = new BulletSettings();
            settings.IconName = "bullets/red/xx_huge";
            settings.Speed = 150;
            settings.MaxBounceCount = 2;
            settings.ApplyGravity = false;
            settings.RestitutionCoefficient = 0.9;
    
            var actors = [
                spawnFollowPlayerFloater("animated_floater_1", new sad.Point2D(1400, 321))
            ];
            for(var i = 0; i < actors.length; i++)
            {
                var a = actors[i];
                a.setLives(2);
                decrementCounterOnActorDeath(a);
                
                weapon = new Weapon();
                weapon.setShootingInterval(3000);
                weapon.setAmountOfProjectiles(1);
                weapon.setBaseDamage(1);
                weapon.setSettings(settings);
                a.pushWeapon(weapon);
                
                var strategy = new RandomStrategy();
                strategy.setInterval(3000);
                a.setShootingStrategy(strategy); 
            }
            setLootForActor(actors[0], {"Book of Flight": 100});
        });
        addDelayedTask(900.0, function() {
            playEnemySpawnSound();

            var settings = new BulletSettings();
            settings.IconName = "bullets/red/xx_huge";
            settings.Speed = 150;
            settings.MaxBounceCount = 2;
            settings.ApplyGravity = false;
            settings.RestitutionCoefficient = 0.9;
    
            var actors = [
                spawnFollowPlayerFloater("animated_floater_1", new sad.Point2D(2134, 321))
            ];
            for(var i = 0; i < actors.length; i++)
            {
                var a = actors[i];
                a.setLives(2);
                decrementCounterOnActorDeath(a);
                
                weapon = new Weapon();
                weapon.setShootingInterval(3000);
                weapon.setAmountOfProjectiles(1);
                weapon.setBaseDamage(1);
                weapon.setSettings(settings);
                a.pushWeapon(weapon);
                
                var strategy = new RandomStrategy();
                strategy.setInterval(3000);
                a.setShootingStrategy(strategy); 
            }
        });
    });;
});

}

addTriggerOnce(2294, function() {
    startPlayingCameraLockAnimation(-2286.0, 500.0);
    setEnemyCounter(2);
    onZeroEnemies(function() { unlockScreen(); spawnItem("Blade of fire", new sad.Point2D(2609, 161));  spawnItem("Red potion", new sad.Point2D(2702, 161)); });
    playEnemySpawnSound();

    var a = spawnWayFloater("animated_floater_1", new sad.Point2D(2465, 16), 0,  "Way2");
    decrementCounterOnActorDeath(a);
    var settings = new BulletSettings();
    settings.IconName = "bullets/green/xx_huge";
    settings.Speed = 150;
    settings.MaxBounceCount = 0;
    settings.ApplyGravity = false;
    settings.RestitutionCoefficient = 0.9;
    
    var strategy = new PlayerLocatingStrategy();
    strategy.setInterval(3000);
    a.setShootingStrategy(strategy);
    
    var weapon = new Weapon();
    weapon.setShootingInterval(2000);
    weapon.setAmountOfProjectiles(3);
    weapon.setBaseDamage(1);
    weapon.setDelay(500);
    weapon.setSettings(settings);
    a.pushWeapon(weapon);
    
    a = spawnWayFloater("animated_floater_1", new sad.Point2D(2837, 306), 0,  "Way3");
    decrementCounterOnActorDeath(a);
    strategy = new PlayerLocatingStrategy();
    strategy.setInterval(3000);
    a.setShootingStrategy(strategy);
    
    weapon = new Weapon();
    weapon.setShootingInterval(2000);
    weapon.setAmountOfProjectiles(3);
    weapon.setBaseDamage(1);
    weapon.setDelay(500);
    weapon.setSettings(settings);
    a.pushWeapon(weapon);
});

addTriggerOnce(3152, function() {
    startPlayingCameraLockAnimation(-3116.0, 500.0);
    setEnemyCounter(6);
    onZeroEnemies(function() { unlockScreen(); });
    playEnemySpawnSound();
    if (Math.random() > 0.75) {
        spawnItem("Phandaal\'s Excellent Prismatic Spray", new sad.Point2D(3513, 390));
    }


    var c = 10000.0 / 6.0;
    var settings = new BulletSettings();
    settings.IconName = "bullets/green/xx_huge";
    settings.Speed = 50;
    settings.MaxBounceCount = 0;
    settings.ApplyGravity = false;
    settings.RestitutionCoefficient = 0.9;

    for (var i = 0; i < 6; i++)
    {
        var a = spawnWayFloater("animated_floater_1", new sad.Point2D(3592, 294), c * i,  "Way4");
        decrementCounterOnActorDeath(a);
        
        var strategy = new PlayerLocatingStrategy();
        strategy.setInterval(2000);
        a.setShootingStrategy(strategy);
        a.setLives(9);
        
        var weapon = new Weapon();
        weapon.setShootingInterval(4000);
        weapon.setAmountOfProjectiles(3);
        weapon.setBaseDamage(2);
        weapon.setDelay(500);
        weapon.setSettings(settings);
        a.pushWeapon(weapon);
    }
});

addTriggerOnce(4100, function() {
    playEnemySpawnSound();
    var settings = new BulletSettings();
    settings.IconName = "bullets/blue/xx_huge";
    settings.Speed = 150;
    settings.MaxBounceCount = 0;
    settings.ApplyGravity = false;
    settings.RestitutionCoefficient = 0.9;
    
    var a = spawnEnemyInDirection("green_walker", new sad.Point2D(4460, 236), HDir.Left, VDir.None);
    setLootForActor(a, {"Red potion": 100});
    var strategy = new PlayerLocatingStrategy();
    strategy.setInterval(2000);
    a.setShootingStrategy(strategy);
    a.setLives(6);
    
    var weapon = new Weapon();
    weapon.setShootingInterval(3000);
    weapon.setAmountOfProjectiles(3);
    weapon.setBaseDamage(2);
    weapon.setDelay(500);
    weapon.setSettings(settings);
    a.pushWeapon(weapon);
    
    a = spawnEnemyInDirection("green_walker", new sad.Point2D(4583, 300), HDir.Left, VDir.None);
    setLootForActor(a, {"Red potion": 100});
    strategy = new PlayerLocatingStrategy();
    strategy.setInterval(2000);
    a.setShootingStrategy(strategy);
    a.setLives(6);
    
    weapon = new Weapon();
    weapon.setShootingInterval(3000);
    weapon.setAmountOfProjectiles(3);
    weapon.setBaseDamage(2);
    weapon.setDelay(500);
    weapon.setSettings(settings);
    a.pushWeapon(weapon);
    
    a = spawnEnemyInDirection("green_walker", new sad.Point2D(4750, 410), HDir.Left, VDir.None);
    setLootForActor(a, {"Red potion": 100});
    strategy = new PlayerLocatingStrategy();
    strategy.setInterval(2000);
    a.setShootingStrategy(strategy);
    a.setLives(6);
    
    weapon = new Weapon();
    weapon.setShootingInterval(3000);
    weapon.setAmountOfProjectiles(3);
    weapon.setBaseDamage(2);
    weapon.setDelay(500);
    weapon.setSettings(settings);
    a.pushWeapon(weapon);
});


addTriggerOnce(5062, function() {
    playEnemySpawnSound();
    makePlatformGoOnWay("MovingPlatform5", "LongWay1");
    addDelayedTask(50000, function() { stopMovingPlatformOnWay("MovingPlatform5"); });
    var a = spawnWayFloater("animated_floater_1", new sad.Point2D(5308, 29), 0,  "Way5");
    a.setLives(15);
    notFoundActors[0] = a;
    addOnActorDeathAction(a, function() { notFoundActors[0] = null; });
    
    a = spawnWayFloater("animated_floater_1", new sad.Point2D(5476, 29), 0,  "Way6");
    a.setLives(15);
    notFoundActors[1] = a;
    addOnActorDeathAction(a, function() { notFoundActors[1] = null; });

    a = spawnWayFloater("animated_floater_1", new sad.Point2D(5622, 29), 0,  "Way7");
    a.setLives(15);
    notFoundActors[2] = a;
    addOnActorDeathAction(a, function() { notFoundActors[2] = null; });
});

addTriggerOnce(5800, function() {
    playEnemySpawnSound();

    var settings = new BulletSettings();
    settings.IconName = "bullets/yellow/xxx_huge";
    settings.Speed = 500;
    settings.MaxBounceCount = 3;
    settings.ApplyGravity = true;
    settings.RestitutionCoefficient = 1.1;

    var a = spawnPlatformPatrol("green_walker", new sad.Point2D(5997, 275));
    setLootForActor(a, {"Light blob": 50, "Advanced fire spell": 50});
    var weapon = new Weapon();
    weapon.setShootingInterval(1000);
    weapon.setAmountOfProjectiles(1);
    weapon.setBaseDamage(3);
    weapon.setDelay(500);
    weapon.setSettings(settings);
    a.pushWeapon(weapon);
    
    var strategy = new PlayerLocatingStrategy();
    strategy.setInterval(3000);
    a.setShootingStrategy(strategy);
    a.setLives(15);
    notFoundActors[3] = a;
    addOnActorDeathAction(a, function() { notFoundActors[3] = null; });
    
    
    a = spawnPlatformPatrol("green_walker", new sad.Point2D(6382, 275));
    setLootForActor(a, {"Light blob": 50, "Advanced fire spell": 50});
    strategy = new PlayerLocatingStrategy();
    strategy.setInterval(2000);
    
    weapon = new Weapon();
    weapon.setShootingInterval(1000);
    weapon.setAmountOfProjectiles(1);
    weapon.setBaseDamage(3);
    weapon.setDelay(500);
    weapon.setSettings(settings);
    a.pushWeapon(weapon);

    a.setShootingStrategy(strategy);
    a.setLives(15);
    notFoundActors[4] = a;
    addOnActorDeathAction(a, function() { notFoundActors[4] = null; });
});

addTriggerOnce(6570, function() {
    // No reason to remove those now
    //removePlatform("MovingPlatform5");
    //removePlatform("Floor28");
    //removePlatform("Floor29");
    for (var i = 0; i < 5; i++) {
        if (notFoundActors[i] != null) {
            _sheduleKillActorByBody(notFoundActors[i]);
            notFoundActors[i] = null;
        }
    }
});

var isWindEnabled = false;
var tryChangeWindSpeed = function() {
    if (!isWindEnabled) {
        return;
    }
    var val = Math.random() * 100;
    var speed = 0;
    if (val >= 0 && val < 20) {
        speed = -60;
    }
    if (val >= 20 && val < 40) {
        speed = -20;
    }
    if (val >= 40 && val < 60) {
        speed = 0;
    }
    if (val >= 60 && val < 80) {
        speed = 20;
    }
    if (val >= 80 && val <= 100) {
        speed = 60;
    }
    setWindSpeed(speed);
    addDelayedTask(4000.0, tryChangeWindSpeed);
}
var startWind = function() {
    isWindEnabled = true;
    snow.start();
    tryChangeWindSpeed();
};
var stopWind  = function() {
    isWindEnabled = false;
    snow.stop();
};

addTriggerOnce(7279, function() {
    startWind();
});

addTriggerOnce(8229, function() {
    if (!easyMode) {
        makePlatformGoOnWay("MovingPlatform6", "Way8");
        makePlatformGoOnWay("MovingPlatform7", "Way9");
        makePlatformGoOnWay("MovingPlatform8", "Way10");
        makePlatformGoOnWay("MovingPlatform9", "Way11");
        makePlatformGoOnWay("MovingPlatform10", "Way12");
        makePlatformGoOnWay("MovingPlatform11", "Way13");
    }
});

addTriggerOnce(9030, function() {
   if (!easyMode) {
        enablePlatformBlinking("MovingPlatform13", 3000);
        enablePlatformBlinking("MovingPlatform15", 3000);
        addDelayedTask(1000.0, function() {
            enablePlatformBlinking("MovingPlatform14", 10000);
            enablePlatformBlinking("MovingPlatform16", 10000);
        });
   }
});

addTriggerOnce(9830, function() {
    if (!easyMode) {
        stopMovingPlatformOnWay("MovingPlatform6");
        stopMovingPlatformOnWay("MovingPlatform7");
        stopMovingPlatformOnWay("MovingPlatform8");
        stopMovingPlatformOnWay("MovingPlatform9");
        stopMovingPlatformOnWay("MovingPlatform10");
        stopMovingPlatformOnWay("MovingPlatform11");
    }
});


var spawnPlatformPatrol2 = function(p) {
    actor = spawnPlatformPatrol("enemy_walker", p);
    actor.setLives(15);
    decrementCounterOnActorDeath(actor);
    
    var settings = new BulletSettings();
    settings.IconName = "bullets/green/xx_huge";
    settings.MaxBounceCount = 0;
    settings.ApplyGravity = false;
    settings.RestitutionCoefficient = 0.9;
    
    weapon = new Weapon();
    weapon.setShootingInterval(1000);
    weapon.setAmountOfProjectiles(1);
    weapon.setBaseDamage(1);
    weapon.setSettings(settings);
    actor.pushWeapon(weapon);
    var strategy = new PlayerLocatingStrategy();
    strategy.setInterval(200);
    
    actor.setShootingStrategy(strategy); 
};

addTriggerOnce(9890, function() {
    if (!easyMode) {
        disablePlatformBlinking("MovingPlatform13");
        disablePlatformBlinking("MovingPlatform14");
        disablePlatformBlinking("MovingPlatform15");
        disablePlatformBlinking("MovingPlatform16");
    }

    startPlayingCameraLockAnimation(-9830.0, 800.0);
    setEnemyCounter(3);
    onZeroEnemies(function() { 
        unlockScreen();
    });
    playEnemySpawnSound();
    spawnItem("Magic wand", new sad.Point2D(9930, 200));
    spawnPlatformPatrol2(new sad.Point2D(9952, 200));
    spawnPlatformPatrol2(new sad.Point2D(10245, 200));
    spawnPlatformPatrol2(new sad.Point2D(10551, 200));
});


var shouldRespawnEnemy1 = true;

var spawnPlatformPatrol3 = function(p, fn) {
    actor = spawnPlatformPatrol("green_walker", p);
    actor.setLives(17);
    addOnActorDeathAction(actor, fn);

    var settings = new BulletSettings();
    settings.IconName = "bullets/green/xx_huge";
    settings.MaxBounceCount = 0;
    settings.ApplyGravity = false;
    settings.RestitutionCoefficient = 0.9;
    
    weapon = new Weapon();
    weapon.setShootingInterval(1000);
    weapon.setAmountOfProjectiles(1);
    weapon.setBaseDamage(1);
    weapon.setSettings(settings);
    actor.pushWeapon(weapon);
    var strategy = new PlayerLocatingStrategy();
    strategy.setInterval(200);
    
    actor.setShootingStrategy(strategy);
    
    return actor;
};

var spawnFloater4 = function(p, fn) {
    actor = spawnFollowPlayerFloater("red_disc", p);
    actor.setLives(17);
    addOnActorDeathAction(actor, fn);

    var settings = new BulletSettings();
    settings.IconName = "bullets/green/xx_huge";
    settings.MaxBounceCount = 0;
    settings.ApplyGravity = false;
    settings.RestitutionCoefficient = 0.9;
    
    weapon = new Weapon();
    weapon.setShootingInterval(1000);
    weapon.setAmountOfProjectiles(1);
    weapon.setBaseDamage(1);
    weapon.setSettings(settings);
    actor.pushWeapon(weapon);
    var strategy = new PlayerLocatingStrategy();
    strategy.setInterval(200);
    
    actor.setShootingStrategy(strategy); 
    
    return actor;
};

var eternal_enemies1 = [null, null, null, null];

var onDeathSpecialActor1 = function() {
    eternal_enemies1[0] = null;
    if (shouldRespawnEnemy1) {
        playEnemySpawnSound();
        eternal_enemies1[0] = spawnPlatformPatrol3(new sad.Point2D(10755, 130), onDeathSpecialActor1);
    }
}

var onDeathSpecialActor2 = function() {
    eternal_enemies1[1] = null;

    if (shouldRespawnEnemy1) {
        playEnemySpawnSound();
        eternal_enemies1[1] = spawnPlatformPatrol3(new sad.Point2D(11194, 206), onDeathSpecialActor2);
    }
}

var onDeathSpecialActor3 = function() {
    eternal_enemies1[2] = null;

    if (shouldRespawnEnemy1) {
        playEnemySpawnSound();
        eternal_enemies1[2] = spawnFloater4(new sad.Point2D(10954, 585), onDeathSpecialActor3);
    }
}


var onDeathSpecialActor4 = function() {
    eternal_enemies1[3] = null;
    
    if (shouldRespawnEnemy1) {
        playEnemySpawnSound();
        eternal_enemies1[3] = spawnFloater4(new sad.Point2D(11365, 585), onDeathSpecialActor4);
    }
}

addTriggerOnce(10650, function() {
    stopWind();
    setWindSpeed(0);
    
    playEnemySpawnSound();
    eternal_enemies1[0] = spawnPlatformPatrol3(new sad.Point2D(10755, 130), onDeathSpecialActor1);
    eternal_enemies1[1] = spawnPlatformPatrol3(new sad.Point2D(11194, 206), onDeathSpecialActor2);
    eternal_enemies1[2] = spawnFloater4(new sad.Point2D(10954, 585), onDeathSpecialActor3);
    eternal_enemies1[3] = spawnFloater4(new sad.Point2D(11365, 585), onDeathSpecialActor4);
});

addTriggerOnce(11374, function() {
    shouldRespawnEnemy1 = false;
});

var eternal_enemies2 = [null, null, null];
var shouldRespawnEnemy2 = true;

var onDeathSpecialActor11 = function() {
    eternal_enemies2[0] = null;
    if (shouldRespawnEnemy2) {
        playEnemySpawnSound();
        eternal_enemies2[0] = spawnFloater4(new sad.Point2D(11531, 590), onDeathSpecialActor11);
    }
}

var onDeathSpecialActor12 = function() {
    eternal_enemies2[1] = null;

    if (shouldRespawnEnemy2) {
        playEnemySpawnSound();
        eternal_enemies2[1] = spawnFloater4(new sad.Point2D(12210, 416), onDeathSpecialActor12);
    }
}

var onDeathSpecialActor13 = function() {
    eternal_enemies2[2] = null;

    if (shouldRespawnEnemy2) {
        playEnemySpawnSound();
        eternal_enemies2[2] = spawnFloater4(new sad.Point2D(11527, 310), onDeathSpecialActor13);
    }
}


addTriggerOnce(11574, function() {
    shouldRespawnEnemy1 = false;
    for (var i = 0; i < 4; i++) {
        if (eternal_enemies1[i] != null) {
            _sheduleKillActorByBody(eternal_enemies1[i]);
            eternal_enemies1[i] = null;
        }
    }
    playEnemySpawnSound();
    eternal_enemies2[0] = spawnFloater4(new sad.Point2D(11531, 590), onDeathSpecialActor11);
    eternal_enemies2[1] = spawnFloater4(new sad.Point2D(12210, 416), onDeathSpecialActor12);
    eternal_enemies2[2] = spawnFloater4(new sad.Point2D(11527, 310), onDeathSpecialActor13);
});


addTriggerOnce(12000, function() {
    shouldRespawnEnemy2 = false;
});


addTriggerOnce(12363, function() {
    for (var i = 0; i < 3; i++) {
        if (eternal_enemies2[i] != null) {
            _sheduleKillActorByBody(eternal_enemies2[i]);
            eternal_enemies2[i] = null;
        }
    }
});

addTriggerOnce(12990, function() {
    triggerWinGame();
});

