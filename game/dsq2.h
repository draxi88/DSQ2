//DSq2

//Weapons
#define WeaponLevel 1000

//STATS PLAYER
#define HEALTH_PLAYER 50
#define STAMINA_PLAYER 10

//HEALTH MONSTERS
#define HEALTH_M_BERSERK 400
#define HEALTH_M_BOSS2 10000
#define HEALTH_M_BOSS31 15000
#define HEALTH_M_BOSS32 15000
#define HEALTH_M_BRAIN 500
#define HEALTH_M_CHICK 400
#define HEALTH_M_FLIPPER 100
#define HEALTH_M_FLOAT 300
#define HEALTH_M_FLYER 300
#define HEALTH_M_GLADIATOR  800
#define HEALTH_M_GUNNER  400
#define HEALTH_M_HOVER 400
#define HEALTH_M_INSANE  150
#define HEALTH_M_INFANTRY  300
#define HEALTH_M_MEDIC 300
#define HEALTH_M_MUTANT 400
#define HEALTH_M_PARASITE 250
#define HEALTH_M_SOLDIER  100
#define HEALTH_M_SOLDIER_SS  120
#define HEALTH_M_SOLDIER_LIGHT  80
#define HEALTH_M_SUPERTANK 2000
#define HEALTH_M_TANK 1600



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
		int			spawnflags;
		char		classname[32];
		char		*targetname;
		char		*target;
	} monsters_s;

	monsters_s all_monsters[MAX_EDICTS];
#endif // !MONSTER_SET

//menu stuff
void MainMenuOpen(edict_t *ent);
void WeaponMenuOpen(edict_t *ent, pmenuhnd_t *p);
void ArmorMenuOpen(edict_t *ent, pmenuhnd_t *p);
void DrinkHealth(edict_t *ent);
void UpdateHealth(edict_t *ent);
void UpdateStamina(edict_t *ent);
void UpgradeWeapon(edict_t *ent, pmenuhnd_t *hnd);

//Monsters
void DS_Respawn(edict_t *ent);
int monsterCount;
void SpawnMonsters(edict_t *ent);
void GetMonsters();
void DeleteItems(edict_t *ent);