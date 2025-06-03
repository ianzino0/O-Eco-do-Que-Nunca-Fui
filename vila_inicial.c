#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>

#define GRID_SIZE 10
#define MAX_DIALOG_LENGTH 500

// Estrutura para posição
typedef struct {
    int x, y;
} Position;

// Estrutura para NPC
typedef struct {
    Position pos;
    char dialog[MAX_DIALOG_LENGTH];
    char name[50];
} NPC;

// Estrutura do jogo
typedef struct {
    char grid[GRID_SIZE][GRID_SIZE];
    Position player;
    Position key;
    Position door;
    Position button;
    NPC npcs[3];
    bool hasKey;
    bool doorOpen;
    bool buttonPressed;
    bool gameWon;
} Game;

// Cores para console
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Limpar tela
void clearScreen() {
    system("cls");
}

// Posicionar cursor
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Inicializar o jogo
void initGame(Game* game) {
    // Limpar grid
    for(int i = 0; i < GRID_SIZE; i++) {
        for(int j = 0; j < GRID_SIZE; j++) {
            game->grid[i][j] = ' '; // Chão verde
        }
    }
    
    // Criar paredes ao redor
    for(int i = 0; i < GRID_SIZE; i++) {
        game->grid[0][i] = '*';      // Parede superior
        game->grid[GRID_SIZE-1][i] = '*';  // Parede inferior
        game->grid[i][0] = '*';      // Parede esquerda
        game->grid[i][GRID_SIZE-1] = '*';  // Parede direita
    }
    
    // Criar algumas paredes internas
    game->grid[3][3] = '*';
    game->grid[3][4] = '*';
    game->grid[6][6] = '*';
    game->grid[6][7] = '*';
    
    // Posicionar casas (marrons) - representadas por 'H'
    game->grid[2][2] = 'H';
    game->grid[2][7] = 'H';
    game->grid[7][2] = 'H';
    
    // Posicionar jogador
    game->player.x = 5;
    game->player.y = 8;
    
    // Posicionar chave
    game->key.x = 2;
    game->key.y = 5;
    
    // Posicionar porta da masmorra (entrada)
    game->door.x = 5;
    game->door.y = 1;
    game->grid[1][5] = 'D'; // Marca a porta no grid
    
    // Posicionar botão
    game->button.x = 7;
    game->button.y = 5;
    
    // Configurar NPCs
    game->npcs[0].pos.x = 1;
    game->npcs[0].pos.y = 2;
    strcpy(game->npcs[0].name, "o grandissimo Girotto");
    strcpy(game->npcs[0].dialog, "Ola, aventureiro! Use WASD para se mover.\nPressione I para interagir com objetos e NPCs.\nColete a chave (@) para abrir a porta da masmorra!");
    
    game->npcs[1].pos.x = 8;
    game->npcs[1].pos.y = 2;
    strcpy(game->npcs[1].name, "Sabia Polyana");
    strcpy(game->npcs[1].dialog, "Cuidado na masmorra, jovem guerreiro!\nAlguns botoes (O) podem ativar armadilhas ou abrir passagens.\nSempre explore cada canto para encontrar tesouros!");
    
    game->npcs[2].pos.x = 1;
    game->npcs[2].pos.y = 7;
    strcpy(game->npcs[2].name, "Guarda Alessandra");
    strcpy(game->npcs[2].dialog, "A porta da masmorra esta ao norte (D).\nVoce precisa da chave dourada para abri-la.\nQuando aberta, ela se tornara uma entrada (=).\nBoa sorte em sua jornada, aventureiro!");
    
    // Estados iniciais
    game->hasKey = false;
    game->doorOpen = false;
    game->buttonPressed = false;
    game->gameWon = false;
}

// Desenhar o grid
void drawGrid(Game* game) {
    gotoxy(0, 0);
    
    printf("\n=== VILA - O Eco do Que Nunca Fui ===\n\n");
    
    // Desenhar grid
    for(int i = 0; i < GRID_SIZE; i++) {
        printf("  ");
        for(int j = 0; j < GRID_SIZE; j++) {
            char cell = game->grid[i][j];
            
            // Verificar se é a posição do jogador
            if(i == game->player.y && j == game->player.x) {
                setColor(10); // Verde brilhante
                printf("& ");
                setColor(7);  // Branco
            }
            // Verificar NPCs
            else if((i == game->npcs[0].pos.y && j == game->npcs[0].pos.x) ||
                    (i == game->npcs[1].pos.y && j == game->npcs[1].pos.x) ||
                    (i == game->npcs[2].pos.y && j == game->npcs[2].pos.x)) {
                setColor(11); // Ciano brilhante
                printf("P ");
                setColor(7);
            }
            // Verificar chave
            else if(i == game->key.y && j == game->key.x && !game->hasKey) {
                setColor(14); // Amarelo
                printf("@ ");
                setColor(7);
            }
            // Verificar porta da masmorra
            else if(i == game->door.y && j == game->door.x) {
                if(game->doorOpen) {
                    setColor(8); // Cinza escuro
                    printf("= ");
                } else {
                    setColor(0); // Preto com fundo branco para visibilidade
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
                    printf("D ");
                    setColor(7);
                }
                setColor(7);
            }
            // Verificar botão
            else if(i == game->button.y && j == game->button.x) {
                if(game->buttonPressed) {
                    setColor(12); // Vermelho
                } else {
                    setColor(6);  // Amarelo escuro
                }
                printf("O ");
                setColor(7);
            }
            // Parede
            else if(cell == '*') {
                setColor(8); // Cinza
                printf("* ");
                setColor(7);
            }
            // Casa
            else if(cell == 'H') {
                setColor(6); // Marrom
                printf("H ");
                setColor(7);
            }
            // Chão
            else {
                setColor(2); // Verde
                printf("  ");
                setColor(7);
            }
        }
        printf("\n");
    }
    
    // Status do jogo
    printf("\n=== STATUS ===\n");
    printf("Posicao: (%d, %d)\n", game->player.x, game->player.y);
    printf("Chave: %s\n", game->hasKey ? "COLETADA" : "NAO COLETADA");
    printf("Porta: %s\n", game->doorOpen ? "ABERTA" : "FECHADA");
    printf("Botao: %s\n", game->buttonPressed ? "ATIVADO" : "INATIVO");
    
    printf("\n=== CONTROLES ===\n");
    printf("W/A/S/D - Mover\n");
    printf("I - Interagir\n");
    printf("Q - Sair\n");
    
    printf("\n=== LEGENDA ===\n");
    setColor(10); printf("& "); setColor(7); printf("Jogador  ");
    setColor(11); printf("P "); setColor(7); printf("NPC  ");
    setColor(14); printf("@ "); setColor(7); printf("Chave\n");
    setColor(8);  printf("* "); setColor(7); printf("Parede   ");
    setColor(6);  printf("H "); setColor(7); printf("Casa ");
    printf("D/= Porta  ");
    setColor(6);  printf("O "); setColor(7); printf("Botao\n");
}

// Mostrar diálogo
void showDialog(char* name, char* text) {
    clearScreen();
    printf("\n" "==========================================\n");
    printf("  %s\n", name);
    printf("==========================================\n\n");
    printf("%s\n\n", text);
    printf("Pressione qualquer tecla para continuar...\n");
    getch();
}

// Verificar se posição é válida
bool isValidMove(Game* game, int x, int y) {
    if(x < 0 || x >= GRID_SIZE || y < 0 || y >= GRID_SIZE) {
        return false;
    }
    if(game->grid[y][x] == '*') {
        return false;
    }
    return true;
}

// Mover jogador
void movePlayer(Game* game, int dx, int dy) {
    int newX = game->player.x + dx;
    int newY = game->player.y + dy;
    
    if(isValidMove(game, newX, newY)) {
        game->player.x = newX;
        game->player.y = newY;
    }
}

// Interagir
void interact(Game* game) {
    int px = game->player.x;
    int py = game->player.y;
    
    // Verificar interação com NPCs
    for(int i = 0; i < 3; i++) {
        if(abs(px - game->npcs[i].pos.x) <= 1 && abs(py - game->npcs[i].pos.y) <= 1) {
            showDialog(game->npcs[i].name, game->npcs[i].dialog);
            return;
        }
    }
    
    // Verificar coleta de chave
    if(px == game->key.x && py == game->key.y && !game->hasKey) {
        game->hasKey = true;
        showDialog("Sistema", "Voce coletou a chave dourada!\nAgora voce pode abrir a porta da masmorra!");
        return;
    }
    
    // Verificar abertura da porta
    if(abs(px - game->door.x) <= 1 && abs(py - game->door.y) <= 1) {
        if(game->hasKey && !game->doorOpen) {
            game->doorOpen = true;
            showDialog("Sistema", "A porta da masmorra foi aberta!\nVoce pode entrar na masmorra agora!");
        } else if(!game->hasKey) {
            showDialog("Sistema", "A porta da masmorra esta trancada.\nVoce precisa encontrar a chave dourada!");
        } else {
            showDialog("Sistema", "Parabens! Voce pode entrar na masmorra!\nEsta e a entrada para sua grande aventura!\nTutorial concluido com sucesso!");
            game->gameWon = true;
        }
        return;
    }
    
    // Verificar botão
    if(px == game->button.x && py == game->button.y) {
        game->buttonPressed = !game->buttonPressed;
        if(game->buttonPressed) {
            showDialog("Sistema", "Botao ativado! Uma passagem secreta se abriu...\n(Esta e uma demonstracao da mecanica de botoes)");
        } else {
            showDialog("Sistema", "Botao desativado! A passagem secreta se fechou...");
        }
        return;
    }
    
    showDialog("Sistema", "Nao ha nada para interagir aqui.\nTente se aproximar de NPCs, chaves, portas ou botoes!");
}

// Loop principal do jogo
void gameLoop(Game* game) {
    char input;
    
    while(!game->gameWon) {
        clearScreen();
        drawGrid(game);
        
        input = getch();
        
        switch(input) {
            case 'w':
            case 'W':
                movePlayer(game, 0, -1);
                break;
            case 's':
            case 'S':
                movePlayer(game, 0, 1);
                break;
            case 'a':
            case 'A':
                movePlayer(game, -1, 0);
                break;
            case 'd':
            case 'D':
                movePlayer(game, 1, 0);
                break;
            case 'i':
            case 'I':
                interact(game);
                break;
            case 'q':
            case 'Q':
                printf("\nObrigado por jogar!\n");
                return;
        }
    }
    
    clearScreen();
    printf("\n" "==========================================\n");
    printf("        TUTORIAL CONCLUIDO!\n");
    printf("==========================================\n\n");
    printf("Parabens! Voce aprendeu:\n");
    printf("- Como se mover (WASD)\n");
    printf("- Como interagir (I)\n");
    printf("- Como coletar itens\n");
    printf("- Como abrir a entrada da masmorra\n");
    printf("- Como usar botoes\n\n");
    printf("A porta da masmorra esta agora aberta!\n");
    printf("Sua aventura pode comecar!\n\n");
    printf("Pressione qualquer tecla para sair...\n");
    getch();
}

// Tela de introdução
void showIntro() {
    clearScreen();
    printf("\n" "==========================================\n");
    printf("     BEM-VINDO A VILA!\n");
    printf("==========================================\n\n");
    printf("Apos um longa caminhada na neve voce encontra uma\n");
    printf("pequena vila que possui uma entrada para uma masmorra.\n\n");
    printf("Os aldeoes vao te ensinar como jogar!\n\n");
    printf("OBJETIVO:\n");
    printf("- Converse com os NPCs (P) usando I\n");
    printf("- Colete a chave dourada (@) com I\n");
    printf("- Abra a porta da masmorra (D) com I\n");
    printf("- Explore a mecanica do botao (O) com I\n\n");
    printf("Pressione qualquer tecla para comecar...\n");
    getch();
}

int main() {
    Game game;
    
    // Configurar console
    SetConsoleTitle("Vila Tutorial - Jogo de Aventura");
    
    showIntro();
    initGame(&game);
    gameLoop(&game);
    
    return 0;
}