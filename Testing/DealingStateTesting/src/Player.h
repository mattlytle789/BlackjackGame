class Player {
    private: 
        int number;
        int hand[10];
    public:
        Player() {
            for (int i = 0; i < 10; i++) {
                hand[i] = 0;
            }
        }
        Player(int num) {
            number = num;
            for (int i = 0; i < 10; i++) {
                hand[i] = 0;
            }
        }
        // getter methods
        int getNumber() {
            return number;
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
                handTotal += hand[i];
            }
            return handTotal;
        }
};