enum Outcome_Type {win,loss,push,blackjack};

class Player {
private:
	int number;
	int hand[10];
	int handIndex;
	int numAces;
	Outcome_Type outcome;
public:
	Player() {
		for (int i = 0; i < 10; i++) {
			hand[i] = 0;
		}
		handIndex = 0;
		numAces = 0;
	}
	Player(int num) {
		number = num;
		for (int i = 0; i < 10; i++) {
			hand[i] = 0;
		}
		handIndex = 0;
		numAces = 0;
	}
	// getter methods
	int getNumber() {
		return number;
	}
	int getCard(int index) {
		return hand[index];
	}
	int getNumAces() {
		return numAces;
	}
	Outcome_Type getOutcome() {
		return outcome;
	}
	// mutator methods
	void setNumber(int num) {
		number = num;
	}
	void setOutcome(Outcome_Type o) {
		outcome = o;
	}
	// overloading operators
	bool operator==(Player& P) {
		if (this->number == P.getNumber()) {
			return true;
		}
		return false;
	}
	bool operator!=(Player& P) {
		if (this->number != P.getNumber()) {
			return true;
		}
		return false;
	}
	// methods
	int calculateHandTotal() {
		int handTotal = 0;
		for (int i = 0; i < 10; i++) {
			if (hand[i] == 11 || hand[i] == 12 || hand[i] == 13) {
				handTotal += 10;
			}
			else if (hand[i] == 1) {
				handTotal += 11;
			}
			else {
				handTotal += hand[i];
			}
		}
		if (numAces > 0 && handTotal > 21) {
			int subtractions = 0;
			while (subtractions < numAces && handTotal > 21) {
				handTotal -= 10;
				subtractions++;
			}
		}
		return handTotal;
	}
	void addCard(int num) {
		hand[handIndex] = num;
		if (num == 1) {
			numAces++;
		}
		handIndex++;
	}
};