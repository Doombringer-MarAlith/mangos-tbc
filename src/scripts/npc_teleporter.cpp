#include "Precompiled.h"
#include "ArenaTeam.h"
#include "Settings.h"
#include "CPlayer.h"
#include "CPlusMgr.h"

class NPC_Teleporter : public CreatureScript
{
public:
    NPC_Teleporter() : CreatureScript("npc_teleporter") {}

    bool OnGossipHello(Player* pPlayer, Creature* pCreature) override
    {
        CPlayer* pCPlayer = pPlayer->GetCPlayer();

        if (pPlayer->isInCombat())
        {
            pCPlayer->BoxChat << sCustom.ChatNameWrapper(pCreature->GetName()) << " You are in combat!" << std::endl;
            return true;
        }

        if ((pPlayer->GetAreaId() != 3536 && pPlayer->GetTeam() == HORDE) ||
            (pPlayer->GetAreaId() != 3538 && pPlayer->GetTeam() == ALLIANCE) ||
            pPlayer->isGameMaster())
            pCPlayer->AddGossipMenuItem(Icon::TAXI, "Teleport To: Shopping Mall", GOSSIP_SENDER_MAIN, 1);

        if (pPlayer->GetAreaId() != 3539 || pPlayer->isGameMaster())
            pCPlayer->AddGossipMenuItem(Icon::TAXI, "Teleport To: Neutral  Mall", GOSSIP_SENDER_MAIN, 2);

        if (pPlayer->GetAreaId() != 3738 || pPlayer->isGameMaster())
            pCPlayer->AddGossipMenuItem(Icon::TAXI, "Teleport To: Stormspire", GOSSIP_SENDER_MAIN, 4);

        if ((pPlayer->GetAreaId() != 1741 && pPlayer->GetAreaId() != 2177) ||
            pPlayer->isGameMaster())
            pCPlayer->AddGossipMenuItem(Icon::TAXI, "Teleport To: Gurubashi Arena", GOSSIP_SENDER_MAIN, 3);

        pCPlayer->AddGossipMenuItem(Icon::GEAR, "Open Tools                  ", GOSSIP_SENDER_MAIN, 100);
        pCPlayer->AddGossipMenuItem(Icon::GEAR, "Change Your Level           ", GOSSIP_SENDER_MAIN, 200);
        pCPlayer->AddGossipMenuItem(Icon::BAG, "Open Vendors                ", GOSSIP_SENDER_MAIN, 300);
        pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Queue for battle            ", GOSSIP_SENDER_MAIN, 400);

        if (pPlayer->isGameMaster())
            pCPlayer->AddGossipMenuItem(Icon::BAG, "GM menu", GOSSIP_SENDER_MAIN, 1337);

        pPlayer->PlayerTalkClass->SendGossipMenu("Your wish, my law!", pCreature->GetObjectGuid());
        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action, std::string code)
    {
        CPlayer* pCPlayer = pPlayer->GetCPlayer();

        const char* cName = pCreature->GetName();
        pPlayer->PlayerTalkClass->CloseGossip();

        if (pPlayer->isInCombat())
        {
            pCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << " You are in combat!" << std::endl;
            return true;
        }

        if (action == 0)
            OnGossipHello(pPlayer, pCreature);
        else if (sender == GOSSIP_SENDER_MULTIVENDOR)
            pCPlayer->SendMultiVendorInventory(action, pCreature->GetObjectGuid());
        else if (action == 1) // Teleport To: Shopping Mall
        {
            pCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << " Welcome to The Shopping Mall" << std::endl;
            if (pPlayer->GetTeam() == HORDE)
                pPlayer->TeleportTo(530, 237.497f, 2696.65f, 90.755f, 3.25544f);
            else
                pPlayer->TeleportTo(530, -796.259f, 2625.9f, 124.095f, 3.70235f);
        }
        else if (action == 2) // Teleport To: Neutral Mall
        {
            pCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << " Welcome to Neutral shopping mall" << std::endl;
            pPlayer->TeleportTo(530, -248.403f, 939.569f, 84.3788f, 1.53148f);
        }
        else if (action == 3) // Teleport To: Gurubashi Arena
        {
            pCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << " Welcome to Gurubashi Arena" << std::endl;
            pPlayer->TeleportTo(0, -13235.7f, 213.741f, 31.2181f, 1.14919f);
        }
        else if (action == 4) // Teleport To: Stormspire
        {
            pCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << " Welcome to The Stormspire" << std::endl;

            if (pPlayer->GetTeam() == ALLIANCE)
                pPlayer->TeleportTo(530, 4201.07f, 3100.86f, 335.821f, 4.02089f);
            else
                pPlayer->TeleportTo(530, 4118.95f, 2936.97f, 354.523f, 1.62542f);
        }
        else if (action == 100) // Open Tools
        {
            pCPlayer->AddGossipMenuItem(Icon::TRAINER, "Reset Talents     ", GOSSIP_SENDER_MAIN, 101);
            pCPlayer->AddGossipMenuItem(Icon::TRAINER, "Reset Cooldowns   ", GOSSIP_SENDER_MAIN, 104);
            pCPlayer->AddGossipMenuItem(Icon::TRAINER, "Max Weapon Skills ", GOSSIP_SENDER_MAIN, 103);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "Open Bank         ", GOSSIP_SENDER_MAIN, 105);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "Open Auction House", GOSSIP_SENDER_MAIN, 106);
            pCPlayer->AddGossipMenuItem(Icon::TABARD, "Create a guild    ", GOSSIP_SENDER_MAIN, 107);
            pCPlayer->AddGossipMenuItem(Icon::TABARD, "Design your tabard", GOSSIP_SENDER_MAIN, 108);

            if (pPlayer->GetSettings()->GetSetting(SETTING_UINT_HIDETEMPLATEMENU))
                pCPlayer->AddGossipMenuItem(Icon::GEAR, "Show templates on login", GOSSIP_SENDER_MAIN, 109);

            pCPlayer->AddGossipMenuItem(Icon::GEAR, "Back              ", GOSSIP_SENDER_MAIN, 0);
        }
        else if (action == 101) // Reset Talents
        {
            pCPlayer->AddGossipMenuItem(Icon::GEAR, "Are you sure you want to reset your talents?", GOSSIP_SENDER_MAIN, 100);
            pCPlayer->AddGossipMenuItem(Icon::GEAR, "Yes                                         ", GOSSIP_SENDER_MAIN, 102);
            pCPlayer->AddGossipMenuItem(Icon::GEAR, "No                                          ", GOSSIP_SENDER_MAIN, 100);
            pCPlayer->AddGossipMenuItem(Icon::GEAR, "Back                                        ", GOSSIP_SENDER_MAIN, 0);
        }
        else if (action == 102) // Talents Reset
        {
            pCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << " Talents Reset" << std::endl;
            pPlayer->resetTalents(true);
        }
        else if (action == 103) // Weapon skills maxed
        {
            pCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << " Weapon skills maxed" << std::endl;
            pPlayer->UpdateSkillsToMaxSkillsForLevel();
        }
        else if (action == 104) // Cooldowns reset
        {
            pCPlayer->BoxChat << sCustom.ChatNameWrapper(cName) << " Cooldowns reset" << std::endl;
            pPlayer->RemoveArenaSpellCooldowns();
        }
        else if (action == 105) // Show bank
            pPlayer->GetSession()->SendShowBank(pCreature->GetObjectGuid());
        else if (action == 106) // Auctioneer
            pPlayer->GetSession()->SendAuctionHello(pCreature);
        else if (action == 107) // Guild Master
            pPlayer->GetSession()->SendPetitionShowList(pCreature->GetObjectGuid());
        else if (action == 108) // Tabard Petitioner
            pPlayer->GetSession()->SendTabardVendorActivate(pCreature->GetObjectGuid());
        else if (action == 109)
        {
            pPlayer->GetSettings()->SetSetting(SETTING_UINT_HIDETEMPLATEMENU, false);
            pCPlayer->BoxChat << sCustom.ChatNameWrapper(pCreature->GetName()) << " Template menu will be shown on next login" << std::endl;
        }
        else if (action == 200) // Change Level
        {
            if (pPlayer->getLevel() != 70)
                pCPlayer->AddGossipMenuItem(Icon::GEAR, "Level: 70", GOSSIP_SENDER_MAIN, 201);
            if (pPlayer->getLevel() != 49)
                pCPlayer->AddGossipMenuItem(Icon::GEAR, "Level: 49", GOSSIP_SENDER_MAIN, 202);
            if (pPlayer->getLevel() != 19)
                pCPlayer->AddGossipMenuItem(Icon::GEAR, "Level: 19", GOSSIP_SENDER_MAIN, 203);
            
            pCPlayer->AddGossipMenuItem(Icon::GEAR, "Back", GOSSIP_SENDER_MAIN, 0);
        }
        else if (action >= 201 && action <= 203)
        {
            for (uint8 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; ++i)
            {
                if (pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
                {
                    pCPlayer->BoxChat << sCustom.ChatNameWrapper(pCreature->GetName()) << " You cannot have any gear equipped while changing level" << std::endl;
                    return true;
                }
            }

            if (action == 201)
                pPlayer->SetLevel(70);
            else if (action == 202)
                pPlayer->SetLevel(49);
            else if (action == 203)
                pPlayer->SetLevel(19);

            pPlayer->resetTalents(true);
        }
        else if (action == 300)
        {
            pCPlayer->AddGossipMenuItem(Icon::BAG, "Starter Items   ", GOSSIP_SENDER_MAIN, 301);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "Season Items    ", GOSSIP_SENDER_MAIN, 302);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "PvE Items       ", GOSSIP_SENDER_MAIN, 303);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "Gems            ", GOSSIP_SENDER_MAIN, 304);
            pCPlayer->AddGossipMenuItem(Icon::BAG, "Jewelry         ", GOSSIP_SENDER_MAIN, 305);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "Cloaks          ", GOSSIP_SENDER_MULTIVENDOR, 81023);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "Relics          ", GOSSIP_SENDER_MULTIVENDOR, 82016);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "Offhands/Shields", GOSSIP_SENDER_MULTIVENDOR, 82034);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "Tabards         ", GOSSIP_SENDER_MULTIVENDOR, 81024);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "Profession Goods", GOSSIP_SENDER_MULTIVENDOR, 81034);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "General Goods   ", GOSSIP_SENDER_MULTIVENDOR, 81027);

            if (pPlayer->getClass() == CLASS_ROGUE)
                pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "Rogue Supplies ", GOSSIP_SENDER_MULTIVENDOR, 81026);
            else if (pPlayer->getClass() == CLASS_WARLOCK)
                pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "Pet Spell books", GOSSIP_SENDER_MULTIVENDOR, 81037);

            pCPlayer->AddGossipMenuItem(Icon::GEAR, "Back            ", GOSSIP_SENDER_MAIN, 0);
        }
        else if (action == 301)
        {
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "Starter Weapons ", GOSSIP_SENDER_MULTIVENDOR, 82022);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "Starter Mainset ", GOSSIP_SENDER_MULTIVENDOR, 82020);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "Starter Offset  ", GOSSIP_SENDER_MULTIVENDOR, 82021);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "Starter Jewelery", GOSSIP_SENDER_MULTIVENDOR, 82015);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "Starter Gems    ", GOSSIP_SENDER_MULTIVENDOR, 82027);
            pCPlayer->AddGossipMenuItem(Icon::GEAR, "Back            ", GOSSIP_SENDER_MAIN, 300);
        }
        else if (action == 302)
        {
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "S1: Gladiator ", GOSSIP_SENDER_MULTIVENDOR, 81000);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "S1: General   ", GOSSIP_SENDER_MULTIVENDOR, 81001);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "S2: Merciless ", GOSSIP_SENDER_MULTIVENDOR, 81002);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "S2: Veteran   ", GOSSIP_SENDER_MULTIVENDOR, 81003);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "S3: Vengeful  ", GOSSIP_SENDER_MULTIVENDOR, 81035);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "S3: Vindicator", GOSSIP_SENDER_MULTIVENDOR, 81005);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "S4: Brutal    ", GOSSIP_SENDER_MULTIVENDOR, 81006);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "S4: Guardian  ", GOSSIP_SENDER_MULTIVENDOR, 81029);
            pCPlayer->AddGossipMenuItem(Icon::GEAR, "Back          ", GOSSIP_SENDER_MAIN, 300);
        }
        else if (action == 303)
        {
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "T6: Mainset      ", GOSSIP_SENDER_MULTIVENDOR, 82035);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "T6: Offset       ", GOSSIP_SENDER_MULTIVENDOR, 82037);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "MISC: PvE Gear   ", GOSSIP_SENDER_MULTIVENDOR, 81036);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "MISC: PvE Weapons", GOSSIP_SENDER_MULTIVENDOR, 81033);
            pCPlayer->AddGossipMenuItem(Icon::GEAR, "Back             ", GOSSIP_SENDER_MAIN, 300);
        }
        else if (action == 304)
        {
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, MSG_COLOR_WHITE   "Meta  ", GOSSIP_SENDER_MULTIVENDOR, 81028);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, MSG_COLOR_RED     "Red   ", GOSSIP_SENDER_MULTIVENDOR, 81014);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, MSG_COLOR_ORANGE  "Orange", GOSSIP_SENDER_MULTIVENDOR, 81009);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, MSG_COLOR_YELLOW  "Yellow", GOSSIP_SENDER_MULTIVENDOR, 81012);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, MSG_COLOR_GREEN   "Green ", GOSSIP_SENDER_MULTIVENDOR, 81010);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, MSG_COLOR_BLUE    "Blue  ", GOSSIP_SENDER_MULTIVENDOR, 81013);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, MSG_COLOR_PURPLE  "Purple", GOSSIP_SENDER_MULTIVENDOR, 81011);
            pCPlayer->AddGossipMenuItem(Icon::GEAR, "Back             ", GOSSIP_SENDER_MAIN, 300);
        }
        else if (action == 305)
        {
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "Trinkets ", GOSSIP_SENDER_MULTIVENDOR, 81022);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "Rings    ", GOSSIP_SENDER_MULTIVENDOR, 81025);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG, "Necklaces", GOSSIP_SENDER_MULTIVENDOR, 81021);
            pCPlayer->AddGossipMenuItem(Icon::GEAR, "Back     ", GOSSIP_SENDER_MAIN, 300);
        }
        else if (action == 400)
        {
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Warsong Gulch   ", GOSSIP_SENDER_MAIN, 401);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Arathi Basin    ", GOSSIP_SENDER_MAIN, 402);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Eye of the Storm", GOSSIP_SENDER_MAIN, 403);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Alterac Valley  ", GOSSIP_SENDER_MAIN, 404);

            if (Group* grp = pPlayer->GetGroup())
            {
                for (uint8 i = 0; i < 3; ++i)
                {
                    if (ArenaTeam* at = sObjectMgr.GetArenaTeamById(pPlayer->GetArenaTeamId(i)))
                    {
                        bool showrated = true;

                        for (Group::MemberSlotList::const_iterator itr = grp->GetMemberSlots().begin(); itr != grp->GetMemberSlots().end(); ++itr)
                        {
                            if (Player* member = sObjectMgr.GetPlayer(itr->guid))
                            {
                                if (sObjectMgr.GetArenaTeamById(member->GetArenaTeamId(i)) != at)
                                    showrated = false;
                            }
                            else
                                showrated = false;
                        }

                        if (showrated)
                        {
                            if (i == 0)
                                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Rated 2v2", GOSSIP_SENDER_MAIN, 405);
                            else if (i == 1)
                                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Rated 3v3", GOSSIP_SENDER_MAIN, 406);
                            else if (i == 2)
                                pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Rated 5v5", GOSSIP_SENDER_MAIN, 407);
                        }
                    }
                }
            }

            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Skirmish 2v2", GOSSIP_SENDER_MAIN, 408);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Skirmish 3v3", GOSSIP_SENDER_MAIN, 409);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS, "Skirmish 5v5", GOSSIP_SENDER_MAIN, 410);
            pCPlayer->AddGossipMenuItem(Icon::GEAR, "Back        ", GOSSIP_SENDER_MAIN, 0);
        }
        else if (action >= 401 && action <= 404)
        {
            BattleGroundTypeId bgtype = BATTLEGROUND_TYPE_NONE;

            switch (action)
            {
            case 401: bgtype = BATTLEGROUND_WS; break;
            case 402: bgtype = BATTLEGROUND_AB; break;
            case 403: bgtype = BATTLEGROUND_EY; break;
            case 404: bgtype = BATTLEGROUND_AV; break;
            }

            WorldPacket data;
            data.SetOpcode(CMSG_BATTLEMASTER_JOIN);
            data << ObjectGuid(pCreature->GetObjectGuid());
            data << uint32(bgtype);
            data << uint32(0);
            data << uint8(pPlayer->GetGroup());
            pPlayer->GetSession()->HandleBattlemasterJoinOpcode(data);
        }
        else if (action >= 405 && action <= 410)
        {
            uint8 arenaslot = 0; // 2v2 default
            bool rated = false;

            switch (action)
            {
            case 406:
            case 409:
                arenaslot = 1;
                break;
            case 407:
            case 410:
                arenaslot = 2;
                break;
            }

            if (action <= 407)
                rated = true;

            WorldPacket data;
            data.SetOpcode(CMSG_BATTLEMASTER_JOIN_ARENA);
            data << ObjectGuid(pCreature->GetObjectGuid());
            data << uint8(arenaslot);
            data << uint8(pPlayer->GetGroup());
            data << uint8(rated);
            pPlayer->GetSession()->HandleBattlemasterJoinArena(data);
        }
        else if (action == 1337)
        {
            pCPlayer->AddGossipMenuItem(Icon::GEAR, "Send boxmessage", CHAT_BOX, 1339);
            pCPlayer->AddGossipMenuItem(Icon::GEAR, "Send widemessage", CHAT_WIDE, 1339);
            pCPlayer->AddGossipMenuItem(Icon::GEAR, "Send bothmessage", CHAT_BOTH, 1339);
            pCPlayer->AddGossipMenuItem(Icon::GEAR, "Show all icons", GOSSIP_SENDER_MAIN, 1340);
        }
        else if (action == 1339)
        {
            if (sender == CHAT_BOX)
                pCPlayer->BoxChat << "Boxchat" << std::endl;
            else if (sender == CHAT_WIDE)
                pCPlayer->WideChat << "WideChat" << std::endl;
            else if (sender == CHAT_BOTH)
                pCPlayer->BothChat << "BothChat" << std::endl;
        }
        else if (action == 1340)
        {
            pCPlayer->AddGossipMenuItem(Icon::CHAT,     "Icon::CHAT     ", GOSSIP_SENDER_MAIN, 0);
            pCPlayer->AddGossipMenuItem(Icon::BAG,      "Icon::BAG      ", GOSSIP_SENDER_MAIN, 0);
            pCPlayer->AddGossipMenuItem(Icon::TAXI,     "Icon::TAXI     ", GOSSIP_SENDER_MAIN, 0);
            pCPlayer->AddGossipMenuItem(Icon::TRAINER,  "Icon::TRAINER  ", GOSSIP_SENDER_MAIN, 0);
            pCPlayer->AddGossipMenuItem(Icon::GEAR,     "Icon::GEAR     ", GOSSIP_SENDER_MAIN, 0);
            pCPlayer->AddGossipMenuItem(Icon::MONEY_BAG,"Icon::MONEY_BAG", GOSSIP_SENDER_MAIN, 0);
            pCPlayer->AddGossipMenuItem(Icon::CHAT_DOTS,"Icon::CHAT_DOTS", GOSSIP_SENDER_MAIN, 0);
            pCPlayer->AddGossipMenuItem(Icon::TABARD,   "Icon::TABARD   ", GOSSIP_SENDER_MAIN, 0);
            pCPlayer->AddGossipMenuItem(Icon::SWORDS,   "Icon::SWORDS   ", GOSSIP_SENDER_MAIN, 0);
        }

        if (pPlayer->PlayerTalkClass->GetGossipMenu().MenuItemCount() > 0)
            pPlayer->PlayerTalkClass->SendGossipMenu("Your wish, my law!", pCreature->GetObjectGuid());

        return true;
    }
};

void AddSC_npc_teleporter()
{
    new NPC_Teleporter;
}