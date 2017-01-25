#pragma once

#include "AntiCheat.h"

class AntiCheat_nofall : public AntiCheat
{
public:
    AntiCheat_nofall(Player* player);

    bool HandleMovement(MovementInfo& moveInfo, Opcodes opcode, bool cheat);
    void HandleRelocate(float x, float y, float z, float o);
    void HandleTeleport(uint32 map, float x, float y, float z, float o);
private:
    time_t m_LastCheck;
};
