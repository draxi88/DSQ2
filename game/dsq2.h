//DSq2

//HEALTH
#define HEALTH_M_SOLDIER  50
#define HEALTH_M_SOLDIER_SS  60
#define HEALTH_M_SOLDIER_LIGHT  40
#define HEALTH_M_INFANTRY  150
#define HEALTH_M_GUNNER  200
#define HEALTH_M_GLADIATOR  400


//DMG
#define DMG_BLASTER		ent->client->pers.dmg_blaster
#define DMG_SHOTGUN		ent->client->pers.dmg_shotgun
#define DMG_SSHOTGUN	ent->client->pers.dmg_sshotgun
#define DMG_BULLET		ent->client->pers.dmg_bullet
#define DMG_GRENADE		ent->client->pers.dmg_grenade
#define DMG_ROCKET		ent->client->pers.dmg_rocket
#define DMG_HYPERBLASTER ent->client->pers.dmg_hyperblaster
#define DMG_RAILGUN		ent->client->pers.dmg_railgun
#define DMG_BFG			ent->client->pers.dmg_bfg

//menu stuff
void MainMenuOpen(edict_t *ent);
void WeaponMenuOpen(edict_t *ent, pmenuhnd_t *p);
void DrinkHealth(edict_t *ent);
void UpgradeWeapon(edict_t *ent, pmenuhnd_t *hnd);