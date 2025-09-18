#include "utils.h"


/*
*   Description : 
*   get_uuid() generates a random uuid (taken from stack overflow they know better xD) 
*
*
*
*   Return Value :
*   void but does return to the writer a copy of the node 
*/
void copy_node(mpack_writer_t* writer, mpack_node_t node) {
    switch (mpack_node_type(node)) {
        case mpack_type_nil:
            mpack_write_nil(writer);
            break;
        case mpack_type_bool:
            mpack_write_bool(writer, mpack_node_bool(node));
            break;
        case mpack_type_int:
            mpack_write_int(writer, mpack_node_i64(node));
            break;
        case mpack_type_uint:
            mpack_write_uint(writer, mpack_node_u64(node));
            break;
        case mpack_type_float:
            mpack_write_float(writer, mpack_node_float(node));
            break;
        case mpack_type_double:
            mpack_write_double(writer, mpack_node_double(node));
            break;
        case mpack_type_str:
            mpack_write_str(writer, mpack_node_str(node), mpack_node_strlen(node));
            break;
        case mpack_type_bin:
            mpack_write_bin(writer, mpack_node_bin_data(node), mpack_node_bin_size(node));
            break;
        case mpack_type_array: {
            size_t count = mpack_node_array_length(node);
            mpack_start_array(writer, count);
            for (size_t i = 0; i < count; ++i)
                copy_node(writer, mpack_node_array_at(node, i));
            mpack_finish_array(writer);
            break;
        }
        case mpack_type_map: {
            size_t count = mpack_node_map_count(node);
            mpack_start_map(writer, count);
            for (size_t i = 0; i < count; ++i) {
                mpack_node_t key = mpack_node_map_key_at(node, i);
                mpack_node_t val = mpack_node_map_value_at(node, i);
                copy_node(writer, key);
                copy_node(writer, val);
            }
            mpack_finish_map(writer);
            break;
        }
        default:
            // handle error
            mpack_writer_flag_error(writer, mpack_error_bug);
            break;
    }
}

/*
*   Description : 
*   get_uuid() generates a random uuid (taken from stack overflow they know better xD) 
*
*
*
*   Return Value :
*   returns a string as a uuid
*/
char* get_uuid() {
    char v[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    
    static _Thread_local char buf[37] = {0};

    for(int i = 0; i < 36; ++i) {
        buf[i] = v[rand()%16];
    }

    buf[8] = '-';
    buf[13] = '-';
    buf[18] = '-';
    buf[23] = '-';
    buf[36] = '\0';

    return buf;
}

/*
*   Description : 
*   is_included() checks if key is included in key_list
*
*
*   Return Value :
*   returns true if key is in key_list 
*/
bool is_included(const char* key, const char** key_list) {

    int len = 0;
    while(key_list[len] != NULL) {
        const char* current_key = key_list[len];

        if(strcmp(key, current_key) != 0) {
            return true;
        }

        ++len;
    }


    return false;
}

