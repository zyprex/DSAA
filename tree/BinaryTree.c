#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define EXIT_NULL(x) if((x) == NULL){ return; }
#define MAX(x, y) ((x) > (y)) ? (x) : (y)
#define MIN(x, y) ((x) < (y)) ? (x) : (y)

typedef char DataType;
typedef struct Btree {
  DataType data;
  struct Btree *left_child, *right_child;
} Btree;

void initBtreeNode(Btree** T, DataType data) {
  *T = (Btree*)malloc(sizeof(Btree));
  (*T)->data = data;
  (*T)->left_child = NULL;
  (*T)->right_child = NULL;
}

/**
 * \fn            createBtreeFromArray
 * \brief         create binary tree from array
 * \param[in]     T: binary tree
 * \param[in]     i: index of node
 * \param[in]     data_arr: level order array of a full binary tree
 *                          , use '?' represent NULL node
 * \param[in]     data_arr_len: length of data_arr
 * \return        none
 */
void createBtreeFromArray(Btree** T, int i, DataType* data_arr, int data_arr_len) {
  if (data_arr[i] == '?' || i >= data_arr_len) {
    return;
  }
  initBtreeNode(T, data_arr[i]);
  createBtreeFromArray(&((*T)->left_child), 2 * i + 1, data_arr, data_arr_len);
  createBtreeFromArray(&((*T)->right_child), 2 * i + 2, data_arr, data_arr_len);
}

void destroyBtreePreOrder(Btree* T) {
  EXIT_NULL(T);
  Btree* lChild = T->left_child;
  Btree* rChild = T->right_child;
  free(T);
  destroyBtreePreOrder(lChild);
  destroyBtreePreOrder(rChild);
}

void preOrder(Btree* T) {
  EXIT_NULL(T);
  printf("%3c", T->data);
  preOrder(T->left_child);
  preOrder(T->right_child);
}

void inOrder(Btree* T) {
  EXIT_NULL(T);
  inOrder(T->left_child);
  printf("%3c", T->data);
  inOrder(T->right_child);
}

void postOrder(Btree* T) {
  EXIT_NULL(T);
  postOrder(T->left_child);
  postOrder(T->right_child);
  printf("%3c", T->data);
}

void traverseBtree(Btree* T, void (*visit)(Btree* T)) {
  visit(T);
  putchar(10);
}

/* The maximum depth is the number of nodes along the longest path from
 * the root node down to the farthest leaf node. */
int maxDepthBtree(Btree* T) {
  if (T == NULL) return 0;
  return MAX(
      maxDepthBtree(T->left_child),
      maxDepthBtree(T->right_child)) + 1;
}

void levelOrderCurrentLevel(Btree* T, int level) {
  if (T == NULL) return;
  if (level == 1) {
    printf("%3c", T->data);
  }
  else if (level > 1) {
    levelOrderCurrentLevel(T->left_child, level - 1);
    levelOrderCurrentLevel(T->right_child, level - 1);
  }
}

void levelOrder(Btree* T) {
  int depth = maxDepthBtree(T);
  int i;
  for (i = 1; i <= depth; ++i) {
    levelOrderCurrentLevel(T, i);
  }
}

int minDepthBtree(Btree* T) {
  if (T == NULL) return 0;
  if (T->left_child == NULL && T->right_child == NULL) {
    return 1;
  }
  if (T->left_child == NULL) {
    return minDepthBtree(T->right_child) + 1;
  }
  else if (T->right_child == NULL) {
    return minDepthBtree(T->left_child) + 1;
  }
  else {
    return MIN(
        minDepthBtree(T->left_child),
        minDepthBtree(T->right_child)) + 1;
  }
}

int main(int argc, char *argv[]) {
  Btree* tree;
  DataType data_arr[] = "ABCDE?F?G????HI";
  createBtreeFromArray(&tree, 0, data_arr, strlen(data_arr));
  traverseBtree(tree, preOrder);
  traverseBtree(tree, inOrder);
  traverseBtree(tree, postOrder);
  traverseBtree(tree, levelOrder);
  printf("%d\n", maxDepthBtree(tree));
  printf("%d\n", minDepthBtree(tree));
  destroyBtreePreOrder(tree);
  return 0;
}
