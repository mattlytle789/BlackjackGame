#include "src/Player.h"
#include "src/Constants.h"

/***** Player Variables **************/
Player player1; // Player 1 player object
Player player2; // Player 2 player object
Player player3; // Player 3 player object
Player dealer; // Dealer player object
Player *playerList; // Array of all players and dealer :: Player 1, index 0; Player 2, index 1; Player 3, index 2; Dealer, index 4
int numPlayers = 0; // number of players in current game
/*************************************/

/***** Flag Variables ****************/
bool dealButtonFlag = false; // flag for deal button being pressed :: true if pressed; false otherwise
bool turnOverFlag = false; // flag for a player's turn ending :: true when player stands; false otherwise
bool dealerTurnOverFlag = false; // flag for dealer's turn ending :: true when dealer stands; false otherwise
bool player1JoinedFlag = false; // flag for if player 1 joined the game :: true when joined; false otherwise
bool player2JoinedFlag = false; // flag for if player 2 joined the game :: true when joined; false otherwise
bool player3JoinedFlag = false; // flag for if player 2 joined the game :: true when joined; false otherwise
/*************************************/

/***** Button State Variables ********/
volatile int player1PlayButtonState = 0; // state of player 1 play button
volatile int player2PlayButtonState = 0; // state of player 2 play button
volatile int player3PlayButtonState = 0; // state of player 3 play button
volatile int dealButtonState = 0; // state of the deal button
/*************************************/

/***** Testing Variables *************/
char input = NULL;
/*************************************/

/***** Game State Variables **********/
enum State_Type {pregame, dealing, playerAction, dealerAction, gameOver, clearTable}; // enumeration of game states
State_Type gameState = pregame; // Current state of the game :: initialized to Pregame state 
/*************************************/

void setup() {
  // For testing purposes
  Serial.begin(9600);
  
  // allocating memory for player list and filling with null
  playerList = new Player[4];
  playerList[4] = dealer;
  for (int i = 1; i < 3; i++) {
    playerList[i] = NULL;
  }

  // setting input pins 
  pinMode(player1PlayButton, INPUT_PULLUP);
  pinMode(player2PlayButton, INPUT_PULLUP);
  pinMode(player3PlayButton, INPUT_PULLUP);
  pinMode(dealButton, INPUT_PULLUP);

  // setting interrupts
  PCICR |= B00000100; // Opening Port D
  PCMSK2 |= B00010000; // setting pin 4 to have interrupt
  PCMSK2 |= B00100000; // setting pin 5 to have interrupt
  PCMSK2 |= B01000000; // setting pin 6 to have interrupt
  PCICR |= B00000001; // Opening Port B
  PCMSK0 |= B00000001; // stting pin 8 to have interrupt

  // initializing the game state to pregame
  gameState = pregame;
}

void loop() {
  switch (gameState) {
    // Pregame State
    case pregame :
      Serial.println("Pregame State"); // for testing purposes
      while (!dealButtonFlag) {
        // waiting for the deal button to be pressed
      }
      // for testing purposes
      Serial.println("Players in List: ");
      for (int i = 0; i < 3; i++) {
        if (playerList[i].getNumber() != 0) {
          Serial.println(playerList[i].getNumber());
        }
      }
      Serial.println("Pregame State Over");

      // transitioning to dealing state
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
      for (int i = 0; i < 3; i++) {
        if (playerList[i].getNumber() != 0) {
          Serial.print("Player ");
          Serial.print(playerList[i].getNumber());
          Serial.println(" turn start");
          while (!turnOverFlag) {
            if (playerList[i].calculateHandTotal() > 21) {
              playerBust(i);
            }
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
      Serial.println("Game Over State");
      for (int i = 0; i < 3; i++) {
        if (playerList[i].getNumber() != 0) {
          Serial.print("Dealer total: ");
          Serial.println(playerList[4].calculateHandTotal());
          Serial.print("Player ");
          Serial.print(i+1);
          Serial.print(" total: ");
          Serial.println(playerList[i].calculateHandTotal());
          if (playerList[i].calculateHandTotal() > 21) {
            playerBustGameOver(i);
          }
          else if (playerList[i].calculateHandTotal() > playerList[4].calculateHandTotal()) {
            playerWin(i);
          }
          else if (playerList[i].calculateHandTotal() < playerList[4].calculateHandTotal()) {
            playerLose(i);
          }
          else if (playerList[i].calculateHandTotal() == playerList[4].calculateHandTotal()) {
            playerPush(i);
          }
        }
      }
      Serial.println("Game Over State Over");
      gameState = clearTable;
      break;
    // Clear Table State
    case clearTable :
      Serial.println("Clear Table State");
      clearTableProcess();
      turnOverFlag = false;
      dealerTurnOverFlag = false;
      dealButtonFlag = false;
      player1JoinedFlag = false;
      player2JoinedFlag = false;
      player3JoinedFlag = false;
      delete[] playerList;
      playerList = new Player[4];
      playerList[4] = dealer;
      for (int i = 1; i < 3; i++) {
        playerList[i] = NULL;
      }
      numPlayers = 0;
      Serial.println("Clear Table State Over");
      gameState = pregame;
      break;
  }
}

// Interrupt method for pins on Port D :: used for play buttons 
ISR (PCINT2_vect) {
  // read each players play button 
  player1PlayButtonState = digitalRead(player1PlayButton);
  player2PlayButtonState = digitalRead(player2PlayButton);
  player3PlayButtonState = digitalRead(player3PlayButton);
  // decide which play button was pressed and add the player
  if (!player1JoinedFlag && player1PlayButtonState == 1) {
    Serial.println("button 1 pressed");
    player1 = Player(1);
    playerList[0] = player1;
    numPlayers++;
    player1JoinedFlag = true;
  }
  if (!player2JoinedFlag && player2PlayButtonState == 1) {
    Serial.println("button 2 pressed");
    player2 = Player(2);
    playerList[1] = player2;
    numPlayers++;
    player2JoinedFlag = true;
  }
  if (!player3JoinedFlag && player3PlayButtonState == 1) {
    Serial.println("button 3 pressed");
    player3 = Player(3);
    playerList[2] = player3;
    numPlayers++;
    player3JoinedFlag = true;
  }
}

// interrupt method for pins on Port C :: used for dealing and clear table buttons
ISR (PCINT0_vect) {
  dealButtonState = digitalRead(dealButton);
  if (numPlayers > 0 && dealButtonState == 1) {
    dealButtonFlag = true;
  }
}

/* Methods for testing
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
*/

void dealCards() {
  Serial.println("Dealing Cards to Players");
  for (int i = 0; i < 3; i++) {
    if (playerList[i].getNumber() != 0) {
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
      delay(5000);
    }
  }
  Serial.println("Dealing Cards to Dealer");
  playerList[4].addCard(10);
  playerList[4].addCard(2);
  Serial.print("Dealer hand total: ");
  Serial.println(playerList[4].calculateHandTotal());
  delay(5000);
}

void playerHit(int playerNumber) {
  Serial.println("Dealing a card to player");
  playerList[playerNumber].addCard(10);
  delay(10000);
  Serial.print("Player ");
  Serial.print(playerNumber+1);
  Serial.println(" turn");
}

void playerBust(int playerNumber) {
  Serial.print("Player ");
  Serial.print(playerNumber+1);
  Serial.println(" busts, turn over");
  turnOverFlag = true;
}

void dealerHit() {
  Serial.println("Dealer Hit, deal a card");
  playerList[4].addCard(5);
  delay(5000);
}

void dealerStand() {
  Serial.println("Dealer stand, turn over");
  dealerTurnOverFlag = true;
}

void dealerBust() {
  Serial.println("Dealer bust, turn over");
  dealerTurnOverFlag = true;
}

void playerWin(int playerNumber) {
  Serial.print("Player ");
  Serial.print(playerNumber+1);
  Serial.println(" wins!");
}

void playerLose(int playerNumber) {
  Serial.print("Player ");
  Serial.print(playerNumber+1);
  Serial.println(" loses!");
}

void playerPush(int playerNumber) {
  Serial.print("Player ");
  Serial.print(playerNumber+1);
  Serial.println(" pushes!");
}

void playerBustGameOver(int playerNumber) {
  Serial.print("Player ");
  Serial.print(playerNumber+1);
  Serial.println(" busts!");
}

void clearTableProcess() {
  Serial.println("Clearing Table");
  delay(5000);
}
