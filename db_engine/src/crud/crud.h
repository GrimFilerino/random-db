#ifndef CRUD_H
#define CRUD_H


char* create(const char* collection, const char* data);
char* update(const char* collection, const char* id, const char* expected_rev, const char* updated_data, const char** removed_fields);
int delete_document(const char* collection,const char* id);
char** grep(const char* collection, int limit);
char* grep_by_id(const char* collection, const char* id);

#endif
