#include "io.h"
#include <stdio.h>
#include <string.h>

#define IO_READ_CHUNK_SIZE 2097152
#define IO_READ_ERROR_GENERAL "Error reading file: %s. errno: %d\n"
#define IO_READ_ERROR_MEMORY "Not enough free memory to read file: %s\n"


File io_file_read(const char *path) {
    File file = { .is_valid = false };

    FILE *fp = fopen(path, "rb");
    
    if(!fp || ferror(fp)) {
        return file;
    }

    char *data = NULL;
    char *tmp;
    size_t used = 0;
    size_t size = 0;
    size_t n;

    while(true) {

        //check if the buffer size has enough space
        if(used + IO_READ_CHUNK_SIZE + 1 > size) {
            size = used + IO_READ_CHUNK_SIZE + 1; //increase buffer size
            
            //check if file it too big
            if(size <= used) {
                free(data);
            }

            tmp = realloc(data, size); //reallocate for another chunk of data
            
            //if this fails there is no more memory on the device for the reallocation
            if(!tmp) {
                free(data);
                fclose(fp);
                return file;
            }

            data = tmp;
        }

        n = fread(data + used, 1, IO_READ_CHUNK_SIZE, fp); //get the number of bytes read
        if(n == 0) //if zero its at the end of the file and break the while loop
            break;

        used += n;
    }

    if(ferror(fp)){
        free(data);
        fclose(fp);
        return file;
    }

    tmp = realloc(data, used + 1); //add space for a null terminator to the end
    
    
    //if this fails there is no more memory on the device for the reallocation
    if(!tmp) {
        free(data);
        fclose(fp);
        return file;
    }
    
    data = tmp;
    data[used] = 0; //add null terminator

    fclose(fp);

    file.data = data;
    file.len = used;
    file.is_valid = true;

    return file;
}

int io_file_write(void *buffer, size_t size, const char* path){
    FILE *fp = fopen(path, "wb");

    if(!fp || ferror(fp)){
        return -1;
    }

    size_t chunk_written = fwrite(buffer, size, 1, fp);
    
    fclose(fp);

    if(chunk_written != 1) {
        return -1;
    }

    return 0;
}
