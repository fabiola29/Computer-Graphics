#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

#define PI 3.14159265359

float left = -1.0f, right = 1.0f, bottom = -1.0f, top = 1.0f, nearVal = 1.0f, farVal = 5.0f;

// Variabili globali per la vista prospettica
float fovY = 45.0f;  // Angolo di campo visivo in Y
float aspect = 1.0f;  // Rapporto di aspetto (larghezza/altezza)
float angleX = 20.0f, angleY = 40.0f;  // Rotazione
float radius = 0.5f;  // Raggio del prisma
float centerX = 0.0f, centerY = 0.0f;  // Posizione centrale
float zNear = -0.5f, zFar = 0.5f;  // Profondità del prisma

// Variabili per la gestione del mouse
double lastX = 0.0, lastY = 0.0;
bool isMousePressed = false;

// Variabile per la profondità del prisma
float prismDepth = -3.0f;  // Profondità iniziale del prisma

// Variabili globali per il mouse
bool isRightMousePressed = false;
bool isLeftMousePressed = false;

// Funzione per controllare errori OpenGL
void checkError(const char *label) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        printf("%s: OpenGL Error %d\n", label, error);
    }
}

// Funzione per modificare la vista con la tastiera
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
            case GLFW_KEY_A:
                // Espande la vista orizzontalmente (aumenta il campo visivo orizzontale)
                aspect -= 0.05f;
                if (aspect < 0.1f) aspect = 0.1f; // Prevent aspect ratio from getting too small
                printf("aspect: %f\n", aspect);
                break;
            case GLFW_KEY_D:
                // Restringe la vista orizzontalmente (diminuisce il campo visivo orizzontale)
                aspect += 0.05f;
                if (aspect > 2.0f) aspect = 2.0f; // Prevent aspect ratio from getting too large
                printf("aspect: %f\n", aspect);
                break;
            case GLFW_KEY_W:
                // Espande la vista verticalmente (aumenta il campo visivo verticale)
                aspect += 0.05f;
                 if (aspect > 2.0f) aspect = 2.0f; // Prevent aspect ratio from getting too large
                 printf("aspect: %f\n", aspect);
                 break;
            case GLFW_KEY_S:
                // Restringe la vista verticalmente (diminuisce il campo visivo verticale)
                aspect -= 0.05f;
                if (aspect < 0.1f) aspect = 0.1f; // Prevent aspect ratio from getting too small
                printf("aspect: %f\n", aspect);
                break;
            case GLFW_KEY_UP:
                // Aumenta la profondità del prisma (avvicina il prisma lungo l'asse Z)
                prismDepth += 0.1f;
                printf("prismDepthUp: %f\n", prismDepth);  // Debug
                glfwPostEmptyEvent();  // Forza aggiornamento
                
                break;
            case GLFW_KEY_DOWN:
                // Diminuisce la profondità del prisma (allontana il prisma lungo l'asse Z)
                prismDepth -= 0.1f;
                printf("prismDepthDown: %f\n", prismDepth);  // Debug
                glfwPostEmptyEvent();  // Forza aggiornamento
                
                break;
        }
    }
}

// Callback per il movimento del mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (isLeftMousePressed) {
        // Movimento sinistro per larghezza (X)
        angleY += (xpos - lastX) * 0.2f;
    }
    if (isRightMousePressed) {
        // Movimento destro per lunghezza (Z)
        prismDepth -= (ypos - lastY) * 0.05f;
        if (prismDepth < -5.0f) prismDepth = -5.0f;  // Limitazione della profondità
    }

    lastX = xpos;
    lastY = ypos;
}

// Callback per il click dei tasti del mouse
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            isLeftMousePressed = true;
        } else if (action == GLFW_RELEASE) {
            isLeftMousePressed = false;
        }
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            isRightMousePressed = true;
        } else if (action == GLFW_RELEASE) {
            isRightMousePressed = false;
        }
    }
}

// Callback per la rotella del mouse (modifica profondità)
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    prismDepth -= yoffset * 0.1f;
    if (prismDepth < -5.0f) prismDepth = -5.0f;
    else if (prismDepth > -1.0f) prismDepth = -1.0f;
}



// Funzione per disegnare il prisma ottagonale
void redraw(void) {
    // Imposta la modalità di proiezione prospettica (usando gluPerspective)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective(fovY, aspect, nearVal, farVal);  // Proiezione prospettica usando gluPerspective

    // Imposta la matrice del modello
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -3.0f); // Sposta il prisma per essere visibile
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);

    // Pulisce il buffer colore e profondità
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Definizione del prisma ottagonale
    float radius = 0.5f;
    float centerX = 0.0f, centerY = 0.0f;
    // Modifica della profondità del prisma
    float depthFactor = 1.0f + (prismDepth + 3.0f) * 0.5f;
    float zNear = -0.5f * depthFactor;
    float zFar = 0.5f * depthFactor;
        float angle_step = 2.0f * PI / 8;

    // Disegno delle facce laterali
    glBegin(GL_QUADS);
    glColor3f(0.8f, 0.0f, 0.8f);
    for (int i = 0; i < 8; i++) {
        float angle1 = i * angle_step;
        float angle2 = (i + 1) * angle_step;

        float x1 = centerX + radius * cos(angle1);
        float y1 = centerY + radius * sin(angle1);
        float x2 = centerX + radius * cos(angle2);
        float y2 = centerY + radius * sin(angle2);

        glVertex3f(x1, y1, zNear);
        glVertex3f(x2, y2, zNear);
        glVertex3f(x2, y2, zFar);
        glVertex3f(x1, y1, zFar);
    }
    glEnd();

    // Disegno della base inferiore (rossa)
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.0f, 0.0f);
    for (int i = 0; i < 8; i++) {
        float angle = i * angle_step;
        glVertex3f(centerX + radius * cos(angle), centerY + radius * sin(angle), zNear);
    }
    glEnd();
    
    // Disegno della base superiore (blu)
    glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.0f, 1.0f);
    for (int i = 0; i < 8; i++) {
        float angle = i * angle_step;
        glVertex3f(centerX + radius * cos(angle), centerY + radius * sin(angle), zFar);
    }
    glEnd();

    checkError("redraw");
    glFlush();
}

// Funzione principale
int main(void) {
    // Inizializzazione di GLFW
    if (!glfwInit()) {
        printf("Errore nell'inizializzazione di GLFW\n");
        return -1;
    }

    // Creazione della finestra
    GLFWwindow* window = glfwCreateWindow(800, 800, "Prisma Ottagonale - Proiezione Prospettica", NULL, NULL);
    if (!window) {
        printf("Errore nella creazione della finestra\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("Errore nell'inizializzazione di GLEW\n");
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // Ciclo principale di rendering
    while (!glfwWindowShouldClose(window)) {
        redraw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
