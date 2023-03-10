class Player {
    private: 
        int number;
        int hand[10];
		int handIndex;
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
        }
        // getter methods
        int getNumber() {
            return number;
        }
		int getCard(int index) {
			return hand[index];
		}
        // mutator methods
        void setNumber(int num) {
            number = num;
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
				if (hand[i] != 0) {
					if (hand[i] == 11 || hand[i] == 12 || hand[i] == 13) {
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