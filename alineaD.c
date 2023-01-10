#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <math.h>

#define BUFFER_SIZE 64

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

typedef struct nodePlanoViagem {
    char * nomeAstro;
    float * distancia;
    struct nodePlanoViagem * next;
} NodePlanoViagem;

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

typedef struct {
    Estrela * estrela;
    Planeta * planeta;
    Satelite * satelite;
} IdAstro;

typedef struct {
    IdAstro * idAstroPartida;
    IdAstro * idAstroChegada;
    NodePlanoViagem * topoListaPlanoViagem;
    int * duracaoDias;
} Viagem;

void * alocarMemoria(size_t tamanho) {
    void * ptrAlloc = malloc(tamanho);
    if (ptrAlloc == NULL) {
        printf("Falha ao alocar memória.");
        exit(1);
    }
    return ptrAlloc;
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
    free(nodeRemover);
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
    free(nodeRemover);
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

void mostrarIdAstro(IdAstro * idAstro) {
    printf("%s-%s", idAstro -> estrela -> nome, idAstro -> planeta -> nome);
    if (idAstro -> satelite != NULL)
        printf("-%s", idAstro -> satelite -> nome);
    printf("\n");
}

void mostrarAnos(int anos) {
    switch (anos) {
        case 0:
            break;
        case 1:
            printf("1 ano ");
            break;
        default:
            printf("%d anos ", anos);
    }
}

void mostrarMeses(int meses) {
    switch (meses) {
        case 0:
            break;
        case 1:
            printf("1 mês ");
            break;
        default:
            printf("%d meses ", meses);
    }
}

void mostrarDias(int dias) {
    switch (dias) {
        case 0:
            break;
        case 1:
            printf("1 dia ");
            break;
        default:
            printf("%d dias ", dias);
    }
}

void mostrarDuracaoViagem(int duracaoDias) {
    int anos, meses, dias;

    anos = duracaoDias / 360;
    duracaoDias -= anos * 360;
    meses = duracaoDias / 30;
    duracaoDias -= meses * 30;
    dias = duracaoDias;

    mostrarAnos(anos);
    mostrarMeses(meses);
    mostrarDias(dias);
    printf("\n");
}

void mostrarEstacoes(NodePlanoViagem * topoLista) {
    NodePlanoViagem * nodeAtual = topoLista;
    int astroEmComumEncontrado = 0;
    while (nodeAtual != NULL) {
        if (nodeAtual -> distancia == NULL)
            astroEmComumEncontrado = 1;
        printf("%s ", nodeAtual -> nomeAstro);
        if (!astroEmComumEncontrado) {
            printf("(%d) ", (int) ceilf( * nodeAtual -> distancia * 30));
        } else {
            if (nodeAtual -> next != NULL)
                printf("(%d) ", (int) ceilf( * nodeAtual -> next -> distancia * 30));
        }
        nodeAtual = nodeAtual -> next;
    }
    printf("\n");
}

void mostrarInfoViagem(Viagem * viagem) {

    printf("De: ");
    mostrarIdAstro(viagem -> idAstroPartida);
    printf("Para: ");
    mostrarIdAstro(viagem -> idAstroChegada);
    printf("Estações: ");
    mostrarEstacoes(viagem -> topoListaPlanoViagem);
    printf("Duração: ");
    mostrarDuracaoViagem( * viagem -> duracaoDias);
}

void inputViagem(char * nomeAstroPartida, char * nomeAstroChegada) {
    scanf("%s %s", nomeAstroPartida, nomeAstroChegada);
}

IdAstro * obterIdAstro(SistemaPlanetario * sp,
                       const char * nomeAstro) {
    IdAstro * idAstro = alocarMemoria(sizeof(IdAstro));
    idAstro -> estrela = sp -> estrela;
    NodeListaPlanetas * nodeAtualPlanetas = sp -> listaPlanetas;
    int astroEncontrado = 0;

    while (!astroEncontrado && nodeAtualPlanetas != NULL) {
        if (strcmp(nodeAtualPlanetas -> planeta -> nome, nomeAstro) == 0) {
            idAstro -> planeta = nodeAtualPlanetas -> planeta;
            idAstro -> satelite = NULL;
            astroEncontrado = 1;
        } else {
            NodeListaSatelites * nodeAtualSatelites = nodeAtualPlanetas -> planeta -> listaSatelites;
            while (!astroEncontrado && nodeAtualSatelites != NULL) {
                if (strcmp(nodeAtualSatelites -> satelite -> nome, nomeAstro) == 0) {
                    idAstro -> planeta = nodeAtualPlanetas -> planeta;
                    idAstro -> satelite = nodeAtualSatelites -> satelite;
                    astroEncontrado = 1;
                }
                nodeAtualSatelites = nodeAtualSatelites -> next;
            }
        }
        nodeAtualPlanetas = nodeAtualPlanetas -> next;
    }
    if (!astroEncontrado) {
        printf("Astro não encontrado");
        exit(1);
    }
    return idAstro;
}

int astroEstaNoPlano(NodePlanoViagem * topoLista, char * nomeAstro) {
    NodePlanoViagem * nodeAtual = topoLista;
    while (nodeAtual != NULL) {
        if (strcmp(nodeAtual -> nomeAstro, nomeAstro) == 0)
            return 1;
        nodeAtual = nodeAtual -> next;
    }
    return 0;
}

void adicionarNodePlanoViagem(NodePlanoViagem ** topoLista, char * nomeAstro, float * distanciaAstro) {
    NodePlanoViagem * novoNode = alocarMemoria(sizeof(NodePlanoViagem));
    novoNode -> nomeAstro = nomeAstro;
    novoNode -> distancia = distanciaAstro;
    novoNode -> next = NULL;
    if ( * topoLista == NULL) {
        * topoLista = novoNode;
    } else {
        NodePlanoViagem * nodeAtual = * topoLista;
        while (nodeAtual -> next != NULL) {
            nodeAtual = nodeAtual -> next;
        }
        nodeAtual -> next = novoNode;
    }
}

void anularDistanciaAstro(NodePlanoViagem ** topoLista, char * nomeAstro) {
    NodePlanoViagem * nodeAtual = * topoLista;
    while (nodeAtual != NULL) {
        if (strcmp(nodeAtual -> nomeAstro, nomeAstro) == 0) {
            nodeAtual -> distancia = NULL;
            return;
        }
        nodeAtual = nodeAtual -> next;
    }
}

void addAstroPlano(NodePlanoViagem ** topoLista, char * nomeAstro, float * distanciaAstro) {
    if (!astroEstaNoPlano( * topoLista, nomeAstro))
        adicionarNodePlanoViagem(topoLista, nomeAstro, distanciaAstro);
    else
        anularDistanciaAstro(topoLista, nomeAstro);
}

void addRotaAstrosPlanoViagem(NodePlanoViagem ** topoLista, IdAstro * idAstroPartida, IdAstro * idAstroChegada,
                              int adicionarEstrela) {
    if (idAstroPartida -> satelite != NULL)
        addAstroPlano(topoLista, idAstroPartida -> satelite -> nome, idAstroPartida -> satelite -> distanciaPlaneta);
    addAstroPlano(topoLista, idAstroPartida -> planeta -> nome, idAstroPartida -> planeta -> distanciaEstrela);
    if (adicionarEstrela)
        addAstroPlano(topoLista, idAstroPartida -> estrela -> nome, 0);

    if (adicionarEstrela)
        addAstroPlano(topoLista, idAstroChegada -> estrela -> nome, 0);
    addAstroPlano(topoLista, idAstroChegada -> planeta -> nome, idAstroChegada -> planeta -> distanciaEstrela);
    if (idAstroChegada -> satelite != NULL)
        addAstroPlano(topoLista, idAstroChegada -> satelite -> nome, idAstroChegada -> satelite -> distanciaPlaneta);
}

NodePlanoViagem * tracarPlanoViagem(IdAstro * idAstroPartida, IdAstro * idAstroChegada) {
    int adicionarEstrelaRota = 1;
    NodePlanoViagem * topoLista = NULL;
    if (idAstroChegada -> planeta == idAstroPartida -> planeta)
        adicionarEstrelaRota = 0;
    addRotaAstrosPlanoViagem( & topoLista, idAstroPartida, idAstroChegada, adicionarEstrelaRota);
    return topoLista;
}

int contarDiasViagem(Viagem * viagem) {
    NodePlanoViagem * nodeActual = viagem -> topoListaPlanoViagem;
    int diasViagem = 0;
    while (nodeActual != NULL) {
        if (nodeActual -> distancia != NULL)
            diasViagem = diasViagem + (int) ceilf( * nodeActual -> distancia * 30);
        nodeActual = nodeActual -> next;
    }
    return diasViagem;
}

Viagem * criarViagem(SistemaPlanetario * sp) {
    Viagem * viagem = alocarMemoria(sizeof(Viagem));
    viagem -> duracaoDias = alocarMemoria(sizeof(int));
    char * nomeAstroPartida = alocarMemoria(BUFFER_SIZE);
    char * nomeAstroChegada = alocarMemoria(BUFFER_SIZE);
    inputViagem(nomeAstroPartida, nomeAstroChegada);
    viagem -> idAstroPartida = obterIdAstro(sp, nomeAstroPartida);
    viagem -> idAstroChegada = obterIdAstro(sp, nomeAstroChegada);
    viagem -> topoListaPlanoViagem = tracarPlanoViagem(viagem -> idAstroPartida, viagem -> idAstroChegada);
    * viagem -> duracaoDias = contarDiasViagem(viagem) + 2;
    free(nomeAstroPartida);
    free(nomeAstroChegada);
    return viagem;
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

void libertarIdAstro(IdAstro * idAstro) {
    free(idAstro);
}

void libertarPlaneta(Planeta * planeta) {
    free(planeta -> nome);
    libertarListaSatelites(planeta -> listaSatelites);
    free(planeta);
}

void libertarPlanoViagem(NodePlanoViagem * nodePlanoViagem) {
    free(nodePlanoViagem);
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

void libertarViagem(Viagem * viagem) {
    libertarPlanoViagem(viagem -> topoListaPlanoViagem);
    libertarIdAstro(viagem -> idAstroPartida);
    libertarIdAstro(viagem -> idAstroChegada);
    free(viagem -> duracaoDias);
    free(viagem);
}

void libertarSistemaPlanetario(SistemaPlanetario * sp) {
    libertarEstrela(sp -> estrela);
    libertarListaPlanetas(sp -> listaPlanetas);
    free(sp);
}

int main() {
    SistemaPlanetario * sp = criarSistemaPlanetario();
    Viagem * viagem = criarViagem(sp);
    mostrarInfoViagem(viagem);
    libertarSistemaPlanetario(sp);
    libertarViagem(viagem);
    return 0;
}
