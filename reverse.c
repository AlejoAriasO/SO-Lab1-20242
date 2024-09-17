// Alejandro Arias Ortiz
// Sebastian Aristizabal Casta;eda

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> // Para usar la estructura stat

// Nodo para almacenar una línea
typedef struct LineNode {
    char *line;
    struct LineNode *next;
} LineNode;

// Inserta una línea al inicio de la lista
LineNode* prepend_line(LineNode *head, const char *line) {
    LineNode *node = (LineNode *)malloc(sizeof(LineNode));
    if (node == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    node->line = strdup(line);
    if (node->line == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    node->next = head;
    return node;
}

// Libera la memoria de la lista de líneas
void destroy_lines(LineNode *head) {
    while (head != NULL) {
        LineNode *temp = head;
        head = head->next;
        free(temp->line);
        free(temp);
    }
}

// Lee una línea de longitud variable
ssize_t get_line(char **lineptr, size_t *n, FILE *stream) {
    if (*lineptr == NULL || *n == 0) {
        *n = 128; // Tamaño inicial del buffer
        *lineptr = malloc(*n);
        if (*lineptr == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
    }

    ssize_t length = getline(lineptr, n, stream);
    return length;
}

// Verifica si dos archivos son el mismo (hardlinked)
int files_are_identical(const char *file1, const char *file2) {
    struct stat stat1, stat2;

    if (stat(file1, &stat1) != 0) {
        exit(1);
    }

    if (stat(file2, &stat2) != 0) {
        exit(1);
    }

    return (stat1.st_dev == stat2.st_dev && stat1.st_ino == stat2.st_ino);
}

// Función principal
int main(int argc, char *argv[]) {
    FILE *input = stdin;
    FILE *output = stdout;
    LineNode *lines = NULL;
    char *buffer = NULL;
    size_t buffer_size = 0;

    // Manejo de argumentos
    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    if (argc >= 2) {
        input = fopen(argv[1], "r");
        if (input == NULL) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
    }

    if (argc == 3) {
        output = fopen(argv[2], "w");
        if (output == NULL) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
            exit(1);
        }

        if (strcmp(argv[1], argv[2]) == 0 || files_are_identical(argv[1], argv[2])) {
            fprintf(stderr, "reverse: input and output file must differ\n");
            exit(1);
        }
    }

    // Leer el archivo de entrada línea por línea y agregar a la lista
    while (get_line(&buffer, &buffer_size, input) != -1) {
        lines = prepend_line(lines, buffer);
    }

    // Imprimir las líneas en orden inverso
    LineNode *current = lines;
    while (current != NULL) {
        fprintf(output, "%s", current->line);
        current = current->next;
    }

    // Limpiar
    destroy_lines(lines);
    free(buffer);
    if (input != stdin) fclose(input);
    if (output != stdout) fclose(output);

    return 0;
}

