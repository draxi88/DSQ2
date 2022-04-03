//DSq2

//HEALTH
#define HEALTH_M_SOLDIER  100
#define HEALTH_M_SOLDIER_SS  120
#define HEALTH_M_SOLDIER_LIGHT  80
#define HEALTH_M_INFANTRY  300
#define HEALTH_M_GUNNER  400
#define HEALTH_M_GLADIATOR  800


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


#ifndef MONSTER_SET
#define MONSTER_SET

	typedef struct {
		vec3_t		origin;
		vec3_t		angles;
		char		classname[32];
	} monsters_s;

	monsters_s all_monsters[MAX_EDICTS];
#endif // !MONSTER_SET

//menu stuff
void MainMenuOpen(edict_t *ent);
void WeaponMenuOpen(edict_t *ent, pmenuhnd_t *p);
void DrinkHealth(edict_t *ent);
void UpgradeWeapon(edict_t *ent, pmenuhnd_t *hnd);


//Monsters
//void ED_CallSpawn(edict_t *ent);
int monsterCount;
void SpawnMonsters(edict_t *ent);
void GetMonsters(edict_t *ent);