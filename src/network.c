#include "network.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

// Parâmetros configuráveis pelo usuário
int NUM_NODES = 100;
float MAG_INIT = 0.55;
int K_AVG = 10;
float NOISE = 0.0;
int SELECT_NODES = 1;

// Variáveis globais
Node* nodes = NULL;
int** adjacencyMatrix = NULL;
int* visited = NULL;
int* ComponentSizes = NULL;
int idComponent = 1, maxComponentSize = 0, MaxIdComponent = 0;
int smallerMaxComponentSize = 0;
int* selecNodes = NULL;
bool* nodesSelected = NULL;
float w = 0.0;

// Registro de tipos de rede
NetworkType networkTypes[] = {
    {"Aleatoria", RandomNetwork},
    {"Continua", continuosNetwork}
};
const int numNetworkTypes = sizeof(networkTypes) / sizeof(NetworkType);

// Registro de modelos de atualização
UpdateModelType updateModels[] = {
    {"Majority Vote", updateNetwork_MajorityVote},
    {"Vote Model", updateNetwork_VoteModel}
};
const int numUpdateModels = sizeof(updateModels) / sizeof(UpdateModelType);

// Índices atuais
int currentNetworkTypeIndex = 0;
int currentUpdateModelIndex = 0;

// Armazenamento da última configuração
NetworkConfig lastConfig;
bool configSaved = false;

void initializeNetwork() {
    srand(time(NULL));
    for (int i = 0; i < NUM_NODES; ++i) {
        nodes[i].x = CUBE_SIZE * ((GLfloat)rand() / RAND_MAX - 0.5);
        nodes[i].y = CUBE_SIZE * ((GLfloat)rand() / RAND_MAX - 0.5);
        nodes[i].z = CUBE_SIZE * ((GLfloat)rand() / RAND_MAX - 0.5);
        nodes[i].state = ((float)rand() / RAND_MAX < MAG_INIT) ? +1 : -1;
        nodes[i].numNeighbors = 0;
        for (int j = 0; j < NUM_NODES; j++) {
            nodes[i].neighbors[j] = -1;
        }
    }
}

double distance_periodic(double x1, double y1, double z1, double x2, double y2, double z2) {
    double dx = fabs(x1 - x2);
    if (dx > CUBE_SIZE / 2.0)
        dx = CUBE_SIZE - dx;
    double dy = fabs(y1 - y2);
    if (dy > CUBE_SIZE / 2.0)
        dy = CUBE_SIZE - dy;
    double dz = fabs(z1 - z2);
    if (dz > CUBE_SIZE / 2.0)
        dz = CUBE_SIZE - dz;
    return sqrt(dx * dx + dy * dy + dz * dz);
}

void dfs(int* visited, int current_vertex, int idComponent) {
    visited[current_vertex] = idComponent;
    for (int i = 0; i < NUM_NODES; i++) {
        if (adjacencyMatrix[current_vertex][i] == 1 && visited[i] == 0) {
            dfs(visited, i, idComponent);
        }
    }
}

void findComponents(int* visited) {
    idComponent = 1;
    for (int i = 0; i < NUM_NODES; i++) {
        if (visited[i] == 0) {
            dfs(visited, i, idComponent);
            idComponent++;
        }
    }
}

void findLargestIsland(int* visited, int* ComponentSizes) {
    maxComponentSize = 0;
    MaxIdComponent = 0;
    
    // Resetar ComponentSizes
    for (int i = 0; i <= idComponent; i++) {
        ComponentSizes[i] = 0;
    }
    
    for (int i = 0; i < NUM_NODES; i++) {
        ComponentSizes[visited[i]]++;
        if (ComponentSizes[visited[i]] > maxComponentSize) {
            maxComponentSize = ComponentSizes[visited[i]];
            MaxIdComponent = visited[i];
        }
    }
    
    if (maxComponentSize < smallerMaxComponentSize) {
        smallerMaxComponentSize = maxComponentSize;
    }
}

void updateAdjacencyMatrix(int* visited, int MaxIdComponent) {
    for (int i = 0; i < NUM_NODES; i++) {
        if (visited[i] != MaxIdComponent) {
            for (int j = 0; j < NUM_NODES; j++) {
                adjacencyMatrix[i][j] = 0;
                adjacencyMatrix[j][i] = 0;
            }
        }
    }
    
    // Reconstruir listas de vizinhos
    for (int i = 0; i < NUM_NODES; i++) {
        nodes[i].numNeighbors = 0;
        for (int j = 0; j < NUM_NODES; j++) {
            if (adjacencyMatrix[i][j] == 1) {
                nodes[i].neighbors[nodes[i].numNeighbors++] = j;
            }
        }
    }
}

void RandomNetwork() {
    int nedges = 0, i = 0, j = 0; 
    
    // Inicializa matriz com zeros
    for (int i = 0; i < NUM_NODES; i++) {
        for (int j = 0; j < NUM_NODES; j++) {
            adjacencyMatrix[i][j] = 0;
        }
    }
    
    int numConnections = (K_AVG * NUM_NODES) / 2;
    while (nedges < numConnections) {
        i = rand() % NUM_NODES;
        j = rand() % NUM_NODES;
        if (i == j) continue;
        if (adjacencyMatrix[i][j] == 0) {
            adjacencyMatrix[i][j] = adjacencyMatrix[j][i] = 1;
            nedges++;
        }
    }
    
    // Inicializa arrays de componentes
    memset(visited, 0, NUM_NODES * sizeof(int));
    memset(ComponentSizes, 0, NUM_NODES * sizeof(int));
    
    findComponents(visited);
    findLargestIsland(visited, ComponentSizes);
    updateAdjacencyMatrix(visited, MaxIdComponent);
}

void continuosNetwork() {
    double radius = CUBE_SIZE * sqrt((double)K_AVG / (PI * NUM_NODES));
    
    // Inicializa matriz com zeros
    for (int i = 0; i < NUM_NODES; i++) {
        for (int j = 0; j < NUM_NODES; j++) {
            adjacencyMatrix[i][j] = 0;
        }
    }
    
    for (int i = 0; i < NUM_NODES; i++) {
        for (int j = i + 1; j < NUM_NODES; j++) {
            if (distance_periodic(nodes[i].x, nodes[i].y, nodes[i].z, 
                                  nodes[j].x, nodes[j].y, nodes[j].z) <= radius) {
                adjacencyMatrix[i][j] = 1;
                adjacencyMatrix[j][i] = 1;
            }
        }
    }
    
    // Inicializa arrays de componentes
    memset(visited, 0, NUM_NODES * sizeof(int));
    memset(ComponentSizes, 0, NUM_NODES * sizeof(int));
    
    findComponents(visited);
    findLargestIsland(visited, ComponentSizes);
    updateAdjacencyMatrix(visited, MaxIdComponent);
}

void updateNetwork_MajorityVote() {
    // Limpa seleções anteriores
    for (int i = 0; i < SELECT_NODES; i++) {
        selecNodes[i] = -1;
    }
    memset(nodesSelected, false, NUM_NODES * sizeof(bool));
    
    bool selected = false;

    for (int count = 0; count < SELECT_NODES; count++) {
        int sum = 0;
        int i = 0;
        int attempts = 0;

        // Seleciona nó aleatório na maior componente
        do {
            i = rand() % NUM_NODES;
            attempts++;
            if (attempts > NUM_NODES * 2) break;
        } while (visited[i] != MaxIdComponent);

        if (visited[i] != MaxIdComponent) continue;
        
        nodesSelected[i] = true;
        
        // Soma estados dos vizinhos
        for (int z = 0; z < nodes[i].numNeighbors; ++z) {
            int neighbor = nodes[i].neighbors[z];
            if (neighbor >= 0 && neighbor < NUM_NODES) {
                sum += nodes[neighbor].state;
            }
        }
        
        // Majority Vote Model
        int majority = 0;
        if (sum > 0) majority = 1;
        else if (sum < 0) majority = -1;
        
        w = (1.0 - (1.0 - 2.0 * NOISE) * nodes[i].state * majority) / 2.0;
        float p = (float)rand() / RAND_MAX;

        if (p < w) {
            selected = true;
            selecNodes[count] = i;
        }
    }

    // Atualiza estados dos nós selecionados
    if (selected) {
        for (int count = 0; count < SELECT_NODES; count++) {
            if (selecNodes[count] != -1) {
                nodes[selecNodes[count]].state *= -1;
            }
        }
    }
}

void updateNetwork_VoteModel() {
    // Limpa seleções anteriores
    for (int i = 0; i < SELECT_NODES; i++) {
        selecNodes[i] = -1;
    }
    memset(nodesSelected, false, NUM_NODES * sizeof(bool));
    
    for (int count = 0; count < SELECT_NODES; count++) {
        int sum = 0;
        int i = 0;
        int attempts = 0;

        // Seleciona nó aleatório na maior componente
        do {
            i = rand() % NUM_NODES;
            attempts++;
            if (attempts > NUM_NODES * 2) break;
        } while (visited[i] != MaxIdComponent);

        if (visited[i] != MaxIdComponent) continue;
        
        nodesSelected[i] = true;
        
        // Soma estados dos vizinhos
        for (int z = 0; z < nodes[i].numNeighbors; ++z) {
            int neighbor = nodes[i].neighbors[z];
            if (neighbor >= 0 && neighbor < NUM_NODES) {
                sum += nodes[neighbor].state;
            }
        }
        
        // Determina novo estado
        int newState = nodes[i].state;
        if (sum > 0) newState = +1;
        else if (sum < 0) newState = -1;
        
        // Aplica ruído
        float p = (float)rand() / RAND_MAX;
        if (p < NOISE) newState = -newState;
        
        // Verifica se estado mudou
        if (newState != nodes[i].state) {
            selecNodes[count] = i;
            nodes[i].state = newState; // Atualiza imediatamente
        }
    }
}

void updateNetwork() {
    updateModels[currentUpdateModelIndex].updateFunc();
}

void saveCurrentConfig() {
    lastConfig.num_nodes = NUM_NODES;
    lastConfig.mag_init = MAG_INIT;
    lastConfig.k_avg = K_AVG;
    lastConfig.noise = NOISE;
    lastConfig.select_nodes = SELECT_NODES;
    lastConfig.network_type_index = currentNetworkTypeIndex;
    lastConfig.update_model_index = currentUpdateModelIndex;
    configSaved = true;
    
    printf("\nConfiguracao salva para reinicializacao:\n");
    printf("Nos: %d, Grau medio: %d, Tipo: %s\n", 
           NUM_NODES, K_AVG, networkTypes[currentNetworkTypeIndex].name);
    printf("Modelo: %s\n", updateModels[currentUpdateModelIndex].name);
    printf("Magnetizacao: %.2f, Ruido: %.2f, Selecionados: %d\n",
           MAG_INIT, NOISE, SELECT_NODES);
}

void reinitializeWithLastConfig() {
    if (!configSaved) {
        printf("Nenhuma configuracao anterior encontrada. Usando valores padrao.\n");
        lastConfig.num_nodes = 100;
        lastConfig.mag_init = 0.55;
        lastConfig.k_avg = 10;
        lastConfig.noise = 0.0;
        lastConfig.select_nodes = 1;
        lastConfig.network_type_index = 0;
        lastConfig.update_model_index = 0;
    }
    
    // Atualiza parâmetros globais
    NUM_NODES = lastConfig.num_nodes;
    MAG_INIT = lastConfig.mag_init;
    K_AVG = lastConfig.k_avg;
    NOISE = lastConfig.noise;
    SELECT_NODES = lastConfig.select_nodes;
    currentNetworkTypeIndex = lastConfig.network_type_index;
    currentUpdateModelIndex = lastConfig.update_model_index;
    
    // Libera recursos anteriores
    freeNetwork();
    
    // Realoca memória
    nodes = (Node*)malloc(NUM_NODES * sizeof(Node));
    if (!nodes) {
        perror("Erro ao alocar memoria para nos");
        exit(EXIT_FAILURE);
    }
    
    selecNodes = (int*)malloc(SELECT_NODES * sizeof(int));
    if (!selecNodes) {
        perror("Erro ao alocar memoria para nos selecionados");
        free(nodes);
        exit(EXIT_FAILURE);
    }
    
    nodesSelected = (bool*)malloc(NUM_NODES * sizeof(bool));
    if (!nodesSelected) {
        perror("Erro ao alocar memoria para flags de selecao");
        free(nodes);
        free(selecNodes);
        exit(EXIT_FAILURE);
    }
    
    visited = (int*)malloc(NUM_NODES * sizeof(int));
    if (!visited) {
        perror("Erro ao alocar memoria para visitados");
        free(nodes);
        free(selecNodes);
        free(nodesSelected);
        exit(EXIT_FAILURE);
    }
    
    ComponentSizes = (int*)malloc(NUM_NODES * sizeof(int));
    if (!ComponentSizes) {
        perror("Erro ao alocar memoria para tamanhos de componentes");
        free(nodes);
        free(selecNodes);
        free(nodesSelected);
        free(visited);
        exit(EXIT_FAILURE);
    }
    
    adjacencyMatrix = (int**)malloc(NUM_NODES * sizeof(int*));
    if (!adjacencyMatrix) {
        perror("Erro ao alocar memoria para matriz de adjacencia");
        free(nodes);
        free(selecNodes);
        free(nodesSelected);
        free(visited);
        free(ComponentSizes);
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < NUM_NODES; i++) {
        adjacencyMatrix[i] = (int*)malloc(NUM_NODES * sizeof(int));
        if (!adjacencyMatrix[i]) {
            perror("Erro ao alocar linha da matriz de adjacencia");
            for (int j = 0; j < i; j++) free(adjacencyMatrix[j]);
            free(adjacencyMatrix);
            free(nodes);
            free(selecNodes);
            free(nodesSelected);
            free(visited);
            free(ComponentSizes);
            exit(EXIT_FAILURE);
        }
    }

    // Reinicializa a rede
    initializeNetwork();
    networkTypes[currentNetworkTypeIndex].initializeFunc();
    
    smallerMaxComponentSize = NUM_NODES;
    
    printf("\n=== REDE REINICIALIZADA ===\n");
    printf("Nos: %d, Grau medio: %d\n", NUM_NODES, K_AVG);
    printf("Magnetizacao inicial: %.2f, Ruido: %.2f\n", MAG_INIT, NOISE);
    printf("Tipo: %s, Nos selecionados: %d\n", 
           networkTypes[currentNetworkTypeIndex].name, SELECT_NODES);
    printf("Modelo de atualizacao: %s\n", updateModels[currentUpdateModelIndex].name);
    printf("==============================================\n");
}

void freeNetwork() {
    if (nodes) free(nodes);
    if (selecNodes) free(selecNodes);
    if (nodesSelected) free(nodesSelected);
    if (visited) free(visited);
    if (ComponentSizes) free(ComponentSizes);
    
    if (adjacencyMatrix) {
        for (int i = 0; i < NUM_NODES; i++) {
            if (adjacencyMatrix[i]) free(adjacencyMatrix[i]);
        }
        free(adjacencyMatrix);
    }
    
    // Reset pointers
    nodes = NULL;
    selecNodes = NULL;
    nodesSelected = NULL;
    visited = NULL;
    ComponentSizes = NULL;
    adjacencyMatrix = NULL;
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void init() {
    clearScreen();
    printf("\n=============================================\n");
    printf("CONFIGURACAO DA REDE COMPLEXA\n");
    printf("=============================================\n");

    // Seleção de tipo de rede
    printf("\nEscolha o tipo de rede:\n");
    for (int i = 0; i < numNetworkTypes; i++) {
        printf("%d. %s\n", i+1, networkTypes[i].name);
    }
    printf("Sua escolha (1-%d): ", numNetworkTypes);
    int networkChoice;
    while (scanf("%d", &networkChoice) != 1 || networkChoice < 1 || networkChoice > numNetworkTypes) {
        printf("Opcao invalida! Digite 1-%d: ", numNetworkTypes);
        while (getchar() != '\n');
    }
    currentNetworkTypeIndex = networkChoice - 1;
    
    // Seleção de modelo de atualização
    printf("\nEscolha o modelo de atualizacao:\n");
    for (int i = 0; i < numUpdateModels; i++) {
        printf("%d. %s\n", i+1, updateModels[i].name);
    }
    printf("Sua escolha (1-%d): ", numUpdateModels);
    int modelChoice;
    while (scanf("%d", &modelChoice) != 1 || modelChoice < 1 || modelChoice > numUpdateModels) {
        printf("Opcao invalida! Digite 1-%d: ", numUpdateModels);
        while (getchar() != '\n');
    }
    currentUpdateModelIndex = modelChoice - 1;
    
    // Coleta de parâmetros com validação
    printf("Numero de nos (2-1000): ");
    while (scanf("%d", &NUM_NODES) != 1 || NUM_NODES < 2 || NUM_NODES > 1000) {
        printf("Valor invalido! Digite um numero entre 2 e 1000: ");
        while (getchar() != '\n');
    }

    printf("Grau medio (1-%d): ", NUM_NODES-1);
    while (scanf("%d", &K_AVG) != 1 || K_AVG < 1 || K_AVG >= NUM_NODES) {
        printf("Valor invalido! Digite um numero entre 1 e %d: ", NUM_NODES-1);
        while (getchar() != '\n');
    }
    
    printf("Magnetizacao inicial (0.0-1.0): ");
    while (scanf("%f", &MAG_INIT) != 1 || MAG_INIT < 0.0 || MAG_INIT > 1.0) {
        printf("Valor invalido! Digite um numero entre 0.0 e 1.0: ");
        while (getchar() != '\n');
    }
    
    printf("Ruido (0.0-1.0): ");
    while (scanf("%f", &NOISE) != 1 || NOISE < 0.0 || NOISE > 1.0) {
        printf("Valor invalido! Digite um numero entre 0.0 e 1.0: ");
        while (getchar() != '\n');
    }
    
    printf("Nos selecionados por passo (1-%d): ", NUM_NODES);
    int max_select = NUM_NODES > 1 ? NUM_NODES : 1;
    while (scanf("%d", &SELECT_NODES) != 1 || SELECT_NODES < 1 || SELECT_NODES > max_select) {
        printf("Valor invalido! Digite um numero entre 1 e %d: ", max_select);
        while (getchar() != '\n');
    }
    
    printf("=============================================\n\n");

    // Aloca memória
    nodes = (Node*)malloc(NUM_NODES * sizeof(Node));
    if (!nodes) {
        perror("Erro ao alocar memoria para nos");
        exit(EXIT_FAILURE);
    }
    
    selecNodes = (int*)malloc(SELECT_NODES * sizeof(int));
    if (!selecNodes) {
        perror("Erro ao alocar memoria para nos selecionados");
        free(nodes);
        exit(EXIT_FAILURE);
    }
    
    nodesSelected = (bool*)malloc(NUM_NODES * sizeof(bool));
    if (!nodesSelected) {
        perror("Erro ao alocar memoria para flags de selecao");
        free(nodes);
        free(selecNodes);
        exit(EXIT_FAILURE);
    }
    
    visited = (int*)malloc(NUM_NODES * sizeof(int));
    if (!visited) {
        perror("Erro ao alocar memoria para visitados");
        free(nodes);
        free(selecNodes);
        free(nodesSelected);
        exit(EXIT_FAILURE);
    }
    
    ComponentSizes = (int*)malloc(NUM_NODES * sizeof(int));
    if (!ComponentSizes) {
        perror("Erro ao alocar memoria para tamanhos de componentes");
        free(nodes);
        free(selecNodes);
        free(nodesSelected);
        free(visited);
        exit(EXIT_FAILURE);
    }
    
    adjacencyMatrix = (int**)malloc(NUM_NODES * sizeof(int*));
    if (!adjacencyMatrix) {
        perror("Erro ao alocar memoria para matriz de adjacencia");
        free(nodes);
        free(selecNodes);
        free(nodesSelected);
        free(visited);
        free(ComponentSizes);
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < NUM_NODES; i++) {
        adjacencyMatrix[i] = (int*)malloc(NUM_NODES * sizeof(int));
        if (!adjacencyMatrix[i]) {
            perror("Erro ao alocar linha da matriz de adjacencia");
            for (int j = 0; j < i; j++) free(adjacencyMatrix[j]);
            free(adjacencyMatrix);
            free(nodes);
            free(selecNodes);
            free(nodesSelected);
            free(visited);
            free(ComponentSizes);
            exit(EXIT_FAILURE);
        }
    }

    // Inicializa a rede
    initializeNetwork();
    networkTypes[currentNetworkTypeIndex].initializeFunc();

    // Salva a configuração
    saveCurrentConfig();
    
    smallerMaxComponentSize = NUM_NODES;
    
    // Configurações OpenGL
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
}