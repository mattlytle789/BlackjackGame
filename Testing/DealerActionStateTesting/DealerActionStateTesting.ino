
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
bool dealerTurnOverFlag = false;

char input = NULL;

// Game state enumeration
enum State_Type {pregame, dealing, playerAction, dealerAction, gameOver};
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
      Serial.println("Dealer Action State");
      while (!dealerTurnOverFlag) {
        if (playerList[4].calculateHandTotal() > 21) {
          dealerBust();
        }
        else if (playerList[4].calculateHandTotal() >= 17) {
          dealerStand();
        }
        else if (playerList[4].calculateHandTotal() <= 16) {
          dealerHit();
        }
      }
      Serial.println("Dealer Action State Over");
      gameState = gameOver;
      break;
    // Game Over State
    case gameOver :
      //Serial.println("Game Over State");
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
    if (i == 0) {
      playerList[i].addCard(5);
      playerList[i].addCard(6);
    }
    else if (i == 1) {
      playerList[i].addCard(8);
      playerList[i].addCard(8);
    }
    Serial.print("Player hand total: ");
    Serial.println(playerList[i].calculateHandTotal());
    delay(10000);
  }
  Serial.println("Dealing Cards to Dealer");
  playerList[4].addCard(10);
  playerList[4].addCard(2);
  Serial.print("Dealer hand total: ");
  Serial.println(playerList[4].calculateHandTotal());
  delay(10000);
}

void playerHit(int playerNumber) {
  Serial.println("Dealing a card to player");
  playerList[playerNumber].addCard(10);
  delay(10000);
  Serial.print("Player ");
  Serial.print(playerNumber+1);
  Serial.println(" turn");
}

void dealerHit() {
  Serial.println("Dealer Hit, deal a card");
  playerList[4].addCard(5);
  delay(10000);
}

void dealerStand() {
  Serial.println("Dealer stand, turn over");
  dealerTurnOverFlag = true;
}

void dealerBust() {
  Serial.println("Dealer bust, turn over");
  dealerTurnOverFlag = true;
}
