#include "g_local.h"
#include "dsq2.h"


void DrinkHealth(edict_t *ent) {
	if (!ent->client->pers.health_flask)
		return;
	ent->health += 25;
	if (ent->health > ent->client->pers.max_health)
		ent->health = ent->client->pers.max_health;
	ent->client->pers.health_flask--;
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