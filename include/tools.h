#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    char numeroCartao[17], nome[31], vencimento[6], uf[3], endereco[31], numeroResidencial[11];
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
    int ePar = 0;

    for (int i = len - 1; i >= 0; i--) {
        int digito = cartao[i] - '0';

        if (ePar) {
            digito *= 2;
            if (digito > 9) {
                digito = digito / 10 + digito % 10;
            }
        }

        soma += digito;
        ePar = !ePar;
    }

    if (soma % 10 == 0) {
        return 1;  // Número de cartão de crédito válido
    } else {
        return 0;  // Número de cartão de crédito inválido
    }
}

int main() {
    const char *cartao = "4417123456789113";

    if (validarCartao(cartao)) {
        printf("Número de cartão de crédito válido\n");
    } else {
        printf("Número de cartão de crédito inválido\n");
    }

    return 0;
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

void importarDados(FILE *arq, Cadastro *clientes, int *ordem, int *quant, int *identificador) {
    while (!feof(arq)) {
        //


    }
}