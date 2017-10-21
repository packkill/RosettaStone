/*************************************************************************
> File Name: Console.cpp
> Project Name: Hearthstone++
> Author: Chan-Ho Chris Ohk
> Purpose: Console version of Hearthstone++ game.
> Created Time: 2017/10/08
> Copyright (c) 2017, Chan-Ho Chris Ohk
*************************************************************************/
#include <Agents/GameAgent.h>
#include <Commons/Constants.h>
#include <Enums/EnumsToString.h>
#include <Loaders/CardLoader.h>
#include <Loaders/PlayerLoader.h>
#include <Models/Card.h>
#include <Models/Cards.h>
#include <Programs/Console.h>

#include <filesystem>
#include <fstream>
#include <iostream>

namespace filesystem = std::experimental::filesystem;

namespace Hearthstonepp
{
	template<std::size_t SIZE>
	void Console::ShowMenu(std::array<std::string, SIZE>& menus)
	{
		std::cout << "========================================\n";
		for (auto& menu : menus)
		{
			std::cout << menu.c_str() << '\n';
		}
		std::cout << "========================================\n";
	}

	size_t Console::InputMenuNum(std::string questionStr, const int menuSize)
	{
		while (true)
		{
			std::cout << questionStr;
			size_t num;
			std::cin >> num;

			if (num < 1 || num > menuSize)
			{
				std::cout << "Invalid number! Try again.\n";
			}
			else
			{
				return num;
			}
		}
	}

	bool Console::InputYesNo(std::string sentence) const
	{
		std::cout << sentence;
		std::cout << "(Please input \"y/yes\" or \"n/no\" (insensitive))\n";
		std::cout << "Input: ";

		std::string str;
		std::cin >> str;
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);

		return (str == "y" || str == "yes") ? true : (str == "n" || str == "no") ? false : InputYesNo(sentence);
	}

	void Console::SignIn()
	{
		std::cout << "Input Player ID to load data.\n";
		std::cout << "If you do not want to load, please input \"STOP\"\n";

		while (true)
		{
			std::cout << "Player ID: ";
			std::string playerID;
			std::cin >> playerID;

			if (playerID == "STOP")
			{
				break;
			}

			if (!filesystem::exists("Datas/" + playerID + ".json"))
			{
				std::cout << playerID << ".json doesn't exist. Try again.\n";
				continue;
			}

			PlayerLoader loader;
			m_player = loader.Load(playerID);

			if (m_player == nullptr)
			{
				std::cout << "An error occurred while loading player data.\n";
				continue;
			}

			std::cout << "You are signed in. Hello, " << playerID << "!\n";

			Main();

			break;
		}
	}

	void Console::SignUp()
	{
		std::cout << "Input Player ID to create data.\n";

		while (true)
		{
			std::cout << "Player ID: ";
			std::string playerID;
			std::cin >> playerID;

			if (filesystem::exists("Datas/" + playerID + ".json"))
			{
				std::cout << playerID << ".json already exists. Try again.\n";
				continue;
			}

			std::cout << "Name: ";
			std::string name;
			std::cin >> name;

			std::ofstream file;

			json j;
			j["name"] = name;

			filesystem::create_directory("Datas");
			file.open("Datas/" + playerID + ".json");

			if (!file.is_open())
			{
				std::cout << "An error occurred while saving player data.\n";
				continue;
			}

			file << std::setw(4) << j << "\n";

			file.close();

			m_player = new Player(std::move(name));

			std::cout << "Your account has been created. Please sign in.\n";
			break;
		}
	}

	void Console::SearchCard()
	{

	}

	void Console::MakeDeck()
	{
		std::cout << "========================================\n";
		std::cout << "               Make Deck!               \n";
		std::cout << "========================================\n";

		ShowMenu(m_playerClassStr);
		const size_t selectedClassNum = InputMenuNum("What's your player class? ", PLAYER_CLASS_SIZE);
		const CardClass playerClass = static_cast<CardClass>(selectedClassNum + 1);

		std::cout << "What's your deck name? ";
		std::string name;
		std::cin >> name;

		Deck deck(playerClass, name);

		std::cout << "Input Card ID to add or delete to your deck.\n";
		std::cout << "If you do not want to add or delete more, please input \"STOP\"\n";

		while (true)
		{
			std::cout << "The number of cards in the current deck = " << deck.GetNumOfCards() << " / " << MAXIMUM_NUM_CARDS_IN_DECK << "\n";
			std::cout << "Card ID: ";
			std::string selectedCardID;
			std::cin >> selectedCardID;

			if (selectedCardID == "STOP")
			{
				break;
			}

			const Card* card = Cards::GetInstance()->FindCardByID(selectedCardID);
			if (card == nullptr)
			{
				std::cout << selectedCardID << " doesn't exist. Try again.\n";
				continue;
			}
			if (card->GetCardClass() != CardClass::NEUTRAL && card->GetCardClass() != playerClass)
			{
				std::cout << "The class of " << selectedCardID << " is " << ConverterFromCardClassToString.at(card->GetCardClass()).c_str() << '\n';
				std::cout << "It is neither a NETURAL nor a " << ConverterFromCardClassToString.at(playerClass).c_str() << '\n';
				continue;
			}

			card->ShowInfo();

			const bool isYes = InputYesNo("Is it correct? ");
			if (isYes == false)
			{
				continue;
			}

			ShowMenu(m_makeDeckOperationStr);
			const size_t selectedOperation = InputMenuNum("What do you want to do? ", MAKE_DECK_OPERATION_SIZE);

			if (selectedOperation != MAKE_DECK_OPERATION_SIZE)
			{
				m_makeDeckOperationFuncs[selectedOperation - 1](*this, deck, selectedCardID);
			}
			else
			{
				break;
			}
		}
	}

	void Console::LoadDeck()
	{

	}

	void Console::StoreDeck()
	{

	}

	void Console::SimulateGame()
	{
		//CardLoader loader;
		//std::vector<Card*> cards;

		//loader.Load(cards);

		//Deck deck1; // temporal deck
		//Deck deck2;

		//deck1.reserve(30);
		//deck2.reserve(30);

		//deck1.assign(cards.begin(), cards.begin() + 30); 
		//deck2.assign(cards.begin() + 30, cards.begin() + 60);

		//User user1(0, new Hero(), new HeroPower(), deck1); // define new user
		//User user2(1, new Hero(), new HeroPower(), deck2);

		//GameAgent agent(&user1, &user2);
		//GameResult result;

		//std::thread *at = agent.StartAgent(result);

		//for (int i = 0; i < 2; ++i)
		//{
		//	Card *list[3] = { 0, };
		//	int result = agent.ReadBuffer((BYTE*)list, sizeof(Card*) * 3); // get card data

		//	for (auto card : list)
		//	{
		//		std::cout << "[" << card->name << "] ";
		//	}
		//	std::cout << std::endl;

		//	BYTE mulligan[] = { 0, 2 }; // index of the card to be mulligan
		//	result = agent.WriteBuffer(mulligan, 2); // send index to agent

		//	result = agent.ReadBuffer((BYTE*)list, sizeof(Card*) * 3); // get new card data

		//	for (auto card : list)
		//	{
		//		std::cout << "[" << card->name << "] ";
		//	}
		//	std::cout << std::endl;
		//}

		//at->join(); // join agent thread
	}

	void Console::Leave()
	{
		if (m_player != nullptr)
		{
			delete m_player;
			m_player = nullptr;
		}

		std::cout << "You have been successfully signed out. Have a nice day!\n";
	}

	void Console::AddCardInDeck(Deck& deck, std::string& selectedCardID)
	{
		if (deck.GetNumOfCards() >= MAXIMUM_NUM_CARDS_IN_DECK)
		{
			std::cout << "The deck " << deck.GetName() << " is full of cards.\n";
			return;
		}

		while (true)
		{
			unsigned int numCardToAddAvailable = Cards::GetInstance()->FindCardByID(selectedCardID)->GetMaxAllowedInDeck() - deck.GetNumCardInDeck(selectedCardID);
			if (deck.GetNumOfCards() + numCardToAddAvailable > MAXIMUM_NUM_CARDS_IN_DECK)
			{
				numCardToAddAvailable = deck.GetNumOfCards() + numCardToAddAvailable - MAXIMUM_NUM_CARDS_IN_DECK;
			}

			std::cout << "How many cards to add (0 - " << numCardToAddAvailable << ") ? ";
			unsigned int numCardToAdd;
			std::cin >> numCardToAdd;

			if (numCardToAdd < 0 || numCardToAdd > numCardToAddAvailable)
			{
				std::cout << "Invalid number! Try again.\n";
			}
			else
			{
				deck.AddCard(selectedCardID, numCardToAdd);
				break;
			}
		}
	}

	void Console::DeleteCardInDeck(Deck& deck, std::string& selectedCardID)
	{
		if (deck.GetNumCardInDeck(selectedCardID) == 0)
		{
			std::cout << selectedCardID << " doesn't exist.\n";
			return;
		}

		while (true)
		{
			std::cout << "How many cards to delete (0 - " << deck.GetNumCardInDeck(selectedCardID) << ") ? ";
			unsigned int numCardToDelete;
			std::cin >> numCardToDelete;

			if (numCardToDelete < 0 || numCardToDelete > deck.GetNumCardInDeck(selectedCardID))
			{
				std::cout << "Invalid number! Try again.\n";
			}
			else
			{
				deck.DeleteCard(selectedCardID, numCardToDelete);
				break;
			}
		}
	}

	int Console::Login()
	{
		std::cout << "    Welcome to Hearthstone++ Ver " << VERSION << '\n';

		ShowMenu(m_loginMenuStr);
		const size_t selectedNum = InputMenuNum("Select: ", LOGIN_MENU_SIZE);
		bool isFinish = false;

		if (selectedNum != LOGIN_MENU_SIZE)
		{
			m_loginMenuFuncs[selectedNum - 1](*this);
		}
		else
		{
			isFinish = true;
		}

		return isFinish ? 0 : Login();
	}

	int Console::Main()
	{
		ShowMenu(m_mainMenuStr);
		const size_t selectedNum = InputMenuNum("Select: ", MAIN_MENU_SIZE);
		bool isFinish = false;

		m_mainMenuFuncs[selectedNum - 1](*this);

		if (selectedNum == MAIN_MENU_SIZE)
		{
			isFinish = true;
		}

		return isFinish ? 0 : Main();
	}
}
