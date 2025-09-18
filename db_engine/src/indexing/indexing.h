#ifndef INDEXING_H
#define INDEXING_H

int add_to_index_file(const char* collection, const char* index_name, const char* id);
int remove_from_index_file(const char* collection, const char* index_name, const char* id);
char** grep_by_index(const char* collection, const char* index_name, int limit);

#endif
