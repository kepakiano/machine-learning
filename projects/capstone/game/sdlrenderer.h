#ifndef SDLRENDERER_H
#define SDLRENDERER_H

#include <list>

#include "gameevents.hpp"
#include "renderer.h"
#include "sprite.hpp"


class SdlRenderer : public Renderer
{
public:
    SdlRenderer();

    virtual int GetDifficultyLevel() override;
    virtual void Init() override;
    virtual void GameOver(const bool is_new_highscore, const int lives_left, const int score, const int old_highscore) override;
    virtual void Quit() override;


    virtual void SpawnScore(int punkte, int x, int y) override;

    virtual void RenderFrame(const std::list<CAsteroid> &asteroids,
                             const list<CExplosion> &explosions,
                             CPlayer *player,
                             const bool game_is_paused, const double seconds) override;
    virtual GameEvent ProcessEvents() override;


    virtual void handleGameEvent(GameEvent game_event) override;


    virtual void setFramesPerSecond(float fps) override {
        m_pFramesPerSecond->UpdateString(int(fps));
    }

private:
    void RenderAsteroids(const std::list<CAsteroid> &asteroids);
    void RenderExplosions(const list<CExplosion> &explosions);
    void RenderPause	(const double seconds);
    void RenderPlayer(CPlayer *player);

    void UpdateTexts	(CPlayer *player, const double seconds);
    void RenderTexts	();
    void RenderNewHighscore	();

    void toggleFPS(){
        showFPS = !showFPS;
    } // toggleFPS

    // Zu ladende Bilder
    CSprite *m_pSpriteBackground;
    CSprite *m_pSpriteAsteroid;
    CSprite *m_pSpriteExplosion;
    CSprite *m_pSpriteShot;

    CText *m_pFramesPerSecond;

    // Texte für das Start-Menü
    CText *m_pWaehle;
    CText *m_pBruchpilot;
    CText *m_pDurchschnittstyp;
    CText *m_pEliteFlieger;

    // Feste Texte während des Spiels
    CText *m_pLebenText;
    CText *m_pScore_Text;
    CText *m_pLebensenergieText;

    // Dynamische Texte (Zahlen) während des Spiels
    CText *m_pScore;
    CText *m_pLebensenergie;
    CText *m_pLeben;
    CText *m_pPauseText;


    list<CText> m_TextList;

    bool showFPS;

    // Müssen diese Texte upgedatet werden?
    bool m_UpdateScore;
    bool m_UpdateLeben;
    bool m_UpdateLebensenergie;

    // Texte am Spielende
    CText *m_pGameOver;
    CText *m_pHighscoreText;
    CText *m_pHighscore;
    CText *m_pNewHighscoreText1;
    CText *m_pNewHighscoreText2;
    CText *m_pNewHighscore;
    CText *m_pOldHighscoreText;
    CText *m_pOldHighscore;


    float m_fPauseTimer;
    CText *m_pVersion;
};

#endif // SDLRENDERER_H
