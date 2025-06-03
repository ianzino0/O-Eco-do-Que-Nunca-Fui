#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define TAMANHO 10
#define JOGADOR '&'
#define NPC 'P'
#define PAREDE '*'
#define CHAVE '@'
#define PORTA_FECHADA 'D'
#define PORTA_ABERTA '='
#define ESPACO_VAZIO ' '
#define NEVE '.'
#define GELO '~'

// Estrutura do jogador
typedef struct {
    int x, y;
    int tem_chave;
    int temperatura;
    int tocha;
} Jogador;

// Função para limpar a tela
void limpar_tela() {
    system("cls");
}

// Função para definir cor do texto
void set_cor(int cor) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cor);
}

// Inicializar o mapa da Fase 1 - "Caverna Gelada"
void inicializar_mapa(char mapa[TAMANHO][TAMANHO]) {
    char caverna_neve[TAMANHO][TAMANHO] = {
        {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
        {'*', '&', '.', '*', '~', '.', '.', '*', '.', '*'},
        {'*', '.', '*', '*', '.', '*', '~', '*', '.', '*'},
        {'*', '~', '.', '.', '.', '*', '.', '~', '.', '*'},
        {'*', '*', '*', '.', '*', '*', '*', '.', '*', '*'},
        {'*', '.', '~', '.', '.', '.', '*', '.', '@', '*'},
        {'*', '.', '*', '*', '*', '~', '*', '*', '.', '*'},
        {'*', '~', '.', '.', '*', '.', '.', '.', '.', '*'},
        {'*', 'P', '*', '.', '~', '.', '*', '.', 'D', '*'},
        {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*'}
    };
    
    for(int i = 0; i < TAMANHO; i++) {
        for(int j = 0; j < TAMANHO; j++) {
            mapa[i][j] = caverna_neve[i][j];
        }
    }
}

// Exibir o mapa com atmosfera sombria
void exibir_mapa(char mapa[TAMANHO][TAMANHO], Jogador *jogador) {
    limpar_tela();
    
    // Efeito de vento gelado
    set_cor(8); // Cinza escuro
    printf("   *** Vento gelado ecoa pela caverna... ***\n");
    
    // Título do jogo
    set_cor(9); // Azul
    printf("\n  +===================================+\n");
    printf("  |      CAVERNA GELADA - FASE 1      |\n");
    printf("  |        Escape da Escuridao        |\n");
    printf("  +===================================+\n\n");
    
    // Status do jogador com temperatura
    set_cor(14); // Amarelo
    printf("  TOCHA: ");
    if(jogador->tocha > 0) {
        set_cor(12); // Vermelho (fogo)
        printf("ACESA (%d turnos restantes)", jogador->tocha);
    } else {
        set_cor(8); // Cinza
        printf("APAGADA - Escuridao total!");
    }
    
    printf("\n");
    set_cor(11); // Ciano
    printf("  TEMPERATURA: ");
    if(jogador->temperatura > 70) {
        set_cor(10); // Verde
        printf("Aquecido (%dC)", jogador->temperatura);
    } else if(jogador->temperatura > 40) {
        set_cor(14); // Amarelo
        printf("Frio (%dC)", jogador->temperatura);
    } else {
        set_cor(12); // Vermelho
        printf("MUITO FRIO! (%dC)", jogador->temperatura);
    }
    
    printf("\n");
    set_cor(13); // Magenta
    printf("  STATUS: ");
    if(jogador->tem_chave) {
        set_cor(10); // Verde
        printf("CHAVE ANTIGA ENCONTRADA! Procure a saida!");
    } else {
        set_cor(12); // Vermelho
        printf("Procure a chave mistica (@) nas profundezas!");
    }
    
    printf("\n\n  ");
    
    // Exibir o mapa com visibilidade limitada
    for(int i = 0; i < TAMANHO; i++) {
        for(int j = 0; j < TAMANHO; j++) {
            // Calcular distância do jogador para simular tocha
            int dist = abs(i - jogador->x) + abs(j - jogador->y);
            int visivel = (jogador->tocha > 0 && dist <= 2) || (dist == 0);
            
            if(!visivel && mapa[i][j] != JOGADOR) {
                set_cor(0); // Preto (escuridão)
                printf("# ");
                continue;
            }
            
            // Definir cores para cada elemento
            switch(mapa[i][j]) {
                case JOGADOR:
                    set_cor(15); // Branco brilhante
                    break;
                case PAREDE:
                    set_cor(8); // Cinza escuro (rocha)
                    break;
                case NEVE:
                    set_cor(15); // Branco (neve)
                    break;
                case GELO:
                    set_cor(11); // Ciano (gelo)
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
                case NPC:
                    set_cor(5); // Roxo (criaturas das cavernas)
                    break;
                default:
                    set_cor(7); // Branco normal
            }
            printf("%c ", mapa[i][j]);
        }
        printf("\n  ");
    }
    
    // Avisos de perigo
    if(jogador->temperatura < 30) {
        set_cor(12); // Vermelho
        printf("\n  >>> ATENCAO: Voce esta com muito frio! Use F para acender tocha!\n");
    }
    if(jogador->tocha <= 3 && jogador->tocha > 0) {
        set_cor(14); // Amarelo
        printf("  >>> Sua tocha esta quase apagando!\n");
    }
    
    // Instruções
    set_cor(7); // Branco normal
    printf("\n  +===================================+\n");
    printf("  | CONTROLES:                        |\n");
    printf("  | W/A/S/D - Explorar caverna        |\n");
    printf("  | F - Acender tocha (se possivel)   |\n");
    printf("  | Q - Desistir da exploracao        |\n");
    printf("  |                                   |\n");
    printf("  | ELEMENTOS DA CAVERNA:             |\n");
    set_cor(15); printf("  | & "); set_cor(7); printf("- Voce (Explorador)           |\n");
    set_cor(8);  printf("  | * "); set_cor(7); printf("- Paredes de Rocha            |\n");
    set_cor(15); printf("  | . "); set_cor(7); printf("- Neve Acumulada              |\n");
    set_cor(11); printf("  | ~ "); set_cor(7); printf("- Gelo Escorregadio           |\n");
    set_cor(14); printf("  | @ "); set_cor(7); printf("- Chave Mistica Dourada       |\n");
    set_cor(4);  printf("  | D "); set_cor(7); printf("- Portal Selado               |\n");
    set_cor(10); printf("  | = "); set_cor(7); printf("- Portal Aberto               |\n");
    printf("  +===================================+\n");
}

// Verificar se o movimento é válido
int movimento_valido(char mapa[TAMANHO][TAMANHO], int x, int y) {
    if(x < 0 || x >= TAMANHO || y < 0 || y >= TAMANHO) {
        return 0; // Fora dos limites
    }
    if(mapa[x][y] == PAREDE) {
        return 0; // Parede
    }
    return 1; // Movimento válido
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
        case 'f': case 'F':
            if(jogador->tocha <= 0) {
                jogador->tocha = 15; // Acender tocha
                jogador->temperatura += 20; // Aquecer
                printf("\n  >>> Tocha acesa! Voce se sente mais aquecido.\n");
                Sleep(1000);
            }
            return;
        default:
            return;
    }
    
    if(movimento_valido(mapa, novo_x, novo_y)) {
        // Efeitos do terreno
        if(mapa[novo_x][novo_y] == NEVE) {
            jogador->temperatura -= 5; // Neve esfria
        } else if(mapa[novo_x][novo_y] == GELO) {
            jogador->temperatura -= 8; // Gelo esfria mais
            // Chance de escorregar (movimento extra)
            srand(time(NULL));
            if(rand() % 3 == 0) {
                printf("\n  *** Voce escorregou no gelo! ***\n");
                Sleep(1000);
            }
        }
        
        // Verificar se coletou a chave
        if(mapa[novo_x][novo_y] == CHAVE) {
            jogador->tem_chave = 1;
            jogador->temperatura += 15; // Calor mágico da chave
            
            // Abrir a porta
            for(int i = 0; i < TAMANHO; i++) {
                for(int j = 0; j < TAMANHO; j++) {
                    if(mapa[i][j] == PORTA_FECHADA) {
                        mapa[i][j] = PORTA_ABERTA;
                    }
                }
            }
            printf("\n  *** CHAVE COLETADA! Portal se abre! ***\n");
            Sleep(1500);
        }
        
        // Verificar se chegou na porta aberta
        if(mapa[novo_x][novo_y] == PORTA_ABERTA) {
            mapa[jogador->x][jogador->y] = ESPACO_VAZIO;
            mapa[novo_x][novo_y] = JOGADOR;
            jogador->x = novo_x;
            jogador->y = novo_y;
            
            // Vitória!
            limpar_tela();
            set_cor(10); // Verde
            printf("\n\n  +===================================+\n");
            printf("  |                                   |\n");
            printf("  |         *** INCRIVEL! ***         |\n");
            printf("  |                                   |\n");
            printf("  |   VOCE ESCAPOU DA CAVERNA GELADA! |\n");
            printf("  |                                   |\n");
            printf("  |     A luz do sol te aquece...     |\n");
            printf("  |      Fase 1 Conquistada!          |\n");
            printf("  |                                   |\n");
            printf("  +===================================+\n");
            
            set_cor(14);
            printf("\n  *** Voce emerge da caverna, sentindo o calor do sol\n");
            printf("      apos horas na escuridao gelada...\n");
            
            set_cor(7);
            printf("\n  Pressione qualquer tecla para continuar sua aventura...");
            getch();
            exit(0);
        }
        
        // Limitar temperatura mínima (sem morte)
        if(jogador->temperatura < 10) {
            jogador->temperatura = 10; // Temperatura mínima
        }
        
        // Mover jogador
        mapa[jogador->x][jogador->y] = ESPACO_VAZIO;
        mapa[novo_x][novo_y] = JOGADOR;
        jogador->x = novo_x;
        jogador->y = novo_y;
        
        // Reduzir tocha e temperatura gradualmente
        if(jogador->tocha > 0) jogador->tocha--;
        jogador->temperatura -= 2; // Frio constante
        
        if(jogador->temperatura > 100) jogador->temperatura = 100;
        if(jogador->temperatura < 10) jogador->temperatura = 10; // Mínimo sem morte
    }
}

// Encontrar posição inicial do jogador
void encontrar_jogador(char mapa[TAMANHO][TAMANHO], Jogador *jogador) {
    for(int i = 0; i < TAMANHO; i++) {
        for(int j = 0; j < TAMANHO; j++) {
            if(mapa[i][j] == JOGADOR) {
                jogador->x = i;
                jogador->y = j;
                jogador->tem_chave = 0;
                jogador->temperatura = 60; // Começa com frio
                jogador->tocha = 10; // Tocha inicial
                return;
            }
        }
    }
}

int main() {
    char mapa[TAMANHO][TAMANHO];
    Jogador jogador;
    char comando;
    
    // Configurar console
    SetConsoleTitle("Caverna Gelada - Escape da Escuridao");
    
    // Inicializar jogo
    inicializar_mapa(mapa);
    encontrar_jogador(mapa, &jogador);
    
    // Tela de início atmosférica
    limpar_tela();
    set_cor(9);
    printf("\n\n  +===================================+\n");
    printf("  |                                   |\n");
    printf("  |       *** CAVERNA GELADA ***      |\n");
    printf("  |                                   |\n");
    printf("  |         FASE 1: ESCURIDAO         |\n");
    printf("  |                                   |\n");
    set_cor(7);
    printf("  |  Voce caiu em uma caverna gelada  |\n");
    printf("  |  e escura. O frio penetra seus    |\n");
    printf("  |  ossos enquanto criaturas ecoam   |\n");
    printf("  |  nas sombras...                   |\n");
    printf("  |                                   |\n");
    printf("  |  Encontre a chave mistica antes   |\n");
    printf("  |  que o frio te mate!              |\n");
    printf("  |                                   |\n");
    set_cor(9);
    printf("  +===================================+\n");
    
    set_cor(14);
    printf("\n  *** O vento uiva... Sua tocha tremula...\n");
    set_cor(7);
    printf("\n  Pressione qualquer tecla para comecar a exploracao...");
    getch();
    
    // Loop principal do jogo
    while(1) {
        exibir_mapa(mapa, &jogador);
        
        comando = getch();
        
        if(comando == 'q' || comando == 'Q') {
            set_cor(8);
            printf("\n  *** Voce desiste da exploracao e desaparece na escuridao...\n");
            set_cor(7);
            break;
        }
        
        mover_jogador(mapa, &jogador, comando);
    }
    
    return 0;
}