// Tutorial Raylib: Câmera 2D, Movimento do Jogador, Rotação e Zoom (2d camera)

// Este código demonstra como criar um sistema de câmera 2D com:
// - Movimentação de um personagem (um retângulo vermelho) usando as setas do teclado.
// - Câmera que segue o jogador, mantendo-o sempre como alvo central.
// - Rotação da câmera usando as teclas A e D, com limites matemáticos de inclinação.
// - Zoom utilizando a rodinha do mouse, com base em escala logarítmica.
// - Separação clara entre "Espaço do Mundo" (tudo que a câmera vê) e "Espaço da Tela" (interface fixa/UI).

#include "raylib.h"
#include <math.h> // Necessário para as funções matemáticas expf() e logf() usadas no zoom.

#define MAX_CONSTRUCOES 100 // Constante que define a quantidade de prédios gerados no cenário.

int main(){

    const int LARGURA_TELA = 800;
    const int ALTURA_TELA = 450;

    InitWindow(LARGURA_TELA, ALTURA_TELA, "Tutorial 2d camera");

    // Cria o jogador usando a estrutura Rectangle (x, y, largura, altura).
    // Começa na posição x= 400, y= 280 e tem tamanho 40x40.
    Rectangle jogador = { 400, 280, 40, 40 };

    Color COR_JOGADOR = RED; // Cor do jogador (retângulo vermelho).

    // Vetores para guardar as informações das construções de cenário (retângulos e cores).
    Rectangle construcoes[MAX_CONSTRUCOES] = { 0 };
    Color cor_construcoes[MAX_CONSTRUCOES] = { 0 };

    Color COR_CHAO = DARKGRAY; // Cor do chão (retângulo cinza escuro).
    Color COR_CEU = RAYWHITE; // Cor do céu (fundo da tela).
    Color COR_GRADE = GREEN; // Cor das linhas da grade (verde).

    int espacamento = 0;
    float VELOCIDADE = 10.0f;

    // Loop para gerar e posicionar as construções do cenário aleatoriamente.
    for (int i = 0; i < MAX_CONSTRUCOES; i++)
    {
        // Define largura e altura aleatórias usando GetRandomValue() (mínimo, máximo).
        construcoes[i].width = (float)GetRandomValue(50, 200);
        construcoes[i].height = (float)GetRandomValue(100, 800);

        // Posiciona a construção no eixo Y (altura) de forma que fiquem alinhadas na mesma base.
        construcoes[i].y = ALTURA_TELA - 130.0f - construcoes[i].height;

        // Posiciona no eixo X (horizontal) espalhando-as da esquerda para a direita
        construcoes[i].x = -6000.0f + espacamento;

        espacamento += (int)construcoes[i].width; // Acumula o espaço para a próxima construção

        // Define uma cor aleatória em tons de cinza/azulado para cada prédio.
        // Color: R, G, B, Alpha (transparência). Valores de 0 a 255.
        cor_construcoes[i] = (Color){
            (unsigned char)GetRandomValue(200, 240),
            (unsigned char)GetRandomValue(200, 240),
            (unsigned char)GetRandomValue(200, 250),
            255};
    }

    // Configuração inicial da Câmera 2D:

    // O struct Camera2D diz ao Raylib como transformar a visão do mundo.
    Camera2D camera = { 0 };

    // camera.target: Ponto no mundo (Vector2) para onde a câmera está olhando (foco). 
    // Apontamos para o centro do jogador (x + 20, y + 20, já que o tamanho dele é 40).
    camera.target = (Vector2){ jogador.x + 20.0f, jogador.y + 20.0f };

    // camera.offset: Ponto na TELA (Vector2) onde o target será desenhado.
    // Colocando metade da largura e altura da tela, o jogador ficará sempre no centro da tela.
    camera.offset = (Vector2){ LARGURA_TELA/2.0f, ALTURA_TELA/2.0f };

    // Rotação inicial (0 graus) e Zoom inicial (1.0f representa 100%, sem zoom).
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_RIGHT)) jogador.x += VELOCIDADE;
        else if (IsKeyDown(KEY_LEFT)) jogador.x -= VELOCIDADE;

        // Limita a posição X do jogador aos limites do chão (-6000 até 7000 no eixo X).
        // 7000 é o limite final (X inicial -6000 + largura 13000) subtraído a largura do jogador (40).
        if (jogador.x < -6000.0f){
            jogador.x = -6000.0f;
        }else if (jogador.x > 7000.0f - jogador.width){
            jogador.x = 7000.0f - jogador.width;
        }

        // Atualiza o alvo da câmera para seguir o jogador constantemente.
        // Se não atualizarmos o target, a câmera ficará parada onde o jogador nasceu.
        camera.target = (Vector2){ jogador.x + 20, jogador.y + 20 };

        // Controles de rotação da câmera (teclas A e D)
        if (IsKeyDown(KEY_A)) camera.rotation--;
        else if (IsKeyDown(KEY_D)) camera.rotation++;

        // Limita a rotação para não passar de -40 e 40 graus.
        // Para evitar que a câmera gire 360 graus e desoriente o jogador.
        if (camera.rotation > 40) camera.rotation = 40;
        else if (camera.rotation < -40) camera.rotation = -40;

        // Controle do Zoom da câmera com a rodinha do mouse.
        // GetMouseWheelMove() retorna 1 ou -1 (ou 0 se estiver parada).
        // Usamos logf e expf (logaritmo e exponencial) para que o zoom seja fluido. 
        // Se somássemos diretamente, o zoom in pareceria mais rápido que o zoom out.
        camera.zoom = expf(logf(camera.zoom) + ((float)GetMouseWheelMove()*0.1f));

        // Limita o zoom da câmera para não afastar demais (< 0.1f) nem aproximar demais (> 3.0f).
        if (camera.zoom > 3.0f) camera.zoom = 3.0f;
        else if (camera.zoom < 0.1f) camera.zoom = 0.1f;

        // Reseta o zoom e a rotação para os valores originais quando a tecla "R" for apertada.
        if (IsKeyPressed(KEY_R))
        {
            camera.zoom = 1.0f;
            camera.rotation = 0.0f;
        }

        BeginDrawing();

            ClearBackground(COR_CEU); // Limpa a tela com a cor do céu (fundo azul claro).
            
            // Inicia o modo de desenho 2D com a nossa câmera.
            // Parâmetro: O struct Camera2D configurado.
            // Tudo que for desenhado entre BeginMode2D e EndMode2D sofrerá deslocamento, rotação e zoom. Esse é o "Espaço do Mundo".
            BeginMode2D(camera);

                // Desenha o fundo/chão cinza escuro.
                // x, y, largura, altura, cor.
                DrawRectangle(-6000, 320, 13000, 8000, COR_CHAO);

                // Loop para desenhar todas as construções que geramos no início.
                for (int i = 0; i < MAX_CONSTRUCOES; i++) DrawRectangleRec(construcoes[i], cor_construcoes[i]);

                // Desenha o jogador (nosso retângulo vermelho).
                // Passando o struct Rectangle do jogador e a cor.
                DrawRectangleRec(jogador, COR_JOGADOR);

                // Desenha um eixo cartesiano (linha verde em forma de cruz) bem no centro da câmera.
                // DrawLine: x inicial, y inicial, x final, y final, cor.
                DrawLine((int)camera.target.x, -ALTURA_TELA*10, (int)camera.target.x, ALTURA_TELA*10, COR_GRADE);
                DrawLine(-LARGURA_TELA*10, (int)camera.target.y, LARGURA_TELA*10, (int)camera.target.y, COR_GRADE);

                // Finaliza o modo 2D. 
                // Tudo que for desenhado a partir daqui será afixado diretamente na tela (ignorando a câmera).
                // Esse é o "Espaço da Tela" (Interface Gráfica / UI)
            EndMode2D();

            // Desenha o texto "SCREEN AREA" no canto superior direito.
            DrawText("SCREEN AREA", 640, 10, 20, RED);

            // Desenha 4 retângulos formando uma borda vermelha ao redor da tela inteira (UI).
            DrawRectangle(0, 0, LARGURA_TELA, 5, RED);  // Borda Superior
            DrawRectangle(0, 5, 5, ALTURA_TELA - 10, RED);  // Borda Esquerda
            DrawRectangle(LARGURA_TELA - 5, 5, 5, ALTURA_TELA - 10, RED); // Borda Direita
            DrawRectangle(0, ALTURA_TELA - 5, LARGURA_TELA, 5, RED);  // Borda Inferior

            // Desenha a caixa de texto azul claro transparente das instruções.
            // A função Fade() altera a transparência de uma cor (0.0f a 1.0f).
            DrawRectangle( 10, 10, 250, 113, Fade(SKYBLUE, 0.5f));
            // Desenha apenas a linha/contorno azul em volta da caixa.
            DrawRectangleLines( 10, 10, 250, 113, BLUE);

            // Escreve os textos explicativos das instruções na tela.
            DrawText("Controles 2D camera:", 20, 20, 10, BLACK);
            DrawText("- Direita/Esquerda para mover jogador", 40, 40, 10, DARKGRAY);
            DrawText("- Rodinha do Mouse para Zoom", 40, 60, 10, DARKGRAY);
            DrawText("- A / D para Rotacionar", 40, 80, 10, DARKGRAY);
            DrawText("- R para reiniciar Zoom e Rotacao", 40, 100, 10, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}