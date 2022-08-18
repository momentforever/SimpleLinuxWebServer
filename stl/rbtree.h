//
// Created by lzt11 on 2022/6/7.
//

#ifndef SOCKET_RBTREE_H
#define SOCKET_RBTREE_H

#include "lib.h"


typedef struct rbtree_node_s rbtree_node_t;

struct rbtree_node_s{
    void* key;

    rbtree_node_t *left;
    rbtree_node_t *right;
    rbtree_node_t *parent;

    char color;
};

typedef struct rbtree_s rbtree_t;
typedef rbtree_node_t* (*rbtree_insert_pt)(rbtree_node_t *root,rbtree_node_t *node,
                                           rbtree_node_t *sentinel);

struct rbtree_s{
    rbtree_node_t* root;
    rbtree_node_t* sentinel;
    rbtree_insert_pt insert_func;
};

#define rbtree_init(tree,s,func)                  \
        (tree)->root = s;                         \
        (tree)->insert_func = func;               \
        (tree)->sentinel = s;                     \
        rbtree_black((tree)->sentinel);

rbtree_node_t* rbtree_insert_value(rbtree_node_t *root,rbtree_node_t *node,
                         rbtree_node_t *sentinel);

void rbtree_node_insert(rbtree_t *tree,rbtree_node_t *node);

void rbtree_node_delete(rbtree_t *tree,rbtree_node_t *node);


#define rbtree_red(node) ((node)->color=1)
#define rbtree_black(node) ((node)->color=0)
#define rbtree_set_color(node,node_cpy) ((node)->color = (node_cpy)->color)
#define rbtree_is_red(node) ((node)->color)
#define rbtree_is_black(node) (!((node)->color))

#define rbtree_is_empty(tree) ((tree)->root == (tree)->sentinel)

void left_turn(rbtree_t *tree, rbtree_node_t *node);
void right_turn(rbtree_t *tree, rbtree_node_t *node);
void switch_node(rbtree_t *tree, rbtree_node_t *a,rbtree_node_t *b);

void rbtree_middleorder(rbtree_t *tree,rbtree_node_t *node);

#endif //SOCKET_RBTREE_H
