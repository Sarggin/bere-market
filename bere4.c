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


// Structs do sistema, Matrizes heterogêneas
typedef struct {
    int id;
    char nome[50];
} Menu;

typedef struct {
    char *login;
    char *password;
    int type; // 1 para Administrador, 2 para Usuário
} Usuario;

typedef struct {
    int id;
    char *nome;
    char *nomeSocial;
    char *cpf;
    char *rua;
    int numero;
    char *celular;
    char *data;
} Clientes;

typedef struct {
  int codigo;
  char *descricao;
  char *categoria;
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

typedef struct {
    int id;
    char *nome;
} Relatorios;

typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

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
void carrinho(Carrinho carrinho[], int numItens);
void sangria();
void pagamento();
void aberturaCaixa();
void permisaoCaixa();
void fechaCaixa();
void resetVariavelGlobal();
void relatorios();
void sair();
void clear();
Data dataAtual();

void opcoesRelatorio(int opcao);
void relatorioClientes();
void relatorioProdutos();
void relatorioVendas();
void listagemClientesAlfabetica();
void listagemClientesPeriodo();
Relatorios* buscarRelatorio(Relatorios relatorios[], int tamanho, int opcao);

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

    int escolha;
    do {
        printf("\n========================================\n");
        printf("         SEJA BEM-VINDO(A) AO BERE\n");
        printf("         ESCOLHA UMA OPCAO ABAIXO\n");
        printf("========================================\n");

        for (int i = 0; i < 6; i++) {
            printf(" %d - %s\n", principal[i].id, principal[i].nome);
            if (i < 5) {
                printf("----------------------------------------\n");
            }
        }

        printf("========================================\n");

        escolha = opcaoEscolhida();

        if (escolha < 1 || escolha > 6) {
            clear();
            printf("\nOpcao invalida! Por favor, escolha uma opcao valida.\n");
        } else {
            opcoes(escolha);
        }

    } while (escolha != 6);
}

// Função para capturar a opção escolhida pelo usuário
int opcaoEscolhida() {
    int opcao;

    printf("\nDigite a opcao desejada: ");
    scanf("%d", &opcao);

    return opcao;
}

// Função para processar as opções do menu principal
void opcoes(int opcao) {
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
}

void cadastro(){
    clear();

    permisaoCaixa();

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

// Função auxiliar para liberar memória
void liberarMemoriaUsuario (Usuario *usuario){
    // Liberar a memória alocada
    free(usuario->login);
    free(usuario->password);
    free(usuario);
}

void cadastroUsuario() {
    FILE *file;
    Usuario *usuario = (Usuario *)malloc(sizeof(Usuario));
    int controlador = 0;

    // Alocar memória para login e password
    usuario->login = (char *)malloc(TAMANHO_MAXIMO_USUARIO * sizeof(char));
    usuario->password = (char *)malloc(TAMANHO_MAXIMO_SENHA * sizeof(char));

    if (usuario == NULL || usuario->login == NULL || usuario->password == NULL) {
        printf("Erro de alocação de memória.\n");
        return;
    }

    // Abrir o arquivo em modo append para adicionar usuários ao final do arquivo
    file = fopen("usuarios.txt", "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de usuários.\n");
        free(usuario->login);
        free(usuario->password);
        free(usuario);
        return;
    }

    // Coletar dados do usuário com tratamento de tamanho para o login
    while (!controlador) {
        printf("\nDigite o login do usuario (min %d caracteres e max %d caracteres): ", TAMANHO_MINIMO_USUARIO, TAMANHO_MAXIMO_USUARIO);
        scanf(" %[^\n]", usuario->login);

        // Remover o newline do final da string, se houver
        usuario->login[strcspn(usuario->login, "\n")] = '\0';

        if (strlen(usuario->login) >= TAMANHO_MINIMO_USUARIO && strlen(usuario->login) <= TAMANHO_MAXIMO_USUARIO) {
            controlador = 1;
        } else {
            printf("Login invalido. O login deve ter entre %d e %d caracteres.\n", TAMANHO_MINIMO_USUARIO, TAMANHO_MAXIMO_USUARIO);
        }
    }

    // Reseta o controlador para a senha
    controlador = 0;
    while (!controlador) {
        printf("Digite a senha do usuario (min %d caracteres e max %d caracteres): ", TAMANHO_MINIMO_SENHA, TAMANHO_MAXIMO_SENHA);
        scanf(" %[^\n]", usuario->password);

        // Remover o newline do final da string, se houver
        usuario->password[strcspn(usuario->password, "\n")] = '\0';

        if (strlen(usuario->password) >= TAMANHO_MINIMO_SENHA && strlen(usuario->password) <= TAMANHO_MAXIMO_SENHA) {
            controlador = 1;
        } else {
            printf("\nSenha invalida. Deve ter entre %d e %d caracteres.\n", TAMANHO_MINIMO_SENHA, TAMANHO_MAXIMO_SENHA);
        }
    }

    // Coletar o tipo de usuário
    printf("Digite o tipo de usuario (1 para Administrador, 2 para Usuario): ");
    scanf("%d", &usuario->type);

    // Limpar o buffer de entrada
    while (getchar() != '\n'); // Limpar o buffer de entrada

    // Escrever os dados do usuário no arquivo
    fprintf(file, "%s %s %d\n", usuario->login, usuario->password, usuario->type);

    // Fechar o arquivo
    fclose(file);

    // Liberar a memória alocada
    liberarMemoriaUsuario(usuario);

    printf("Usuario cadastrado com sucesso!\n");

    system("pause");

    // Chama a próxima função desejada
    cadastro();
}

// Função auxiliar para liberar memória
void liberarMemoriaCliente(Clientes *cliente) {
    free(cliente->nome);
    free(cliente->nomeSocial);
    free(cliente->cpf);
    free(cliente->rua);
    free(cliente->celular);
    free(cliente->data);
    free(cliente); // Libera a estrutura em si
}

void cadastroCliente() {
    FILE *arquivo;
    Clientes *cliente = (Clientes *)malloc(sizeof(Clientes)); // Aloca memória para a estrutura
    size_t len = 0;

    // Verifica se a alocação da estrutura foi bem-sucedida
    if (cliente == NULL) {
        printf("Erro de alocação de memória para a estrutura cliente.\n");
        return;
    }

    // Inicializa ponteiros da estrutura com NULL
    cliente->nome = NULL;
    cliente->nomeSocial = NULL;
    cliente->cpf = NULL;
    cliente->rua = NULL;
    cliente->celular = NULL;
    cliente->data = NULL;

    // Abrindo o arquivo no modo "a" (anexar)
    arquivo = fopen("clientes.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de clientes.\n");
        liberarMemoriaCliente(cliente);
        return;
    }

    // Obtendo dados do cliente
    printf("Informe o ID do cliente: ");
    scanf("%d", &cliente->id);
    getchar(); // Limpar o buffer de entrada

    printf("Informe o nome do cliente: ");
    getline(&cliente->nome, &len, stdin);
    cliente->nome[strcspn(cliente->nome, "\n")] = '\0'; // Remover o newline do final da string

    printf("Informe o nome social do cliente: ");
    getline(&cliente->nomeSocial, &len, stdin);
    cliente->nomeSocial[strcspn(cliente->nomeSocial, "\n")] = '\0';

    printf("Informe o CPF do cliente: ");
    getline(&cliente->cpf, &len, stdin);
    cliente->cpf[strcspn(cliente->cpf, "\n")] = '\0';

    if (strlen(cliente->cpf) != 11) { // Validação do tamanho do CPF
        printf("Erro: CPF deve ter 11 dígitos.\n");
        liberarMemoriaCliente(cliente);
        fclose(arquivo);
        return;
    }

    printf("Informe a rua do cliente: ");
    getline(&cliente->rua, &len, stdin);
    cliente->rua[strcspn(cliente->rua, "\n")] = '\0';

    printf("Informe o número da casa do cliente: ");
    scanf("%d", &cliente->numero);
    getchar(); // Limpar o buffer de entrada

    printf("Informe o celular do cliente: ");
    getline(&cliente->celular, &len, stdin);
    cliente->celular[strcspn(cliente->celular, "\n")] = '\0';

    if (strlen(cliente->celular) != 11) { // Valida o tamanho do Celular
        printf("Erro: Celular deve ter 11 dígitos.\n");
        liberarMemoriaCliente(cliente);
        fclose(arquivo);
        return;
    }

    printf("Informe a data de nascimento do cliente (dd/mm/aaaa): ");
    getline(&cliente->data, &len, stdin);
    cliente->data[strcspn(cliente->data, "\n")] = '\0';

    if (strlen(cliente->data) != 10) { //Valida a data informada
        printf("Erro: Data deve estar no formato dd/mm/aaaa.\n");
        liberarMemoriaCliente(cliente);
        fclose(arquivo);
        return;
    }

    // Escrevendo os dados do cliente no arquivo
    fprintf(arquivo, "%d %s %s %s %s %d %s %s\n", cliente->id, cliente->nome, cliente->nomeSocial, cliente->cpf, cliente->rua, cliente->numero, cliente->celular, cliente->data);

    // Fechando o arquivo
    fclose(arquivo);

    // Liberando a memória alocada
    liberarMemoriaCliente(cliente);

    printf("Cliente cadastrado com sucesso!\n");

    system("pause");

    cadastro();
}

// Função auxiliar para liberar memória
void liberarMemoriaProduto(Produtos *produto){
    free(produto);
}

void cadastroProduto() {
    FILE *arquivo;
    Produtos *produto = (Produtos *)malloc(sizeof(Produtos)); // Aloca memória para a estrutura

    if (produto == NULL) {
        printf("Erro de alocação de memória para a estrutura de produto.\n");
        return;
    }

    // Abrindo o arquivo no modo "a" (anexar)
    arquivo = fopen("produtos.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de produtos.\n");
        free(produto); // Libera a memória alocada para a estrutura de produtos
        return;
    }

    // Alocando memória para descricao e categoria
    produto->descricao = (char *)malloc(100 * sizeof(char));
    produto->categoria = (char *)malloc(50 * sizeof(char));

    if (produto->descricao == NULL || produto->categoria == NULL) {
        printf("Erro de alocação de memória para descricao ou categoria.\n");
        fclose(arquivo);
        liberarMemoriaProduto(produto);
        return;
    }

    // Obtendo dados do produto do usuário
    printf("Informe o codigo do produto: ");
    scanf("%d", &produto->codigo);

    printf("Informe a descricao do produto: ");
    fgets(produto->descricao, 100, stdin);

    // Validações básicas
    if (strlen(produto->descricao) == 0) {
        printf("Erro: Descrição não pode ser vazia.\n");
        fclose(arquivo);
        liberarMemoriaProduto(produto);
        return;
    }

    printf("Informe a categoria do produto (Alimento, Material de Limpeza, Panificacao): ");
    fgets(produto->categoria, 50, stdin);

    printf("Informe o preco de compra do produto: ");
    scanf("%f", &produto->precoCompra);

    if (produto->precoCompra <= 0.0f) {
        printf("Erro: Preco de compra deve ser maior que zero.\n");
        fclose(arquivo);
        liberarMemoriaProduto(produto);
        return;
    }

    printf("Informe a margem de lucro (em porcentagem): ");
    scanf("%f", &produto->margemLucro);

    printf("Informe a quantidade em estoque: ");
    scanf("%d", &produto->quantidadeEstoque);

    printf("Informe a quantidade de estoque minimo: ");
    scanf("%d", &produto->estoqueMinimo);

    // Calculando o preço de venda com base na margem de lucro
    produto->precoVenda = produto->precoCompra * (1.0f + produto->margemLucro / 100.0f);

    // Escrevendo os dados do produto no arquivo
    fprintf(arquivo, "%d %s %s %.2f %.2f %.2f %d %d\n", produto->codigo, produto->descricao, produto->categoria, 
            produto->precoCompra, produto->margemLucro, produto->precoVenda, produto->quantidadeEstoque, produto->estoqueMinimo);

    // Fechando o arquivo
    fclose(arquivo);

    // Liberando memória alocada
    liberarMemoriaProduto(produto);

    printf("Produto cadastrado com sucesso!\n");
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

    permisaoCaixa();

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
    file = fopen("vendas.txt", "a"); // Abrir arquivo para escrita

    if (file == NULL) {
        printf("Erro ao abrir o arquivo.");
        return;
    }

    fprintf(file, "** Documento de Venda **\n\n");
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

Data dataAtual() {
    time_t mytime;
    mytime = time(NULL);
    struct tm tm = *localtime(&mytime);
    
    Data data;
    data.dia = tm.tm_mday;
    data.mes = tm.tm_mon + 1;
    data.ano = tm.tm_year + 1900;

    return data;
}

void carrinho(Carrinho carrinho[], int numItens){
    for (int i = 0; i < numItens; i++) {
        printf("Item %d:\n", i + 1);
        printf("Codigo: %d\n", carrinho[i].codigo);
        printf("Descricao: %s\n", carrinho[i].descricao);
        printf("Preco Unitario: R$ %.2f\n", carrinho[i].precoVenda);
        printf("Quantidade: %d\n", carrinho[i].quantidade);
        printf("Total: R$ %.2f\n\n", carrinho[i].total);
    }
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
    
    printf("\nCaixa Fechado com sucesso");
    system("pause");

    menuPrincipal();
}

void permisaoCaixa(){
    char escolha;

    if (statusCaixa == 0) {
        printf("O caixa nao esta aberto. Gostaria de abrir? (s ou n) ");
        scanf(" %c", &escolha); // Note o espaço antes de %c para ignorar espaços em branco e novas linhas

        if (escolha == 's' || escolha == 'S') {
            aberturaCaixa();
        } else {
            // Código para não abrir o caixa
            printf("Operação cancelada. O caixa continua fechado.\n");
            menuPrincipal();
        }
    }
}

void relatorios() {
    clear();
    int opcao;

    Relatorios relatorioMenu[] = {
        {1, "Listagem dos Clientes"},
        {2, "Listagem dos Produtos"},
        {3, "Listagem das Vendas"},
        {4, "Retornar ao Menu Principal"},
    };

    printf("\nRelatorios\n");
    printf("\nSelecione uma das opcoes abaixo:\n\n");

    for (int i = 0; i < 4; i++){
        printf("%d - %s\n", relatorioMenu[i].id, relatorioMenu[i].nome);
    }

    printf("\nDigite a opcao desejada: ");
    scanf("%d", &opcao);

    // Busca a opção selecionada
    Relatorios* relatorioSelecionado = buscarRelatorio(relatorioMenu, 4, opcao);
    if (relatorioSelecionado != NULL) {
        printf("\nVocê selecionou: %s\n", relatorioSelecionado->nome);
        opcoesRelatorio(opcao);
    } else {
        printf("\nOpcao invalida, digite novamente.\n");
        relatorios();
    }
}

// Função para buscar uma opção no array de Relatorios
Relatorios* buscarRelatorio(Relatorios relatorios[], int tamanho, int opcao) {
    for (int i = 0; i < tamanho; i++) {
        if (relatorios[i].id == opcao) {
            return &relatorios[i];
        }
    }
    return NULL; // Retorna NULL se não encontrar a opção
}

void opcoesRelatorio(int opcao){
    if (opcao < 1 || opcao > 4){
        printf("\nOpcao invalida, digite novamente.\n");
        relatorios();
        return;
    } 

    switch (opcao) {
    case 1:
        relatorioClientes();
        break;
    case 2:
        relatorioProdutos();
        break;
    case 3:
        relatorioVendas();
        break;
    case 4:
        menuPrincipal();
        break;
    default:
        printf("\nOpcao invalida, digite novamente.\n");
        break;
    }
}

void relatorioClientes(){
    

}

void relatorioProdutos(){
    
    

}

void relatorioVendas() {
    clear();
    int opcao;

    Relatorios vendas[] = {
        {1, "Listagem das Vendas (em um determinado periodo)"},
        {2, "Faturamento Consolidado - em um periodo"},
        {3, "Voltar"}
    };

    printf("\nRelatorio de Vendas\n");
    for (size_t i = 0; i < 3; i++){
        printf("%d - %s\n", vendas[i].id, vendas[i].nome);
    }

    printf("\nDigite a opcao desejada: ");
    scanf("%d", &opcao);

    Relatorios* vendaSelecionada = buscarRelatorio(vendas, 3, opcao);
    if (vendaSelecionada != NULL) {
        switch (opcao) {
        case 1:
            listagemClientesPeriodo();
            break;
        case 2:
            listagemClientesAlfabetica();
            break;
        case 3:
            return;
        default:
            printf("\nOpcao invalida, digite novamente\n");
            relatorioVendas();
            break;
        }
    } else {
        printf("\nOpcao invalida, digite novamente\n");
        relatorioVendas();
    }
}

void listagemClientesPeriodo(){
    char periodo[10];

    printf("\nDigite o periodo desejado no seguinte formato dd/mm/yyyy: ");
    scanf(" %[^\n]", periodo);
}

void listagemClientesAlfabetica(){

}



void resetVariavelGlobal(){
    dinheiroCaixa= 0;
    statusCaixa = 0;
    numCaixa = 0;
}

void sair() {
    exit(0);
}

void clear() {
    #ifdef _WIN32
        system("cls"); // Comando para limpar tela no Windows
    #else
        system("clear"); // Comando para limpar tela no Unix/Linux/macOS
    #endif
}

void main() {
    menuPrincipal();
}