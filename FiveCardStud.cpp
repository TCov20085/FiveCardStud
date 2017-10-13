// Five Card Stud


#include<stdafx.h>
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<functional>
#include<time.h>
#include<iomanip>

// Card Class
class Card{
	public:
		Card(std::string &inSuit, std::string &inRank);
		~Card();
		std::string getSuit();
		std::string getRank();
		int getValue();
	private:
		std::string suit;
		std::string rank;
		
};
Card::Card(std::string &inSuit, std::string &inRank){
	suit = inSuit;
	rank = inRank;
}
// Destructor for Card
Card::~Card(){
}
int Card::getValue(){
	if(rank == "A")
		return(14);
	else if(rank == "K")
		return(13);
	else if(rank == "Q")
		return(12);
	else if(rank == "J")
		return(11);
	else
		return(stoi(rank));
}
std::string Card::getSuit(){
	return(suit);
}
std::string Card::getRank(){
	return(rank);
}


// Player Class
class Player{
	public:
		Player(int myNumber);
		~Player();
		std::vector<Card> myCards;
		void receiveCard(Card card);
		int playerNumber;
		void sortCards();
};
// Constructor for player
Player::Player(int myNumber){
	playerNumber = myNumber;
}
Player::~Player(){
}
void Player::sortCards(){
	std::vector<Card> tempCards = myCards;
	myCards.clear();
	// Loop through set of 5 cards and find the biggest
	for(int j=0; j <5; ++j){
	Card tempCard = tempCards.at(0);
	for(std::vector<Card>::iterator it=tempCards.begin(); it!=tempCards.end(); ++it){
		if(it->getValue() > tempCard.getValue())
			tempCard = *it;
	}
	myCards.push_back(tempCard);
	for(int i=0; i<tempCards.size(); ++i){
		if(tempCards.at(i).getValue()==tempCard.getValue()){
			tempCards.erase(tempCards.begin()+i);
			break;
		}	
	}
}

}
// Dealer Class
class Dealer{
	public:
		Dealer();
		~Dealer();
		std::vector<Card> deck;
		void ShuffleCards();
		Card DealCard();
		int determineWinner(std::vector<Player> players);
};

// Constructor for dealer
Dealer::Dealer(){

	// First thing dealer will do is "open a deck of cards"
	//  So he needs to create a deck with 52 appropriate cards
	std::string mySuit, myRank;
	// Loop for the suits
	for(int theSuits = 0; theSuits<4; ++theSuits){
		// Suit order is: S,C,H,D

		if(theSuits == 0)
			mySuit = "S";
		else if(theSuits == 1)
			mySuit = "C";
		else if(theSuits == 2)
			mySuit = "H";
		else
			mySuit = "D";
		// Loop for the rank
		for(int rank = 2; rank<=14; ++rank){
			if(rank == 14)
				myRank = "A";
			else if(rank ==13)
				myRank = "K";
			else if(rank ==12)
				myRank = "Q";
			else if(rank ==11)
				myRank = "J";
			else
				myRank = std::to_string((long long) rank);

			// Right here, we have both a suit and a rank, so we make a card.
			//  Then we pass that card into the deck.
			deck.push_back(Card(mySuit,myRank));
		}
	}

	srand (unsigned (time(NULL)));
	std::random_shuffle(deck.begin(),deck.end());
	
	

}
// Destructor for Dealer
Dealer::~Dealer(){
}
//
//  Four people player
//  QS, JS, 10S, 9S, 8S  = 80000000000  + cardvalues = 80000000000  + 1211100908 = 81211100908
//    80000000000 + 12*100000000+11*100000+10*10000+9*100+8*1
//  6H, 5H, 4H, 3H, 2H   = 80000000000  + cardvalues = 80000000000  + 0605040302 = 80605040302
//  KC, JC, 10C, 8C, 3C  = 40000000000
//  10H, 6D, 6C, 5D, 4D

int determineWinner(std::vector<Player> players){
	// Each player has a vector of cards : std::vector<Card> myCards;
	std::vector<long long> playerValue;
	long long tempValue = 0;
	long long place = 1;
	std::vector<Player>::iterator pIt;


	// This whole code is looped for EACH PLAYER
	for(pIt = players.begin(); pIt!=players.end() ; ++pIt){
		
		// Right here pIt->myCards is the vector of cards for the current player 
		// Assume that the cards are sorted by rank (take care of this later)
		//  This will be programmed after each player receives his cards
		
		// Check for straight flush
		// Check for straight (9,8,7,6,5)
		bool isStraight = true;
		for(std::vector<Card>::iterator cIt=pIt->myCards.begin(); cIt!=pIt->myCards.end()-1; cIt++){
			if(cIt->getValue() != (cIt+1)->getValue()+1){	
				isStraight = false;
				break;
			}
		}
		// If we make it right here and isStraight is still true, then it is a straight

		// Check for flush (JS,9S,4S,3S,2S)
		bool isFlush = true;
		for(std::vector<Card>::iterator cIt=pIt->myCards.begin(); cIt!=pIt->myCards.end()-1; cIt++){
			if(cIt->getSuit() != (cIt+1)->getSuit()){
				isFlush = false;
				break;
			}
		}
		//	long long tempValue = 0; 9,8,7,6,4 = 900000000+8000000+70000+600+4 = 40908070604
		tempValue = 0;
		place = 100000000;
		for(std::vector<Card>::iterator cIt=pIt->myCards.begin(); cIt!=pIt->myCards.end(); cIt++){
			tempValue += cIt->getValue() * place;
			place = place/100;
		}
		// If we make it right here and isStraight is still true, then it is a straight
		if(isFlush && isStraight){
			playerValue.push_back(80000000000+tempValue); //playerValue.push_back(80000000000 + totalvalue(hand)); 
			continue;
		}
		else if(isStraight){
			playerValue.push_back(60000000000+tempValue);
			continue;
		}
		else if(isFlush){
			playerValue.push_back(50000000000+tempValue);
			continue;
		}
		
		// Check 4 of a kind. If sorted this means either 1st 4 are the same, or last 4 are the same
		//   AAAA9 85555
		bool is4Kind = true;
		for(std::vector<Card>::iterator cIt=pIt->myCards.begin(); cIt!=pIt->myCards.begin()+3; cIt++){
			// Check to see if 1st 4 are the same - So get rid of the +1 in the straight
			if(cIt->getValue() != (cIt+1)->getValue()){
				is4Kind = false;
				break;
			}
		}

		if(is4Kind){
			// If 1st 4 are the same and we're sorted, then tempValue is correct here
			playerValue.push_back(70000000000+tempValue);
			continue;
		}
		is4Kind = true;
		for(std::vector<Card>::iterator cIt=pIt->myCards.begin()+1; cIt!=pIt->myCards.begin()+4; cIt++){
			// Check to see if 2nd 4 are the same - So get rid of the +1 in the straight
			if(cIt->getValue() != (cIt+1)->getValue()){
				is4Kind = false;
				break;
			}
		}

		if(is4Kind){
			// Here, the last four cards are the same. This will require us to reverse how
			//  "tempValue" was created: 97777... treat it like 77779
			playerValue.push_back(70000000000+
				pIt->myCards.at(0).getValue()+
				100*pIt->myCards.at(1).getValue()+
				10000*pIt->myCards.at(2).getValue()+
				1000000*pIt->myCards.at(3).getValue()+
				100000000*pIt->myCards.at(4).getValue());
			continue;
		}
		/**/
		// 3 of a kind now - 3 possibilities 1,2,3 or 2,3,4 or 3,4,5
		bool is3Kind = true;
		for(std::vector<Card>::iterator cIt=pIt->myCards.begin(); cIt!=pIt->myCards.begin()+2; cIt++){
			// Check to see if 1st 4 are the same - So get rid of the +1 in the straight
			if(cIt->getValue() != (cIt+1)->getValue()){
				is3Kind = false;
				break;
			}
		}
		if(is3Kind){
			// The person's 1st 3 cards match. Lets see if the last two match. if so, then FH:
			if(pIt->myCards.at(3).getValue() == pIt->myCards.at(4).getValue())
				playerValue.push_back(40000000000+tempValue);
			else // Just 3 of a kind ... 1st 3 cards match, so tempValue okay
				playerValue.push_back(30000000000+tempValue);
			continue;
		}
		// 2nd 3 cards
		is3Kind = true;
		for(std::vector<Card>::iterator cIt=pIt->myCards.begin()+1; cIt!=pIt->myCards.begin()+3; cIt++){
			// Check to see if 1st 4 are the same - So get rid of the +1 in the straight
			if(cIt->getValue() != (cIt+1)->getValue()){
				is3Kind = false;
				break;
			}
		}
		if(is3Kind){
			// 2nd three cards match (1,2,3) so order 1,2,3,0,4
			playerValue.push_back(30000000000+
				pIt->myCards.at(4).getValue()+
				100*pIt->myCards.at(0).getValue()+
				10000*pIt->myCards.at(3).getValue()+
				1000000*pIt->myCards.at(2).getValue()+
				100000000*pIt->myCards.at(1).getValue());
			continue;
		}
		// 3rd 3 cards
		is3Kind = true;
		for(std::vector<Card>::iterator cIt=pIt->myCards.begin()+2; cIt!=pIt->myCards.begin()+4; cIt++){
			// Check to see if 1st 4 are the same - So get rid of the +1 in the straight
			if(cIt->getValue() != (cIt+1)->getValue()){
				is3Kind = false;
				break;
			}
		}
		if(is3Kind){ 
			// The person's lasst 3 cards match. Lets see if the first two match. if so, then FH:
			if(pIt->myCards.at(0).getValue() == pIt->myCards.at(1).getValue()) // 66444
				playerValue.push_back(40000000000+
				pIt->myCards.at(1).getValue()+
				100*pIt->myCards.at(0).getValue()+
				10000*pIt->myCards.at(2).getValue()+
				1000000*pIt->myCards.at(3).getValue()+
				100000000*pIt->myCards.at(4).getValue());
			else // Opposite order now... 
				playerValue.push_back(30000000000+
				pIt->myCards.at(1).getValue()+
				100*pIt->myCards.at(0).getValue()+
				10000*pIt->myCards.at(2).getValue()+
				1000000*pIt->myCards.at(3).getValue()+
				100000000*pIt->myCards.at(4).getValue());
			continue;
		}
	
		// Two pair : 1,2+3,4 
		//			  1,2+4,5
		//			  2,3+4,5
		if(pIt->myCards.at(0).getValue() == pIt->myCards.at(1).getValue() &&
			pIt->myCards.at(2).getValue() == pIt->myCards.at(3).getValue()){ // Added this bracket
			playerValue.push_back(20000000000+ tempValue);
			continue; // Added this
		} // Added this
		else if(pIt->myCards.at(0).getValue() == pIt->myCards.at(1).getValue() &&
			pIt->myCards.at(3).getValue() == pIt->myCards.at(4).getValue()){ // Added this bracket
			playerValue.push_back(20000000000+
				pIt->myCards.at(2).getValue()+
				100*pIt->myCards.at(4).getValue()+
				10000*pIt->myCards.at(3).getValue()+
				1000000*pIt->myCards.at(1).getValue()+
				100000000*pIt->myCards.at(0).getValue());
			continue; // Added this
		} // Added this
		else if(pIt->myCards.at(1).getValue() == pIt->myCards.at(2).getValue() &&
			pIt->myCards.at(3).getValue() == pIt->myCards.at(4).getValue()){ // Added this bracket
			playerValue.push_back(20000000000+
				pIt->myCards.at(0).getValue()+
				100*pIt->myCards.at(4).getValue()+
				10000*pIt->myCards.at(3).getValue()+
				1000000*pIt->myCards.at(2).getValue()+
				100000000*pIt->myCards.at(1).getValue());
			continue; // Added this
		} // Added this


		// One pair - CHANGE THIS
		bool isPair = false;
		for(std::vector<Card>::iterator cIt=pIt->myCards.begin(); cIt!=pIt->myCards.end()-1; cIt++){
			if(cIt->getValue()==(cIt+1)->getValue()){
				isPair = true;
				break;
			}
		}
		if(isPair){
			//01
			if(pIt->myCards.at(0).getValue()==pIt->myCards.at(1).getValue())
				playerValue.push_back(10000000000+
					pIt->myCards.at(4).getValue()+
					100*pIt->myCards.at(3).getValue()+
					10000*pIt->myCards.at(2).getValue()+
					1000000*pIt->myCards.at(1).getValue()+
					100000000*pIt->myCards.at(0).getValue());
			//12
			else if(pIt->myCards.at(1).getValue()==pIt->myCards.at(2).getValue())
				playerValue.push_back(10000000000+
					pIt->myCards.at(4).getValue()+
					100*pIt->myCards.at(3).getValue()+
					10000*pIt->myCards.at(0).getValue()+
					1000000*pIt->myCards.at(2).getValue()+
					100000000*pIt->myCards.at(1).getValue());
			//23
			else if(pIt->myCards.at(2).getValue()==pIt->myCards.at(3).getValue())
				playerValue.push_back(10000000000+
					pIt->myCards.at(4).getValue()+
					100*pIt->myCards.at(1).getValue()+
					10000*pIt->myCards.at(0).getValue()+
					1000000*pIt->myCards.at(3).getValue()+
					100000000*pIt->myCards.at(2).getValue());
			//34
			else if(pIt->myCards.at(3).getValue()==pIt->myCards.at(4).getValue())
				playerValue.push_back(10000000000+
					pIt->myCards.at(2).getValue()+
					100*pIt->myCards.at(1).getValue()+
					10000*pIt->myCards.at(0).getValue()+
					1000000*pIt->myCards.at(4).getValue()+
					100000000*pIt->myCards.at(3).getValue());
			continue;
		}
		playerValue.push_back(tempValue);
	
	}

	// Should have code that determines who is the winner
	long long maxValue = playerValue.at(0);
	int winner = 0;
	for(int i=0;i<playerValue.size();++i){
		std::cout << "Player " << i << " value is = " << playerValue.at(i) << std::endl;
		if(playerValue.at(i) > maxValue){
			maxValue = playerValue.at(i);
			winner = i;
		}
	}

	return(winner);
}

void playAGame(){
	Dealer *myDealer;
	// When making a dealer he need to shuffle
	myDealer = new Dealer();

	
	// Make 4 players
	Player p1(0), p2(1), p3(2), p4(3);


	// Make a vector of players
	std::vector<Player> players;
	players.push_back(p1); // Etc.
	players.push_back(p2);
	players.push_back(p3);
	players.push_back(p4);



	for(int i=0; i<5; ++i){
	// Loop of 5. Each player gets ONE card from the deal on each part of the loop
	//   - Make sure you delete the card from the deck when you deal
	for(std::vector<Player>::iterator pIt=players.begin(); pIt!=players.end(); pIt++){
		pIt->myCards.push_back(myDealer->deck.at(myDealer->deck.size()-1));
				myDealer->deck.pop_back();
		}
	}

	
	for(std::vector<Player>::iterator pIt=players.begin(); pIt!=players.end(); ++pIt){
		// Sort their cards
		pIt->sortCards();
		// Print their cards
	}

	for(std::vector<Player>::iterator pIt=players.begin(); pIt!=players.end(); pIt++){	
		for(int i=0; i<5; ++i){
	// Loop of 5. Each player gets ONE card from the deal on each part of the loop
	//   - Make sure you delete the card from the deck when you deal
				std::cout << i<< ":" <<  pIt->myCards.at(i).getRank() <<  pIt->myCards.at(i).getSuit() << std::endl;
		}
	}

	int myWinner = determineWinner(players);

	// Print winner (include if there is a tie - since lots of ties right now)
	std::cout << "Winner is player " << myWinner << std::endl;

	
}

int main(int argc, char*argv[]){
	playAGame();

	// Asks user whether or not to play again 
	std::string again;
	do{
	std::cout << "Do you want to play again? Y or N" << std::endl;
	std::cin >> again;
	playAGame();
	}while((again == "Y") || (again == "y"));
	

}