#include "graphics.h"
#include "network.h"
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Headers específicos para criação de diretórios
#ifdef _WIN32
    #include <direct.h>  // Para _mkdir e _access
    #include <io.h>      // Para _access
    #define PATH_SEPARATOR '\\'
    #define MKDIR(path) _mkdir(path)
    #define ACCESS(path) _access(path, 0)
#else
    #include <sys/stat.h> // Para mkdir
    #include <unistd.h>   // Para access
    #define PATH_SEPARATOR '/'
    #define MKDIR(path) mkdir(path, 0700)
    #define ACCESS(path) access(path, F_OK)
#endif

// Variáveis globais
GLfloat rotationAngle = 0.0f;
GLfloat cameraAngleX = 0.0f, cameraAngleY = 0.0f;
GLfloat cameraDistance = 10.0f;
int lastMouseX, lastMouseY;
int frame = 0;
bool mouseDown = false;
bool paused = false;
float networkUpdateSpeed = 0.2f; // velocidade da dinamica = 1/(numero de atualizacoes por segundo)

void drawNode(GLfloat x, GLfloat y, GLfloat z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glutSolidSphere(0.15, 20, 20);
    glPopMatrix();
}

void drawLine(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2) {
    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 1.0); // Cor branca para as linhas de conexão
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y2, z2);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Posiciona e orienta a câmera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraDistance * sin(cameraAngleY) * cos(cameraAngleX),
              cameraDistance * sin(cameraAngleX),
              cameraDistance * cos(cameraAngleY) * cos(cameraAngleX),
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);
    
    // Aplica a rotação contínua
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);
    
    // Desenha as conexões (arestas)
    for (int i = 0; i < NUM_NODES; ++i) {
        for (int j = i + 1; j < NUM_NODES; ++j) {
            if (adjacencyMatrix[i][j]) {
                drawLine(nodes[i].x, nodes[i].y, nodes[i].z, 
                         nodes[j].x, nodes[j].y, nodes[j].z);
            }
        }
    }
    
    // Desenha os nós com base nos seus estados
    for (int i = 0; i < NUM_NODES; ++i) {
        if (nodesSelected[i]) {
            glColor3f(0.0, 1.0, 0.0); // Cor verde para nós selecionados
        } else if (nodes[i].state > 0) {
            glColor3f(1.0, 0.0, 0.0); // Cor vermelha para estado +1
        } else {
            glColor3f(0.0, 0.0, 1.0); // Cor azul para estado -1
        }
        drawNode(nodes[i].x, nodes[i].y, nodes[i].z);
    }
    
    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)width / (float)height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void savePPM(int frame) {
    // Cria diretório se não existir (portável)
    if (ACCESS("frr_save") == -1) {
        MKDIR("frr_save");
    }
    
    char filename[50];
    sprintf(filename, "frr_save%cframe%d.ppm", PATH_SEPARATOR, frame);
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }
    
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    
    fprintf(fp, "P6\n%d %d\n255\n", width, height);
    unsigned char *pixels = (unsigned char *)malloc(3 * width * height);
    
    if (!pixels) {
        perror("Erro ao alocar memória");
        fclose(fp);
        return;
    }
    
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    
    // Aloca buffer para pixels invertidos
    unsigned char *invertedPixels = (unsigned char *)malloc(3 * width * height);
    if (!invertedPixels) {
        perror("Erro ao alocar memória para inversão");
        free(pixels);
        fclose(fp);
        return;
    }
    
    // Inverte verticalmente
    for (int y = 0; y < height; y++) {
        memcpy(invertedPixels + 3 * width * y, 
               pixels + 3 * width * (height - 1 - y), 
               3 * width);
    }
    
    fwrite(invertedPixels, 3, width * height, fp);
    fclose(fp);
    free(pixels);
    free(invertedPixels);
}

void idle() {
    if (!paused) {
        glutPostRedisplay();
        // savePPM(frame); // Descomente para salvar frames
        frame++;
        rotationAngle -= 0.2f;
        if (rotationAngle >= 360.0f) {
            rotationAngle -= 360.0f;
        }
    }
}

void updateTimer(int value) {
    (void)value;
    if (!paused) {
        updateNetwork();
        glutPostRedisplay();
    }
    glutTimerFunc((int)(networkUpdateSpeed * 1000), updateTimer, 0);
}

void keyboard(unsigned char key, int x, int y) {
    (void)x;
    (void)y;
    switch (key) {
        case 32: // Tecla de espaço
            reinitializeWithLastConfig(); // Reinicia com mesma configuração
            glutPostRedisplay();
            break;
        case 27: // Tecla Esc
            freeNetwork(); // Libera memória antes de sair
            exit(0);
            break;
        case '+': // Aumenta velocidade
            networkUpdateSpeed *= 0.5f;
            if (networkUpdateSpeed < 0.001f) networkUpdateSpeed = 0.001f;
            printf("Velocidade: %.4f\n", networkUpdateSpeed);
            break;
        case '-': // Diminui velocidade
            networkUpdateSpeed *= 2.0f;
            if (networkUpdateSpeed > 1.0f) networkUpdateSpeed = 1.0f;
            printf("Velocidade: %.4f\n", networkUpdateSpeed);
            break;
        case 'w': // Aproxima câmera
            cameraDistance -= 0.5f;
            if (cameraDistance < 1.0f) cameraDistance = 1.0f;
            glutPostRedisplay();
            break;
        case 's': // Afasta câmera
            cameraDistance += 0.5f;
            glutPostRedisplay();
            break;
        case 'p': // Pausa/despausa
            paused = !paused;
            if (paused) {
                glutIdleFunc(NULL);
                printf("Simulação pausada\n");
            } else {
                glutIdleFunc(idle);
                glutTimerFunc((int)(networkUpdateSpeed * 1000), updateTimer, 0);
                printf("Simulação retomada\n");
            }
            break;
        case 'r': // Reseta rotação
            rotationAngle = 0.0f;
            cameraAngleX = 0.0f;
            cameraAngleY = 0.0f;
            glutPostRedisplay();
            break;
        case 'i': // Mostra informações
            printf("\n=== ESTADO ATUAL ===\n");
            printf("Nós: %d\n", NUM_NODES);
            printf("Grau médio: %d\n", K_AVG);
            printf("Magnetização inicial: %.2f\n", MAG_INIT);
            printf("Ruído: %.2f\n", NOISE);
            // Usando os índices atuais para obter os nomes
            printf("Tipo: %s\n", networkTypes[currentNetworkTypeIndex].name);
            printf("Modelo de atualização: %s\n", updateModels[currentUpdateModelIndex].name);
            printf("Velocidade: %.4f\n", networkUpdateSpeed);
            printf("Componente principal: %d nós\n", maxComponentSize);
            printf("====================\n");
            break;
    }
}

void mouseMotion(int x, int y) {
    if (mouseDown) {
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;
        lastMouseX = x;
        lastMouseY = y;
        cameraAngleX += dy * 0.005f;
        cameraAngleY += dx * 0.005f;
        
        // Limita o ângulo vertical para evitar inversões
        if (cameraAngleX > M_PI/2) cameraAngleX = M_PI/2;
        if (cameraAngleX < -M_PI/2) cameraAngleX = -M_PI/2;
        
        glutPostRedisplay();
    }
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouseDown = true;
            lastMouseX = x;
            lastMouseY = y;
        } else if (state == GLUT_UP) {
            mouseDown = false;
        }
    } else if (button == 3) { // Scroll up
        cameraDistance -= 0.5f;
        if (cameraDistance < 1.0f) cameraDistance = 1.0f;
        glutPostRedisplay();
    } else if (button == 4) { // Scroll down
        cameraDistance += 0.5f;
        glutPostRedisplay();
    }
}