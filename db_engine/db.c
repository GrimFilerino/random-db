#include "db.h"
#include "io/io.h"

#include "include/cJSON.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE_PATH "~/data-base/"
#define MAX_PATH_LEN 4096
#define MAX_DOC_DATA 500000000 //500mb max file size



/*
*   Description : 
*   get_uuid() generates a random uuid 
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
*   add_to_index_file() adds an id of a document to a index file with inputed name
*
*
*
*   Return Value :
*   returns 0 if success and -1 for failure to open file in append mode
*/
int add_to_index_file(const char* collection, const char* index_name, const char* id) {
    char index_path[MAX_PATH_LEN];
    snprintf(index_path, sizeof(index_path), "%s/%s/%s.indx", BASE_PATH, collection, index_name);
    
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
*   remove_from_index_file() removes id from the inputed index file.
*
*
*
*   Return Value : 
*   returns 0 if success and -1 for failure to read file. -2 for 
*   failure to write to file
*/
int remove_from_index_file(const char* collection, const char* index_name, const char* id) {
    char index_path[MAX_PATH_LEN];
    snprintf(index_path, sizeof(index_path), "%s/%s/%s.indx", BASE_PATH, collection, index_name);
    
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
*   remove_fields() removes json fields form document. 
*   original is a pointer to the inputed cJSON object to mutate it
*   and fields is an array of strings. 
*
*   !! Fields must end on a NULL field !!
*
*
*
*   Return Value : 
*   void
*/
static void remove_fields(cJSON* original, const char** fields) {
    size_t count = 0;

    // fields must be null-terminated array of field names
    while(fields[count] != NULL) {
        cJSON_DeleteItemFromObject(original, fields[count]);
        ++count;
    }
}




/*
*   Description : 
*   merge_fields() merge inputed cJSON object of new fields to the
*   inputed cJSON original object.
*
*
*
*   Return Value :
*   void
*/
static void merge_fields(cJSON* original, cJSON* changes) {
    cJSON* field = NULL;
    cJSON_ArrayForEach(field, changes) {
        const char* key = field->string;
        cJSON* copy = cJSON_Duplicate(field, 1);
        cJSON_ReplaceItemInObject(original, key, copy);
    }
}


/*
*   Description :
*   create() creates a new document in a collection with the inputed
*   data. It also adds an id which is a uuid and a _rev (revolution)
*   which is also an uuid and is used to avoid update collisions. It 
*   does also add the document id to the ids.indx file
*
*
*
*   Return Value : 
*   returns 0 if success and -1 for failure to write and -2 for failure
*   to parse data to an cJSON object
*/
int create(const char* collection,char* data) {
    char path[MAX_PATH_LEN];
    snprintf(path, sizeof(path), "%s/%s/%s.bson", BASE_PATH, collection, get_uuid());

    cJSON* root = cJSON_Parse(data);
    if(!root) {
        return -2;
    }

    char* new_rev = get_uuid();
    cJSON_AddItemToObject(root, "_rev", cJSON_CreateString(new_rev));

    char* id = get_uuid();
    cJSON_AddItemToObject(root, "_id", cJSON_CreateString(id));

    char* updated_json = cJSON_PrintUnformatted(root);
    
    int status = io_file_write(updated_json, strlen(updated_json), path);

    cJSON_Delete(root);
    free(updated_json);
    free(new_rev);

    add_to_index_file(collection, "ids", id);

    return status;
}



/*
*   Description :
*   update() updates a current document by merging in new fields and removing fields. It 
*   also matches _rev and updates it after the updates are done.
*
*
*
*   Return Value : 
*   returns 0 for success
*   -1 for failure to write to file
*   -2 for failure to read file
*   -3 for failure to parse data
*   -4 for failure to match _rev (merge error)
*/
int update(const char* collection, const char* id, const char* expected_rev, const char* changed_fields_json, const char** removed_fields) {
    char path[MAX_PATH_LEN];
    snprintf(path, sizeof(path), "%s/%s/%s.bson", BASE_PATH, collection, id);

    File doc = io_file_read(path);
    if (!doc.is_valid) {
        return -2;
    }

    cJSON* root = cJSON_Parse(doc.data);
    if (!root) {
        return -3;
    }

    cJSON* rev_item = cJSON_GetObjectItem(root, "_rev");
    if (!rev_item || strcmp(rev_item->valuestring, expected_rev) != 0) {
        cJSON_Delete(root);
        return -4;
    }

    cJSON* changes = cJSON_Parse(changed_fields_json);
    if (changes) {
        merge_fields(root, changes);
        cJSON_Delete(changes);
    }

    remove_fields(root, removed_fields);

    char* new_rev = get_uuid();
    cJSON_ReplaceItemInObject(root, "_rev", cJSON_CreateString(new_rev));

    char* updated_json = cJSON_PrintUnformatted(root);
    

    int result = io_file_write(updated_json, strlen(updated_json), path);

    cJSON_Delete(root);
    free(updated_json);
    free(new_rev);

    return result;
}


/*
*   Description :
*   delete_document() deletes the document file and removes the id from index
*   file (ids.indx)
*
*
*
*   Return Value :
*   returns 0 for success and -1 for failure to remove file
*/
int delete_document(const char* collection,const char* id){
    char path[MAX_PATH_LEN];
    snprintf(path, sizeof(path), "%s/%s/%s.bson", BASE_PATH, collection, id);
    
    //remove file
    if(remove(path) != 0) {
        return -1;
    }

    remove_from_index_file(collection, "ids", id);

    return 0;
}


/*
*   Description : 
*   grep_by_id() finds a document with id in side a collection
*
*
*   Return Value :
*   function returns the data inside a doc file
*/
char* grep_by_id(const char* collection, const char* id){
    char path[MAX_PATH_LEN];
    snprintf(path, sizeof(path), "%s/%s/%s.bson", BASE_PATH, collection, id);
    
    File file = io_file_read(path);

    if(!file.is_valid || file.data == NULL) {
        return NULL;
    }

    
    return file.data;
}


/*
*   Description :
*   grep() uses the ids.indx file to find all documents inside the collection
*   it also includes a limit parameter to limit the amount of documents read
*
*
*
*   Return Value :
*   returns an array of document data
*/
char** grep(const char* collection, int limit){
    return grep_by_index(collection, "ids", limit);
}


/*
*   Description : 
*   grep_by_index() is a function that greps documents based 
*   on a pre created index file with document ids sorted in most relevant to least
*   for example { age : 1 } this index would have all the document ids where age exists
*   and sort them from smallest to largest (number in this case). 
*
*
*
*   Return Value : 
*   returns an array of JSON data for all the ids in the index or NULL if it fails to read file
*/
char** grep_by_index(const char* collection, const char* index_name, int limit) {
    char id_index_path[MAX_PATH_LEN];
    snprintf(id_index_path, sizeof(id_index_path), "%s/%s/ids.indx", BASE_PATH, collection);
    
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
        snprintf(id_index_path, sizeof(id_index_path), "%s/%s/%s.bson", BASE_PATH, collection, line);
    
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

