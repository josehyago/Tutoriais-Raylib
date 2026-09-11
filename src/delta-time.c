// Tutorial raylib delta time (delta time)
#include "raylib.h"

int main(){

    const int LARGURA_TELA = 800;
    const int ALTURA_TELA = 450;

    InitWindow(LARGURA_TELA, ALTURA_TELA, "Tutorial delta time");

    int FPS_ATUAL = 60;

    // Colocamos as bolas em posições diferentes no eixo Y para visualizar a corrida.
    Vector2 DELTA_BOLA = { 0, (float)ALTURA_TELA/3.0f };
    Vector2 FRAME_BOLA = { 0, (float)ALTURA_TELA * (2.0f/3.0f) };
    
    const float VELOCIDADE = 10.0f;
    const float RAIO_BOLA = 32.0f;
  

    SetTargetFPS(FPS_ATUAL);

    while (!WindowShouldClose())
    {
        // Lógica para alterar o FPS rolando a rodinha do mouse
        float SCROLAR_MOUSE = GetMouseWheelMove();
        if (SCROLAR_MOUSE != 0)
        {
            FPS_ATUAL += (int)SCROLAR_MOUSE;
            if (FPS_ATUAL <= 0) FPS_ATUAL = 0; // 0 significa FPS ilimitado na Raylib
            SetTargetFPS(FPS_ATUAL);

        }

        // Bola vermelha (com Delta Time): 
        // GetFrameTime(): retorna o tempo em segundos desde o último frame.
        // Ao multiplicar pela velocidade, garantimos que a bola ande na mesma velocidade independente de o jogo estar a 60 FPS ou 10 FPS.
        DELTA_BOLA.x += GetFrameTime() * 6.0f * VELOCIDADE;

        // Bola azul (sem Delta Time / Frame-based):
        // Anda um valor fixo a cada frame. Se o FPS cair, a bola fica mais lenta.
        FRAME_BOLA.x += 0.1f * VELOCIDADE;

        // Sistema de loop: se passar da tela, volta pro começo
        if (DELTA_BOLA.x > LARGURA_TELA) DELTA_BOLA.x = 0;
        if (FRAME_BOLA.x > LARGURA_TELA) FRAME_BOLA.x = 0;

        // Resetar posições apertando "R"
        if (IsKeyPressed(KEY_R))
        {
            DELTA_BOLA.x = 0;
            FRAME_BOLA.x = 0;
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        DrawCircleV(DELTA_BOLA, RAIO_BOLA, RED);
        DrawCircleV(FRAME_BOLA, RAIO_BOLA, BLUE);

        // Lógica para mostrar o texto de FPS atualizado
        const char *fpsText = 0;
        if (FPS_ATUAL <= 0) fpsText = TextFormat("FPS: ilimitado (%i)", GetFPS());
        else fpsText = TextFormat("FPS: %i (target: %i)", GetFPS(), FPS_ATUAL);

        DrawText(fpsText, 10, 10, 20, DARKGRAY);

        // Multiplicamos por 1000.0f para converter os segundos de GetFrameTime() em milissegundos (ms)
        DrawText(TextFormat("Frame time: %02.02f ms", GetFrameTime() * 1000.0f), 10, 30, 20, DARKGRAY);

        DrawText("Role a rodinha do mouse para mudar o FPS e R para resetar", 10, 50, 20, DARKGRAY);

        DrawText("BOLA VERMELHA (Delta Time): x += GetFrameTime() * VELOCIDADE", 10, 90, 20, RED);
        DrawText("BOLA AZUL (Sem Delta Time): x += VELOCIDADE", 10, 240, 20, BLUE);

        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}