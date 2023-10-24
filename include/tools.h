#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    char numeroCartao[13], nome[31], vencimento[6], uf[3], endereco[31], numeroResidencial[11];
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

int validarCartao (const char *cartao) {
    int digitoVerificador;

    /*
        num cartao (ex): 4417 1234 5678 9113
        1. Comecando da esquerda, calcule o dobro de um digito e outro nao:
            4(x2) 4 1(x2) 7 1(x2) 2 3(x2) 4 5(x2) 6 7(x2) 8 9(x2) 1 1(x2) 3
            O resultado e: 8427 2264 106148 18123
        2. Se algum resultado tiver mais de dois digitos, some-os:
            10 = 1+0 // 14= 1+4 // 18= 1+8
            Entao tamos 8427 2264 2658 9123
        3. Adicione todos os digitos:
            8+4+2+7 + 2+2+6+4 + 2+6+7+8 + 9+1+2+3 = 70
        4. Se o resultado da soma for multiplo de 10, então o cartao e valido
     */

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