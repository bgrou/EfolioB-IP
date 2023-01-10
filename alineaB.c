#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#define BUFFER_SIZE 64
#define MAX_FLOAT 3.402823466e38

typedef struct {
    char * nome;
    float * distanciaEstrela;
} Planeta;

typedef struct nodeListaPlanetas {
    Planeta * planeta;
    struct nodeListaPlanetas * next;
} NodeListaPlanetas;

typedef struct {
    char * nome;
} Estrela;

typedef struct {
    Estrela * estrela;
    NodeListaPlanetas * listaPlanetas;
} SistemaPlanetario;

void * alocarMemoria(size_t tamanho) {
    void * ptrAlloc = malloc(tamanho);
    if (ptrAlloc == NULL) {
        printf("Falha ao alocar memória.");
        exit(1);
    }
    return ptrAlloc;
}

void obterInputUser(char * buffer) {
    fgets(buffer, BUFFER_SIZE, stdin);
}

int bufferVazio(const char * buffer) {
    return buffer[0] == '\n';
}

Estrela * criarEstrela() {
    Estrela * estrela = alocarMemoria(sizeof(Estrela));
    estrela -> nome = alocarMemoria(BUFFER_SIZE);
    fgets(estrela -> nome, BUFFER_SIZE, stdin);
    estrela -> nome[strcspn(estrela -> nome, " \n")] = 0;
    return estrela;
}

void libertarEstrela(Estrela * estrela) {
    free(estrela -> nome);
    free(estrela);
}

Planeta * criarPlaneta(char * nomePlaneta, float * distanciaEstrela) {
    Planeta * planeta = alocarMemoria(sizeof(Planeta));
    planeta -> nome = nomePlaneta;
    planeta -> distanciaEstrela = distanciaEstrela;
    return planeta;
}

void libertarPlaneta(Planeta * planeta) {
    free(planeta -> nome);
    free(planeta);
}

void libertarListaPlanetas(NodeListaPlanetas * topoLista) {
    NodeListaPlanetas * nodeAtual = topoLista;
    while (nodeAtual != NULL) {
        NodeListaPlanetas * nodeProximo = nodeAtual -> next;
        libertarPlaneta(nodeAtual -> planeta);
        free(nodeAtual);
        nodeAtual = nodeProximo;
    }
}

void adicionarNodeListaPlanetas(NodeListaPlanetas ** topoLista, Planeta * novoPlaneta) {
    NodeListaPlanetas * novoNode = alocarMemoria(sizeof(NodeListaPlanetas));
    novoNode -> planeta = novoPlaneta;
    novoNode -> next = NULL;
    if ( * topoLista == NULL) {
        * topoLista = novoNode;
    } else {
        NodeListaPlanetas * nodeAtual = * topoLista;
        while (nodeAtual -> next != NULL) {
            nodeAtual = nodeAtual -> next;
        }
        nodeAtual -> next = novoNode;
    }
}

NodeListaPlanetas * criarListaPlanetas(void) {
    NodeListaPlanetas * topoLista = NULL;
    char * buffer = alocarMemoria(BUFFER_SIZE);
    char * nome;
    float * distancia;
    Planeta * planeta = NULL;
    obterInputUser(buffer);
    while (!bufferVazio(buffer)) {
        nome = alocarMemoria(BUFFER_SIZE);
        distancia = alocarMemoria(sizeof(float));

        sscanf(buffer, "%s %f\n", nome, distancia);
        planeta = criarPlaneta(nome, distancia);
        adicionarNodeListaPlanetas( & topoLista, planeta);
        obterInputUser(buffer);
    }
    return topoLista;
}

SistemaPlanetario * criarSistemaPlanetario(void) {
    SistemaPlanetario * sp = alocarMemoria(sizeof(SistemaPlanetario));
    sp -> estrela = criarEstrela();
    sp -> listaPlanetas = criarListaPlanetas();
    return sp;
}

void libertarSistemaPlanetario(SistemaPlanetario * sp) {
    libertarEstrela(sp -> estrela);
    libertarListaPlanetas(sp -> listaPlanetas);
    free(sp);
}

int contarNumPlanetas(NodeListaPlanetas * topoListaPlanetas) {
    int numPlanetas = 0;
    NodeListaPlanetas * nodeAtual = topoListaPlanetas;
    while (nodeAtual != NULL) {
        nodeAtual = nodeAtual -> next;
        numPlanetas++;
    }
    return numPlanetas;
}

float obterMin(float valor1, float valor2) {
    return (valor1 < valor2) ? valor1 : valor2;
}

float obterMax(float valor1, float valor2) {
    return (valor1 > valor2) ? valor1 : valor2;
}

void obterDistMinMaxPlanetas(SistemaPlanetario * sp, float * minDistancia, float * maxDistancia) {
    NodeListaPlanetas * nodeAtual = sp -> listaPlanetas;
    while (nodeAtual != NULL) {
        * minDistancia = obterMin( * nodeAtual -> planeta -> distanciaEstrela, * minDistancia);
        * maxDistancia = obterMax( * nodeAtual -> planeta -> distanciaEstrela, * maxDistancia);
        nodeAtual = nodeAtual -> next;
    }
}

void mostrarInfoSistema(SistemaPlanetario * sp) {
    int numPlanetas = contarNumPlanetas(sp -> listaPlanetas);
    float minDistancia = MAX_FLOAT;
    float maxDistancia = -1;
    obterDistMinMaxPlanetas(sp, & minDistancia, & maxDistancia);
    if(numPlanetas == 0)
        printf("O sistema planetário está vazio");
    else if(strcmp(sp->estrela->nome, "") == 0)
        printf("O sistema planetário não tem uma estrela");
    else
        printf("%s, sistema planetário com %d planetas a distâncias entre %.2f e %.2f UA", sp -> estrela -> nome,
                                                                                           numPlanetas,
                                                                                           minDistancia, maxDistancia);
}

int main() {
    SistemaPlanetario * sp = criarSistemaPlanetario();
    mostrarInfoSistema(sp);
    libertarSistemaPlanetario(sp);
    return 0;
}
