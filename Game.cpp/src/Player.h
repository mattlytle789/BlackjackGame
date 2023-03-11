enum Outcome_Type {win,loss,push,blackjack};

class Player {
private:
	int number;
	int hand[10];
	int handIndex;
	bool hasAceFlag;
	Outcome_Type outcome;
public:
	Player() {
		for (int i = 0; i < 10; i++) {
			hand[i] = 0;
		}
		handIndex = 0;
	}
	Player(int num) {
		number = num;
		for (int i = 0; i < 10; i++) {
			hand[i] = 0;
		}
		handIndex = 0;
		hasAceFlag = false;
	}
	// getter methods
	int getNumber() {
		return number;
	}
	int getCard(int index) {
		return hand[index];
	}
	bool getHasAceFlag() {
		return hasAceFlag;
	}
	Outcome_Type getOutcome() {
		return outcome;
	}
	// mutator methods
	void setNumber(int num) {
		number = num;
	}
	void setHasAceFlag(bool flag) {
		hasAceFlag = flag;
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
	int calculateHandTotal(bool aceFlag) {
		int handTotal = 0;
		if (!aceFlag) {
			for (int i = 0; i < 10; i++) {
				if (hand[i] == 11 || hand[i] == 12 || hand[i] == 13) {
					handTotal += 10;
				}
				else {
					handTotal += hand[i];
				}
			}
		}
		else if (aceFlag) {
			for (int i = 0; i < 10; i++) {
				if (hand[i] == 1) {
					handTotal += 11;
				}
				else if (hand[i] == 11 || hand[i] == 12 || hand[i] == 13) {
					handTotal += 10;
				}
				else {
					handTotal += hand[i];
				}
			}
		}
		return handTotal;
	}
	void addCard(int num) {
		hand[handIndex] = num;
		handIndex++;
	}
};