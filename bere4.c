// inclusão das Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// Constantes do sistema
#define TAMANHO_MAXIMO_USUARIO 12
#define TAMANHO_MINIMO_USUARIO 8
#define TAMANHO_MAXIMO_SENHA 8
#define TAMANHO_MINIMO_SENHA 6

// VariaveIS Globais
float dinheiroCaixa;
bool statusCaixa = 0;
int numCaixa = 0;

// Structs do sistema, Matriz heterogênea
typedef struct {
    int id;
    char nome[50];
} Menu;

typedef struct {
    char login[TAMANHO_MAXIMO_USUARIO];
    char password[TAMANHO_MAXIMO_SENHA];
    int type; // 1 para Administrador, 2 para Usuário
} Usuario;

typedef struct {
    int id;
    char nome[50];
    char nomeSocial[50];
    char cpf[11];
    char rua[50];
    int numero;
    char celular[13];
} Clientes;

typedef struct {
  int codigo;
  char descricao[100];
  char categoria[20];
  float precoCompra;
  float margemLucro;
  float precoVenda;
  int quantidadeEstoque;
  int estoqueMinimo;
} Produtos;

typedef struct {
  char nome[50];
} Categorias;

typedef struct {
    int codigo;
    char descricao[50]; 
    float precoVenda;
    int quantidade;
    float total;
} Carrinho;

// Declarando as funções que vamos usar no código
void menuPrincipal();
int opcaoEscolhida();
void opcoes(int opcao);
void cadastro();
void opcaoCadastro(int opcao);
void cadastroCliente();
void cadastroUsuario();
void cadastroProduto();
void cadastroCategoria();
void menuVendas();
void opcaoVendas(int opcao);
void novaVenda();
void documentoVenda(Carrinho *carrinho, int numItensCarrinho);
int produtosBereFixo(Produtos *listaProdutos, int indice);
void carrinho();
void sangria();
void pagamento();
void aberturaCaixa();
void fechaCaixa();
void relatorios();
void sair();
void clear();
void dataAtual();

// Execução das Funções
void menuPrincipal(){
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
    do
    {
        switch (opcao) {
            case 1:
                cadastro();
                break;
            case 2:
                menuVendas();
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
                printf("\nOpcao desejada nao existe!\n");
                break;
        }

        scanf("%d", &opcao); // Guarda a opcao escolhida pelo usuário

    } while (opcao != 6);
}

void cadastro(){
    clear();

    Menu cadastro[] = {
        {1, "Cadastro de Usuarios"},
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
        if (i < 4) {
            printf("----------------------------------------\n");
        }
    }

    printf("========================================\n");

    int escolha = opcaoEscolhida();
    opcaoCadastro(escolha);
}

void opcaoCadastro(int opcao){

    while (opcao < 1 || opcao > 5) {
        printf("\nOpcao invalida! Digite uma opcao valida: ");
        scanf("%d", &opcao);
    }

    switch (opcao) {
        case 1:
            cadastroUsuario();
            break;
        case 2:
            cadastroCliente();
            break;
        case 3:
            cadastroProduto();
            break;
        case 4:
            cadastroCategoria();
            break;
        case 5:
            printf("Retornando ao Menu Principal...\n");
            menuPrincipal();
            break;
        default:
            printf("\nOpcao desejada nao existe!\n");
            break;
    }
}

void cadastroUsuario(){
    FILE *file;
    Usuario usuario;
    int controlador = 0;

    // Abrir o arquivo em modo append para adicionar usuários ao final do arquivo
    file = fopen("usuarios.txt", "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de usuários.\n");
        return;
    }

    // Coletar dados do usuário com tratamento de tamanho para o login
    while (!controlador) {
        printf("\nDigite o login do usuario (min %d caracteres e max %d caracteres): ", TAMANHO_MINIMO_USUARIO, TAMANHO_MAXIMO_USUARIO);
        scanf(" %[^\n]", usuario.login);

        // Remover o newline do final da string, se houver
        usuario.login[strcspn(usuario.login, "\n")] = '\0';

        if (strlen(usuario.login) >= TAMANHO_MINIMO_USUARIO && strlen(usuario.login) <= TAMANHO_MAXIMO_USUARIO) {
            controlador = 1;
        } else {
            printf("Login invalido. O login deve ter entre %d e %d caracteres.\n", TAMANHO_MINIMO_USUARIO, TAMANHO_MAXIMO_USUARIO);
        }
    }

    // Reseta o controlador para a senha
    controlador = 0;
    while (!controlador) {
        printf("Digite a senha do usuario (min %d caracteres e max %d caracteres): ", TAMANHO_MINIMO_SENHA, TAMANHO_MAXIMO_SENHA);
        scanf(" %[^\n]", usuario.password);

        // Remover o newline do final da string, se houver
        usuario.password[strcspn(usuario.password, "\n")] = '\0';

        if (strlen(usuario.password) >= TAMANHO_MINIMO_SENHA && strlen(usuario.password) <= TAMANHO_MAXIMO_SENHA) {
            controlador = 1;
        } else {
            printf("\nSenha invalida. Deve ter entre %d e %d caracteres.\n", TAMANHO_MINIMO_SENHA, TAMANHO_MAXIMO_SENHA);
        }
    }

    // Coletar o tipo de usuário
    printf("Digite o tipo de usuario (1 para Administrador, 2 para Usuario): ");
    scanf("%d", &usuario.type);

    // Limpar o buffer de entrada
    while (getchar() != '\n'); // Limpar o buffer de entrada

    // Escrever os dados do usuário no arquivo
    fprintf(file, "%s %s %d\n", usuario.login, usuario.password, usuario.type);

    // Fechar o arquivo
    fclose(file);

    printf("Usuario cadastrado com sucesso!\n");

    system("pause");

    cadastro();
}

void cadastroCliente(){
    FILE *arquivo;
    Clientes cliente;

    // Abrindo o arquivo no modo "a" (anexar)
    arquivo = fopen("clientes.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de clientes.\n");
        return;
    }

    // Obtendo dados do cliente
    printf("Informe o ID do cliente: ");
    scanf("%d", &cliente.id);

    printf("Informe o nome do cliente: ");
    scanf(" %[^\n]", cliente.nome); // Ignora espaços em branco antes do nome

    printf("Informe o nome social do cliente: ");
    scanf(" %[^\n]", cliente.nomeSocial); // Ignora espaços em branco antes do nome social

    printf("Informe o CPF do cliente: ");
    scanf(" %[^\n]", cliente.cpf);

    printf("Informe a rua do cliente: ");
    scanf(" %[^\n]", cliente.rua);

    printf("Informe o numero da casa do cliente: ");
    scanf("%d", &cliente.numero);

    printf("Informe o celular do cliente: ");
    scanf(" %[^\n]", cliente.celular);

    // Validações básicas (deixe-as mais robustas de acordo com a necessidade)
    if (strlen(cliente.nome) == 0) {
        printf("Erro: Nome não pode ser vazio.\n");
        return;
    }

    if (strlen(cliente.cpf) < 11) {
        printf("Erro: CPF inválido.\n");
        return;
    }

    // Escrevendo os dados do cliente no arquivo
    fprintf(arquivo, "%d %s %s %s %s %d %s\n", cliente.id, cliente.nome, cliente.nomeSocial, cliente.cpf, cliente.rua, cliente.numero, cliente.celular);

    // Fechando o arquivo
    fclose(arquivo);

    printf("Cliente cadastrado com sucesso!\n");
    system("pause");

    cadastro();
}

void cadastroProduto(){
    FILE *arquivo;
    Produtos produto;

    // Abrindo o arquivo no modo "a" (anexar)
    arquivo = fopen("produtos.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de produtos.\n");
        return;
    }

    // Obtendo dados do produto do usuário
    printf("Informe o codigo do produto: ");
    scanf("%d", &produto.codigo);

    printf("Informe a descricao do produto: ");
    scanf(" %[^\n]", produto.descricao); // Ignora espaços em branco antes da descrição

    printf("Informe a categoria do produto (Alimento, Material de Limpeza, Panificacao): ");
    scanf(" %[^\n]", produto.categoria); // Ignora espaços em branco antes da descrição

    printf("Informe o preco de compra do produto: ");
    scanf("%f", &produto.precoCompra);

    printf("Informe a margem de lucro (em porcentagem): ");
    scanf("%f", &produto.margemLucro);

    printf("Informe a quantidade em estoque: ");
    scanf("%d", &produto.quantidadeEstoque);

    printf("Informe a quantidade de estoque minimo: ");
    scanf("%d", &produto.estoqueMinimo);

    // Validações básicas (deixe-as mais robustas de acordo com a necessidade)
    if (strlen(produto.descricao) == 0) {
        printf("Erro: Descrição não pode ser vazia.\n");
        return;
    }

    if (strcmp(produto.categoria, "Alimento") != 0 &&
        strcmp(produto.categoria, "Material de Limpeza") != 0 &&
        strcmp(produto.categoria, "Panificacao") != 0) {
        printf("Erro: Categoria invalida. As categorias validas sao: Alimento, Material de Limpeza, Panificacao.\n");
        return;
    }

    if (produto.precoCompra <= 0.0f) {
        printf("Erro: Preco de compra deve ser maior que zero.\n");
        return;
    }

    if (produto.margemLucro <= 0.0f) {
        printf("Erro: Margem de lucro deve ser maior que zero.\n");
        return;
    }

    // Calculando o preço de venda com base na margem de lucro
    produto.precoVenda = produto.precoCompra * (1.0f + produto.margemLucro / 100.0f);

    // Escrevendo os dados do produto no arquivo
    fprintf(arquivo, "%d %s %s %f %f %f %d %d\n", produto.codigo, produto.descricao, produto.categoria, 
            produto.precoCompra, produto.margemLucro, produto.precoVenda, produto.quantidadeEstoque, produto.estoqueMinimo);

    // Fechando o arquivo
    fclose(arquivo);

    printf("Produto cadastrado com sucesso!\n");
    system("pause");

    cadastro();
}

void cadastroCategoria(){
    FILE *arquivo;
    Categorias categoria;

    // Abrindo o arquivo no modo "a" (anexar)
    arquivo = fopen("categorias.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de categorias.\n");
        return;
    }

    // Obtendo dados da categoria do usuário
    printf("Informe o nome da categoria: ");
    scanf(" %[^\n]", categoria.nome); // Ignora espaços em branco antes do nome

    // Validações básicas (deixe-as mais robustas de acordo com a necessidade)
    if (strlen(categoria.nome) == 0) {
        printf("Erro: Nome da categoria não pode ser vazio.\n");
        return;
    }

    // Escrevendo o nome da categoria no arquivo
    fprintf(arquivo, "%s\n", categoria.nome);

    // Fechando o arquivo
    fclose(arquivo);

    printf("Categoria cadastrada com sucesso!\n");

    system("pause");

    cadastro();
}

void menuVendas(){
    clear();
    Menu vendas[] = {
        {1, "Nova venda"},
        {2, "Retirada de Caixa (Sangria)"},
        {3, "Pagamento"},
        {4, "Retornar ao Menu Principal"},
    };

    printf("\n========================================\n");
    printf("                VENDAS\n");
    printf("==========================================\n");

    for (int i = 0; i < 4; i++) {
        printf(" %d - %s\n", vendas[i].id, vendas[i].nome);
        if (i < 4) {
            printf("----------------------------------------\n");
        }
    }

    printf("========================================\n");

    int escolha = opcaoEscolhida();
    opcaoVendas(escolha);
}

void opcaoVendas(int opcao){

    while (opcao < 1 || opcao > 4) {
        printf("\nOpcao invalida! Digite uma opcao valida: ");
        scanf("%d", &opcao);
    }

    switch (opcao) {
    case 1:
        novaVenda();
        break;
    case 2:
        sangria();
        break;
    case 3:
        pagamento();
        break;
    case 4:
        menuPrincipal();
        break;
    default:
        printf("\nOpcao desejada nao existe!\n");
        break;
    }
}

// Função para exibir o cabeçalho da lista de produtos
void exibirCabecalho(){
    printf("-------------------------------------------------------------------\n");
    printf("Codigo | Descricao         | Categoria         | Preco    | Estoque\n");
    printf("-------------------------------------------------------------------\n");
}

// Função para exibir um produto
void exibirProduto(Produtos p) {
    printf("%-7d| %-18s| %-18s| R$ %-5.2f |  %-6d\n", p.codigo, p.descricao, p.categoria, p.precoVenda, p.quantidadeEstoque);
}

void rodape(){
    printf("-------------------------------------------------------------------\n");
}

int carregarProdutos(Produtos listaProdutos[], int maxProdutos) {
    FILE *arquivo;
    int contador = 0;

    // Abrindo o arquivo no modo leitura
    arquivo = fopen("produtos.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de produtos.\n");
        return 0; // Retorna 0 indicando falha
    }

    // Lendo os produtos do arquivo e armazenando no array
    while (fscanf(arquivo, "%d %s %s %f %f %f %d %d",
                  &listaProdutos[contador].codigo,
                  listaProdutos[contador].descricao,
                  listaProdutos[contador].categoria,
                  &listaProdutos[contador].precoCompra,
                  &listaProdutos[contador].margemLucro,
                  &listaProdutos[contador].precoVenda,
                  &listaProdutos[contador].quantidadeEstoque,
                  &listaProdutos[contador].estoqueMinimo) == 8) {
        contador++;
        if (contador >= maxProdutos) break; // Evita overflow do array
    }

    // Fechando o arquivo
    fclose(arquivo);

    return contador; // Retorna o número de produtos carregados
}

// Função para processar uma nova venda
void novaVenda() {
    clear();
    Produtos produtos[100]; // Array para armazenar os produtos carregados e dinâmicos
    int numProdutos = carregarProdutos(produtos, 50); // Carrega os produtos do arquivo
    numProdutos = produtosBereFixo(produtos, numProdutos); // Adiciona os produtos dinâmicos

    int codigoCompra;
    int quantidade;
    char continuarCompra = 's';

    // Exibindo cabeçalho e lista de produtos disponíveis
    exibirCabecalho();
    for (int i = 0; i < numProdutos; i++) {
        exibirProduto(produtos[i]);
    }
    rodape();

    int encontrado = 0;
    // Simulando a escolha e compra de produtos
    while (continuarCompra == 's' || continuarCompra == 'S') {
        printf("\nCarrinho de Compras\n");
        printf("\nInforme o codigo do produto a ser comprado: ");
        scanf("%d", &codigoCompra);

        for (int i = 0; i < numProdutos; i++) {
            if (produtos[i].codigo == codigoCompra){
                encontrado = 1;
            }
        }

        if (!encontrado) {
            printf("\nProduto nao encontrado.\n");
            continue;
        }

        printf("\nInforme a quantidade: ");
        scanf("%d", &quantidade);

        // Procurando o produto escolhido pelo código
        for (int i = 0; i < numProdutos; i++) {
            printf("\nProduto adicionado ao carrinho.\n");
            printf("Descricao: %s\n", produtos[i].descricao);
            printf("Preco Unitario: R$ %.2f\n", produtos[i].precoVenda);
            printf("Quantidade: %d\n", quantidade);
            printf("Total: R$ %.2f\n", produtos[i].precoVenda * quantidade);
            break;
        }

        printf("\nNovo item no carrinho de compra (s/n): ");
        scanf(" %c", &continuarCompra);
    }
    menuVendas();
}

// Função para inicializar os produtos dinâmicos
int produtosBereFixo(Produtos listaProdutos[], int indice) {
    Produtos produtosDinamicos[] = {
        {1000, "Cafe", "Alimento", 0, 0, 7.70, 10, 0},
        {1001, "Esponja", "Material Limpeza", 0, 0, 2.99, 30, 0},
        {1002, "Biscoito doce", "Panificacao", 0, 0, 12.50, 5, 0}
    };
    int numDinamicos = sizeof(produtosDinamicos) / sizeof(produtosDinamicos[0]);

    for (int i = 0; i < numDinamicos; i++) {
        listaProdutos[indice++] = produtosDinamicos[i];
    }

    return indice; // Retorna o novo índice após adicionar os produtos dinâmicos
}

void documentoVenda(Carrinho carrinho[], int numItensCarrinho){
    FILE *file;
    file = fopen("venda.txt", "a"); // Abrir arquivo para escrita

    if (file == NULL) {
        printf("Erro ao abrir o arquivo.");
        return;
    }

    fprintf(file, "***** Documento de Venda *****\n\n");
    fprintf(file, "Produtos Comprados:\n");
    for (int i = 0; i < numItensCarrinho; i++) {
        fprintf(file, "Codigo: %d\n", carrinho[i].codigo);
        fprintf(file, "Descricao: %s\n", carrinho[i].descricao);
        fprintf(file, "Preco Unitario: %.2f\n", carrinho[i].precoVenda);
        fprintf(file, "Quantidade: %d\n", carrinho[i].quantidade);
        fprintf(file, "Total: %.2f\n\n", carrinho[i].total);
    }

    fclose(file); // Fechar arquivo após escrita
    printf("Documento de venda gerado com sucesso.\n");
}

void dataAtual(){
    time_t mytime;
    mytime = time(NULL);
    struct tm tm = *localtime(&mytime);
    printf("Data da Compra: %d/%d/%d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

void carrinho(){

}

void sangria(){

}

void pagamento(){

}

void aberturaCaixa(){
    clear();
    if (statusCaixa == 1)
    {
        printf("\nO caixa ja esta aberto, pode seguir trabalhando normalmente.\n");
        system("pause");
        menuPrincipal();
    } else {
        printf("\n============== Abertura de Caixa ================\n");
        printf("Digite o valor para iniciar o caixa atual: R$ ");
        scanf("%f", &dinheiroCaixa);
    
        while (dinheiroCaixa <= 0) {
            printf("\nValor minimo para abertura do caixa não atingido.");
            printf("\nDigite novamente: R$ ");
            scanf("%f", &dinheiroCaixa);
        }

        printf("\nCaixa numero %d aberto com sucesso, valor de inicio do caixa: R$ %.2f\n", (numCaixa + 1), dinheiroCaixa);
        numCaixa++;
        statusCaixa = 1;

        system("pause");

        menuPrincipal();
    }
}

void fechaCaixa(){
    // Implementar lógica para fechamento do caixa
}

void relatorios() {
    // Implementar lógica para relatórios
}

void sair() {
    // Implementar lógica para sair
}

void clear() {
    #ifdef _WIN32
        system("cls"); // Comando para limpar tela no Windows
    #else
        system("clear"); // Comando para limpar tela no Unix/Linux/macOS
    #endif
}

int main() {
    menuPrincipal();

    return 0;
}
