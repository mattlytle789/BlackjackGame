/* 
 *  Main code for the blackjack game
 */
#include "src/Player.h"

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


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
