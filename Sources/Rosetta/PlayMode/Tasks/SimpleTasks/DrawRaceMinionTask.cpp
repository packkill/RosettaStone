// This code is based on Sabberstone project.
// Copyright (c) 2017-2021 SabberStone Team, darkfriend77 & rnilva
// RosettaStone is hearthstone simulator using C++ with reinforcement learning.
// Copyright (c) 2017-2021 Chris Ohk

#include <Rosetta/PlayMode/Actions/Draw.hpp>
#include <Rosetta/PlayMode/Games/Game.hpp>
#include <Rosetta/PlayMode/Tasks/SimpleTasks/DrawRaceMinionTask.hpp>
#include <Rosetta/PlayMode/Zones/DeckZone.hpp>

#include <effolkronium/random.hpp>

using Random = effolkronium::random_static;

namespace RosettaStone::PlayMode::SimpleTasks
{
DrawRaceMinionTask::DrawRaceMinionTask(Race race, int amount, bool addToStack)
    : m_race(race), m_amount(amount), m_addToStack(addToStack)
{
    // Do nothing
}

TaskStatus DrawRaceMinionTask::Impl(Player* player)
{
    if (m_addToStack)
    {
        player->game->taskStack.playables.clear();
    }

    auto deck = player->GetDeckZone()->GetAll();
    if (deck.empty())
    {
        return TaskStatus::STOP;
    }

    std::vector<Playable*> cards;
    cards.reserve(m_amount);

    for (auto& deckCard : deck)
    {
        if (deckCard->card->GetCardType() == CardType::MINION &&
            (deckCard->card->GetRace() == m_race ||
             deckCard->card->GetRace() == Race::ALL))
        {
            cards.emplace_back(deckCard);
        }
    }

    if (cards.empty())
    {
        return TaskStatus::STOP;
    }

    if (static_cast<int>(cards.size()) <= m_amount)
    {
        for (int i = 0; i < m_amount; ++i)
        {
            if (m_addToStack)
            {
                player->game->taskStack.playables.emplace_back(cards[i]);
            }

            Generic::Draw(player, cards[i]);
        }
    }
    else
    {
        for (int i = 0; i < m_amount; ++i)
        {
            const auto pick = Random::get<std::size_t>(0, cards.size() - 1);

            if (m_addToStack)
            {
                player->game->taskStack.playables.emplace_back(cards[pick]);
            }

            Generic::Draw(player, cards[pick]);
            cards.erase(std::begin(cards) + static_cast<std::ptrdiff_t>(pick));
        }
    }

    return TaskStatus::COMPLETE;
}

std::unique_ptr<ITask> DrawRaceMinionTask::CloneImpl()
{
    return std::make_unique<DrawRaceMinionTask>(m_race, m_amount, m_addToStack);
}
}  // namespace RosettaStone::PlayMode::SimpleTasks