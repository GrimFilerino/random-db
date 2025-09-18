#ifndef UTILS_H
#define UTILS_H

#include "../../include/mpack/mpack.h"

#define BASE_FOLDER "data-base"
#define MAX_PATH_LEN 4096
#define MAX_DOC_DATA 500000000 //500mb max doc size

void copy_node(mpack_writer_t* writer, mpack_node_t node);
char* get_uuid();
bool is_included(const char* key, const char** key_list);

#endif
