#ifndef DB_H
#define DB_H

int create(const char* collection, char* data);
int update(const char* collection, const char* id, const char* expected_rev, const char* changed_fields_json, const char** removed_fields);
int delete_document(const char* collection,const char* id);
char** grep(const char* collection, int limit);
char* grep_by_id(const char* collection, const char* id);

int add_to_index_file(const char* collection, const char* index_name, const char* id);
int remove_from_index_file(const char* collection, const char* index_name, const char* id);
char** grep_by_index(const char* collection, const char* indexId, int limit);

#endif
