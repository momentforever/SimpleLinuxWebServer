#include "util.h"

int path_join(char* s, ...){
    // Linux
    return 0;
}

int find_one_pos(int num) {
    int position = 0;
    while (!(num & 1)) {
        num >>= 1;
        position++;
    }
    return position;
}
