#ifndef FIREBASE_H
#define FIREBASE_H

#define FIREBASE_URL "https://wizard-adventure.firebaseio.com/scores.json?key=AIzaSyBtU6Chf5izpCknsrqOqQTc3nohAJfp7Qg"

void SaveScoreToFirebase(const char* playerName, int score);

#endif
