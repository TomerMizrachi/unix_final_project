#include <stdio.h>
#include <stdlib.h>
int global_array[100] = [argc +100];

void CopyToGlobal (int* temp_buffer, global_array){
    if( temp_buffer != 0)
        int *bufflen = temp_buffer;
    global_array = realloc (global_array, *bufflen);
}
free (global_array);
