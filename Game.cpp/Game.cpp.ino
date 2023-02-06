/* 
 *  Main code for the blackjack game
 */
#include "src/Player.h"
//#include "src/InterruptMethods.h"

// Pin constants
const int buttonPlayer1Play = 1;
const int buttonPlayer2Play = 2;
const int buttonPlayer3Play = 3;
const int buttonDeal = 4;
const int buttonPlayer1Stand = 5;
const int buttonPlayer2Stand = 6;
const int buttonPlayer3Stand = 7;
const int buttonPlayer1Hit = 8;
const int buttonPlayer2Hit = 9;
const int buttonPlayer3Hit = 10;
const int buttonClear = 11;

// Variable Declarations
// Game state enumeration
enum State_Type {pregame, dealing, playerAction, dealerAction, gameOver, clearTable};
State_Type gameState = pregame; // Current state of the game :: initialized to Pregame state

// Action types enumeration
enum Action_Type {hit, stand};
Action_Type actionSelected; // Action the player or dealer has chose to perform

// Player objects and variables
Player player1; 
Player player2;
Player player3; 
Player dealer; 
Player *playerList;
int playerInTurnIndex;
Player playerInTurn;
int dealOrder [4] = {1,2,3,0};
bool playerStandFlag = false;
bool dealerStandFlag = false;
bool dealerBustFlag = false;

// misc variables
volatile int player1PlayButtonState = 0;
volatile int player2PlayButtonState = 0;
volatile int player3PlayButtonState = 0;
volatile int dealButtonState = 0;

void setup() {
  // used for debugging 
  Serial.begin(9600); // comment out when testing on board

  // input pin assignments
  pinMode(buttonPlayer1Play, INPUT);
  pinMode(buttonPlayer2Play, INPUT);
  pinMode(buttonPlayer3Play, INPUT);
  pinMode(buttonDeal, INPUT);
  pinMode(buttonPlayer1Stand, INPUT);
  pinMode(buttonPlayer2Stand, INPUT);
  pinMode(buttonPlayer3Stand, INPUT);
  pinMode(buttonPlayer1Hit, INPUT);
  pinMode(buttonPlayer2Hit, INPUT);
  pinMode(buttonPlayer3Hit, INPUT);
  pinMode(buttonClear, INPUT);

  // attaching interrupts to buttons
  attachInterrupt(0, player1PlayButton, CHANGE);
  attachInterrupt(1, player2PlayButton, CHANGE);
  attachInterrupt(2, player3PlayButton, CHANGE);
  attachInterrupt(3, dealButton, CHANGE);
  attachInterrupt(4, player1StandButton, CHANGE);
  attachInterrupt(5, player2StandButton, CHANGE);
  attachInterrupt(6, player3StandButton, CHANGE);
  attachInterrupt(7, player1HitButton, CHANGE);
  attachInterrupt(8, player2HitButton, CHANGE);
  attachInterrupt(9, player3HitButton, CHANGE);
  attachInterrupt(10, clearButton, CHANGE);

  // initializing the game state to the pregame state
  gameState = pregame;
  // allocating memory for the player list
  playerList = new Player[4];
  playerList[0] = dealer;
  for (int i = 1; i < 4; i++) {
    playerList[i] = NULL;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  switch (gameState) {
    // Pregame State
    case pregame :
      // printing game state for debugging 
      Serial.println(gameState);

      // transitioning to Dealing State if the deal button is pressed
      if (dealButtonState == 1) {
        gameState = dealing;
      }
      break; 
    // Dealing State
    case dealing :
      // printing game state for debugging
      Serial.println(gameState);
         
      // calling the dealPlayers method to deal all players
      // dealPlayers(playerList);

      // transitioning to PlayerAction State
      gameState = playerAction;
      break;
    // PlayerAction State
    case playerAction :
      // printing game state for debugging 
      Serial.println(gameState);
      
      // progressing through players
      for (int i = 1; i < 4; i++) {
        playerInTurnIndex = i;
        if (&playerList[playerInTurnIndex] == NULL) {
          continue;
        }
        /* Display to player in turn 
        displayTurn(playerInTurnIndex);
        */
        while(!playerStandFlag) {

        }
      }

      // transitioning to Dealer Action State whne all players have taken their turn
      gameState = dealerAction;
      break;
    // DealerAction State
    case dealerAction :
      // looping until the dealer has stood or busted
      while (!dealerStandFlag || !dealerBustFlag) {
        // dealer's hand is greater than 21 :: dealer busts :: tranisition to gameOver state
        if (playerList[1].calculateHandTotal() > 21) {
          dealerBustFlag = true;
          gameState = gameOver;
          break;
        }
        // dealer's hand is 16 or less :: dealer must hit
        else if (playerList[1].calculateHandTotal() <= 16) {

        }
        // dealer's hand is 17 or greater :: dealer must stand :: tranistion to gameOver state
        else if (playerList[1].calculateHandTotal() >= 17) {
          dealerStandFlag = true;
          gameState = gameOver;
          break;
        }
      }
      break;
    // GameOver State
    case gameOver :
      break;
    // ClearTable State
    case clearTable :
      // resetting all flag variables
      dealerStandFlag = false;
      dealerBustFlag = false;
      playerStandFlag = false;

      // tranisitioning to pregame state
      gameState = pregame;
      break;
  }
}

void player1PlayButton() {
    player1PlayButtonState = digitalRead(buttonPlayer1Play);
    if (player1PlayButtonState == 1) {
        player1 = Player(1);
        playerList[1] = player1;
    }
}

void player2PlayButton() {
    player2PlayButtonState = digitalRead(buttonPlayer2Play);
    if (player2PlayButtonState == 1) {
        player2 = Player(2);
        playerList[2] = player2;
    }
}

void player3PlayButton() {
    player3PlayButtonState = digitalRead(buttonPlayer3Play);
    if (player3PlayButtonState == 1) {
        player3 = Player(3);
        playerList[3] = player3;
    }
}

void dealButton() {
    dealButtonState = digitalRead(buttonDeal);
}

void player1StandButton() {

}

void player2StandButton() {

}

void player3StandButton() {

}

void player1HitButton() {

}

void player2HitButton() {

}

void player3HitButton() {

}

void clearButton() {

}
