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

void DeleteItems(edict_t *ent) {
	edict_t *item;
	for (int i = 0; i < globals.num_edicts; i++) {
		item = &g_edicts[i];
		if (!item->inuse)
			continue;
		if (!strstr(item->classname, "armor_shard"))
			continue;
		//gi.dprintf("%s deleted.\n", item->classname);
		G_FreeEdict(item);
	}
}

void GetEntities() {
	edict_t *ent;
	entCount = 0;
	for (int i = 0; i < globals.num_edicts; i++) {
		ent = &g_edicts[i];
		if (!ent->inuse)
			continue;
		if (!strstr(ent->classname, "monster_"))
			continue;
		strcpy(all_entities[entCount].classname, ent->classname);
		VectorCopy(ent->s.origin, all_entities[entCount].origin);
		all_entities[entCount].origin[2] += 5; //don't spawn in ground.
		VectorCopy(ent->s.angles, all_entities[entCount].angles);
		all_entities[entCount].spawnflags = ent->spawnflags;
		all_entities[entCount].targetname = ent->targetname;
		all_entities[entCount].target = ent->target;
#ifdef DEBUG
		gi.dprintf("%-24s - sf: %i", monster->classname, monster->spawnflags);
		if (monster->target)
			gi.dprintf(" - target: %s", monster->target);
		if (monster->targetname)
			gi.dprintf(" - targetname: %s", monster->targetname);
		gi.dprintf("\n");
#endif
		entCount++;
	}
}


void spawner(int i) {
	edict_t *newEnt;

	newEnt = G_Spawn();
	newEnt->classname = all_entities[i].classname;
	VectorCopy(all_entities[i].origin, newEnt->s.origin);
	VectorCopy(all_entities[i].angles, newEnt->s.angles);
	newEnt->spawnflags = all_entities[i].spawnflags;
	newEnt->targetname = all_entities[i].targetname;
	newEnt->target = all_entities[i].target;
	ED_CallSpawn(newEnt);
	//gi.dprintf("name: %s - spawnflag: %i - target: %s  - targetname: %s\n", newEnt->classname, newEnt->spawnflags, newEnt->target, newEnt->targetname);
	gi.linkentity(newEnt);
}

void RespawnEntities(edict_t *ent) {
	edict_t *oldent;

	//delete old monsters;
	for (int i = 0; i < globals.num_edicts; i++) {
		oldent = &g_edicts[i];
		if (!oldent->inuse)
			continue;
		if (!strstr(oldent->classname, "monster_"))
			continue;
		G_FreeEdict(oldent);
		//
	}
	for (int i = 0; i < entCount; i++) {
		if (!strstr(all_entities[i].classname, "monster_"))
			continue;
		spawner(i);
	}
}

void RemoveSouls(edict_t *ent) {
	edict_t *soul_ent;

	//remove all souls.
	for (int i = 0; i < globals.num_edicts; i++) {
		soul_ent = &g_edicts[i];
		if (!soul_ent->inuse)
			continue;
		if (strcmp(soul_ent->classname, "strogg_soul") == 0 || strcmp(soul_ent->classname, "player_soul") == 0)
			G_FreeEdict(soul_ent);
	}
}

void FindBonfire(edict_t *ent) {
	vec3_t bonfire;
	VectorCopy(ent->client->pers.last_bonfire->s.origin, bonfire);
	bonfire[2] += 10;
	for (int i = 0; i < 3; i++)
	{
		ent->client->ps.pmove.delta_angles[i] = ANGLE2SHORT(ent->client->pers.last_bonfire->s.angles[i] - ent->client->resp.cmd_angles[i]);
	}

	VectorClear(ent->s.angles);
	VectorClear(ent->client->ps.viewangles);
	VectorClear(ent->client->v_angle);
	VectorCopy(bonfire, ent->s.origin);
}

void DS_Respawn(edict_t *ent) {
	gitem_t *soul;

	RemoveSouls(ent);

	//drop soul
	if (ent->client->pers.souls) {
		soul = FindItemByClassname("player_soul");//&itemlist[43];
		soul->count_width = ent->client->pers.souls;
		Drop_Item(ent, soul);
	}
	ent->client->pers.souls = 0;

	PutClientInServer(ent);
	FindBonfire(ent);
	RespawnEntities(ent);
	ent->s.event = EV_PLAYER_TELEPORT;
	// hold in place briefly
	ent->client->ps.pmove.pm_flags = PMF_TIME_TELEPORT;
	ent->client->ps.pmove.pm_time = 300;
	ent->client->respawn_time = level.time;
}

void SpawnSouls() {
	//DSQ2
	edict_t *ent;
	gitem_t *soul;
	soul = FindItemByClassname("spawned_soul");
	ent = G_Spawn();

	if (strcmp(level.mapname, "base1") == 0) {
		VectorSet(ent->s.origin, -234, 1415, -75);
		soul->count_width = 1000;
		Drop_Item(ent, soul);
		gi.dprintf("%s spawned at %s\n", soul->classname, vtos(ent->s.origin));
	}
	gi.linkentity(ent);

	G_FreeEdict(ent);
}