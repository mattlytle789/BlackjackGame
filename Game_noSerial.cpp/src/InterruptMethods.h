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