#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // required by sleep()

typedef struct {
  int x;
  int y;
} Point;

typedef struct Kstack {
  Point pos;
  struct Kstack* next;
} Kstack;

typedef int Maze10x10[10][10];
static Maze10x10 maze = {
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
  {1, 0, 0, 1, 0, 0, 0, 1, 0, 1, },
  {1, 0, 0, 1, 0, 0, 0, 1, 0, 1, },
  {1, 0, 0, 0, 0, 1, 1, 0, 0, 1, },
  {1, 0, 1, 1, 1, 0, 0, 0, 0, 1, },
  {1, 0, 0, 0, 1, 0, 0, 0, 0, 1, },
  {1, 0, 1, 0, 0, 0, 1, 0, 0, 1, },
  {1, 0, 1, 1, 1, 0, 1, 1, 0, 1, },
  {1, 1, 0, 0, 0, 0, 0, 0, 0, 1, },
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
};

void printMaze(Maze10x10 maze) {
  int i, j;
  for (i = 0; i < 10; ++i) {
    for (j = 0; j < 10; ++j) {
      switch (maze[i][j]) {
        case  0:
          putchar(' ');
          break;
        case  1:
          putchar('#');
          break;
        case  2:
          putchar('.');
          break;
        case  3:
          putchar('x');
          break;
        default:
          break;
      }
    }
    putchar(10);
  }
}

void initKstack(Kstack** S) {
  (*S) = (Kstack*)malloc(sizeof(Kstack));
  if (!(*S)) {
    puts("init malloc failed");
    exit(1);
  }
  (*S)->pos.x = -1;
  (*S)->pos.y = -1;
  (*S)->next = NULL;
}

void destroyKstack(Kstack** S) {
  Kstack* top = *S;
  while (top->next != NULL) {
    Kstack* temp = top->next;
    top->next = top->next->next;
    free(temp);
  }
  *S = NULL;
}

int pushKstack(Kstack* S, Point pos) {
  Kstack* new_top = (Kstack*)malloc(sizeof(Kstack));
  if (!new_top) {
    puts("push malloc failed!");
    return -1;
  }
  new_top->pos = pos;
  new_top->next = S->next;
  S->next = new_top;
  return 0;
}

int popKstack(Kstack* S, Point* pos) {
  if (S->next == NULL) {
    puts("pop empty stack");
    return -1;
  }
  Kstack* temp = S->next;
  S->next = S->next->next;
  *pos = temp->pos;
  free(temp);
  temp = NULL;
  return 0;
}

void visitPoint(Point pos) {
  printf( "(%d, %d)", pos.x, pos.y );
}

void traverseKstack(Kstack* S, void (*visit)(Point pos)) {
  /* visit(S->pos); */
  while (S->next != NULL) {
    S = S->next;
    visit(S->pos);
  }
  putchar(10);
}

Point mazeMove(Maze10x10 maze, Point cur) {
  maze[cur.x][cur.y] = 2; /* mark path */
  if (maze[cur.x+1][cur.y] == 0) {
    cur.x += 1;
  }
  else if (maze[cur.x][cur.y+1] == 0) {
    cur.y += 1;
  }
  else if (maze[cur.x-1][cur.y] == 0) {
    cur.x -= 1;
  }
  else if (maze[cur.x][cur.y-1] == 0) {
    cur.y -= 1;
  }
  printMaze(maze);
  sleep(1);
  return cur;
}

int isEqualPoint(Point a, Point b) {
  return a.x == b.x && a.y == b.y;
}

void mazePath(Maze10x10 maze, Kstack* workstack,
    Point start, Point end) {
  Point next, cur, prev;
  pushKstack(workstack, start); /* start point */
  cur = mazeMove(maze, start);
  next = mazeMove(maze, cur);
  while (!isEqualPoint(cur, end)) {
    if (isEqualPoint(cur, next)) {
      popKstack(workstack, &prev);
      maze[cur.x][cur.y] = 3;     /* mark impassable path */
      cur = workstack->next->pos;
    }
    else {
      pushKstack(workstack, next);
      cur = next;
    }
    next = mazeMove(maze, cur);
  }
}

int main(int argc, char *argv[]) {
  Kstack* workstack;
  initKstack(&workstack);
  Point start = {1, 1}, end = {8, 8};
  mazePath(maze, workstack, start, end);
  traverseKstack(workstack, visitPoint);
  destroyKstack(&workstack);
  sleep(1);
  return 0;
}

