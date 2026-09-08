// Tutorial Raylib: Mapa 2D, Câmera, Panning, Zoom no mouse e limites de mundo (2D camera mouse zoom)

// Este código demonstra como criar um sistema completo de câmera 2D com:
// - Movimentação de personagem (bola) contida dentro de um mapa.
// - Arraste de tela (panning) com o botão esquerdo do mouse.
// - Zoom fluido centralizado na posição do ponteiro do mouse.
// - Barreira matemática de câmera que impede visualizar áreas fora do mapa.
// - Grade 2D desenhada manualmente com linhas.

#include "raylib.h"
#include <math.h>     // Necessário para funções matemáticas de zoom avançadas (expf, logf, fmaxf)
#include "raymath.h"  // Funções Utilitárias para Matemática de Vetores (Vector2Add, Vector2Scale, etc.)

int main(){

    const int LARGURA_TELA = 800;
    const int ALTURA_TELA = 450;

    InitWindow(LARGURA_TELA, ALTURA_TELA, "Tutorial 2D mouse zoom");

    // As coordenadas (0,0) representam o centro do nosso mapa no mundo.
    Vector2 posicaoBola = { 0.0f, 0.0f };
    const float RAIO_BOLA = 50.0f;
    const float VELOCIDADE = 10.0f;

    // Dimensões e Limites do Mapa:

    // O mapa possui 5000x5000 pixels no total.
    // Como a origem (0,0) fica no meio, os limites vão de -2500 até +2500 nos eixos X e Y.
    const float MAPA_TAMANHO_TOTAL = 5000.0f;
    const float MIN_MAPA = -2500.0f; // Borda esquerda e superior
    const float MAX_MAPA = 2500.0f;  // Borda direita e inferior
    const float ESPACAMENTO_GRADE = 50.0f; // Tamanho em pixels de cada célula do grid

    // Estrutura e Configuração da Câmera 2D:

    // A Camera2D converte coordenadas do "Mundo" (jogo) para coordenadas da "Tela" (monitor).
    // - target: O ponto em coordenadas do mundo para onde a lente da câmera está apontando.
    // - offset: O ponto na tela (em pixels) onde o 'target' será desenhado (centralizado aqui).
    // - zoom: Escala de visualização (1.0f = tamanho original 100%).
    // - rotation: Rotação da câmera em graus (0.0f = sem rotação).
    Camera2D camera = { 0 };
    camera.zoom = 1.0f; 
    camera.target = posicaoBola; // Câmera inicia focada no ponto central (0,0)
    camera.offset = (Vector2){ (float)LARGURA_TELA / 2.0f, (float)ALTURA_TELA / 2.0f };

    SetTargetFPS(60);

    while(!WindowShouldClose()){

        if (IsKeyDown(KEY_RIGHT)) posicaoBola.x += VELOCIDADE;
        if (IsKeyDown(KEY_LEFT))  posicaoBola.x -= VELOCIDADE;
        if (IsKeyDown(KEY_UP))    posicaoBola.y -= VELOCIDADE;
        if (IsKeyDown(KEY_DOWN))  posicaoBola.y += VELOCIDADE;

        // Colisão com as bordas do mapa:

        // Subtraímos/Somamos o RAIO_BOLA para garantir que a BORDA da bola toque o limite, e não apenas o seu centro.
        if (posicaoBola.x < MIN_MAPA + RAIO_BOLA) posicaoBola.x = MIN_MAPA + RAIO_BOLA;
        if (posicaoBola.x > MAX_MAPA - RAIO_BOLA) posicaoBola.x = MAX_MAPA - RAIO_BOLA;
        if (posicaoBola.y < MIN_MAPA + RAIO_BOLA) posicaoBola.y = MIN_MAPA + RAIO_BOLA;
        if (posicaoBola.y > MAX_MAPA - RAIO_BOLA) posicaoBola.y = MAX_MAPA - RAIO_BOLA;


        // Panning da Câmera (Arrastar o mapa com o Botão Esquerdo do Mouse):

        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            // GetMouseDelta(): Retorna a variação (diferença em pixels) que o cursor se moveu entre o quadro anterior e o quadro atual.
            Vector2 delta = GetMouseDelta();
            
            // Invertemos a direção (-1.0f) para criar a sensação de "pegar e arrastar a mesa".
            // Dividimos pelo zoom para que a velocidade de arraste seja proporcional à distância focal.
            delta = Vector2Scale(delta, -1.0f / camera.zoom); 
            
            // Adiciona o deslocamento ao alvo da câmera
            camera.target = Vector2Add(camera.target, delta);
        }

        // Controle de Zoom Dinâmico com o Mouse:

        // Para impedir que o usuário dê um "Zoom Out" muito distante e veja o fundo fora do mapa, calculamos a taxa mínima de zoom suportada pela resolução da tela atual.
        float zoomMinimoX = (float)LARGURA_TELA / MAPA_TAMANHO_TOTAL;
        float zoomMinimoY = (float)ALTURA_TELA / MAPA_TAMANHO_TOTAL;
        float zoomMinimo = fmaxf(zoomMinimoX, zoomMinimoY); // Pega o maior valor de escala necessário

        float scrollMouse = GetMouseWheelMove(); // Retorna +1 (rolou p/ cima) ou -1 (p/ baixo)
        if(scrollMouse != 0){
            // GetScreenToWorld2D(): Transforma as coordenadas em pixels do monitor onde o mouse está para a posição real correspondente no mundo do jogo.
            Vector2 mouseMundoPos = GetScreenToWorld2D(GetMousePosition(), camera);
            
            // Reajusta a âncora da câmera para a posição atual do mouse no monitor, permitindo que o zoom seja direcionado para onde o ponteiro está apontando.
            camera.offset = GetMousePosition();
            camera.target = mouseMundoPos;
            
            // Cálculo exponencial de escala: Garante um zoom suave, fluido e proporcional.
            float escala = 0.2f * scrollMouse;
            camera.zoom = Clamp(expf(logf(camera.zoom) + escala), zoomMinimo, 64.0f);
        }

        // Clamp/Barreira da Câmera (Sistema Anti-Exposição de Borda):

        // Mapeia onde os 4 cantos da TELA do monitor estão tocando no MUNDO do jogo.
        Vector2 telaTopLeft = GetScreenToWorld2D((Vector2){0, 0}, camera);
        Vector2 telaBottomRight = GetScreenToWorld2D((Vector2){LARGURA_TELA, ALTURA_TELA}, camera);

        float correcaoX = 0.0f;
        float correcaoY = 0.0f;

        // Se o canto superior esquerdo da visão ultrapassar a borda esquerda do mapa (-2500)
        if (telaTopLeft.x < MIN_MAPA) correcaoX = MIN_MAPA - telaTopLeft.x;
        // Se o canto inferior direito ultrapassar a borda direita (+2500)
        if (telaBottomRight.x > MAX_MAPA) correcaoX = MAX_MAPA - telaBottomRight.x;
        
        // Aplica a mesma lógica para o eixo Y (superior e inferior)
        if (telaTopLeft.y < MIN_MAPA) correcaoY = MIN_MAPA - telaTopLeft.y;
        if (telaBottomRight.y > MAX_MAPA) correcaoY = MAX_MAPA - telaBottomRight.y;

        // Empurra o foco da câmera de volta para o limite permitido
        camera.target.x += correcaoX;
        camera.target.y += correcaoY;

        BeginDrawing();
        
            ClearBackground(DARKGRAY);

            // Modo 2D (Afetado pela posição, zoom e rotação da Câmera):

            BeginMode2D(camera);
                
                // Desenha o chão do mapa em branco (área jogável)
                DrawRectangle(MIN_MAPA, MIN_MAPA, MAPA_TAMANHO_TOTAL, MAPA_TAMANHO_TOTAL, RAYWHITE);

                // Desenha as linhas da grade 2D manualmente usando loops
                for (float i = MIN_MAPA; i <= MAX_MAPA; i += ESPACAMENTO_GRADE) {
                    // Linhas Verticais
                    DrawLineV((Vector2){i, MIN_MAPA}, (Vector2){i, MAX_MAPA}, LIGHTGRAY);
                    // Linhas Horizontais
                    DrawLineV((Vector2){MIN_MAPA, i}, (Vector2){MAX_MAPA, i}, LIGHTGRAY);
                }

                DrawCircleV(posicaoBola, RAIO_BOLA, MAROON);

            EndMode2D();

            // Exibe as coordenadas do mouse no canto do cursor
            DrawTextEx(GetFontDefault(), TextFormat("[%i, %i]", GetMouseX(), GetMouseY()),
                       Vector2Add(GetMousePosition(), (Vector2){ -44, -24 }), 20, 2, BLACK);

            // Textos de instrução no topo da tela
            DrawText("Use o botao esquerdo do mouse para arrastar o mapa", 20, 20, 20, BLACK);
            DrawText("Use a rodinha do mouse para dar zoom (com limite de borda)", 20, 50, 20, BLACK);
            DrawText("Setas direcionais movem a bola sem sair da grade", 20, 80, 20, BLACK);

        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}