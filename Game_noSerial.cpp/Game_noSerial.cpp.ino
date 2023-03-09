#include "src/Player.h"
#include "src/Constants.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

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
bool player1JoinedDisplayFlag = false; // flag for if player 1's join message has been displayed :: true when displayed, false otherwise
/*************************************/

/***** Button State Variables ********/
volatile int player1PlayButtonState = 0; // state of player 1 play button
volatile int player2PlayButtonState = 0; // state of player 2 play button
volatile int player3PlayButtonState = 0; // state of player 3 play button
volatile int dealButtonState = 0; // state of the deal button
volatile int hitButtonState = 0; // state of the hit button
volatile int standButtonState = 0; // state of the stand button
volatile int clearButtonState = 0; // state of the clear button
/*************************************/

/***** Testing Variables *************/
char input = NULL;
/*************************************/

/***** Game State Variables **********/
enum State_Type {pregame, dealing, playerAction, dealerAction, gameOver, clearTable}; // enumeration of game states
State_Type gameState = pregame; // Current state of the game :: initialized to Pregame state 
/*************************************/

/***** LCD Display Variables *********/
LiquidCrystal_I2C lcd1(0x27, 16, 2);
/*************************************/

void setup() {
  // allocating memory for player list and filling with null
  playerList = new Player[4];
  playerList[4] = dealer;
  for (int i = 1; i < 3; i++) {
    playerList[i] = NULL;
  }

  // initializing LCD Display
  lcd1.init();
  lcd1.clear();
  lcd1.backlight();

  // setting input pins 
  pinMode(player1PlayButton, INPUT_PULLUP);
  pinMode(player2PlayButton, INPUT_PULLUP);
  pinMode(player3PlayButton, INPUT_PULLUP);
  pinMode(dealButton, INPUT_PULLUP);
  pinMode(hitButton, INPUT_PULLUP);
  pinMode(standButton, INPUT_PULLUP);
  pinMode(clearButton, INPUT_PULLUP);

  // setting output pins
  pinMode(player1ControllerPower, OUTPUT);
  pinMode(player2ControllerPower, OUTPUT);
  pinMode(player3ControllerPower, OUTPUT);

  // setting interrupts
  PCICR |= B00000100; // Opening Port D
  PCMSK2 |= B00010000; // setting pin 4 to have interrupt
  PCMSK2 |= B00100000; // setting pin 5 to have interrupt
  PCMSK2 |= B01000000; // setting pin 6 to have interrupt
  PCICR |= B00000001; // Opening Port B
  PCMSK0 |= B00000001; // setting pin 8 to have interrupt

  // initializing the game state to pregame
  gameState = pregame;
}

void loop() {
  switch (gameState) {
    // Pregame State
    case pregame :
      while (!dealButtonFlag) {
        // waiting for the deal button to be pressed
        if (player1JoinedFlag && !player1JoinedDisplayFlag) {
          displayPlayerJoined(0);
          player1JoinedDisplayFlag = true;
        }
        delay(500);
      }

      // transitioning to dealing state
      gameState = dealing;
      break;
    // Dealing state
    case dealing :
      dealCards();
      gameState = playerAction;
      break;
    // Player Action state
    case playerAction :
      for (int i = 0; i < 3; i++) {
        if (playerList[i].getNumber() != 0) {
          switch (i) {
            case 0 : // Turning on player 1 controller
              digitalWrite(player1ControllerPower, HIGH);
              digitalWrite(player2ControllerPower, LOW);
              digitalWrite(player3ControllerPower, LOW);
              break;
            case 1 : // Turning on player 2 controller
              digitalWrite(player1ControllerPower, LOW);
              digitalWrite(player2ControllerPower, HIGH);
              digitalWrite(player3ControllerPower, LOW);
              break;
            case 2 : // Turning on player 3 controller
              digitalWrite(player1ControllerPower, LOW);
              digitalWrite(player2ControllerPower, LOW);
              digitalWrite(player3ControllerPower, HIGH);
              break;
          }
          // waiting for a player to stand
          while (!turnOverFlag) {
            if (playerList[i].calculateHandTotal() > 21) {
              playerBust(i);
            }
            // reading the state of the action buttons
            hitButtonState = digitalRead(hitButton);
            standButtonState = digitalRead(standButton);
            if (hitButtonState == 1) { // player hit call hit method
              playerHit(i);
            }
            if (standButtonState == 1) { // player stand call stand method
              playerStand(i);
            }
          }
          turnOverFlag = false;
        }
      }
      gameState = dealerAction;    
      break;
    // Dealer Action State
    case dealerAction :
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
      gameState = gameOver;
      break;
    // Game Over State
    case gameOver :
      for (int i = 0; i < 3; i++) {
        if (playerList[i].getNumber() != 0) {
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
      while (clearButtonState != 1) {
        clearButtonState = digitalRead(clearButton);
      }
      gameState = clearTable;
      break;
    // Clear Table State
    case clearTable :
      clearTableProcess();
      // resetting all flag variables to false
      turnOverFlag = false;
      dealerTurnOverFlag = false;
      dealButtonFlag = false;
      player1JoinedFlag = false;
      player2JoinedFlag = false;
      player3JoinedFlag = false;
      // resetting all button state variables 
      player1PlayButtonState = 0;
      player2PlayButtonState = 0;
      player3PlayButtonState = 0;
      dealButtonState = 0;
      hitButtonState = 0;
      standButtonState = 0;
      clearButtonState = 0;
      // resetting the player list
      delete[] playerList;
      playerList = new Player[4];
      playerList[4] = dealer;
      for (int i = 1; i < 3; i++) {
        playerList[i] = NULL;
      }
      numPlayers = 0;
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
    player1 = Player(1);
    playerList[0] = player1;
    numPlayers++;
    player1JoinedFlag = true;
  }
  if (!player2JoinedFlag && player2PlayButtonState == 1) {
    player2 = Player(2);
    playerList[1] = player2;
    numPlayers++;
    player2JoinedFlag = true;
  }
  if (!player3JoinedFlag && player3PlayButtonState == 1) {
    player3 = Player(3);
    playerList[2] = player3;
    numPlayers++;
    player3JoinedFlag = true;
  }
}

// interrupt method for pins on Port C :: used for dealing and clear table buttons
ISR (PCINT0_vect) {
  if (gameState == pregame) {
    dealButtonState = digitalRead(dealButton);
    if (numPlayers > 0 && dealButtonState == 1) {
      dealButtonFlag = true;
    }
  }
}

void displayPlayerJoined(int playerNumber) {
  lcd1.setCursor(0,0);
  lcd1.print("Joined");
  /*switch (playerNumber) {
    case 0 :
      lcd1.setCursor(0,0);
      lcd1.print("Joined");
      break;
    default :
      break;
  }*/
}

void displayPlayerHand(int playerNumber) {

}

void displayPlayerOutcome(int playerNumber) {

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
  for (int i = 0; i < 3; i++) {
    if (playerList[i].getNumber() != 0) {
      if (i == 0) {
        playerList[i].addCard(5);
        playerList[i].addCard(6);
      }
      else if (i == 1) {
        playerList[i].addCard(8);
        playerList[i].addCard(8);
      }
      delay(5000);
    }
  }
}

void playerHit(int playerNumber) {
  playerList[playerNumber].addCard(10);
  delay(5000);
}

void playerStand(int playerNumber) {
  turnOverFlag = true;
}

void playerBust(int playerNumber) {
  turnOverFlag = true;
}

void dealerHit() {
  playerList[4].addCard(5);
  delay(5000);
}

void dealerStand() {
  dealerTurnOverFlag = true;
}

void dealerBust() {
  dealerTurnOverFlag = true;
}

void playerWin(int playerNumber) {
}

void playerLose(int playerNumber) {
}

void playerPush(int playerNumber) {
}

void playerBustGameOver(int playerNumber) {
}

void clearTableProcess() {
  delay(5000);
}
