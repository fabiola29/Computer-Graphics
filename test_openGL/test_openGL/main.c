#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <libgen.h>  // Inclusione per basename

void checkError(char *label)
{
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        printf("%s: %s\n", label, gluErrorString(error));
    }
}


// Funzione che disegna il punto
void redraw(void)
{
    // Imposta il colore di sfondo (nero)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      
    // Pulisce il buffer del colore, applicando il colore di sfondo
    glClear(GL_COLOR_BUFFER_BIT);
    
    checkError("Dopo glClear");

    //DISEGNA UN PUNTO
    //glColor3f(1.0f, 0.0f, 1.0f);  // Colore magenta per il punto

    //glPointSize(3.2f);   // Imposta la dimensione del punto

    // glBegin(GL_POINTS); // Inizia a disegnare un punto
    
            // glVertex2f(0.0f, 0.0f); // Colore bianco
    
    // glEnd(); // Termina il disegno
    

    //DISEGNA PIU PUNTI
    //glPointSize(3.0f);  // Il punto sarà disegnato con una dimensione di 3.0 pixels

    // glBegin(GL_POINTS);   // Inizia a disegnare i punti

        // Disegna un punto bianco
        // glColor3f(1.0f, 1.0f, 1.0f);  // Colore bianco per il primo punto
        // glVertex2f(-0.8f, -0.8f);  // Punto alla posizione (-0.8, -0.8)

        // Disegna un punto rosso
        // glColor3f(1.0f, 0.0f, 0.0f);  // Colore rosso per il secondo punto
        // glVertex3f(0.5f, 0.2f, -0.3f);  // Punto alla posizione (0.5, 0.2, -0.3) (in 3D)

        // Disegna un altro punto
        // glVertex4f(-0.5f, 0.4f, 0.7f, 1.0f);  // Punto con coordinate omogenee (x, y, z, w)

        // Cambia colore per il prossimo punto
        // glColor3f(0.0f, 1.0f, 0.0f);  // Colore verde per il quarto punto
        // glVertex4f(-2.0f, 0.4f, 0.7f, 3.0f);  // Punto alla posizione (-2.0, 0.4, 0.7, 3.0)

    // glEnd();  // Termina la sezione di disegno

    //DISEGNA LINEA
    //glBegin(GL_LINE_STRIP);  // Inizia a disegnare una linea continua (linea connessa)
        //glColor3f(0.0f, 1.0f, 0.0f);  // Colore verde per la linea

        // Definisci i vertici della linea
        //glVertex2f(-0.8f, -0.8f);  // Primo punto
        //glVertex2f(-0.4f, 0.4f);   // Secondo punto
        //glVertex2f(0.0f, -0.2f);   // Terzo punto
        //glVertex2f(0.4f, 0.6f);    // Quarto punto
        //glVertex2f(0.8f, -0.4f);   // Quinto punto
    //glEnd();  // Fine del disegno

    //DISEGNA LINEE CONNESSE
    //glBegin(GL_LINE_LOOP);  // Inizia a disegnare una serie di linee connesse, chiuse
        //glColor3f(0.0f, 1.0f, 0.0f);  // Colore verde per la linea

        // Definisci i vertici del poligono (triangolo in questo caso)
        //glVertex2f(-0.8f, -0.8f);  // Primo punto
        //glVertex2f(0.8f, -0.8f);   // Secondo punto
        //glVertex2f(0.0f, 0.8f);    // Terzo punto
    //glEnd();  // Fine del disegno
    
    //DISEGNA POLIGONO
    // glBegin(GL_POLYGON);  // Inizia a disegnare un poligono pieno
    //    glColor3f(1.0f, 0.0f, 0.0f);  // Colore rosso per il poligono
    //   glVertex2f(-0.8f, -0.8f);     // Primo vertice
    //   glVertex2f(0.8f, -0.8f);      // Secondo vertice
    //   glVertex2f(0.0f, 0.8f);       // Terzo vertice
    //glEnd();  // Fine del disegno

    
    //DISEGNA TRIANGOLI
    // glBegin(GL_TRIANGLES);
        // Primo vertice del primo triangolo
        //glColor3f(1.0f, 1.0f, 1.0f);  // Colore bianco
        //glVertex2f(-0.8f, -0.8f);
              
        // Secondo vertice del primo triangolo
        //glColor3f(1.0f, 0.0f, 0.0f);  // Colore rosso
        //glVertex2f(0.5f, 0.2f);
              
        // Terzo vertice del primo triangolo
        //glColor3f(0.0f, 1.0f, 0.0f);  // Colore verde
        //glVertex2f(-0.16f, 0.11f);
    
        //glColor3f(0.0f, 0.0f, 1.0f);  // Colore verde
        //glVertex2f(-0.5f, 0.6f);
        //glVertex2f(-0.2f, 0.3f);
        //glVertex2f(0.6f, 0.8f);
    //glEnd();
    

    // DISEGNO STRISCIA DI TRIANGOLI
    //glBegin(GL_TRIANGLE_FAN);
    
        //glColor3f(1.0f, 1.0f, 1.0f);  // Colore bianco
        //glVertex2f(0.0f, 0.0f);  // Centro del ventaglio
            
        //glColor3f(1.0f, 0.0f, 0.0f);  // Colore rosso
        // glVertex2f(0.8f, 0.2f);  // Primo vertice del triangolo

        //glColor3f(0.0f, 1.0f, 0.0f);  // Colore verde
        //glVertex2f(0.7f, 0.5f);  // Secondo vertice del triangolo

        //glColor3f(0.0f, 0.0f, 1.0f);  // Colore blu
        // glVertex2f(-0.5f, 0.5f);  // Terzo vertice del triangolo

        // glColor3f(1.0f, 1.0f, 0.0f);  // Colore giallo
        //glVertex2f(-0.7f, 0.0f);  // Quarto vertice del triangolo

        // glColor3f(1.0f, 0.0f, 1.0f);  // Colore magenta
        // glVertex2f(-0.5f, -0.6f);  // Ultimo vertice del triangolo

    //glEnd();  // Termina il disegno
    
    // DISEGNO UN QUAD_STRIP
    //glBegin(GL_QUAD_STRIP);

        // Colore bianco e primo vertice
        //glColor3f(1.0f, 1.0f, 1.0f);  // Colore bianco
        //glVertex2f(0.8f, 0.2f);

        // Colore rosso e secondo vertice
        // glColor3f(1.0f, 0.0f, 0.0f);  // Colore rosso
        // glVertex2f(0.7f, -0.5f);

        // Colore verde e terzo vertice
        // glColor3f(0.0f, 1.0f, 0.0f);  // Colore verde
        //glVertex2f(0.5f, 0.2f);

        // Colore giallo e quarto vertice
        // glColor3f(1.0f, 1.0f, 0.0f);  // Colore giallo
        //glVertex2f(-0.1f, -0.5f);

        // Colore blu e quinto vertice
        //glColor3f(0.0f, 0.0f, 1.0f);  // Colore blu
        //glVertex2f(0.6f, 0.8f);

        // Colore cyan e sesto vertice
        //  glColor3f(0.0f, 1.0f, 1.0f);  // Colore cyan
        //glVertex2f(-0.5f, -0.6f);

        // Colore magenta e settimo vertice
        //glColor3f(1.0f, 0.0f, 1.0f);  // Colore magenta
        //glVertex2f(-0.5f, 0.6f);

        // Colore bianco e ottavo vertice
        // glColor3f(1.0f, 1.0f, 1.0f);  // Colore bianco
        //glVertex2f(-0.7f, 0.0f);

    //glEnd();  // Termina il disegno

    // Inizia a disegnare un quadrato
        glBegin(GL_QUADS);  // GL_QUADS indica che stiamo disegnando un quadrato (quattro vertici)

            // Primo vertice: colore bianco
            glColor3f(1.0f, 1.0f, 1.0f);  // Imposta il colore a bianco
            glVertex2f(0.8f, 0.2f);  // Primo vertice del quadrato

            // Secondo vertice: colore rosso
            glColor3f(1.0f, 0.0f, 0.0f);  // Imposta il colore a rosso
            glVertex2f(0.7f, -0.5f);  // Secondo vertice del quadrato

            // Terzo vertice: colore verde
            glColor3f(0.0f, 1.0f, 0.0f);  // Imposta il colore a verde
            glVertex2f(-0.1f, -0.5f);  // Terzo vertice del quadrato

            // Quarto vertice: colore giallo
            glColor3f(1.0f, 1.0f, 0.0f);  // Imposta il colore a giallo
            glVertex2f(0.5f, 0.2f);  // Quarto vertice del quadrato

            // Quinto vertice: colore blu
            glColor3f(0.0f, 0.0f, 1.0f);  // Imposta il colore a blu
            glVertex2f(0.6f, 0.8f);  // Quinto vertice del quadrato

            // Sesto vertice: colore ciano
            glColor3f(0.0f, 1.0f, 1.0f);  // Imposta il colore a ciano
            glVertex2f(-0.5f, -0.6f);  // Sesto vertice del quadrato

            // Settimo vertice: colore magenta
            glColor3f(1.0f, 0.0f, 1.0f);  // Imposta il colore a magenta
            glVertex2f(-0.7f, 0.0f);  // Settimo vertice del quadrato

            // Ottavo vertice: colore bianco
            glColor3f(1.0f, 1.0f, 1.0f);  // Imposta il colore a bianco
            glVertex2f(-0.5f, 0.6f);  // Ottavo vertice del quadrato

        glEnd();  // Termina la sequenza di disegno del quadrato
    
    checkError("Dopo disegno quadrato");


        // Garantisce che tutte le operazioni OpenGL siano completate
        glFlush();
    }


// Funzione principale che esegue il programma
int main(int argc, char** argv)
{
    // Inizializza GLFW, la libreria per la gestione della finestra e degli eventi
    if (!glfwInit())
    {
        printf("Failed to initialize GLFW\n");  // Se l'inizializzazione fallisce, stampa un errore
        return -1;
    }

    // Estrai solo il nome del programma (senza il percorso completo) usando basename
    char* programName = basename(argv[0]);

    // Crea una finestra GLFW con dimensioni 800x600 e il titolo dinamico preso dal nome del programma
    GLFWwindow* window = glfwCreateWindow(800, 600, programName, NULL, NULL);

    // GLFWwindow* window = glfwCreateWindow(800, 600, "Window Title", NULL, NULL); //Il titolo della finestra è "Window Title"
    
    // Verifica se la finestra è stata creata con successo
    if (!window)
    {
        printf("Failed to create GLFW window\n");  // Se la finestra non viene creata, stampa un errore
        glfwTerminate();  // Termina GLFW
        return -1;
    }

    // Imposta il contesto OpenGL per la finestra appena creata
    glfwMakeContextCurrent(window);

    // Imposta la posizione della finestra sulla schermata (300, 100)
    glfwSetWindowPos(window, 300, 100);

    // Imposta una dimensione della finestra di 150x150 pixel
    glfwSetWindowSize(window, 800, 600);

    // Inizializza GLEW, una libreria per la gestione delle estensioni OpenGL
    if (glewInit() != GLEW_OK)
    {
        printf("Failed to initialize GLEW\n");  // Se GLEW non viene inizializzato correttamente, stampa un errore
        return -1;
    }

    // Ciclo principale per il rendering della finestra
    while (!glfwWindowShouldClose(window))  // Continua finché la finestra non viene chiusa
    {
        redraw();  // Chiamata alla funzione per disegnare gli oggetti nella finestra

        // Scambia i buffer, rendendo visibile il disegno appena fatto
        glfwSwapBuffers(window);

        // Gestisce gli eventi della finestra, come input da tastiera o mouse
        glfwPollEvents();
    }

    // Pulizia finale: distrugge la finestra e termina GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;  // Termina il programma
}
