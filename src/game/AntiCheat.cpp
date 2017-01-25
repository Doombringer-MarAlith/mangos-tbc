#include "AntiCheat.h"
#include "Player.h"

AntiCheat::AntiCheat(Player* player)
{
    m_Player = player;

    for (uint8 i = 0; i < 2; ++i)
        m_MoveInfo[i] = MovementInfo();

    m_Initialized = false;

    player->AddAntiCheatModule(this);
}

void AntiCheat::HandleRelocate(float x, float y, float z, float o)
{
    m_MoveInfo[1].ChangePosition(x, y, z, o);
}

void AntiCheat::HandleTeleport(uint32 map, float x, float y, float z, float o)
{
    m_MoveInfo[1].ChangePosition(x, y, z, o);
}

bool AntiCheat::Initialized()
{
    if (!m_Initialized)
    {
        m_Initialized = true;
        return false;
    }

    return true;
}

bool AntiCheat::CanFly()
{
    return m_Player->HasAuraType(SPELL_AURA_FLY) || m_Player->GetGMFly();
}

bool AntiCheat::IsMoving(MovementInfo& moveInfo)
{
    return moveInfo.HasMovementFlag(MOVEFLAG_FORWARD) ||
        moveInfo.HasMovementFlag(MOVEFLAG_BACKWARD) ||
        moveInfo.HasMovementFlag(MOVEFLAG_STRAFE_LEFT) ||
        moveInfo.HasMovementFlag(MOVEFLAG_STRAFE_RIGHT) ||
        moveInfo.HasMovementFlag(MOVEFLAG_FALLING) ||
        moveInfo.HasMovementFlag(MOVEFLAG_FALLINGFAR) ||
        moveInfo.HasMovementFlag(MOVEFLAG_ASCENDING) ||
        moveInfo.HasMovementFlag(MOVEFLAG_SAFE_FALL);
}

bool AntiCheat::IsMoving()
{
    return IsMoving(m_MoveInfo[0]) || IsMoving(m_MoveInfo[1]);
}

bool AntiCheat::isFlying(MovementInfo& moveInfo)
{
    return moveInfo.HasMovementFlag(MOVEFLAG_FLYING) || moveInfo.HasMovementFlag(MOVEFLAG_FLYING2);
}

bool AntiCheat::isFlying()
{
    return isFlying(m_MoveInfo[0]) || isFlying(m_MoveInfo[1]);
}

bool AntiCheat::isWalking(MovementInfo& moveInfo)
{
    return moveInfo.HasMovementFlag(MOVEFLAG_WALK_MODE);
}

bool AntiCheat::isWalking()
{
    return isWalking(m_MoveInfo[0]) || isWalking(m_MoveInfo[1]);
}

bool AntiCheat::isFalling(MovementInfo& moveInfo)
{
    return moveInfo.HasMovementFlag(MOVEFLAG_FALLING) || moveInfo.HasMovementFlag(MOVEFLAG_FALLINGFAR);
}

bool AntiCheat::isFalling()
{
    return isFalling(m_MoveInfo[0]) || isFalling(m_MoveInfo[1]);
}

bool AntiCheat::isTransport(MovementInfo& moveInfo)
{
    return moveInfo.HasMovementFlag(MOVEFLAG_ONTRANSPORT);
}

bool AntiCheat::isTransport()
{
    return isTransport(m_MoveInfo[0]) || isTransport(m_MoveInfo[1]);
}

bool AntiCheat::isSwimming(MovementInfo& moveInfo)
{
    return moveInfo.HasMovementFlag(MOVEFLAG_SWIMMING);
}

bool AntiCheat::isSwimming()
{
    return isSwimming(m_MoveInfo[0]) || isSwimming(m_MoveInfo[1]) || m_Player->IsInWater();
}

bool AntiCheat::verifyTransportCoords(MovementInfo& moveInfo)
{
    return !(std::abs(m_MoveInfo[0].GetTransportPos()->x) > 100 || std::abs(m_MoveInfo[0].GetTransportPos()->y) > 100 || std::abs(m_MoveInfo[0].GetTransportPos()->z) > 100);
}

bool AntiCheat::verifyTransportCoords()
{
    return verifyTransportCoords(m_MoveInfo[0]) || verifyTransportCoords(m_MoveInfo[1]);
}

float AntiCheat::GetDistOrTransportDist()
{
    return GetDistOrTransportDist(isFlying());
}

float AntiCheat::GetDistOrTransportDist(bool threed)
{
    return isTransport(m_MoveInfo[0]) && isTransport(m_MoveInfo[1]) ? GetTransportDist(threed) : GetDistance(threed);
}

float AntiCheat::GetDistanceZ()
{
    return m_MoveInfo[0].GetPos()->z - m_MoveInfo[1].GetPos()->z;
}

float AntiCheat::GetDistance()
{
    return GetDistance(isFlying());
}

float AntiCheat::GetDistance(bool threed)
{
    return threed ? GetDistance3D() : GetDistance2D();
}

float AntiCheat::GetDistance2D()
{
    return
        sqrt(pow(m_MoveInfo[0].GetPos()->x - m_MoveInfo[1].GetPos()->x, 2) +
            pow(m_MoveInfo[0].GetPos()->y - m_MoveInfo[1].GetPos()->y, 2));
}

float AntiCheat::GetDistance3D()
{
    return
        sqrt(pow(m_MoveInfo[0].GetPos()->x - m_MoveInfo[1].GetPos()->x, 2) +
            pow(m_MoveInfo[0].GetPos()->y - m_MoveInfo[1].GetPos()->y, 2) +
            pow(m_MoveInfo[0].GetPos()->z - m_MoveInfo[1].GetPos()->z, 2));
}

float AntiCheat::GetTransportDist()
{
    return GetTransportDist(isFlying());
}

float AntiCheat::GetTransportDist(bool threed)
{
    return threed ? GetTransportDist3D() : GetTransportDist2D();
}

float AntiCheat::GetTransportDist2D()
{
    return
        sqrt(pow(m_MoveInfo[0].GetTransportPos()->x - m_MoveInfo[1].GetTransportPos()->x, 2) +
            pow(m_MoveInfo[0].GetTransportPos()->y - m_MoveInfo[1].GetTransportPos()->y, 2));
}

float AntiCheat::GetTransportDist3D()
{
    return
        sqrt(pow(m_MoveInfo[0].GetTransportPos()->x - m_MoveInfo[1].GetTransportPos()->x, 2) +
            pow(m_MoveInfo[0].GetTransportPos()->y - m_MoveInfo[1].GetTransportPos()->y, 2) +
            pow(m_MoveInfo[0].GetTransportPos()->z - m_MoveInfo[1].GetTransportPos()->z, 2));
}

float AntiCheat::GetTransportDistZ()
{
    return m_MoveInfo[0].GetTransportPos()->z - m_MoveInfo[1].GetTransportPos()->z;
}

float AntiCheat::GetSpeed()
{
    // Positive speed modifiers are applied if they're active in current or previous, negative if they're active in previous and current.
    bool back = m_MoveInfo[0].HasMovementFlag(MOVEFLAG_BACKWARD) && m_MoveInfo[1].HasMovementFlag(MOVEFLAG_BACKWARD);

    float speed = m_Player->GetSpeed(back ? MOVE_RUN_BACK : MOVE_RUN);

    if (isFlying(m_MoveInfo[0]) || isFlying(m_MoveInfo[1]))
        speed = m_Player->GetSpeed(back ? MOVE_FLIGHT_BACK : MOVE_FLIGHT);
    else if (isWalking(m_MoveInfo[0]) && isWalking(m_MoveInfo[1]))
        speed = m_Player->GetSpeed(MOVE_WALK);
    else if (isSwimming(m_MoveInfo[0]) && isSwimming(m_MoveInfo[1]))
        speed = m_Player->GetSpeed(back ? MOVE_SWIM_BACK : MOVE_SWIM);

    return speed;
}

float AntiCheat::GetAllowedDistance()
{
    return GetDiffInSec() * GetSpeed();
}

uint32 AntiCheat::GetDiff()
{
    uint32 t1 = m_MoveInfo[0].GetTime();
    uint32 t2 = m_MoveInfo[1].GetTime();

    return std::max(uint32(1), std::max(t1, t2) - std::min(t1, t2));
}

float AntiCheat::GetDiffInSec()
{
    return GetDiff() / 1000.f;
}

float AntiCheat::GetVirtualDiffInSec()
{
    if (IsMoving(m_MoveInfo[0]) && !IsMoving(m_MoveInfo[1]))
        return 1.f;

    return GetDiffInSec();
}
