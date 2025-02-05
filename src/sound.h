#ifndef SOUND_H
#define SOUND_H

#include "DxLib.h"

extern int titleBGM, playingBGM;
extern int startSE, hitSE, shootSE;

void LoadSounds();
void PlaySE(int sound);

#endif
