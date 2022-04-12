#include "g_local.h"
#include "dsq2.h"
xplevel[99] = { 500, 700, 1000, 1400, 1900, 2500, 3200, 4000, 4900, 5900, 7000, 8200, 9500, 
10900, 12400, 14000, 15700, 17500, 19400, 21400, 23500, 25700, 28000, 30400, 32900, 35500, 
38200, 41000, 43900, 46900, 50000, 53200, 56500, 59900, 63400, 67000, 70700, 74500, 78400, 82400, 
86500, 90700, 95000, 99400, 103900, 108500, 113200, 118000, 122900, 127900, 133000, 138200, 143500, 
148900, 154400, 160000, 165700, 171500, 177400, 183400, 189500, 195700, 202000, 208400, 214900, 221500, 
228200, 235000, 241900, 248900, 256000, 263200, 270500, 277900, 285400, 293000, 300700, 308500, 316400, 
324400, 332500, 340700, 349000, 357400, 365900, 374500, 383200, 392000, 400900, 409900, 419000, 428200, 
437500, 446900, 456400, 466000, 475700, 485500, 495400 };


void DrinkHealth(edict_t *ent) {
	if (!ent->client->resp.health_flask)
		return;
	ent->health += 25;
	if (ent->health > ent->client->pers.max_health)
		ent->health = ent->client->pers.max_health;
	ent->client->resp.health_flask--;
}

void UpdateHealth(edict_t *ent) {
	if (ent->client->pers.souls >= xplevel[ent->client->pers.health_level] && ent->client->pers.health_level < 99) {
		//upgrade
		ent->client->pers.max_health += HEALTH_PLAYER_LEVEL;
		ent->client->pers.souls -= xplevel[ent->client->pers.health_level];
		ent->client->pers.health_level++;
	}
	else {
		gi.cprintf(ent, PRINT_HIGH, "Could not upgrade health. %s\n", ent->client->pers.health_level < 99 ? "Not enough stroggpoints" : "Health is maxed!");
		return;
	}
	ent->health = ent->client->pers.max_health;

	MainMenuOpen(ent);
}

void UpdateStamina(edict_t *ent) {
	if (ent->client->pers.souls >= xplevel[ent->client->pers.stamina_level] && ent->client->pers.stamina_level < 99) {
		//upgrade
		ent->client->pers.max_stamina += STAMINA_PLAYER_LEVEL;
		ent->client->pers.souls -= xplevel[ent->client->pers.stamina_level];
		ent->client->pers.stamina_level++;
	}
	else {
		gi.cprintf(ent, PRINT_HIGH, "Could not upgrade stamina. %s\n", ent->client->pers.stamina_level < 99 ? "Not enough stroggpoints" : "Stamina is maxed!");
		return;
	}
	ent->client->pers.stamina = ent->client->pers.max_stamina;
	MainMenuOpen(ent);
}

void UpgradeWeapon(edict_t *ent, pmenuhnd_t *hnd) {
	gitem_t *it;
	int i;
	i = hnd->cur+5;
	while (i < 18) {
		if (ent->client->pers.inventory[i]) {
			it = &itemlist[i];
			if (ent->client->pers.souls >= xplevel[it->level] && it->level < 99) {
				//upgrade
				ent->client->pers.souls -= xplevel[it->level];
				it->level++;
				gi.cprintf(ent, PRINT_HIGH, "%s updated.\n", it->classname);
				break;
			}
			else {
				gi.cprintf(ent, PRINT_HIGH, "Could not upgrade this weapon. %s\n", it->level < 99 ? "Not enough stroggpoints" : "Weapon is maxed!");
				return;
			}
		}
		i++;
	}
	//gi.cprintf(ent, PRINT_HIGH, "hnd->num:%i, hnd->cur:%i hnd->entries->text:%s\n",hnd->num, hnd->cur, hnd->entries->text);
	WeaponMenuOpen(ent, hnd);
}

void DeleteItems(edict_t *ent) {
	edict_t *item;
	(void)ent;
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
	(void)ent;
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
	(void)ent;
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