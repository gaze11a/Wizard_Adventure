#include "DxLib.h"
#include "../src/sound.h"

int titleBGM, playingBGM, gameOverBGM, startSE, enemyHitSE, moveSE, shootSE;

// サウンドの読み込み
void LoadSounds() {
    titleBGM = LoadSoundMem("../assets/bgm/title.mp3");
    playingBGM = LoadSoundMem("../assets/bgm/playing.wav");
    gameOverBGM = LoadSoundMem("../assets/bgm/gameover.wav");

    startSE = LoadSoundMem("../assets/sound/start.wav");
    enemyHitSE = LoadSoundMem("../assets/sound/hit.wav");
    moveSE = LoadSoundMem("../assets/sound/move.wav");
    shootSE = LoadSoundMem("../assets/sound/shoot.wav");

    ChangeVolumeSoundMem(200, titleBGM);
}

// 効果音を再生
void PlaySE(int sound) {
    PlaySoundMem(sound, DX_PLAYTYPE_BACK);
}