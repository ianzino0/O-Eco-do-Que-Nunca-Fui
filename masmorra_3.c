#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define TAMANHO 40
#define JOGADOR '&'
#define PAREDE '*'
#define CHAVE '@'
#define PORTA_FECHADA 'D'
#define PORTA_ABERTA '='
#define ESPACO_VAZIO ' '
#define BOTAO 'O'
#define ESPINHO '#'
#define TELETRANSPORTE '>'
#define MONSTRO_1 'X'
#define MONSTRO_2 'V'
#define ARVORE 'T'

// Estrutura do jogador
typedef struct {
    int x, y;
    int tem_chave;
    int vidas;
} Jogador;

// Estrutura dos monstros
typedef struct {
    int x, y;
    int ativo;
    int tipo; // 1 = aleatório, 2 = inteligente
} Monstro;

// Estrutura do teletransporte
typedef struct {
    int x1, y1; // Primeiro teletransporte
    int x2, y2; // Segundo teletransporte
} Teletransporte;

// Variáveis globais
Monstro monstros[8]; // 5 nível 1 + 3 nível 2
Teletransporte teletransporte;
int espinhos_ativos[10][2];
int armadilhas_ativas = 0;
int reinicializacoes = 0;

// Função para limpar a tela
void limpar_tela() {
    system("cls");
}

// Função para definir cor do texto
void set_cor(int cor) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cor);
}

// Inicializar o mapa da Fase 3 - "Floresta Densa"
void inicializar_mapa(char mapa[TAMANHO][TAMANHO]) {
    // Limpar mapa
    for(int i = 0; i < TAMANHO; i++) {
        for(int j = 0; j < TAMANHO; j++) {
            mapa[i][j] = ESPACO_VAZIO;
        }
    }
    
    // Paredes externas
    for(int i = 0; i < TAMANHO; i++) {
        mapa[0][i] = PAREDE;
        mapa[TAMANHO-1][i] = PAREDE;
        mapa[i][0] = PAREDE;
        mapa[i][TAMANHO-1] = PAREDE;
    }
    
    // Árvores espalhadas (representando floresta densa)
    srand(time(NULL));
    for(int i = 0; i < 150; i++) {
        int x = rand() % (TAMANHO-2) + 1;
        int y = rand() % (TAMANHO-2) + 1;
        if(mapa[x][y] == ESPACO_VAZIO) {
            mapa[x][y] = ARVORE;
        }
    }
    
    // Jogador na posição inicial
    mapa[1][1] = JOGADOR;
    
    // Chave em posição aleatória (longe do jogador)
    int chave_colocada = 0;
    while(!chave_colocada) {
        int x = rand() % (TAMANHO-10) + 10;
        int y = rand() % (TAMANHO-10) + 10;
        if(mapa[x][y] == ESPACO_VAZIO) {
            mapa[x][y] = CHAVE;
            chave_colocada = 1;
        }
    }
    
    // Porta na posição final
    mapa[TAMANHO-2][TAMANHO-2] = PORTA_FECHADA;
    
    // Colocar teletransportes
    int tele_colocados = 0;
    while(tele_colocados < 2) {
        int x = rand() % (TAMANHO-2) + 1;
        int y = rand() % (TAMANHO-2) + 1;
        if(mapa[x][y] == ESPACO_VAZIO) {
            mapa[x][y] = TELETRANSPORTE;
            if(tele_colocados == 0) {
                teletransporte.x1 = x;
                teletransporte.y1 = y;
            } else {
                teletransporte.x2 = x;
                teletransporte.y2 = y;
            }
            tele_colocados++;
        }
    }
    
    // Colocar botões (armadilhas)
    for(int i = 0; i < 2; i++) {
        int botao_colocado = 0;
        while(!botao_colocado) {
            int x = rand() % (TAMANHO-2) + 1;
            int y = rand() % (TAMANHO-2) + 1;
            if(mapa[x][y] == ESPACO_VAZIO) {
                mapa[x][y] = BOTAO;
                botao_colocado = 1;
            }
        }
    }
    
    // Colocar espinhos fixos
    for(int i = 0; i < 10; i++) {
        int espinho_colocado = 0;
        while(!espinho_colocado) {
            int x = rand() % (TAMANHO-2) + 1;
            int y = rand() % (TAMANHO-2) + 1;
            if(mapa[x][y] == ESPACO_VAZIO) {
                mapa[x][y] = ESPINHO;
                espinhos_ativos[i][0] = x;
                espinhos_ativos[i][1] = y;
                espinho_colocado = 1;
            }
        }
    }
    
    // Inicializar monstros nível 1 (5 monstros)
    for(int i = 0; i < 5; i++) {
        int monstro_colocado = 0;
        while(!monstro_colocado) {
            int x = rand() % (TAMANHO-2) + 1;
            int y = rand() % (TAMANHO-2) + 1;
            if(mapa[x][y] == ESPACO_VAZIO) {
                mapa[x][y] = MONSTRO_1;
                monstros[i].x = x;
                monstros[i].y = y;
                monstros[i].ativo = 1;
                monstros[i].tipo = 1;
                monstro_colocado = 1;
            }
        }
    }
    
    // Inicializar monstros nível 2 (3 monstros)
    for(int i = 5; i < 8; i++) {
        int monstro_colocado = 0;
        while(!monstro_colocado) {
            int x = rand() % (TAMANHO-2) + 1;
            int y = rand() % (TAMANHO-2) + 1;
            if(mapa[x][y] == ESPACO_VAZIO) {
                mapa[x][y] = MONSTRO_2;
                monstros[i].x = x;
                monstros[i].y = y;
                monstros[i].ativo = 1;
                monstros[i].tipo = 2;
                monstro_colocado = 1;
            }
        }
    }
}

// Exibir o mapa
void exibir_mapa(char mapa[TAMANHO][TAMANHO], Jogador *jogador) {
    limpar_tela();
    
    // Título do jogo
    set_cor(10); // Verde
    printf("\n  +=============================================+\n");
    printf("  |          FLORESTA DENSA - FASE 3            |\n");
    printf("  |            Sobreviva aos Perigos            |\n");
    printf("  +=============================================+\n\n");
    
    // Status do jogador
    set_cor(14); // Amarelo
    printf("  VIDAS RESTANTES: %d/3\n", jogador->vidas);
    set_cor(13); // Magenta
    printf("  STATUS: ");
    if(jogador->tem_chave) {
        set_cor(10); // Verde
        printf("CHAVE ENCONTRADA! Encontre a saida!");
    } else {
        set_cor(12); // Vermelho
        printf("Procure a chave magica (@) na floresta!");
    }
    
    printf("\n  REINICIALIZACOES: %d/3\n\n", reinicializacoes);
    
    // Exibir apenas uma parte do mapa (visão limitada 20x20 centrada no jogador)
    int start_x = jogador->x - 10;
    int start_y = jogador->y - 10;
    if(start_x < 0) start_x = 0;
    if(start_y < 0) start_y = 0;
    if(start_x > TAMANHO - 20) start_x = TAMANHO - 20;
    if(start_y > TAMANHO - 20) start_y = TAMANHO - 20;
    
    printf("  ");
    for(int i = start_x; i < start_x + 20 && i < TAMANHO; i++) {
        for(int j = start_y; j < start_y + 20 && j < TAMANHO; j++) {
            // Definir cores para cada elemento
            switch(mapa[i][j]) {
                case JOGADOR:
                    set_cor(15); // Branco brilhante
                    break;
                case PAREDE:
                    set_cor(8); // Cinza escuro
                    break;
                case ARVORE:
                    set_cor(2); // Verde escuro
                    break;
                case CHAVE:
                    set_cor(14); // Amarelo dourado
                    break;
                case PORTA_FECHADA:
                    set_cor(4); // Vermelho escuro
                    break;
                case PORTA_ABERTA:
                    set_cor(10); // Verde
                    break;
                case BOTAO:
                    set_cor(6); // Amarelo escuro
                    break;
                case ESPINHO:
                    set_cor(12); // Vermelho brilhante
                    break;
                case TELETRANSPORTE:
                    set_cor(13); // Magenta
                    break;
                case MONSTRO_1:
                    set_cor(9); // Azul
                    break;
                case MONSTRO_2:
                    set_cor(5); // Roxo
                    break;
                default:
                    set_cor(7); // Branco normal
            }
            printf("%c", mapa[i][j]);
        }
        printf("\n  ");
    }
    
    // Instruções
    set_cor(7); // Branco normal
    printf("\n  +=============================================+\n");
    printf("  | CONTROLES:                                  |\n");
    printf("  | W/A/S/D - Mover pela floresta              |\n");
    printf("  | Q - Desistir                               |\n");
    printf("  |                                             |\n");
    printf("  | ELEMENTOS DA FLORESTA:                      |\n");
    set_cor(15); printf("  | & "); set_cor(7); printf("- Voce (Explorador)                    |\n");
    set_cor(8);  printf("  | * "); set_cor(7); printf("- Paredes de Pedra                     |\n");
    set_cor(2);  printf("  | T "); set_cor(7); printf("- Arvores Densas                       |\n");
    set_cor(14); printf("  | @ "); set_cor(7); printf("- Chave Magica                         |\n");
    set_cor(4);  printf("  | D "); set_cor(7); printf("- Portal Selado                        |\n");
    set_cor(6);  printf("  | O "); set_cor(7); printf("- Botao Suspeito (ARMADILHA!)          |\n");
    set_cor(12); printf("  | # "); set_cor(7); printf("- Espinhos Mortais                     |\n");
    set_cor(13); printf("  | > "); set_cor(7); printf("- Teletransporte                       |\n");
    set_cor(9);  printf("  | X "); set_cor(7); printf("- Monstro Nivel 1 (Aleatorio)          |\n");
    set_cor(5);  printf("  | V "); set_cor(7); printf("- Monstro Nivel 2 (Inteligente)        |\n");
    printf("  +=============================================+\n");
}

// Verificar se o movimento é válido
int movimento_valido(char mapa[TAMANHO][TAMANHO], int x, int y) {
    if(x < 0 || x >= TAMANHO || y < 0 || y >= TAMANHO) {
        return 0;
    }
    if(mapa[x][y] == PAREDE || mapa[x][y] == ARVORE) {
        return 0;
    }
    return 1;
}

// Ativar armadilha de espinhos
void ativar_armadilha(char mapa[TAMANHO][TAMANHO], int botao_x, int botao_y) {
    // Colocar espinhos em volta do botão
    for(int dx = -2; dx <= 2; dx++) {
        for(int dy = -2; dy <= 2; dy++) {
            int nx = botao_x + dx;
            int ny = botao_y + dy;
            if(nx >= 0 && nx < TAMANHO && ny >= 0 && ny < TAMANHO) {
                if(mapa[nx][ny] == ESPACO_VAZIO) {
                    mapa[nx][ny] = ESPINHO;
                }
            }
        }
    }
    armadilhas_ativas = 1;
}

// Reinicializar fase
void reinicializar_fase(char mapa[TAMANHO][TAMANHO], Jogador *jogador) {
    reinicializacoes++;
    
    if(reinicializacoes >= 3) {
        limpar_tela();
        set_cor(12);
        printf("\n\n  +=============================================+\n");
        printf("  |                                             |\n");
        printf("  |          *** GAME OVER ***                 |\n");
        printf("  |                                             |\n");
        printf("  |    Voce falhou 3 vezes na floresta!        |\n");
        printf("  |       Voltando ao menu principal...        |\n");
        printf("  |                                             |\n");
        printf("  +=============================================+\n");
        set_cor(7);
        printf("\n  Pressione qualquer tecla para continuar...");
        getch();
        exit(0);
    }
    
    // Reinicializar posições
    inicializar_mapa(mapa);
    jogador->x = 1;
    jogador->y = 1;
    jogador->tem_chave = 0;
    armadilhas_ativas = 0;
    
    set_cor(14);
    printf("\n  *** FASE REINICIADA! Tentativa %d/3 ***\n", reinicializacoes + 1);
    Sleep(2000);
}

// Mover monstros
void mover_monstros(char mapa[TAMANHO][TAMANHO], Jogador *jogador) {
    for(int i = 0; i < 8; i++) {
        if(!monstros[i].ativo) continue;
        
        int old_x = monstros[i].x;
        int old_y = monstros[i].y;
        
        if(monstros[i].tipo == 1) {
            // Movimento aleatório
            int direcao = rand() % 4;
            int nx = old_x, ny = old_y;
            
            switch(direcao) {
                case 0: nx--; break; // Cima
                case 1: nx++; break; // Baixo
                case 2: ny--; break; // Esquerda
                case 3: ny++; break; // Direita
            }
            
            if(movimento_valido(mapa, nx, ny) && mapa[nx][ny] != JOGADOR) {
                mapa[old_x][old_y] = ESPACO_VAZIO;
                monstros[i].x = nx;
                monstros[i].y = ny;
                mapa[nx][ny] = MONSTRO_1;
            }
        } else {
            // Movimento inteligente (segue o jogador)
            int dx = 0, dy = 0;
            
            if(jogador->x > old_x) dx = 1;
            else if(jogador->x < old_x) dx = -1;
            
            if(jogador->y > old_y) dy = 1;
            else if(jogador->y < old_y) dy = -1;
            
            int nx = old_x + dx;
            int ny = old_y + dy;
            
            if(movimento_valido(mapa, nx, ny) && mapa[nx][ny] != JOGADOR) {
                mapa[old_x][old_y] = ESPACO_VAZIO;
                monstros[i].x = nx;
                monstros[i].y = ny;
                mapa[nx][ny] = MONSTRO_2;
            }
        }
    }
}

// Mover jogador
void mover_jogador(char mapa[TAMANHO][TAMANHO], Jogador *jogador, char direcao) {
    int novo_x = jogador->x;
    int novo_y = jogador->y;
    
    switch(direcao) {
        case 'w': case 'W':
            novo_x--;
            break;
        case 's': case 'S':
            novo_x++;
            break;
        case 'a': case 'A':
            novo_y--;
            break;
        case 'd': case 'D':
            novo_y++;
            break;
        default:
            return;
    }
    
    if(movimento_valido(mapa, novo_x, novo_y)) {
        char destino = mapa[novo_x][novo_y];
        
        // Verificar colisões mortais
        if(destino == ESPINHO || destino == MONSTRO_1 || destino == MONSTRO_2) {
            printf("\n  *** VOCE MORREU! ***\n");
            Sleep(1500);
            reinicializar_fase(mapa, jogador);
            return;
        }
        
        // Verificar chave
        if(destino == CHAVE) {
            jogador->tem_chave = 1;
            // Abrir porta
            for(int i = 0; i < TAMANHO; i++) {
                for(int j = 0; j < TAMANHO; j++) {
                    if(mapa[i][j] == PORTA_FECHADA) {
                        mapa[i][j] = PORTA_ABERTA;
                    }
                }
            }
            printf("\n  *** CHAVE ENCONTRADA! Portal se abre! ***\n");
            Sleep(1000);
        }
        
        // Verificar porta aberta (vitória)
        if(destino == PORTA_ABERTA) {
            limpar_tela();
            set_cor(10);
            printf("\n\n  +=============================================+\n");
            printf("  |                                             |\n");
            printf("  |           *** PARABENS! ***                 |\n");
            printf("  |                                             |\n");
            printf("  |    VOCE ESCAPOU DA FLORESTA DENSA!          |\n");
            printf("  |                                             |\n");
            printf("  |        Fase 3 Conquistada!                  |\n");
            printf("  |                                             |\n");
            printf("  +=============================================+\n");
            set_cor(7);
            printf("\n  Pressione qualquer tecla para continuar...");
            getch();
            exit(0);
        }
        
        // Verificar botão (armadilha)
        if(destino == BOTAO) {
            ativar_armadilha(mapa, novo_x, novo_y);
            printf("\n  *** ARMADILHA ATIVADA! Espinhos aparecem! ***\n");
            Sleep(1000);
        }
        
        // Verificar teletransporte
        if(destino == TELETRANSPORTE) {
            if(novo_x == teletransporte.x1 && novo_y == teletransporte.y1) {
                novo_x = teletransporte.x2;
                novo_y = teletransporte.y2;
            } else {
                novo_x = teletransporte.x1;
                novo_y = teletransporte.y1;
            }
            printf("\n  *** TELETRANSPORTADO! ***\n");
            Sleep(1000);
        }
        
        // Mover jogador
        mapa[jogador->x][jogador->y] = ESPACO_VAZIO;
        mapa[novo_x][novo_y] = JOGADOR;
        jogador->x = novo_x;
        jogador->y = novo_y;
        
        // Mover monstros após movimento do jogador
        mover_monstros(mapa, jogador);
        
        // Verificar se jogador colidiu com monstro após movimento dos monstros
        for(int i = 0; i < 8; i++) {
            if(monstros[i].ativo && monstros[i].x == jogador->x && monstros[i].y == jogador->y) {
                printf("\n  *** MONSTRO TE PEGOU! ***\n");
                Sleep(1500);
                reinicializar_fase(mapa, jogador);
                return;
            }
        }
    }
}

// Encontrar posição inicial do jogador
void encontrar_jogador(char mapa[TAMANHO][TAMANHO], Jogador *jogador) {
    jogador->x = 1;
    jogador->y = 1;
    jogador->tem_chave = 0;
    jogador->vidas = 3;
}

int main() {
    char mapa[TAMANHO][TAMANHO];
    Jogador jogador;
    char comando;
    
    // Configurar console
    SetConsoleTitle("Floresta Densa - Fase 3");
    
    // Inicializar variáveis globais
    reinicializacoes = 0;
    armadilhas_ativas = 0;
    
    // Inicializar jogo
    srand(time(NULL));
    inicializar_mapa(mapa);
    encontrar_jogador(mapa, &jogador);
    
    // Tela de início
    limpar_tela();
    set_cor(10);
    printf("\n\n  +=============================================+\n");
    printf("  |                                             |\n");
    printf("  |        *** FLORESTA DENSA ***               |\n");
    printf("  |                                             |\n");
    printf("  |           FASE 3: PERIGOS                   |\n");
    printf("  |                                             |\n");
    set_cor(7);
    printf("  |  apos sair do jardim voce se depara         |\n");
    printf("  |  com uma floresta densa e perigosa          |\n");
    printf("  |  Monstros vagam entre os arbustos e         |\n");
    printf("  |  arvores, espinhos mortais estao            |\n");
    printf("  |  espalhados e armadilhas aguardam...        |\n");
    printf("  |                                             |\n");
    printf("  |  Encontre a chave e escape vivo!            |\n");
    printf("  |                                             |\n");
    set_cor(10);
    printf("  +=============================================+\n");
    
    set_cor(14);
    printf("\n  *** Sons sinistros ecoam pela floresta...\n");
    set_cor(7);
    printf("\n  Pressione qualquer tecla para comecar...");
    getch();
    
    // Loop principal do jogo
    while(1) {
        exibir_mapa(mapa, &jogador);
        
        comando = getch();
        
        if(comando == 'q' || comando == 'Q') {
            set_cor(8);
            printf("\n  *** Voce desiste e foge da floresta...\n");
            set_cor(7);
            break;
        }
        
        mover_jogador(mapa, &jogador, comando);
    }
    
    return 0;
}