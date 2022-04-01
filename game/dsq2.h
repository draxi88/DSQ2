//DSq2

//HEALTH
#define HEALTH_M_SOLDIER  100
#define HEALTH_M_SOLDIER_SS  120
#define HEALTH_M_SOLDIER_LIGHT  80
#define HEALTH_M_INFANTRY  300
#define HEALTH_M_GUNNER  400
#define HEALTH_M_GLADIATOR  800


//DMG
#define DMG_BLASTER		ent->client->resp.dmg_blaster
#define DMG_SHOTGUN		ent->client->resp.dmg_shotgun
#define DMG_SSHOTGUN	ent->client->resp.dmg_sshotgun
#define DMG_BULLET		ent->client->resp.dmg_bullet
#define DMG_GRENADE		ent->client->resp.dmg_grenade
#define DMG_ROCKET		ent->client->resp.dmg_rocket
#define DMG_HYPERBLASTER ent->client->resp.dmg_hyperblaster
#define DMG_RAILGUN		ent->client->resp.dmg_railgun
#define DMG_BFG			ent->client->resp.dmg_bfg

//menu stuff
void MainMenuOpen(edict_t *ent);
void WeaponMenuOpen(edict_t *ent, pmenuhnd_t *p);
void DrinkHealth(edict_t *ent);
void UpgradeWeapon(edict_t *ent, pmenuhnd_t *hnd);