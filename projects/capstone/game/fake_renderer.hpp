#ifndef FAKE_RENDERER_HPP
#define FAKE_RENDERER_HPP

#include <iostream>

#include "renderer.h"

class FakeRenderer : public Renderer{
    virtual int GetDifficultyLevel() override {return 1;}

    virtual void Init() override {}
    virtual void GameOver(const bool is_new_highscore,
                          const int lives_left,
                          const int score,
                          const int old_highscore) override {
        std::cout << "The player reached a score of " << score << std::endl;
    }
    virtual void Quit() override {}


    virtual void SpawnScore(int punkte, int x, int y) override {}


    virtual void RenderFrame(const std::list<CAsteroid> &asteroids,
                             const list<CExplosion> &explosions,
                             CPlayer *player,
                             const bool game_is_paused,
                             const double seconds) override {}

    virtual GameEvent ProcessEvents() override {}
    virtual void handleGameEvent(GameEvent game_event) override {}
    virtual void setFramesPerSecond(float fps) override {}
};
#endif // FAKE_RENDERER_HPP
