#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cidades.h"

int comparar(const void *a, const void *b) {
    return ((Cidade *)a)->Posicao - ((Cidade *)b)->Posicao;
}

Estrada *getEstrada(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) return NULL;

    Estrada *e = (Estrada *)malloc(sizeof(Estrada));
    if (!e) return NULL;

    if (fscanf(arquivo, "%d", &e->T) != 1 || e->T < 3 || e->T > 1000000) {
        fclose(arquivo);
        free(e);
        return NULL;
    }

    if (fscanf(arquivo, "%d", &e->N) != 1 || e->N < 2 || e->N > 10000) {
        fclose(arquivo);
        free(e);
        return NULL;
    }

    e->C = (Cidade *)malloc(sizeof(Cidade) * e->N);
    if (!e->C) {
        fclose(arquivo);
        free(e);
        return NULL;
    }

    for (int i = 0; i < e->N; i++) {
        if (fscanf(arquivo, "%d %[^\n]", &e->C[i].Posicao, e->C[i].Nome) != 2) {
            fclose(arquivo);
            free(e->C);
            free(e);
            return NULL;
        }

        if (e->C[i].Posicao <= 0 || e->C[i].Posicao >= e->T) {
            fclose(arquivo);
            free(e->C);
            free(e);
            return NULL;
        }

        for (int j = 0; j < i; j++) {
            if (e->C[i].Posicao == e->C[j].Posicao) {
                fclose(arquivo);
                free(e->C);
                free(e);
                return NULL;
            }
        }
    }

    fclose(arquivo);
    qsort(e->C, e->N, sizeof(Cidade), comparar);
    return e;
}

double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *e = getEstrada(nomeArquivo);
    if (!e) return -1;

    double menor = e->T;
    for (int i = 0; i < e->N; i++) {
        double inicio, fim;

        if (i == 0)
            inicio = 0;
        else
            inicio = (e->C[i].Posicao + e->C[i-1].Posicao) / 2.0;

        if (i == e->N - 1)
            fim = e->T;
        else
            fim = (e->C[i].Posicao + e->C[i+1].Posicao) / 2.0;

        double vizinhanca = fim - inicio;
        if (vizinhanca < menor)
            menor = vizinhanca;
    }

    free(e->C);
    free(e);
    return menor;
}

char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *e = getEstrada(nomeArquivo);
    if (!e) return NULL;

    double menor = e->T;
    int indice = 0;

    for (int i = 0; i < e->N; i++) {
        double inicio, fim;

        if (i == 0)
            inicio = 0;
        else
            inicio = (e->C[i].Posicao + e->C[i-1].Posicao) / 2.0;

        if (i == e->N - 1)
            fim = e->T;
        else
            fim = (e->C[i].Posicao + e->C[i+1].Posicao) / 2.0;

        double vizinhanca = fim - inicio;
        if (vizinhanca < menor) {
            menor = vizinhanca;
            indice = i;
        }
    }

    char *resultado = (char *)malloc(strlen(e->C[indice].Nome) + 1);
    if (resultado)
        strcpy(resultado, e->C[indice].Nome);

    free(e->C);
    free(e);
    return resultado;
}
