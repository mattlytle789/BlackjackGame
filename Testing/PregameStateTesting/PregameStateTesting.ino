#include "src/Player.h"

int buttonPlayer1Play = 2;
int buttonPlayer2Play = 3;
volatile int button1PlayState = 0;
volatile int button2PlayState = 0;

Player player1; 
Player player2;
Player player3; 
Player dealer; 
Player *playerList;

// Game state enumeration
enum State_Type {pregame};
State_Type gameState = pregame; // Current state of the game :: initialized to Pregame state


void setup() {
  Serial.begin(9600);

  pinMode(buttonPlayer1Play, INPUT);
  pinMode(buttonPlayer2Play, INPUT);

  attachInterrupt(0,playPlayer1,CHANGE);
  attachInterrupt(0,playPlayer2,CHANGE);
  
  playerList = new Player[4];
  playerList[0] = dealer;
  for (int i = 1; i < 4; i++) {
    playerList[i] = NULL;
  }

  gameState = pregame;

}

void loop() {
  switch (gameState) {
    case pregame :
      for (int i = 1; i < 3; i++) {
        Serial.println(playerList[i].getNumber());
      }
    break;
  }

}

void playPlayer1() {
  button1PlayState = digitalRead(buttonPlayer1Play);
  Serial.println("player 1 play button");
  if (button1PlayState == 1) {
    player1 = Player(1);
    playerList[1] = player1;
  }
}

void playPlayer2() {
  button2PlayState = digitalRead(buttonPlayer2Play);
  if (button2PlayState == 1) {
    player2 = Player(2);
    playerList[2] = player2;
  }
}
