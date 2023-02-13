#include "src/Player.h"

Player player1; 
Player player2;
Player player3; 
Player dealer; 
Player *playerList;
int numPlayers = 0;

// Flag for deal button 
bool dealButtonFlag = false;

char input = NULL;

// Game state enumeration
enum State_Type {pregame, dealing};
State_Type gameState = pregame; // Current state of the game :: initialized to Pregame state


void setup() {
  Serial.begin(9600);
  
  playerList = new Player[4];
  playerList[0] = dealer;
  for (int i = 1; i < 4; i++) {
    playerList[i] = NULL;
  }

  gameState = pregame;
}

void loop() {
  switch (gameState) {
    // Pregame State
    case pregame :
      Serial.println("Pregame State");
      while (!dealButtonFlag) {
        if (Serial.available()) {
          input = Serial.read();
          if (input == '1') {
            playPlayer1();
          }
          else if (input == '2') {
            playPlayer2();
          }
          else if (input == '3') {
            dealButtonFlag = true;
          }
          input = NULL;
        }
      }
      Serial.println("Players in List: ");
      for (int i = 1; i < numPlayers+1; i++) {
        Serial.println(playerList[i].getNumber());
      }
      Serial.println("Pregame State Over");
      gameState = dealing;
      break;
    // Dealing state
    case dealing :

      break;
  }

}

void playPlayer1() {
  Serial.println("Player 1 play button pressed");
  player1 = Player(1);
  playerList[1] = player1;
  numPlayers++;
}

void playPlayer2() {
  Serial.println("Player 2 play button pressed");
  player2 = Player(2);
  playerList[2] = player2;
  numPlayers++;
}
