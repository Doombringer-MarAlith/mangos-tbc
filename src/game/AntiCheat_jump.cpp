#include "AntiCheat_jump.h"
#include "Player.h"

AntiCheat_jump::AntiCheat_jump(Player* player) : AntiCheat(player)
{
}

bool AntiCheat_jump::HandleMovement(MovementInfo& moveInfo, Opcodes opcode, bool cheat)
{
    m_MoveInfo[0] = moveInfo; // moveInfo shouldn't be used anymore then assigning it in the beginning.

    if (!Initialized())
    {
        m_MoveInfo[2] = m_MoveInfo[0];
        return SetOldMoveInfo(false);
    }

    if (!cheat && opcode == MSG_MOVE_JUMP && isFalling(m_MoveInfo[1]))
    {
        const Position* p = m_MoveInfo[2].GetPos();
        m_Player->TeleportTo(m_Player->GetMapId(), p->x, p->y, p->z, p->o, TELE_TO_NOT_LEAVE_COMBAT);

        // if (m_Player->GetSession()->GetSecurity() > SEC_PLAYER)
        //     m_Player->BoxChat << "Jump hack" << "\n";

        return SetOldMoveInfo(true);
    }

    if (opcode == MSG_MOVE_JUMP)
        m_MoveInfo[2] = m_MoveInfo[0];

    return SetOldMoveInfo(false);
}
