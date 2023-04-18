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
volatile int receiverState = 0; // state of the position receiver
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
LiquidCrystal_I2C lcd1(0x23, 20, 4);
LiquidCrystal_I2C lcd2(0x25, 20, 4);
LiquidCrystal_I2C lcd3(0x20, 20, 4);
LiquidCrystal_I2C lcd4(0x27, 20, 4);
/*************************************/

/***** Misc Variables ****************/
String cardNames[] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
//String outcomes[] = {"Win","Lose","Push","Blackjack"};
volatile int receiverCount = 0; // count of the number of receivers
/*************************************/
void setup() {
  // For testing purposes
  Serial.begin(115200);
  randomSeed(analogRead(A3));
  
  // allocating memory for player list and filling with null
  playerList = new Player[4];
  dealer = Player(3);
  playerList[3] = dealer;
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
  pinMode(receiver, INPUT_PULLUP);
  //pinMode(clearButton, INPUT_PULLUP);
  //pinMode(cvModuleInputSignal, INPUT);

  // setting output pins
  pinMode(player1ControllerPower, OUTPUT);
  pinMode(player2ControllerPower, OUTPUT);
  pinMode(player3ControllerPower, OUTPUT);
  pinMode(CVout, OUTPUT);
  //pinMode(cvModuleOutputSignal, OUTPUT);

  // setting interrupts
  PCICR |= B00000100; // Opening Port D
  PCMSK2 |= B00010000; // setting pin 4 to have interrupt
  PCMSK2 |= B00100000; // setting pin 5 to have interrupt
  PCMSK2 |= B01000000; // setting pin 6 to have interrupt
  PCICR |= B00000001; // Opening Port B
  PCMSK0 |= B00000010; // setting pin 9 to have interrupt
  PCMSK0 |= B00100000; // setting pin 13 as Receiver

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
      Serial.println(player1JoinedFlag);
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
      Serial.println("Dealing State Over");
      for (int i = 0; i < 4; i++) {
        if (playerList[i].getNumber() != 0) {
          if (playerList[i].calculateHandTotal() == 21) {
            playerList[i].setBlackjackFlag(true);
          }
        }
      }
      if (playerList[3].getBlackjackFlag()) {
        gameState = gameOver;
      }
      else {
        gameState = playerAction;
      }
      break;
    // Player Action state
    case playerAction :
      Serial.println("Player Action State");
      for (int i = 0; i < 3; i++) {
        if (playerList[i].getNumber() != 0 && !playerList[i].getBlackjackFlag()) {
          Serial.print("Player ");
          Serial.print(playerList[i].getNumber());
          Serial.println(" turn start");
          switch (i) {
            case 0 : // Turning on player 1 controller
              digitalWrite(player1ControllerPower, HIGH);
              digitalWrite(player2ControllerPower, LOW);
              digitalWrite(player3ControllerPower, LOW);
              lcd1.setCursor(0,2);
              lcd1.print("Your Turn");
              lcd2.setCursor(0,2);
              lcd2.print("         ");
              lcd3.setCursor(0,2);
              lcd3.print("         ");
              break;
            case 1 : // Turning on player 2 controller
              digitalWrite(player1ControllerPower, LOW);
              digitalWrite(player2ControllerPower, HIGH);
              digitalWrite(player3ControllerPower, LOW);
              lcd2.setCursor(0,2);
              lcd2.print("Your Turn");
              lcd1.setCursor(0,2);
              lcd1.print("         ");
              lcd3.setCursor(0,2);
              lcd3.print("         ");
              break;
            case 2 : // Turning on player 3 controller
              digitalWrite(player1ControllerPower, LOW);
              digitalWrite(player2ControllerPower, LOW);
              digitalWrite(player3ControllerPower, HIGH);
              lcd3.setCursor(0,2);
              lcd3.print("Your Turn");
              lcd2.setCursor(0,2);
              lcd2.print("         ");
              lcd1.setCursor(0,2);
              lcd1.print("         ");
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
      lcd1.setCursor(0,2);
      lcd1.print("         ");
      lcd2.setCursor(0,2);
      lcd2.print("         ");
      lcd3.setCursor(0,2);
      lcd3.print("         ");
      Serial.println("Player Action State Over");
      gameState = dealerAction;    
      break;
    // Dealer Action State
    case dealerAction :
      Serial.println("Dealer Action State");
      displayDealerHand();
      while (!dealerTurnOverFlag) {
        if (playerList[3].calculateHandTotal() > 21) {
          dealerBust();
        }
        else if (playerList[3].calculateHandTotal() >= 17) {
          dealerStand();
        }
        else if (playerList[3].calculateHandTotal() <= 16) {
          dealerHit();
        }
      }
      Serial.println("Dealer Action State Over");
      gameState = gameOver;
      break;
    // Game Over State
    case gameOver :
      Serial.println("Game Over State");
      if (playerList[3].getBlackjackFlag()) {
            dealerBlackjackGameOver();
      }
      else if (playerList[3].calculateHandTotal() > 21) {
            dealerBustGameOver();
      }
      else { 
        for (int i = 0; i < 3; i++) {
          if (playerList[i].getNumber() != 0) {
            Serial.print("Dealer total: ");
            Serial.println(playerList[3].calculateHandTotal());
            Serial.print("Player ");
            Serial.print(i+1);
            Serial.print(" total: ");
            Serial.println(playerList[i].calculateHandTotal());
            if (playerList[i].calculateHandTotal() > 21) {
              playerBustGameOver(i);
            }
            else if (playerList[i].getBlackjackFlag()) {
              playerBlackjack(i);
            }
            else if (playerList[i].calculateHandTotal() > playerList[3].calculateHandTotal()) {
              playerWin(i);
            }
            else if (playerList[i].calculateHandTotal() < playerList[3].calculateHandTotal()) {
              playerLose(i);
            }
            else if (playerList[i].calculateHandTotal() == playerList[3].calculateHandTotal()) {
              playerPush(i);
            }
          }
        }
      }
      //displayPlayerOutcome();
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
      playerList[3] = dealer;
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
  receiverState = digitalRead(receiver);
  if (receiverState == 1) {
    receiverCount++;
  }
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
    lcd4.print(cardNames[playerList[3].getCard(1)-1]);
    lcd4.print(" ");
    lcd4.setCursor(0,1);
    lcd4.print("Hand Total: ");
    if (playerList[3].getCard(1) == 1) {
      lcd4.print("11");
    }
    else if (playerList[3].getCard(1) == 11 || playerList[3].getCard(1) == 12 || playerList[3].getCard(1) == 13) {
      lcd4.print("10");
    }
    else {
      lcd4.print(playerList[3].getCard(1));
    }
  }
  else {
    lcd4.setCursor(0,0);
    lcd4.print("Cards: ");
    for (int i = 0; i < 10; i++) {
      if (playerList[3].getCard(i) != 0) {
        lcd4.print(cardNames[playerList[3].getCard(i)-1]);
        lcd4.print(" ");
      }
    }
    lcd4.setCursor(0,1);
    lcd4.print("Hand Total: ");
    lcd4.print(playerList[3].calculateHandTotal());
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

void displayPlayerOutcome(int playerNumber, String outcome) {
  Serial.println("Display Check");
  switch (playerNumber) {
    case 0 :
      lcd1.setCursor(0,3);
      lcd1.print("Outcome: "+outcome);
      break;
    case 1 :
      lcd2.setCursor(0,3);
      lcd2.print("Outcome: "+outcome);
      break;
    case 2 :
      lcd3.setCursor(0,3);
      lcd3.print("Outcome: "+outcome);
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
  for (int i = 0; i < 8; i++) {
    int playerNumber = i%4;
    receiverCount = 0;
    if (playerList[playerNumber].getNumber() != 0) {
      drawCard(playerNumber);
      // deal card to player
      if (playerNumber == 3) {
        dealDealer();
        displayDealerHand();
      }
      else {
        dealPlayer(playerNumber);
        displayPlayerHand(playerNumber);
      }  
    }
  }
}

void drawCard(int playerNumber) {
  // Drop arm onto card
  digitalWrite(motorPolarity, LOW);
  analogWrite(enMotor1, 0);
  analogWrite(enMotor2, 0);
  analogWrite(enMotor3, 100);
  delay(350);
  digitalWrite(motorPolarity, HIGH);
  analogWrite(enMotor1, 0);
  analogWrite(enMotor2, 0);
  analogWrite(enMotor3, 0);
  delay(500);
  while (Serial.available() > 0) {
    Serial.read();
  }
  digitalWrite(CVout, HIGH);
  receivePlayerCard(playerNumber);
  digitalWrite(CVout, LOW);
  /*
  // Drag to reader
  digitalWrite(motorPolarity, LOW);
  analogWrite(enMotor1, 0);
  analogWrite(enMotor2, 210);
  analogWrite(enMotor3, 0);
  delay(420);
  digitalWrite(motorPolarity, HIGH);
  analogWrite(enMotor1, 0);
  analogWrite(enMotor2, 0);
  analogWrite(enMotor3, 0);
  delay(500);*/
}

void receivePlayerCard(int playerNumber) {
  Serial.println("reading card...");
  int input  = '0';
  while (true) {
    if (Serial.available()) {
      break;
    }
  }
  input = Serial.read();
  Serial.println(input);
  switch (input) {
    case 'A' :
      Serial.println("Received A");
      playerList[playerNumber].addCard(1);
      break;
    case '2' :
      Serial.println("Received 2");
      playerList[playerNumber].addCard(2);
      break;
    case '3' :
      Serial.println("Received 3");
      playerList[playerNumber].addCard(3);
      break;
    case '4' :
      Serial.println("Received 4");
      playerList[playerNumber].addCard(4);
      break;
    case '5' :
      Serial.println("Received 5");
      playerList[playerNumber].addCard(5);
      break;
    case '6' :
      Serial.println("Received 6");
      playerList[playerNumber].addCard(6);
      break;
    case '7' :
      Serial.println("Received 7");
      playerList[playerNumber].addCard(7);
      break;
    case '8' :
      Serial.println("Received 8");
      playerList[playerNumber].addCard(8);
      break;
    case '9' :
      Serial.println("Received 9");
      playerList[playerNumber].addCard(9);
      break;
    case 'T' :
      Serial.println("Received 10");
      playerList[playerNumber].addCard(10);
      break;
    case 'J' :
      Serial.println("Received J");
      playerList[playerNumber].addCard(11);
      break;
    case 'Q' :
      Serial.println("Received Q");
      playerList[playerNumber].addCard(12);
      break;
    case 'K' :
      Serial.println("Received K");
      playerList[playerNumber].addCard(13);
      break;
    default :
      Serial.println("ERROR! Bad Card Read");
      break;
  }
}

void dealPlayer(int playerNumber) {
  receiverCount = 0;
  int receiverCountMax = 0;
  switch (playerNumber) {
    case 0 :
      receiverCountMax = 1;
      break;
    case 1 :
      receiverCountMax = 2;
      break;
    case 2 :
      receiverCountMax = 3;
      break;
  }
  Serial.print("Receiver Max: ");
  Serial.println(receiverCountMax);
  // swing arm to player position
  digitalWrite(motorPolarity, LOW);
  analogWrite(enMotor1, 0);
  analogWrite(enMotor2, 200);
  analogWrite(enMotor3, 0);
  while (receiverCount < receiverCountMax) {
    continue;
  }
  digitalWrite(motorPolarity, HIGH);
  analogWrite(enMotor1, 0);
  analogWrite(enMotor2, 0);
  analogWrite(enMotor3, 0);
  delay(500);
  // extend arm to player
  digitalWrite(motorPolarity, HIGH);
  analogWrite(enMotor1, 110);
  analogWrite(enMotor2, 0);
  analogWrite(enMotor3, 0);
  delay(475);
  digitalWrite(motorPolarity, LOW);
  analogWrite(enMotor1, 0);
  analogWrite(enMotor2, 0);
  analogWrite(enMotor3, 0);
  delay(500);
  // raise arm 
  digitalWrite(motorPolarity, HIGH);
  analogWrite(enMotor1, 0);
  analogWrite(enMotor2, 0);
  analogWrite(enMotor3, 140);
  delay(520);
  digitalWrite(motorPolarity, LOW);
  analogWrite(enMotor1, 0);
  analogWrite(enMotor2, 0);
  analogWrite(enMotor3, 0);
  delay(500);
  // retract arm 
  digitalWrite(motorPolarity, LOW);
  analogWrite(enMotor1, 110);
  analogWrite(enMotor2, 0);
  analogWrite(enMotor3, 0);
  delay(480);
  digitalWrite(motorPolarity, HIGH);
  analogWrite(enMotor1, 0);
  analogWrite(enMotor2, 0);
  analogWrite(enMotor3, 0);
  delay(500);
  returnToStart(playerNumber, receiverCountMax);
}

void returnToStart(int playerNumber, int receiverCountMax) {
  if (playerNumber == 0) {
    // swing to card shoe
    receiverCount = 0;
    digitalWrite(motorPolarity, HIGH);
    analogWrite(enMotor1, 0);
    analogWrite(enMotor2, 200);
    analogWrite(enMotor3, 0);
    while (receiverCount < 1) {
      continue;
    }
    digitalWrite(motorPolarity, LOW);
    analogWrite(enMotor1, 0);
    analogWrite(enMotor2, 0);
    analogWrite(enMotor3, 0);
    delay(500);
    // Drag to reader
    digitalWrite(motorPolarity, LOW);
    analogWrite(enMotor1, 0);
    analogWrite(enMotor2, 200);
    analogWrite(enMotor3, 0);
    delay(480);
    digitalWrite(motorPolarity, HIGH);
    analogWrite(enMotor1, 0);
    analogWrite(enMotor2, 0);
    analogWrite(enMotor3, 0);
    delay(500);
  } 
  else {
    // swing to player 1
    receiverCount = 0;
    digitalWrite(motorPolarity, HIGH);
    analogWrite(enMotor1, 0);
    analogWrite(enMotor2, 210);
    analogWrite(enMotor3, 0);
    while (receiverCount < receiverCountMax-1) {
      continue;
    }
    digitalWrite(motorPolarity, LOW);
    analogWrite(enMotor1, 0);
    analogWrite(enMotor2, 0);
    analogWrite(enMotor3, 0);
    delay(500);
    // swing to card shoe
    receiverCount = 0;
    digitalWrite(motorPolarity, HIGH);
    analogWrite(enMotor1, 0);
    analogWrite(enMotor2, 200);
    analogWrite(enMotor3, 0);
    while (receiverCount < 1) {
      continue;
    }
    digitalWrite(motorPolarity, LOW);
    analogWrite(enMotor1, 0);
    analogWrite(enMotor2, 0);
    analogWrite(enMotor3, 0);
    delay(500);
    // Drag to reader
    digitalWrite(motorPolarity, LOW);
    analogWrite(enMotor1, 0);
    analogWrite(enMotor2, 200);
    analogWrite(enMotor3, 0);
    delay(480);
    digitalWrite(motorPolarity, HIGH);
    analogWrite(enMotor1, 0);
    analogWrite(enMotor2, 0);
    analogWrite(enMotor3, 0);
    delay(500);
  }
}

void dealDealer() {
  receiverCount = 0;
  int receiverCountMax = 2;
  // swing arm to dealer position
  digitalWrite(motorPolarity, LOW);
  analogWrite(enMotor1, 0);
  analogWrite(enMotor2, 200);
  analogWrite(enMotor3, 0);
  while (receiverCount < receiverCountMax) {
    continue;
  }
  digitalWrite(motorPolarity, HIGH);
  analogWrite(enMotor1, 0);
  analogWrite(enMotor2, 0);
  analogWrite(enMotor3, 0);
  delay(500);
  // retract arm 
  digitalWrite(motorPolarity, LOW);
  analogWrite(enMotor1, 120);
  analogWrite(enMotor2, 0);
  analogWrite(enMotor3, 0);
  delay(500);
  digitalWrite(motorPolarity, HIGH);
  analogWrite(enMotor1, 0);
  analogWrite(enMotor2, 0);
  analogWrite(enMotor3, 0);
  delay(500);
  // raise arm 
  digitalWrite(motorPolarity, HIGH);
  analogWrite(enMotor1, 0);
  analogWrite(enMotor2, 0);
  analogWrite(enMotor3, 140);
  delay(450);
  digitalWrite(motorPolarity, LOW);
  analogWrite(enMotor1, 0);
  analogWrite(enMotor2, 0);
  analogWrite(enMotor3, 0);
  delay(500);
  // extend arm 
  digitalWrite(motorPolarity, HIGH);
  analogWrite(enMotor1, 110);
  analogWrite(enMotor2, 0);
  analogWrite(enMotor3, 0);
  delay(400);
  digitalWrite(motorPolarity, LOW);
  analogWrite(enMotor1, 0);
  analogWrite(enMotor2, 0);
  analogWrite(enMotor3, 0);
  delay(500);
  returnToStart(3, receiverCountMax);
}

void playerHit(int playerNumber) {
  Serial.println("Dealing a card to player");
  //delay(2000);
  //int newCard = random(13)+1;
  //playerList[playerNumber].addCard(newCard);
  drawCard(playerNumber);
  dealPlayer(playerNumber);

  displayPlayerHand(playerNumber);
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
  //playerList[3].addCard(random(13)+1);
  drawCard(3);
  dealDealer();
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
  Serial.print("Player ");
  Serial.print(playerNumber+1);
  Serial.println(" wins!");
  displayPlayerOutcome(playerNumber, "Win");
}

void playerLose(int playerNumber) {
  Serial.print("Player ");
  Serial.print(playerNumber+1);
  Serial.println(" loses!");
  displayPlayerOutcome(playerNumber, "Loss");
}

void playerPush(int playerNumber) {
  Serial.print("Player ");
  Serial.print(playerNumber+1);
  Serial.println(" pushes!");
  displayPlayerOutcome(playerNumber, "Push");
}

void playerBustGameOver(int playerNumber) {
  Serial.print("Player ");
  Serial.print(playerNumber+1);
  Serial.println(" busts!");
  displayPlayerOutcome(playerNumber, "Bust");
}

void playerBlackjack(int playerNumber) {
  Serial.print("Player ");
  Serial.print(playerNumber+1);
  Serial.println(" blackjack!");
  displayPlayerOutcome(playerNumber, "Blackjack");
}

void dealerBustGameOver() {
  for (int i = 0; i < 3; i++) {
    Serial.print("Player Total: ");
    Serial.println(playerList[i].calculateHandTotal());
    Serial.print("Dealer Total: ");
    Serial.println(playerList[3].calculateHandTotal());
    Serial.print("Dealer Busts, Player ");
    Serial.print(i+1);
    Serial.println(" wins!");
    if (playerList[i].calculateHandTotal() > 21) {
      displayPlayerOutcome(i, "Loss");
    }
    else {
      displayPlayerOutcome(i, "Win");
    }    
  }
}

void dealerBlackjackGameOver() {
  displayDealerHand();
  for (int i = 0; i < 3; i++) {
    if (playerList[i].getNumber()) {
      Serial.print("Player Total: ");
      Serial.println(playerList[i].calculateHandTotal());
      Serial.print("Dealer Total: ");
      Serial.println(playerList[3].calculateHandTotal());
      Serial.print("Dealer Blackjack, Player ");
      Serial.print(i+1);
      Serial.println(" loses!");
      if (playerList[i].getBlackjackFlag()) {
        displayPlayerOutcome(i, "Push");
      }
      else {
        displayPlayerOutcome(i, "Loss");
      }
    }
  }
}

void clearTableProcess() {
  Serial.println("Clearing Table");
}
