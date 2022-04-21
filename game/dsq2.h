//DSq2
#define DS_VERSION "Beta v0.3"

//Weapons
#define BlasterLevel		2
#define ShotgunLevel		2
#define SupershotgunLevel	2
#define MachinegunLevel		2
#define ChaingunLevel		2
#define GrenadeLevel		5
#define GLauncherLevel		5
#define RocketLevel			5
#define HyperblasterLevel	2
#define RailgunLevel		5
#define BFGLevel			10

int xplevel[99];

//STATS PLAYER
#define HEALTH_PLAYER_LEVEL 20
#define HEALTH_PLAYER_START 100
#define STAMINA_PLAYER_LEVEL 5
#define STAMINA_PLAYER_START 5

//HEALTH MONSTERS
#define HEALTH_M_BERSERK 400
#define HEALTH_M_BOSS2 10000
#define HEALTH_M_JORG 15000
#define HEALTH_M_MAKRON 15000
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
#define DMG_MACHINEGUN	ent->client->pers.dmg_machinegun
#define DMG_CHAINGUN	ent->client->pers.dmg_chaingun
#define DMG_GRENADE		ent->client->pers.dmg_grenade
#define DMG_GLAUNCHER	ent->client->pers.dmg_glauncher
#define DMG_ROCKET		ent->client->pers.dmg_rocket
#define DMG_HYPERBLASTER ent->client->pers.dmg_hyperblaster
#define DMG_RAILGUN		ent->client->pers.dmg_railgun
#define DMG_BFG			ent->client->pers.dmg_bfg

#define START_BLASTER		10
#define START_SHOTGUN		4
#define START_SSHOTGUN		6
#define START_MACHINEGUN	8
#define START_CHAINGUN		8
#define START_GRENADE		120
#define START_GLAUNCHER		120
#define START_ROCKET		100
#define START_HYPERBLASTER	15
#define START_RAILGUN		100
#define START_BFG			200


#ifndef ENTITY_SET
#define ENTITY_SET

	typedef struct {
		vec3_t		origin;
		vec3_t		angles;
		int			spawnflags;
		char		classname[32];
		char		*targetname;
		char		*target;
	} entity_s;

	entity_s all_entities[MAX_EDICTS];
#endif // !ITEM_SET

char *HighAscii(char *str);

//menu stuff
void MainMenuOpen(edict_t *ent);
void WeaponMenuOpen(edict_t *ent, pmenuhnd_t *p);
void ArmorMenuOpen(edict_t *ent, pmenuhnd_t *p);
void DrinkHealth(edict_t *ent);
void UpdateHealth(edict_t *ent);
void UpdateStamina(edict_t *ent);
void UpgradeWeapon(edict_t *ent, pmenuhnd_t *hnd);

//Entities
int entCount;
void RemoveSouls(edict_t *ent);
void RespawnEntities(edict_t *ent);
void GetEntities();
void DeleteItems(edict_t *ent);

//spawn
void SP_misc_teleporter_dest(edict_t *self);
void ED_CallSpawn(edict_t *ent);
void FindBonfire(edict_t *ent);
void DS_Respawn(edict_t *ent);
void SpawnSouls();