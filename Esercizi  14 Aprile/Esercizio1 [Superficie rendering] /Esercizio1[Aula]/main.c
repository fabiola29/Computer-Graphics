#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

#define GRID_SIZE 100  // Numero di suddivisioni della griglia in ogni direzione (risoluzione)
#define RANGE 10.0      // Estensione dell'intervallo in x e z: da -RANGE a +RANGE

float angle = 0.0f;
float angleX = 0.0f;             // Angolo di rotazione sull'asse X
float angleY = 0.0f;             // Angolo di rotazione sull'asse Y
float angleZ = 0.0f;
int lastX, lastY;  // Posizione del mouse nel momento precedente

// Calcola il valore della funzione y = 2*sin(r)/r dove r = sqrt(x^2 + z^2)
float computeY(float x, float z) {
    float r = sqrtf(x * x + z * z);  // Distanza radiale dal centro
    return (r != 0.0f) ? (2.0f * sinf(r)) / r : 2.0f;  // Evita divisione per zero
}

// Calcola la normale di un piano definito da 3 vertici
void computeNormal(float* p1, float* p2, float* p3, float* normal) {
    float u[3], v[3];

    u[0] = p2[0] - p1[0];
    u[1] = p2[1] - p1[1];
    u[2] = p2[2] - p1[2];

    v[0] = p3[0] - p1[0];
    v[1] = p3[1] - p1[1];
    v[2] = p3[2] - p1[2];

    // Prodotto vettoriale u x v
    normal[0] = u[1]*v[2] - u[2]*v[1];
    normal[1] = u[2]*v[0] - u[0]*v[2];
    normal[2] = u[0]*v[1] - u[1]*v[0];

    // Normalizzazione
    float length = sqrtf(normal[0]*normal[0] + normal[1]*normal[1] + normal[2]*normal[2]);
    if (length != 0.0f) {
        normal[0] /= length;
        normal[1] /= length;
        normal[2] /= length;
    }
}

void drawSurface(void) {
    glBegin(GL_QUADS);  // Inizia disegno quadrilateri
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            float x0 = -RANGE + 2.0f * RANGE * i / GRID_SIZE;
            float z0 = -RANGE + 2.0f * RANGE * j / GRID_SIZE;
            float x1 = -RANGE + 2.0f * RANGE * (i + 1) / GRID_SIZE;
            float z1 = -RANGE + 2.0f * RANGE * (j + 1) / GRID_SIZE;

            float y00 = computeY(x0, z0);
            float y01 = computeY(x0, z1);
            float y11 = computeY(x1, z1);
            float y10 = computeY(x1, z0);

            float p1[] = {x0, y00, z0};
            float p2[] = {x0, y01, z1};
            float p3[] = {x1, y11, z1};
            float normal[3];
            computeNormal(p1, p2, p3, normal);

            glNormal3fv(normal); // Imposta la normale per il quadrilatero

            glColor3f(0.4f + y00 * 0.1f, 0.2f + y00 * 0.05f, 0.6f);
            glVertex3fv(p1);

            glColor3f(0.4f + y01 * 0.1f, 0.2f + y01 * 0.05f, 0.6f);
            glVertex3fv(p2);

            glColor3f(0.4f + y11 * 0.1f, 0.2f + y11 * 0.05f, 0.6f);
            glVertex3fv(p3);

            glColor3f(0.4f + y10 * 0.1f, 0.2f + y10 * 0.05f, 0.6f);
            glVertex3f(x1, y10, z0);
        }
    }
    glEnd();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(25.0, 15.0, 25.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);
    glRotatef(angleZ, 0.0f, 0.0f, 1.0f);

    drawSurface();

    glutSwapBuffers();
}

// Gestisce il ridimensionamento della finestra
void reshape(int w, int h) {
    if (h==0) h=1;
    float ratio = 1.0f * w / h;
    
    glMatrixMode(GL_PROJECTION); //Passa alla matrice di proiezione
    glLoadIdentity();
    gluPerspective(45.0, ratio, 1.0, 100.0);
    
    glMatrixMode(GL_MODELVIEW); //Restituisce la matrice di modellazione
    glViewport(0, 0, w, h);
}

void mouseMotion(int x, int y) {
    if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
        // Se il tasto Shift è premuto, muovi la vista su e giù (verticale)
        angleX += (y - lastY) * 0.1f;
    } else {
        // Muovi la vista a sinistra e destra (orizzontale)
        angleY += (x - lastX) * 0.1f;
    }

    lastX = x;
    lastY = y;

    glutPostRedisplay();
}

void mouseButton(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        lastX = x;
        lastY = y;
    }
}

// Inizializza OpenGL
void init(void) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE); // Per normalizzare le normali automaticamente

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

// Funzione principale
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Superficie 3D");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMotionFunc(mouseMotion);  // Aggiungi il movimento del mouse
    glutMouseFunc(mouseButton);   // Aggiungi il click del mouse

    glutMainLoop();
    return 0;
}
