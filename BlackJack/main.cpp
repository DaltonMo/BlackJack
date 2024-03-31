#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <string>

// Enum to represent card suits
enum class Suit { Hearts, Diamonds, Clubs, Spades };

// Enum to represent card values
enum class Value { Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };

// Structure to represent a single playing card
struct Card {
	Suit suit;
	Value value;
};

// Structure to represent a hand of cards
struct Hand {
	std::vector<Card> cards;
};

// Function to initialize the deck of cards
std::vector<Card> initializeDeck() {
	std::vector<Card> deck;

	// Iterate over each suit and value to populate the deck
	for (int s = static_cast<int>(Suit::Hearts); s <= static_cast<int>(Suit::Spades); ++s) {
		for (int v = static_cast<int>(Value::Two); v <= static_cast<int>(Value::Ace); ++v) {
			Card card;
			card.suit = static_cast<Suit>(s);
			card.value = static_cast<Value>(v);
			deck.push_back(card);
		}
	}

	return deck;
}

// Function to shuffle the deck of cards using Fisher-Yates shuffle algorithm
void shuffleDeck(std::vector<Card>& deck) {
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	for (int i = deck.size() - 1; i > 0; --i) {
		int j = std::rand() % (i + 1);
		std::swap(deck[i], deck[j]);
	}
}

// Function to deal a single card from the deck
Card dealCard(std::vector<Card>& deck) {
	Card card = deck.back();
	deck.pop_back();
	return card;
}

// Function to calculate the total value of a hand in blackjack
int calculateHandValue(const Hand& hand) {
	int totalValue = 0;
	int numAces = 0;

	for (const auto& card : hand.cards) {
		switch (card.value) {
		case Value::Two:
		case Value::Three:
		case Value::Four:
		case Value::Five:
		case Value::Six:
		case Value::Seven:
		case Value::Eight:
		case Value::Nine:
		case Value::Ten:
			totalValue += static_cast<int>(card.value) + 2;
			break;
		case Value::Jack:
		case Value::Queen:
		case Value::King:
			totalValue += 10;
			break;
		case Value::Ace:
			numAces++;
			totalValue += 11;
			break;
		}
	}

	while (totalValue > 21 && numAces > 0) {
		totalValue -= 10;
		numAces--;
	}

	return totalValue;
}

// Function to display a single card
void displayCard(const Card& card) {
	std::string suit;
	std::string value;

	switch (card.suit) {
	case Suit::Hearts:
		suit = "Hearts";
		break;
	case Suit::Diamonds:
		suit = "Diamonds";
		break;
	case Suit::Clubs:
		suit = "Clubs";
		break;
	case Suit::Spades:
		suit = "Spades";
		break;
	}

	switch (card.value) {
	case Value::Two:
		value = "2";
		break;
	case Value::Three:
		value = "3";
		break;
	case Value::Four:
		value = "4";
		break;
	case Value::Five:
		value = "5";
		break;
	case Value::Six:
		value = "6";
		break;
	case Value::Seven:
		value = "7";
		break;
	case Value::Eight:
		value = "8";
		break;
	case Value::Nine:
		value = "9";
		break;
	case Value::Ten:
		value = "10";
		break;
	case Value::Jack:
		value = "Jack";
		break;
	case Value::Queen:
		value = "Queen";
		break;
	case Value::King:
		value = "King";
		break;
	case Value::Ace:
		value = "Ace";
		break;
	}

	std::cout << value << " of " << suit << std::endl;
}

// Function to handle the player's turn
void playerTurn(std::vector<Card>& deck, Hand& playerHand, Hand& dealerHand) {
	char choice;

	while (true) {
		std::cout << "Do you want to hit (h) or stand (s)? ";
		std::cin >> choice;

		if (choice == 'h') {
			// Player chooses to hit
			Card newCard = dealCard(deck);
			playerHand.cards.push_back(newCard);

			std::cout << "Player draws a card: ";
			displayCard(newCard);
			std::cout << std::endl;

			int playerTotal = calculateHandValue(playerHand);
			if (playerTotal > 21) {
				std::cout << "Player busts! Dealer wins." << std::endl;
				return;
			}
		}
		else if (choice == 's') {
			// Player chooses to stand
			std::cout << "Player stands." << std::endl;
			return;
		}
		else {
			std::cout << "Invalid choice. Please choose 'h' to hit or 's' to stand." << std::endl;
		}
	}
}

// Function to handle the dealer's turn
void dealerTurn(std::vector<Card>& deck, Hand& dealerHand) {
	int dealerTotal = calculateHandValue(dealerHand);

	std::cout << "Dealer's turn:" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));

	while (dealerTotal < 17) {
		Card newCard = dealCard(deck);
		dealerHand.cards.push_back(newCard);

		std::cout << "Dealer draws a card: ";
		displayCard(newCard);

		dealerTotal = calculateHandValue(dealerHand);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	std::cout << "Dealer stands." << std::endl;
	std::cout << std::endl;
}

// Function to determine the winner of the round
void determineWinner(const Hand& playerHand, const Hand& dealerHand) {
	int playerTotal = calculateHandValue(playerHand);
	int dealerTotal = calculateHandValue(dealerHand);

	std::cout << "Player's total: " << playerTotal << std::endl;
	std::cout << "Dealer's total: " << dealerTotal << std::endl;

	if (playerTotal > 21) {
		std::cout << "Player busts! Dealer wins." << std::endl;
	}
	else if (dealerTotal > 21) {
		std::cout << "Dealer busts! Player wins." << std::endl;
	}
	else if (playerTotal == dealerTotal) {
		std::cout << "It's a tie." << std::endl;
	}
	else if (playerTotal > dealerTotal) {
		std::cout << "Player wins!" << std::endl;
	}
	else {
		std::cout << "Dealer wins." << std::endl;
	}
}

int main() {
	std::vector<Card> deck = initializeDeck();
	shuffleDeck(deck);

	Hand playerHand;
	Hand dealerHand;

	// Deal initial cards
	playerHand.cards.push_back(dealCard(deck));
	dealerHand.cards.push_back(dealCard(deck));
	playerHand.cards.push_back(dealCard(deck));
	dealerHand.cards.push_back(dealCard(deck));

	// Display initial hands
	std::cout << "Player's Hand:" << std::endl;
	displayCard(playerHand.cards[0]);
	displayCard(playerHand.cards[1]);
	std::cout << std::endl;

	std::cout << "Dealer's Hand:" << std::endl;
	displayCard(dealerHand.cards[0]);
	std::cout << "Unknown card" << std::endl;
	std::cout << std::endl;

	// Player's turn
	playerTurn(deck, playerHand, dealerHand);
	std::cout << std::endl;

	// Display dealer's second card
	std::cout << "Dealer's Hand:" << std::endl;
	displayCard(dealerHand.cards[0]);
	displayCard(dealerHand.cards[1]);
	std::cout << std::endl;

	// Dealer's turn
	dealerTurn(deck, dealerHand);

	// Determine the winner
	determineWinner(playerHand, dealerHand);

	return 0;
}
