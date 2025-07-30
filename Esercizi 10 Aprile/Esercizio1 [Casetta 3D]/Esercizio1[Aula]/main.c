#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
#include <math.h>
#include <stdbool.h>

// Variabili per il controllo della camera orbitale
float angleX = 0.0f;
float angleY = 0.0f;
float distance = 10.0f;
double lastX = 400, lastY = 400;
bool firstMouse = true;

void drawNormal(float x, float y, float z, float nx, float ny, float nz) {
    glDisable(GL_LIGHTING); // Disattiva illuminazione per vedere il colore della linea
    glDisable(GL_DEPTH_TEST); // Disattiva il depth test per disegnare sopra gli oggetti

    glPushMatrix(); // Salva lo stato corrente della matrice

    glColor3f(1.0f, 1.0f, 1.0f); // Colore bianco per la normale

    // Disegna solo una linea semplice
    glBegin(GL_LINES);
        glVertex3f(x, y, z); // Punto iniziale
        glVertex3f(x + nx * 1.0f, y + ny * 1.0f, z + nz * 1.0f); // Punto finale
    glEnd();

    glPopMatrix(); // Ripristina stato matrice

    glEnable(GL_DEPTH_TEST); // Riattiva il depth test
    glEnable(GL_LIGHTING); // Riattiva illuminazione
}



// Disegna il corpo della casa
void drawHouseBody(void) {
    // Base
    glBegin(GL_POLYGON);
        glNormal3f(0, -1, 0); // Normale verso il basso
        glVertex3f(-2, -1, -1);
        glVertex3f( 2, -1, -1);
        glVertex3f( 2, -1,  1);
        glVertex3f(-2, -1,  1);
    glEnd();
    // Disegna la normale per la base
    drawNormal(0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f);

    // Facciata frontale
    glBegin(GL_POLYGON);
        glNormal3f(0, 0, -1); // Normale frontale
        glVertex3f(-2, -1, -1);
        glVertex3f( 2, -1, -1);
        glVertex3f( 2,  1, -1);
        glVertex3f(-2,  1, -1);
    glEnd();
    // Disegna la normale per la facciata frontale
    drawNormal(0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f);

    // Lato sinistro
    glBegin(GL_POLYGON);
        glNormal3f(-1, 0, 0); // Normale a sinistra
        glVertex3f(-2, -1,  1);
        glVertex3f(-2, -1, -1);
        glVertex3f(-2,  1, -1);
        glVertex3f(-2,  1,  1);
    glEnd();
    // Disegna la normale per il lato sinistro
    drawNormal(-2.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);

    // Lato destro
    glBegin(GL_POLYGON);
        glNormal3f(1, 0, 0); // Normale a destra
        glVertex3f( 2, -1, -1);
        glVertex3f( 2, -1,  1);
        glVertex3f( 2,  1,  1);
        glVertex3f( 2,  1, -1);
    glEnd();
    // Disegna la normale per il lato destro
    drawNormal(2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);

    // Retro
    glBegin(GL_POLYGON);
        glNormal3f(0, 0, 1); // Normale posteriore
        glVertex3f(-2, -1,  1);
        glVertex3f( 2, -1,  1);
        glVertex3f( 2,  1,  1);
        glVertex3f(-2,  1,  1);
    glEnd();
    // Disegna la normale per il retro
    drawNormal(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
}


// Disegna il tetto della casa
void drawRoof(void) {
    // Falda anteriore
    glBegin(GL_POLYGON);
        glNormal3f(0, 0, -1); // Normale anteriore
        glVertex3f(-2, 1, -1);
        glVertex3f( 2, 1, -1);
        glVertex3f( 0, 2.5, -1);
    glEnd();
    // Disegna la normale per la falda anteriore
    drawNormal(0.0f, 1.75f, -1.0f, 0.0f, 0.0f, -1.0f);

    // Falda posteriore
    glBegin(GL_POLYGON);
        glNormal3f(0, 0, 1); // Normale posteriore
        glVertex3f( 0, 2.5, 1);
        glVertex3f( 2, 1, 1);
        glVertex3f(-2, 1, 1);
    glEnd();
    // Disegna la normale per la falda posteriore
    drawNormal(0.0f, 2.5f, 1.0f, 0.0f, 0.0f, 1.0f);

    // Falda sinistra
    glBegin(GL_POLYGON);
        glNormal3f(-1, 0, 0); // Normale a sinistra
        glVertex3f(-2, 1, -1);
        glVertex3f(-2, 1,  1);
        glVertex3f( 0, 2.5,  1);
        glVertex3f( 0, 2.5, -1);
    glEnd();
    // Disegna la normale per la falda sinistra
    drawNormal(-2.0f, 1.75f, 0.0f, -1.0f, 0.0f, 0.0f);

    // Falda destra
    glBegin(GL_POLYGON);
        glNormal3f(1, 0, 0); // Normale a destra
        glVertex3f( 2, 1, -1);
        glVertex3f( 2, 1,  1);
        glVertex3f( 0, 2.5,  1);
        glVertex3f( 0, 2.5, -1);
    glEnd();
    // Disegna la normale per la falda destra
    drawNormal(2.0f, 1.75f, 0.0f, 1.0f, 0.0f, 0.0f);
}


// Funzione di rendering
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Calcola posizione camera orbitale
    float camX = distance * cos(angleX * M_PI / 180.0f) * cos(angleY * M_PI / 180.0f);
    float camY = distance * sin(angleX * M_PI / 180.0f);
    float camZ = distance * cos(angleX * M_PI / 180.0f) * sin(angleY * M_PI / 180.0f);

    // Imposta la vista
    gluLookAt(camX, camY, camZ, 0, 0, 0, 0, 1, 0);

    // Abilita illuminazione
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Imposta posizione della luce
    GLfloat light_position[] = {5, 5, 5, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // Disegna i modelli
    drawHouseBody();
    drawRoof();

    // Aggiorna il display
    glFlush();
    glutSwapBuffers();
}

// Gestione dei tasti freccia
void handleMovement(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        angleX += 1.0f;
        if (angleX > 60.0f) angleX = 60.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        angleX -= 1.0f;
        if (angleX < -60.0f) angleX = -60.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) angleY -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) angleY += 1.0f;
}

// Gestione movimento mouse per rotazione orbitale
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float sensitivity = 0.1f;
    float xoffset = (xpos - lastX) * sensitivity;
    float yoffset = (lastY - ypos) * sensitivity;

    lastX = xpos;
    lastY = ypos;

    angleY += xoffset;
    angleX += yoffset;

    // Limita inclinazione verticale
    if (angleX > 60.0f) angleX = 60.0f;
    if (angleX < -60.0f) angleX = -60.0f;
}

// Inizializzazione di OpenGL
void init(void) {
    glEnable(GL_DEPTH_TEST);  // Attiva il depth buffer

    // Abilita illuminazione e sorgente luminosa
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Imposta colore della luce diffusa
    GLfloat light_diffuse[] = {1, 1, 1, 1};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    // Imposta la prospettiva
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0f, 800.0f/600.0f, 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

// Funzione principale
int main(int argc, char** argv) {
    // Inizializza GLFW
    if (!glfwInit()) return -1;

    // Crea finestra GLFW
    GLFWwindow* window = glfwCreateWindow(800, 800, "Casetta 3D", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Imposta il contesto OpenGL su questa finestra
    glfwMakeContextCurrent(window);

    // Collega la funzione di callback per il mouse
    glfwSetCursorPosCallback(window, mouse_callback);

    // Imposta colore di sfondo
    glClearColor(0, 0, 0, 1);

    // Inizializzazione OpenGL
    init();

    // Ciclo principale
    while (!glfwWindowShouldClose(window)) {
        handleMovement(window); // Gestione input tastiera
        display();              // Rendering scena
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Distrugge la finestra e termina GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
