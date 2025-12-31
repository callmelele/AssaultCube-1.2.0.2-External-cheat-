#include <iostream>
#include "Entity.h"

void Entity::Print()
{
	std::cout << "Name: " << name << "\n";
	std::cout << "Health: " << health << "\n";
	std::cout << "Position (head): (" << position_head.x << ", " << position_head.y << ", " << position_head.z << ")\n";
	std::cout << "Position (feet): (" << position_feet.x << ", " << position_feet.y << ", " << position_feet.z << ")\n";
	std::cout << "Team: " << team << "\n";
    if (ar_ammo >= 1 && ar_ammo <= 30)
        int Ar = true;
        std::cout << "AR Ammo: " << ar_ammo << "\n";
    if (smg_ammo >= 1 && smg_ammo <= 30)
        int Smg = true;
        std::cout << "SMG Ammo: " << smg_ammo << "\n";
    if (sniper_ammo >= 1 && sniper_ammo <= 30)
        int Sniper = true;
        std::cout << "Sniper Ammo: " << sniper_ammo << "\n";
    if (shotgun_ammo >= 1 && shotgun_ammo <= 30)
        int Shotgun = true; 
        std::cout << "Shotgun Ammo: " << shotgun_ammo << "\n";
    if (carbine_ammo >= 1 && carbine_ammo <= 30)
        int carbine = true;
        std::cout << "Carbine Ammo: " << carbine_ammo << "\n";
	
	std::cout << "\n\n";
}