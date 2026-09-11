// Tutorial raylib de como fazer um objeto se mover com a rodinha do mouse (input mouse wheel)
#include "raylib.h"

int main(){

    const int LARGURA_TELA = 800;
    const int ALTURA_TELA = 450;

    InitWindow(LARGURA_TELA, ALTURA_TELA, "Tutorial input mouse wheel");

    // Variável para o tamanho da caixa (largura e altura iguais, pois é um quadrado)
    int TAMANHO_CAIXA = 80;

    // A posição do Retângulo é baseada no seu CANTO SUPERIOR ESQUERDO, e não no centro.
    // Para centralizar perfeitamente na tela, subtraímos a metade do tamanho da caixa.
    int CAIXA_POSICAOY = ALTURA_TELA/2 - (TAMANHO_CAIXA/2);
    int CAIXA_POSICAOX = LARGURA_TELA/2 - (TAMANHO_CAIXA/2);

    // Velocidade de movimento ao rolar o mouse
    int SCROLAR_VELOCIDADE = 4; 

    Color COR_CAIXA = MAROON;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Movimentação com a rodinha do mouse

        // GetMouseWheelMove: Retorna o movimento da rodinha do mouse. 
        // Multiplicamos pela velocidade para andar mais pixels por rolagem.
        CAIXA_POSICAOY -= (int)(GetMouseWheelMove() *  SCROLAR_VELOCIDADE);

        // Sistema de Colisão (Canto Superior Esquerdo)
        // Como o retângulo é desenhado pelo canto superior esquerdo:

        // Barreira de Cima: O limite é o Y chegar a 0.
        if (CAIXA_POSICAOY <= 0) CAIXA_POSICAOY = 0;
        
        // Barreira de Baixo: O limite é a altura da tela menos o tamanho inteiro da caixa.
        // Se não subtrairmos o tamanho, a caixa desce até sumir da tela antes de travar.
        if (CAIXA_POSICAOY >= ALTURA_TELA - TAMANHO_CAIXA) CAIXA_POSICAOY = ALTURA_TELA - TAMANHO_CAIXA;

        BeginDrawing();

        ClearBackground(RAYWHITE);

        // DrawRectangle: Pede a Posição X, Posição Y, Largura, Altura e a Cor.
        DrawRectangle(CAIXA_POSICAOX, CAIXA_POSICAOY, TAMANHO_CAIXA,TAMANHO_CAIXA, COR_CAIXA);

        DrawText("Mova a caixa para cima e para baixo rolando o mouse", 10, 10, 20, GRAY);
        
        // TextFormat: Permite formatar um texto com variáveis dentro do DrawText (parecido com o printf).
        // %03i formata o número inteiro para sempre ter 3 dígitos (ex: 005, 050, 500).
        DrawText(TextFormat("Caixa posicao Y: %03i", CAIXA_POSICAOY), 10, 40, 20, LIGHTGRAY);

        EndDrawing();

    }
    
    CloseWindow();

    return 0;
}