#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>

// Funzione per controllare eventuali errori OpenGL e stamparli
void checkError(const char *label)
{
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        printf("%s: OpenGL Error %d\n", label, error);
    }
}

// Funzione per disegnare un cerchio con bordo blu e centro nero
void drawCircleWithBlueBorderAndBlackCenter(float centerX, float centerY, float outerRadius, float innerRadius, int segments)
{
    // Abilita il blending per la trasparenza
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Disegna il cerchio esterno (bordo blu)
    glBegin(GL_POLYGON);
    glColor4f(0.0, 0.0, 1.0, 1.0); // Colore blu (opaco)

    for (int i = 0; i < segments; i++)
    {
        float angle = 2.0f * M_PI * i / segments;
        float x = outerRadius * cos(angle);
        float y = outerRadius * sin(angle);
        glVertex2f(centerX + x, centerY + y);
    }

    glEnd();

    int segment = 360;  // Divide il cerchio interno in 360 segmenti per un aspetto più liscio

    // Disegna il cerchio interno (centro nero)
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(0.0, 0.0, 0.0, 1.0); // Colore nero opaco
    
    glVertex2f(centerX, centerY); // Punto centrale del cerchio interno

    for (int i = 0; i <= segment; i++) // Usa <= per chiudere il cerchio
    {
        float angle = 2.0f * M_PI * i / segment;
        float x = innerRadius * cos(angle);
        float y = innerRadius * sin(angle);
        glVertex2f(centerX + x, centerY + y);
    }

    glEnd();

    // Controllo errori OpenGL dopo il rendering
    checkError("drawCircleWithBlueBorderAndBlackCenter");

    // Disabilita il blending per il resto
    glDisable(GL_BLEND);
}

int main(void)
{
    // Inizializza GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Errore: Impossibile inizializzare GLFW\n");
        return -1;
    }

    // Crea una finestra OpenGL
    GLFWwindow *window = glfwCreateWindow(800, 800, "Cerchio blu con centro nero", NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Errore: Impossibile creare la finestra GLFW\n");
        glfwTerminate();
        return -1;
    }

    // Imposta il contesto OpenGL per la finestra
    glfwMakeContextCurrent(window);
    checkError("glfwMakeContextCurrent");

    // Imposta una proiezione ortografica per il rendering 2D
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glClearColor(0.0, 0.0, 0.0, 1.0); // Imposta il colore di sfondo (nero)

    // Loop principale per la gestione della finestra
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT); // Pulisce il buffer del colore
        checkError("glClear");

        // Disegna un cerchio con bordo blu e centro nero
        drawCircleWithBlueBorderAndBlackCenter(0.0, 0.0, 0.5, 0.3, 20); // Segmenti ridotti per prestazioni migliori

        glfwSwapBuffers(window); // Scambia i buffer per mostrare il disegno
        glfwPollEvents(); // Gestisce gli eventi della finestra
    }

    // Pulizia delle risorse prima di chiudere
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
