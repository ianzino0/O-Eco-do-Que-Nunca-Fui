#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define TAMANHO 20
#define JOGADOR '&'
#define NPC 'P'
#define PAREDE '*'
#define CHAVE '@'
#define PORTA_FECHADA 'D'
#define PORTA_ABERTA '='
#define ESPACO_VAZIO ' '
#define GRAMA '.'
#define FLOR 'o'
#define BOTAO 'O'
#define ESPINHO '#'
#define MONSTRO 'X'

// Estrutura do jogador
typedef struct {
    int x, y;
    int tem_chave;
    int vida;
    int energia;
    int pontos;
} Jogador;

// Estrutura do monstro
typedef struct {
    int x, y;
    int direcao; // 0=norte, 1=leste, 2=sul, 3=oeste
    int ativo;
} Monstro;

// Função para limpar a tela
void limpar_tela() {
    system("cls");
}

// Função para definir cor do texto
void set_cor(int cor) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cor);
}

// Inicializar o mapa da Fase 2 - "Campo Ensolarado"
void inicializar_mapa(char mapa[TAMANHO][TAMANHO]) {
    char campo_ensolarado[TAMANHO][TAMANHO] = {
        {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
        {'*', '&', '.', 'o', '.', '*', '.', '#', '.', 'o', '.', '*', '.', '.', '.', 'o', '.', '*', '.', '*'},
        {'*', '.', '*', '*', '.', '*', '*', '.', '*', '*', '.', '*', '*', '.', '*', '*', '.', '*', '#', '*'},
        {'*', 'o', '*', '.', '.', '.', '.', '.', '*', '#', '.', '.', '.', '.', '*', '.', '.', '*', '.', '*'},
        {'*', '.', '*', '.', '*', '*', '*', 'X', '*', '.', '*', '*', '*', '.', '*', '.', '*', '*', 'o', '*'},
        {'*', '#', '.', '.', '*', 'o', '.', '.', '.', '.', '*', '.', '.', '.', '*', '.', '*', '.', '.', '*'},
        {'*', '*', '*', '.', '*', '.', '*', '*', '*', '.', '*', '.', '*', '*', '*', '.', '*', '.', '*', '*'},
        {'*', '.', 'o', '.', '.', '.', '*', '.', '.', '.', '.', '.', '*', '.', '.', '.', '*', '.', 'o', '*'},
        {'*', '.', '*', '*', '*', '.', '*', '.', 'O', '.', '#', '.', '*', '.', '*', '*', '*', '.', '.', '*'},
        {'*', '#', '*', 'X', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '*', '#', '.', '*'},
        {'*', '.', '*', '.', '*', '*', '*', '.', '.', '.', '#', '.', '*', '*', '*', '.', '*', '.', '*', '*'},
        {'*', 'o', '.', '.', '*', '.', '.', '.', '*', '.', '.', '.', '*', '.', '.', '.', '*', '.', '.', '*'},
        {'*', '*', '*', '.', '*', '.', '*', '*', '*', 'o', '*', '*', '*', '.', '*', '.', '*', '*', '*', '*'},
        {'*', '.', '.', '.', '.', '.', '*', '.', '.', '.', '.', '.', '*', '.', '*', 'X', '.', '.', '.', '*'},
        {'*', '.', '*', '*', '*', 'o', '*', '.', '#', '.', '@', '.', '*', 'o', '*', '*', '*', '.', '#', '*'},
        {'*', '#', '*', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '*', '.', '.', '*'},
        {'*', '.', '*', '.', '*', '*', '*', '*', '*', '.', '*', '*', '*', '*', '*', '.', '*', 'o', '.', '*'},
        {'*', 'o', '.', '.', '*', '.', '.', '.', '.', '.', '*', '.', '.', '.', '*', '.', '.', '.', '*', '*'},
        {'*', '.', '*', '*', '*', '.', '*', '*', '*', '#', '*', '*', '*', '.', '*', '*', '*', '.', 'D', '*'},
        {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'}
    };
    
    for(int i = 0; i < TAMANHO; i++) {
        for(int j = 0; j < TAMANHO; j++) {
            mapa[i][j] = campo_ensolarado[i][j];
        }
    }
}

// Inicializar monstros
void inicializar_monstros(Monstro monstros[3], char mapa[TAMANHO][TAMANHO]) {
    int cont = 0;
    for(int i = 0; i < TAMANHO && cont < 3; i++) {
        for(int j = 0; j < TAMANHO && cont < 3; j++) {
            if(mapa[i][j] == MONSTRO) {
                monstros[cont].x = i;
                monstros[cont].y = j;
                monstros[cont].direcao = rand() % 4;
                monstros[cont].ativo = 1;
                cont++;
            }
        }
    }
}

// Exibir o mapa com atmosfera ensolarada
void exibir_mapa(char mapa[TAMANHO][TAMANHO], Jogador *jogador) {
    limpar_tela();
    
    // Efeito de luz solar
    set_cor(14); // Amarelo
    printf("   *** Raios de sol iluminam o campo pela abertura da caverna... ***\n");
    
    // Título do jogo
    set_cor(11); // Ciano claro
    printf("\n  +=============================================+\n");
    printf("  |         CAMPO ENSOLARADO - FASE 2         |\n");
    printf("  |           Jardim dos Perigos              |\n");
    printf("  +=============================================+\n\n");
    
    // Status do jogador
    set_cor(12); // Vermelho
    printf("  VIDAS: ");
    for(int i = 0; i < jogador->vida; i++) printf("S2 ");
    for(int i = jogador->vida; i < 3; i++) printf("F ");
    
    
    printf("\n");
    set_cor(13); // Magenta
    printf("  STATUS: ");
    if(jogador->tem_chave) {
        set_cor(10); // Verde
        printf("CHAVE DOURADA COLETADA! Encontre a saida!");
    } else {
        set_cor(12); // Vermelho
        printf("Colete a chave dourada (@) evitando os perigos!");
    }
    
    printf("\n\n  ");
    
    // Exibir o mapa colorido
    for(int i = 0; i < TAMANHO; i++) {
        for(int j = 0; j < TAMANHO; j++) {
            // Definir cores para cada elemento
            switch(mapa[i][j]) {
                case JOGADOR:
                    set_cor(15); // Branco brilhante
                    break;
                case PAREDE:
                    set_cor(8); // Cinza escuro (pedra)
                    break;
                case GRAMA:
                    set_cor(10); // Verde (grama)
                    break;
                case FLOR:
                    set_cor(13); // Magenta (flores)
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
                    set_cor(6); // Marrom/Amarelo escuro
                    break;
                case ESPINHO:
                    set_cor(12); // Vermelho
                    break;
                case MONSTRO:
                    set_cor(5); // Roxo
                    break;
                default:
                    set_cor(7); // Branco normal
            }
            printf("%c ", mapa[i][j]);
        }
        printf("\n  ");
    }
    
    // Avisos de perigo
    if(jogador->vida <= 2) {
        set_cor(12); // Vermelho
        printf("\n  >>> PERIGO: Vida baixa! Cuidado com espinhos e monstros!\n");
    }
    if(jogador->energia < 20) {
        set_cor(14); // Amarelo
        printf("  >>> ATENCAO: Energia baixa! Colete flores para recuperar!\n");
    }
    
    // Instruções
    set_cor(7); // Branco normal
    printf("\n  +=============================================+\n");
    printf("  | CONTROLES:                            |\n");
    printf("  | W/A/S/D - Explorar o campo            |\n");
    printf("  | I - Ativar botao (quando em cima)     |\n");
    printf("  | Q - Desistir da aventura              |\n");
    printf("  |                                       |\n");
    printf("  | ELEMENTOS DO CAMPO:                   |\n");
    set_cor(15); printf("  | & "); set_cor(7); printf("- Voce (Aventureiro)                |\n");
    set_cor(8);  printf("  | * "); set_cor(7); printf("- Paredes de Pedra                  |\n");
    set_cor(10); printf("  | . "); set_cor(7); printf("- Grama (recupera energia)          |\n");
    set_cor(13); printf("  | o "); set_cor(7); printf("- Flores (recupera vida)            |\n");
    set_cor(14); printf("  | @ "); set_cor(7); printf("- Chave Dourada                     |\n");
    set_cor(6);  printf("  | O "); set_cor(7); printf("- Botao (ativa armadilha!)          |\n");
    set_cor(12); printf("  | # "); set_cor(7); printf("- Espinhos (perde vida!)            |\n");
    set_cor(5);  printf("  | M "); set_cor(7); printf("- Monstro (perde vida!)             |\n");
    printf("  +=============================================+\n");
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

// Ativar armadilha de espinhos
void ativar_armadilha(char mapa[TAMANHO][TAMANHO], int x, int y) {
    // Criar espinhos em cruz ao redor do botão
    int dx[] = {-1, 1, 0, 0, -2, 2, 0, 0};
    int dy[] = {0, 0, -1, 1, 0, 0, -2, 2};
    
    for(int i = 0; i < 8; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        
        if(nx >= 0 && nx < TAMANHO && ny >= 0 && ny < TAMANHO) {
            if(mapa[nx][ny] == ESPACO_VAZIO || mapa[nx][ny] == GRAMA) {
                mapa[nx][ny] = ESPINHO;
            }
        }
    }
}

// Mover monstros
void mover_monstros(char mapa[TAMANHO][TAMANHO], Monstro monstros[3]) {
    int dx[] = {-1, 0, 1, 0}; // norte, leste, sul, oeste
    int dy[] = {0, 1, 0, -1};
    
    for(int i = 0; i < 3; i++) {
        if(!monstros[i].ativo) continue;
        
        int novo_x = monstros[i].x + dx[monstros[i].direcao];
        int novo_y = monstros[i].y + dy[monstros[i].direcao];
        
        // Verificar se pode mover
        if(movimento_valido(mapa, novo_x, novo_y) && mapa[novo_x][novo_y] != JOGADOR) {
            // Limpar posição atual
            if(mapa[monstros[i].x][monstros[i].y] == MONSTRO) {
                mapa[monstros[i].x][monstros[i].y] = ESPACO_VAZIO;
            }
            
            // Mover para nova posição
            monstros[i].x = novo_x;
            monstros[i].y = novo_y;
            mapa[novo_x][novo_y] = MONSTRO;
        } else {
            // Mudar direção se não pode mover
            monstros[i].direcao = (monstros[i].direcao + 1) % 4;
        }
    }
}

// Reiniciar fase (quando toca espinho)
void reiniciar_fase(char mapa[TAMANHO][TAMANHO], Jogador *jogador, Monstro monstros[3]) {
    inicializar_mapa(mapa);
    inicializar_monstros(monstros, mapa);
    
    // Resetar jogador
    jogador->x = 1;
    jogador->y = 1;
    jogador->vida = 5;
    jogador->energia = 100;
    jogador->tem_chave = 0;
    
    set_cor(12);
    printf("\n  *** OUCH! Voce foi ferido! Fase reiniciada! ***\n");
    set_cor(7);
    Sleep(2000);
}

// Mover jogador
void mover_jogador(char mapa[TAMANHO][TAMANHO], Jogador *jogador, Monstro monstros[3], char direcao) {
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
        case ' ': // Espaço para ativar botão
            if(mapa[jogador->x][jogador->y] == BOTAO) {
                ativar_armadilha(mapa, jogador->x, jogador->y);
                printf("\n  *** CUIDADO! Armadilha ativada! ***\n");
                Sleep(1500);
            }
            return;
        default:
            return;
    }
    
    if(movimento_valido(mapa, novo_x, novo_y)) {
        // Verificar colisão com monstro
        if(mapa[novo_x][novo_y] == MONSTRO) {
            jogador->vida--;
            if(jogador->vida <= 0) {
                reiniciar_fase(mapa, jogador, monstros);
                return;
            }
            printf("\n  *** Monstro te atacou! Vida: %d ***\n", jogador->vida);
            Sleep(1000);
            return;
        }
        
        // Verificar espinho
        if(mapa[novo_x][novo_y] == ESPINHO) {
            reiniciar_fase(mapa, jogador, monstros);
            return;
        }
        
        // Efeitos do terreno
        if(mapa[novo_x][novo_y] == GRAMA) {
            jogador->energia += 5; // Grama recupera energia
            jogador->pontos += 1;
        } else if(mapa[novo_x][novo_y] == FLOR) {
            jogador->vida++;
            jogador->energia += 10;
            jogador->pontos += 5;
            if(jogador->vida > 5) jogador->vida = 5;
            printf("\n  *** Flor coletada! Vida restaurada! ***\n");
            Sleep(800);
        }
        
        // Verificar se coletou a chave
        if(mapa[novo_x][novo_y] == CHAVE) {
            jogador->tem_chave = 1;
            jogador->pontos += 50;
            
            // Abrir a porta
            for(int i = 0; i < TAMANHO; i++) {
                for(int j = 0; j < TAMANHO; j++) {
                    if(mapa[i][j] == PORTA_FECHADA) {
                        mapa[i][j] = PORTA_ABERTA;
                    }
                }
            }
            printf("\n  *** CHAVE DOURADA COLETADA! Portal se abre! ***\n");
            Sleep(1500);
        }
        
        // Verificar se chegou na porta aberta
        if(mapa[novo_x][novo_y] == PORTA_ABERTA) {
            // Vitória!
            limpar_tela();
            set_cor(11); // Ciano
            printf("\n\n  +=============================================+\n");
            printf("  |                                           |\n");
            printf("  |          *** FANTASTICO! ***              |\n");
            printf("  |                                           |\n");
            printf("  |    VOCE CONQUISTOU O CAMPO ENSOLARADO!    |\n");
            printf("  |                                           |\n");
            printf("  |      O sol brilha sobre sua vitoria!      |\n");
            printf("  |         Fase 2 Concluida!                 |\n");
            printf("  |                                           |\n");
            printf("  |         Pontuacao Final: %4d              |\n", jogador->pontos);
            printf("  |                                           |\n");
            printf("  +=============================================+\n");
            
            set_cor(14);
            printf("\n  *** Voce sente a brisa fresca enquanto avanca\n");
            printf("      para a proxima fase de sua jornada...\n");
            
            set_cor(7);
            printf("\n  Pressione qualquer tecla para continuar...");
            getch();
            exit(0);
        }
        
        // Limitar energia
        if(jogador->energia > 100) jogador->energia = 100;
        if(jogador->energia < 0) jogador->energia = 0;
        
        // Mover jogador
        mapa[jogador->x][jogador->y] = ESPACO_VAZIO;
        mapa[novo_x][novo_y] = JOGADOR;
        jogador->x = novo_x;
        jogador->y = novo_y;
        
        // Reduzir energia gradualmente
        jogador->energia -= 1;
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
                jogador->vida = 5;
                jogador->energia = 100;
                jogador->pontos = 0;
                return;
            }
        }
    }
}

int main() {
    char mapa[TAMANHO][TAMANHO];
    Jogador jogador;
    Monstro monstros[3];
    char comando;
    int turno = 0;
    
    // Inicializar gerador de números aleatórios
    srand(time(NULL));
    
    // Configurar console
    SetConsoleTitle("Campo Ensolarado - Jardim dos Perigos");
    
    // Inicializar jogo
    inicializar_mapa(mapa);
    encontrar_jogador(mapa, &jogador);
    inicializar_monstros(monstros, mapa);
    
    // Tela de início atmosférica
    limpar_tela();
    set_cor(11);
    printf("\n\n  +=============================================+\n");
    printf("  |                                           |\n");
    printf("  |       *** CAMPO ENSOLARADO ***            |\n");
    printf("  |                                           |\n");
    printf("  |         FASE 2: JARDIM DOS PERIGOS        |\n");
    printf("  |                                           |\n");
    set_cor(7);
    printf("  |  apos atravessar a porta voce se depara   |\n");
    printf("  |  com um belo campo ensolarado iluminado   |\n");
    printf("  |  por raios de sol que atravessam uma      |\n");
    printf("  |  abertura na caverna. Mas cuidado!        |\n");
    printf("  |  Monstros vagam entre as flores...        |\n");
    printf("  |                                           |\n");
    printf("  |  Colete a chave dourada evitando os       |\n");
    printf("  |  espinhos e as criaturas hostis!          |\n");
    printf("  |                                           |\n");
    set_cor(11);
    printf("  +=============================================+\n");
    
    set_cor(14);
    printf("\n  *** Os passaros cantam ao longe... Flores balancam na brisa...\n");
    set_cor(12);
    printf("  *** Mas algo sombrio se move entre as sombras...\n");
    set_cor(7);
    printf("\n  Pressione qualquer tecla para iniciar a aventura...");
    getch();
    
    // Loop principal do jogo
    while(1) {
        exibir_mapa(mapa, &jogador);
        
        comando = getch();
        
        if(comando == 'q' || comando == 'Q') {
            set_cor(8);
            printf("\n  *** Voce desiste da aventura e retorna para a escuridao...\n");
            set_cor(7);
            break;
        }
        
        mover_jogador(mapa, &jogador, monstros, comando);
        
        // Mover monstros a cada 2 turnos
        if(turno % 2 == 0) {
            mover_monstros(mapa, monstros);
        }
        
        turno++;
        
        // Verificar se energia chegou a zero
        if(jogador.energia <= 0) {
            jogador.vida--;
            jogador.energia = 20;
            if(jogador.vida <= 0) {
                reiniciar_fase(mapa, &jogador, monstros);
                turno = 0;
            }
        }
    }
    
    return 0;
}