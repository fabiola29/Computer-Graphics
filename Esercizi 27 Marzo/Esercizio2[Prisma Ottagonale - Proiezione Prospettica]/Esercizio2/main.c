#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

#define PI 3.14159265359

// Variabili globali per la vista prospettica
float left = -1.0f, right = 1.0f, bottom = -1.0f, top = 1.0f, nearVal = 1.0f, farVal = 5.0f;
float angleX = 20.0f, angleY = 40.0f;  // Angoli di rotazione per il prisma

// Variabili per la gestione del mouse
double lastX = 0.0, lastY = 0.0;  // Posizione precedente del mouse
bool isMousePressed = false;  // Stato del tasto sinistro del mouse

// Variabile per la profondità del prisma (posizione lungo l'asse Z)
float prismDepth = -3.0f;  // Profondità iniziale del prisma

// Variabili globali per il mouse (gestione tasto destro)
bool isRightMousePressed = false;

// Funzione per controllare errori OpenGL
void checkError(const char *label) {
    GLenum error;
    // Ciclo per controllare gli errori OpenGL
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

// Funzione per gestire l'input del mouse (movimento del mouse)
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (isMousePressed) {
        // Trascinamento con il tasto sinistro → modifica del campo visivo (pan)
        float deltaX = (float)(xpos - lastX);
        float deltaY = (float)(ypos - lastY);
        
        // Modifica dei limiti della vista in base al movimento del mouse
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

// Funzione per gestire la pressione del mouse (quando il mouse viene premuto o rilasciato)
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        // Gestisce la pressione e il rilascio del tasto sinistro
        if (action == GLFW_PRESS) {
            isMousePressed = true;  // Segna che il mouse è premuto
        } else if (action == GLFW_RELEASE) {
            isMousePressed = false;  // Segna che il mouse non è più premuto
        }
    }
}

// Funzione per gestire lo scroll del mouse (modifica la profondità)
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    // yoffset > 0 → scorrimento verso l'alto (aumenta la profondità)
    // yoffset < 0 → scorrimento verso il basso (diminuisce la profondità)
    prismDepth += yoffset * 0.1f;

    // Limiti per evitare spessori assurdi
    if (prismDepth < 0.1f) prismDepth = 0.1f;
    if (prismDepth > 5.0f) prismDepth = 5.0f;

    printf("Profondità prisma: %f\n", prismDepth);
}

// Funzione per disegnare il prisma ottagonale
void redraw(void) {
    // Pulisce il buffer colore e profondità per il nuovo frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Imposta la modalità di proiezione prospettica
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Imposta la proiezione prospettica usando glFrustum
    glFrustum(left, right, bottom, top, nearVal, farVal);

    // Imposta la matrice del modello (modelview)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Traslazione del prisma lungo l'asse Z (gestita dalla variabile prismDepth)
    glTranslatef(0.0f, 0.0f, prismDepth);

    // Fattore di scala per dimensionare il prisma in base alla sua profondità
    float scaleFactor = 1.0f + (prismDepth + 4.0f) * 0.3f;
    glScalef(scaleFactor, scaleFactor, scaleFactor);
    
    // Rotazione del prisma attorno agli assi X e Y
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);
        
    // Parametri del prisma ottagonale
    float radius = 0.5f;
    float centerX = 0.0f, centerY = 0.0f;
    // Calcolo della profondità del prisma
    float depthFactor = 1.5f + (prismDepth + 3.0f) * 0.3f;
    float zNear = -0.5f * depthFactor;
    float zFar = 0.5f * depthFactor;
    float angle_step = 2.0f * PI / 8;  // 8 lati dell'ottagono

    // Disegno delle facce laterali del prisma (quadrati)
    glBegin(GL_QUADS);
    glColor3f(0.8f, 0.0f, 0.8f);  // Colore magenta
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
    glColor3f(1.0f, 0.0f, 0.0f);  // Colore rosso
    for (int i = 0; i < 8; i++) {
        float angle = i * angle_step;
        glVertex3f(centerX + radius * cos(angle), centerY + radius * sin(angle), zNear);
    }
    glEnd();
    
    // Disegno della base superiore (blu)
    glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.0f, 1.0f);  // Colore blu
    for (int i = 0; i < 8; i++) {
        float angle = i * angle_step;
        glVertex3f(centerX + radius * cos(angle), centerY + radius * sin(angle), zFar);
    }
    glEnd();

    checkError("redraw");  // Verifica gli errori OpenGL
    glFlush();  // Assicura che il rendering sia completato
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

    glfwMakeContextCurrent(window);  // Imposta il contesto corrente
    glfwSetKeyCallback(window, key_callback);  // Callback per la tastiera
    glfwSetCursorPosCallback(window, mouse_callback);  // Callback per il movimento del mouse
    glfwSetMouseButtonCallback(window, mouse_button_callback);  // Callback per i tasti del mouse
    glfwSetScrollCallback(window, scroll_callback);  // Callback per lo scroll del mouse

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("Errore nell'inizializzazione di GLEW\n");
        return -1;
    }

    glEnable(GL_DEPTH_TEST);  // Abilita il test di profondità per la corretta gestione delle sovrapposizioni

    // Ciclo principale di rendering
    while (!glfwWindowShouldClose(window)) {
        redraw();  // Rendi la scena
        glfwSwapBuffers(window);  // Scambia i buffer
        glfwPollEvents();  // Gestisci gli eventi
    }

    glfwDestroyWindow(window);  // Distruggi la finestra
    glfwTerminate();  // Termina GLFW
    return 0;
}
