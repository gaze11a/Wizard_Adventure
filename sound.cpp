#include "DxLib.h"
#include "../src/sound.h"

int titleBGM, playingBGM, gameOverBGM, startSE, enemyHitSE, moveSE, shootSE;

// ƒTƒEƒ“ƒh‚Ì“Ç‚İ‚İ
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

// Œø‰Ê‰¹‚ğÄ¶
void PlaySE(int sound) {
    PlaySoundMem(sound, DX_PLAYTYPE_BACK);
}