#ifndef HEADER_FILE
#define HEADER_FILE

struct args
{
  char* dir_path;
  char* ip_addr;
  char* buffer;
};


void update_webserver(char* buf){
  printf("000");
  FILE* fp = fopen("/var/www/html/index.htmal","w+");
  fprintf(fp, "<html><body> %s </body></html>",buf);
  fclose(fp);
}

#endif

