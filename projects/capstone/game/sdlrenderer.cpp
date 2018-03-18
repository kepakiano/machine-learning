#include "sdlrenderer.h"

#include <thread>

SdlRenderer::SdlRenderer(int screen_width, int screen_height)
    : m_pSpriteBackground(nullptr)
    , m_pSpriteAsteroid(nullptr)
    , m_fPauseTimer(0.0f)
    , screen_width(screen_width)
    , screen_height(screen_height)
{
}


int SdlRenderer::GetDifficultyLevel(){

    SDL_Event Event;

    m_pSpriteBackground->Render();
    m_pWaehle->Render();
    m_pBruchpilot->Render();
    m_pDurchschnittstyp->Render();
    m_pEliteFlieger->Render();
    m_pVersion->Render();
    g_pFramework->Flip();

    while(true){
        if(SDL_PollEvent(&Event)){
            if(SDL_KEYDOWN) {
                switch(Event.key.keysym.sym){
                    case (SDLK_1): return 1;
                    case (SDLK_2): return 2;
                    case (SDLK_3): return 3;
                }
            }
        }
    }
}


void SdlRenderer::Init(){
    if(g_pFramework->Init(screen_width, screen_height,16,false) == false){
        std::cerr << "Could not initialize SDL framework" << std::endl;
        return;
    }
    TTF_Init();

    m_pWaehle = new CText("Wie gut fliegst du?", 210, 260, 15);
    m_pBruchpilot = new CText("1. Bruchpilot", 210, 290, 15);
    m_pDurchschnittstyp = new CText("2. Durchschnittstyp", 210, 320, 15);
    m_pEliteFlieger = new CText("3. Elite-Flieger", 210, 350, 15);

    m_pSpriteBackground = new CSprite;
    m_pSpriteBackground->Load("../assets/Background.png");

    m_pSpriteExplosion = new CSprite;
    m_pSpriteExplosion->Load("../assets/explosion3.png", 25, 64, 64);
    m_pSpriteExplosion->SetColorKey(0,0,0);

    m_pSpriteAsteroid = new CSprite;
    m_pSpriteAsteroid->Load("../assets/Asteroid.png", 20, 64, 64);
    m_pSpriteAsteroid->SetColorKey(255,0,255);

    m_pSpritePlayer = new CSprite;
    m_pSpritePlayer->Load("../assets/Player.png",11,64,64);
    m_pSpritePlayer->SetColorKey(255,0,255);

    m_pSpriteShot = new CSprite;
    m_pSpriteShot->Load("../assets/Laser.png",0,64,64);
    m_pSpriteShot->SetColorKey(255,0,255);

    m_pLebensenergie = new CText(200, 15, 15);
    m_pLebensenergieText = new CText("Raumstation: ", 10, 15, 15);

    m_pLeben = new CText(120, 35, 15);
    m_pLebenText = new CText("Leben: ", 10, 35, 15);

    m_pScore_Text = new CText("Score:", 600, 15, 15);
    m_pScore = new CText(720, 15, 15);

    m_pHighscoreText = new CText("Erreichte Punktzahl: ", 210, 320, 15);
    m_pHighscore = new CText(560, 320, 15);

    m_pFramesPerSecond = new CText(10, 580, 12);
    showFPS = false;

    m_UpdateScore = true;
    m_UpdateLeben = true;
    m_UpdateLebensenergie = true;

    m_pPauseText = new CText("PAUSE",230, 260, 72);

    m_pVersion = new CText("0.14",10,580, 12);
} // Init

void SdlRenderer::Quit(){
    if(m_pSpriteBackground != NULL){
        delete(m_pSpriteBackground);
        m_pSpriteBackground = NULL;
    }
    if(m_pSpriteAsteroid != NULL){
        delete(m_pSpriteAsteroid);
        m_pSpriteAsteroid = NULL;
    }
    if(m_pSpriteExplosion != NULL){
        delete(m_pSpriteExplosion);
        m_pSpriteExplosion = NULL;
    }
}

void SdlRenderer::RenderFrame(const std::list<CAsteroid> &asteroids,
                              const list<CExplosion> &explosions,
                              CPlayer *player,
                              const bool game_is_paused,
                              const double seconds){
    g_pFramework->Update();
    g_pFramework->Clear();
    m_pSpriteBackground->Render();
    if(showFPS)
        m_pFramesPerSecond->Render();
    if(game_is_paused){
        RenderPause(seconds);
    }
    else{
        RenderAsteroids(asteroids);
        RenderExplosions(explosions);
        UpdateTexts(player, seconds);
        RenderTexts();
        RenderPlayer(player);
    }

    g_pFramework->Flip();
}


void SdlRenderer::RenderAsteroids(const std::list<CAsteroid> &asteroids){
    for(const CAsteroid &asteroid : asteroids){
        m_pSpriteAsteroid->SetPos(asteroid.GetXPos(), asteroid.GetYPos());
        m_pSpriteAsteroid->Render(asteroid.GetAnimPhase());
    }
}

void SdlRenderer::RenderExplosions(const list<CExplosion> &explosions){
    for(const CExplosion &explosion : explosions){
        m_pSpriteExplosion->SetPos(explosion.GetXPos(), explosion.GetYPos());
        m_pSpriteExplosion->Render(explosion.GetAnimPhase());
    }
}

void SdlRenderer::RenderPause(const double seconds){
    m_fPauseTimer += seconds;
    // Wenn das Spiel pausiert ist, wird der Schriftzug PAUSE jeweils für 0,4 Sekunden gezeigt
    if(m_fPauseTimer > 0.4f){
        m_pPauseText->Render();
        if(m_fPauseTimer > 0.8f)
            m_fPauseTimer = 0.0f;
    }
}

void SdlRenderer::RenderPlayer(CPlayer *player){
    if(player->shouldBeRendered()){
        m_pSpritePlayer->SetPos(player->GetXPosition(), player->GetYPosition());
        m_pSpritePlayer->Render(player->getAnimPhase());
    }

    for(const CShot & shot : player->GetShotList()){
        if(shot.IsAlive()){
            m_pSpriteShot->SetPos(shot.GetXPos(), shot.GetYPos());
            m_pSpriteShot->Render();
        }
    }
}

GameEvent SdlRenderer::ProcessEvents(){
    SDL_Event Event;
    if(SDL_PollEvent(&Event)){
        switch(Event.type){
            case (SDL_QUIT): {
                return END_GAME;
            } break;
            case(SDL_KEYDOWN): {
                switch(Event.key.keysym.sym){
                    case (SDLK_ESCAPE): {
                        return END_GAME;
                    } break;
                    case (SDLK_f):{
                        toggleFPS();
                    } break;
                    case (SDLK_p):{
                        return TOGGLE_PAUSE;
                    } break;
                }
            } break;
        }
    }
    return EMPTY_EVENT;
}

void SdlRenderer::handleGameEvent(GameEvent game_event)
{
    if(game_event == UPDATE_LIVES_TEXT)
        m_UpdateLeben = true;
    else if(game_event == UPDATE_SCORE_TEXT)
        m_UpdateScore = true;
    else if(game_event == UPDATE_ENERGY_TEXT)
        m_UpdateLebensenergie = true;

}


void SdlRenderer::UpdateTexts(CPlayer *player,
                              const double seconds){
    // Lebensenergie der Raumstation updaten?
    if(m_UpdateLebensenergie == true)
        m_pLebensenergie->UpdateString(player->GetLebensenergie_Raumstation());

    // Leben updaten?
    if(m_UpdateLeben == true){
        m_pLeben->UpdateString(player->GetLeben());
        m_UpdateLeben = false;
    }

    // Score updaten?
    if(m_UpdateScore == true){
        m_pScore->UpdateString(player->GetScore());
        m_UpdateScore = false;
    }

    // Wenn die Raumstation noch nicht komplett wieder repariert ist, soll sie im nächsten Frame upgedatet werden
    if(player->GetLebensenergie_Raumstation() == 100)
        m_UpdateLebensenergie = false;

    // Updaten der aufsteigend transparenter werdenden Score-Texte
    list<CText>::iterator ItText = m_TextList.begin();
    while(ItText != m_TextList.end()){
        // Ist der Text kurz vom verschwinden? Dann löschen.
        if(ItText->GetTransparency() < 20.0f)
            ItText = m_TextList.erase(ItText);
        else {
            ItText->Update(3.0f, seconds);
            ItText->Render();
            ItText++;
        }
    }

} // UpdateTexts

void SdlRenderer::RenderTexts(){
    // Alle (ggf geupdaten) Texte rendern
    m_pLebenText->Render();
    m_pLebensenergieText->Render();
    m_pScore_Text->Render();
    m_pLebensenergie->Render();
    m_pLeben->Render();
    m_pScore->Render();
} // RenderTexts

void SdlRenderer::GameOver(const bool is_new_highscore,
                           const int lives_left,
                           const int score,
                           const int old_highscore){
    g_pFramework->Clear();
    m_pSpriteBackground->Render();
    if(lives_left == 0)
        m_pGameOver = new CText("Du hast all deine Leben verloren!", 210,300, 15);
    else
        m_pGameOver = new CText("Die Raumstation wurde vernichtet!", 210,300, 15);

    if(is_new_highscore){
        m_pNewHighscoreText1 = new CText("Du hast einen neuen Highscore erreicht!", 210, 350, 15);
        m_pOldHighscoreText = new CText("Alter Highscore: ", 210, 380, 15);
        m_pOldHighscore = new CText(440, 380, 15);
        m_pNewHighscoreText2 = new CText("Neuer Highscore: ", 210, 410, 15);
        m_pNewHighscore = new CText(440, 410, 15);

        m_pNewHighscore->UpdateString(score);
        m_pOldHighscore->UpdateString(old_highscore);
        RenderNewHighscore();
    }
    m_pGameOver->Render();
    m_pHighscoreText->Render();
    m_pHighscore->UpdateString(score);
    m_pHighscore->Render();
    g_pFramework->Flip();

    std::this_thread::sleep_for(std::chrono::seconds(3));
}

void SdlRenderer::RenderNewHighscore(){
    m_pNewHighscoreText1->Render();
    m_pNewHighscoreText2->Render();
    m_pOldHighscoreText->Render();

    m_pNewHighscore->Render();

    m_pOldHighscore->Render();
}

void SdlRenderer::SpawnScore(int punkte, int x, int y){

    // Erzeugen eines neuen aufsteigenden Texts
    CText *PunkteText = new CText(std::to_string(punkte), x, y, 14);
    m_TextList.push_back(*PunkteText);
}
