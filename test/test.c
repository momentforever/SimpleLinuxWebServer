////
//// Created by lzt11 on 2022/7/21.
////
//
//#include "test.h"
//
//#include "core.h"
//#include "lib.h"
//#include "common.h"
//#include "stl.h"
//#include <yaml.h>
//
//typedef struct part_s part_t;
//
//struct part_s{
//    int key;
//    int value;
//    rbtree_node_t rb_node;
//};
//
//void rbtree_test(){
//    rbtree_node_t sentinel;
//    sentinel.left = NULL;
//    sentinel.right =NULL;
//    sentinel.parent = NULL;
//    rbtree_t tree;
//    rbtree_init(&tree,&sentinel,rbtree_insert_value);
//
//    part_t ps[20];
//    part_t *p = ps;
//    srand((unsigned)time(NULL));
//    int s;
//    for(int i = 0;i<3;i++){
//        //p = malloc(sizeof(part_t));
//        if(i==0){
//            s=1;
//        }else if(i==1){
//            s=0;
//        }else{
//            s=2;
//        }
//        p->value = 0;
//        p->key = s;
//        p->rb_node.key = (void*)&p->key;
//        printf("node->%d\n",p->key);
//        rbtree_node_insert(&tree,&p->rb_node);
//        rbtree_middleorder(&tree,tree.root);
//        printf("\n");
//        p++;
//    }
//    printf("-------------------\n");
//    printf("node->%d\n",ps[1].key);
//    rbtree_node_delete(&tree,&ps[1].rb_node);
//    rbtree_middleorder(&tree,tree.root);
//    printf("\n");
//
//    printf("node->%d\n",ps[0].key);
//    rbtree_node_delete(&tree,&ps[0].rb_node);
//    rbtree_middleorder(&tree,tree.root);
//    printf("\n");
//
//    printf("node->%d\n",ps[2].key);
//    rbtree_node_delete(&tree,&ps[2].rb_node);
//    rbtree_middleorder(&tree,tree.root);
//    printf("\n");
//
//
//    //p = malloc(sizeof(part_t) * 20);
////    for(int i = 0;i<20;i++){
////        //p = malloc(sizeof(part_t));
////        p->value = 0;
////        p->key = rand() % 100;
////        p->rb_node.key = (void*)&p->key;
////        printf("node->%d\n",p->key);
////        rbtree_node_insert(&tree,&p->rb_node);
////        rbtree_middleorder(&tree,tree.root);
////        printf("\n");
////        p++;
////    }
////
////    printf("node->%d\n",ps[0].key);
////    rbtree_node_delete(&tree,&ps[0].rb_node);
////    rbtree_middleorder(&tree,tree.root);
////    printf("\n");
////
////    printf("node->%d\n",ps[15].key);
////    rbtree_node_delete(&tree,&ps[15].rb_node);
////    rbtree_middleorder(&tree,tree.root);
////    printf("\n");
//
//}
//
//void cjson_test(){
////    FILE *config_fd = fopen("../config.json","r");
////    if(config_fd == NULL){
////        return;
////    }
////    char *config = malloc(1024);
////    char *ptr = malloc(255);
////    while(fgets(ptr,SMALL_BUF,config_fd)!=NULL){
////        strcat(config,ptr);
////    }
////    fclose(config_fd);
//    FILE *config_fd = fopen("../config.json","r");
//    if(config_fd == NULL){
//        return;
//    }
//    //查找文件尾偏移
//    fseek(config_fd,0,SEEK_END);
//    int length;
//    //计算长度
//    length = ftell(config_fd);
//    char *config = malloc((length + 1) * sizeof(char));
//    //回到文件头
//    rewind(config_fd);
//    length = fread(config,1,length,config_fd);
//    config[length] = '\0';
//    fclose(config_fd);
//
//    printf("%s\n",config);
//
//    cJSON* cjson = NULL;
//    cjson = cJSON_Parse(config);
//    cJSON *version;
//    version = cJSON_GetObjectItem(cjson,"version");
//    printf("version: %s\n",version->valuestring);
//
//};
//
//void tcp_handler(event_t* ev){
//    printf("tcp handler\n");
//}
//
//void udp_handler(event_t* ev){
//    printf("udp handler\n");
//}
//
//void http_handler(event_t* ev){
//    if(ev->rw == W){
//        printf("http write handler\n");
//    }else if(ev->rw == R){
//        printf("http read handler\n");
//    }
//}
//
//
//void yaml_test(){
//    FILE *config_fd = fopen("../config.yaml","r");
//    if(config_fd == NULL){
//        return;
//    }
//    yaml_parser_t parser;
//    yaml_token_t token;
//
//    if(!yaml_parser_initialize(&parser))
//        fputs("Fail to initialize parser!\n",stderr);
//
//    yaml_parser_set_input_file(&parser,config_fd);
//
//    do{
//        yaml_parser_scan(&parser,&token);
//        switch (token.type) {
//            case YAML_STREAM_START_TOKEN:
//                puts("STREAM START");break;
//            case YAML_STREAM_END_TOKEN:
//                puts("STREAM END");break;
//            case YAML_KEY_TOKEN:
//                printf("(Key token) ");break;
//            case YAML_VALUE_TOKEN:
//                printf("(Value token) ");break;
//            case YAML_BLOCK_SEQUENCE_START_TOKEN:
//                puts("<b>Start Block(Sequence)</b>");break;
//            case YAML_BLOCK_ENTRY_TOKEN:
//                puts("<b>Start Block(Entry)</b>");break;
//            case YAML_BLOCK_END_TOKEN:
//                puts("<b>End Block</b>");break;
//            case YAML_BLOCK_MAPPING_START_TOKEN:
//                puts("[Block mapping]");break;
//            case YAML_SCALAR_TOKEN:
//                printf("scalar %s \n",token.data.scalar.value);break;
//            default:
//                printf("..... %d\n",token.type);
//        }
//        if(token.type!=YAML_STREAM_END_TOKEN)
//            yaml_token_delete(&token);
//    }while(token.type!=YAML_STREAM_END_TOKEN);
//    yaml_token_delete(&token);
//
//    yaml_parser_delete(&parser);
//    fclose(config_fd);
//}
//

#include "common.h"

void memory_test(){
    memory_pool_t *pool;
    pool = memory_pool_create();
    int *a = pmalloc(pool,sizeof(int));
    *a = 10;
    debugln("!");

    int *b = malloc(sizeof(int));
    *b = 200;
    debugln("%p", b);
    pget(pool,b,free);

    debugln("%d", *(int*)(pool->end->ptr));
//    int *b = pmalloc(pool,sizeof(int));
//    *b = 20;


    debugln("%ld", pool->size);

    pfree(pool);

    debugln("%ld", pool->size);
}