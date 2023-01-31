/* 
 *  Main code for the blackjack game
 */
#include "src/Player.h"

// Pin constants
const int buttonPlayer1Play = 1;
const int buttonPlayer2Play = 2;
const int buttonPlayer3Play = 3;
const int buttonDeal = 4;

// Variable Declarations
// Game state enumeration
enum State_Type {pregame, dealing, playerAction, dealerAction, gameOver, clearTable};
State_Type gameState = pregame; // Current state of the game :: initialized to Pregame state

// Action types enumeration
enum Action_Type {hit, stand};
Action_Type actionSelected; // Action the player or dealer has chose to perform

// Player objects
Player player1; 
Player player2;
Player player3; 
Player dealer; 
Player *playerList;

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

  // attaching interrupts to buttons
  attachInterrupt(0, player1PlayButton, CHANGE);
  attachInterrupt(1, player2PlayButton, CHANGE);
  attachInterrupt(2, player3PlayButton, CHANGE);
  attachInterrupt(3, dealButton, CHANGE);

  // initializing the game state to the pregame state
  gameState = pregame;
  // allocating memory for the player list
  playerList = new Player[4];
  playerList[0] = dealer;
}

void loop() {
  // put your main code here, to run repeatedly:
  switch (gameState) {
    case pregame :
      // printing game state for debugging 
      Serial.println(gameState);
      // transitioning to dealing state if the deal button is pressed
      if (dealButtonState == 1) {
        gameState = dealing;
      }
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
