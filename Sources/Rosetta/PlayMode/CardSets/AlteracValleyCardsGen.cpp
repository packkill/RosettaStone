// This code is based on Sabberstone project.
// Copyright (c) 2017-2019 SabberStone Team, darkfriend77 & rnilva
// Hearthstone++ is hearthstone simulator using C++ with reinforcement learning.
// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

#include <Rosetta/PlayMode/CardSets/AlteracValleyCardsGen.hpp>

namespace RosettaStone::PlayMode
{
void AlteracValleyCardsGen::AddHeroes(std::map<std::string, CardDef>& cards)
{
}

void AlteracValleyCardsGen::AddHeroPowers(std::map<std::string, CardDef>& cards)
{
}

void AlteracValleyCardsGen::AddDruid(std::map<std::string, CardDef>& cards)
{
}

void AlteracValleyCardsGen::AddDruidNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void AlteracValleyCardsGen::AddHunter(std::map<std::string, CardDef>& cards)
{
}

void AlteracValleyCardsGen::AddHunterNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void AlteracValleyCardsGen::AddMage(std::map<std::string, CardDef>& cards)
{
}

void AlteracValleyCardsGen::AddMageNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void AlteracValleyCardsGen::AddPaladin(std::map<std::string, CardDef>& cards)
{
}

void AlteracValleyCardsGen::AddPaladinNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void AlteracValleyCardsGen::AddPriest(std::map<std::string, CardDef>& cards)
{
}

void AlteracValleyCardsGen::AddPriestNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void AlteracValleyCardsGen::AddRogue(std::map<std::string, CardDef>& cards)
{
}

void AlteracValleyCardsGen::AddRogueNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void AlteracValleyCardsGen::AddShaman(std::map<std::string, CardDef>& cards)
{
}

void AlteracValleyCardsGen::AddShamanNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void AlteracValleyCardsGen::AddWarlock(std::map<std::string, CardDef>& cards)
{
}

void AlteracValleyCardsGen::AddWarlockNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void AlteracValleyCardsGen::AddWarrior(std::map<std::string, CardDef>& cards)
{
}

void AlteracValleyCardsGen::AddWarriorNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void AlteracValleyCardsGen::AddDemonHunter(
    std::map<std::string, CardDef>& cards)
{
}

void AlteracValleyCardsGen::AddDemonHunterNonCollect(
    std::map<std::string, CardDef>& cards)
{
}

void AlteracValleyCardsGen::AddNeutral(std::map<std::string, CardDef>& cards)
{
    // --------------------------------------- MINION - NEUTRAL
    // [AV_100] Drek'Thar - COST:4 [ATK:4/HP:4]
    // - Set: ALTERAC_VALLEY, Rarity: Legendary
    // --------------------------------------------------------
    // Text: <b>Battlecry</b>: If this costs more than
    //       every minion in your deck, summon 2 of them.
    // --------------------------------------------------------
    // GameTag:
    // - ELITE = 1
    // - BATTLECRY = 1
    // --------------------------------------------------------

    // --------------------------------------- MINION - NEUTRAL
    // [AV_223] Vanndar Stormpike - COST:4 [ATK:4/HP:4]
    // - Set: ALTERAC_VALLEY, Rarity: Legendary
    // --------------------------------------------------------
    // Text: <b>Battlecry</b>: If this costs less than
    //       every minion in your deck, reduce their Cost by (3).
    // --------------------------------------------------------
    // GameTag:
    // - ELITE = 1
    // - BATTLECRY = 1
    // --------------------------------------------------------
}

void AlteracValleyCardsGen::AddNeutralNonCollect(
    std::map<std::string, CardDef>& cards)
{
    // ---------------------------------- ENCHANTMENT - NEUTRAL
    // [AV_223e] Occupy the Keep - COST:0
    // - Set: ALTERAC_VALLEY
    // --------------------------------------------------------
    // Text: Costs (3) less.
    // --------------------------------------------------------
}

void AlteracValleyCardsGen::AddAll(std::map<std::string, CardDef>& cards)
{
    AddHeroes(cards);
    AddHeroPowers(cards);

    AddDruid(cards);
    AddDruidNonCollect(cards);

    AddHunter(cards);
    AddHunterNonCollect(cards);

    AddMage(cards);
    AddMageNonCollect(cards);

    AddPaladin(cards);
    AddPaladinNonCollect(cards);

    AddPriest(cards);
    AddPriestNonCollect(cards);

    AddRogue(cards);
    AddRogueNonCollect(cards);

    AddShaman(cards);
    AddShamanNonCollect(cards);

    AddWarlock(cards);
    AddWarlockNonCollect(cards);

    AddWarrior(cards);
    AddWarriorNonCollect(cards);

    AddDemonHunter(cards);
    AddDemonHunterNonCollect(cards);

    AddNeutral(cards);
    AddNeutralNonCollect(cards);
}
}  // namespace RosettaStone::PlayMode