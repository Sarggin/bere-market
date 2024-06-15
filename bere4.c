//inclusão das bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Constantes do sistema
#define MAX_USER_LENGTH 12
#define MIN_USER_LENGTH 8
#define MAX_PASS_LENGTH 8
#define MIN_PASS_LENGTH 6

// Structs do sistema, Matriz heterogênea
typedef struct {
    int id;
    char nome[50];
} Menu;

typedef struct {
    char login[MAX_USER_LENGTH + 1];
    char password[MAX_PASS_LENGTH + 1];
    int type; // 1 para Administrador, 2 para Usuário
} Usuario;

// Declarando as funções que vamos usar no código
void menuPrincipal();
int opcaoEscolhida();
void opcoes(int opcao);
void cadastro();
void vendas();
void aberturaCaixa();
void fechaCaixa();
void relatorios();
void sair();
void clear();
void opcaoCadastro();

// Execução das Funções
void menuPrincipal() {
    clear();

    Menu principal[] = {
        {1, "Cadastros"},
        {2, "Vendas"},
        {3, "Abertura do Caixa"},
        {4, "Fechamento do Caixa"},
        {5, "Relatorios"},
        {6, "Sair"},
    };

    printf("\n========================================\n");
    printf("         SEJA BEM-VINDO(A) AO BERE\n");
    printf("         ESCOLHA UMA OPCAO ABAIXO:\n");
    printf("========================================\n");

    for (int i = 0; i < 6; i++) {
        printf(" %d - %s\n", principal[i].id, principal[i].nome);
        if (i < 5) {
            printf("----------------------------------------\n");
        }
    }

    printf("========================================\n");

    int escolha = opcaoEscolhida();
    opcoes(escolha);
}

int opcaoEscolhida(){
    int opcao;

    printf("\nDigite a opcao desejada: ");
    scanf("%d", &opcao);

    return opcao;
}

void opcoes(int opcao){
    switch (opcao)
    {
    case 1:
        cadastro();
        break;
    case 2:
        vendas();
        break;
    case 3:
        aberturaCaixa();
        break;
    case 4:
        fechaCaixa();
        break;
    case 5:
        relatorios();
        break;
    case 6:
        sair();
        break;
    default:
        printf("\nOpcao desejada não existe!\n");
        break;
    }
}
void cadastro(){
    clear();

    Menu cadastro[] = {
        {1, "Cadastro de usuarios"},
        {2, "Cadastro de Clientes"},
        {3, "Cadastro de Produtos"},
        {4, "Cadastro de Categoria"},
        {5, "Retornar ao Menu Principal"}
    };

    printf("\n========================================\n");
    printf("         CADASTROS\n");
    printf("========================================\n");

    for (int i = 0; i < 5; i++) {
        printf(" %d - %s\n", cadastro[i].id, cadastro[i].nome);
        if (i < 5) {
            printf("----------------------------------------\n");
        }
    }

    printf("========================================\n");

    int escolha = opcaoEscolhida();
    opcaoCadastro(escolha);
}

void opcaoCadastro(){

}

void clear(){
    #ifdef _WIN32
        system("cls"); // Comando para limpar tela no Windows
    #else
        system("clear"); // Comando para limpar tela no Unix/Linux/macOS
    #endif
}

int main(){
    menuPrincipal();

    return 0;
}