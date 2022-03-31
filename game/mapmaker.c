
#include "g_local.h"
/*--------------------------
Add a point to the map;
----------------------------*/
#define point1 self->point1
#define point2 self->point2

void laser_think(edict_t *self) {
	gi.WriteByte(svc_temp_entity);
	gi.WriteByte(TE_DEBUGTRAIL);
	gi.WritePosition(self->pos1);
	gi.WritePosition(self->pos2);
	gi.multicast(self->s.origin, MULTICAST_PVS);

	gi.dprintf("1:%s - 2:%s\n", vtos(self->pos1), vtos(self->pos2));
	self->nextthink = level.time + FRAMETIME;
}

void point_add(edict_t *self) {
	vec3_t  start, forward, right, end;
	int nr;
	edict_t *laser;

	if (Q_stricmp(gi.argv(1), "laser") == 0) {
		laser = G_Spawn();

		VectorCopy(point1->s.origin, laser->pos1);
		VectorCopy(point2->s.origin, laser->pos2);
		VectorCopy(point1->s.origin, laser->s.origin);
		laser->think = laser_think;
		gi.linkentity(laser);
		return;
	}

	nr = atoi(gi.argv(1));
	if (nr != 1 && nr != 2) {
		gi.cprintf(self, PRINT_HIGH, "Please specify either addpoint 1 for point 1, or addpoint 2 for point 2\n");
		return;
	}
	switch (nr) {
	case 1:
		if (point1) {
			G_FreeEdict(point1);
			point1 = NULL;
			gi.cprintf(self, PRINT_HIGH, "point 1 removed.\n");
			return;
		}
		gi.cprintf(self, PRINT_HIGH, "point 1 added.\n");

		AngleVectors(self->client->v_angle, forward, right, NULL);

		VectorSet(end, 100, 0, 0);
		G_ProjectSource(self->s.origin, end, forward, right, start);

		point1 = G_Spawn();
		point1->owner = self;
		point1->movetype = MOVETYPE_NOCLIP;
		point1->solid = SOLID_NOT;
		point1->classname = "point";
		point1->s.modelindex = gi.modelindex("models/objects/grenade2/tris.md2");
		point1->s.skinnum = 0;

		point1->s.renderfx |= RF_FULLBRIGHT;

		point1->think = point_think;
		point1->nextthink = level.time + FRAMETIME;
		break;
	case 2:
		if (point2) {
			G_FreeEdict(point2);
			point2 = NULL;
			gi.cprintf(self, PRINT_HIGH, "point 2 removed.\n");
			return;
		}
		gi.cprintf(self, PRINT_HIGH, "point 2 added.\n");

		AngleVectors(self->client->v_angle, forward, right, NULL);

		VectorSet(end, 100, 0, 0);
		G_ProjectSource(self->s.origin, end, forward, right, start);

		point2 = G_Spawn();
		point2->owner = self;
		point2->movetype = MOVETYPE_NOCLIP;
		point2->solid = SOLID_NOT;
		point2->classname = "point";
		point2->s.modelindex = gi.modelindex("models/objects/grenade2/tris.md2");
		point2->s.skinnum = 0;

		point2->s.renderfx |= RF_FULLBRIGHT;

		point2->think = point_think;
		point2->nextthink = level.time + FRAMETIME;
		break;
	}
}

void point_think(edict_t *self)
{
	vec3_t start, end, endp, offset;
	vec3_t forward, right, up;
	trace_t tr;

	AngleVectors(self->owner->client->v_angle, forward, right, up);

	VectorSet(offset, 24, 6, self->owner->viewheight - 7);
	G_ProjectSource(self->owner->s.origin, offset, forward, right, start);
	VectorMA(start, 8192, forward, end);

	tr = gi.trace(start, NULL, NULL, end, self->owner, CONTENTS_SOLID | CONTENTS_MONSTER | CONTENTS_DEADMONSTER);

	if (tr.fraction != 1) {
		VectorMA(tr.endpos, -4, forward, endp);
		VectorCopy(endp, tr.endpos);
	}

	if ((tr.ent->svflags & SVF_MONSTER) || (tr.ent->client)) {
		if ((tr.ent->takedamage) && (tr.ent != self->owner)) {
			self->s.skinnum = 1;
		}
	}
	else
		self->s.skinnum = 0;

	vectoangles(tr.plane.normal, self->s.angles);
	VectorCopy(tr.endpos, self->s.origin);
	gi.linkentity(self);
}

void drawbbox(edict_t *self)
{
	int lines[4][3] = {
		{1, 2, 4},
		{1, 2, 7},
		{1, 4, 5},
		{2, 4, 7}
	};

	if (!self)
	{
		return;
	}

	int starts[4] = { 0, 3, 5, 6 };

	vec3_t pt[8];
	int i, j, k;
	vec3_t coords[2];
	vec3_t newbox;
	vec3_t f, r, u, dir;

	VectorCopy(self->absmin, coords[0]);
	VectorCopy(self->absmax, coords[1]);

	for (i = 0; i <= 1; i++)
	{
		for (j = 0; j <= 1; j++)
		{
			for (k = 0; k <= 1; k++)
			{
				pt[4 * i + 2 * j + k][0] = coords[i][0];
				pt[4 * i + 2 * j + k][1] = coords[j][1];
				pt[4 * i + 2 * j + k][2] = coords[k][2];
			}
		}
	}

	for (i = 0; i <= 3; i++)
	{
		for (j = 0; j <= 2; j++)
		{
			gi.WriteByte(svc_temp_entity);
			gi.WriteByte(TE_DEBUGTRAIL);
			gi.WritePosition(pt[starts[i]]);
			gi.WritePosition(pt[lines[i][j]]);
			gi.multicast(pt[starts[i]], MULTICAST_ALL);
		}
	}

	vectoangles(self->s.angles, dir);        // FIXME: vectoangles2
	AngleVectors(dir, f, r, u);

	VectorMA(self->s.origin, 50, f, newbox);
	gi.WriteByte(svc_temp_entity);
	gi.WriteByte(TE_DEBUGTRAIL);
	gi.WritePosition(self->s.origin);
	gi.WritePosition(newbox);
	gi.multicast(self->s.origin, MULTICAST_PVS);
	VectorClear(newbox);

	VectorMA(self->s.origin, 50, r, newbox);
	gi.WriteByte(svc_temp_entity);
	gi.WriteByte(TE_DEBUGTRAIL);
	gi.WritePosition(self->s.origin);
	gi.WritePosition(newbox);
	gi.multicast(self->s.origin, MULTICAST_PVS);
	VectorClear(newbox);

	VectorMA(self->s.origin, 50, u, newbox);
	gi.WriteByte(svc_temp_entity);
	gi.WriteByte(TE_DEBUGTRAIL);
	gi.WritePosition(self->s.origin);
	gi.WritePosition(newbox);
	gi.multicast(self->s.origin, MULTICAST_PVS);
	VectorClear(newbox);
}
