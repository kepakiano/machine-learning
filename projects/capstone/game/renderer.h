#ifndef RENDERER_H
#define RENDERER_H

#include "asteroid.hpp"
#include "explosion.hpp"
#include "gameevents.hpp"
#include "player.hpp"

class Renderer
{
public:
    Renderer();

    virtual int GetDifficultyLevel() = 0;
    virtual void Init() = 0;
    virtual void GameOver(const bool is_new_highscore,
                          const int lives_left,
                          const int score,
                          const int old_highscore) = 0;
    virtual void Quit() = 0;


    virtual void SpawnScore(int punkte, int x, int y) =0;

    virtual void RenderFrame(const std::list<CAsteroid> &asteroids,
                             const list<CExplosion> &explosions,
                             CPlayer *player,
                             const bool game_is_paused) = 0;
    virtual GameEvent ProcessEvents() = 0;
    virtual void handleGameEvent(GameEvent game_event) = 0;

    virtual void setFramesPerSecond(float fps) = 0;
};

#endif // RENDERER_H
