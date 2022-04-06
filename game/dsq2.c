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

void UpdateHealth(edict_t *ent) {
	if (ent->client->pers.souls >= (ent->client->pers.health_level*WeaponLevel) + WeaponLevel) {
		//upgrade
		ent->client->pers.max_health += HEALTH_PLAYER;
		ent->client->pers.souls -= (ent->client->pers.health_level*WeaponLevel) + WeaponLevel;
		ent->client->pers.health_level++;
	}
	else {
		gi.cprintf(ent, PRINT_HIGH, "Could not upgrade health. Not enough stroggpoints\n");
		return;
	}
	gi.cprintf(ent, PRINT_HIGH, "Health updated.\n");
	ent->health = ent->client->pers.max_health;
}

void UpdateStamina(edict_t *ent) {
	if (ent->client->pers.souls >= (ent->client->pers.stamina_level*WeaponLevel) + WeaponLevel) {
		//upgrade
		ent->client->pers.max_stamina += STAMINA_PLAYER;
		ent->client->pers.souls -= (ent->client->pers.stamina_level*WeaponLevel) + WeaponLevel;
		ent->client->pers.stamina_level++;
	}
	else {
		gi.cprintf(ent, PRINT_HIGH, "Could not upgrade stamina. Not enough stroggpoints\n");
		return;
	}
	gi.cprintf(ent, PRINT_HIGH, "Stamina updated.\n");
	ent->client->pers.stamina = ent->client->pers.max_stamina;
}

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

void DS_Respawn(edict_t *ent) {
	gitem_t *soul;
	edict_t *soul_ent;
	
	//remove all souls.
	for (int i = 0; i < globals.num_edicts; i++) {
		soul_ent = &g_edicts[i];
		if (!soul_ent->inuse)
			continue;
		if (!strstr(soul_ent->classname, "key_data_cd"))
			continue;
		G_FreeEdict(soul_ent);
		//
	}

	//drop soul
	soul = &itemlist[32];
	soul->count_width = ent->client->pers.souls;
	Drop_Item(ent, soul);
	ent->client->pers.souls = 0;

	PutClientInServer(ent);
	gi.dprintf("Respawn angles: %s\n", vtos(ent->s.angles));
	SpawnMonsters(ent);
	ent->s.event = EV_PLAYER_TELEPORT;
	// hold in place briefly
	ent->client->ps.pmove.pm_flags = PMF_TIME_TELEPORT;
	ent->client->ps.pmove.pm_time = 500;
	ent->client->respawn_time = level.time;
}