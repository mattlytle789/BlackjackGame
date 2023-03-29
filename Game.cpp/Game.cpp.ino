#include "src/Player.h"
#include "src/Constants.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
//#include <string>

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
bool player2JoinedDisplayFlag = false; // flag for if player 2's join message has been displayed :: true when displayed, false otherwise
bool player3JoinedDisplayFlag = false; // flag for if player 3's join message has been displayed :: true when displayed, false otherwise
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
long randNum;
/*************************************/

/***** Game State Variables **********/
enum State_Type {pregame, dealing, playerAction, dealerAction, gameOver, clearTable}; // enumeration of game states
State_Type gameState = pregame; // Current state of the game :: initialized to Pregame state 
/*************************************/

/***** LCD Display Variables *********/
LiquidCrystal_I2C lcd1(0x27, 20, 4);
LiquidCrystal_I2C lcd2(0x20, 20, 4);
LiquidCrystal_I2C lcd3(0x23, 20, 4);
LiquidCrystal_I2C lcd4(0x25, 20, 4);
/*************************************/

/***** Misc Variables ****************/
String cardNames[] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
String outcomes[] = {"Win","Lose","Push","Blackjack"};
/*************************************/
void setup() {
  // For testing purposes
  Serial.begin(9600);
  randomSeed(analogRead(A3));
  
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
  lcd2.init();
  lcd2.clear();
  lcd2.backlight();
  lcd3.init();
  lcd3.clear();
  lcd3.backlight();
  lcd4.init();
  lcd4.clear();
  lcd4.backlight();

  // setting input pins 
  pinMode(player1PlayButton, INPUT_PULLUP);
  pinMode(player2PlayButton, INPUT_PULLUP);
  pinMode(player3PlayButton, INPUT_PULLUP);
  pinMode(dealButton, INPUT_PULLUP);
  pinMode(hitButton, INPUT_PULLUP);
  pinMode(standButton, INPUT_PULLUP);
  //pinMode(clearButton, INPUT_PULLUP);
  //pinMode(cvModuleInputSignal, INPUT);

  // setting output pins
  pinMode(player1ControllerPower, OUTPUT);
  pinMode(player2ControllerPower, OUTPUT);
  pinMode(player3ControllerPower, OUTPUT);
  //pinMode(cvModuleOutputSignal, OUTPUT);

  // setting interrupts
  PCICR |= B00000100; // Opening Port D
  PCMSK2 |= B00010000; // setting pin 4 to have interrupt
  PCMSK2 |= B00100000; // setting pin 5 to have interrupt
  PCMSK2 |= B01000000; // setting pin 6 to have interrupt
  PCICR |= B00000001; // Opening Port B
  PCMSK0 |= B00000010; // setting pin 9 to have interrupt

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
        if (player1JoinedFlag && !player1JoinedDisplayFlag) {
          displayPlayerJoined(0);
          player1JoinedDisplayFlag = true;
        }
        if (player2JoinedFlag && !player2JoinedDisplayFlag) {
          displayPlayerJoined(1);
          player2JoinedDisplayFlag = true;
        }
        if (player3JoinedFlag && !player3JoinedDisplayFlag) {
          displayPlayerJoined(2);
          player3JoinedDisplayFlag = true;
        }
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
      dealButtonState = 0;
      dealCards();
      // check for blackjack
      for (int i = 0; i < 3; i++) {
        if (playerList[i].getNumber() != 0) {
          if (playerList[i].calculateHandTotal() == 21) {
            playerList[i].setOutcome(blackjack);
          }
        }
      }
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
      Serial.println("Player Action State Over");
      gameState = dealerAction;    
      break;
    // Dealer Action State
    case dealerAction :
      Serial.println("Dealer Action State");
      displayDealerHand();
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
          if (playerList[4].calculateHandTotal() > 21) {
            //displayPlayerOutcome(i,outcomes[0]);
            dealerBustGameOver(i);
          }
          else if (playerList[i].calculateHandTotal() > 21) {
            //displayPlayerOutcome(i,outcomes[1]);
            playerBustGameOver(i);
          }
          else if (playerList[i].calculateHandTotal() > playerList[4].calculateHandTotal()) {
            //displayPlayerOutcome(i,outcomes[0]);
            playerWin(i);
          }
          else if (playerList[i].calculateHandTotal() < playerList[4].calculateHandTotal()) {
            //displayPlayerOutcome(i,outcomes[1]);
            playerLose(i);
          }
          else if (playerList[i].calculateHandTotal() == playerList[4].calculateHandTotal()) {
            //displayPlayerOutcome(i,outcomes[3]);
            playerPush(i);
          }
        }
      }
      while (dealButtonState != 1) {
        dealButtonState = digitalRead(dealButton);
      }
      Serial.println("Game Over State Over");
      gameState = clearTable;
      break;
    // Clear Table State
    case clearTable :
      clearTableProcess();
      // clearing LCD screens
      lcd1.clear();
      lcd2.clear();
      lcd3.clear();
      lcd4.clear();
      // resetting all flag variables to false
      turnOverFlag = false;
      dealerTurnOverFlag = false;
      dealButtonFlag = false;
      player1JoinedFlag = false;
      player2JoinedFlag = false;
      player3JoinedFlag = false;
      player1JoinedDisplayFlag = false;
      player2JoinedDisplayFlag = false;
      player3JoinedDisplayFlag = false;
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
  if (gameState == pregame) {
    dealButtonState = digitalRead(dealButton);
    if (numPlayers > 0 && dealButtonState == 1) {
      Serial.println("deal button pressed");
      dealButtonFlag = true;
    }
  }
}

void displayPlayerJoined(int playerNumber) {
  switch (playerNumber) {
    case 0 :
      lcd1.setCursor(0,0);
      lcd1.print("Joined");
      break;
    case 1 :
      lcd2.setCursor(0,0);
      lcd2.print("Joined");
      break;
    case 2 :
      lcd3.setCursor(0,0);
      lcd3.print("Joined");
      break;
  }
}

void displayDealerHand() {
  if (gameState == dealing) {
    lcd4.setCursor(0,0);
    lcd4.print("Cards: ");
    lcd4.print(cardNames[playerList[4].getCard(1)-1]);
    lcd4.print(" ");
    lcd4.setCursor(0,1);
    lcd4.print("Hand Total: ");
    if (playerList[4].getCard(1) == 1) {
      lcd4.print("11");
    }
    else if (playerList[4].getCard(1) == 11 || playerList[4].getCard(1) == 12 || playerList[4].getCard(1) == 13) {
      lcd4.print("10");
    }
    else {
      lcd4.print(playerList[4].getCard(1));
    }
  }
  else {
    lcd4.setCursor(0,0);
    lcd4.print("Cards: ");
    for (int i = 0; i < 10; i++) {
      if (playerList[4].getCard(i) != 0) {
        lcd4.print(cardNames[playerList[4].getCard(i)-1]);
        lcd4.print(" ");
      }
    }
    lcd4.setCursor(0,1);
    lcd4.print("Hand Total: ");
    lcd4.print(playerList[4].calculateHandTotal());
  }
}

void displayPlayerHand(int playerNumber) {
  switch (playerNumber) {
    case 0 :
      lcd1.setCursor(0,0);
      lcd1.print("Cards: ");
      for (int i = 0; i < 10; i++) {
        if (playerList[0].getCard(i) != 0) {
          lcd1.print(cardNames[playerList[0].getCard(i)-1]);
          lcd1.print(" ");
        }
      }
      lcd1.setCursor(0,1);
      lcd1.print("Hand Total: ");
      lcd1.print(playerList[0].calculateHandTotal());
      break;
    case 1 :
      lcd2.setCursor(0,0);
      lcd2.print("Cards: ");
      for (int i = 0; i < 10; i++) {
        if (playerList[1].getCard(i) != 0) {
          lcd2.print(cardNames[playerList[1].getCard(i)-1]);
          lcd2.print(" ");
        }
      }
      lcd2.setCursor(0,1);
      lcd2.print("Hand Total: ");
      lcd2.print(playerList[1].calculateHandTotal());
      break;
    case 2 :
      lcd3.setCursor(0,0);
      lcd3.print("Cards: ");
      for (int i = 0; i < 10; i++) {
        if (playerList[2].getCard(i) != 0) {
          lcd3.print(cardNames[playerList[2].getCard(i)-1]);
          lcd3.print(" ");
        }
      }
      lcd3.setCursor(0,1);
      lcd3.print("Hand Total: ");
      lcd3.print(playerList[2].calculateHandTotal());
      break;
    default :
      break;
  }
}

void displayPlayerOutcome(int playerNumber, String display) {
  switch (playerNumber) {
    case 0 :
      lcd1.clear();
      lcd1.setCursor(0,0);
      lcd1.print(display);
      break;
    case 1 :
      lcd2.clear();
      lcd2.setCursor(0,0);
      lcd2.print(display);
      break;
    case 2 :
      lcd3.clear();
      lcd3.setCursor(0,0);
      lcd3.print(display);
      break;
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
        playerList[i].addCard(random(13)+1);
        playerList[i].addCard(random(13)+1);
        displayPlayerHand(0);
      }
      else if (i == 1) {
        playerList[i].addCard(random(13)+1);
        playerList[i].addCard(random(13)+1);
        displayPlayerHand(1);
      }
      else if (i == 2) {
        playerList[i].addCard(random(13)+1);
        playerList[i].addCard(random(13)+1);
        displayPlayerHand(2);
      }
      delay(5000);
      Serial.print("Player hand total: ");
      Serial.println(playerList[i].calculateHandTotal());
    }
  }
  Serial.println("Dealing Cards to Dealer");
  playerList[4].addCard(random(13)+1);
  playerList[4].addCard(random(13)+1);
  displayDealerHand();
  Serial.print("Dealer hand total: ");
  Serial.println(playerList[4].calculateHandTotal());
  delay(5000);
}

void playerHit(int playerNumber) {
  Serial.println("Dealing a card to player");
  int newCard = random(13)+1;
  playerList[playerNumber].addCard(newCard);
  displayPlayerHand(playerNumber);
  delay(5000);
  Serial.print("Total: ");
  Serial.println(playerList[playerNumber].calculateHandTotal());
  Serial.print("Player ");
  Serial.print(playerNumber+1);
  Serial.println(" turn");
}

void playerStand(int playerNumber) {
  Serial.print("Player ");
  Serial.print(playerNumber+1);
  Serial.println(" stands, turn over");
  turnOverFlag = true;
}

void playerBust(int playerNumber) {
  Serial.print("Player ");
  Serial.print(playerNumber+1);
  Serial.println(" busts, turn over");
  turnOverFlag = true;
}

void dealerHit() {
  Serial.println("Dealer Hit, deal a card");
  playerList[4].addCard(random(13)+1);
  delay(5000);
  displayDealerHand();
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
  playerList[playerNumber].setOutcome(win);
  Serial.print("Player ");
  Serial.print(playerNumber+1);
  Serial.println(" wins!");
}

void playerLose(int playerNumber) {
  playerList[playerNumber].setOutcome(loss);
  Serial.print("Player ");
  Serial.print(playerNumber+1);
  Serial.println(" loses!");
}

void playerPush(int playerNumber) {
  playerList[playerNumber].setOutcome(push);
  Serial.print("Player ");
  Serial.print(playerNumber+1);
  Serial.println(" pushes!");
}

void playerBustGameOver(int playerNumber) {
  playerList[playerNumber].setOutcome(loss);
  Serial.print("Player ");
  Serial.print(playerNumber+1);
  Serial.println(" busts!");
}

void dealerBustGameOver(int playerNumber) {
  for (int i = 0; i < 3; i++) {
    playerList[i].setOutcome(win);
  }
  Serial.print("Dealer Busts, Player ");
  Serial.print(playerNumber+1);
  Serial.println(" wins!");
}

void clearTableProcess() {
  Serial.println("Clearing Table");
  delay(5000);
}
