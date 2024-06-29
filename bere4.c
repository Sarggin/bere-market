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
int numItensCarrinho, numCaixa = 0;


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
    char tipo; // 'd' para dinheiro, 'c' para cartão, etc.
    float valor;
} Pagamento;

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
  char *nome;
} Categorias;

typedef struct {
    int codigo;
    char *descricao; 
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

typedef struct {
    int codigo;
    char *descricao;
    float precoVenda;
    int quantidade;
    float total;
} ItemCarrinho;

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
void carrinho(Carrinho carrinho[], int numItens);
void sangria();
void pagamento(Carrinho carrinho[], float totalCarrinho);
void relatorios();
void aberturaCaixa();
void permisaoCaixa();
void fechaCaixa();
void resetVariavelGlobal();
void sair();
void clear();
Data dataAtual();
float calcularTotalCarrinho(Carrinho carrinho[], int numItens);
void realizarPagamentoDinheiro(float total);
void realizarPagamentoCartao(float total);
void realizarPagamentoMisto(float total);
void listaCliente();
void clienteAlfabetica();
void clientePeriodo();

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

    // Abrir o arquivo em modo append para adicionar usuários ao final do arquivo
    file = fopen("usuarios.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de usuários.\n");
        free(usuario->login);
        free(usuario->password);
        free(usuario);
        return;
    }

    // Alocar memória para login e password
    usuario->login = (char *)malloc(TAMANHO_MAXIMO_USUARIO * sizeof(char));
    usuario->password = (char *)malloc(TAMANHO_MAXIMO_SENHA * sizeof(char));

    if (usuario == NULL || usuario->login == NULL || usuario->password == NULL) {
        printf("Erro de alocação de memória.\n");
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

    // Abrindo o arquivo no modo "a" (anexar)
    arquivo = fopen("clientes.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de clientes.\n");
        liberarMemoriaCliente(cliente);
        return;
    }

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

    printf("Informe o numero da casa do cliente: ");
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
    free(produto->descricao);
    free(produto->categoria);
    free(produto);
}

void cadastroProduto() {
    FILE *arquivo;
    Produtos *produto = (Produtos *)malloc(sizeof(Produtos)); // Aloca memória para a estrutura
    size_t len = 0;

    // Abrindo o arquivo no modo "a" (anexar)
    arquivo = fopen("produtos.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de produtos.\n");
        free(produto); // Libera a memória alocada para a estrutura de produtos
        return;
    }

    if (produto == NULL) {
        printf("Erro de alocação de memória para a estrutura de produto.\n");
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
    scanf("\n%d", &produto->codigo);
    getchar(); // Limpa o buffer

    printf("Informe a descricao do produto: ");
    getline(&produto->descricao, &len, stdin);
    produto->descricao[strcspn(produto->descricao, "\n")] = '\0';

    // Validações básicas
    if (strlen(produto->descricao) == 0) {
        printf("Erro: Descrição não pode ser vazia.\n");
        fclose(arquivo);
        liberarMemoriaProduto(produto);
        return;
    }

    printf("Informe a categoria do produto (Alimento, Material de Limpeza, Panificacao): ");
    fgets(produto->categoria, 50, stdin);
    produto->categoria[strcspn(produto->categoria, "\n")] = '\0';

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

    Carrinho carrinho[50]; // Supondo que o carrinho terá no máximo 50 itens
    float totalCarrinho = 0.0;

    switch (opcao) {
    case 1:
        novaVenda();
        break;
    case 2:
        sangria();
        break;
    case 3:
        pagamento(carrinho, totalCarrinho);
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

void exibirProduto(Produtos produto) {
    printf("%-8d | %-17s | %-17s | R$ %7.2f | %3d\n",
           produto.codigo,
           produto.descricao,
           produto.categoria,
           produto.precoVenda,
           produto.quantidadeEstoque);
}

void rodape(){
    printf("-------------------------------------------------------------------\n");
}

// Função de comparação para qsort
int comparaProdutos(const void *a, const void *b) {
    Produtos *produtoA = (Produtos *)a;
    Produtos *produtoB = (Produtos *)b;
    return produtoA->codigo - produtoB->codigo;
}

int carregarProdutos(Produtos **listaProdutos, int *maxProdutos) {
    FILE *arquivo;
    int contador = 0;
    int capacidade = *maxProdutos;

    // Produtos fixos
    Produtos produtosFixos[] = {
        {1000, "Cafe", "Alimento", 0, 0, 7.70, 10, 0},
        {1001, "Esponja", "Material Limpeza", 0, 0, 2.99, 30, 0},
        {1002, "Biscoito doce", "Panificacao", 0, 0, 12.50, 5, 0}
    };

    int tamanhoProduto = sizeof(produtosFixos) / sizeof(produtosFixos[0]);

    // Copiar produtos fixos para a lista dinâmica, se houver capacidade
    for (int i = 0; i < tamanhoProduto; ++i) {
        if (contador >= capacidade) {
            capacidade *= 2;
            *listaProdutos = realloc(*listaProdutos, capacidade * sizeof(Produtos));
        }
        Produtos *produto = &(*listaProdutos)[contador];
        produto->codigo = produtosFixos[i].codigo;
        produto->descricao = strdup(produtosFixos[i].descricao);
        produto->categoria = strdup(produtosFixos[i].categoria);
        produto->precoCompra = produtosFixos[i].precoCompra;
        produto->margemLucro = produtosFixos[i].margemLucro;
        produto->precoVenda = produtosFixos[i].precoVenda;
        produto->quantidadeEstoque = produtosFixos[i].quantidadeEstoque;
        produto->estoqueMinimo = produtosFixos[i].estoqueMinimo;
        contador++;
    }

    // Abrindo o arquivo no modo leitura
    arquivo = fopen("produtos.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de produtos.\n");
        return contador; // Retorna o número de produtos carregados
    }

    // Lendo os produtos do arquivo e armazenando no array
    while (!feof(arquivo)) {
        if (contador >= capacidade) {
            capacidade *= 2;
            *listaProdutos = realloc(*listaProdutos, capacidade * sizeof(Produtos));
        }
        Produtos *produto = &(*listaProdutos)[contador];
        produto->descricao = (char *)malloc(100 * sizeof(char));
        produto->categoria = (char *)malloc(50 * sizeof(char));
        fscanf(arquivo, "%d %99s %49s %f %f %f %d %d",
               &produto->codigo,
               produto->descricao,
               produto->categoria,
               &produto->precoCompra,
               &produto->margemLucro,
               &produto->precoVenda,
               &produto->quantidadeEstoque,
               &produto->estoqueMinimo);
        contador++;
    }

    // Fechando o arquivo
    fclose(arquivo);

    *maxProdutos = capacidade;

    qsort(*listaProdutos, contador, sizeof(Produtos), comparaProdutos);

    return contador; // Retorna o número de produtos carregados
}

// Função para salvar documento de venda
void documentoVenda(Carrinho *carrinho, int numItensCarrinho) {
    FILE *file = fopen("vendas.txt", "a"); // Abrir arquivo para escrita

    if (file == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    Data data = dataAtual();
    fprintf(file, "Data da Venda: %02d/%02d/%04d\n", data.dia, data.mes, data.ano);
    fprintf(file, "Produtos Comprados:\n");
    for (int i = 0; i < numItensCarrinho; i++) {
        fprintf(file, "Codigo: %d\n", carrinho[i].codigo);
        fprintf(file, "Descricao: %s\n", carrinho[i].descricao);
        fprintf(file, "Preco Unitario: %.2f\n", carrinho[i].precoVenda);
        fprintf(file, "Quantidade: %d\n", carrinho[i].quantidade);
        fprintf(file, "Total: %.2f\n\n", carrinho[i].total);
    }

    fclose(file); // Fechar arquivo após escrita
    printf("\nDocumento de venda gerado com sucesso.\n");
}

// Função para salvar documento de venda
void documentoCarrinho(Carrinho *carrinho, int numItensCarrinho) {
    FILE *file = fopen("carrinho.txt", "a"); // Abrir arquivo para escrita

    if (file == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    Data data = dataAtual();
    fprintf(file, "\nData da Venda: %02d/%02d/%04d\n", data.dia, data.mes, data.ano);
    fprintf(file, "Produtos no Carrinho:\n");
    for (int i = 0; i < numItensCarrinho; i++) {
        fprintf(file, "Codigo: %d\n", carrinho[i].codigo);
        fprintf(file, "Descricao: %s\n", carrinho[i].descricao);
        fprintf(file, "Preco Unitario: %.2f\n", carrinho[i].precoVenda);
        fprintf(file, "Quantidade: %d\n", carrinho[i].quantidade);
        fprintf(file, "Total: %.2f\n", carrinho[i].total);
    }

    fclose(file); // Fechar arquivo após escrita
    printf("\nDocumento de venda gerado com sucesso.\n");
}

void novaVenda() {
    clear();
    int quantidadeProdutos = 50;
    Produtos *produtos = (Produtos *)malloc(quantidadeProdutos * sizeof(Produtos));
    
    if (produtos == NULL) {
        printf("Erro ao alocar memória para produtos.\n");
        return;
    }

    int numProdutos = carregarProdutos(&produtos, &quantidadeProdutos);

    int codigoCompra;
    int quantidade;
    char continuarCompra = 's';

    Carrinho *carrinho = (Carrinho *)malloc(quantidadeProdutos * sizeof(Carrinho));
    if (carrinho == NULL) {
        printf("Erro ao alocar memória para carrinho.\n");
        free(produtos);
        return;
    }
    int numItensCarrinho = 0;

    while (continuarCompra == 's' || continuarCompra == 'S') {
        clear();    
        // Exibindo cabeçalho e lista de produtos disponíveis
        exibirCabecalho();
        for (int i = 0; i < numProdutos; i++) {
            exibirProduto(produtos[i]);
        }
        rodape();

        int encontrado = 0;
        
        printf("\nCarrinho de Compras\n");
        printf("\nInforme o codigo do produto a ser comprado: ");
        scanf("%d", &codigoCompra);

        encontrado = 0;
        for (int i = 0; i < numProdutos; i++) {
            if (produtos[i].codigo == codigoCompra) {
                encontrado = 1;
                printf("\nInforme a quantidade: ");
                scanf("%d", &quantidade);

                if (produtos[i].quantidadeEstoque < quantidade) {
                    printf("Quantidade em estoque insuficiente.\n");
                    break;
                }

                carrinho[numItensCarrinho].codigo = produtos[i].codigo;
                carrinho[numItensCarrinho].descricao = strdup(produtos[i].descricao);
                carrinho[numItensCarrinho].precoVenda = produtos[i].precoVenda;
                carrinho[numItensCarrinho].quantidade = quantidade;
                carrinho[numItensCarrinho].total = produtos[i].precoVenda * quantidade;

                produtos[i].quantidadeEstoque -= quantidade;
                if (produtos[i].quantidadeEstoque <= produtos[i].estoqueMinimo) {
                    printf("Estoque mínimo atingido. Deseja continuar a compra? (s/n): ");
                    char resposta;
                    scanf(" %c", &resposta);
                    if (resposta != 's' && resposta != 'S') {
                        continuarCompra = 'n';
                        break;
                    }
                }

                printf("\nProduto adicionado ao carrinho.\n");
                printf("Descricao: %s\n", produtos[i].descricao);
                printf("Preco Unitario: R$ %.2f\n", produtos[i].precoVenda);
                printf("Quantidade: %d\n", quantidade);
                printf("Total: R$ %.2f\n", produtos[i].precoVenda * quantidade);

                numItensCarrinho++;
                break;
            }
        }

        if (!encontrado) {
            printf("Produto não encontrado.\n");
            continue;
        }

        printf("\nNovo item no carrinho de compra (s/n): ");
        scanf(" %c", &continuarCompra);
    }

    // Gerar documento de venda
    documentoVenda(carrinho, numItensCarrinho);
    documentoCarrinho(carrinho, numItensCarrinho);

    free(produtos);
    free(carrinho);

    printf("Venda concluída com sucesso!\n");
    menuVendas();
}

// Função para inicializar os produtos dinâmicos
int produtosFixos(Produtos listaProdutos[], int indice) {
    Produtos produtosFixos[] = {
        {1000, "Cafe", "Alimento", 0, 0, 7.70, 10, 0},
        {1001, "Esponja", "Material Limpeza", 0, 0, 2.99, 30, 0},
        {1002, "Biscoito doce", "Panificacao", 0, 0, 12.50, 5, 0}
    };
    int numDinamicos = sizeof(produtosFixos) / sizeof(produtosFixos[0]);

    for (int i = 0; i < numDinamicos; i++) {
        listaProdutos[indice++] = produtosFixos[i];
    }

    return indice; // Retorna o novo índice após adicionar os produtos dinâmicos
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

// Função para calcular o total em dinheiro disponível no caixa
float calcularTotalDinheiro(Pagamento pagamentos[], int numPagamentos) {
    for (int i = 0; i < numPagamentos; i++) {
        if (pagamentos[i].tipo == 'd' || pagamentos[i].tipo == 'md') {
            dinheiroCaixa += pagamentos[i].valor;
        }
    }
    return dinheiroCaixa;
}

void sangria(){
 // Verificar se o usuário logado é do tipo 1 (ADMIN)
    int tipoUsuario; // Exemplo: 1 para ADMIN
    printf("\nQual seu tipo de usuario: ");
    scanf("%d", &tipoUsuario);

    if (tipoUsuario != 1) {
        // Solicitar login e senha de administrador
        printf("Para realizar a sangria, eh necessario autenticacao de administrador.\n");
        // Aqui seria implementada a lógica de autenticação
        return;
    }

    // Supondo uma estrutura de pagamentos

    int tamanhoPagamento = sizeof(Pagamento);

    Pagamento pagamentos[tamanhoPagamento]; // Ajuste o tamanho conforme necessário

    int numPagamentos = 0; // Número de pagamentos registrados

    // Exemplo de cálculo do total em dinheiro
    float totalDinheiro = calcularTotalDinheiro(pagamentos, numPagamentos);

    // Apresentar o total em dinheiro para a Dona Berê
    printf("\nTotal em dinheiro no caixa: R$ %.2f\n", totalDinheiro);

    // Solicitar o valor da retirada da sangria
    float valorRetirada;
    printf("\nInforme o valor da retirada (deve ser menor que o valor presente no caixa, deixando troco minimo de 50 reias): ");
    scanf("%f", &valorRetirada);

    // Validar se o valor da retirada é maior que o valor presente no caixa
    if (valorRetirada >= totalDinheiro) {
        printf("\nValor da retirada nao pode ser igual ou maior que o valor presente no caixa.\n");
        return;
    }

    // Calcular o troco mínimo necessário (exemplo mínimo de R$ 50)
    float trocoMinimo = 50.0;
    float troco = valorRetirada - totalDinheiro;

    if (troco >= trocoMinimo) {
        printf("\nPara a retirada de %.2f, deve sobrar um troco minimo de R$ %.2f.\n", valorRetirada, trocoMinimo);
        return;
    }

    // Atualizar o valor do caixa após a sangria
    dinheiroCaixa -= valorRetirada;

    // Mensagem de confirmação
    printf("\nSangria de R$ %.2f realizada com sucesso. Troco disponivel: R$ %.2f\n", valorRetirada, dinheiroCaixa);
}

float carregaCarrinhoAtual(Carrinho carrinho[], float total) {
    FILE *arquivo;

    arquivo = fopen("carrinho.txt", "r");

    if (arquivo == NULL) {
        printf("\nErro ao abrir arquivo para calcular o total do carrinho.\n");
        return 0;
    }

    char linha[100];
    char descricaoTemp[100];

    // Ignorar a primeira linha (Data da Venda)
    fgets(linha, sizeof(linha), arquivo); // Descarta a linha

    // Ignorar a linha "Produtos no Carrinho:"
    fgets(linha, sizeof(linha), arquivo); // Descarta a linha

    int i = 0;
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (strstr(linha, "Codigo: ") != NULL) {
            sscanf(linha, "Codigo: %d", &carrinho[i].codigo);
        }
        else if (strstr(linha, "Descricao: ") != NULL) {
            sscanf(linha, "Descricao: %[^\n]", descricaoTemp);
            carrinho[i].descricao = strdup(descricaoTemp);
        }
        else if (strstr(linha, "Preco Unitario: ") != NULL) {
            sscanf(linha, "Preco Unitario: %f", &carrinho[i].precoVenda);
        }
        else if (strstr(linha, "Quantidade: ") != NULL) {
            sscanf(linha, "Quantidade: %d", &carrinho[i].quantidade);
        }
        else if (strstr(linha, "Total: ") != NULL) {
            sscanf(linha, "Total: %f", &carrinho[i].total);
            total += carrinho[i].total;
            i++;
        }
    }

    fclose(arquivo);
    return total;
}

void pagamento(Carrinho carrinho[], float totalCarrinho) {
    totalCarrinho = carregaCarrinhoAtual(carrinho, totalCarrinho);
    
    printf("\nMenu Pagamento\n");
    printf("\nComo deseja pagar o total de R$ %.2f\n", totalCarrinho);

    system("pause");
}

float calcularTotalCarrinho(Carrinho carrinho[], int numItens) {
    float total = 0.0;

    for (int i = 0; i < numItens; i++) {
        total += carrinho[i].total;
    }

    return total;
}

void realizarPagamentoDinheiro(float total) {
    float valorPago, troco;

    printf("Total a pagar: R$ %.2f\n", total);
    printf("Digite o valor pago: R$ ");
    scanf("%f", &valorPago);

    while (valorPago < total) {
        printf("\nValor insuficiente! Digite um valor maior ou igual a R$ %.2f: R$ ", total);
        scanf("%f", &valorPago);
    }

    troco = valorPago - total;
    printf("Troco: R$ %.2f\n", troco);
    system("pause");
    menuVendas();
}

// Função para pagamento com cartão
void realizarPagamentoCartao(float total) {
    printf("\nTotal a pagar: R$ %.2f\n", total);
    printf("\nPagamento realizado com sucesso!\n");
    system("pause");
    menuVendas();
}

// Função para pagamento misto
void realizarPagamentoMisto(float total) {
    float valorPagoDinheiro, valorPagoCartao, valorRestante;

    printf("Total a pagar: R$ %.2f\n", total);
    printf("Digite o valor pago em dinheiro: R$ ");
    scanf("%f", &valorPagoDinheiro);

    valorRestante = total - valorPagoDinheiro;

    printf("Valor restante a pagar no cartao: R$ %.2f\n", valorRestante);
    printf("Digite o valor pago no cartao: R$ ");
    scanf("%f", &valorPagoCartao);

    while (valorPagoCartao < valorRestante) {
        printf("Valor insuficiente no cartao! Digite um valor maior ou igual a R$ %.2f: R$ ", valorRestante);
        scanf("%f", &valorPagoCartao);
    }

    printf("Pagamento misto realizado com sucesso!\n");
    system("pause");
    menuVendas();
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

// Função para ler clientes do arquivo e armazenar em um array dinâmico
int lerClientes(Clientes **clientes) {
    FILE *arquivo = fopen("clientes.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    int capacidade = 10;
    int contador = 0;
    *clientes = malloc(capacidade * sizeof(Clientes));
    if (*clientes == NULL) {
        printf("Erro ao alocar memória.\n");
        fclose(arquivo);
        return 0;
    }

    char buffer[100]; // Buffer para ler uma linha do arquivo

    // Leitura dos clientes do arquivo
    while (fgets(buffer, sizeof(buffer), arquivo) != NULL) {
        if (contador >= capacidade) {
            capacidade += 10;
            *clientes = realloc(*clientes, capacidade * sizeof(Clientes));
            if (*clientes == NULL) {
                printf("Erro ao realocar memória.\n");
                fclose(arquivo);
                return contador;
            }
        }

        // Aloca memória para cada campo de string na estrutura Clientes
        (*clientes)[contador].nome = malloc(100 * sizeof(char));
        (*clientes)[contador].nomeSocial = malloc(100 * sizeof(char));
        (*clientes)[contador].cpf = malloc(20 * sizeof(char));
        (*clientes)[contador].rua = malloc(100 * sizeof(char));
        (*clientes)[contador].celular = malloc(20 * sizeof(char));
        (*clientes)[contador].data = malloc(20 * sizeof(char));

        // Lê os dados do buffer
        sscanf(buffer, "%d %99[^,], %99[^,], %19[^,], %99[^,], %d %19[^,], %19s",
               &(*clientes)[contador].id,
               (*clientes)[contador].nome,
               (*clientes)[contador].nomeSocial,
               (*clientes)[contador].cpf,
               (*clientes)[contador].rua,
               &(*clientes)[contador].numero,
               (*clientes)[contador].celular,
               (*clientes)[contador].data);

        contador++;
    }

    fclose(arquivo);
    return contador;
}

// Função de comparação para qsort por nome
int comparaClientes(const void *a, const void *b) {
    const Clientes *clienteA = (const Clientes *)a;
    const Clientes *clienteB = (const Clientes *)b;
    return strcmp(clienteA->nome, clienteB->nome);
}

// Função para ordenar clientes alfabeticamente e salvar no arquivo
void clienteAlfabetica(const char *nomeArquivo) {
    Clientes *clientes = NULL;
    int numClientes = lerClientes(&clientes);

    if (numClientes == 0) {
        printf("Não há clientes para ordenar.\n");
        return;
    }

    // Ordena os clientes pelo nome
    qsort(clientes, numClientes, sizeof(Clientes), comparaClientes);

    // Imprime os nomes ordenados
    printf("Nomes dos clientes ordenados:\n");
    for (int i = 0; i < numClientes; i++) {
        printf("\n%s", clientes[i].nome);
    }

    // Abre o arquivo para escrita
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s para escrita.\n", nomeArquivo);
        free(clientes);
        return;
    }

    // Escreve os clientes ordenados no arquivo
    for (int i = 0; i < numClientes; i++) {
        fprintf(arquivo, "%d, %s, %s, %s, %s, %d, %s, %s\n",
                clientes[i].id,
                clientes[i].nome,
                clientes[i].nomeSocial,
                clientes[i].cpf,
                clientes[i].rua,
                clientes[i].numero,
                clientes[i].celular,
                clientes[i].data);
    }

    // Fecha o arquivo e libera a memória alocada
    fclose(arquivo);
    for (int i = 0; i < numClientes; i++) {
        free(clientes[i].nome);
        free(clientes[i].nomeSocial);
        free(clientes[i].cpf);
        free(clientes[i].rua);
        free(clientes[i].celular);
        free(clientes[i].data);
    }
    free(clientes);
}

// Função para listar clientes que compraram a partir de uma determinada data
void clientePeriodo() {
    char dataInicio[20];

    printf("\nDigite a data inicial para a busca (dd/mm/yyyy): ");
    scanf(" %19s", dataInicio);

    // Lógica para buscar e listar clientes a partir da data fornecida
    Clientes *clientes = NULL;
    int numClientes = lerClientes(&clientes);

    if (numClientes == 0) {
        printf("Não há clientes para listar.\n");
        return;
    }

    printf("Clientes que compraram a partir de %s:\n", dataInicio);

    for (int i = 0; i < numClientes; i++) {
        if (strncmp(dataInicio, clientes[i].data, 10) <= 0) {
            printf("%s\n", clientes[i].nome);
        }
    }

    free(clientes);
}

void listaClientes() {
    clear();

    int opcao = 0;

    typedef struct {
        int id;
        char nome[100];
    } Relatorios;

    Relatorios clientes[] = {
        {1,"Listagem de Clientes (ordenada em ordem alfabetica por nome)"},
        {2,"Listagem dos Clientes que Compraram (em um determinado periodo)"},
        {3,"Voltar ao menu"},
    };

    printf("\nRelatorio de Clientes\n");
    for (size_t i = 0; i < sizeof(clientes) / sizeof(clientes[0]); i++) {
        printf("%d - %s\n", clientes[i].id, clientes[i].nome);
    }

    printf("\nDigite a opcao desejada: ");
    scanf("%d", &opcao);
    getchar(); // Limpa o caractere de nova linha do buffer

    switch (opcao) {
        case 1:
            clienteAlfabetica("clientes_ordenados.txt"); // Nome do arquivo para salvar os clientes ordenados
            break;
        case 2:
            clientePeriodo();
            break;
        case 3:
            menuPrincipal();
            break;
        default:
            printf("Opcao invalida.\n");
            break;
    }
}

int lerProdutos(Produtos **produtos) {
    FILE *arquivo = fopen("produtos.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    int capacidade = 10;
    int contador = 0;
    *produtos = malloc(capacidade * sizeof(Produtos));
    if (*produtos == NULL) {
        printf("Erro ao alocar memória.\n");
        fclose(arquivo);
        return 0;
    }

    char buffer[256]; // Buffer para ler uma linha do arquivo

    while (fgets(buffer, sizeof(buffer), arquivo) != NULL) {
        if (contador >= capacidade) {
            capacidade += 10;
            *produtos = realloc(*produtos, capacidade * sizeof(Produtos));
            if (*produtos == NULL) {
                printf("Erro ao realocar memória.\n");
                fclose(arquivo);
                return contador;
            }
        }

        // Aloca memória para cada campo de string na estrutura Produtos
        (*produtos)[contador].descricao = malloc(100 * sizeof(char));
        (*produtos)[contador].categoria = malloc(100 * sizeof(char));

        if ((*produtos)[contador].descricao == NULL || (*produtos)[contador].categoria == NULL) {
            printf("Erro ao alocar memória para as strings.\n");
            fclose(arquivo);
            return contador;
        }

        // Lê os dados do buffer
        sscanf(buffer, "%d %99[^,], %99[^,], %f %f %f %d %d",
               &(*produtos)[contador].codigo,
               (*produtos)[contador].descricao,
               (*produtos)[contador].categoria,
               &(*produtos)[contador].precoCompra,
               &(*produtos)[contador].margemLucro,
               &(*produtos)[contador].precoVenda,
               &(*produtos)[contador].quantidadeEstoque,
               &(*produtos)[contador].estoqueMinimo);

        contador++;
    }

    fclose(arquivo);
    return contador;
}

int comparaProduto(const void *a, const void *b) {
    const Produtos *produtoA = (const Produtos *)a;
    const Produtos *produtoB = (const Produtos *)b;
    return strcmp(produtoA->descricao, produtoB->descricao);
}

void produtoAlfabetica(const char *nomeArquivo) {
    Produtos *produtos = NULL;
    int numProdutos = lerProdutos(&produtos);

    if (numProdutos == 0) {
        printf("Não há produtos para ordenar.\n");
        return;
    }

    // Ordena os produtos pela descrição
    qsort(produtos, numProdutos, sizeof(Produtos), comparaProduto);

    // Imprime as descrições ordenadas
    printf("Descrições dos produtos ordenadas:\n");
    for (int i = 0; i < numProdutos; i++) {
        printf("%s\n", produtos[i].descricao);
    }

    // Abre o arquivo para escrita
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s para escrita.\n", nomeArquivo);
        free(produtos);
        return;
    }

    // Escreve os produtos ordenados no arquivo
    for (int i = 0; i < numProdutos; i++) {
        fprintf(arquivo, "%d, %s, %s, %.2f, %.2f, %.2f, %d, %d\n",
                produtos[i].codigo,
                produtos[i].descricao,
                produtos[i].categoria,
                produtos[i].precoCompra,
                produtos[i].margemLucro,
                produtos[i].precoVenda,
                produtos[i].quantidadeEstoque,
                produtos[i].estoqueMinimo);
    }

    // Fecha o arquivo e libera a memória alocada
    fclose(arquivo);
    for (int i = 0; i < numProdutos; i++) {
        free(produtos[i].descricao);
        free(produtos[i].categoria);
    }
    free(produtos);
}

void listaProdutos(){
    clear();

    int opcao = 0;

    typedef struct {
        int id;
        char nome[100];
    } Relatorios;

    Relatorios produtos[] = {
        {1,"Listagem de Produtos (ordenada em ordem alfabetica por descricao)"},
        {2,"Listagem de Produtos com Estoque zero ou Minimo(ordenada em ordem alfabetica por descricao)"},
        {3,"Voltar ao menu"},
    };

    printf("\nRelatorio de Produtos\n");
    for (size_t i = 0; i < sizeof(produtos) / sizeof(produtos[0]); i++) {
        printf("%d - %s\n", produtos[i].id, produtos[i].nome);
    }

    printf("\nDigite a opcao desejada: ");
    scanf("%d", &opcao);
    getchar(); // Limpa o caractere de nova linha do buffer

    switch (opcao) {
        case 1:
            produtoAlfabetica("produtos_ordenados.txt"); // Nome do arquivo para salvar os clientes ordenados
            break;
        case 2:
            clientePeriodo();
            break;
        case 3:
            menuPrincipal();
            break;
        default:
            printf("Opcao invalida.\n");
            break;
    }
}

void relatorios() {
    clear();
    int opcao = 0;

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

    switch (opcao)
    {
    case 1:
        listaClientes();
        break;
    case 2:
        listaProdutos();
        break;
    case 3:
        /* code */
        break;
    case 4:
        /* code */
        break;
    
    default:
        printf("Opcao invalida, digite novamente\n");
        break;
    }
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