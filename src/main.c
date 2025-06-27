#include <GL/glut.h>
#include <stdio.h>
#include "network.h"
#include "graphics.h"

int main(int argc, char** argv) {
    // Inicializa a rede com parâmetros do usuário
    init();

    // Inicializa o GLUT e cria a janela
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 900);
    glutCreateWindow("Simulacao de Rede Complexa - Modelo de Ising");
    
    // Configura callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    
    // Inicia o timer para atualizações da rede
    glutTimerFunc((int)(networkUpdateSpeed * 1000), updateTimer, 0);
    
    // Configura função idle para animação contínua
    glutIdleFunc(idle);
    
    // Mensagem inicial com instruções
    printf("\n=== INSTRUCOES ===\n");
    printf("ESPACO: Reiniciar simulacao\n");
    printf("ESC: Sair\n");
    printf("+/-: Aumentar/diminuir velocidade\n");
    printf("W/S: Zoom in/out\n");
    printf("P: Pausar/retomar\n");
    printf("R: Resetar camera\n");
    printf("I: Mostrar informacoes\n");
    printf("Arrastar mouse: Rotacionar cena\n");
    printf("Scroll mouse: Zoom\n");
    printf("==================\n\n");

    // Inicia o loop principal
    glutMainLoop();
    
    // Libera memória antes de sair (embora glutMainLoop() geralmente não retorne)
    freeNetwork();
    
    return 0;
}