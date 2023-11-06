#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>

typedef struct {
    char numeroCartao[21], nome[31], vencimento[6], uf[3], endereco[31], numeroResidencial[11];
    int ccv, cep;
} DadosPagamentos;

typedef struct {
    int numeroCliente, tipoAssinatura;
    char nome[31], cpf[12], telefone[12], email[31];
    bool ativo, renovacaoAutomatica;
    DadosPagamentos pagamento;
} Cadastro;

//Troca os valores de duas variaveis
void trocar(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

//Transforma todas as letras em maiusculas
void upperCase (char *string) {
    while (*string != '\0') {
        if (*string >= 97 && *string <= 122) {
            string -= 32;
        }

        string++;
    }
}

void recriarIndices (int *indices, int quant) {
    for (int i = 0; i < quant; i++) {
        *(indices + i) = i;
    }
}

int validarCPF (const char *cpf) {
    int digitosVerificadores[2];
    bool digitosIguais = true;

    //verificar se todos digitos são iguais
    for (int i = 1; i < 11; i++) {
        if (cpf[0] != cpf[i]) {
            digitosIguais = false;
            break;
        }
    }
    if (digitosIguais) return -1;

    //criar os digitos verificadores
    for (int i = 0; i < 2; i++) {
        int temp = 0;

        for (int j = 0; j < (9 + i); j++) {
            temp += (cpf[j] - 48) * (10 + i - j);
        }
        temp = temp % 11;

        if (temp < 2) {
            digitosVerificadores[i] = 0;
        } else {
            digitosVerificadores[i] = 11 - temp;
        }
    }

    //verifica ultimos 2 digitos do CPF
    for (int i = 9; i < 11; i++) {
        if ((cpf[i] - 48) != digitosVerificadores[i - 9]) return -1;
    }

    return 0;
}

int validarCartao(const char *cartao) {
    int len = strlen(cartao);
    int soma = 0;
    bool ePar = false;

    if (len != 16) return -1;

    for (int i = len - 1; i >= 0; i--) {
        int digito = cartao[i] - '0';

        if (ePar) {
            digito *= 2;
            if (digito > 9) {
                digito = digito / 10 + digito % 10;
            }
        }

        soma += digito;
        ePar = !ePar; //alternar o digito (um sim, outro nao)
    }

    if (soma % 10 == 0) {
        return 0;  // Numero de cartao de credito valido
    } else {
        return -1;  // Numero de cartao de credito invalido
    }
}

int dividir(Cadastro *clientes, int *ordem, int min, int max) {
	int pivot = ordem[max], i = (min - 1);

	for (int j = min; j <= max - 1; j++) {
		// se o elemento atual for menor, trocar com o pivot
		if (strcmp(clientes[ordem[j]].nome, clientes[pivot].nome) < 0) {
			i++;
			trocar(&ordem[i], &ordem[j]);
		}
	}

	trocar(&ordem[i + 1], &ordem[max]);
	return (i + 1);
}

// string[][] -> Vetor com as strings
// ordem[] -> Vetor para ser organizado (usando string como referencia),
// min -> Indice inicial,
// max -> Indice final
void quickSort(Cadastro *clientes, int *ordem, int min, int max)
{
	if (min < max) {
		int particao = dividir(clientes, ordem, min, max);

		//Organiza elementos antes e depois da particao
		quickSort(clientes, ordem, min, particao - 1);
		quickSort(clientes, ordem, particao + 1, max);
	}
}

Cadastro* importarDados(FILE *arq, Cadastro *clientes, int **ordem, int *quant, int *identificador) {
    char c;
    *quant = 0;

    //pular a primeira linha do arquivo
    do {
        c = fgetc(arq);
    } while (c != '\n');

    while (!feof(arq)) {
        clientes = (Cadastro *) realloc(clientes, (*quant + 1) * sizeof(Cadastro));
        *ordem = (int *) realloc(*ordem, (*quant + 1) * sizeof(int));
        if (*ordem == NULL || clientes == NULL) {
            return NULL;
        }

        fscanf(arq,
               "%d;%d;%30[^;];%11[^;];%11[^;];%30[^;];%d;%d;%30[^;];%20[^;];%5[^;];%d;%2[^;];%d;%30[^;];%10[^;];\n",
               &clientes[*quant].numeroCliente, &clientes[*quant].ativo, clientes[*quant].nome, clientes[*quant].cpf,
               clientes[*quant].telefone, clientes[*quant].email, &clientes[*quant].tipoAssinatura,
               &clientes[*quant].renovacaoAutomatica, clientes[*quant].pagamento.nome,
               clientes[*quant].pagamento.numeroCartao, clientes[*quant].pagamento.vencimento,
               &clientes[*quant].pagamento.ccv, clientes[*quant].pagamento.uf, &clientes[*quant].pagamento.cep,
               clientes[*quant].pagamento.endereco, clientes[*quant].pagamento.numeroResidencial);
        *ordem[*quant] = clientes[*quant].numeroCliente;

        *quant += 1;
    }

    *identificador = clientes[*quant - 1].numeroCliente;

    return clientes;
}