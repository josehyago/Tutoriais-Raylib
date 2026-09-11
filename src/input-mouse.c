// Tutorial raylib de como fazer um objeto se mover com o mouse (input mouse)
#include "raylib.h"

int main(){

    const int LARGURA_TELA = 800;
    const int ALTURA_TELA = 450;

    InitWindow(LARGURA_TELA, ALTURA_TELA, "Tutorial input mouse");

    Vector2 POSICAO_BOLA = { -100.0f, -100.0f };
    // Iniciamos a posição no negativo (-100, -100) para que a bola comece desenhada fora da tela.
    // Isso evita que ela pisque no canto (0,0) antes de conseguir ler a posição do mouse.

    Color COR_BOLA = DARKBLUE;
    // Color: Dá a cor.

    float RAIO_BOLA = 40.0f;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // IsKeyPressed: Retorna verdadeiro apenas uma vez no exato momento do aperto.
        // Diferente do IsKeyDown que aciona continuamente enquanto segurado.
        if (IsKeyPressed(KEY_H))
        {
            // IsCursorHidden: Checa se o cursor está invisível na janela.
            if(IsCursorHidden()) ShowCursor(); // ShowCursor: Torna o cursor do mouse visível.
            else HideCursor(); // HideCursor: Esconde a setinha do mouse.

        }
        
        // GetMousePosition: Retorna um Vector2 contendo o X e o Y exatos de onde o cursor está na tela.
        POSICAO_BOLA = GetMousePosition();

        // IsMouseButtonPressed: Checa se o botão do mouse foi clicado (apenas no momento do clique).
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) COR_BOLA = MAROON;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) COR_BOLA = LIME;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) COR_BOLA = DARKBLUE;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_SIDE)) COR_BOLA = PURPLE;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_EXTRA)) COR_BOLA = YELLOW;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_FORWARD)) COR_BOLA = ORANGE;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_BACK)) COR_BOLA = BEIGE;

        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawCircleV(POSICAO_BOLA, RAIO_BOLA, COR_BOLA);

        DrawText("Mova a bola com o mouse e aperte os botoes do mouse para mudar de cor", 10, 10, 20, DARKGRAY);
        DrawText("Aperte H para esconder/aparecer o cursor", 10, 30, 20, DARKGRAY);

        if (IsCursorHidden()) DrawText("Cursor Escondido", 20, 60, 20, RED);
        else DrawText("Cursor visivel", 20, 60, 20, LIME);

        EndDrawing();

    }
    
    CloseWindow();

    return 0;
}