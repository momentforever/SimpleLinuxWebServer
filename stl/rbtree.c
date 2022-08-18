//
// Created by lzt11 on 2022/6/7.
//

#include "rbtree.h"


rbtree_node_t* rbtree_insert_value(rbtree_node_t *root,rbtree_node_t *node,
                         rbtree_node_t *sentinel){
    for(;;){
        if(*(int*)root->key > *(int*)node->key){
            if(root->left==sentinel){
                root->left = node;
                break;
            }else{
                root = root->left;
            }
        }else if(*(int*)root->key < *(int*)node->key){
            if(root->right==sentinel){
                root->right = node;
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
    node->parent = root;
    node->left = sentinel;
    node->right = sentinel;
    rbtree_red(node);
    return node;
}

void rbtree_node_insert(rbtree_t *tree,rbtree_node_t *node){
    rbtree_node_t *root = tree->root;
    rbtree_node_t *sentinel = tree->sentinel;

    //empty
    if(rbtree_is_empty(tree)){
        node->parent = sentinel;
        node->left = sentinel;
        node->right = sentinel;
        rbtree_black(node);
        tree->root = node;
        return;
    }

    //insert
    if(tree->insert_func(root,node,sentinel) == NULL){
        printf("error\n");
        return;
    }
    //balance
    for(;;){
        //rbtree_node_t *i = node;
        rbtree_node_t *p = node->parent; // exist / root

        if(node == root){
            rbtree_black(node);
            break;
        }
        else if(p == root || p == sentinel || rbtree_is_black(p)){
            break;
        }
        else if(rbtree_is_red(p)){
            //pp exist
            rbtree_node_t *pp = node->parent->parent; // root
            rbtree_node_t *u = node->parent->parent->left == node->parent ? node->parent->parent->right:node->parent->parent->left; //sentinel
            if(u == sentinel || rbtree_is_black(u)){
                if(p == pp->left){
                    if(node == p->right){
                        //left
                        left_turn(tree,p);
                        node = node->left;
                        p = node->parent;
                        pp = p->parent;
                    }
                    //right
                    rbtree_black(p);
                    rbtree_red(pp);
                    right_turn(tree,pp);
                    break;
                }else{
                    if(node == p->left){
                        //right
                        right_turn(tree,p);
                        node = node->right;
                        p = node->parent;
                        pp = p->parent;
                    }
                    //right
                    rbtree_black(p);
                    rbtree_red(pp);
                    left_turn(tree,pp);
                    break;
                }
            }else{
                rbtree_red(pp);
                rbtree_black(p);
                rbtree_black(u);
                if(pp == root){

                    rbtree_red(pp);
                    break;
                }
                node = pp;
            }
        }else{
            // TODO 相等时异常处理
            break;
        }
    }

}

void rbtree_node_delete(rbtree_t *tree,rbtree_node_t *node){

    // empty
    if(rbtree_is_empty(tree)){
        return;
    }
    if(node==tree->root && node->left == tree->sentinel && node->right == tree->sentinel){
        tree->root = tree->sentinel;
        return;
    }

    rbtree_node_t *replace_node;

    //replace
    if(node->left != tree->sentinel && node->right != tree->sentinel){
        replace_node = node->right;
        while(replace_node->left != tree->sentinel){
            replace_node = replace_node->left;
        }
        switch_node(tree,node,replace_node);
    }
    if(node->left == tree->sentinel && node->right != tree->sentinel){
        replace_node = node->right;
        switch_node(tree,node,replace_node);
    }
    else if(node->left != tree->sentinel && node->right == tree->sentinel){
        replace_node = node->left;
        switch_node(tree,node,replace_node);
    }

    replace_node = node;

balance:
    // balance
    if(rbtree_is_red(node)){
        rbtree_black(node);
    }
    else{
        rbtree_node_t *s;
        rbtree_node_t *sl;
        rbtree_node_t *sr;
        if(node->parent->left == node){
            s = node->parent->right;
            sl = s->left;
            sr = s->right;
            if(rbtree_is_red(s)){
                rbtree_black(s);
                rbtree_red(node->parent);
                left_turn(tree, node->parent);

                goto left_double_black;
            }
            else{
                if(rbtree_is_red(sr) || rbtree_is_red(sl)) {
                    if(rbtree_is_black(sr)){
                        rbtree_red(s);
                        rbtree_black(sl);
                        right_turn(tree,s);

                        s = node->parent->right;
                        sl = s->left;
                        sr = s->right;
                    }
                    rbtree_set_color(s, node->parent);
                    rbtree_black(node->parent);
                    rbtree_black(sr);
                    left_turn(tree, node->parent);
                }
                else{
left_double_black:
                    rbtree_red(node->parent->right);
                    node = node->parent;

                    goto balance;
                }
            }

        }
        else{
            s = node->parent->left;
            sl = s->left;
            sr = s->right;
            if(rbtree_is_red(s)){
                rbtree_black(s);
                rbtree_red(node->parent);
                right_turn(tree, node->parent);


                goto right_double_black;
            }
            else{
                if(rbtree_is_red(sr) || rbtree_is_red(sl)) {
                    if(rbtree_is_black(sr)){
                        rbtree_red(s);
                        rbtree_black(sr);
                        left_turn(tree,s);
                        s = node->parent->left;
                        sl = s->left;
                        sr = s->right;
                    }
                    rbtree_set_color(s, node->parent);
                    rbtree_black(node->parent);
                    rbtree_black(sr);
                    right_turn(tree, node->parent);

                }
                else{
right_double_black:
                    rbtree_red(node->parent->left);
                    node = node->parent;

                    goto balance;
                }
            }

        }

    }

    //delete
    (replace_node->parent->left == replace_node)?(replace_node->parent->left = tree->sentinel):(replace_node->parent->right = tree->sentinel);

}

void switch_node(rbtree_t *tree, rbtree_node_t *a,rbtree_node_t *b){
    rbtree_node_t *tmp_a;
    rbtree_node_t *tmp_b;

    tmp_a = a->parent;
    tmp_b = b->parent;
    if(tmp_a == tree->sentinel){
        //root
        tree->root = b;
    }else{
        (a->parent->left== a) ? (tmp_a->left = b) : (tmp_a->right = b);
    }
    (b->parent->left== b) ? (tmp_b->left = a) : (tmp_b->right = a);
    b->parent = tmp_a;
    a->parent = tmp_b;

    tmp_a = a->left;
    tmp_b = b->left;
    if(tmp_a != tree->sentinel){
        tmp_a->parent = b;
    }
    if(tmp_b != tree->sentinel){
        tmp_b->parent = a;
    }
    a->left = tmp_b;
    b->left = tmp_a;

    tmp_a = a->right;
    tmp_b = b->right;
    if(tmp_a != tree->sentinel){
        tmp_a->parent = b;
    }
    if(tmp_b != tree->sentinel){
        tmp_b->parent = a;
    }
    a->right = tmp_b;
    b->right = tmp_a;

    //color
    if(a->color!=b->color){
        a->color = ~a->color;
        b->color = ~b->color;
    }

}

void left_turn(rbtree_t *tree, rbtree_node_t *node){
    rbtree_node_t *right = node->right;
    rbtree_node_t *rightleft = right->left;
    rbtree_node_t *parent = node->parent;

    if(parent != tree->sentinel){
        if(parent->left == node){
            parent->left = right;
        }else{
            parent->right = right;
        }
    }else{
        tree->root = right;
    }
    right->parent = node->parent;
    right->left = node;
    node->parent = right;

    node->right = rightleft;

    if(rightleft!= tree->sentinel){
        rightleft->parent = node;
    }
}

void right_turn(rbtree_t *tree, rbtree_node_t *node){
    rbtree_node_t *left = node->left;
    rbtree_node_t *leftright = left->right;
    rbtree_node_t *parent = node->parent;

    if(parent != tree->sentinel){
        if(parent->left == node){
            parent->left = left;
        }else{
            parent->right = left;
        }
    }else{
        tree->root = left;
    }
    left->parent = node->parent;
    left->right = node;
    node->parent = left;

    node->left = leftright;
    if(leftright!=tree->sentinel){
        leftright->parent = node;
    }
}

void rbtree_middleorder(rbtree_t *tree,rbtree_node_t *node){
    if(node==tree->sentinel)return;
    rbtree_middleorder(tree,node->left);
    printf("%d ",*(int*)node->key);
    rbtree_middleorder(tree,node->right);
}