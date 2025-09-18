#include "crud.h"

#include "../../include/mpack/mpack.h"
#include "../utils/utils.h"
#include "../io/io.h"
#include "../indexing/indexing.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
*   Description :
*   create() creates a new document in a collection with the inputed
*   msgpack data. It also adds a _rev (revolution) which is a uuid.
*   This is used to avoid update collisions. It does also add the 
*   document id to the ids.indx doc.
*
*
*
*   Return Value : 
*   returns 0 if success and -1 for failure to write and -2 for failure
*   to parse data to an cJSON object
*/

char* create(const char* collection, const char* data) {
    char path[MAX_PATH_LEN];
    const char* home = getenv("HOME");

    char* id = get_uuid();

    snprintf(path, sizeof(path), "%s/%s/%s/%s.pson",home, BASE_FOLDER, collection, id);

    mpack_tree_t original_tree;
    mpack_tree_init_data(&original_tree, data, strlen((char*)data));
    mpack_tree_parse(&original_tree);
    mpack_node_t root = mpack_tree_root(&original_tree);

    mpack_writer_t writer;
    char* buffer = NULL;
    size_t buffer_size = 0;
    mpack_writer_init_growable(&writer, &buffer, &buffer_size);

    mpack_start_map(&writer, mpack_node_map_count(root)); 

    char* new_rev = get_uuid();
    mpack_write_cstr(&writer, "_rev");
    mpack_write_cstr(&writer, new_rev);

    mpack_finish_map(&writer);

    if (mpack_writer_destroy(&writer) != mpack_ok) {
        mpack_tree_destroy(&original_tree);
        return NULL;
    }

    const int status = io_file_write(buffer, buffer_size, path);
    
    if(status == -1) {
        return NULL;
    }


    return id;
}



/*
*   Description :
*   update() updates a current document by merging in new fields and removing fields. It 
*   also matches _rev and updates it after the updates are done.
*
*
*
*   Return Value :
*   The updated msgpack data
*/
char* update(const char* collection, const char* id, const char* expected_rev, const char* updated_data, const char** removed_fields) {
    const char* home = getenv("HOME");

    char path[MAX_PATH_LEN];
    snprintf(path, sizeof(path), "%s/%s/%s/%s.pson",home, BASE_FOLDER, collection, id);

    File doc = io_file_read(path);
    if (!doc.is_valid) {
        return NULL;
    }

    // Original document
    mpack_tree_t original_tree;
    mpack_tree_init_data(&original_tree, doc.data, doc.len);
    mpack_tree_parse(&original_tree);
    mpack_node_t root = mpack_tree_root(&original_tree);

    mpack_node_t rev_node = mpack_node_map_cstr_optional(root, "_rev");

    if (mpack_node_is_missing(rev_node)) {
        mpack_tree_destroy(&original_tree);
        return NULL;
    }

    const char* current_rev = mpack_node_str(rev_node);

    if (strcmp(current_rev, expected_rev) != 0) {
        mpack_tree_destroy(&original_tree);
        return NULL;
    }
    

    mpack_tree_t patch_tree;
    mpack_tree_init_data(&patch_tree, updated_data, doc.len); // <- maybe should be calculated idk. 
    mpack_tree_parse(&patch_tree);
    mpack_node_t patch = mpack_tree_root(&patch_tree);

    mpack_writer_t writer;
    char* buffer = NULL;
    size_t buffer_size = 0;
    mpack_writer_init_growable(&writer, &buffer, &buffer_size);

    mpack_start_map(&writer, mpack_node_map_count(root)); 


    for (size_t i = 0; i < mpack_node_map_count(root); i++) {
        const char* key = mpack_node_str(mpack_node_map_key_at(root, i));

        if (is_included(key, removed_fields)) continue;

        mpack_write_cstr(&writer, key);

        if (mpack_node_map_contains_cstr(patch, key)) {
            copy_node(&writer, mpack_node_map_cstr(patch, key));
        } else {
            copy_node(&writer, mpack_node_map_value_at(root, i));
        }
    }


    for (size_t i = 0; i < mpack_node_map_count(patch); i++) {
        const char* key = mpack_node_str(mpack_node_map_key_at(patch, i));
        if (!mpack_node_map_contains_cstr(root, key)) {
            mpack_write_cstr(&writer, key);
            copy_node(&writer, mpack_node_map_cstr(patch, key));
        }
    }

    char* new_rev = get_uuid();
    mpack_write_cstr(&writer, "_rev");
    mpack_write_cstr(&writer, new_rev);

    mpack_finish_map(&writer);

    if (mpack_writer_destroy(&writer) != mpack_ok) {
        mpack_tree_destroy(&original_tree);
        mpack_tree_destroy(&patch_tree);
        return NULL;
    }

    int result = io_file_write(buffer, buffer_size, path);


    return buffer;
}


/*
*   Description :
*   delete_document() deletes the document doc and removes the id from index
*   doc (ids.indx)
*
*
*
*   Return Value :
*   returns 0 for success and -1 for failure to remove doc
*/
int delete_document(const char* collection,const char* id){
    char path[MAX_PATH_LEN];
    const char* home = getenv("HOME");
    
    snprintf(path, sizeof(path), "%s/%s/%s/%s.pson", home, BASE_FOLDER, collection, id);
    
    //remove doc
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
*   function returns the data inside a doc with _id added
*/
char* grep_by_id(const char* collection, const char* id){
    char path[MAX_PATH_LEN];
    const char* home = getenv("HOME");
    
    snprintf(path, sizeof(path), "%s/%s/%s/%s.pson", home, BASE_FOLDER, collection, id);
    
    File doc = io_file_read(path);

    if(!doc.is_valid || doc.data == NULL) {
        return NULL;
    }

    //Adds id to outputted document
    mpack_tree_t original_tree;
    mpack_tree_init_data(&original_tree, doc.data, doc.len);
    mpack_tree_parse(&original_tree);
    mpack_node_t root = mpack_tree_root(&original_tree);

    mpack_writer_t writer;
    char* buffer = NULL;
    size_t buffer_size = 0;
    mpack_writer_init_growable(&writer, &buffer, &buffer_size);

    mpack_start_map(&writer, mpack_node_map_count(root)); 
   
    mpack_write_cstr(&writer, "_id");
    mpack_write_cstr(&writer, id);

    mpack_finish_map(&writer);

    if (mpack_writer_destroy(&writer) != mpack_ok) {
        mpack_tree_destroy(&original_tree);
        return NULL;
    }
    
    return buffer;
}


/*
*   Description :
*   grep() uses the ids.indx doc to find all documents inside the collection
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
