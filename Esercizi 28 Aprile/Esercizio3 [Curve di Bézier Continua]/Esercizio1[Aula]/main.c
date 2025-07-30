#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_POINTS 100  // Numero massimo di punti di controllo
#define STEP 100  // Numero di passi per il disegno della curva

// Definizione della struttura per un punto (x, y)
typedef struct {
    float x, y;
} Point;

// Array per memorizzare i punti di controllo
Point controlPoints[MAX_POINTS];
int pointCount = 0;  // Conteggio dei punti di controllo inseriti

// Funzione per calcolare un punto sulla curva di Bézier
Point deCasteljau(Point* P, int n, float t) {
    // Crea un array temporaneo per memorizzare i punti intermedi
    Point* temp = (Point*)malloc(n * sizeof(Point));
    
    // Copia i punti di controllo iniziali
    for (int i = 0; i < n; i++) {
        temp[i] = P[i];
    }

    // Calcola i punti intermedi della curva di Bézier usando il metodo di De Casteljau
    for (int r = 1; r < n; r++) {
        for (int i = 0; i < n - r; i++) {
            // Calcola i punti intermedi ponderati
            temp[i].x = (1 - t) * temp[i].x + t * temp[i + 1].x;
            temp[i].y = (1 - t) * temp[i].y + t * temp[i + 1].y;
        }
    }

    // Il punto finale della curva è il primo punto dell'array temporaneo
    Point result = temp[0];
    free(temp);  // Libera la memoria allocata
    return result;
}

// Funzione per la visualizzazione della scena
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);  // Pulisce lo schermo

    // Disegna i punti di controllo
    glPointSize(5);  // Imposta la dimensione del punto
    glColor3f(1, 0, 0);  // Colore rosso per i punti di controllo
    glBegin(GL_POINTS);
    for (int i = 0; i < pointCount; i++) {
        glVertex2f(controlPoints[i].x, controlPoints[i].y);  // Disegna ogni punto di controllo
    }
    glEnd();

    // Disegna le linee di controllo tra i punti
    glColor3f(0.7, 0.7, 0.7);  // Colore grigio per le linee di controllo
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < pointCount; i++) {
        glVertex2f(controlPoints[i].x, controlPoints[i].y);  // Collega i punti di controllo
    }
    glEnd();

    // Disegna la curva Bézier continua
    if (pointCount >= 1) {  // Assicurati che ci siano abbastanza punti
        glColor3f(0, 0, 1);  // Colore blu per la curva
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j <= STEP; j++) {
            float t = (float)j / STEP;  // Calcola il parametro t
            Point p = deCasteljau(controlPoints, pointCount, t);  // Calcola il punto sulla curva
            glVertex2f(p.x, p.y);  // Disegna il punto sulla curva
        }
        glEnd();
    }

    glFlush();  // Forza il rendering
}

// Funzione di callback per la gestione del mouse
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && pointCount < MAX_POINTS) {
        // Registra un nuovo punto di controllo quando si fa clic con il tasto sinistro del mouse
        controlPoints[pointCount].x = x;
        controlPoints[pointCount].y = 500 - y;  // Adatta la coordinata y per l'origine in basso a sinistra
        pointCount++;  // Incrementa il numero di punti di controllo
        glutPostRedisplay();  // Rende necessario il ridisegno della scena
    }
}

// Funzione di inizializzazione di OpenGL
void init(void) {
    glClearColor(1, 1, 1, 1);  // Imposta lo sfondo bianco
    gluOrtho2D(0, 500, 0, 500);  // Definisce un sistema di coordinate ortogonale 2D
}

// Funzione principale
int main(int argc, char** argv) {
    glutInit(&argc, argv);  // Inizializza GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  // Imposta il modello di display
    glutInitWindowSize(500, 500);  // Imposta la dimensione della finestra
    glutCreateWindow("Curve di Bézier Continua - Input da Mouse");  // Crea la finestra con un titolo
    
    init();  // Inizializza le impostazioni OpenGL
    glutDisplayFunc(display);  // Imposta la funzione di callback per la visualizzazione
    glutMouseFunc(mouse);  // Imposta la funzione di callback per il mouse
    glutMainLoop();  // Avvia il ciclo principale di GLUT
    return 0;
}
