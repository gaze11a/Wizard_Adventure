#include "DxLib.h"
#include "firebase.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>


void SaveScoreToFirebase(const char* playerName, int score) {
    CURL* curl = curl_easy_init();
    if (!curl) return;

    char jsonData[256];  // JSON データを格納するバッファ
    sprintf(jsonData, "{\"player\":\"%s\", \"score\":%d}", playerName, score);

    curl_easy_setopt(curl, CURLOPT_URL, FIREBASE_URL);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData);

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        printf("Failed to send data to Firebase!\n");
    }

    curl_easy_cleanup(curl);
}
