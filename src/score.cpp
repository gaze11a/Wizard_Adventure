#include "score.h"
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <json/json.h>

// Firebase からのレスポンスを受け取るバッファ
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    strncat((char*)userp, (char*)contents, size * nmemb);
    return size * nmemb;
}

void LoadHighScore(Game& game) {
    CURL* curl = curl_easy_init();
    if (!curl) return;

    char response[1024] = { 0 };

    curl_easy_setopt(curl, CURLOPT_URL, FIREBASE_URL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        printf("Failed to load high score from Firebase\n");
        return;
    }

    Json::Value jsonData;
    Json::Reader reader;
    if (!reader.parse(response, jsonData)) {
        printf("Failed to parse Firebase response\n");
        return;
    }

    if (!jsonData["L_max"].isNull()) {
        game.L_max = jsonData["L_max"].asInt();
    }
    if (!jsonData["high_score"].isNull()) {
        game.high_score = jsonData["high_score"].asInt();
    }
}

void SaveHighScore(Game& game) {
    if (game.L > game.L_max) {
        game.L_max = game.L;
    }
    if (game.score > game.high_score) {
        game.high_score = game.score;
    }

    CURL* curl = curl_easy_init();
    if (!curl) return;

    char jsonData[256];
    sprintf(jsonData, "{\"L_max\": %d, \"high_score\": %d}", game.L_max, game.high_score);

    curl_easy_setopt(curl, CURLOPT_URL, FIREBASE_URL);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData);

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        printf("Failed to save high score to Firebase\n");
    }
}
