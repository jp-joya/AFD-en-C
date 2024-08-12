#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALPHABET_SIZE 10  // Tamaño máximo del alfabeto
#define MAX_STATES 10         // Número máximo de estados

// Estructura que define un autómata finito determinista (DFA)
typedef struct
{
    char alphabet[MAX_ALPHABET_SIZE];    // Alfabeto del DFA
    int num_states;                      // Número de estados en el DFA
    int accept_states[MAX_STATES];       // Estados de aceptación
    int transitions[MAX_STATES][MAX_ALPHABET_SIZE]; // Matriz de transiciones
} DFA;

// Declaración de funciones
void init_dfa(DFA *dfa);                // Inicializa el DFA
void read_config(char *filename, DFA *dfa); // Lee la configuración del DFA desde un archivo
int run_dfa(DFA *dfa, char *input);    // Ejecuta el DFA sobre una cadena de entrada

int main(int argc, char *argv[])
{
    // Verifica que el número de argumentos sea correcto
    if (argc != 2)
    {
        printf("Uso: %s <archivo>\n", argv[0]);
        return 1;
    }

    DFA dfa;                           // Declara una instancia del DFA
    init_dfa(&dfa);                    // Inicializa el DFA
    read_config(argv[1], &dfa);        // Lee la configuración del DFA desde el archivo

    char input[100];                   // Buffer para la cadena de entrada
    printf("Introduce la cadena de entrada: ");
    scanf("%s", input);

    // Ejecuta el DFA y muestra si acepta o rechaza la cadena
    if (run_dfa(&dfa, input))
    {
        printf("Aceptado\n");
    }
    else
    {
        printf("Rechazado\n");
    }

    return 0;
}

// Inicializa el DFA
void init_dfa(DFA *dfa)
{
    memset(dfa->alphabet, 0, sizeof(dfa->alphabet)); // Inicializa el alfabeto
    dfa->num_states = 0;                             // Inicializa el número de estados
    memset(dfa->accept_states, -1, sizeof(dfa->accept_states)); // Inicializa los estados de aceptación con -1 (estado inválido)
    memset(dfa->transitions, -1, sizeof(dfa->transitions)); // Inicializa las transiciones con -1 (transición inválida)
}

// Lee la configuración del DFA desde un archivo
void read_config(char *filename, DFA *dfa)
{
    FILE *fp = fopen(filename, "r"); // Abre el archivo en modo lectura
    if (fp == NULL)
    {
        printf("Error al abrir el archivo\n");
        exit(1);
    }

    // Lee el alfabeto
    if (fgets(dfa->alphabet, sizeof(dfa->alphabet), fp) == NULL) {
        printf("Error al leer el alfabeto\n");
        fclose(fp);
        exit(1);
    }
    dfa->alphabet[strcspn(dfa->alphabet, "\n")] = '\0'; // Elimina el carácter de nueva línea
    
    // Lee el número de estados
    if (fscanf(fp, "%d", &dfa->num_states) != 1 || dfa->num_states > MAX_STATES) {
        printf("Error al leer el número de estados\n");
        fclose(fp);
        exit(1);
    }
    
    // Lee el número de estados de aceptación
    int num_accept_states;
    if (fscanf(fp, "%d", &num_accept_states) != 1 || num_accept_states > MAX_STATES) {
        printf("Error al leer el número de estados de aceptación\n");
        fclose(fp);
        exit(1);
    }
    for (int i = 0; i < num_accept_states; i++)
    {
        if (fscanf(fp, "%d", &dfa->accept_states[i]) != 1 || dfa->accept_states[i] >= dfa->num_states) {
            printf("Error al leer el estado de aceptación\n");
            fclose(fp);
            exit(1);
        }
    }

    // Lee las transiciones
    for (int i = 0; i < dfa->num_states; i++)
    {
        for (int j = 0; j < strlen(dfa->alphabet); j++)
        {
            int next_state;
            if (fscanf(fp, "%d", &next_state) != 1 || next_state >= dfa->num_states) {
                printf("Error al leer la transición\n");
                fclose(fp);
                exit(1);
            }
            dfa->transitions[i][j] = next_state;
        }
    }

    fclose(fp); // Cierra el archivo
}

// Ejecuta el DFA sobre una cadena de entrada
int run_dfa(DFA *dfa, char *input)
{
    int state = 0; // Estado inicial
    for (int i = 0; i < strlen(input); i++)
    {
        int symbol_index = strchr(dfa->alphabet, input[i]) - dfa->alphabet;
        if (symbol_index < 0 || symbol_index >= strlen(dfa->alphabet))
        {
            return 0;  // Símbolo no válido
        }
        int next_state = dfa->transitions[state][symbol_index];
        if (next_state < 0 || next_state >= dfa->num_states)
        {
            return 0;  // Transición no válida
        }
        state = next_state;
    }
    // Verifica si el estado final es un estado de aceptación
    for (int i = 0; i < MAX_STATES; i++)
    {
        if (dfa->accept_states[i] == state)
        {
            return 1;  // Aceptado
        }
    }
    return 0;  // Rechazado
}

