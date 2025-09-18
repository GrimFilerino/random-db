#include "indexing.h"

#include "../io/io.h"

/*
*   Description : 
*   add_to_index_file() adds an id to the end of a document to a index doc with inputed name
*
*
*
*   Return Value :
*   returns 0 if success and -1 for failure to open doc in append mode
*/
int add_to_index_file(const char* collection, const char* index_name, const char* id) {
    char index_path[MAX_PATH_LEN];
    const char* home = getenv("HOME");

    snprintf(index_path, sizeof(index_path), "%s/%s/%s/%s.indx", home, BASE_FOLDER, collection, index_name);
    
    FILE* index_file = fopen(index_path, "a");
    if(!index_file) {
        return -1;
    }

    fprintf(index_file, "%s\n", id);
    fclose(index_file);

    return 0;
}


/*
*   Description :
*   remove_from_index_file() removes id from the inputed index doc.
*

*
*   Return Value : 
*   returns 0 if success and -1 for failure to read doc. -2 for 
*   failure to write to doc
*/
int remove_from_index_file(const char* collection, const char* index_name, const char* id) {
    char index_path[MAX_PATH_LEN];
    snprintf(index_path, sizeof(index_path), "%s/%s/%s/%s.indx", BASE_FOLDER, collection, index_name);
    
    File index_file = io_file_read(index_path);

    if(!index_file.is_valid || index_file.data == NULL) {
        return -1;
    }

    char* line = strtok(index_file.data,"\n");
    char* new_data = malloc(index_file.len);
    new_data[0] = '\0';

    while(line != NULL) {
        if(strcmp(line, id) != 0) {
            strcat(new_data, "\n");
            strcat(new_data, line);
        }
        
        line = strtok(NULL, "\n");
    }
   
    if(io_file_write(new_data, sizeof(new_data), index_path) == -1) {
        return -2;
    }

    free(new_data);
    free(index_file.data);
    
    return 0;
}




/*
*   Description : 
*   grep_by_index() is a function that greps documents based 
*   on a pre created index doc with document ids sorted 
*   from first added to last added at the end
*
*
*   Return Value : 
*   returns an array of msgpack data for all the ids in the index or NULL if it fails to read doc
*/
char** grep_by_index(const char* collection, const char* index_name, int limit) {
    char id_index_path[MAX_PATH_LEN];

    const char* home = getenv("HOME");
    snprintf(id_index_path, sizeof(id_index_path), "%s/%s/%s/ids.indx", home, BASE_FOLDER, collection);
    
    File ids = io_file_read(id_index_path);

    if(!ids.is_valid || ids.data == NULL) {
        return NULL;
    }

    char* line = strtok(ids.data, "\n");
    char** data = malloc(sizeof(ids.len)* MAX_DOC_DATA);
    int row = 0;

    while(line != NULL) {
        if(limit != 0 && row >= limit-1) { 
            break;
        }
    
        char doc_path[MAX_PATH_LEN];
        snprintf(id_index_path, sizeof(id_index_path), "%s/%s/%s/%s.pson", home, BASE_FOLDER, collection, line);
    
        File doc = io_file_read(doc_path);

        if(!doc.is_valid || doc.data == NULL) {
            continue;
        }

        data[row] = doc.data;

        ++row;
        line = strtok(NULL, "\n");
    }
    
    return data;
}
