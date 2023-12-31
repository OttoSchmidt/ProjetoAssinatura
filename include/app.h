#include "tools.h"

const char tiposAssinatura[3][9] = {"Pro", "Max", "Ultra"};

void logo() {
    printf("|     ____  _              ____  _           _    \n"
                "|    |  _ \\| |            / ___ | |__   __ _| | __\n"
                "|    | |_) | |_   _  ___  \\___ \\| '_ \\ / _` | |/ /\n"
                "|    |  _ <| | | | |/ _ \\  ___) | | | | (_| |   < \n"
                "|    | |_) | | |_| |  __/ |____/|_| |_|\\__,_|_|\\_\\\n"
                "|    |____/|_|\\__,_|\\___|                          \n");
}

int menu () {
    int opcao;

    logo();

    printf("|_____________________________________________________________________ \n"
            "|                                                                     |\n"
            "|       [1] - Cadastrar novo cliente                                  |\n"
            "|       [2] - Alterar plano (realizar pedido)                         |\n"
            "|       [3] - Desativar cliente                                       |\n"
            "|       [4] - Reativar cliente                                        |\n"
            "|       [5] - Excluir cliente                                         |\n"
            "|       [6] - Listar clientes                                         |\n"
            "|       [7] - Sair                                                    |\n"
            "|_____________________________________________________________________|\n");

    scanf("%d", &opcao);

    system("cls");

    return opcao;
}

int cadastroClientePagamento (DadosPagamentos *pagamento) {
    system("cls");

    printf(" _____________________________________________________________________ \n"
           "|       DADOS DE PAGAMENTO                                            |\n\n");

    printf("       Informe o nome no cartao: ");
    scanf(" %30[^\n]", pagamento->nome);
    upperCase(pagamento->nome);

    while (1) {
        printf("       Informe o numero do cartao (somente numeros): ");
        scanf(" %20[^\n]", pagamento->numeroCartao);

        if (validarCartao(pagamento->numeroCartao) != 0) {
            int continuar;

            printf("       NUMERO DO CARTAO INVALIDO\n\n       DESEJA TENTAR NOVAMENTE (0 - nao, 1 - sim): ");
            scanf("%d", &continuar);

            if (!continuar) return -1;

            continue;
        }

        break;
    }

    printf("       Informe a data de vencimento: ");
    scanf(" %5[^\n]", pagamento->vencimento);

    printf("       Informe o CCV: ");
    scanf("%d", &pagamento->ccv);

    printf("       Informe a sigla do estado: ");
    scanf(" %s", pagamento->uf);
    upperCase(pagamento->uf);

    printf("       Informe o CEP: ");
    scanf("%d", &pagamento->cep);

    printf("       Informe o endereco: ");
    scanf(" %30[^\n]", pagamento->endereco);

    printf("       Informe o numero da residencia: ");
    scanf(" %10[^\n]", pagamento->numeroResidencial);

    return 0;
}

int cadastroNovoCliente (Cadastro *clientes, int i, int novoNumeroCliente) {
    int renovacaoAutomaticaTemp;
    (clientes + i)->ativo = true;
    (clientes + i)->numeroCliente = novoNumeroCliente;

    system("cls");

    printf(" _____________________________________________________________________ \n"
                "|       CADASTRO CLIENTE                                              |\n\n");

    printf("       Informe o nome: ");
    scanf(" %30[^\n]", (clientes + i)->nome);

    while (1) {
        printf("       Informe o CPF (somente numeros): ");
        scanf(" %11[^\n]", (clientes + i)->cpf);

        if (validarCPF(clientes[i].cpf) == -1) {
            int continuar;

            printf("CPF INVALIDO\nDESEJA TENTAR NOVAMENTE (0 - nao, 1 - sim): ");
            scanf("%d", &continuar);

            if (!continuar) return -1;

            continue;
        }

        break;
    }

    printf("       Informe o telefone (somente numeros): ");
    scanf(" %11[^\n]", (clientes + i)->telefone);

    printf("       Informe o email: ");
    scanf(" %30[^\n]", (clientes + i)->email);

    printf("\n\n       OPCOES DE PLANOS:\n\t       1 - Pro\n\t       2 - Max\n\t       3 - Ultra\n\n       Informe o tipo de assinatura: ");
    scanf("%d", &(clientes + i)->tipoAssinatura);

    printf("\n\n       Ativar renovacao automatica (0 - nao / 1 - sim): ");
    scanf("%d", &renovacaoAutomaticaTemp);

    if (renovacaoAutomaticaTemp) {
        (clientes + i)->renovacaoAutomatica = true;
    } else {
        (clientes + i)->renovacaoAutomatica = false;
    }

    if (cadastroClientePagamento(&(clientes + i)->pagamento) != 0) return -1;

    printf("       ID CLIENTE: %d\n", (clientes + i)->numeroCliente);
    return 0;
}

//buscar cliente usando busca binaria
int buscarCliente (Cadastro *clientes, int quant, int num) {
    int inicio = 0, fim = quant - 1, meio;

    while (inicio <= fim) {
        meio = inicio + (fim - inicio) / 2;

        if ((clientes+meio)->numeroCliente == num) {
            return meio;
        } else if ((clientes+meio)->numeroCliente < num) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }

    return -1;
}

Cadastro *excluirCliente(Cadastro *clientes, int numCliente, int *clienteSelecionado, int tam) {
    if (*clienteSelecionado == numCliente) *clienteSelecionado = -1;

    for (int i = numCliente; i < tam - 1; i++) {
        clientes[i].ativo = clientes[i+1].ativo;
        clientes[i].numeroCliente = clientes[i+1].numeroCliente;
        clientes[i].tipoAssinatura = clientes[i+1].tipoAssinatura;
        strcpy(clientes[i].nome, clientes[i+1].nome);
        strcpy(clientes[i].cpf, clientes[i+1].cpf);
        strcpy(clientes[i].telefone, clientes[i+1].telefone);
        strcpy(clientes[i].email, clientes[i+1].email);

        clientes[i].pagamento.ccv = clientes[i+1].pagamento.ccv;
        clientes[i].pagamento.cep = clientes[i+1].pagamento.cep;
        strcpy(clientes[i].pagamento.nome, clientes[i+1].pagamento.nome);
        strcpy(clientes[i].pagamento.numeroCartao, clientes[i+1].pagamento.numeroCartao);
        strcpy(clientes[i].pagamento.vencimento, clientes[i+1].pagamento.vencimento);
        strcpy(clientes[i].pagamento.uf, clientes[i+1].pagamento.uf);
        strcpy(clientes[i].pagamento.endereco, clientes[i+1].pagamento.endereco);
        strcpy(clientes[i].pagamento.numeroResidencial, clientes[i+1].pagamento.numeroResidencial);
    }

    return (Cadastro*) realloc(clientes, (tam - 1) * sizeof(Cadastro));
}

void imprimirClienteInd(Cadastro *clientes, int i) {
    printf("\nNUM CLIENTE: %d\nATIVO: %d\nNOME: %s\nCPF: %s\nTIPO DE ASSINATURA: %s\nTELEFONE: %s\nEMAIL: %s\n", clientes[i].numeroCliente, clientes[i].ativo, clientes[i].nome, clientes[i].cpf, tiposAssinatura[clientes[i].tipoAssinatura - 1], clientes[i].telefone, clientes[i].email);
}

void imprimirClientes(Cadastro *clientes, int *ordemAlfabetica, int tipo, int tam, int *clienteSelecionado) {
    if (tipo == 2) {
        for (int i = 0; i < tam; i++) {
            imprimirClienteInd(clientes, ordemAlfabetica[i]);
        }
    } else {
        for (int i = 0; i < tam; i++) {
            imprimirClienteInd(clientes, i);
        }
    }

    printf("\nInsira o numero do cliente que deseja copiar para futura operacoes (caso nao precise, digite -1): ");
    scanf("%d", clienteSelecionado);
}
