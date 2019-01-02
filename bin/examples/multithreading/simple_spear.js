{
    "onAdded": function() {
        print("Added");
        player().incrementAttackModifier(1);
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
        player().pushWeapon(weapon);
    },
    "onRemoved": function() {
        print("Removed");
        player().decrementAttackModifier(1);
        player().popWeapon();
    },
    "apply": function() {
        
    }
}