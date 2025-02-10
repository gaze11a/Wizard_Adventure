#ifndef SCORE_H
#define SCORE_H

#include "game.h"

#define FIREBASE_URL "https://wizard-adventure.firebaseio.com/scores.json?key=AIzaSyBtU6Chf5izpCknsrqOqQTc3nohAJfp7Qg"

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
void LoadHighScore(Game& game);
void SaveHighScore(Game& game);

#endif
