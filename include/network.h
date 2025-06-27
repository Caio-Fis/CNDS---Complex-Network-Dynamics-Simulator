#ifndef NETWORK_H
#define NETWORK_H

#include <GL/glut.h>
#include <stdbool.h>

// Parâmetros configuráveis pelo usuário (valores padrão)
extern int NUM_NODES;
extern float MAG_INIT;
extern int K_AVG;
extern float NOISE;
extern int SELECT_NODES;

#define PI 3.14159265358979323846
#define RANDOM_NUM ((float)(rand()))/(float)(RAND_MAX+1.0)
#define CUBE_SIZE 5.0

typedef struct {
    GLfloat x, y, z;
    int state;
    int neighbors[1000];
    int numNeighbors;
    bool selected;
} Node;

// Estrutura para representar um tipo de rede
typedef struct {
    const char* name;
    void (*initializeFunc)();
} NetworkType;

// Estrutura para representar um modelo de atualização
typedef struct {
    const char* name;
    void (*updateFunc)();
} UpdateModelType;

typedef struct {
    int num_nodes;
    float mag_init;
    int k_avg;
    float noise;
    int select_nodes;
    int network_type_index;
    int update_model_index;
} NetworkConfig;

extern Node* nodes;
extern int** adjacencyMatrix;
extern int* visited;
extern int* ComponentSizes;
extern int idComponent, maxComponentSize, MaxIdComponent;
extern int smallerMaxComponentSize;
extern int* selecNodes;
extern bool* nodesSelected;
extern float w;

extern NetworkType networkTypes[];
extern const int numNetworkTypes;
extern UpdateModelType updateModels[];
extern const int numUpdateModels;
extern int currentNetworkTypeIndex;
extern int currentUpdateModelIndex;

extern NetworkConfig lastConfig;
extern bool configSaved;

void initializeNetwork();
double distance_periodic(double x1, double y1, double z1, double x2, double y2, double z2);
void dfs(int* visited, int current_vertex, int idComponent);
void findComponents(int* visited);
void findLargestIsland(int* visited, int* ComponentSizes);
void updateAdjacencyMatrix(int* visited, int MaxIdComponent);
void RandomNetwork();
void continuosNetwork();
void updateNetwork();
void updateNetwork_MajorityVote();
void updateNetwork_VoteModel();
void init();
void freeNetwork();
void saveCurrentConfig();
void reinitializeWithLastConfig();
void clearScreen();

#endif