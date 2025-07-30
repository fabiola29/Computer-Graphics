#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>

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

// Funzione per disegnare un cerchio con gradiente radiale usando vertici lungo la circonferenza
void drawManualGradientCircle(float centerX, float centerY, float radius, int segments)
{
    // Inizio di un "fan" di triangoli (una forma che condivide un vertice centrale)
    glBegin(GL_TRIANGLE_FAN);
    
    // Imposto il colore del centro del cerchio (rosso)
    glColor3f(1.0, 0.0, 0.0); // Colore rosso al centro
    
    // Imposto il vertice centrale del cerchio
    glVertex2f(centerX, centerY); // Centro del cerchio
    
    // Ciclo per creare i vertici lungo la circonferenza del cerchio
    for (int i = 0; i <= segments; i++)
    {
        // Calcolo l'angolo per ogni vertice lungo la circonferenza
        float angle = 2.0f * M_PI * i / segments;
        // Calcolo le coordinate x e y del vertice sulla circonferenza
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        
        // Calcolo la distanza normalizzata dal centro per determinare l'intensità del colore
        float dist = sqrt(x * x + y * y) / radius; // Normalizzo la distanza dal centro
        
        // Applico un gradiente che va dal rosso (centro) al blu (bordo)
        glColor3f(1.0 - dist, 0.0, dist); // Gradiente dal rosso al blu
        // Aggiungo il vertice sulla circonferenza
        glVertex2f(centerX + x, centerY + y);
    }

    // Fine del disegno del cerchio con triangoli
    glEnd();
    
    // Verifica di eventuali errori dopo il disegno del cerchio
    checkError("drawManualGradientCircle");
}

int main(void)
{
    // Inizializzazione di GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Errore: Impossibile inizializzare GLFW\n");
        return -1;
    }

    // Creazione della finestra GLFW
    GLFWwindow *window = glfwCreateWindow(800, 800, "Cerchio sfumato manualmente", NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Errore: Impossibile creare la finestra GLFW\n");
        glfwTerminate();
        return -1;
    }

    // Imposto il contesto OpenGL per la finestra
    glfwMakeContextCurrent(window);
    checkError("glfwMakeContextCurrent");

    // Impostazione della proiezione ortografica per la visualizzazione 2D
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0); // Imposta il sistema di coordinate per visualizzare il cerchio nella finestra

    // Impostazione del colore di sfondo (nero)
    glClearColor(0.0, 0.0, 0.0, 1.0); // Colore di sfondo nero

    // Ciclo di rendering principale
    while (!glfwWindowShouldClose(window))
    {
        // Pulisce il buffer del colore
        glClear(GL_COLOR_BUFFER_BIT);
        checkError("glClear");

        // Disegna un cerchio con 20 segmenti lungo la circonferenza
        drawManualGradientCircle(0.0, 0.0, 0.5, 20); // 20 segmenti per il cerchio

        // Scambia i buffer (mostra il rendering finale)
        glfwSwapBuffers(window);
        
        // Gestisce gli eventi della finestra (ad esempio, la chiusura della finestra)
        glfwPollEvents();
    }

    // Distrugge la finestra e termina GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
