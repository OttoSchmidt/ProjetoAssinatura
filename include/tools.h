#include <stdbool.h>

void upperCase (char *string) {
    while (*string != '\0') {
        if (*string >= 97 && *string <= 122) {
            string -= 32;
        }

        string++;
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

//int validarCartao (const char *cartao) {}

//int validarEmail (const char *email) {}

//void ordernarAlfabeticamente (Cadastro *clientes, int *ordemAlfabetica) {}
