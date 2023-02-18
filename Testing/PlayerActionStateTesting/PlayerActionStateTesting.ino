#include "src/Player.h"

Player player1; 
Player player2;
Player player3; 
Player dealer; 
Player *playerList;
int numPlayers = 0;

// Flags 
bool dealButtonFlag = false;
bool turnOverFlag = false;

char input = NULL;

// Game state enumeration
enum State_Type {pregame, dealing, playerAction, dealerAction};
State_Type gameState = pregame; // Current state of the game :: initialized to Pregame state


void setup() {
  Serial.begin(9600);
  
  playerList = new Player[4];
  playerList[4] = dealer;
  for (int i = 1; i < 3; i++) {
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
      for (int i = 0; i < numPlayers; i++) {
        Serial.println(playerList[i].getNumber());
      }
      Serial.println("Pregame State Over");
      gameState = dealing;
      break;
    // Dealing state
    case dealing :
      Serial.println("Dealing State Begin");
      dealCards();
      Serial.println("Dealing State Over");
      gameState = playerAction;
      break;
    // Player Action state
    case playerAction :
      Serial.println("Player Action State");
      for (int i = 0; i < numPlayers; i++) {
        Serial.print("Player ");
        Serial.print(playerList[i].getNumber());
        Serial.println(" turn start");
        while (!turnOverFlag) {
          if (Serial.available()) {
            input = Serial.read();
            if (input == '4') {
              Serial.print("Player ");
              Serial.print(playerList[i].getNumber());
              Serial.println(" performed hit action");
              playerHit(i);
            }
            else if (input == '5') {
              Serial.print("Player ");
              Serial.print(playerList[i].getNumber());
              Serial.println(" performed stand action");
              turnOverFlag = true;
            }
          }
        }
      Serial.print("Player ");
      Serial.print(playerList[i].getNumber());
      Serial.println(" turn over");
      turnOverFlag = false;
      }
      Serial.println("Player Action State Over");
      gameState = dealerAction;    
      break;
    // Dealer Action State
    case dealerAction :
      break;
  }

}

void playPlayer1() {
  Serial.println("Player 1 play button pressed");
  player1 = Player(1);
  playerList[0] = player1;
  numPlayers++;
}

void playPlayer2() {
  Serial.println("Player 2 play button pressed");
  player2 = Player(2);
  playerList[1] = player2;
  numPlayers++;
}

void dealCards() {
  Serial.println("Dealing Cards to Players");
  for (int i = 0; i < numPlayers; i++) {
    Serial.print("Dealing Player ");
    Serial.println(i+1);
    delay(10000);
  }
}

void playerHit(int playerNumber) {
  Serial.println("Dealing a card to player");
  playerList[playerNumber].addCard(10);
  delay(10000);
}
