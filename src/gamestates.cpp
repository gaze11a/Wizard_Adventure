#include "DxLib.h"
#include "game.h"
#include "gamestates.h"
#include "input.h"
#include "sound.h"
#include "score.h"

void HandleMenu(Game& game) {
    if (CheckSoundMem(titleBGM) == 0) {
        PlaySoundMem(titleBGM, DX_PLAYTYPE_LOOP);
    }

    DrawRotaGraph(game.x1, 240, 0.625, 0.0, back[game.i % 3], TRUE);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
    DrawBox(0, 0, 640, 480, GetColor(100, 100, 100), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    DrawExtendGraph(0, -35, 640, 480, titlelogo, TRUE); // タイトル

    // 最高スコアの表示
    int text_x = 320 - GetDrawFormatStringWidth("最高スコア: %d (%dm)", game.high_score, game.L_max) / 2;
    DrawFormatString(text_x, 420, GetColor(75, 0, 130), "最高スコア: %d (%dm)", game.high_score, game.L_max);

    // SPACEキーのテキスト
    text_x = 320 - GetDrawFormatStringWidth("SPACEを押してスタート") / 2;
    DrawFormatString(text_x, 450, GetColor(0, 0, 0), "SPACEを押してスタート");

    DrawFormatString(500, 20, GetColor(0, 0, 0), "設定: S");

    if (Key[KEY_INPUT_SPACE] == 1) {
        StopSoundMem(titleBGM);

        PlaySE(switchSE);
        WaitTimer(1000);
        PlaySoundMem(playingBGM, DX_PLAYTYPE_LOOP);
        game.state = PLAYING;
    }

    if (Key[KEY_INPUT_S] == 1) {
        PlaySE(switchSE);
        game.state = MENU_SETTINGS;
    }
}

void HandlePlaying(Game& game) {
    if (CheckSoundMem(titleBGM) == 1) {
        StopSoundMem(titleBGM);
    }
    if (CheckSoundMem(playingBGM) == 0) {
        PlaySoundMem(playingBGM, DX_PLAYTYPE_LOOP);
    }


    game.x1 -= 2;
    if (game.x1 <= 0) game.x2 -= 2;
    if (game.x2 == 640) {
        game.x1 = game.x2;
        game.x2 = 1280;
    }

    // 背景
    DrawRotaGraph(game.x1, 240, 0.625, 0.0, back[game.i % 3], TRUE);
    DrawRotaGraph(game.x2, 240, 0.625, 0.0, back[game.i % 3], TRUE);

    game.L += 2;
    //DrawFormatString(20, 10, GetColor(0, 255, 0), "%dm", game.L);
    if (game.L % 10000 == 0) game.i = (game.i + 1) % 3; // 10000mごとに背景切り替え

    if (game.L % 1000 == 0) game.score += 300; // 1000mごとにポイント加算
    if ((game.L - game.shot) == 10000 && game.bulletnum < 5) game.bulletnum++; // 弾を撃ってから10000mごとに弾追加


    DrawFormatString(125, 10, GetColor(195, 144, 22), "Score:%d", game.score);
    DrawFormatString(400, 10, GetColor(255, 0, 0), "Press D: attack %d/5", game.bulletnum);

    for (int i = 0; i < game.life; i++) {
        DrawRotaGraph(35 + i * 30, 20, 0.1, 0.0, heartIMG, TRUE);
    }

    // キャラクターの移動
    if (Key[KEY_INPUT_UP] >= 1 && game.y < 220) game.y += 6;
    if (Key[KEY_INPUT_DOWN] >= 1 && game.y > 0) game.y -= 6;

    // キャラクターの描画
    if (game.life > 0) {
        if (game.hitTimer > 0) {
            game.hitTimer--;
            if (game.hitTimer % 10 < 5) {
                DrawRotaGraph(60, 300 - game.y, 0.1, 0.0, charactor, TRUE);
            }
        }
        else {
            DrawRotaGraph(60, 300 - game.y, 0.1, 0.0, charactor, TRUE);
        }
    }

    if (Key[KEY_INPUT_D] == 1 && game.bulletnum > 0) {
        PlaySE(shootSE);
        FireBullet(game);
    }
    UpdateBullets(game);
    UpdateEnemies(game);
}

void HandlePause(Game& game) {
    if (CheckSoundMem(titleBGM) == 1) {
        StopSoundMem(titleBGM);
    }
    // 背景を描画してゲーム画面をうっすら見せる
    DrawRotaGraph(game.x1, 240, 0.625, 0.0, back[game.i % 3], TRUE);
    DrawRotaGraph(game.x2, 240, 0.625, 0.0, back[game.i % 3], TRUE);
    DrawRotaGraph(60, 300 - game.y, 0.1, 0.0, charactor, TRUE);

    // 半透明レイヤーを重ねる
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
    DrawBox(0, 0, 640, 480, GetColor(100, 100, 100), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    DrawFormatString(500, 20, GetColor(255, 255, 255), "設定: S");
    DrawFormatString(230, 230, GetColor(255, 255, 255), "Spaceを押して再開");

    // 設定
	if (Key[KEY_INPUT_S] == 1) {
		game.state = PAUSE_SETTINGS;
        PlaySE(switchSE);
	}

    // ゲーム再開
    if (Key[KEY_INPUT_SPACE] == 1) {
        game.state = PLAYING;
		PlaySE(switchSE);
    }
}

void HandleGameOver(Game& game) {
    if (CheckSoundMem(titleBGM) == 1) {
        StopSoundMem(titleBGM);
    }
    if (CheckSoundMem(playingBGM) == 1) {
        StopSoundMem(playingBGM);
    }

    // 背景を描画してゲーム画面をうっすら見せる
    DrawRotaGraph(game.x1, 240, 0.625, 0.0, back[game.i % 3], TRUE);
    DrawRotaGraph(game.x2, 240, 0.625, 0.0, back[game.i % 3], TRUE);

    // 半透明レイヤーを重ねる
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
    DrawBox(0, 0, 640, 480, GetColor(100, 100, 100), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // 最高記録の更新
    SaveHighScore(game);

    // GAME OVER メッセージ
    SetFontSize(48); // フォントサイズを大きくする

    int text_x = 320 - GetDrawFormatStringWidth("GAME OVER") / 2;
    DrawFormatString(text_x, 120, GetColor(255, 0, 0), "GAME OVER");
    SetFontSize(20); // 他のテキストは通常サイズ

    text_x = 320 - GetDrawFormatStringWidth("スコア: %d (%dm)", game.score, game.L) / 2;
    DrawFormatString(text_x, 230, GetColor(255, 255, 255), "スコア: %d (%dm)", game.score, game.L);

    text_x = 320 - GetDrawFormatStringWidth("最高スコア: %d (%dm)", game.high_score, game.L_max) / 2;
    DrawFormatString(text_x, 290, GetColor(255, 255, 0), "最高スコア: %d (%dm)", game.high_score, game.L_max);

    text_x = 320 - GetDrawFormatStringWidth("Spaceを押してリスタート") / 2;
    DrawFormatString(text_x, 350, GetColor(255, 255, 255), "Spaceを押してリスタート");

    DrawFormatString(500, 20, GetColor(255, 255, 255), "設定: S");

    if (Key[KEY_INPUT_S] == 1) {
        game.state = GAMEOVER_SETTINGS;
        PlaySE(switchSE);
    }

    if (Key[KEY_INPUT_SPACE] == 1) {
        InitGame(game);
        WaitTimer(1000);
        game.state = PLAYING;
    }
}

void HandleSettings(Game& game) {
    // 背景を描画
    DrawRotaGraph(game.x1, 240, 0.625, 0.0, back[game.i % 3], TRUE);
    DrawRotaGraph(game.x2, 240, 0.625, 0.0, back[game.i % 3], TRUE);

    // 半透明レイヤーを重ねる
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
    DrawBox(0, 0, 640, 480, GetColor(100, 100, 100), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // タイトル
    int title_x = 320 - GetDrawFormatStringWidth("設定") / 2;
    DrawFormatString(title_x, 50, GetColor(255, 255, 255), "設定");

    // スライダー用の変数
    static int volumeBGM = 200;
    static int volumeSE = 200;
    static int selected = 0;  // 0: BGM, 1: SE

    // BGM音量の表示
    int bgm_x = 320 - GetDrawFormatStringWidth("BGM 音量: %d") / 2;
    DrawFormatString(bgm_x, 150, GetColor(255, 255, 255), "BGM 音量: %d", volumeBGM);

    // SE音量の表示
    int se_x = 320 - GetDrawFormatStringWidth("SE 音量: %d") / 2;
    DrawFormatString(se_x, 250, GetColor(255, 255, 255), "SE 音量: %d", volumeSE);

    // スライダーの描画
    int sliderWidth = 300;
    int sliderStartX = 320 - (sliderWidth / 2);

    // BGMスライダー
    DrawBox(sliderStartX, 180, sliderStartX + sliderWidth, 200, GetColor(255, 255, 255), FALSE);
    DrawBox(sliderStartX + volumeBGM, 175, sliderStartX + volumeBGM + 20, 205, GetColor(255, 255, 255), TRUE);

    // SEスライダー
    DrawBox(sliderStartX, 280, sliderStartX + sliderWidth, 300, GetColor(255, 255, 255), FALSE);
    DrawBox(sliderStartX + volumeSE, 275, sliderStartX + volumeSE + 20, 305, GetColor(255, 255, 255), TRUE);

    // 戻るボタン
    int back_x = 320 - GetDrawFormatStringWidth("戻る: SPACE") / 2;
    DrawFormatString(back_x, 400, GetColor(255, 255, 255), "戻る: SPACE");

    // 矢印キーで選択変更
    if (Key[KEY_INPUT_UP] == 1 || Key[KEY_INPUT_DOWN] == 1) {
        selected = 1 - selected;
    }

    // 左右キーで音量変更
    if (selected == 0) {
        if (Key[KEY_INPUT_LEFT] == 1 && volumeBGM > 0) volumeBGM -= 10;
        if (Key[KEY_INPUT_RIGHT] == 1 && volumeBGM < 255) volumeBGM += 10;
        ChangeVolumeSoundMem(volumeBGM, titleBGM);
        ChangeVolumeSoundMem(volumeBGM, playingBGM);
    }
    else {
        if (Key[KEY_INPUT_LEFT] == 1 && volumeSE > 0) volumeSE -= 10;
        if (Key[KEY_INPUT_RIGHT] == 1 && volumeSE < 255) volumeSE += 10;
        ChangeVolumeSoundMem(volumeSE, switchSE);
        ChangeVolumeSoundMem(volumeSE, hitSE);
        ChangeVolumeSoundMem(volumeSE, shootSE);
    }

    // 選択中のスライダーを強調表示
    if (selected == 0) {
        DrawBox(sliderStartX - 10, 140, sliderStartX + sliderWidth + 10, 210, GetColor(255, 255, 0), FALSE);
    }
    else {
        DrawBox(sliderStartX - 10, 240, sliderStartX + sliderWidth + 10, 310, GetColor(255, 255, 0), FALSE);
    }

    // 設定画面を閉じる
    if (Key[KEY_INPUT_SPACE] == 1) {
        PlaySE(switchSE);
        switch (game.state)
        {
		case MENU_SETTINGS:
			game.state = MENU;
			break;
		case PAUSE_SETTINGS:
			game.state = PAUSED;
			break;
		case GAMEOVER_SETTINGS:
			game.state = GAME_OVER;
			break;
        }
    }
}

