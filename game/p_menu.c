/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
#include "g_local.h"

// Note that the pmenu entries are duplicated
// this is so that a static set of pmenu entries can be used
// for multiple clients and changed without interference
// note that arg will be freed when the menu is closed, it must be allocated memory
pmenuhnd_t *PMenu_Open(edict_t *ent, pmenu_t *entries, int cur, int num, void *arg)
{
	pmenuhnd_t *hnd;
	pmenu_t *p;
	int i;

	if (!ent->client)
		return NULL;

	if (ent->client->menu) {
		//gi.dprintf("warning, ent already has a menu\n");
		PMenu_Close(ent);
	}

	hnd = malloc(sizeof(*hnd));

	hnd->arg = arg;
	hnd->entries = malloc(sizeof(pmenu_t) * num);
	memcpy(hnd->entries, entries, sizeof(pmenu_t) * num);
	// duplicate the strings since they may be from static memory
	for (i = 0; i < num; i++)
		if (entries[i].text)
			hnd->entries[i].text = strdup(entries[i].text);

	hnd->num = num;

	if (cur < 0 || !entries[cur].SelectFunc) {
		for (i = 0, p = entries; i < num; i++, p++)
			if (p->SelectFunc)
				break;
	}
	else
		i = cur;

	if (i >= num)
		hnd->cur = -1;
	else
		hnd->cur = i;

	ent->client->showscores = 1;
	ent->client->inmenu = true;
	ent->client->menu = hnd;

	PMenu_Do_Update(ent);
	gi.unicast(ent, true);

	return hnd;
}

void PMenu_Close(edict_t *ent)
{
	int i;
	pmenuhnd_t *hnd;

	if (!ent->client->menu)
		return;

	hnd = ent->client->menu;
	for (i = 0; i < hnd->num; i++)
		if (hnd->entries[i].text)
			free(hnd->entries[i].text);
	free(hnd->entries);
	if (hnd->arg)
		free(hnd->arg);
	free(hnd);
	ent->client->menu = NULL;
	ent->client->showscores = 0;
}

// only use on pmenu's that have been called with PMenu_Open
void PMenu_UpdateEntry(pmenu_t *entry, const char *text, int align, SelectFunc_t SelectFunc)
{
	if (entry->text)
		free(entry->text);
	entry->text = strdup(text);
	entry->align = align;
	entry->SelectFunc = SelectFunc;
}

void PMenu_Do_Update(edict_t *ent)
{
	char string[1400];
	int i;
	pmenu_t *p;
	int x;
	pmenuhnd_t *hnd;
	char *t;
	qboolean alt = false;

	if (!ent->client->menu) {
		gi.dprintf("warning:  ent has no menu\n");
		return;
	}

	hnd = ent->client->menu;

	strcpy(string, "xv 32 yv 8 picn inventory ");
	//	strcpy(string, "xv -96 yv 8 picn jven ");

	for (i = 0, p = hnd->entries; i < hnd->num; i++, p++) {
		if (!p->text || !*(p->text))
			continue; // blank line
		t = p->text;
		if (*t == '*') {
			alt = true;
			t++;
		}
		sprintf(string + strlen(string), "yv %d ", 32 + i * 8);
		if (p->align == PMENU_ALIGN_CENTER)
			x = 196 / 2 - strlen(t) * 4 + 64;
		else if (p->align == PMENU_ALIGN_RIGHT)
			x = 64 + (196 - strlen(t) * 8);
		else
			x = 64;

		sprintf(string + strlen(string), "xv %d ",
			x - ((hnd->cur == i) ? 8 : 0));

		if (hnd->cur == i)
			sprintf(string + strlen(string), "string2 \"\x0d%s\" ", t);
		else if (alt)
			sprintf(string + strlen(string), "string2 \"%s\" ", t);
		else
			sprintf(string + strlen(string), "string \"%s\" ", t);
		alt = false;
	}
	gi.WriteByte(svc_layout);
	gi.WriteString(string);
	gi.unicast(ent, true);
}

void PMenu_Update(edict_t *ent)
{
	if (!ent->client->menu) {
		gi.dprintf("warning:  ent has no menu\n");
		return;
	}

	if (level.time - ent->client->menutime >= 1.0) {
		// been a second or more since last update, update now
		PMenu_Do_Update(ent);
		
		ent->client->menutime = level.time;
		ent->client->menudirty = false;
	}
	ent->client->menutime = level.time + 0.2;
	ent->client->menudirty = true;
}

void PMenu_Next(edict_t *ent)
{
	pmenuhnd_t *hnd;
	int i;
	pmenu_t *p;

	if (!ent->client->menu) {
		gi.dprintf("warning:  ent has no menu\n");
		return;
	}

	hnd = ent->client->menu;

	if (hnd->cur < 0)
		return; // no selectable entries

	i = hnd->cur;
	p = hnd->entries + hnd->cur;
	do {
		i++, p++;
		if (i == hnd->num)
			i = 0, p = hnd->entries;
		if (p->SelectFunc)
			break;
	} while (i != hnd->cur);

	hnd->cur = i;

	PMenu_Update(ent);
}

void PMenu_Prev(edict_t *ent)
{
	pmenuhnd_t *hnd;
	int i;
	pmenu_t *p;

	if (!ent->client->menu) {
		gi.dprintf("warning:  ent has no menu\n");
		return;
	}

	hnd = ent->client->menu;

	if (hnd->cur < 0)
		return; // no selectable entries

	i = hnd->cur;
	p = hnd->entries + hnd->cur;
	do {
		if (i == 0) {
			i = hnd->num - 1;
			p = hnd->entries + i;
		}
		else
			i--, p--;
		if (p->SelectFunc)
			break;
	} while (i != hnd->cur);

	hnd->cur = i;

	PMenu_Update(ent);
}

void PMenu_Select(edict_t *ent)
{
	pmenuhnd_t *hnd;
	pmenu_t *p;

	if (!ent->client->menu) {
		gi.dprintf("warning:  ent has no menu\n");
		return;
	}

	hnd = ent->client->menu;

	if (hnd->cur < 0)
		return; // no selectable entries

	p = hnd->entries + hnd->cur;

	if (p->SelectFunc)
		p->SelectFunc(ent, hnd);
}


/*
==================
Paril's Menus

Generic Menu
  Thanks to ZOID for Pmenu
==================
pmenu_t genericmenu() = {
	{ NULL,					PMENU_ALIGN_CENTER, NULL },
	{ NULL,                	PMENU_ALIGN_CENTER, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_RIGHT, NULL },
};

void GenericMenuOpen edict_t *ent, pmenuhnd_t *p
{
	PMenu_Close(ent);
	PMenu_Openent, genericmenu, -1, sizeofgenericmenu / sizeofpmenu_t, NULL;
}*/

pmenu_t mainmenu[] = {
	{ "*DARK STROGGS Quake II",					PMENU_ALIGN_CENTER, NULL },
	{ NULL,                	PMENU_ALIGN_CENTER, NULL },
	{ "Weapons->",			PMENU_ALIGN_LEFT, WeaponMenuOpen },
	{ "Update health",				PMENU_ALIGN_LEFT, UpdateHealth },
	{ "Update stamina",					PMENU_ALIGN_LEFT, UpdateStamina },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_RIGHT, NULL },
};

pmenu_t weaponmenu[] = {
	{ "*DARK STROGGS Quake II",					PMENU_ALIGN_CENTER, NULL },
	{ NULL,                	PMENU_ALIGN_CENTER, NULL },
	{ "wep1",				PMENU_ALIGN_LEFT, NULL },
	{ "wep2",				PMENU_ALIGN_LEFT, NULL },
	{ "wep3",				PMENU_ALIGN_LEFT, NULL },
	{ "wep4",				PMENU_ALIGN_LEFT, NULL },
	{ "wep5",				PMENU_ALIGN_LEFT, NULL },
	{ "wep6",				PMENU_ALIGN_LEFT, NULL },
	{ "wep7",				PMENU_ALIGN_LEFT, NULL },
	{ "wep8",				PMENU_ALIGN_LEFT, NULL },
	{ "wep9",				PMENU_ALIGN_LEFT, NULL },
	{ "wep10",				PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_LEFT, NULL },
	{ NULL,					PMENU_ALIGN_RIGHT, NULL },
};

void MainMenuOpen(edict_t *ent)
{
	if (!ent->client->pers.bonfire) {
		gi.cprintf(ent, PRINT_HIGH, "Not at a bonfire!\n");
		return;
	}
	G_FreeEdict(ent);
	PutClientInServer(ent);
	SpawnMonsters(ent);
	PMenu_Close(ent);
	PMenu_Open(ent, mainmenu, -1, sizeof(mainmenu) / sizeof(pmenu_t), NULL);
	mainmenu[2].SelectFunc = WeaponMenuOpen;
	mainmenu[3].SelectFunc = UpdateHealth;
	mainmenu[4].SelectFunc = UpdateStamina;
}

void WeaponMenuOpen(edict_t *ent, pmenuhnd_t *p)
{
	gitem_t *it;
	int i,j;
	j = 7;
	for (i = 2; i <= 12; i++) {
		weaponmenu[i].text = "";
		while(j <= 18) {
			if (ent->client->pers.inventory[j]) {
				it = &itemlist[j];
				if (!it->weapmodel) {
					j++;
					continue;
				}
				sprintf(it->menuname, "%s (level %i)", it->pickup_name, it->level+1);
				weaponmenu[i].text = it->menuname;
				weaponmenu[i].SelectFunc = UpgradeWeapon;
				j++;
				break;
			}
			j++;
		}
	}
	PMenu_Close(ent);
	PMenu_Open(ent, weaponmenu, -1, sizeof(weaponmenu) / sizeof(pmenu_t), NULL);
}