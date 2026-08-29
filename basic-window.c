// Tutorial raylib de como fazer uma simples janela (basic window)
#include "raylib.h"

// Biblioteca raylib.
// < >: O compilador procura a biblioteca nos diretórios padrões do sistema (onde o compilador foi instalado).
// " ": O compilador procura a biblioteca primeiro no diretório atual do seu projeto (onde está o seu código).

int main(){

    const int LARGURA_TELA = 800;
    const int ALTURA_TELA = 450;
    // Definindo a altura e largura da janela do programa.
    // const serve para definir valores constantes, que não podem ser mudados.

    InitWindow(LARGURA_TELA, ALTURA_TELA, "Tutorial basic window");
    // InitWindow: inicializa a janela do programa, coloca-se a largura, altura e nome do programa.

    SetTargetFPS(60);
    // SetTargetFPS: Define a quantidade de quadros por segundo (FPS) que o programa vai rodar.

    // Loop principal que garante o funcionamento do programa.
    // ! = Diferente; 
    // WindowShouldClose: Checa se o programa deve o usuário clicou no x ou ESC para que o programa feche.
    while (!WindowShouldClose())
    {
        BeginDrawing();
        // BeginDrawing: Prepara a tela para desenhar o que for posto no código.
        ClearBackground(RAYWHITE);
        // ClearBackground: Limpa a tela anterior e define a cor do fundo.
        DrawText("Primeiro tutorial: Basic Window", 190, 200, 20, BLACK);
        // DrawText: Desenha um texto na tela, coloca-se o texto, posição de x, y, tamanho da fonte e cor.
        EndDrawing();
        // EndDrawing: Finaliza a renderização e exibe o desenho na tela.
    }
    
    CloseWindow();
    // CloseWindow: Fecha a janela do programa e libera a memória alocada.

    return 0;
}