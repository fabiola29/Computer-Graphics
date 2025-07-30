#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>

#define WIDTH 800  // Larghezza della finestra (800 pixel)
#define HEIGHT 800 // Altezza della finestra (800 pixel)

#define PI 3.14159265359  // Costante per il valore di pi greco (approssimato a 15 decimali)


// Variabili per gli angoli delle lancette
float secondAngle = 0.0f;
float minuteAngle = 0.0f; // Lancetta dei minuti
float hourAngle = 0.0f;   // Lancetta delle ore
float previousMinuteAngle = 0.0f; // Posizione iniziale della lancetta dei minuti
int isDragging = 0; // Flag per il trascinamento del mouse
int isRunning = 0;
// Funzione per controllare eventuali errori di OpenGL
void checkError(const char *label)
{
    GLenum error;
    // Ciclo attraverso gli errori di OpenGL
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        // Stampa l'errore con il suo label associato
        printf("%s: OpenGL Error %d\n", label, error);
    }
}

// Funzione per disegnare una linea (per le lancette)
void drawLine(float x1, float y1, float x2, float y2) {
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
    checkError("drawLine");
}

// Funzione per disegnare l'orologio e le lancette
void drawClock(float secondAngle, float minuteAngle, float hourAngle) {
    // Disegnare il quadrante dell'orologio
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++) {
        float angle = i * PI / 180.0f;
        glVertex2f(0.5f * cos(angle), 0.5f * sin(angle));
    }
    glEnd();
    checkError("drawClock");

    // Disegnare la lancetta dei minuti (in blu)
    glColor3f(0.0f, 0.0f, 1.0f);
    drawLine(0.0f, 0.0f, 0.35f * cos(minuteAngle), 0.35f * sin(minuteAngle));

    // Disegnare la lancetta delle ore (in verde)
    glColor3f(0.0f, 1.0f, 0.0f);
    drawLine(0.0f, 0.0f, 0.25f * cos(hourAngle), 0.25f * sin(hourAngle));
}

// Funzione per aggiornare l'orologio (DA RIVEDERE)
void updateClock(void) {
    // Aggiorna l'angolo dei secondi
    secondAngle += PI / 30.0f;
    if (secondAngle >= 2 * PI) {
        secondAngle -= 2 * PI;
        
        // Aggiorna l'angolo dei minuti
        minuteAngle += PI / 30.0f;
        if (minuteAngle >= 2 * PI) {
            minuteAngle -= 2 * PI;
            
            // Ogni giro completo dei minuti, aggiorna l'ora
            hourAngle += PI / 6.0f;
            if (hourAngle >= 2 * PI) {
                hourAngle -= 2 * PI;
            }
        }
    }
    
    // Controllo per evitare valori negativi
    if (hourAngle < 0) hourAngle += 2 * PI;
    
    printf("Minuti: %.3f rad, Ore: %.3f rad\n", minuteAngle, hourAngle);
}

// Callback per il tasto del mouse
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            isDragging = 1;  // Il mouse è premuto
        } else if (action == GLFW_RELEASE) {
            isDragging = 0;  // Il mouse è rilasciato
        }
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (isDragging) {
        // Calcola la posizione normalizzata del cursore nel range [-1,1]
        float mx = (xpos / WIDTH) * 2.0f - 1.0f;
        float my = -((ypos / HEIGHT) * 2.0f - 1.0f);

        // Calcola il nuovo angolo della lancetta dei minuti
        float newMinuteAngle = atan2(my, mx);
        if (newMinuteAngle < 0) newMinuteAngle += 2 * PI;

        // Calcola la differenza tra il nuovo angolo e l'angolo precedente
        float angleDifference = newMinuteAngle - previousMinuteAngle;

        // Se la differenza è maggiore di π, significa che abbiamo attraversato la posizione delle 12 in senso antiorario
        if (angleDifference > PI) {
            angleDifference -= 2 * PI;
        }
        // Se la differenza è minore di -π, significa che abbiamo attraversato la posizione delle 12 in senso orario
        else if (angleDifference < -PI) {
            angleDifference += 2 * PI;
        }

        // Aggiorna la lancetta delle ore solo se la lancetta dei minuti ha completato un giro completo
        if (angleDifference > PI / 30.0f) { // Soglia per evitare piccoli movimenti accidentali
            hourAngle += (PI / 6.0f) * (angleDifference / (2 * PI));
            if (hourAngle >= 2 * PI) hourAngle -= 2 * PI;
            if (hourAngle < 0) hourAngle += 2 * PI;
        }

        // Aggiorna l'angolo dei minuti
        minuteAngle = newMinuteAngle;
        previousMinuteAngle = newMinuteAngle;
    }
}



int main(void) {
    // Inizializza GLFW
    if (!glfwInit()) {
        printf("Impossibile inizializzare GLFW\n");
        return -1;
    }

    // Crea la finestra GLFW
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Orologio", NULL, NULL);
    if (!window) {
        glfwTerminate();
        printf("Impossibile creare la finestra\n");
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Imposta la proiezione ortogonale
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    // Imposta i callback
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);  // Pulisce lo schermo
        updateClock();  // Aggiorna la posizione delle lancette
        drawClock(secondAngle, minuteAngle, hourAngle);  // Disegna l'orologio
        glfwSwapBuffers(window);  // Scambia il buffer
        glfwPollEvents();  // Gestisce gli eventi
    }

    glfwTerminate();  // Termina GLFW
    return 0;
} 
