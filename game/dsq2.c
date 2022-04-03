#include "g_local.h"
#include "dsq2.h"


void DrinkHealth(edict_t *ent) {
	if (!ent->client->resp.health_flask)
		return;
	ent->health += 25;
	if (ent->health > ent->client->pers.max_health)
		ent->health = ent->client->pers.max_health;
	ent->client->resp.health_flask--;
}

#define WeaponLevel 1000
void UpgradeWeapon(edict_t *ent, pmenuhnd_t *hnd) {
	gitem_t *it;
	int i;
	i = hnd->cur+5;
	while (i < 18) {
		if (ent->client->pers.inventory[i]) {
			it = &itemlist[i];
			if (ent->client->pers.souls >= (it->level*WeaponLevel) + WeaponLevel) {
				//upgrade
				ent->client->pers.souls -= (it->level*WeaponLevel) + WeaponLevel;
				it->level++;
				gi.cprintf(ent, PRINT_HIGH, "%s updated.\n", it->classname);
			}
			else {
				gi.cprintf(ent, PRINT_HIGH, "Could not upgrade this weapon. Not enough stroggpoints\n");
			}
			break;
		}
		i++;
	}
	//gi.cprintf(ent, PRINT_HIGH, "hnd->num:%i, hnd->cur:%i hnd->entries->text:%s\n",hnd->num, hnd->cur, hnd->entries->text);
	PMenu_Close(ent);
	//PMenu_Open(ent, weaponmenu, -1, sizeof(weaponmenu) / sizeof(pmenu_t), NULL);
}

void GetMonsters(edict_t *ent) {
	edict_t *monster;
	monsterCount = 0;
	for (int i = 0; i < globals.num_edicts; i++) {
		monster = &g_edicts[i];
		if (!monster->inuse)
			continue;
		if (!strstr(monster->classname, "monster_"))
			continue;
		strcpy(all_monsters[monsterCount].classname,monster->classname);
		VectorCopy(monster->s.origin, all_monsters[monsterCount].origin);
		VectorCopy(monster->s.angles, all_monsters[monsterCount].angles);
		//gi.dprintf("%s found at %s.\n", all_monsters[monsterCount].classname, vtos(all_monsters[monsterCount].origin));
		monsterCount++;
	}
}


void spawner(int i) {
	edict_t *newMonster;

	newMonster = G_Spawn();
	newMonster->classname = all_monsters[i].classname;
	VectorCopy(all_monsters[i].origin, newMonster->s.origin);
	VectorCopy(all_monsters[i].angles, newMonster->s.angles);
	ED_CallSpawn(newMonster);
	gi.linkentity(newMonster);
}

void SpawnMonsters(edict_t *ent) {
	edict_t *monster;

	//delete old monsters;
	for (int i = 0; i < globals.num_edicts; i++) {
		monster = &g_edicts[i];
		if (!monster->inuse)
			continue;
		if (!strstr(monster->classname, "monster_"))
			continue;
		G_FreeEdict(monster);
		//
	}

	for (int i = 0; i < monsterCount; i++) {
		if (!strstr(all_monsters[i].classname, "monster_")) //shouldn't be possible.. but..
			continue;
		spawner(i);
	}
}