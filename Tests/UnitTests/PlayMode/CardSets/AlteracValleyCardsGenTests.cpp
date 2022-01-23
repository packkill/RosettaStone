// Copyright (c) 2017-2021 Chris Ohk

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include "doctest_proxy.hpp"

#include <Utils/CardSetUtils.hpp>
#include <Utils/TestUtils.hpp>

#include <Rosetta/PlayMode/Actions/Draw.hpp>
#include <Rosetta/PlayMode/Cards/Cards.hpp>
#include <Rosetta/PlayMode/Zones/DeckZone.hpp>
#include <Rosetta/PlayMode/Zones/FieldZone.hpp>
#include <Rosetta/PlayMode/Zones/HandZone.hpp>
#include <Rosetta/PlayMode/Zones/SecretZone.hpp>

using namespace RosettaStone;
using namespace PlayMode;
using namespace PlayerTasks;
using namespace SimpleTasks;

// ----------------------------------------- MINION - DRUID
// [AV_211] Dire Frostwolf - COST:4 [ATK:4/HP:4]
// - Race: Beast, Set: ALTERAC_VALLEY, Rarity: Common
// --------------------------------------------------------
// Text: <b>Stealth</b>
//       <b>Deathrattle:</b> Summon a 2/2 Wolf with <b>Stealth</b>.
// --------------------------------------------------------
// GameTag:
// - DEATHRATTLE = 1
// - STEALTH = 1
// --------------------------------------------------------
TEST_CASE("[Druid : Minion] - AV_211 : Dire Frostwolf")
{
    GameConfig config;
    config.player1Class = CardClass::DRUID;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curField = *(curPlayer->GetFieldZone());

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Dire Frostwolf"));
    const auto card2 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Flamestrike"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, PlayCardTask::Spell(card2));
    CHECK_EQ(curField.GetCount(), 1);
    CHECK_EQ(curField[0]->card->name, "Frostwolf Cub");
    CHECK_EQ(curField[0]->GetAttack(), 2);
    CHECK_EQ(curField[0]->GetHealth(), 2);
    CHECK_EQ(curField[0]->HasStealth(), true);
}

// ------------------------------------------ SPELL - DRUID
// [AV_360] Frostwolf Kennels - COST:3
// - Set: ALTERAC_VALLEY, Rarity: Rare
// --------------------------------------------------------
// Text: At the end of your turn, summon a 2/2 Wolf
//       with <b>Stealth</b>. Lasts 3 turns.
// --------------------------------------------------------
// RefTag:
// - STEALTH = 1
// --------------------------------------------------------
TEST_CASE("[Druid : Spell] - AV_360 : Frostwolf Kennels")
{
    GameConfig config;
    config.player1Class = CardClass::DRUID;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curField = *(curPlayer->GetFieldZone());

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::FindCardByName("Frostwolf Kennels"));

    game.Process(curPlayer, PlayCardTask::Spell(card1));
    CHECK_EQ(curField.GetCount(), 0);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curField.GetCount(), 1);
    CHECK_EQ(curField[0]->card->name, "Frostwolf Cub");
    CHECK_EQ(curField[0]->GetAttack(), 2);
    CHECK_EQ(curField[0]->GetHealth(), 2);
    CHECK_EQ(curField[0]->HasStealth(), true);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curField.GetCount(), 1);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curField.GetCount(), 2);
    CHECK_EQ(curField[1]->card->name, "Frostwolf Cub");
    CHECK_EQ(curField[1]->GetAttack(), 2);
    CHECK_EQ(curField[1]->GetHealth(), 2);
    CHECK_EQ(curField[1]->HasStealth(), true);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curField.GetCount(), 2);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curField.GetCount(), 3);
    CHECK_EQ(curField[2]->card->name, "Frostwolf Cub");
    CHECK_EQ(curField[2]->GetAttack(), 2);
    CHECK_EQ(curField[2]->GetHealth(), 2);
    CHECK_EQ(curField[2]->HasStealth(), true);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curField.GetCount(), 3);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curField.GetCount(), 3);
}

// ----------------------------------------- SPELL - HUNTER
// [AV_147] Dun Baldar Bunker - COST:2
// - Set: ALTERAC_VALLEY, Rarity: Rare
// --------------------------------------------------------
// Text: At the end of your turn, draw a <b>Secret</b>
//       and set its Cost to (1). Lasts 3 turns.
// --------------------------------------------------------
// RefTag:
// - SECRET = 1
// --------------------------------------------------------
TEST_CASE("[Hunter : Spell] - AV_147 : Dun Baldar Bunker")
{
    GameConfig config;
    config.player1Class = CardClass::HUNTER;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    for (int i = 0; i < 30; i += 3)
    {
        config.player1Deck[i] = Cards::FindCardByName("Snake Trap");
        config.player1Deck[i + 1] = Cards::FindCardByName("Holy Light");
        config.player1Deck[i + 2] = Cards::FindCardByName("Wisp");
    }

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curDeck = *(curPlayer->GetDeckZone());
    auto& curHand = *(curPlayer->GetHandZone());

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::FindCardByName("Dun Baldar Bunker"));

    game.Process(curPlayer, PlayCardTask::Spell(card1));
    CHECK_EQ(curDeck.GetCount(), 26);
    CHECK_EQ(curHand.GetCount(), 4);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curDeck.GetCount(), 25);
    CHECK_EQ(curHand.GetCount(), 5);
    CHECK_EQ(curHand[4]->card->IsSecret(), true);
    CHECK_EQ(curHand[4]->GetCost(), 1);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curDeck.GetCount(), 24);
    CHECK_EQ(curHand.GetCount(), 6);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curDeck.GetCount(), 23);
    CHECK_EQ(curHand.GetCount(), 7);
    CHECK_EQ(curHand[6]->card->IsSecret(), true);
    CHECK_EQ(curHand[6]->GetCost(), 1);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curDeck.GetCount(), 22);
    CHECK_EQ(curHand.GetCount(), 8);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curDeck.GetCount(), 21);
    CHECK_EQ(curHand.GetCount(), 9);
    CHECK_EQ(curHand[8]->card->IsSecret(), true);
    CHECK_EQ(curHand[8]->GetCost(), 1);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curDeck.GetCount(), 20);
    CHECK_EQ(curHand.GetCount(), 10);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curDeck.GetCount(), 20);
    CHECK_EQ(curHand.GetCount(), 10);
}

// ----------------------------------------- SPELL - HUNTER
// [AV_224] Spring the Trap - COST:4
// - Set: ALTERAC_VALLEY, Rarity: Epic
// --------------------------------------------------------
// Text: Deal 3 damage to a minion and cast a <b>Secret</b>
//       from your deck. <b>Honorable Kill:</b> Cast 2.
// --------------------------------------------------------
// PlayReq:
// - REQ_TARGET_TO_PLAY = 0
// - REQ_MINION_TARGET = 0
// --------------------------------------------------------
// RefTag:
// - HONORABLEKILL = 1
// - SECRET = 1
// --------------------------------------------------------
TEST_CASE("[Hunter : Spell] - AV_224 : Spring the Trap")
{
    GameConfig config;
    config.player1Class = CardClass::HUNTER;
    config.player2Class = CardClass::SHAMAN;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;
    config.doShuffle = false;

    for (int i = 0; i < 30; i += 3)
    {
        config.player1Deck[i] = Cards::FindCardByName("Snake Trap");
        config.player1Deck[i + 1] = Cards::FindCardByName("Explosive Trap");
        config.player1Deck[i + 2] = Cards::FindCardByName("Freezing Trap");
    }

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curDeck = *(curPlayer->GetDeckZone());
    auto& curSecret = *(curPlayer->GetSecretZone());

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Spring the Trap"));
    const auto card2 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Spring the Trap"));
    const auto card3 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Strongman"));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, PlayCardTask::Minion(card3));

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curDeck.GetCount(), 25);

    game.Process(curPlayer, PlayCardTask::SpellTarget(card1, card3));
    CHECK_EQ(curDeck.GetCount(), 24);
    CHECK_EQ(curSecret.GetCount(), 1);

    game.Process(curPlayer, PlayCardTask::SpellTarget(card2, card3));
    CHECK_EQ(curDeck.GetCount(), 22);
    CHECK_EQ(curSecret.GetCount(), 3);
}

// ---------------------------------------- WEAPON - HUNTER
// [AV_244] Bloodseeker - COST:2
// - Set: ALTERAC_VALLEY, Rarity: Common
// --------------------------------------------------------
// Text: <b>Honorable Kill:</b> Gain +1/+1.
// --------------------------------------------------------
// GameTag:
// - HONORABLEKILL = 1
// - TRIGGER_VISUAL = 1
// --------------------------------------------------------
TEST_CASE("[Hunter : Weapon] - AV_244 : Bloodseeker")
{
    GameConfig config;
    config.player1Class = CardClass::HUNTER;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Bloodseeker"));
    const auto card2 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Wisp"));
    const auto card3 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Intrepid Initiate"));

    game.Process(curPlayer, PlayCardTask::Weapon(card1));
    CHECK_EQ(curPlayer->GetHero()->weapon->GetAttack(), 2);
    CHECK_EQ(curPlayer->GetHero()->weapon->GetDurability(), 2);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, PlayCardTask::Minion(card2));
    game.Process(opPlayer, PlayCardTask::Minion(card3));

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(curPlayer, AttackTask(curPlayer->GetHero(), card2));
    CHECK_EQ(curPlayer->GetHero()->weapon->GetAttack(), 2);
    CHECK_EQ(curPlayer->GetHero()->weapon->GetDurability(), 1);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(curPlayer, AttackTask(curPlayer->GetHero(), card3));
    CHECK_EQ(curPlayer->GetHero()->HasWeapon(), true);
    CHECK_EQ(curPlayer->GetHero()->weapon->GetAttack(), 3);
    CHECK_EQ(curPlayer->GetHero()->weapon->GetDurability(), 1);
}

// ---------------------------------------- MINION - HUNTER
// [AV_334] Stormpike Battle Ram - COST:4 [ATK:4/HP:3]
// - Race: Beast, Set: ALTERAC_VALLEY, Rarity: Rare
// --------------------------------------------------------
// Text: <b>Rush</b>
//       <b>Deathrattle:</b> Your next Beast costs (2) less.
// --------------------------------------------------------
// GameTag:
// - DEATHRATTLE = 1
// - RUSH = 1
// --------------------------------------------------------
TEST_CASE("[Hunter : Minion] - AV_334 : Stormpike Battle Ram")
{
    GameConfig config;
    config.player1Class = CardClass::SHAMAN;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::FindCardByName("Stormpike Battle Ram"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::FindCardByName("Unbound Elemental"));
    const auto card3 = Generic::DrawCard(
        curPlayer, Cards::FindCardByName("Stranglethorn Tiger"));
    const auto card4 = Generic::DrawCard(
        curPlayer, Cards::FindCardByName("Stranglethorn Tiger"));
    const auto card5 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Fireball"));

    CHECK_EQ(card2->GetCost(), 3);
    CHECK_EQ(card3->GetCost(), 5);
    CHECK_EQ(card4->GetCost(), 5);

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    CHECK_EQ(card2->GetCost(), 3);
    CHECK_EQ(card3->GetCost(), 5);
    CHECK_EQ(card4->GetCost(), 5);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, PlayCardTask::SpellTarget(card5, card1));
    CHECK_EQ(card2->GetCost(), 3);
    CHECK_EQ(card3->GetCost(), 3);
    CHECK_EQ(card4->GetCost(), 3);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(curPlayer, PlayCardTask::Minion(card3));
    CHECK_EQ(curPlayer->GetRemainingMana(), 7);
    CHECK_EQ(card2->GetCost(), 3);
    CHECK_EQ(card4->GetCost(), 5);
}

// ---------------------------------------- MINION - HUNTER
// [AV_335] Ram Tamer - COST:3 [ATK:4/HP:3]
// - Set: ALTERAC_VALLEY, Rarity: Common
// --------------------------------------------------------
// Text: <b>Battlecry:</b> If you control a <b>Secret</b>,
//       gain +1/+1 and <b>Stealth</b>.
// --------------------------------------------------------
// GameTag:
// - BATTLECRY = 1
// --------------------------------------------------------
// RefTag:
// - SECRET = 1
// - STEALTH = 1
// --------------------------------------------------------
TEST_CASE("[Hunter : Minion] - AV_335 : Ram Tamer")
{
    GameConfig config;
    config.formatType = FormatType::STANDARD;
    config.player1Class = CardClass::PALADIN;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curField = *(curPlayer->GetFieldZone());

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Ram Tamer"));
    const auto card2 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Ram Tamer"));
    const auto card3 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Snake Trap"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    CHECK_EQ(curField[0]->GetAttack(), 4);
    CHECK_EQ(curField[0]->GetHealth(), 3);
    CHECK_EQ(curField[0]->HasStealth(), false);

    game.Process(curPlayer, PlayCardTask::Spell(card3));
    game.Process(curPlayer, PlayCardTask::Minion(card2));
    CHECK_EQ(curField[1]->GetAttack(), 5);
    CHECK_EQ(curField[1]->GetHealth(), 4);
    CHECK_EQ(curField[1]->HasStealth(), true);
}

// ---------------------------------------- MINION - HUNTER
// [AV_337] Mountain Bear - COST:7 [ATK:5/HP:6]
// - Race: Beast, Set: ALTERAC_VALLEY, Rarity: Common
// --------------------------------------------------------
// Text: <b>Taunt</b>
//       <b>Deathrattle:</b> Summon two 2/4 Cubs with <b>Taunt</b>.
// --------------------------------------------------------
// GameTag:
// - DEATHRATTLE = 1
// - TAUNT = 1
// --------------------------------------------------------
TEST_CASE("[Hunter : Minion] - AV_337 : Mountain Bear")
{
    GameConfig config;
    config.player1Class = CardClass::HUNTER;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curField = *(curPlayer->GetFieldZone());

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Mountain Bear"));
    const auto card2 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Fireball"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, PlayCardTask::SpellTarget(card2, card1));
    CHECK_EQ(curField.GetCount(), 2);
    CHECK_EQ(curField[0]->card->name, "Mountain Cub");
    CHECK_EQ(curField[0]->GetAttack(), 2);
    CHECK_EQ(curField[0]->GetHealth(), 4);
    CHECK_EQ(curField[0]->HasTaunt(), true);
    CHECK_EQ(curField[1]->card->name, "Mountain Cub");
    CHECK_EQ(curField[1]->GetAttack(), 2);
    CHECK_EQ(curField[1]->GetHealth(), 4);
    CHECK_EQ(curField[1]->HasTaunt(), true);
}

// ------------------------------------------- SPELL - MAGE
// [AV_218] Mass Polymorph - COST:7
// - Set: ALTERAC_VALLEY, Rarity: Epic
// - Spell School: Arcane
// --------------------------------------------------------
// Text: Transform all minions into 1/1 Sheep.
// --------------------------------------------------------
TEST_CASE("[Mage : Spell] - AV_218 : Mass Polymorph")
{
    GameConfig config;
    config.player1Class = CardClass::MAGE;
    config.player2Class = CardClass::DEMONHUNTER;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curField = *(curPlayer->GetFieldZone());
    auto& opField = *(opPlayer->GetFieldZone());

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Mass Polymorph"));
    const auto card2 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Wolfrider"));
    const auto card3 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Wisp"));
    const auto card4 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Malygos"));

    game.Process(curPlayer, PlayCardTask::Minion(card2));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, PlayCardTask::Minion(card3));
    game.Process(opPlayer, PlayCardTask::Minion(card4));

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(curPlayer, PlayCardTask::Spell(card1));
    CHECK_EQ(curField.GetCount(), 1);
    CHECK_EQ(curField[0]->card->name, "Sheep");
    CHECK_EQ(curField[0]->GetAttack(), 1);
    CHECK_EQ(curField[0]->GetHealth(), 1);
    CHECK_EQ(opField.GetCount(), 2);
    CHECK_EQ(opField[0]->card->name, "Sheep");
    CHECK_EQ(opField[0]->GetAttack(), 1);
    CHECK_EQ(opField[0]->GetHealth(), 1);
    CHECK_EQ(opField[1]->card->name, "Sheep");
    CHECK_EQ(opField[1]->GetAttack(), 1);
    CHECK_EQ(opField[1]->GetHealth(), 1);
}

// ----------------------------------------- MINION - ROGUE
// [AV_201] Coldtooth Yeti - COST:3 [ATK:1/HP:5]
// - Set: ALTERAC_VALLEY, Rarity: Common
// --------------------------------------------------------
// Text: <b>Combo:</b> Gain +3 Attack.
// --------------------------------------------------------
// GameTag:
// - COMBO = 1
// --------------------------------------------------------
TEST_CASE("[Rogue : Minion] - AV_201 : Coldtooth Yeti")
{
    GameConfig config;
    config.player1Class = CardClass::ROGUE;
    config.player2Class = CardClass::WARRIOR;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curField = *(curPlayer->GetFieldZone());

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Coldtooth Yeti"));
    const auto card2 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Coldtooth Yeti"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    CHECK_EQ(curField[0]->GetAttack(), 1);
    CHECK_EQ(curField[0]->GetHealth(), 5);

    game.Process(curPlayer, PlayCardTask::Minion(card2));
    CHECK_EQ(curField[1]->GetAttack(), 4);
    CHECK_EQ(curField[1]->GetHealth(), 5);
}

// ---------------------------------------- MINION - SHAMAN
// [AV_260] Sleetbreaker - COST:2 [ATK:3/HP:2]
// - Race: Elemental, Set: ALTERAC_VALLEY, Rarity: Rare
// --------------------------------------------------------
// Text: <b>Battlecry:</b> Add a Windchill to your hand.
// --------------------------------------------------------
// GameTag:
// - BATTLECRY = 1
// --------------------------------------------------------
TEST_CASE("[Shaman : Minion] - AV_260 : Sleetbreaker")
{
    GameConfig config;
    config.player1Class = CardClass::SHAMAN;
    config.player2Class = CardClass::WARRIOR;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curHand = *(curPlayer->GetHandZone());

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Sleetbreaker"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    CHECK_EQ(curHand.GetCount(), 1);
    CHECK_EQ(curHand[0]->card->name, "Windchill");
}

// ----------------------------------------- SPELL - SHAMAN
// [AV_266] Windchill - COST:1
// - Set: ALTERAC_VALLEY, Rarity: Common
// - Spell School: Frost
// --------------------------------------------------------
// Text: <b>Freeze</b> a minion. Draw a card.
// --------------------------------------------------------
// PlayReq:
// - REQ_TARGET_TO_PLAY = 0
// - REQ_MINION_TARGET = 0
// --------------------------------------------------------
// RefTag:
// - FREEZE = 1
// --------------------------------------------------------
TEST_CASE("[Shaman : Spell] - AV_266 : Windchill")
{
    GameConfig config;
    config.player1Class = CardClass::SHAMAN;
    config.player2Class = CardClass::WARRIOR;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curHand = *(curPlayer->GetHandZone());
    auto& opField = *(opPlayer->GetFieldZone());

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Windchill"));
    const auto card2 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Wolfrider"));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, PlayCardTask::Minion(card2));
    CHECK_EQ(opField[0]->IsFrozen(), false);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curHand.GetCount(), 6);

    game.Process(curPlayer, PlayCardTask::SpellTarget(card1, card2));
    CHECK_EQ(opField[0]->IsFrozen(), true);
    CHECK_EQ(curHand.GetCount(), 6);
}

// ----------------------------------------- SPELL - SHAMAN
// [AV_268] Wildpaw Cavern - COST:4
// - Set: ALTERAC_VALLEY, Rarity: Rare
// --------------------------------------------------------
// Text: At the end of your turn, summon a 3/4 Elemental
//       that <b>Freezes</b>. Lasts 3 turns.
// --------------------------------------------------------
// RefTag:
// - FREEZE = 1
// --------------------------------------------------------
TEST_CASE("[Shaman : Spell] - AV_268 : Wildpaw Cavern")
{
    GameConfig config;
    config.player1Class = CardClass::SHAMAN;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curField = *(curPlayer->GetFieldZone());

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Wildpaw Cavern"));

    game.Process(curPlayer, PlayCardTask::Spell(card1));
    CHECK_EQ(curField.GetCount(), 0);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curField.GetCount(), 1);
    CHECK_EQ(curField[0]->card->name, "Frozen Stagguard");
    CHECK_EQ(curField[0]->GetAttack(), 3);
    CHECK_EQ(curField[0]->GetHealth(), 4);
    CHECK_EQ(curField[0]->HasFreeze(), true);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curField.GetCount(), 1);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curField.GetCount(), 2);
    CHECK_EQ(curField[1]->card->name, "Frozen Stagguard");
    CHECK_EQ(curField[1]->GetAttack(), 3);
    CHECK_EQ(curField[1]->GetHealth(), 4);
    CHECK_EQ(curField[1]->HasFreeze(), true);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curField.GetCount(), 2);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curField.GetCount(), 3);
    CHECK_EQ(curField[2]->card->name, "Frozen Stagguard");
    CHECK_EQ(curField[2]->GetAttack(), 3);
    CHECK_EQ(curField[2]->GetHealth(), 4);
    CHECK_EQ(curField[2]->HasFreeze(), true);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curField.GetCount(), 3);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curField.GetCount(), 3);
}

// ---------------------------------------- SPELL - WARLOCK
// [AV_277] Seeds of Destruction - COST:2
// - Set: ALTERAC_VALLEY, Rarity: Rare
// - Spell School: Fel
// --------------------------------------------------------
// Text: Shuffle four Rifts into your deck.
//       They summon a 3/3 Dread Imp when drawn.
// --------------------------------------------------------
TEST_CASE("[Warlock : Spell] - AV_277 : Seeds of Destruction")
{
    GameConfig config;
    config.player1Class = CardClass::WARLOCK;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curField = *(curPlayer->GetFieldZone());
    auto& curDeck = *(curPlayer->GetDeckZone());

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::FindCardByName("Seeds of Destruction"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    CHECK_EQ(curField.GetCount(), 0);
    CHECK_EQ(curDeck.GetCount(), 4);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curField.GetCount(), 4);
    CHECK_EQ(curField[0]->card->name, "Dread Imp");
    CHECK_EQ(curField[0]->GetAttack(), 3);
    CHECK_EQ(curField[0]->GetHealth(), 3);
    CHECK_EQ(curField[1]->card->name, "Dread Imp");
    CHECK_EQ(curField[1]->GetAttack(), 3);
    CHECK_EQ(curField[1]->GetHealth(), 3);
    CHECK_EQ(curField[2]->card->name, "Dread Imp");
    CHECK_EQ(curField[2]->GetAttack(), 3);
    CHECK_EQ(curField[2]->GetHealth(), 3);
    CHECK_EQ(curField[3]->card->name, "Dread Imp");
    CHECK_EQ(curField[3]->GetAttack(), 3);
    CHECK_EQ(curField[3]->GetHealth(), 3);
}

// ---------------------------------------- SPELL - WARLOCK
// [AV_281] Felfire in the Hole! - COST:5
// - Set: ALTERAC_VALLEY, Rarity: Epic
// - Spell School: Fel
// --------------------------------------------------------
// Text: Draw a spell and deal 2 damage to all enemies.
//       If it's a Fel spell, deal 1 more.
// --------------------------------------------------------
TEST_CASE("[Warlock : Spell] - AV_281 : Felfire in the Hole!")
{
    GameConfig config;
    config.player1Class = CardClass::WARLOCK;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;
    config.doShuffle = false;

    for (int i = 0; i < 5; ++i)
    {
        config.player1Deck[i] = Cards::FindCardByName("Felfire in the Hole!");
    }

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curHand = *(curPlayer->GetHandZone());

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::FindCardByName("Felfire in the Hole!"));
    const auto card2 = Generic::DrawCard(
        curPlayer, Cards::FindCardByName("Felfire in the Hole!"));

    opPlayer->GetHero()->SetDamage(0);

    game.Process(curPlayer, PlayCardTask::Spell(card1));
    CHECK_EQ(curHand.GetCount(), 6);
    CHECK_EQ(curHand[5]->card->GetSpellSchool(), SpellSchool::FEL);
    CHECK_EQ(opPlayer->GetHero()->GetHealth(), 27);

    game.Process(curPlayer, PlayCardTask::Spell(card2));
    CHECK_EQ(curHand.GetCount(), 5);
    CHECK_EQ(opPlayer->GetHero()->GetHealth(), 25);
}

// ---------------------------------------- SPELL - WARLOCK
// [AV_285] Full-Blown Evil - COST:3
// - Set: ALTERAC_VALLEY, Rarity: Rare
// - Spell School: Fel
// --------------------------------------------------------
// Text: Deal 5 damage randomly split among all enemy minions.
//       Repeatable this turn.
// --------------------------------------------------------
// GameTag:
// - ImmuneToSpellpower = 1
// --------------------------------------------------------
TEST_CASE("[Warlock : Spell] - AV_285 : Full-Blown Evil")
{
    GameConfig config;
    config.player1Class = CardClass::WARLOCK;
    config.player2Class = CardClass::WARRIOR;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curHand = *(curPlayer->GetHandZone());
    auto& opField = *(opPlayer->GetFieldZone());

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Full-Blown Evil"));
    const auto card2 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Malygos"));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, PlayCardTask::Minion(card2));
    CHECK_EQ(opField[0]->GetHealth(), 12);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(curPlayer, PlayCardTask::Spell(card1));
    CHECK_EQ(opField[0]->GetHealth(), 7);
    CHECK_EQ(curHand.GetCount(), 1);
    CHECK_EQ(curHand[0]->card->name, "Full-Blown Evil");
    CHECK_EQ(curHand[0]->GetGameTag(GameTag::GHOSTLY), 1);

    game.Process(curPlayer, PlayCardTask::Spell(curHand[0]));
    CHECK_EQ(opField[0]->GetHealth(), 2);
    CHECK_EQ(curHand.GetCount(), 1);
    CHECK_EQ(curHand[0]->card->name, "Full-Blown Evil");
    CHECK_EQ(curHand[0]->GetGameTag(GameTag::GHOSTLY), 1);

    game.Process(curPlayer, PlayCardTask::Spell(curHand[0]));
    CHECK_EQ(opField.GetCount(), 0);
    CHECK_EQ(curHand.GetCount(), 1);
    CHECK_EQ(curHand[0]->card->name, "Full-Blown Evil");
    CHECK_EQ(curHand[0]->GetGameTag(GameTag::GHOSTLY), 1);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curHand.GetCount(), 0);
}

// --------------------------------------- MINION - WARLOCK
// [AV_308] Grave Defiler - COST:1 [ATK:2/HP:1]
// - Set: ALTERAC_VALLEY, Rarity: Common
// --------------------------------------------------------
// Text: <b>Battlecry:</b> Copy a Fel spell in your hand.
// --------------------------------------------------------
// GameTag:
// - BATTLECRY = 1
// --------------------------------------------------------
TEST_CASE("[Warlock : Minion] - AV_308 : Grave Defiler")
{
    GameConfig config;
    config.player1Class = CardClass::WARLOCK;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curHand = *(curPlayer->GetHandZone());

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Grave Defiler"));
    [[maybe_unused]] const auto card2 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Fireball"));
    [[maybe_unused]] const auto card3 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Felosophy"));
    [[maybe_unused]] const auto card4 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Unstable Felbolt"));

    CHECK_EQ(curHand.GetCount(), 4);

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    CHECK_EQ(curHand.GetCount(), 4);
    const bool check = curHand[3]->card->name == "River Felosophy" ||
                       curHand[3]->card->name == "Unstable Felbolt";
    CHECK_EQ(check, true);
}

// ---------------------------------------- SPELL - WARRIOR
// [AV_109] Frozen Buckler - COST:2
// - Set: ALTERAC_VALLEY, Rarity: Epic
// - Spell School: Frost
// --------------------------------------------------------
// Text: Gain 10 Armor.
//       At the start of your next turn, lose 5 Armor.
// --------------------------------------------------------
TEST_CASE("[Warrior : Spell] - AV_109 : Frozen Buckler")
{
    GameConfig config;
    config.player1Class = CardClass::WARRIOR;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Frozen Buckler"));
    const auto card2 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Fireball"));

    game.Process(curPlayer, PlayCardTask::Spell(card1));
    CHECK_EQ(curPlayer->GetHero()->GetArmor(), 10);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer,
                 PlayCardTask::SpellTarget(card2, curPlayer->GetHero()));
    CHECK_EQ(curPlayer->GetHero()->GetArmor(), 4);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curPlayer->GetHero()->GetArmor(), 0);

    game.Process(curPlayer, HeroPowerTask());
    CHECK_EQ(curPlayer->GetHero()->GetArmor(), 2);
}

// --------------------------------------- MINION - WARRIOR
// [AV_321] Glory Chaser - COST:3 [ATK:4/HP:3]
// - Set: ALTERAC_VALLEY, Rarity: Common
// --------------------------------------------------------
// Text: After you play a <b>Taunt</b> minion, draw a card.
// --------------------------------------------------------
// GameTag:
// - TRIGGER_VISUAL = 1
// --------------------------------------------------------
// RefTag:
// - TAUNT = 1
// --------------------------------------------------------
TEST_CASE("[Warrior : Minion] - AV_321 : Glory Chaser")
{
    GameConfig config;
    config.player1Class = CardClass::WARRIOR;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curHand = *(curPlayer->GetHandZone());

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Glory Chaser"));
    const auto card2 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Wolfrider"));
    const auto card3 = Generic::DrawCard(
        curPlayer, Cards::FindCardByName("Sen'jin Shieldmasta"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    CHECK_EQ(curHand.GetCount(), 6);

    game.Process(curPlayer, PlayCardTask::Minion(card2));
    CHECK_EQ(curHand.GetCount(), 5);

    game.Process(curPlayer, PlayCardTask::Minion(card3));
    CHECK_EQ(curHand.GetCount(), 5);
}

// ---------------------------------------- SPELL - WARRIOR
// [AV_322] Snowed In - COST:3
// - Set: ALTERAC_VALLEY, Rarity: Epic
// - Spell School: Frost
// --------------------------------------------------------
// Text: Destroy a damaged minion.
//       <b>Freeze</b> all other minions.
// --------------------------------------------------------
// RefTag:
// - FREEZE = 1
// --------------------------------------------------------
// PlayReq:
// - REQ_TARGET_TO_PLAY = 0
// - REQ_MINION_TARGET = 0
// - REQ_DAMAGED_TARGET = 0
// --------------------------------------------------------
TEST_CASE("[Warrior : Spell] - AV_322 : Snowed In")
{
    GameConfig config;
    config.player1Class = CardClass::WARRIOR;
    config.player2Class = CardClass::DEMONHUNTER;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curField = *(curPlayer->GetFieldZone());
    auto& opField = *(opPlayer->GetFieldZone());

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Snowed In"));
    const auto card2 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Wolfrider"));
    const auto card3 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Wisp"));
    const auto card4 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Malygos"));

    game.Process(curPlayer, PlayCardTask::Minion(card2));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, PlayCardTask::Minion(card3));
    game.Process(opPlayer, PlayCardTask::Minion(card4));

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(curPlayer, PlayCardTask::SpellTarget(card1, card3));
    CHECK_EQ(opField.GetCount(), 2);

    opField[1]->SetDamage(1);

    game.Process(curPlayer, PlayCardTask::SpellTarget(card1, card4));
    CHECK_EQ(curField.GetCount(), 1);
    CHECK_EQ(curField[0]->IsFrozen(), true);
    CHECK_EQ(opField.GetCount(), 1);
    CHECK_EQ(opField[0]->IsFrozen(), true);
}

// --------------------------------------- MINION - WARRIOR
// [AV_323] Scrapsmith - COST:3 [ATK:2/HP:4]
// - Set: ALTERAC_VALLEY, Rarity: Common
// --------------------------------------------------------
// Text: <b>Taunt</b>
//       <b>Battlecry:</b> Add two 2/4 Grunts
//       with <b>Taunt</b> to your hand.
// --------------------------------------------------------
// GameTag:
// - BATTLECRY = 1
// - TAUNT = 1
// --------------------------------------------------------
TEST_CASE("[Warrior : Minion] - AV_323 : Scrapsmith")
{
    GameConfig config;
    config.player1Class = CardClass::WARRIOR;
    config.player2Class = CardClass::DEMONHUNTER;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curHand = *(curPlayer->GetHandZone());

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Scrapsmith"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    CHECK_EQ(curHand.GetCount(), 2);
    CHECK_EQ(curHand[0]->card->name, "Scrappy Grunt");
    CHECK_EQ(dynamic_cast<Minion*>(curHand[0])->GetAttack(), 2);
    CHECK_EQ(dynamic_cast<Minion*>(curHand[0])->GetHealth(), 4);
    CHECK_EQ(dynamic_cast<Minion*>(curHand[0])->HasTaunt(), true);
    CHECK_EQ(curHand[1]->card->name, "Scrappy Grunt");
    CHECK_EQ(dynamic_cast<Minion*>(curHand[1])->GetAttack(), 2);
    CHECK_EQ(dynamic_cast<Minion*>(curHand[1])->GetHealth(), 4);
    CHECK_EQ(dynamic_cast<Minion*>(curHand[1])->HasTaunt(), true);
}

// --------------------------------------- MINION - WARRIOR
// [AV_565] Axe Berserker - COST:4 [ATK:3/HP:5]
// - Set: ALTERAC_VALLEY, Rarity: Common
// --------------------------------------------------------
// Text: <b>Rush</b>. <b>Honorable Kill:</b> Draw a weapon.
// --------------------------------------------------------
// GameTag:
// - RUSH = 1
// --------------------------------------------------------
// RefTag:
// - HONORABLEKILL = 1
// --------------------------------------------------------
TEST_CASE("[Warrior : Minion] - AV_565 : Axe Berserker")
{
    GameConfig config;
    config.player1Class = CardClass::WARRIOR;
    config.player2Class = CardClass::HUNTER;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;
    config.doShuffle = false;

    for (int i = 0; i < 30; ++i)
    {
        config.player1Deck[i] = Cards::FindCardByName("Fiery War Axe");
    }

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curHand = *(curPlayer->GetHandZone());

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Axe Berserker"));
    const auto card2 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("River Crocolisk"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, PlayCardTask::Minion(card2));

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curHand.GetCount(), 5);

    game.Process(curPlayer, AttackTask(card1, card2));

    CHECK_EQ(curHand.GetCount(), 6);
    CHECK_EQ(curHand[5]->card->name, "Fiery War Axe");
}

// ---------------------------------------- SPELL - WARRIOR
// [AV_660] Iceblood Garrison - COST:2
// - Set: ALTERAC_VALLEY, Rarity: Rare
// --------------------------------------------------------
// Text: At the end of your turn,
//       deal 1 damage to all minions. Lasts 3 turns.
// --------------------------------------------------------
TEST_CASE("[Warrior : Spell] - AV_660 : Iceblood Garrison")
{
    GameConfig config;
    config.player1Class = CardClass::DRUID;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curField = *(curPlayer->GetFieldZone());
    auto& opField = *(opPlayer->GetFieldZone());

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::FindCardByName("Iceblood Garrison"));
    const auto card2 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Strongman"));
    const auto card3 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Strongman"));

    game.Process(curPlayer, PlayCardTask::Minion(card2));
    game.Process(curPlayer, PlayCardTask::Spell(card1));
    CHECK_EQ(curField[0]->GetHealth(), 6);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curField[0]->GetHealth(), 5);

    game.Process(opPlayer, PlayCardTask::Minion(card3));
    CHECK_EQ(opField[0]->GetHealth(), 6);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curField[0]->GetHealth(), 5);
    CHECK_EQ(opField[0]->GetHealth(), 6);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curField[0]->GetHealth(), 4);
    CHECK_EQ(opField[0]->GetHealth(), 5);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curField[0]->GetHealth(), 4);
    CHECK_EQ(opField[0]->GetHealth(), 5);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curField[0]->GetHealth(), 3);
    CHECK_EQ(opField[0]->GetHealth(), 4);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curField[0]->GetHealth(), 3);
    CHECK_EQ(opField[0]->GetHealth(), 4);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curField[0]->GetHealth(), 3);
    CHECK_EQ(opField[0]->GetHealth(), 4);
}

// --------------------------------------- MINION - NEUTRAL
// [AV_101] Herald of Lokholar - COST:4 [ATK:3/HP:5]
// - Set: ALTERAC_VALLEY, Rarity: Common
// --------------------------------------------------------
// Text: <b>Battlecry:</b> Draw a Frost spell.
// --------------------------------------------------------
// GameTag:
// - BATTLECRY = 1
// --------------------------------------------------------
TEST_CASE("[Neutral : Minion] - AV_101 : Herald of Lokholar")
{
    GameConfig config;
    config.player1Class = CardClass::MAGE;
    config.player2Class = CardClass::WARRIOR;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    for (int i = 0; i < 30; i += 3)
    {
        config.player1Deck[i] = Cards::FindCardByName("Ice Barrier");
        config.player1Deck[i + 1] = Cards::FindCardByName("Holy Light");
        config.player1Deck[i + 2] = Cards::FindCardByName("Wisp");
    }

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curHand = *(curPlayer->GetHandZone());

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::FindCardByName("Herald of Lokholar"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    CHECK_EQ(curHand.GetCount(), 5);
    CHECK_EQ(curHand[4]->card->name, "Ice Barrier");
}

// --------------------------------------- MINION - NEUTRAL
// [AV_112] Snowblind Harpy - COST:3 [ATK:3/HP:4]
// - Set: ALTERAC_VALLEY, Rarity: Rare
// --------------------------------------------------------
// Text: <b>Battlecry:</b> If you're holding a Frost spell,
//       gain 5 Armor.
// --------------------------------------------------------
// GameTag:
// - BATTLECRY = 1
// --------------------------------------------------------
TEST_CASE("[Neutral : Minion] - AV_112 : Snowblind Harpy")
{
    GameConfig config;
    config.player1Class = CardClass::MAGE;
    config.player2Class = CardClass::HUNTER;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Snowblind Harpy"));
    const auto card2 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Snowblind Harpy"));
    const auto card3 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Ice Barrier"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    CHECK_EQ(curPlayer->GetHero()->GetArmor(), 5);

    game.Process(curPlayer, PlayCardTask::Spell(card3));
    game.Process(curPlayer, PlayCardTask::Minion(card2));
    CHECK_EQ(curPlayer->GetHero()->GetArmor(), 5);
}

// --------------------------------------- MINION - NEUTRAL
// [AV_121] Gnome Private - COST:1 [ATK:1/HP:3]
// - Set: ALTERAC_VALLEY, Rarity: Common
// --------------------------------------------------------
// Text: <b>Honorable Kill:</b> Gain +2 Attack.
// --------------------------------------------------------
// GameTag:
// - HONORABLEKILL = 1
// --------------------------------------------------------
TEST_CASE("[Neutral : Minion] - AV_121 : Gnome Private")
{
    GameConfig config;
    config.player1Class = CardClass::PRIEST;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curField = *(curPlayer->GetFieldZone());

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Gnome Private"));
    const auto card2 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Gnome Private"));
    const auto card3 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Wisp"));
    const auto card4 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Wisp"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    CHECK_EQ(curField[0]->GetAttack(), 1);
    CHECK_EQ(curField[0]->GetHealth(), 3);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, PlayCardTask::Minion(card3));

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(curPlayer, AttackTask(card1, card3));
    CHECK_EQ(curField[0]->GetAttack(), 3);
    CHECK_EQ(curField[0]->GetHealth(), 2);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, PlayCardTask::Minion(card2));

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(curPlayer, AttackTask(card1, card2));
    CHECK_EQ(curField[0]->GetAttack(), 5);
    CHECK_EQ(curField[0]->GetHealth(), 1);

    game.Process(curPlayer, HeroPowerTask(card1));
    CHECK_EQ(curField[0]->GetAttack(), 5);
    CHECK_EQ(curField[0]->GetHealth(), 3);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, PlayCardTask::Minion(card4));

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(curPlayer, AttackTask(card1, card4));
    CHECK_EQ(curField[0]->GetAttack(), 5);
    CHECK_EQ(curField[0]->GetHealth(), 2);
}

// --------------------------------------- MINION - NEUTRAL
// [AV_122] Corporal - COST:2 [ATK:2/HP:3]
// - Set: ALTERAC_VALLEY, Rarity: Common
// --------------------------------------------------------
// Text: <b>Honorable Kill:</b> Give your other minions
//       <b>Divine Shield</b>.
// --------------------------------------------------------
// GameTag:
// - HONORABLEKILL = 1
// --------------------------------------------------------
// RefTag:
// - DIVINE_SHIELD = 1
// --------------------------------------------------------
TEST_CASE("[Neutral : Minion] - AV_122 : Corporal")
{
    GameConfig config;
    config.player1Class = CardClass::PALADIN;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curField = *(curPlayer->GetFieldZone());

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Corporal"));
    const auto card2 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Wisp"));
    const auto card3 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Intrepid Initiate"));
    const auto card4 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Wisp"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    game.Process(curPlayer, PlayCardTask::Minion(card2));
    CHECK_EQ(curField[0]->HasDivineShield(), false);
    CHECK_EQ(curField[1]->HasDivineShield(), false);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, PlayCardTask::Minion(card3));
    game.Process(opPlayer, PlayCardTask::Minion(card4));

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(curPlayer, AttackTask(card1, card4));
    CHECK_EQ(curField[0]->HasDivineShield(), false);
    CHECK_EQ(curField[1]->HasDivineShield(), false);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(curPlayer, AttackTask(card1, card3));
    CHECK_EQ(curField[0]->HasDivineShield(), false);
    CHECK_EQ(curField[1]->HasDivineShield(), true);
}

// --------------------------------------- MINION - NEUTRAL
// [AV_123] Sneaky Scout - COST:2 [ATK:3/HP:2]
// - Set: ALTERAC_VALLEY, Rarity: Common
// --------------------------------------------------------
// Text: <b>Stealth</b>
//       <b>Honorable Kill:</b> Your next Hero Power costs (0).
// --------------------------------------------------------
// GameTag:
// - HONORABLEKILL = 1
// - STEALTH = 1
// --------------------------------------------------------
TEST_CASE("[Neutral : Minion] - AV_123 : Sneaky Scout")
{
    GameConfig config;
    config.player1Class = CardClass::WARRIOR;
    config.player2Class = CardClass::HUNTER;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curHand = *(curPlayer->GetHandZone());

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Sneaky Scout"));
    const auto card2 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("River Crocolisk"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, PlayCardTask::Minion(card2));

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    CHECK_EQ(curPlayer->GetHero()->heroPower->GetCost(), 2);

    game.Process(curPlayer, AttackTask(card1, card2));
    CHECK_EQ(curPlayer->GetHero()->heroPower->GetCost(), 0);

    game.Process(curPlayer, HeroPowerTask());
    CHECK_EQ(curPlayer->GetHero()->heroPower->GetCost(), 2);
}

// --------------------------------------- MINION - NEUTRAL
// [AV_124] Direwolf Commander - COST:3 [ATK:2/HP:5]
// - Set: ALTERAC_VALLEY, Rarity: Common
// --------------------------------------------------------
// Text: <b>Honorable Kill:</b> Summon a 2/2 Wolf
//       with <b>Stealth</b>.
// --------------------------------------------------------
// GameTag:
// - HONORABLEKILL = 1
// --------------------------------------------------------
// RefTag:
// - STEALTH = 1
// --------------------------------------------------------
TEST_CASE("[Neutral : Minion] - AV_124 : Direwolf Commander")
{
    GameConfig config;
    config.player1Class = CardClass::PALADIN;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curField = *(curPlayer->GetFieldZone());

    const auto card1 = Generic::DrawCard(
        curPlayer, Cards::FindCardByName("Direwolf Commander"));
    const auto card2 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Intrepid Initiate"));
    const auto card3 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Wisp"));

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    CHECK_EQ(curField.GetCount(), 1);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, PlayCardTask::Minion(card2));
    game.Process(opPlayer, PlayCardTask::Minion(card3));

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(curPlayer, AttackTask(card1, card3));
    CHECK_EQ(curField.GetCount(), 1);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(curPlayer, AttackTask(card1, card2));
    CHECK_EQ(curField.GetCount(), 2);
    CHECK_EQ(curField[1]->card->name, "Frostwolf Cub");
    CHECK_EQ(curField[1]->GetAttack(), 2);
    CHECK_EQ(curField[1]->GetHealth(), 2);
    CHECK_EQ(curField[1]->HasStealth(), true);
}

// --------------------------------------- MINION - NEUTRAL
// [AV_125] Tower Sergeant - COST:4 [ATK:4/HP:4]
// - Set: ALTERAC_VALLEY, Rarity: Common
// --------------------------------------------------------
// Text: <b>Battlecry:</b> If you control at least
//       2 other minions, gain +2/+2.
// --------------------------------------------------------
// GameTag:
// - BATTLECRY = 1
// --------------------------------------------------------
TEST_CASE("[Neutral : Minion] - AV_124 : Direwolf Commander")
{
    GameConfig config;
    config.player1Class = CardClass::PALADIN;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& curField = *(curPlayer->GetFieldZone());

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Tower Sergeant"));
    const auto card2 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Tower Sergeant"));
    const auto card3 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Wisp"));
    const auto card4 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Wisp"));

    game.Process(curPlayer, PlayCardTask::Minion(card3));
    game.Process(curPlayer, PlayCardTask::Minion(card1));
    CHECK_EQ(curField[1]->GetAttack(), 4);
    CHECK_EQ(curField[1]->GetHealth(), 4);

    game.Process(curPlayer, PlayCardTask::Minion(card4));
    game.Process(curPlayer, PlayCardTask::Minion(card2));
    CHECK_EQ(curField[3]->GetAttack(), 6);
    CHECK_EQ(curField[3]->GetHealth(), 6);
}

// --------------------------------------- MINION - NEUTRAL
// [AV_126] Bunker Sergeant - COST:3 [ATK:2/HP:4]
// - Set: ALTERAC_VALLEY, Rarity: Common
// --------------------------------------------------------
// Text: <b>Battlecry:</b> If your opponent has 2 or
//       more minions, deal 1 damage to all enemy minions.
// --------------------------------------------------------
// GameTag:
// - BATTLECRY = 1
// --------------------------------------------------------
TEST_CASE("[Neutral : Minion] - AV_126 : Bunker Sergeant")
{
    GameConfig config;
    config.player1Class = CardClass::PALADIN;
    config.player2Class = CardClass::MAGE;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_ACTION);

    Player* curPlayer = game.GetCurrentPlayer();
    Player* opPlayer = game.GetOpponentPlayer();
    curPlayer->SetTotalMana(10);
    curPlayer->SetUsedMana(0);
    opPlayer->SetTotalMana(10);
    opPlayer->SetUsedMana(0);

    auto& opField = *(opPlayer->GetFieldZone());

    const auto card1 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Bunker Sergeant"));
    const auto card2 =
        Generic::DrawCard(curPlayer, Cards::FindCardByName("Bunker Sergeant"));
    const auto card3 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Malygos"));
    const auto card4 =
        Generic::DrawCard(opPlayer, Cards::FindCardByName("Wisp"));

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, PlayCardTask::Minion(card3));

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(curPlayer, PlayCardTask::Minion(card1));
    CHECK_EQ(opField[0]->GetHealth(), 12);

    game.Process(curPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(opPlayer, PlayCardTask::Minion(card4));
    CHECK_EQ(opField.GetCount(), 2);

    game.Process(opPlayer, EndTurnTask());
    game.ProcessUntil(Step::MAIN_ACTION);

    game.Process(curPlayer, PlayCardTask::Minion(card2));
    CHECK_EQ(opField.GetCount(), 1);
    CHECK_EQ(opField[0]->GetHealth(), 11);
}