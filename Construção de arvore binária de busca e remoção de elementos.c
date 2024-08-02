#include <stdio.h>
#include <stdlib.h>

// Estrutura para um nó da árvore de busca binária
typedef struct node Node;
struct node {
  int info;
  Node *left;
  Node *right;
};

typedef struct bst BST; // BST: Binary Search Tree
struct bst {
  Node *root;
};

Node *createNode() {
  Node *p;
  p = (Node *)malloc(sizeof(Node));
  p->info = -1;
  p->left = NULL;
  p->right = NULL;
  return p;
}

BST createBST() {
  BST bst = {.root = NULL};

  return bst;
}

void printTree(Node *p, int level) {
  if (p == NULL) {
    for (int i = 0; i < level; i++)
      putchar('\t');
    puts("~");
    return;
  } else {
    printTree(p->right, level + 1);
    for (int i = 0; i < level; i++)
      putchar('\t');
    printf("%d\n", p->info);
    printTree(p->left, level + 1);
  }
}

void insert(BST *bst, int el) {
  Node *new_node = createNode();
  new_node->info = el;

  // Busca pelo local de inserção
  Node *p = bst->root;
  Node *prev = NULL;

  while (p != NULL) {
    prev = p;
    if (el < p->info)
      p = p->left;
    else
      p = p->right;
  }

  // Inserção
  if (bst->root == NULL) //árvore vazia
    bst->root = new_node;
  else if (el < prev->info)
    prev->left = new_node;
  else
    prev->right = new_node;
}

// Função auxiliar para encontrar o nó mínimo em uma árvore
Node *findMin(Node *node) {
  // Node é a raíz da sub-árvore que estamos a fim de conhecer o valor mínimo
  while (node->left != NULL)  // Os menores valores estão à esquerda
    node = node->left;
  return node;                // Retorna o nó de menor valor
}

// Função para remover um nó usando a técnica de mesclagem (merging)
void remove_by_merging(BST *bst, int el) {
  // No caso de a árvore estar vazia.
  if (bst->root == NULL) {
    printf("A árvore está vazia.\n");
    return;
  }

  // Localizar o nó a ser removido e seu pai
  Node *parent = NULL;        // Ponteiro de referência para o pai do nó
  Node *current = bst->root;  // Ponteiro representante do nó atual

  // Enquanto o nó não for nulo nem ter info igual a el
  while (current != NULL && current->info != el) {
    parent = current;            // Atualização do ponteiro parent
    if (el < current->info)      // E caso el seja menor que o número buscado...
      current = current->left;   // O nó atual passa a ser o da esquerda
    else
      current = current->right;  // Caso contrário, o da direita
  }

  // Depois do interrompimento da busca o nó pode ter sido encontrado ou não
  if (current == NULL) {        // Se o ponteiro é nulo significa que não foi encontrado
    printf("Elemento nao encontrado na árvore.\n");
    return;
  }

  // Com o ponteiro diferente de nulo, ele deve ser removido
  // Caso 1: Nó a ser removido é uma folha
  if (current->left == NULL && current->right == NULL) {
    if (parent == NULL) {
      // Caso especial: Remover raiz (único nó na árvore)(único que não tem pai)
      free(bst->root);
      bst->root = NULL;
    } else {
      // Remover folha
      if (parent->left == current)    // Se o nó estiver à esquerda
        parent->left = NULL;          // Atualiza para NULL o ponteiro left
      else                            // Se não
        parent->right = NULL;         // Atualiza para NULL o ponteiro right
      free(current);
    }
  }
  // Caso 2: Nó a ser removido tem um filho
  else if (current->left == NULL || current->right == NULL) {
    // Escolher o nó da esquerda de current, se ele existir, ou o da direita
    // Temp é o filho único do nó a ser removido
    Node *temp = (current->left != NULL) ? current->left : current->right;

    if (parent == NULL) {
      // Caso especial: Remover raiz com um filho
      free(bst->root);
      bst->root = temp;
    } else {
      // Remover nó com um filho
      if (parent->left == current)      // Se current é o nó da esquerda
        parent->left = temp;            // O left do parent aponta pra temp
      else
        parent->right = temp;
      free(current);
    }
  }
  // Caso 3: Nó a ser removido tem dois filhos
  else {
    Node *successor = findMin(current->right);
    int tempData = successor->info;
    remove_by_merging(&current->right, successor->info);
    current->info = tempData;
  }
}

// Função para remover um nó usando a técnica de cópia
void remove_by_copying(BST *bst, int el) {
  // No caso de a árvore estar vazia.
  if (bst->root == NULL) {
    printf("A arvore esta vazia.\n");
    return;
  }

  // Localizar o nó a ser removido e seu pai
  Node *parent = NULL;        // Ponteiro de referência para o pai do nó
  Node *current = bst->root;  // Ponteiro representante do nó atual

  // Enquanto o nó não for nulo nem ter info igual a el
  while (current != NULL && current->info != el) {
    parent = current;            // Atualização do ponteiro parent
    if (el < current->info)      // E caso el seja menor que o número buscado...
      current = current->left;   // O nó atual passa a ser o da esquerda
    else
      current = current->right;  // Caso contrário, o da direita
  }

  // Depois do interrompimento da busca o nó pode ter sido encontrado ou não
  if (current == NULL) {        // Se o ponteiro é nulo significa que não foi encontrado
    printf("Elemento nao encontrado na arvore.\n");
    return;
  }

  // Com o ponteiro diferente de nulo, ele deve ser removido
  // Caso 1: Nó a ser removido é uma folha
  if (current->left == NULL && current->right == NULL) {
    if (parent == NULL) {
      // Caso especial: Remover raiz (único nó na árvore)(único que não tem pai)
      free(bst->root);
      bst->root = NULL;
    } else {
      // Remover folha
      if (parent->left == current)    // Se o nó estiver à esquerda
        parent->left = NULL;          // Atualiza para NULL o ponteiro left
      else                            // Se não
        parent->right = NULL;         // Atualiza para NULL o ponteiro right
      free(current);
    }
  }
  // Caso 2: Nó a ser removido tem um filho
  else if (current->left == NULL || current->right == NULL) {
    Node *temp = (current->left != NULL) ? current->left : current->right;

    if (parent == NULL) {
      // Caso especial: Remover raiz com um filho
      free(bst->root);
      bst->root = temp;
    } else {
      // Remover nó com um filho
      if (parent->left == current)
        parent->left = temp;
      else
        parent->right = temp;
      free(current);
    }
  }
  // Caso 3: Nó a ser removido tem dois filhos
  else {
    Node *predecessorParent = current;
    Node *predecessor = current->left;

    // Encontrar predecessor (maior nó na subárvore esquerda)
    while (predecessor->right != NULL) {
      predecessorParent = predecessor;
      predecessor = predecessor->right;
    }

    // Copiar dados do predecessor para o nó a ser removido
    current->info = predecessor->info;

    // Remover o predecessor (que agora tem no máximo um filho à esquerda)
    if (predecessorParent == current)
      predecessorParent->left = predecessor->left;
    else
      predecessorParent->right = predecessor->left;

    free(predecessor);
  }
}

// Função principal para testar as operações de remoção
int main() {
  BST treeA = createBST();
  BST treeB = createBST();

  // Testando remove_by_merging

  printf("Árvore original: \n");
  printTree(treeA.root, 0);
  printf("\n");
  int element_to_remove = 15;

  // REMOÇÃO EM ÁRVORE VAZIA
  printf("REMOÇÃO EM ÁRVORE VAZIA: \n");

  printf("Removendo de árvore vazia usando remove_by_merging: \n");
  printf("Removendo %d usando remove_by_merging: \n", element_to_remove);
  remove_by_merging(&treeA, element_to_remove);
  printf("Árvore depois da remoção do %d: \n", element_to_remove);
  printTree(treeA.root, 0);
  printf("\n");

  printf("Removendo de árvore vazia usando remove_by_copying: \n");
  printf("Removendo %d usando remove_by_copying: \n", element_to_remove);
  remove_by_copying(&treeA, element_to_remove);
  printf("Árvore depois da remoção do %d: \n", element_to_remove);
  printTree(treeA.root, 0);
  printf("\n");

  // REMOÇÃO EM ÁRVORE DE 1 NÓ
  printf("REMOÇÃO EM ÁRVORE DE 1 NÓ: \n");

  insert(&treeA, 15);
  printf("Árvore de 1 nó: \n");
  printTree(treeA.root, 0);
  printf("\n");

  printf("Removendo único nó da árvore usando remove_by_merging: \n");
  printf("Removendo %d usando remove_by_merging: \n", element_to_remove);
  remove_by_merging(&treeA, element_to_remove);
  printf("Árvore depois da remoção do %d: \n", element_to_remove);
  printTree(treeA.root, 0);
  printf("\n");

  insert(&treeA, 15);
  printf("Árvore de 1 nó: \n");
  printTree(treeA.root, 0);
  printf("\n");

  printf("Removendo único nó da árvore usando remove_by_copying: \n");
  printf("Removendo %d usando remove_by_copying: \n", element_to_remove);
  remove_by_copying(&treeA, element_to_remove);
  printf("Árvore depois da remoção do %d: \n", element_to_remove);
  printTree(treeA.root, 0);
  printf("\n");

  insert(&treeA, 15);
  insert(&treeA, 10);
  insert(&treeA, 30);
  insert(&treeA, 5);
  insert(&treeA, 11);
  insert(&treeA, 20);
  insert(&treeA, 40);
  insert(&treeA, 12);
  printf("Árvore da Figura 6.31a: \n");
  printTree(treeA.root, 0);
  printf("\n");

  // ÁRVORE (a)
  // TESTE DE REMOÇÃO DE NÓ FOLHA
  printf("REMOÇÃO DE NÓ FOLHA: \n");

  element_to_remove = 5;
  printf("Removendo nó FOLHA de (a) usando remove_by_merging: \n");
  printf("Removendo %d usando remove_by_merging: \n", element_to_remove);
  remove_by_merging(&treeA, element_to_remove);
  printf("Árvore depois da remoção do %d: \n", element_to_remove);
  printTree(treeA.root, 0);
  printf("\n");
  element_to_remove = 20;
  printf("Removendo nó FOLHA de (a) usando remove_by_copying: \n");
  printf("Removendo %d usando remove_by_copying: \n", element_to_remove);
  remove_by_copying(&treeA, element_to_remove);
  printf("Árvore depois da remoção do %d: \n", element_to_remove);
  printTree(treeA.root, 0);
  printf("\n");

  // TESTE DE REMOÇÃO DE NÓ INTERMEDIÁRIO (1 Filho)
  element_to_remove = 11;
  printf("Removendo nó INTERMEDIÁRIO COM UM FILHO: \n");
  printf("Removendo nó INTERMEDIÁRIO de (a) usando remove_by_merging: \n");
  printf("Removendo %d usando remove_by_merging: \n", element_to_remove);
  remove_by_merging(&treeA, element_to_remove);
  printf("Árvore depois da remoção do %d: \n", element_to_remove);
  printTree(treeA.root, 0);
  printf("\n");
  element_to_remove = 30;
  printf("Removendo nó INTERMEDIÁRIO de (a) usando remove_by_copying: \n");
  printf("Removendo %d usando remove_by_copying: \n", element_to_remove);
  remove_by_copying(&treeA, element_to_remove);
  printf("Árvore depois da remoção do %d: \n", element_to_remove);
  printf("Árvore depois da remoção do %d: \n", element_to_remove);
  printTree(treeA.root, 0);
  printf("\n");

  // RECONSTRUINDO A ÁRVORE...
  remove_by_merging(&treeA, 10);
  remove_by_merging(&treeA, 40);
  remove_by_merging(&treeA, 12);
  remove_by_merging(&treeA, 15);

  insert(&treeA, 15);
  insert(&treeA, 10);
  insert(&treeA, 30);
  insert(&treeA, 5);
  insert(&treeA, 11);
  insert(&treeA, 20);
  insert(&treeA, 40);
  insert(&treeA, 12);

  printf("Árvore original: \n");
  printTree(treeA.root, 0);
  printf("\n");

  // TESTE DE REMOÇÃO DE NÓ INTERMEDIÁRIO (2 filhos)
  printf("Removendo nó INTERMEDIÁRIO COM DOIS FILHOS: \n");
  element_to_remove = 10;
  printf("Removendo nó INTERMEDIÁRIO de (a) usando remove_by_merging: \n");
  printf("Removendo %d usando remove_by_merging: \n", element_to_remove);
  remove_by_merging(&treeA, element_to_remove);
  printf("Árvore depois da remoção do %d: \n", element_to_remove);
  printTree(treeA.root, 0);
  printf("\n");
  element_to_remove = 30;
  printf("Removendo nó INTERMEDIÁRIO de (a) usando remove_by_copying: \n");
  printf("Removendo %d usando remove_by_copying: \n", element_to_remove);
  remove_by_copying(&treeA, element_to_remove);
  printf("Árvore depois da remoção do %d: \n", element_to_remove);
  printTree(treeA.root, 0);
  printf("\n");

  // TESTE DE REMOÇÃO DA RAÍZ
  printf("Removendo nó RAÍZ: \n");
  element_to_remove = 15;
  printf("Removendo nó RAÍZ de (a) usando remove_by_merging: \n");
  printf("Removendo %d usando remove_by_merging: \n", element_to_remove);
  remove_by_merging(&treeA, element_to_remove);
  printf("Árvore depois da remoção do %d: \n", element_to_remove);
  printTree(treeA.root, 0);
  printf("\n");
  element_to_remove = 20;
  printf("Removendo nó RAÍZ de (a) usando remove_by_copying: \n");
  printf("Removendo %d usando remove_by_copying: \n", element_to_remove);
  remove_by_copying(&treeA, element_to_remove);
  printf("Árvore depois da remoção do %d: \n", element_to_remove);
  printTree(treeA.root, 0);
  printf("\n");
  
  // ÁRVORE (b)
  insert(&treeB, 15);
  insert(&treeB, 10);
  insert(&treeB, 30);
  insert(&treeB, 5);
  insert(&treeB, 20);
  insert(&treeB, 40);
  insert(&treeB, 4);
  insert(&treeB, 7);
  printf("Árvore da Figura 6.31b: ");
  printTree(treeB.root, 0);
  printf("\n");
  
  // TESTE DE REMOÇÃO DE NÓ FOLHA
  printf("REMOÇÃO DE NÓ FOLHA: \n");

  element_to_remove = 4;
  printf("Removendo nó FOLHA de (b) usando remove_by_merging: \n");
  printf("Removendo %d usando remove_by_merging: \n", element_to_remove);
  remove_by_merging(&treeB, element_to_remove);
  printf("Árvore depois da remoção do %d: \n", element_to_remove);
  printTree(treeB.root, 0);
  printf("\n");
  element_to_remove = 20;
  printf("Removendo nó FOLHA de (b) usando remove_by_copying: \n");
  printf("Removendo %d usando remove_by_copying: \n", element_to_remove);
  remove_by_copying(&treeB, element_to_remove);
  printf("Árvore depois da remoção do %d: \n", element_to_remove);
  printTree(treeB.root, 0);
  printf("\n");

  // TESTE DE REMOÇÃO DE NÓ INTERMEDIÁRIO (1 Filho)
  element_to_remove = 10;
  printf("Removendo nó INTERMEDIÁRIO COM UM FILHO: \n");
  printf("Removendo nó INTERMEDIÁRIO de (b) usando remove_by_merging: \n");
  printf("Removendo %d usando remove_by_merging: \n", element_to_remove);
  remove_by_merging(&treeB, element_to_remove);
  printf("Árvore depois da remoção do %d: \n", element_to_remove);
  printTree(treeB.root, 0);
  printf("\n");
  element_to_remove = 30;
  printf("Removendo nó INTERMEDIÁRIO de (b) usando remove_by_copying: \n");
  printf("Removendo %d usando remove_by_copying: \n", element_to_remove);
  remove_by_copying(&treeB, element_to_remove);
  printf("Árvore depois da remoção do %d: \n", element_to_remove);
  printf("Árvore depois da remoção do %d: \n", element_to_remove);
  printTree(treeB.root, 0);
  printf("\n");

  // RECONSTRUINDO A ÁRVORE...
  remove_by_merging(&treeB, 7);
  remove_by_merging(&treeB, 5);
  remove_by_merging(&treeB, 40);
  remove_by_merging(&treeB, 15);

  insert(&treeB, 15);
  insert(&treeB, 10);
  insert(&treeB, 30);
  insert(&treeB, 5);
  insert(&treeB, 20);
  insert(&treeB, 40);
  insert(&treeB, 4);
  insert(&treeB, 7);

  printf("Árvore original: \n");
  printTree(treeB.root, 0);
  printf("\n");

  // TESTE DE REMOÇÃO DE NÓ INTERMEDIÁRIO (2 filhos)
  printf("Removendo nó INTERMEDIÁRIO COM DOIS FILHOS: \n");
  element_to_remove = 5;
  printf("Removendo nó INTERMEDIÁRIO de (b) usando remove_by_merging: \n");
  printf("Removendo %d usando remove_by_merging: \n", element_to_remove);
  remove_by_merging(&treeB, element_to_remove);
  printf("Árvore depois da remoção do %d: \n", element_to_remove);
  printTree(treeB.root, 0);
  printf("\n");
  element_to_remove = 30;
  printf("Removendo nó INTERMEDIÁRIO de (b) usando remove_by_copying: \n");
  printf("Removendo %d usando remove_by_copying: \n", element_to_remove);
  remove_by_copying(&treeB, element_to_remove);
  printf("Árvore depois da remoção do %d: \n", element_to_remove);
  printTree(treeB.root, 0);
  printf("\n");

  // TESTE DE REMOÇÃO DA RAÍZ
  printf("Removendo nó RAÍZ: \n");
  element_to_remove = 15;
  printf("Removendo nó RAÍZ de (b) usando remove_by_merging: \n");
  printf("Removendo %d usando remove_by_merging: \n", element_to_remove);
  remove_by_merging(&treeB, element_to_remove);
  printf("Árvore depois da remoção do %d: \n", element_to_remove);
  printTree(treeB.root, 0);
  printf("\n");
  element_to_remove = 20;
  printf("Removendo nó RAÍZ de (b) usando remove_by_copying: \n");
  printf("Removendo %d usando remove_by_copying: \n", element_to_remove);
  remove_by_copying(&treeB, element_to_remove);
  printf("Árvore depois da remoção do %d: \n", element_to_remove);
  printTree(treeB.root, 0);
  printf("\n");

  return 0;
}

