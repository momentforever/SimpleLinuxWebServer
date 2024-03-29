//
// Created by lzt11 on 2022/5/13.
//

#include "cycle_module.h"
#include "listening_module.h"

int cycle_parse_json(cJSON* data,config_node_t *cn, config_t* c);
int cycle_config_init(cJSON* data, config_node_t *cn, config_t* c);

static command_t cycle_commands[] = {
        {
                "cycle",
                MAIN_MODULE,
                ALL_MAIN,
                UNSET,
                UNSET,
                UNSET,
                cycle_parse_json,
        },
        NULL
};

module_t cycle_module = {
        MODULE_START,
        "cycle_module",
        MAIN_MODULE,
        UNSET,
        UNSET,
        cycle_config_init,
        UNSET,
        UNSET,
        UNSET,
        cycle_commands,
};

int cycle_config_init(cJSON* data, config_node_t *cn, config_t* c){
    cycle_config_t *ccf;
    ccf = pmalloc(c->pool,sizeof(cycle_config_t));
    if(ccf == NULL){
        return ERROR;
    }
    ccf->connection = 512;
    ccf->process = 1;

    cn->data[cycle_module.main_index] = ccf;
    return OK;
}

int cycle_parse_json(cJSON* data, config_node_t *cn, config_t* c){
    cJSON* json;
    cJSON* cycle_json = cJSON_GetObjectItem(data,"cycle");
    cycle_config_t *cf = cn->data[cycle_module.main_index];

    json = cJSON_GetObjectItem(cycle_json,"connection");
    if(json!=NULL){
        cf->connection = json->valueint;
    }
    debugln("connection -> %d", cf->connection);

    json = cJSON_GetObjectItem(cycle_json,"process");
    if(json!=NULL){
        cf->process = json->valueint;
    }
    debugln("process -> %d", cf->process);

    return OK;
}

void connection_init(connection_t *c){
    c->data = NULL;
    c->fd = 0;
    memset(&c->clnt_addr,0,sizeof(struct sockaddr));
    c->clnt_addr_size = 0;
    c->listening = NULL;
    event_init(c->read);
    event_init(c->write);
    if(c->pool == NULL){
        c->pool=memory_pool_create();
    }else{
        memory_pool_init(c->pool);
    }
}

cycle_t* cycle_create(config_t *c){

    cycle_config_t *ccf = c->root->data[cycle_module.main_index];
    int connection_n = ccf->connection;

    cycle_t *cycle;
    cycle = malloc(sizeof(cycle_t));
    if(cycle==NULL)return NULL;
    memset(cycle,0, sizeof(cycle_t));

    cycle->pool = memory_pool_create();
    if(cycle->pool==NULL)return NULL;

    cycle->connection_n = connection_n;
    cycle->connection_cur = 0;

    cycle->connections = pmalloc(cycle->pool,connection_n * sizeof(connection_t));
    if(cycle->connections==NULL){
        return NULL;
    }
    cycle->read_events = pmalloc(cycle->pool, connection_n * sizeof(event_t));
    if(cycle->read_events==NULL){
        return NULL;
    }
    cycle->write_events = pmalloc(cycle->pool, connection_n * sizeof(event_t));
    if(cycle->write_events==NULL){
        return NULL;
    }

    int n;
    //init event
    connection_t *cs = cycle->connections;
    event_t *res = cycle->read_events;
    event_t *wes = cycle->write_events;
    n = 0;
    while(1){
        //double ptr
        cs[n].read = &res[n];
        res[n].data = &cs[n];
        cs[n].write = &wes[n];
        wes[n].data = &cs[n];
        connection_init(&cs[n]);

        if(n >= connection_n - 1){
            cs[n].data = NULL;
            break;
        }
        cs[n].data = &cs[n+1];

        n++;
    }
    cycle->free_connection = cycle->connections;

//    cycle->config = config_create(cycle->config);
//    pget(cycle->pool,cycle->config,config_delete_void);
    cycle->config = c;
    pget(cycle->pool,cycle->config,config_delete_void);

    cycle->listenings = listenings_create(cycle->config);

    return cycle;
}

connection_t *get_free_connection(cycle_t *cycle){
    if(cycle->free_connection == NULL){
        return NULL;
    }
    connection_t *conn = cycle->free_connection;
    // next
    cycle->free_connection = (connection_t*)cycle->free_connection->data;
    connection_init(conn);
    cycle->connection_cur+=1;
    debug_assert(cycle->connection_cur<=cycle->connection_n);
    return conn;
}

int release_connection(cycle_t *cycle,connection_t *conn){
    conn->data = (void*)cycle->free_connection;
    //TODO 释放request
    pfree(conn->pool);
    cycle->free_connection = (connection_t*)conn;
    cycle->connection_cur-=1;
    debug_assert(cycle->connection_cur>=0);
    return OK;
}

void cycle_delete(cycle_t* cycle){
    memory_pool_delete(cycle->pool);
    free(cycle);
}

void cycle_process_fork(cycle_t *cycle){
    cycle_config_t *ccf = cycle->config->root->data[cycle_module.main_index];
    int worker_process = ccf->process;

    int pid = 0;
    set_proctitle(g_argv,environ,"slws_master");

    for (int i = 0; i < worker_process;i++) {
        pid = fork();
        if (pid == 0) {
            set_proctitle(g_argv,environ,"slws_worker");
            g_process_type = WORKER;
            debugln("worker%d -> %d", i, getpid());
            break;
        }
    }
}

void cycle_process_restart(){
    debugln("worker is restart");
    int pid;
    pid = fork();
    if (pid == 0) {
        set_proctitle(g_argv,environ,"slws_worker");
        g_process_type = WORKER;
    }
}

void set_proctitle(char** argv,char** env, const char* name){
    debugln("set_proctitle -> %s",name);
    //quick path
    if(strlen(argv[0]) > strlen(name)){
        memset(argv[0],0,strlen(argv[0]));
        strncpy(argv[0],name,strlen(name));
        return;
    }
    //slow path
    int size = 0;
    int i;
    for(i = 0;env[i];i++){
        size += (int)strlen(env[i]) + 1;
    }
    char* p = (char*)malloc(size);
    char* last_argv = argv[0];
    for(i = 0;argv[i];i++){
        if(last_argv == argv[i]){
            last_argv = argv[i] + strlen(argv[i]) + 1;
        }
    }

    for(i = 0;env[i];i++){
        if(last_argv == env[i]){
            size = (int)strlen(env[i]) + 1;
            last_argv = env[i] + size;

            memcpy(p,env[i],size);
            env[i]=(char*)p;
            p += size;
        }
    }
    last_argv--;
    strncpy(argv[0],name,last_argv - argv[0]);
    p = argv[0] + strlen(argv[0]) + 1;
    if(last_argv - p > 0){
        memset(p,0,last_argv - p);
    }
}