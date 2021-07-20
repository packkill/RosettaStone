// This code is based on Sabberstone project.
// Copyright (c) 2017-2019 SabberStone Team, darkfriend77 & rnilva
// RosettaStone is hearthstone simulator using C++ with reinforcement learning.
// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

#ifndef ROSETTASTONE_PLAYMODE_TRIGGERS_HPP
#define ROSETTASTONE_PLAYMODE_TRIGGERS_HPP

#include <Rosetta/PlayMode/Triggers/Trigger.hpp>

#include <string>

namespace RosettaStone::PlayMode
{
//!
//! \brief Triggers class.
//!
//! This class includes utility methods for trigger.
//!
class Triggers
{
 public:
    //! Trigger for enrage.
    //! \param enchantmentID The card ID of enchantment.
    static Trigger EnrageTrigger(std::string&& enchantmentID)
    {
        Trigger trigger(TriggerType::PREDAMAGE);
        trigger.triggerSource = TriggerSource::SELF;
        trigger.condition =
            std::make_shared<SelfCondition>(SelfCondition::IsUndamaged());
        trigger.tasks = { std::make_shared<SimpleTasks::AddEnchantmentTask>(
            std::move(enchantmentID), EntityType::SOURCE) };

        return trigger;
    }

    //! Trigger for upgradable card.
    //! \param mana The amount of mana requires for upgrade.
    //! \param enchantmentID The upgraded card ID.
    static Trigger UpgradableTrigger(int mana, std::string&& upgradedCardID)
    {
        Trigger trigger(TriggerType::MANA_CRYSTAL);
        trigger.triggerActivation = TriggerActivation::HAND;
        trigger.condition = std::make_shared<SelfCondition>(
            SelfCondition::HasAtLeastManaCrystal(mana));
        trigger.tasks = { std::make_shared<SimpleTasks::ChangeEntityTask>(
            std::move(upgradedCardID), EntityType::SOURCE) };
        trigger.fastExecution = true;

        return trigger;
    }
};
}  // namespace RosettaStone::PlayMode

#endif  // ROSETTASTONE_PLAYMODE_TRIGGERS_HPP