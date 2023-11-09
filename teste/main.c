#include <stdio.h>
#include <stdlib.h>

typedef struct Celula {
    struct Celula *direita, *abaixo;
    int linha, coluna;
    float valor;
} Celula;

typedef struct {
    Celula *cabeca;
    int nlin, ncol;
} Matriz;

Matriz leMatriz(FILE *arquivo) {
    int nlin, ncol;
    fscanf(arquivo, "%d %d", &nlin, &ncol);

    Matriz matriz;
    matriz.nlin = nlin;
    matriz.ncol = ncol;

    matriz.cabeca = (Celula *)malloc(sizeof(Celula));
    matriz.cabeca->linha = -1;
    matriz.cabeca->coluna = -1;
    matriz.cabeca->valor = 0;
    matriz.cabeca->direita = matriz.cabeca;
    matriz.cabeca->abaixo = matriz.cabeca;

    Celula *coluna_cabecal = matriz.cabeca;
    Celula *linha_cabecal = matriz.cabeca;

    for (int i = 0; i < nlin; i++) {
        Celula *novo = (Celula *)malloc(sizeof(Celula));
        novo->linha = i;
        novo->coluna = -1;
        novo->valor = 0;
        novo->direita = novo;
        novo->abaixo = linha_cabecal->abaixo;
        linha_cabecal->abaixo = novo;
        linha_cabecal = novo;

        for (int j = 0; j < ncol; j++) {
            float valor;
            fscanf(arquivo, "%f", &valor);
            if (valor != 0) {
                Celula *novo_elem = (Celula *)malloc(sizeof(Celula));
                novo_elem->linha = i;
                novo_elem->coluna = j;
                novo_elem->valor = valor;
                novo_elem->direita = novo_elem;
                novo_elem->abaixo = novo_elem;

                Celula *anterior = coluna_cabecal;
                while (anterior->abaixo->linha < i)
                    anterior = anterior->abaixo;

                Celula *anterior_coluna = coluna_cabecal;
                while (anterior_coluna->direita->coluna < j)
                    anterior_coluna = anterior_coluna->direita;

                novo_elem->abaixo = anterior->abaixo;
                anterior->abaixo = novo_elem;

                novo_elem->direita = anterior_coluna->direita;
                anterior_coluna->direita = novo_elem;
            }
        }
    }

    return matriz;
}

void imprimeMatriz(Matriz A) {
    Celula *linha_cabecal = A.cabeca->abaixo;
    while (linha_cabecal != A.cabeca) {
        Celula *atual = linha_cabecal->direita;
        while (atual != linha_cabecal) {
            printf("(%d,%d) = %f ", atual->linha, atual->coluna, atual->valor);
            atual = atual->direita;
        }
        printf("\n");
        linha_cabecal = linha_cabecal->abaixo;
    }
}

Matriz somaMatrizes(Matriz A, Matriz B) {
    if (A.nlin != B.nlin || A.ncol != B.ncol) {
        printf("As matrizes têm dimensões diferentes e não podem ser somadas.\n");
        exit(1);
    }

    Matriz C;
    C.nlin = A.nlin;
    C.ncol = A.ncol;

    C.cabeca = (Celula *)malloc(sizeof(Celula));
    C.cabeca->linha = -1;
    C.cabeca->coluna = -1;
    C.cabeca->valor = 0;
    C.cabeca->direita = C.cabeca;
    C.cabeca->abaixo = C.cabeca;

    Celula *linha_cabecal_A = A.cabeca->abaixo;
    Celula *linha_cabecal_B = B.cabeca->abaixo;

    while (linha_cabecal_A != A.cabeca && linha_cabecal_B != B.cabeca) {
        Celula *atual_A = linha_cabecal_A->direita;
        Celula *atual_B = linha_cabecal_B->direita;
        Celula *linha_cabecal_C = C.cabeca;
        while (atual_A != linha_cabecal_A && atual_B != linha_cabecal_B) {
            if (atual_A->coluna == atual_B->coluna) {
                float soma = atual_A->valor + atual_B->valor;
                if (soma != 0) {
                    Celula *novo = (Celula *)malloc(sizeof(Celula));
                    novo->linha = atual_A->linha;
                    novo->coluna = atual_A->coluna;
                    novo->valor = soma;
                    novo->direita = novo;
                    novo->abaixo = linha_cabecal_C->abaixo;
                    linha_cabecal_C->abaixo = novo;
                    linha_cabecal_C = novo;
                }
                atual_A = atual_A->direita;
                atual_B = atual_B->direita;
            } else if (atual_A->coluna < atual_B->coluna) {
                Celula *novo = (Celula *)malloc(sizeof(Celula));
                novo->linha = atual_A->linha;
                novo->coluna = atual_A->coluna;
                novo->valor = atual_A->valor;
                novo->direita = novo;
                novo->abaixo = linha_cabecal_C->abaixo;
                linha_cabecal_C->abaixo = novo;
                linha_cabecal_C = novo;
                atual_A = atual_A->direita;
            } else {
                Celula *novo = (Celula *)malloc(sizeof(Celula));
                novo->linha = atual_B->linha;
                novo->coluna = atual_B->coluna;
                novo->valor = atual_B->valor;
                novo->direita = novo;
                novo->abaixo = linha_cabecal_C->abaixo;
                linha_cabecal_C->abaixo = novo;
                linha_cabecal_C = novo;
                atual_B = atual_B->direita;
            }
        }

        while (atual_A != linha_cabecal_A) {
            Celula *novo = (Celula *)malloc(sizeof(Celula));
            novo->linha = atual_A->linha;
            novo->coluna = atual_A->coluna;
            novo->valor = atual_A->valor;
            novo->direita = novo;
            novo->abaixo = linha_cabecal_C->abaixo;
            linha_cabecal_C->abaixo = novo;
            linha_cabecal_C = novo;
            atual_A = atual_A->direita;
        }

        while (atual_B != linha_cabecal_B) {
            Celula *novo = (Celula *)malloc(sizeof(Celula));
            novo->linha = atual_B->linha;
            novo->coluna = atual_B->coluna;
            novo->valor = atual_B->valor;
            novo->direita = novo;
            novo->abaixo = linha_cabecal_C->abaixo;
            linha_cabecal_C->abaixo = novo;
            linha_cabecal_C = novo;
            atual_B = atual_B->direita;
        }

        linha_cabecal_A = linha_cabecal_A->abaixo;
        linha_cabecal_B = linha_cabecal_B->abaixo;
    }

    return C;
}

Matriz multiplicaMatrizes(Matriz A, Matriz B) {
    if (A.ncol != B.nlin) {
        printf("O número de colunas de A deve ser igual ao número de linhas de B para multiplicação de matrizes.\n");
        exit(1);
    }

    Matriz C;
    C.nlin = A.nlin;
    C.ncol = B.ncol;

    C.cabeca = (Celula *)malloc(sizeof(Celula));
    C.cabeca->linha = -1;
    C.cabeca->coluna = -1;
    C.cabeca->valor = 0;
    C.cabeca->direita = C.cabeca;
    C.cabeca->abaixo = C.cabeca;

    for (int i = 0; i < A.nlin; i++) {
        Celula *linha_A = A.cabeca->abaixo;
        while (linha_A->linha != i)
            linha_A = linha_A->abaixo;

        for (int j = 0; j < B.ncol; j++) {
            Celula *coluna_B = B.cabeca->direita;
            while (coluna_B->coluna != j)
                coluna_B = coluna_B->direita;

            float soma = 0;
            Celula *elem_A = linha_A->direita;
            Celula *elem_B = coluna_B->abaixo;
            while (elem_A != linha_A && elem_B != coluna_B) {
                if (elem_A->coluna == elem_B->linha) {
                    soma += elem_A->valor * elem_B->valor;
                    elem_A = elem_A->direita;
                    elem_B = elem_B->abaixo;
                } else if (elem_A->coluna < elem_B->linha) {
                    elem_A = elem_A->direita;
                } else {
                    elem_B = elem_B->abaixo;
                }
            }

            if (soma != 0) {
                Celula *novo = (Celula *)malloc(sizeof(Celula));
                novo->linha = i;
                novo->coluna = j;
                novo->valor = soma;
                novo->direita = novo;
                novo->abaixo = C.cabeca->abaixo;
                C.cabeca->abaixo = novo;
            }
        }
    }

    return C;
}

int main() {
    FILE *arquivoA = fopen("matrizA.txt", "r");
    if (arquivoA == NULL) {
        printf("Falha ao abrir o arquivo matrizA.txt.\n");
        return 1;
    }

    Matriz A = leMatriz(arquivoA);
    printf("Matriz A:\n");
    imprimeMatriz(A);
    fclose(arquivoA);

    FILE *arquivoB = fopen("matrizB.txt", "r");
    if (arquivoB == NULL) {
        printf("Falha ao abrir o arquivo matrizB.txt.\n");
        return 1;
    }

    Matriz B = leMatriz(arquivoB);
    printf("Matriz B:\n");
    imprimeMatriz(B);
    fclose(arquivoB);

    Matriz C = somaMatrizes(A, B);
    printf("Matriz C (soma de A e B):\n");
    imprimeMatriz(C);

    Matriz D = multiplicaMatrizes(A, B);
    printf("Matriz D (multiplicação de A e B):\n");
    imprimeMatriz(D);

    return 0;
}
