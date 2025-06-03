#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

// Função para limpar a tela (multiplataforma)
void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Função para pausar e aguardar entrada do usuário
void pausar() {
    printf("\nPressione ENTER para continuar...");
    getchar();
    getchar(); // Para capturar o ENTER após a escolha do menu
}

// Função do jogo principal
void historia() {
    setlocale(LC_ALL, "Portuguese"); // Configura o locale para português
    limparTela();
    printf("=== O Eco do Que Nunca Fui ===\n\n");
    
    // AQUI VOCÊ PODE COLOCAR O CÓDIGO DO SEU JOGO
    printf(" Capitulo 1: Aquele Que Caminha\n");
    printf("Ele nao lembrava o proprio nome. Nem se isso importava.\n");
    printf("Acordou no assoalho frio de um templo abandonado, sob o som de sinos que nao estavam la.\n");
    printf("Seu corpo doia como se tivesse sido costurado por dentro, musculo por musculo.\n");
    printf("O sangue ja havia secado. Os cacos de espelho ao lado refletiam um rosto que nao parecia dele - e talvez nao fosse.\n\n");
    printf("Havia uma espada enferrujada ao seu lado. Pegou-a. Nao por coragem, mas por instinto.\n");
    printf("O mundo la fora era cinzento, feito de torres vazias, florestas que sussurravam e ruas onde as sombras andavam mesmo sem luz.\n");
    printf("Em cada canto, algo o observava — mas nunca se mostrava.\n");
    printf("Ele não falava. Não precisava. Onde passava, os outros o chamavam de muitas coisas: Vazio, O Peregrino, O Último Filho da Cicatriz.\n");
    printf("A cada vilarejo, ele ajudava estranhos com maldições que espelhavam a sua própria: gente esquecida por deuses, consumida por passado ou transformada em símbolos de dor.\n");
    printf("E cada vez que resolvia algo, um fragmento de memória voltava... mas não eram memórias dele.\n");
    printf("E então surgiu o espelho negro. Um artefato dito capaz de mostrar a “Verdade Original” — quem ele foi, ou o que o fez esquecer. Diante dele, o reflexo sorriu antes dele.");
    printf("E então falou:\n");
    printf("              “Você se apagou por escolha. Quer mesmo lembrar?”");
    printf("Ele hesitou. A espada, sempre silenciosa, agora pesava.");
    printf("Virou-se. E andou.");
    printf("O espelho ficou para trás.");

    
    pausar();
}

// Função para mostrar créditos
void creditos() {
    limparTela();
    printf("=== CREDITOS ===\n\n");
    printf("Desenvolvedor: Ian Mendes\n");
    printf("Agradecimentos especiais: meus pensamentos caóticos\n");
    printf("Obrigado por jogar!\n");
    
    pausar();
}

// Função para exibir o menu principal
void exibirMenu() {
    printf("================================\n");
    printf("   O Eco do Que Nunca Fui       \n");
    printf("================================\n");
    printf("  1. Jogar                      \n");
    printf("  2. Creditos                   \n");
    printf("  3. Sair                       \n");
    printf("  4. Historia                   \n");
    printf("================================\n\n");
    printf("Escolha uma opcao: ");
}

int main() {
    int opcao;
    int continuar = 1;
    
    while (continuar) {
        limparTela();
        exibirMenu();
        
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                jogar();
                break;
                
            case 2:
                creditos();
                break;
                
            case 3:
                limparTela();
                printf("Obrigado por jogar!\n");
                printf("Ate a proxima!\n\n");
                continuar = 0;
                break;
                
                case 4:
                historia();
                break;

            default:
                limparTela();
                printf("Opcao invalida! Tente novamente.\n");
                pausar();
                break;
        }
    }
    
    return 0;
}

/*
=====================================
  ÁREA PARA IMPLEMENTAR SEU JOGO
=====================================

Você pode adicionar suas funções de jogo aqui:

// Exemplo de estrutura para um jogo simples:

typedef struct {
    int pontuacao;
    int vidas;
    int nivel;
} StatusJogo;

void inicializarJogo(StatusJogo *status) {
    status->pontuacao = 0;
    status->vidas = 3;
    status->nivel = 1;
}

void loopPrincipalJogo() {
    StatusJogo status;
    inicializarJogo(&status);
    
    // Implementar lógica do jogo aqui
    // while (jogo_ativo) {
    //     processar_entrada();
    //     atualizar_jogo();
    //     desenhar_tela();
    // }
}

// Adicione mais funções conforme necessário:
// - processarEntrada()
// - atualizarJogo()
// - desenharTela()
// - verificarColisao()
// - etc.

*/