#include "AntiCheat_fly.h"
#include "Player.h"

AntiCheat_fly::AntiCheat_fly(Player* player) : AntiCheat(player)
{
}

bool AntiCheat_fly::HandleMovement(MovementInfo& moveInfo, Opcodes opcode, bool cheat)
{
    m_MoveInfo[0] = moveInfo; // moveInfo shouldn't be used anymore then assigning it in the beginning.

    if (!Initialized())
    {
        m_LastCanFlyTime = m_MoveInfo[0].GetTime();
        return SetOldMoveInfo(false);
    }

    if (CanFly())
        m_LastCanFlyTime = m_MoveInfo[0].GetTime();

    if (isFlying(m_MoveInfo[0]) && !CanFly() && m_LastCanFlyTime + 500 < m_MoveInfo[0].GetTime())
    {
        // if (m_Player->GetSession()->GetSecurity() > SEC_PLAYER)
        //     m_Player->BoxChat << "FLY CHEAT" << "\n";

        m_Player->SetCanFly(false);

        return SetOldMoveInfo(true);
    }

    return SetOldMoveInfo(false);
}
