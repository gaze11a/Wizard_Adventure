#include "DxLib.h"
#include "game.h"
#include "gamestates.h"
#include "input.h"
#include "sound.h"
#include <stdio.h>

// 未実装機能
// 
// 宝箱の追加
// 効果音の追加

Game game;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    ChangeWindowMode(TRUE);
    DxLib_Init();
    SetDrawScreen(DX_SCREEN_BACK);

    game.state = MENU;
    InitGame(game);
    LoadHighScore(game);
    LoadSounds();

    while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && gpUpdateKey() == 0) {
        if (Key[KEY_INPUT_ESCAPE] == 1) {
            if (game.state == PLAYING) game.state = PAUSED;
            else if (game.state == PAUSED) game.state = PLAYING;
        }

        switch (game.state) {
        case MENU:
            HandleMenu(game);
            break;
        case PAUSED:
            HandlePause(game);
            break;
        case PLAYING:
            HandlePlaying(game);
            break;
        case GAME_OVER:
            HandleGameOver(game);
            break;
        }
    }
    DxLib_End();
    return 0;
}
