#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/glut.h>
#include <stdbool.h>
#include "network.h"  // Para acessar Node, NUM_NODES, etc.

// Variáveis globais para controle da visualização
extern int frame;
extern GLfloat rotationAngle;
extern GLfloat cameraAngleX, cameraAngleY, cameraDistance;
extern bool paused;
extern float networkUpdateSpeed;

// Protótipos de funções
void drawNode(GLfloat x, GLfloat y, GLfloat z);
void drawLine(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2);
void display();
void reshape(int width, int height);
void savePPM(int frame);
void idle();
void updateTimer(int value);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void mouseMotion(int x, int y);

#endif