#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX 256
#define LENGTH 5

/* TREE */
typedef struct treenode {
    char data[LENGTH];
    struct treenode *left;
    struct treenode *right;
}treenode;
treenode *createNode(char data[LENGTH]);

/* OPERATOR STACK */
char operatorStack[MAX][LENGTH];
int operatorCount = 0;
void operatorPush(char string[LENGTH]);
char *operatorPop();
bool operatorIsEmpty();
char *operatorTop();

/* OPERAND STACK */
treenode *operandStack[MAX];
int operandCount = 0;
void operandPush(treenode *t);
treenode *operandPop();
bool operandIsEmpty();
treenode *operandTop();
void freeTree(treenode* root);

/* HELPER FUNCTIONS */
int parseInfixToComponents(char componentsArr[MAX][LENGTH], char infixExpression[]);
bool isOperator(char operator);
treenode *parseComponentsToTree(char infixExpression[]);
int priority(char operator[]);
bool isOperand(char operand[]);
void postOrder(treenode *root);
void preOrder(treenode *root);

int main(int argc, char *argv[]) {
    // Declare variables
    char temp[strlen(argv[1])+2];
    char infixExpression[strlen(argv[1])];
    treenode *t;
    int choice;

    // Surround expression with brackets 
    temp[0] = '(';
    strcat(temp, argv[1]);
    temp[strlen(argv[1])+1] = ')';

    strcpy(infixExpression, temp);
    strcpy(infixExpression, argv[1]);
    t = parseComponentsToTree(infixExpression);

    // Menu
    while(true) {
        printf("Please select from the following options:\n");
        printf("1. Preorder\n");
        printf("2. Postorder\n");
        printf("3. Exit\n");
        scanf("%d", &choice);
        printf("\n");
        switch(choice) {
        case 1:
            preOrder(t);
            break;
        case 2:
            postOrder(t);
            break;
        case 3:
            exit(0);
            break;
        default:
            printf("Enter a number between 1-3.");
        }
        printf("\n\n");
    }

    freeTree(t);

    return 0;
}

int parseInfixToComponents(char componentsArr[MAX][LENGTH], char infixExpression[]) {
    int j = 0;

    // Iterate through infix expression, add components to array
    for(int i = 0; i < strlen(infixExpression); i++) {
        if(infixExpression[i] == '(') {
            strcpy(componentsArr[j], "(");
            j++;
        }
        else if(infixExpression[i] == ')') {
            strcpy(componentsArr[j], ")");
            j++;
        }
        else if(isOperator(infixExpression[i])) {
            char operator[2];
            operator[0] = infixExpression[i];
            operator[1] = '\0';
            strcpy(componentsArr[j], operator);
            j++;
        }
        else if(infixExpression[i] == 'x') {
            char variable[3];
            variable[0] = infixExpression[i];
            variable[1] = infixExpression[++i];
            variable[2] = '\0';
            strcpy(componentsArr[j], variable);
            j++;
        }
        else {
            char number[5];
            number[0] = infixExpression[i];
            number[1] = infixExpression[++i];
            number[2] = infixExpression[++i];
            number[3] = infixExpression[++i];
            number[4] = '\0';
            strcpy(componentsArr[j], number);
            j++;
        }
    }

    return j;
}

bool isOperator(char operator) {
    // Returns true if char is an operatore, false otherwise
    if(operator == '+' || operator == '-' || operator == '*' || operator == '/') {
        return true;
    }
    return false;
}

treenode *createNode(char data[LENGTH]) {
    // Allocate and create a default node, return node
    treenode* result = malloc(sizeof(treenode));
    result->left = NULL;
    result->right = NULL;
    strcpy(result->data, data);
    return result;
}

treenode *parseComponentsToTree(char infixExpression[]) {
    char componentsArr[MAX][LENGTH];
    int numComponents;
    treenode *t, *t1, *t2;

    // Create components array
    numComponents = parseInfixToComponents(componentsArr, infixExpression);

    // Iterate through components array
    for(int i = 0; i < numComponents; i++) {
        // If component is open bracket, push operator
        if(strcmp(componentsArr[i], "(") == 0) {
            operatorPush(componentsArr[i]);
        }
        // If component if operand, push operand 
        else if(isOperand(componentsArr[i])) {
            t = createNode(componentsArr[i]);
            operandPush(t);
        }
        // If prority of operator is greater than 0
        else if(priority(componentsArr[i]) > 0) {
            while(!operatorIsEmpty() && 
                  strcmp(operatorTop(), "(") != 0 && 
                  ((strcmp(componentsArr[i], "^") != 0 && priority(operatorTop()) >= priority(componentsArr[i])) ||
                  ((strcmp(componentsArr[i], "^") == 0) && priority(operatorTop()) > priority(componentsArr[i]))))  
            {
                // Create subtree
                t = createNode(operatorTop());
                operatorPop();

                t1 = operandTop();
                operandPop();

                t2 = operandTop();
                operandPop();

                t->left = t2;
                t->right = t1;

                // Push t to stack
                operandPush(t);
            }
            operatorPush(componentsArr[i]);
        }
        // if component is closed bracket
        else if(strcmp(componentsArr[i], ")") == 0) {
            while(!operatorIsEmpty() && strcmp(operatorTop(), "(") != 0) {
                // Create subtree
                t = createNode(operatorTop());
                operatorPop();
                t1 = operandTop();
                operandPop();
                t2 = operandTop();
                operandPop();
                t->left = t2;
                t->right = t1;

                // Push subtree
                operandPush(t);
            }
            operatorPop();
        }
    }
    t = operandTop();

    // Return root of completed tree
    return t;
}

void postOrder(treenode *root) {
    // Post order traversal of tree
    if(root == NULL) {
        return;
    }
    postOrder(root->left);
    postOrder(root->right);
    if(root->data[0] == 'x') {
        printf("%s[0.00]   ", root->data);
    }
    else {
        printf("%s   ", root->data);
    }
}

void preOrder(treenode *root) {
    // Pre order traversal of tree
    if(root == NULL) {
        return;
    }
    if(root->data[0] == 'x') {
        printf("%s[0.00]   ", root->data);
    }
    else {
        printf("%s   ", root->data);
    }
    preOrder(root->left);
    preOrder(root->right);
}

bool isOperand(char operand[]) {
    // returns true if char is operand, false otherwise
    if(isdigit(operand[0])) {
        return true;
    }
    if(operand[0] == 'x') {
        return true;
    }
    return false;
}

int priority(char operator[]) {
    // returns the priority of a specified operator
    if(strcmp(operator,"+")==0 || strcmp(operator,"-")==0) {
        return 1;
    }
    if(strcmp(operator,"*")==0 || strcmp(operator,"/")==0) {
        return 2;
    }
    if(strcmp(operator,"^")==0) {
        return 3;
    }
    // if operator == ')'
    return 0;
}

/* OPERATOR STACK OPERATIONS */
void operatorPush(char string[LENGTH]) {
    if(operatorCount == MAX) {
        printf("Overflow.");
        return;
    }
    strcpy(operatorStack[operatorCount], string);
    operatorCount++;
}

char *operatorPop() {
    if(operatorCount == 0) {
        printf("Underflow.");
        exit(0);
    }
    char* result = operatorStack[operatorCount-1];
    operatorCount--;
    return result;
}

bool operatorIsEmpty() {
    if(operatorCount == 0) {
        return true;
    }
    return false;
}

char *operatorTop() {
    if(operatorCount == 0) {
        printf("Underflow.");
        exit(0);
    }
    char* result = operatorStack[operatorCount-1];
    return result;
}

/* OPERAND STACK OPERATIONS */
void operandPush(treenode *t) {
    if(operandCount == MAX) {
        printf("Overflow.");
        return;
    }
    operandStack[operandCount] = t;
    operandCount++;
}

treenode *operandPop() {
    if(operandCount == 0) {
        printf("Underflow.");
        exit(0);
    }
    treenode *t = operandStack[operandCount-1];
    operandCount--;
    return t;
}

bool operandIsEmpty() {
    if(operandCount == 0) {
        return true;
    }
    return false;
}

treenode *operandTop() {
    if(operandCount == 0) {
        printf("Underflow.");
        exit(0);
    }
    treenode *t = operandStack[operandCount-1];
    return t;
}

void freeTree(treenode* root) {
    if (root != NULL) {
        freeTree(root->right);
        free(root->data);
        freeTree(root->left);
        free(root);
    }
}
