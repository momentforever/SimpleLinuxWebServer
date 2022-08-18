//
// Created by lzt11 on 2022/6/16.
//

#include "timer.h"
#include "clock.h"


rbtree_node_t* rbtree_insert_timer(rbtree_node_t *root,rbtree_node_t *node,
                                   rbtree_node_t *sentinel){
    for(;;){
        if(((timer_node_t*)root->key)->timeout_time > ((timer_node_t*)node->key)->timeout_time){
            if(root->left==sentinel){
                break;
            }else{
                root = root->left;
            }
        }else if(((timer_node_t*)root->key)->timeout_time < ((timer_node_t*)node->key)->timeout_time){
            if(root->right==sentinel){
                break;
            }else{
                root = root->right;
            }
        }else{
            //==
            //break;
            return NULL;
        };
    }
    //insert
    root->right = node;
    node->parent = root;
    node->left = sentinel;
    node->right = sentinel;
    rbtree_red(node);
    return node;
}

timer_rbtree_t* timer_tree_create(){
    timer_rbtree_t *timer_tree;
    timer_tree = malloc(sizeof(timer_rbtree_t));
    timer_tree_init(timer_tree);
    return timer_tree;
}

void timer_tree_init(timer_rbtree_t *timer_tree){
    rbtree_init(&timer_tree->rbtree,&timer_tree->sentinel,rbtree_insert_timer);
}

void timer_tree_timer_add(timer_rbtree_t *timer_tree,timer_node_t *timer,time_t ms){

    time_t key;

    key = current_monotonic_timestamp + ms;
    if(timer->timed_set){
        if( key - timer->timeout_time < TIMER_LAZY_DELAY){
            return;
        }
        rbtree_node_delete(&timer_tree->rbtree,&timer->rbtree_node);
    }

    timer->timeout_time = key;
    timer->start_time = current_monotonic_timestamp;
    timer->rbtree_node.key = (void*)timer;
    timer->timed_set = ON;

    rbtree_node_insert(&timer_tree->rbtree,&timer->rbtree_node);
}

void timer_tree_timer_delete(timer_rbtree_t *timer_tree,timer_node_t *timer){
    timer->timed_set = 0;
    rbtree_node_delete(&timer_tree->rbtree,&timer->rbtree_node);
}

timer_node_t *timer_tree_get_min_time(timer_rbtree_t *timer_tree){
    rbtree_node_t *node = timer_tree->rbtree.root;
    if(rbtree_is_empty(&timer_tree->rbtree)){
        return NULL;
    }
    for(;;){
        if(node->left != &timer_tree->sentinel){
            node = node->left;
        }else if(node->right != &timer_tree->sentinel){
            node = node->right;
        }else{
            return (timer_node_t*)node->key;
        }
    }
}

timer_node_t *timer_tree_get_least_time(timer_rbtree_t *timer_tree){
    rbtree_node_t *node = timer_tree->rbtree.root;
    if(rbtree_is_empty(&timer_tree->rbtree)){
        return NULL;
    }
    for(;;){
        if(node->left != &timer_tree->sentinel || ((timer_node_t*)node->left->key)->timed_out == OFF){
            node = node->left;
        }else if(node->right != &timer_tree->sentinel || ((timer_node_t*)node->right->key)->timed_out == OFF){
            node = node->right;
        }else{
            if(((timer_node_t*)node->key)->timed_out == ON){
                return NULL;
            }else{
                return (timer_node_t*)node->key;
            }
        }
    }
}

void rec_timer_set_timed_out(rbtree_node_t *node,rbtree_node_t *sentinel){
    if(node == sentinel){
        return;
    }
    ((timer_node_t*)node->key)->timed_out = ON;
    //need handler

    rec_timer_set_timed_out(node->left,sentinel);
    rec_timer_set_timed_out(node->right,sentinel);
}

void timer_tree_update(timer_rbtree_t *timer_tree){
    rbtree_node_t *node = timer_tree->rbtree.root;
    for(;;){
        if(node == &timer_tree->sentinel){
            return;
        }
        if(((timer_node_t*)node->key)->timeout_time < current_monotonic_timestamp){
            ((timer_node_t*)node->key)->timed_out = ON;
            //need handler

            break;
        }else{
            node = node->left;
        }
    }
    rec_timer_set_timed_out(node->left,&timer_tree->sentinel);
}