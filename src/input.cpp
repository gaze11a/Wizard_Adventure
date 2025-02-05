#include "DxLib.h"
#include "input.h"

int Key[256]; // 実体の定義（グローバル変数）

int gpUpdateKey() {
    char tmpKey[256];
    GetHitKeyStateAll(tmpKey);
    for (int i = 0; i < 256; i++) {
        if (tmpKey[i] != 0)
            Key[i]++;
        else
            Key[i] = 0;
    }

	return 0;
}