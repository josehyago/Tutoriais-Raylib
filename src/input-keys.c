// Tutorial raylib de como fazer um objeto se mover com base nas teclas do teclado (input keys)
#include "raylib.h"

int main(){

    const int LARGURA_TELA = 800;
    const int ALTURA_TELA = 450;

    InitWindow(LARGURA_TELA, ALTURA_TELA, "Tutorial input keys");

    
    Vector2 POSICAO_BOLA = { (float)LARGURA_TELA/2, (float)ALTURA_TELA/2 };
    // Vector2: Estrutura do Raylib que guarda dois valores: x e y. Muito usado para posições 2D.
    // Usamos (float) para converter a divisão inteira em um número decimal, garantindo precisão na tela.
    // Ao dividirmos a largura e a altura da tela por 2, colocamos a bola exatamente no centro.

    float RAIO_BOLA = 50.0f;
    // Váriavel que guarda o raio da bola, para alterar o tamanho dela é só mudar o valor aqui.

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Movimentação:
        // Note que os "if"s abaixo não usam chaves { }. 
        // Quando um "if" tem apenas uma instrução, as chaves são opcionais. 

        // IsKeyDown: Checa se a tecla especificada está sendo pressionada e mantida.
        // O eixo X cresce para a direita. O eixo Y cresce para BAIXO.
        if (IsKeyDown(KEY_RIGHT)) POSICAO_BOLA.x += 2.0f;
        if (IsKeyDown(KEY_LEFT)) POSICAO_BOLA.x -= 2.0f;
        if (IsKeyDown(KEY_UP)) POSICAO_BOLA.y -= 2.0f;
        if (IsKeyDown(KEY_DOWN)) POSICAO_BOLA.y += 2.0f;
        
        // Sistema de Colisão (Limites da tela)
        // Se a posição da bola passar do limite, nós a forçamos a ficar no limite máximo permitido.
        // Detalhes: A posição X e Y da POSICAO_BOLA representa o centro dela;
        // Se usássemos apenas "LARGURA_TELA" ou "0" como limite, metade da bola sairia da tela antes de parar.
        // Para evitar isso, usamos o RAIO_BOLA em todos os lados.

        // Barreira da Direita: Limite é a LARGURA total menos a metade da bola (o raio).
        // Se a bola tentar passar desse limite, nós a travamos nele.
        if (POSICAO_BOLA.x >= (LARGURA_TELA - RAIO_BOLA)) POSICAO_BOLA.x = LARGURA_TELA - RAIO_BOLA;
        
        // Barreira da Esquerda: Limite é o próprio tamanho do raio.
        // Impede que o X chegue a 0 ou fique negativo.
        if (POSICAO_BOLA.x <= RAIO_BOLA) POSICAO_BOLA.x = RAIO_BOLA;
        
        // Barreira de Baixo: Limite é a ALTURA total menos a metade da bola (o raio).
        // Se a bola tentar passar desse limite, nós a travamos nele.
        if (POSICAO_BOLA.y >= (ALTURA_TELA - RAIO_BOLA)) POSICAO_BOLA.y = ALTURA_TELA - RAIO_BOLA;
        
        // Barreira de Cima: Limite é o próprio tamanho do raio.
        // Impede que o Y chegue a 0 ou fique negativo.
        if (POSICAO_BOLA.y <= RAIO_BOLA) POSICAO_BOLA.y = RAIO_BOLA;

        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Mova a bola com as seta", 10, 10, 20, DARKGRAY);

        DrawCircleV(POSICAO_BOLA, RAIO_BOLA, MAROON);
        // DrawCircleV: Desenha um círculo usando um Vector2 para a posição, utiliza-se Posição(Vector2), Raio, Cor.


        EndDrawing();
    }
    
    CloseWindow();

    return 0;
}