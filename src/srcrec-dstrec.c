// Tutorial Raylib: Tutorial de Rotação do Personagem (srcrec dstrec)

// Este código demonstra como manipular e girar um personagem 2D com Raylib:
// - Recorte de quadros de animação a partir de uma spritesheet (sourceRec).
// - Posicionamento e escala do personagem na tela com retângulo de destino (destRec).
// - Definição do ponto de pivô/origem (origin) para centralizar a rotação.
// - Rotação contínua do sprite em torno do próprio eixo usando DrawTexturePro().
// - Linhas guia de eixos desenhadas manualmente no centro da tela.

#include "raylib.h"

int main(void)
{
    const int LARGURA_TELA = 800;
    const int ALTURA_TELA = 450;

    InitWindow(LARGURA_TELA, ALTURA_TELA, "Raylib - Tutorial de Rotação do Personagem");

    // LoadTexture: Pega a imagem salva no seu computador e a envia para a memória da placa de vídeo (VRAM). ("caminho/do/arquivo.png").
    // A variável "scarfy" agora guarda essa referência. 
    Texture2D scarfy = LoadTexture("../assets/images/scarfy.png");

    // O arquivo "scarfy.png" é uma "spritesheet" (vários quadros de animação em uma imagem só).
    // Como ele tem 6 quadros, dividimos a largura total da imagem por 6 para pegar apenas 1 quadro.
    int frameLargura = scarfy.width / 6;
    int frameAltura = scarfy.height;

    // sourceRec (Retângulo de Origem): Define qual "pedaço" da imagem original vamos recortar.
    // {posição_X_na_imagem, posição_Y_na_imagem, largura_do_recorte, altura_do_recorte}.
    Rectangle sourceRec = { 0.0f, 0.0f, (float)frameLargura, (float)frameAltura };

    // destRec (Retângulo de Destino): Define onde o personagem vai aparecer na tela e seu tamanho final.
    // {posição_X_na_tela, posição_Y_na_tela, largura_final, altura_final}.
    // Colocamos no meio da tela (screenWidth/2, screenHeight/2) e multiplicamos o tamanho do frame por 2 (frameLargura*2.0f) para que o personagem fique o dobro do tamanho original.
    Rectangle destRec = { LARGURA_TELA / 2.0f, ALTURA_TELA / 2.0f, frameLargura * 2.0f, frameAltura * 2.0f };

    // origin (Ponto de Origem): O ponto exato do personagem onde o giro vai acontecer.
    // {posição_X_relativa, posição_Y_relativa}
    // Como o tamanho final (destRec) é frameLargura*2 e frameAltura*2, definir a origem apenas como frameLargura e frameAltura coloca o "pino" de giro exatamente no meio do personagem.
    Vector2 origin = { (float)frameLargura, (float)frameAltura };

    // Variável que vai guardar o ângulo atual do personagem.
    int rotacao = 0;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Aumentamos o ângulo em 1 grau a cada quadro (frame). É isso que faz ele girar continuamente.
        rotacao++;

        BeginDrawing();

            ClearBackground(RAYWHITE);

            // DrawTexturePro: É a versão "Avançada" (Pro) de desenhar texturas. Ela é a única que permite girar e escalar a imagem usando um ponto de pivô customizado. 
            // Parâmetros: 
            // 1. A textura original (scarfy)
            // 2. O retângulo de recorte (sourceRec) -> qual parte da textura usar
            // 3. O retângulo de destino (destRec) -> onde desenhar e qual o tamanho
            // 4. O ponto de origem (origin) -> o "centro de massa" ou "pivô" da rotação
            // 5. O ângulo de rotação (rotation) -> o valor que está crescendo no loop
            // 6. A cor de tingimento (WHITE) -> WHITE significa desenhar com as cores originais
            DrawTexturePro(scarfy, sourceRec, destRec, origin, (float)rotacao, WHITE);

            // DrawLine: Desenha duas linhas na tela em forma de cruz para mostrar exatamente onde fica o centro da tela (e o centro da rotação do personagem).
            // (X_inicial, Y_inicial, X_final, Y_final, COR)
            DrawLine((int)destRec.x, 0, (int)destRec.x, ALTURA_TELA, GRAY); // Linha vertical
            DrawLine(0, (int)destRec.y, LARGURA_TELA, (int)destRec.y, GRAY);  // Linha horizontal

            DrawText("(c) Scarfy sprite by Eiden Marsal", LARGURA_TELA - 200, ALTURA_TELA - 20, 10, GRAY);

        EndDrawing();
    }

    // UnloadTexture: Quando o jogo acaba, precisamos liberar o espaço que a imagem estava ocupando na memória da placa de vídeo.
    // (textura_carregada)
    UnloadTexture(scarfy);

    CloseWindow();

    return 0;
}