#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Alejandro Arias Ortiz
// Sebastian Aristizabal Castañeda

// Nodo de la lista enlazada para almacenar líneas
typedef struct Node {
    char *content;
    struct Node *next;
} Node;

// Inserta una nueva línea en la lista
Node* insert_line(Node *head, const char *line) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new_node->content = strdup(line);
    if (!new_node->content) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }
    new_node->next = head;
    return new_node;
}

// Libera la memoria de la lista
void destroy_list(Node *head) {
    Node *temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp->content);
        free(temp);
    }
}

// Lee una línea de un archivo con tamaño dinámico
ssize_t get_line(char **lineptr, size_t *size, FILE *file) {
    if (!*lineptr) {
        *size = 128;
        *lineptr = malloc(*size);
        if (!*lineptr) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
    }

    return getline(lineptr, size, file);
}

// Función principal
int main(int argc, char *argv[]) {
    FILE *input = stdin;
    FILE *output = stdout;
    Node *lines = NULL;
    char *line_buffer = NULL;
    size_t buffer_size = 0;

    // Procesar argumentos
    if (argc > 3) {
        fprintf(stderr, "Usage: reverse <input> <output>\n");
        return EXIT_FAILURE;
    }

    if (argc >= 2) {
        input = fopen(argv[1], "r");
        if (!input) {
            perror("fopen");
            return EXIT_FAILURE;
        }
    }

    if (argc == 3) {
        output = fopen(argv[2], "w");
        if (!output) {
            perror("fopen");
            return EXIT_FAILURE;
        }
        if (strcmp(argv[1], argv[2]) == 0) {
            fprintf(stderr, "Input and output files must be different.\n");
            return EXIT_FAILURE;
        }
    }

    // Leer líneas del archivo de entrada y agregar a la lista
    while (get_line(&line_buffer, &buffer_size, input) != -1) {
        lines = insert_line(lines, line_buffer);
    }

    // Imprimir las líneas en orden inverso
    for (Node *current = lines; current; current = current->next) {
        fprintf(output, "%s", current->content);
    }

    // Liberar recursos
    destroy_list(lines);
    free(line_buffer);
    if (input != stdin) fclose(input);
    if (output != stdout) fclose(output);

    return EXIT_SUCCESS;
}
