#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

#define NUM_POINTS 5 // Numero di punte della stella
#define PI 3.14159265359 // Definizione del valore di π

// Variabili per la trasformazione dell'oggetto
float angle = 0.0f;       // Angolo di rotazione
float scale = 1.0f;       // Fattore di scala
float x_translate = 0.0f; // Traslazione lungo l'asse X
float y_translate = 0.0f; // Traslazione lungo l'asse Y

// Variabili per il controllo del mouse
bool leftMousePressed = false;  // Stato del tasto sinistro del mouse
bool rightMousePressed = false; // Stato del tasto destro del mouse
double lastX = 0.0, lastY = 0.0; // Posizione precedente del mouse

// Funzione per controllare eventuali errori OpenGL
void checkError(const char *label) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        printf("%s: OpenGL Error %d\n", label, error);
    }
}

// Funzione per disegnare una stella a cinque punte
void draw_star(float radius_outer, float radius_inner) {
    radius_inner *= 0.5f; // Riduzione della dimensione del raggio interno

    glBegin(GL_TRIANGLE_FAN); // Inizio della modalità di disegno "triangolo a ventaglio"
    glVertex2f(0.0f, 0.0f); // Centro della stella

    for (int i = 0; i < NUM_POINTS * 2; i++) {
        float angle = i * 2.0f * PI / (NUM_POINTS * 2);
        float radius = (i % 2 == 0) ? radius_outer : radius_inner; // Alternanza tra raggio esterno ed interno
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glVertex2f(x, y);
    }

    glVertex2f(radius_outer, 0.0f); // Chiude la figura
    glEnd();
    
    checkError("draw_star");
}


// Funzione per la callback della tastiera
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
            case GLFW_KEY_LEFT:
                angle -= 5.0f; // Ruota a sinistra
                break;
            case GLFW_KEY_RIGHT:
                angle += 5.0f; // Ruota a destra
                break;
            case GLFW_KEY_UP:
                scale += 0.1f; // Aumenta la scala
                break;
            case GLFW_KEY_DOWN:
                scale -= 0.1f; // Diminuisce la scala
                break;
            case GLFW_KEY_W:
                y_translate += 0.1f; // Muove la stella in alto
                break;
            case GLFW_KEY_S:
                y_translate -= 0.1f; // Muove la stella in basso
                break;
            case GLFW_KEY_A:
                x_translate -= 0.1f; // Muove la stella a sinistra
                break;
            case GLFW_KEY_D:
                x_translate += 0.1f; // Muove la stella a destra
                break;
        }
    }
}

//FA TRANSAZIONE, ROTAZIONE E ZOOM CONTEMPORANEAMENTE
/*void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (isMousePressed) {
        float deltaX = (float)(xpos - lastX); // Calcola la differenza di posizione orizzontale
        float deltaY = (float)(ypos - lastY); // Calcola la differenza di posizione verticale

        // Traslazione della stella
        x_translate += deltaX * 0.01f;
        y_translate -= deltaY * 0.01f;

        // Limita i valori di traslazione per evitare che la stella esca dallo schermo
        if (x_translate > 1.0f) x_translate = 1.0f;
        if (x_translate < -1.0f) x_translate = -1.0f;
        if (y_translate > 1.0f) y_translate = 1.0f;
        if (y_translate < -1.0f) y_translate = -1.0f;

        // Scala la stella in base al movimento verticale del mouse
        scale += deltaY * 0.01f;

        // Limita la scala per evitare che diventi troppo piccola o troppo grande
        if (scale < 0.1f) scale = 0.1f;  // Scala minima
        if (scale > 3.0f) scale = 3.0f;  // Scala massima

        // Ruota la stella in base al movimento orizzontale del mouse
        angle += deltaX * 0.5f;

        // Aggiorna la posizione del mouse
        lastX = xpos;
        lastY = ypos;
    }
}*/


// Callback per il movimento del mouse: gestisce traslazione e scalatura
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    float deltaX = (float)(xpos - lastX); // Differenza di posizione X
    float deltaY = (float)(ypos - lastY); // Differenza di posizione Y

    if (leftMousePressed) { // Se il tasto sinistro è premuto, trasla la stella
        x_translate += deltaX * 0.01f;
        y_translate -= deltaY * 0.01f;
    }

    if (rightMousePressed) { // Se il tasto destro è premuto, scala la stella
        scale += deltaY * 0.01f;
        if (scale < 0.1f) scale = 0.1f; // Limite inferiore della scala
        if (scale > 3.0f) scale = 3.0f; // Limite superiore della scala
    }

    lastX = xpos; // Aggiorna la posizione del mouse
    lastY = ypos;
}

// Callback per la gestione della pressione dei pulsanti del mouse
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) { // Se un tasto del mouse viene premuto
        glfwGetCursorPos(window, &lastX, &lastY); // Registra la posizione corrente del mouse

        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            leftMousePressed = true; // Attiva la traslazione
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            rightMousePressed = true; // Attiva la scalatura
        }
    } else if (action == GLFW_RELEASE) { // Se un tasto del mouse viene rilasciato
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            leftMousePressed = false;
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            rightMousePressed = false;
        }
    }
}

// Callback della rotella del mouse per la rotazione della stella
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    angle += yoffset * 5.0f; // Ruota in base allo scroll
    if (angle >= 360.0f) angle -= 360.0f; // Mantiene l'angolo tra 0 e 360 gradi
    if (angle < 0.0f) angle += 360.0f;
}

// Funzione principale: inizializza GLFW, crea la finestra e avvia il loop di rendering
int main(void) {
    if (!glfwInit()) { // Inizializza GLFW
        printf("Errore nell'inizializzazione di GLFW\n");
        return -1;
    }

    // Crea una finestra di 800x800 pixel
    GLFWwindow* window = glfwCreateWindow(800, 800, "Stella a Cinque Punte", NULL, NULL);
    if (!window) {
        printf("Errore nella creazione della finestra\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); // Imposta la finestra come contesto OpenGL attivo

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) { // Inizializza GLEW
        printf("Errore nell'inizializzazione di GLEW\n");
        return -1;
    }

    glEnable(GL_DEPTH_TEST); // Abilita il test di profondità per un rendering corretto

    // Imposta i callback per tastiera, mouse e scroll
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    // Loop principale di rendering
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Pulisce lo schermo
        glLoadIdentity(); // Resetta la matrice di trasformazione

        // Applica le trasformazioni (traslazione, scala, rotazione)
        glTranslatef(x_translate, y_translate, 0.0f);
        glScalef(scale, scale, 1.0f);
        glRotatef(angle, 0.0f, 0.0f, 1.0f);

        draw_star(0.5f, 0.2f); // Disegna la stella

        glfwSwapBuffers(window); // Scambia i buffer per mostrare il frame
        glfwPollEvents(); // Controlla gli eventi della finestra
    }

    glfwDestroyWindow(window); // Distrugge la finestra
    glfwTerminate(); // Termina GLFW
    return 0;
}


/*Per Mouse : Tasto sinistro premuto + movimento → Traslazione ; Tasto destro premuto + movimento → Scalatura e Rotella del mouse → Rotazione*/
