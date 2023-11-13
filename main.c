#include "include/app.h"

int main () {
    setbuf(stdout, 0); //necessario para o debug do clion funcionar
    int quantClientes = 0, identificadorCliente = 0, clienteCopiado = -1, opcaoSelecionada, num, *ordemAlfabeticaIndices;
    Cadastro *clientes;
    FILE *arq;

    clientes = (Cadastro*) malloc(0);
    ordemAlfabeticaIndices = (int*) malloc(0);

    printf("Deseja importar arquivo 'clientes.csv' com dados ou iniciar do zero (0 - nao // 1 - sim): ");
    scanf("%d", &opcaoSelecionada);

    if (opcaoSelecionada) {
        //importar .csv
        arq = fopen("clientes.csv", "r");
        if (arq == NULL) {
            printf("Erro ao abrir o arquivo\n");
            Sleep(2000);
        } else {
            clientes = importarDados(arq, clientes, &ordemAlfabeticaIndices, &quantClientes, &identificadorCliente);
            fclose(arq);

            if (clientes == NULL || ordemAlfabeticaIndices == NULL) {
                printf("Erro ao importar o arquivo\n");
                Sleep(2000);
                return -2;
            }

            if (quantClientes == 0) {
                printf("Nenhum cliente encontrado no arquivo\n");
                Sleep(2000);
            }
        }

        mergeSort(clientes, ordemAlfabeticaIndices, 0, quantClientes-1);
    }

    while (1) {
        system("cls");

        opcaoSelecionada = menu();
        if (opcaoSelecionada == 7) {
            arq = fopen("clientes.csv", "w");
            if (arq == NULL) {
                printf("Nao foi possivel abrir o arquivo\nDeseja sair (0 - nao // 1 - sim): ");
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
                ordemAlfabeticaIndices = (int*) realloc(ordemAlfabeticaIndices, quantClientes * sizeof(int));

                if (quantClientes > 0) ordemAlfabeticaIndices[quantClientes-1] = quantClientes-1;

                if (cadastroNovoCliente(clientes, quantClientes - 1, identificadorCliente) == -1) { //cadastro invalidado
                    identificadorCliente--;
                    quantClientes--;

                    clientes = (Cadastro*) realloc(clientes, quantClientes * sizeof(Cadastro));
                    ordemAlfabeticaIndices = (int*) realloc(ordemAlfabeticaIndices, quantClientes * sizeof(int));
                } else {
                    mergeSort(clientes, ordemAlfabeticaIndices, 0, quantClientes-1);
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
                    recriarIndices(ordemAlfabeticaIndices, quantClientes);
                    mergeSort(clientes, ordemAlfabeticaIndices, 0, quantClientes-1);
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