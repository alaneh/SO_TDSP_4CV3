#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_EXPR 100
#define MAX_TOKENS 50

// Tipos de token
typedef enum {
    TOKEN_NUMBER, TOKEN_OPERATOR, TOKEN_FUNCTION, TOKEN_PARENTHESIS, TOKEN_INVALID
} TokenType;

// Estructura de un token
typedef struct {
    TokenType type;
    char value[20];  // El valor del token (ej: "sin", "+", "45")
} Token;

// Nodo del árbol de expresión
typedef struct Node {
    Token token;
    struct Node* left;
    struct Node* right;
} Node;
// Función para verificar si un carácter es un operador
int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

// Función para verificar si una palabra es una función matemática
int is_function(const char* word) {
    const char* functions[] = {"sin", "cos", "tan", "log", "sqrt", "exp", "abs", "floor", "ceil"};
    for (int i = 0; i < 9; ++i) {
        if (strcmp(word, functions[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Función para tokenizar la expresión
int tokenize(const char* expr, Token tokens[]) {
    int pos = 0, token_count = 0;
    while (expr[pos] != '\0') {
        // Ignorar espacios
        if (isspace(expr[pos])) {
            pos++;
            continue;
        }

        // Paréntesis
        if (expr[pos] == '(' || expr[pos] == ')') {
            tokens[token_count].type = TOKEN_PARENTHESIS;
            tokens[token_count].value[0] = expr[pos];
            tokens[token_count].value[1] = '\0';
            token_count++;
            pos++;
            continue;
        }

        // Operadores
        if (is_operator(expr[pos])) {
            tokens[token_count].type = TOKEN_OPERATOR;
            tokens[token_count].value[0] = expr[pos];
            tokens[token_count].value[1] = '\0';
            token_count++;
            pos++;
            continue;
        }

        // Funciones o números
        if (isalpha(expr[pos])) {
            int start = pos;
            while (isalpha(expr[pos])) pos++;
            int length = pos - start;
            strncpy(tokens[token_count].value, expr + start, length);
            tokens[token_count].value[length] = '\0';

            if (is_function(tokens[token_count].value)) {
                tokens[token_count].type = TOKEN_FUNCTION;
            } else {
                tokens[token_count].type = TOKEN_INVALID;  // Función no reconocida
            }
            token_count++;
            continue;
        }

        // Números
        if (isdigit(expr[pos])) {
            int start = pos;
            while (isdigit(expr[pos])) pos++;
            int length = pos - start;
            strncpy(tokens[token_count].value, expr + start, length);
            tokens[token_count].value[length] = '\0';
            tokens[token_count].type = TOKEN_NUMBER;
            token_count++;
            continue;
        }

        // Si no es un token válido
        tokens[token_count].type = TOKEN_INVALID;
        token_count++;
        pos++;
    }

    return token_count;  // Retornar el número de tokens
}
// Función para crear un nuevo nodo
Node* create_node(Token token) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->token = token;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Verificar si el token es un número o una función
int is_operand(Token token) {
    return token.type == TOKEN_NUMBER || token.type == TOKEN_FUNCTION;
}

// Función para validar la expresión y construir el árbol
Node* parse_expression(Token tokens[], int token_count) {
    Node* stack[MAX_TOKENS];
    int stack_top = -1;

    for (int i = 0; i < token_count; i++) {
        Token token = tokens[i];

        // Si es un operando (número o función)
        if (is_operand(token)) {
            Node* node = create_node(token);
            stack[++stack_top] = node;
        }
        // Si es un operador, sacar dos operandos de la pila
        else if (token.type == TOKEN_OPERATOR) {
            if (stack_top < 1) {
                printf("Error: Expresión inválida\n");
                return NULL;
            }
            Node* right = stack[stack_top--];
            Node* left = stack[stack_top--];
            Node* node = create_node(token);
            node->left = left;
            node->right = right;
            stack[++stack_top] = node;
        }
        // Validar paréntesis
        else if (token.type == TOKEN_PARENTHESIS) {
            if (token.value[0] == '(') {
                // Manejar el paréntesis de apertura
            } else if (token.value[0] == ')') {
                // Manejar el paréntesis de cierre
            }
        }
        // Token inválido
        else {
            printf("Error: Token no reconocido '%s'\n", token.value);
            return NULL;
        }
    }

    if (stack_top != 0) {
        printf("Error: Expresión incompleta\n");
        return NULL;
    }

    return stack[0];  // El nodo raíz del árbol de expresión
}
void print_infix(Node* node) {
    if (node == NULL) return;

    if (node->left) print_infix(node->left);
    printf("%s ", node->token.value);
    if (node->right) print_infix(node->right);
}
int main() {
    char expr[MAX_EXPR];
    Token tokens[MAX_TOKENS];

    printf("Ingresa una expresión: ");
    fgets(expr, MAX_EXPR, stdin);

    int token_count = tokenize(expr, tokens);

    // Mostrar los tokens obtenidos
    for (int i = 0; i < token_count; i++) {
        printf("Token: %s (Tipo: %d)\n", tokens[i].value, tokens[i].type);
    }

    Node* root = parse_expression(tokens, token_count);

    if (root) {
        printf("La expresión es válida. Árbol en notación infija:\n");
        print_infix(root);
        printf("\n");
    } else {
        printf("La expresión NO es válida.\n");
    }

    return 0;
}
