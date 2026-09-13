// Tutorial Raylib: Mapa 2D, Câmera, Panning, Zoom no mouse e limites de mundo (2D camera platformer)

// Este código demonstra como criar um sistema completo de câmera 2D com:
// - Movimentação de personagem (bola e retângulo de colisão) contida dentro de um mapa.
// - Pulos e sistema de gravidade básica.
// - Diferentes modos de câmera (seguir direto, com limites, suavizada, etc).
// - Zoom fluido com a rodinha do mouse.

#include "raylib.h"
#include "raymath.h"

#define GRAVIDADE 400
#define VEL_PULO_JOGADOR 350.0f
#define VEL_HORIZ_JOGADOR 200.0f

// Definição de Tipos e Estruturas
typedef struct Jogador {
    Vector2 posicao;
    float velocidade;
    bool podePular;
    Color cor;
} Jogador;

typedef struct ElementoCenario {
    Rectangle retangulo;
    int temColisao;
    Color cor;
} ElementoCenario;

// Declaração das Funções do Módulo
void AtualizarJogador(Jogador *jogador, ElementoCenario *elementocenario, int elementocenarioTotal, float delta);
void AtualizarCameraCentro(Camera2D *camera, Jogador *jogador, ElementoCenario *elementocenario, int elementocenarioTotal, float delta, int largura, int altura);
void AtualizarCameraCentroNoMapa(Camera2D *camera, Jogador *jogador, ElementoCenario *elementocenario, int elementocenarioTotal, float delta, int largura, int altura);
void AtualizarCameraSeguirSuave(Camera2D *camera, Jogador *jogador, ElementoCenario *elementocenario, int elementocenarioTotal, float delta, int largura, int altura);
void AtualizarCameraSuavizarNoPouso(Camera2D *camera, Jogador *jogador, ElementoCenario *elementocenario, int elementocenarioTotal, float delta, int largura, int altura);
void AtualizarCameraEmpurrarNosLimites(Camera2D *camera, Jogador *jogador, ElementoCenario *elementocenario, int elementocenarioTotal, float delta, int largura, int altura);

int main()
{
    const int LARGURA_TELA = 800;
    const int ALTURA_TELA = 450;

    InitWindow(LARGURA_TELA, ALTURA_TELA, "Tutorial - 2D Camera Platformer");

    // Inicializando o jogador
    Jogador jogador = { 0 };
    jogador.posicao = (Vector2){ 400, 280 };
    jogador.velocidade = 0;
    jogador.podePular = false;
    jogador.cor = RED;

    // Inicializando o cenário (plataformas e limites)
    ElementoCenario elementocenario[] = {
        {{ 0, 0, 1000, 400 }, 0, LIGHTGRAY }, // Fundo
        {{ 0, 400, 1000, 200 }, 1, GRAY },    // Chão principal
        {{ 300, 200, 400, 10 }, 1, GRAY },    // Plataforma flutuante
        {{ 250, 300, 100, 10 }, 1, GRAY },    // Plataforma esquerda
        {{ 650, 300, 100, 10 }, 1, GRAY }     // Plataforma direita
    };

    int elementocenarioTotal = sizeof(elementocenario)/sizeof(elementocenario[0]);

    // Configuração inicial da Câmera
    Camera2D camera = { 0 };
    camera.target = jogador.posicao; // Ponto que a câmera olha
    camera.offset = (Vector2){ LARGURA_TELA/2.0f, ALTURA_TELA/2.0f }; // Deslocamento para centralizar na tela
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // Vetor de ponteiros para funções, permitindo alternar facilmente entre os modos de câmera
    void (*cameraAtualizadores[])(Camera2D*, Jogador*, ElementoCenario*, int, float, int, int) = {
        AtualizarCameraCentro,
        AtualizarCameraCentroNoMapa,
        AtualizarCameraSeguirSuave,
        AtualizarCameraSuavizarNoPouso,
        AtualizarCameraEmpurrarNosLimites
    };

    int cameraOpcao = 0;
    int cameraAtualizadoresTotal = sizeof(cameraAtualizadores)/sizeof(cameraAtualizadores[0]);

    char *cameraDescricoes[] = {
        "Seguir o centro do jogador",
        "Seguir o centro do jogador, mas travar nas bordas",
        "Seguir o centro do jogador (suavizado)",
        "Seguir horizontalmente; atualizar verticalmente apos pouso",
        "Jogador empurra a camera ao chegar perto da borda"
    };

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Pega o tempo entre o frame atual e o anterior (usado para movimento independente da taxa de quadros)
        float deltaTime = GetFrameTime();

        // Atualiza a física e controles do jogador
        AtualizarJogador(&jogador, elementocenario, elementocenarioTotal, deltaTime);

        // Lida com o Zoom da câmera
        camera.zoom += ((float)GetMouseWheelMove()*0.05f);
        if (camera.zoom > 3.0f) camera.zoom = 3.0f;       // Zoom máximo
        else if (camera.zoom < 0.25f) camera.zoom = 0.25f; // Zoom mínimo

        // Reinicia a posição se apertar R
        if (IsKeyPressed(KEY_R))
        {
            camera.zoom = 1.0f;
            jogador.posicao = (Vector2){ 400, 280 };
            jogador.velocidade = 0;
        }

        // Alterna o modo de câmera se apertar C
        if (IsKeyPressed(KEY_C)) cameraOpcao = (cameraOpcao + 1) % cameraAtualizadoresTotal;

        // Chama a função de atualizar a câmera de acordo com a opção atual
        cameraAtualizadores[cameraOpcao](&camera, &jogador, elementocenario, elementocenarioTotal, deltaTime, LARGURA_TELA, ALTURA_TELA);

        BeginDrawing();
            ClearBackground(LIGHTGRAY);

            // Inicia o modo de câmera 2D (tudo desenhado aqui sofrerá zoom/panning)
            BeginMode2D(camera);

                // Desenha os blocos do cenário
                for (int i = 0; i < elementocenarioTotal; i++) 
                    DrawRectangleRec(elementocenario[i].retangulo, elementocenario[i].cor);

                // Desenha o jogador (Retângulo de colisão e um círculo no centro)
                Rectangle jogadorRetangulo = { jogador.posicao.x - 20, jogador.posicao.y - 40, 40.0f, 40.0f };
                DrawRectangleRec(jogadorRetangulo, jogador.cor); // Usando a nova cor do jogador
                DrawCircleV(jogador.posicao, 5.0f, GOLD); // Ponto centralizador

            EndMode2D();

            // Desenho da Interface (HUD) que fica fixa na tela
            DrawText("Controles:", 20, 20, 10, BLACK);
            DrawText("- Direita/Esquerda para mover", 40, 40, 10, DARKGRAY);
            DrawText("- Baixo para descer da plataforma", 40, 55, 10, DARKGRAY);
            DrawText("- Espaco para pular", 40, 70, 10, DARKGRAY);
            DrawText("- Rodinha do Mouse para Zoom", 40, 85, 10, DARKGRAY);
            DrawText("- R para resetar posicao + zoom", 40, 100, 10, DARKGRAY);
            DrawText("- C para mudar modo de camera", 40, 115, 10, DARKGRAY);
            
            DrawText("Atual modo de camera:", 20, 135, 10, BLACK);
            DrawText(cameraDescricoes[cameraOpcao], 40, 160, 10, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

// Função: AtualizarJogador
// Propósito: Controlar a movimentação, pulo, gravidade e colisão do jogador.
// Parâmetros:
// - jogador: Ponteiro para a estrutura do jogador (para alterar sua posição/estado).
// - elementocenario: Vetor contendo todas as plataformas/obstáculos.
// - elementocenarioTotal: Quantidade de elementos no cenário.
// - delta: Tempo passado desde o último frame.
void AtualizarJogador(Jogador *jogador, ElementoCenario *elementocenario, int elementocenarioTotal, float delta)
{
    // Movimento horizontal
    if (IsKeyDown(KEY_LEFT)) jogador->posicao.x -= VEL_HORIZ_JOGADOR * delta;
    if (IsKeyDown(KEY_RIGHT)) jogador->posicao.x += VEL_HORIZ_JOGADOR * delta;
    
    // Pulo (só ocorre se a variável podePular for verdadeira)
    if (IsKeyDown(KEY_SPACE) && jogador->podePular)
    {
        jogador->velocidade = -VEL_PULO_JOGADOR; // Velocidade negativa joga para cima
        jogador->podePular = false;
    }

    // Checagem de Colisão com o cenário
    bool bateuNoObstaculo = false;
    for (int i = 0; i < elementocenarioTotal; i++)
    {
        ElementoCenario *blocoAtual = elementocenario + i;
        Vector2 *posJogador = &(jogador->posicao);

        // Verifica se está segurando a seta para baixo
        bool querDescer = IsKeyDown(KEY_DOWN);
        
        // O chão principal está na posição Y 400. Se for maior ou igual a isso, é o chão inquebrável.
        bool ChaoPrincipal = (blocoAtual->retangulo.y >= 400);
        
        // Verifica se o bloco atual tem colisão ativa e se a posição do jogador intercala o bloco
        if (blocoAtual->temColisao &&
            (!querDescer || ChaoPrincipal) && // Se ele não quer descer ou se for o chão principal, a colisão acontece normal
            blocoAtual->retangulo.x <= posJogador->x &&
            blocoAtual->retangulo.x + blocoAtual->retangulo.width >= posJogador->x &&
            blocoAtual->retangulo.y >= posJogador->y &&
            blocoAtual->retangulo.y <= posJogador->y + jogador->velocidade * delta)
        {
            bateuNoObstaculo = true;
            jogador->velocidade = 0.0f; // Para de cair
            posJogador->y = blocoAtual->retangulo.y; // Fixa a posição do jogador exatamente em cima do bloco
            break;
        }
    }

    // Aplicação da gravidade
    if (!bateuNoObstaculo)
    {
        jogador->posicao.y += jogador->velocidade * delta; // Cai
        jogador->velocidade += GRAVIDADE * delta;          // Aumenta a velocidade de queda
        jogador->podePular = false;                        // Enquanto cai, não pode pular
    }
    else 
    {
        jogador->podePular = true; // Ao encostar no chão, habilita o pulo
    }

    // Barreira para não cair no buraco infinito (se passar do Y 600, ele fica preso no fundo)
    if (jogador->posicao.y > 600) 
    {
        jogador->posicao.y = 600;
        jogador->velocidade = 0.0f;
        jogador->podePular = true;
    }

    // Barreira para não deixar ele fugir pelas laterais do mapa
    if (jogador->posicao.x < 5) jogador->posicao.x = 5;
    if (jogador->posicao.x > 995) jogador->posicao.x = 995;
}

// Função: AtualizarCameraCentro
// Propósito: Câmera mais básica possível. Trava o alvo exatamente na posição do jogador.
// Parâmetros: (Todos recebem a câmera e contexto do mundo, mas aqui só usamos o jogador).
void AtualizarCameraCentro(Camera2D *camera, Jogador *jogador, ElementoCenario *elementocenario, int elementocenarioTotal, float delta, int largura, int altura)
{
    camera->offset = (Vector2){ largura/2.0f, altura/2.0f }; // Garante que a câmera centraliza na tela
    camera->target = jogador->posicao;
}

// Função: AtualizarCameraCentroNoMapa
// Propósito: Segue o jogador, mas impede que a câmera mostre coisas fora dos limites dos objetos do cenário.
void AtualizarCameraCentroNoMapa(Camera2D *camera, Jogador *jogador, ElementoCenario *elementocenario, int elementocenarioTotal, float delta, int largura, int altura)
{
    camera->target = jogador->posicao;
    camera->offset = (Vector2){ largura/2.0f, altura/2.0f };
    
    float minX = 1000, minY = 1000, maxX = -1000, maxY = -1000;

    // Acha os limites extremos de todos os blocos do cenário
    for (int i = 0; i < elementocenarioTotal; i++)
    {
        ElementoCenario *bloco = elementocenario + i;
        minX = fminf(bloco->retangulo.x, minX);
        maxX = fmaxf(bloco->retangulo.x + bloco->retangulo.width, maxX);
        minY = fminf(bloco->retangulo.y, minY);
        maxY = fmaxf(bloco->retangulo.y + bloco->retangulo.height, maxY);
    }

    // Converte os pontos extremos do mundo para coordenadas da tela
    Vector2 max = GetWorldToScreen2D((Vector2){ maxX, maxY }, *camera);
    Vector2 min = GetWorldToScreen2D((Vector2){ minX, minY }, *camera);

    // Corrige o offset da câmera se estiver revelando uma área fora dos blocos
    if (max.x < largura) camera->offset.x = largura - (max.x - (float)largura/2);
    if (max.y < altura) camera->offset.y = altura - (max.y - (float)altura/2);
    if (min.x > 0) camera->offset.x = (float)largura/2 - min.x;
    if (min.y > 0) camera->offset.y = (float)altura/2 - min.y;
}
// Função: AtualizarCameraSeguirSuave
// Propósito: A câmera "persegue" o jogador em vez de ficar grudada. 
void AtualizarCameraSeguirSuave(Camera2D *camera, Jogador *jogador, ElementoCenario *elementocenario, int elementocenarioTotal, float delta, int largura, int altura)
{
    static float velocidadeMinima = 30;
    static float distanciaMinimaEfeito = 10; // Câmera só age se o jogador se afastar mais que isso
    static float fracaoVelocidade = 0.8f;    // O quão rápido ela persegue (0 a 1)

    camera->offset = (Vector2){ largura/2.0f, altura/2.0f };
    
    // Vetor que representa a distância entre onde a câmera olha e onde o jogador está
    Vector2 direcao = Vector2Subtract(jogador->posicao, camera->target);
    float distancia = Vector2Length(direcao);

    if (distancia > distanciaMinimaEfeito)
    {
        // A velocidade baseia-se na distância, para que persiga mais rápido se estiver mais longe
        float velocidadeCamera = fmaxf(fracaoVelocidade * distancia, velocidadeMinima);
        
        // Move o alvo da câmera gradualmente na direção do jogador
        camera->target = Vector2Add(camera->target, Vector2Scale(direcao, velocidadeCamera * delta / distancia));
    }
}

// Função: AtualizarCameraSuavizarNoPouso
// Propósito: Segue rigidamente no eixo X, mas só ajusta a câmera no eixo Y quando o jogador pousa no chão. Impede a câmera de ficar tremendo ou pulando junto com o personagem a cada pulinho.
void AtualizarCameraSuavizarNoPouso(Camera2D *camera, Jogador *jogador, ElementoCenario *elementocenario, int elementocenarioTotal, float delta, int largura, int altura)
{
    static float velocidadeSuavizacao = 700;
    static int suavizando = false;
    static float alvoSuavizacao;

    camera->offset = (Vector2){ largura/2.0f, altura/2.0f };
    camera->target.x = jogador->posicao.x; // Eixo X sempre colado no jogador

    if (suavizando)
    {
        // Movimento de interpolação no Eixo Y
        if (alvoSuavizacao > camera->target.y)
        {
            camera->target.y += velocidadeSuavizacao * delta;
            if (camera->target.y > alvoSuavizacao)
            {
                camera->target.y = alvoSuavizacao;
                suavizando = false; // Fim da suavização
            }
        }
        else
        {
            camera->target.y -= velocidadeSuavizacao * delta;
            if (camera->target.y < alvoSuavizacao)
            {
                camera->target.y = alvoSuavizacao;
                suavizando = false;
            }
        }
    }
    else
    {
        // Ativa a suavização Y somente se o jogador tiver acabado de pousar 
        // (podePular é true, parou de cair/velocidade 0) e a câmera estiver desalinhada no Y
        if (jogador->podePular && (jogador->velocidade == 0) && (jogador->posicao.y != camera->target.y))
        {
            suavizando = true;
            alvoSuavizacao = jogador->posicao.y;
        }
    }
}

// Função: AtualizarCameraEmpurrarNosLimites
// Propósito: Cria uma "caixa" imaginária no centro da tela. 
// A câmera só se move se o jogador esbarrar nas paredes dessa caixa invisível, "empurrando" a câmera.
void AtualizarCameraEmpurrarNosLimites(Camera2D *camera, Jogador *jogador, ElementoCenario *elementocenario, int elementocenarioTotal, float delta, int largura, int altura)
{
    // Tamanho da caixa (0.2 significa 20% da tela)
    static Vector2 caixaDelimitadora = { 0.2f, 0.2f };

    // Calcula os limites dessa caixa em coordenadas do mundo do jogo
    Vector2 limiteMinimoMundo = GetScreenToWorld2D((Vector2){ (1 - caixaDelimitadora.x)*0.5f*largura, (1 - caixaDelimitadora.y)*0.5f*altura }, *camera);
    Vector2 limiteMaximoMundo = GetScreenToWorld2D((Vector2){ (1 + caixaDelimitadora.x)*0.5f*largura, (1 + caixaDelimitadora.y)*0.5f*altura }, *camera);
    
    // Centraliza o offset da câmera em cima dessa caixa
    camera->offset = (Vector2){ (1 - caixaDelimitadora.x)*0.5f*largura, (1 - caixaDelimitadora.y)*0.5f*altura };

    // Se o jogador passar do limite, o alvo da câmera é atualizado para acompanhar (sendo "empurrado")
    if (jogador->posicao.x < limiteMinimoMundo.x) camera->target.x = jogador->posicao.x;
    if (jogador->posicao.y < limiteMinimoMundo.y) camera->target.y = jogador->posicao.y;
    
    if (jogador->posicao.x > limiteMaximoMundo.x) camera->target.x = limiteMinimoMundo.x + (jogador->posicao.x - limiteMaximoMundo.x);
    if (jogador->posicao.y > limiteMaximoMundo.y) camera->target.y = limiteMinimoMundo.y + (jogador->posicao.y - limiteMaximoMundo.y);
}