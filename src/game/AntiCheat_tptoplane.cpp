#include "AntiCheat_tptoplane.h"
#include "Player.h"

AntiCheat_tptoplane::AntiCheat_tptoplane(Player* player) : AntiCheat(player)
{
}

bool AntiCheat_tptoplane::HandleMovement(MovementInfo& moveInfo, Opcodes opcode, bool cheat)
{
    m_MoveInfo[0] = moveInfo; // moveInfo shouldn't be used anymore then assigning it in the beginning.

    if (!Initialized())
        return SetOldMoveInfo(false);

    const Position* p = m_MoveInfo[0].GetPos();

    if (GetDiff() < 1000 || std::abs(p->z) > 0.1f)
        return false;

    TerrainInfo const* terrain = m_Player->GetTerrain();

    float groundZ = terrain->GetHeightStatic(p->x, p->y, p->z, true);

    float playerZ = p->z;

    if (!cheat && playerZ - groundZ < -1.f && playerZ < groundZ)
    {
        p = m_MoveInfo[1].GetPos();
        groundZ = terrain->GetHeightStatic(p->x, p->y, p->z);

        m_Player->TeleportTo(m_Player->GetMapId(), p->x, p->y, groundZ, p->o, TELE_TO_NOT_LEAVE_COMBAT);

        // if (m_Player->GetSession()->GetSecurity() > SEC_PLAYER)
        //     m_Player->BoxChat << "TELEPORT TO PLANE CHEAT" << "\n";

        return SetOldMoveInfo(true);
    }

    return SetOldMoveInfo(false);
}
