class Player {
    private: 
        int number;
    public:
        Player() {
            
        }
        Player(int num) {
            number = num;
        }
        // getter methods
        int getNumber() {
            return number;
        }
        // mutator methods
        void setNumber(int num) {
            number = num;
        }
        // overloading == operator
        bool operator==(const Player& P) {
            if (this->number == P.getNumber) {
                return true;
            }
            return false;
        }
        // overloading != operator 
        bool operator!=(const Player& P) {
            if (this->number != Player.getNumber()) {
                return true;
            }
            return false;
        }
};