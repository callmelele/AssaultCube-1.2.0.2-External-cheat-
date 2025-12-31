#pragma once
#include <vector>
#include "WinFunctions.h"
#include "Entity.h"


class Entities {
private:
    WinFunc winFunc;

public:
    int amount;
    std::vector<Entity> list;

    void GetEntityAmount();

    Entities(WinFunc wFunc)
    {
        winFunc = wFunc;
        GetEntityAmount(); // amount is set here
        list.resize(amount); // Now list is sized correctly
    }

    void GetInfo();
    void Print();
};
