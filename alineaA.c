#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <locale.h>

#define BUFFER_SIZE 64
#define MAX_float 3.402823466e38

typedef struct {
    char * nome;
    float * distanciaPlaneta;
} Satelite;

typedef struct nodeListaSatelites {
    Satelite * satelite;
    struct nodeListaSatelites * next;
} NodeListaSatelites;

typedef struct {
    char * nome;
    float * distanciaEstrela;
    NodeListaSatelites * listaSatelites;
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

int bufferVazio(const char * buffer) {
    return buffer[0] == '\n';
}

void obterInputUser(char * buffer) {
    fgets(buffer, BUFFER_SIZE, stdin);
}

int inputESatelite(const char * buffer) {
    return buffer[0] == '#';
}

void removerCaracterInput(char * buffer) {
    memmove(buffer, buffer + 1, strlen(buffer));
}

Estrela * criarEstrela() {
    Estrela * estrela = alocarMemoria(sizeof(Estrela));
    estrela -> nome = alocarMemoria(BUFFER_SIZE);
    fgets(estrela -> nome, BUFFER_SIZE, stdin);
    estrela -> nome[strcspn(estrela -> nome, " \n")] = 0;
    return estrela;
}

Planeta * criarPlaneta(char * nomePlaneta, float * distanciaEstrela) {
    Planeta * planeta = alocarMemoria(sizeof(Planeta));
    planeta -> nome = nomePlaneta;
    planeta -> distanciaEstrela = distanciaEstrela;
    planeta -> listaSatelites = NULL;
    return planeta;
}

Satelite * criarSatelite(char * nomeSatelite, float * distanciaPlaneta) {
    Satelite * satelite = alocarMemoria(sizeof(Satelite));
    satelite -> nome = nomeSatelite;
    satelite -> distanciaPlaneta = distanciaPlaneta;
    return satelite;
}

void libertarEstrela(Estrela * estrela) {
    free(estrela -> nome);
    free(estrela);
}

void libertarSatelite(Satelite * satelite) {
    free(satelite -> nome);
    free(satelite -> distanciaPlaneta);
    free(satelite);
}

void libertarListaSatelites(NodeListaSatelites * topoLista) {
    NodeListaSatelites * nodeAtual = topoLista;
    while (nodeAtual != NULL) {
        NodeListaSatelites * nodeProximo = nodeAtual -> next;
        libertarSatelite(nodeAtual -> satelite);
        free(nodeAtual);
        nodeAtual = nodeProximo;
    }
}

void libertarPlaneta(Planeta * planeta) {
    free(planeta -> nome);
    libertarListaSatelites(planeta -> listaSatelites);
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

void adicionarNodeListaSatelites(NodeListaSatelites ** topoLista, Satelite * novoSatelite) {
    NodeListaSatelites * novoNode = alocarMemoria(sizeof(NodeListaSatelites));
    novoNode -> satelite = novoSatelite;
    novoNode -> next = NULL;
    if ( * topoLista == NULL) {
        * topoLista = novoNode;
    } else {
        NodeListaSatelites * nodeAtual = * topoLista;
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
        if (inputESatelite(buffer)) {
            removerCaracterInput(buffer);
            sscanf(buffer, "%s %f\n", nome, distancia);
            adicionarNodeListaSatelites( & planeta -> listaSatelites, criarSatelite(nome, distancia));
        } else {
            sscanf(buffer, "%s %f\n", nome, distancia);
            planeta = criarPlaneta(nome, distancia);
            adicionarNodeListaPlanetas( & topoLista, planeta);
        }
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

void inputFiltrosOrdenacao(int * filtroOrdenacao, char * filtroNome, float * filtroMin, float * filtroMax) {
    scanf("%d %s %f %f", filtroOrdenacao, filtroNome, filtroMin, filtroMax);
}

void swapPlanetasNodes(NodeListaPlanetas * nodeAtual, NodeListaPlanetas * nodeAux) {
    Planeta * planetaAux = nodeAtual -> planeta;
    nodeAtual -> planeta = nodeAux -> planeta;
    nodeAux -> planeta = planetaAux;
}
void swapSatelitesNodes(NodeListaSatelites * nodeAtual, NodeListaSatelites * nodeAux) {
    Satelite * planetaAux = nodeAtual -> satelite;
    nodeAtual -> satelite = nodeAux -> satelite;
    nodeAux -> satelite = planetaAux;
}

void sortSatelitesNome(NodeListaSatelites * topoLista) {
    NodeListaSatelites * nodeAtual = topoLista;
    NodeListaSatelites * nodeAux;
    while (nodeAtual != NULL) {
        nodeAux = nodeAtual -> next;
        while (nodeAux != NULL) {
            if (strcmp(nodeAtual -> satelite -> nome, nodeAux -> satelite -> nome) > 0)
                swapSatelitesNodes(nodeAtual, nodeAux);
            nodeAux = nodeAux -> next;
        }
        nodeAtual = nodeAtual -> next;
    }
}

//Bubble Sort
void sortAstrosNome(NodeListaPlanetas * topoLista) {
    NodeListaPlanetas * nodeAtual = topoLista;
    NodeListaPlanetas * nodeAux;
    while (nodeAtual != NULL) {
        nodeAux = nodeAtual -> next;
        while (nodeAux != NULL) {
            if (strcmp(nodeAtual -> planeta -> nome, nodeAux -> planeta -> nome) > 0)
                swapPlanetasNodes(nodeAtual, nodeAux);
            nodeAux = nodeAux -> next;
        }
        if (nodeAtual -> planeta -> listaSatelites != NULL)
            sortSatelitesNome(nodeAtual -> planeta -> listaSatelites);
        nodeAtual = nodeAtual -> next;
    }
}

void sortSatelitesUA(NodeListaSatelites * topoLista) {
    NodeListaSatelites * nodeAtual = topoLista;
    NodeListaSatelites * nodeAux;
    while (nodeAtual != NULL) {
        nodeAux = nodeAtual -> next;
        while (nodeAux != NULL) {
            if ( * nodeAtual -> satelite -> distanciaPlaneta > * nodeAux -> satelite -> distanciaPlaneta)
                swapSatelitesNodes(nodeAtual, nodeAux);
            nodeAux = nodeAux -> next;
        }
        nodeAtual = nodeAtual -> next;
    }
}

//Bubble Sort
void sortAstrosUA(NodeListaPlanetas * topoLista) {
    NodeListaPlanetas * nodeAtual = topoLista;
    NodeListaPlanetas * nodeAux;
    while (nodeAtual != NULL) {
        nodeAux = nodeAtual -> next;
        while (nodeAux != NULL) {
            if ( * nodeAtual -> planeta -> distanciaEstrela > * nodeAux -> planeta -> distanciaEstrela)
                swapPlanetasNodes(nodeAtual, nodeAux);
            nodeAux = nodeAux -> next;
        }
        if (nodeAtual -> planeta -> listaSatelites != NULL)
            sortSatelitesUA(nodeAtual -> planeta -> listaSatelites);
        nodeAtual = nodeAtual -> next;
    }
}

void aplicarFiltroOrdenacao(SistemaPlanetario * sp, int filtroOrdenacao) {
    switch (filtroOrdenacao) {
        case 2:
            sortAstrosNome(sp -> listaPlanetas);
            break;
        case 3:
            sortAstrosUA(sp -> listaPlanetas);
            break;
        default:
            break;
    }
}

void removerNodeListaSatelites(NodeListaSatelites ** topoLista, NodeListaSatelites ** nodeRemover) {
    if ( * topoLista == * nodeRemover) {
        * topoLista = ( * nodeRemover) -> next;
    } else {
        NodeListaSatelites * nodeAtual = * topoLista;
        while (nodeAtual -> next -> satelite -> nome != ( * nodeRemover) -> satelite -> nome) {
            nodeAtual = nodeAtual -> next;
        }
        nodeAtual -> next = ( * nodeRemover) -> next;
    }
    libertarSatelite(( * nodeRemover) -> satelite);
}

void removerNodeListaPlanetas(NodeListaPlanetas ** topoLista, NodeListaPlanetas ** nodeRemover) {
    if ( * topoLista == * nodeRemover) {
        * topoLista = ( * nodeRemover) -> next;
    } else {
        NodeListaPlanetas * nodeAtual = * topoLista;
        while (nodeAtual -> next -> planeta -> nome != ( * nodeRemover) -> planeta -> nome) {
            nodeAtual = nodeAtual -> next;
        }
        nodeAtual -> next = ( * nodeRemover) -> next;
    }
    libertarPlaneta(( * nodeRemover) -> planeta);
}

void aplicarFiltroNomeSatelites(NodeListaSatelites ** topoLista, char * filtroNome) {
    NodeListaSatelites * nodeActual = * topoLista;
    while (nodeActual != NULL) {
        if (!strstr(nodeActual -> satelite -> nome, filtroNome))
            removerNodeListaSatelites(topoLista, & nodeActual);
        nodeActual = nodeActual -> next;
    }
}

void aplicarFiltroNome(SistemaPlanetario * sp, char * filtroNome) {
    if (strcmp(filtroNome, "*") == 0)
        return;
    NodeListaPlanetas * nodeActual = sp -> listaPlanetas;
    while (nodeActual != NULL) {
        if (!strstr(nodeActual -> planeta -> nome, filtroNome))
            removerNodeListaPlanetas( & sp -> listaPlanetas, & nodeActual);
        else
            aplicarFiltroNomeSatelites( & nodeActual -> planeta -> listaSatelites, filtroNome);
        nodeActual = nodeActual -> next;
    }
}

void aplicarFiltroDistanciaSatelites(NodeListaSatelites ** topoLista,
                                     const float * filtroMin,
                                     const float * filtroMax) {
    NodeListaSatelites * nodeActual = * topoLista;
    while (nodeActual != NULL) {
        if ( * nodeActual -> satelite -> distanciaPlaneta < * filtroMin || * nodeActual -> satelite -> distanciaPlaneta > * filtroMax)
            removerNodeListaSatelites(topoLista, & nodeActual);
        nodeActual = nodeActual -> next;
    }
}

void aplicarFiltroDistancia(SistemaPlanetario * sp, float * filtroMin, float * filtroMax) {
    if ( * filtroMin < 0 && * filtroMax < 0)
        return;
    if ( * filtroMax < 0)
        *
                filtroMax = MAX_float;
    NodeListaPlanetas * nodeActual = sp -> listaPlanetas;
    while (nodeActual != NULL) {
        if ( * nodeActual -> planeta -> distanciaEstrela < * filtroMin || * nodeActual -> planeta -> distanciaEstrela > * filtroMax)
            removerNodeListaPlanetas( & sp -> listaPlanetas, & nodeActual);
        else
            aplicarFiltroDistanciaSatelites( & nodeActual -> planeta -> listaSatelites, filtroMin, filtroMax);
        nodeActual = nodeActual -> next;
    }
}

void aplicarFiltros(SistemaPlanetario * sp) {
    int * filtroOrdenacao = alocarMemoria(sizeof(int));
    char * filtroNome = alocarMemoria(BUFFER_SIZE);
    float * filtroMin = alocarMemoria(sizeof(float));
    float * filtroMax = alocarMemoria(sizeof(float));
    inputFiltrosOrdenacao(filtroOrdenacao, filtroNome, filtroMin, filtroMax);
    aplicarFiltroOrdenacao(sp, * filtroOrdenacao);
    aplicarFiltroNome(sp, filtroNome);
    aplicarFiltroDistancia(sp, filtroMin, filtroMax);
}

void mostrarEstrela(Estrela * estrela) {
    printf("%s:\n", estrela -> nome);
}

void mostrarPlaneta(Planeta * planeta) {
    printf("- %s %.2f\n", planeta -> nome, * planeta -> distanciaEstrela);
}

void mostrarSatelite(Satelite * satelite) {
    printf("--- %s %.4f\n", satelite -> nome, * satelite -> distanciaPlaneta);
}

void mostrarAstros(NodeListaPlanetas * topoLista) {
    NodeListaPlanetas * nodeActualPlanetas = topoLista;
    NodeListaSatelites * nodeActualSatelites;
    while (nodeActualPlanetas != NULL) {
        mostrarPlaneta(nodeActualPlanetas -> planeta);
        nodeActualSatelites = nodeActualPlanetas -> planeta -> listaSatelites;
        while (nodeActualSatelites != NULL) {
            mostrarSatelite(nodeActualSatelites -> satelite);
            nodeActualSatelites = nodeActualSatelites -> next;
        }
        nodeActualPlanetas = nodeActualPlanetas -> next;
    }
}

void mostrarInfoSistema(SistemaPlanetario * sp) {
    if(sp->listaPlanetas == NULL)
        printf("O sistema planetário está vazio\n");
    if(strcmp(sp->estrela->nome, "" ) == 0)
        printf("O sistema planetário não tem uma estrela\n");
    else
    {
        mostrarEstrela(sp -> estrela);
        mostrarAstros(sp -> listaPlanetas);
    }
}

int main() {
    SistemaPlanetario * sp = criarSistemaPlanetario();
    aplicarFiltros(sp);
    mostrarInfoSistema(sp);
    libertarSistemaPlanetario(sp);
    return 0;
}
