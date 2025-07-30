#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

#define PI 3.14159265359


// Variabili globali per la vista prospettica
float left = -1.0f, right = 1.0f, bottom = -1.0f, top = 1.0f, nearVal = 1.0f, farVal = 5.0f;

float angleX = 20.0f, angleY = 40.0f;  // Angoli di rotazione iniziali

// Variabili per la gestione del mouse
double lastX = 0.0, lastY = 0.0;
bool isMousePressed = false;

// Variabile per la profondità del prisma
float prismDepth = -3.0f;  // Profondità iniziale del prisma (asse Z)

// Variabili globali per il mouse
bool isRightMousePressed = false;

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
            case GLFW_KEY_W:
                // Sposta la vista verso l'alto (aumenta il campo visivo verticale)
                top += 0.1f;
                bottom -= 0.1f;
                break;
            case GLFW_KEY_S:
                // Sposta la vista verso il basso (diminuisce il campo visivo verticale)
                top -= 0.1f;
                bottom += 0.1f;
                break;
            case GLFW_KEY_A:
                // Espande la vista orizzontalmente (aumenta il campo visivo orizzontale)
                left -= 0.1f;
                right += 0.1f;
                break;
            case GLFW_KEY_D:
                // Restringe la vista orizzontalmente (diminuisce il campo visivo orizzontale)
                left += 0.1f;
                right -= 0.1f;
                break;
            case GLFW_KEY_UP:
                // Aumenta la profondità del prisma (avvicina il prisma lungo l'asse Z)
                prismDepth += 0.1f;
                printf("prismDepthUp: %f\n", prismDepth);  // Debug
                glfwPostEmptyEvent();  // Forza aggiornamento della finestra
                break;
            case GLFW_KEY_DOWN:
                // Diminuisce la profondità del prisma (allontana il prisma lungo l'asse Z)
                prismDepth -= 0.1f;
                printf("prismDepthDown: %f\n", prismDepth);  // Debug
                glfwPostEmptyEvent();  // Forza aggiornamento della finestra
                break;
        }
    }
}

// Funzione per gestire il movimento del mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (isMousePressed) {
        // Trascinamento con il tasto sinistro → modifica del campo visivo (pan)
        float deltaX = (float)(xpos - lastX);
        float deltaY = (float)(ypos - lastY);
        
        left -= deltaX * 0.01f;
        right += deltaX * 0.01f;
        top += deltaY * 0.01f;
        bottom -= deltaY * 0.01f;
    }
    if (isRightMousePressed) {
        // Trascinamento con il tasto destro → modifica della profondità (avanti/indietro)
        float deltaY = (float)(ypos - lastY);
        
        prismDepth += deltaY * 0.01f;

        // Limiti per evitare valori estremi
        if (prismDepth < 0.1f) prismDepth = 0.1f;
        if (prismDepth > 5.0f) prismDepth = 5.0f;
    }

    // Aggiorna la posizione precedente del mouse
    lastX = xpos;
    lastY = ypos;
}

// Funzione per gestire la pressione dei pulsanti del mouse
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        isMousePressed = (action == GLFW_PRESS);
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        isRightMousePressed = (action == GLFW_PRESS);
    }
}

// Funzione per gestire lo scroll della rotella del mouse
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    prismDepth -= yoffset * 0.1f;  // Invertito per ottenere l'effetto giusto

    // Limiti per evitare valori estremi
    if (prismDepth < 0.1f) prismDepth = 0.1f;
    if (prismDepth > 5.0f) prismDepth = 5.0f;

    printf("Profondità prisma (rotella): %f\n", prismDepth);
    glfwPostEmptyEvent();
}

// Funzione per disegnare il prisma ottagonale
void redraw(void) {
    // Pulisce il buffer colore e profondità
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Imposta la modalità di proiezione ortogonale (parallela)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(left, right, bottom, top, nearVal, farVal);  // Definisce la proiezione ortogonale

    // Imposta la matrice del modello (modifica la posizione e rotazione degli oggetti)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Traslazione del prisma lungo l'asse Z (gestita dalla variabile prismDepth)
    glTranslatef(0.0f, 0.0f, prismDepth);

    // Calcolo della scala in base alla profondità del prisma
    float scaleFactor = 1.0f + (prismDepth + 2.0f) * 0.3f;
    glScalef(scaleFactor, scaleFactor, scaleFactor);
    
    // Rotazione del prisma lungo gli assi X e Y
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);
        
    // Definizione del prisma ottagonale (un prisma con base ottagonale)
    float radius = 0.5f;
    float centerX = 0.0f, centerY = 0.0f;
    
    // Modifica della profondità del prisma
    float depthFactor = 1.0f + (prismDepth + 3.0f) * 0.5f;
    float zNear = -0.5f * depthFactor;
    float zFar = 0.5f * depthFactor;
    float angle_step = 2.0f * PI / 8;

    // Disegno delle facce laterali del prisma
    glBegin(GL_QUADS);
    glColor3f(0.8f, 0.0f, 0.8f);  // Colore viola per le facce laterali
    for (int i = 0; i < 8; i++) {
        float angle1 = i * angle_step;
        float angle2 = (i + 1) * angle_step;

        float x1 = centerX + radius * cos(angle1);
        float y1 = centerY + radius * sin(angle1);
        float x2 = centerX + radius * cos(angle2);
        float y2 = centerY + radius * sin(angle2);

        // Vertici del quadrato che costituisce la faccia laterale
        glVertex3f(x1, y1, zNear);
        glVertex3f(x2, y2, zNear);
        glVertex3f(x2, y2, zFar);
        glVertex3f(x1, y1, zFar);
    }
    glEnd();

    // Disegno della base inferiore (rossa)
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.0f, 0.0f);  // Colore rosso per la base inferiore
    for (int i = 0; i < 8; i++) {
        float angle = i * angle_step;
        glVertex3f(centerX + radius * cos(angle), centerY + radius * sin(angle), zNear);
    }
    glEnd();
    
    // Disegno della base superiore (blu)
    glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.0f, 1.0f);  // Colore blu per la base superiore
    for (int i = 0; i < 8; i++) {
        float angle = i * angle_step;
        glVertex3f(centerX + radius * cos(angle), centerY + radius * sin(angle), zFar);
    }
    glEnd();

    // Controllo degli errori OpenGL
    checkError("redraw");

    // Assicurati che tutte le operazioni siano state eseguite
    glFlush();
}

// Funzione principale
int main(void) {
    // Inizializzazione di GLFW
    if (!glfwInit()) {
        printf("Errore nell'inizializzazione di GLFW\n");
        return -1;
    }

    // Set GLFW window hints for a 24-bit depth buffer
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    
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

    // Inizializzazione di GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("Errore nell'inizializzazione di GLEW\n");
        return -1;
    }

    // Abilita il test di profondità
    glEnable(GL_DEPTH_TEST);

    // Ciclo principale di rendering
    while (!glfwWindowShouldClose(window)) {
        redraw();  // Ridisegna la scena
        glfwSwapBuffers(window);  // Scambia il buffer
        glfwPollEvents();  // Gestisci gli eventi (tastiera, mouse)
    }

    // Pulizia e chiusura della finestra
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
