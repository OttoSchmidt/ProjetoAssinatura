#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "include/tools.h"

const char tiposAssinatura[3][9] = {"Starter", "Standard", "Premium"};

typedef struct {
    char numeroCartao[13], nome[31], vencimento[6], uf[3], endereco[31], numeroResidencial[11];
    int ccv, cep;
} DadosPagamentos;

/* TIPOS DE ASSINATURA:
 *  1 - BASIC
    2 - STANDARD
    3 - PREMIUM
*/

typedef struct {
    int numeroCliente, tipoAssinatura;
    char nome[31], cpf[12], telefone[12], email[21];
    bool ativo, renovacaoAutomatica;
    DadosPagamentos pagamento;
} Cadastro;

int menu () {
    int opcao;

    printf("[1] - Cadastrar novo cliente\n[2] - Alterar plano\n[3] - Desativar cliente\n[4] - Reativar cliente\n[5] - Excluir cliente\n[6] - Listar clientes\n[7] - Realizar pedido\n[8] - Sair\n");
    scanf("%d", &opcao);

    system("clear");

    return opcao;
}

void cadastroClientePagamento (DadosPagamentos *pagamento) {
    system("clear");

    printf("Informe o nome no cartao: ");
    scanf(" %30[^\n]", pagamento->nome);
    upperCase(pagamento->nome);

    while (1) {
        printf("Informe o numero do cartao (somente numeros): ");
        scanf(" %12[^\n]", pagamento->numeroCartao);

        //validarCartao(pagamento->numeroCartao);

        break;
    }

    printf("Informe a data de vencimento: ");
    scanf(" %5[^\n]", pagamento->vencimento);

    printf("Informe o CCV: ");
    scanf("%d", &pagamento->ccv);

    printf("Informe a sigla do estado: ");
    scanf(" %s", pagamento->uf);
    upperCase(pagamento->uf);

    printf("Informe o CEP: ");
    scanf("%d", &pagamento->cep);

    printf("Informe o endereco: ");
    scanf(" %30[^\n]", pagamento->endereco);

    printf("Informe o numero da residencia: ");
    scanf(" %10[^\n]", pagamento->numeroResidencial);
}

int cadastroNovoCliente (Cadastro *clientes, int i, int novoNumeroCliente) {
    int renovacaoAutomaticaTemp;
    (clientes + i)->ativo = true;
    (clientes + i)->numeroCliente = novoNumeroCliente;

    system("cls");

    printf("Informe o nome: ");
    scanf(" %30[^\n]", (clientes + i)->nome);

    printf("Informe o CPF (somente numeros): ");
    scanf(" %11[^\n]", (clientes + i)->cpf);

    if (validarCPF(clientes[i].cpf) == -1) {
        int continuar;

        printf("CPF INVALIDO\nDESEJA TENTAR NOVAMENTE (1 - sim, 2 - nao): ");
        scanf("%d", &continuar);

        if (continuar == 2) return -1;

        return cadastroNovoCliente(clientes, i, novoNumeroCliente);
    }

    printf("Informe o telefone (somente numeros): ");
    scanf(" %11[^\n]", (clientes + i)->telefone);

    printf("Informe o email: ");
    scanf("%20[^\n]", (clientes + i)->email);

    printf("OPCOES DE PLANOS:\n\t1 - Starter\n\t2 - Standard\n\t3 - Premium\nInforme o tipo de assinatura: ");
    scanf("%d", &(clientes + i)->tipoAssinatura);

    printf("Ativar renovacao automatica (0 - nao / 1 - sim): ");
    scanf("%d", &renovacaoAutomaticaTemp);

    if (renovacaoAutomaticaTemp) {
        (clientes + i)->renovacaoAutomatica = true;
    } else {
        (clientes + i)->renovacaoAutomatica = false;
    }

    cadastroClientePagamento(&(clientes + i)->pagamento);

    //ordernarAlfabeticamente(clientes);
    printf("ID CLIENTE: %d\n", (clientes + i)->numeroCliente);
    return 0;
}

int buscarCliente(Cadastro *clientes, int numeroBuscado, int tam) {
    for (int i = 0; i < tam; i++) {
        if ((clientes+i)->numeroCliente == numeroBuscado) return i;
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

    clientes = (Cadastro*) realloc(clientes, (tam - 1) * sizeof(Cadastro));
    //ordernarAlfabeticamente(clientes);
    return clientes;
}

void imprimirClienteInd(Cadastro *clientes, int i) {
    printf("\nNUM CLIENTE: %d\nATIVO: %d\nNOME: %s\nCPF: %s\nTIPO DE ASSINATURA: %s\nTELEFONE: %sEMAIL: %s\n", clientes[i].numeroCliente, clientes[i].ativo, clientes[i].nome, clientes[i].cpf, tiposAssinatura[clientes[i].tipoAssinatura - 1], clientes[i].telefone, clientes[i].email);
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

    printf("\nInsira o numero do cliente que deseja copiar para futura operacoes (caso nao precise, digite 0): ");
    scanf("%d", clienteSelecionado);
}

int main () {
    setbuf(stdout, 0); //necessario para o debug do clion funcionar
    int quantClientes = 0, identificadorCliente = 0, clienteCopiado = -1, opcaoSelecionada, num, *ordemAlfabeticaIndices;
    Cadastro *clientes;
    FILE *arq;

    clientes = (Cadastro*) malloc(0);
    ordemAlfabeticaIndices = (int*) malloc(0);

    while (1) {
        system("cls");

        opcaoSelecionada = menu();
        if (opcaoSelecionada == 8) {
            arq = fopen("clientes.csv", "w");
            if (arq == NULL) {
                printf("Nao foi possivl abrir o arquivo\nDeseja sair (0 - nao // 1 - sim): ");
                scanf("%d", &opcaoSelecionada);

                if (opcaoSelecionada == 1) {
                    break;
                } else {
                    continue;
                }
            } else {
                break;
            }
        }

        switch (opcaoSelecionada) {
            case 1: //cadastrar cliente
                quantClientes++;
                identificadorCliente++;

                clientes = (Cadastro*) realloc(clientes, quantClientes * sizeof(Cadastro));
                //ordemAlfabeticaIndices = (int*) realloc(ordemAlfabeticaIndices, quantClientes * sizeof(int));

                if (cadastroNovoCliente(clientes, quantClientes - 1, identificadorCliente) == -1) {
                    //cadastro invalido
                    identificadorCliente--;
                    quantClientes--;

                    clientes = (Cadastro*) realloc(clientes, quantClientes * sizeof(Cadastro));
                    ordemAlfabeticaIndices = (int*) realloc(ordemAlfabeticaIndices, quantClientes * sizeof(int));
                }
                break;
            case 2: //alterar plano
                if (clienteCopiado != -1) {
                    printf("Cliente selecionado anteriormente:");
                    imprimirClienteInd(clientes, clienteCopiado);
                }

                printf("Informe o numero do cliente para alterar o plano: ");
                scanf("%d", &num);

                num = buscarCliente(clientes, num, quantClientes);

                if (num == -1) {
                    printf("CLIENTE NAO ENCONTRADO\n");
                } else {
                    printf("PLANO ATUAL: %s\n\nPLANOS DISPONIVEIS:\n\t1 - Starter\n\t2 - Standard\n\t3 - Premium\nPlano escolhido: ", tiposAssinatura[clientes[num].tipoAssinatura - 1]);
                    scanf("%d", &opcaoSelecionada);

                    if (opcaoSelecionada < 1 || opcaoSelecionada > 3) {
                        printf("Opcao invalida\n");
                    } else {
                        clientes[num].tipoAssinatura = opcaoSelecionada;
                    }
                }

                break;
            case 3: //desativar cliente
                if (clienteCopiado != -1) {
                    printf("Cliente selecionado anteriormente:");
                    imprimirClienteInd(clientes, clienteCopiado);
                }

                printf("\nInforme o numero do cliente a ser desativado: ");
                scanf("%d", &num);

                num = buscarCliente(clientes, num, quantClientes);

                if (num == -1) {
                    printf("CLIENTE NAO ENCONTRADO\n");
                } else {
                    clientes[num].ativo = false;
                }

                break;
            case 4: //reativar cliente
                if (clienteCopiado != -1) {
                    printf("Cliente selecionado anteriormente:");
                    imprimirClienteInd(clientes, clienteCopiado);
                }

                printf("\nInforme o numero do cliente a ser reativado: ");
                scanf("%d", &num);

                num = buscarCliente(clientes, num, quantClientes);

                if (num == -1) {
                    printf("CLIENTE NÃO ENCONTRADO\n");
                } else {
                    clientes[num].ativo = true;
                }

                break;
            case 5: //excluir cliente
                if (clienteCopiado != -1) {
                    printf("Cliente selecionado anteriormente:");
                    imprimirClienteInd(clientes, clienteCopiado);
                }

                printf("\nInforme o numero do cliente a ser excluido: ");
                scanf("%d", &num);

                num = buscarCliente(clientes, num, quantClientes);

                if (num == -1) {
                    printf("CLIENTE NÃO ENCONTRADO\n");
                } else {
                    clientes = excluirCliente(clientes, num, &clienteCopiado, quantClientes);
                    quantClientes--;
                }

                printf("QUANTIDADE CLIENTES: %d\n", quantClientes);

                break;
            case 6: //imprimir lista clientes
                printf("Selecione a forma para ordernar (1 - numerico, 2 - alfabetico): ");
                scanf("%d", &num);

                if (num != 1 && num != 2) {
                    printf("Opcao invalida!\n");
                } else {
                    imprimirClientes(clientes, ordemAlfabeticaIndices, num, quantClientes, &clienteCopiado);
                }

                if (clienteCopiado != 0) {
                    clienteCopiado = buscarCliente(clientes, clienteCopiado, quantClientes);
                }

                break;
            case 7: //realizar pedido
                printf("REALIZAR PEDIDO\n");
                break;
            default: printf("Opção invalida");
        }
    }

    //armazenar clientes em .csv
    fprintf(arq, "ID CLIENTE;ATIVO;NOME CLIENTE;CPF;TELEFONE;EMAIL;TIPO ASSINATURA;RENOVACAO AUTOMATICA;NOME CARTAO;NUM CARTAO;DATA VENCIMENTO;CCV;UF;CEP;ENDERECO;NUMERO;\n");
    for (int i = 0; i < quantClientes; i++) {
        fprintf(arq, "%d;%d;%s;%s;%s;%s;%d;%d;%s;%s;%s;%d;%s;%d;%s;%s;\n", clientes[i].numeroCliente, clientes[i].ativo, clientes[i].nome, clientes[i].cpf, clientes[i].telefone, clientes[i].email, clientes[i].tipoAssinatura, clientes[i].renovacaoAutomatica, clientes[i].pagamento.nome, clientes[i].pagamento.numeroCartao, clientes[i].pagamento.vencimento, clientes[i].pagamento.ccv, clientes[i].pagamento.uf, clientes[i].pagamento.cep, clientes[i].pagamento.endereco, clientes[i].pagamento.numeroResidencial);
    }
    fclose(arq);

    free(clientes);
    free(ordemAlfabeticaIndices);

    return 0;
}