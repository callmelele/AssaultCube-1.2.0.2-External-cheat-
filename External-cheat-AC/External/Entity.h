#pragma once
#include "Structs.h"

class Entity {

public:

	char name[20];
	int base;
	int health;
	vec3d_f position_head;
	vec3d_f position_feet;
	int team;
	int ar_ammo;
	int smg_ammo;
	int sniper_ammo;
	int shotgun_ammo;
	int carbine_ammo;
	int grenade_ammo;


	void Print();
};