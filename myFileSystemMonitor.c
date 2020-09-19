#include <execinfo.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <pthread.h> // and link with -lpthread
#include <libcli.h> // and link with -lcli.
#include "telnet_thread.c"
#include "inotify_thread.c"


#define BT_BUF_SIZE 100

void b_t_func(void)
{
	int j, nptrs;
	void *buffer[BT_BUF_SIZE];
	char **strings;

	nptrs = backtrace(buffer, BT_BUF_SIZE);
	printf("backtrace() returned %d addresses\n", nptrs);

	/* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
	   would produce similar output to the following: */

	strings = backtrace_symbols(buffer, nptrs);
	if (strings == NULL) {
		perror("backtrace_symbols");
		exit(EXIT_FAILURE);
	}

	for (j = 0; j < nptrs; j++)
		printf("%s\n", strings[j]);

	free(strings);
}

int main(int argc, char *argv[])
{
  char opt;
  char dir_path[100];
  pthread_t tid_telnet;
  pthread_t tid_inotify;
  pthread_t tid_webserver;

  if (pthread_create(&tid_telnet, NULL, telnet, NULL))
		return 1;

  while ((opt = getopt(argc, argv, "d:i:")) != -1)
  {
    switch (opt)
    {
    case 'd':
      memset(dir_path, '\0', sizeof(dir_path));
      strcat(dir_path, optarg);
      if (pthread_create(&tid_telnet, NULL, inotify, (void*)dir_path))
	    	return 1;
      break;
    case 'i':

      break;
    default:
      break;
    }
      
  }
  pthread_join(tid_telnet, NULL);
  pthread_join(tid_inotify, NULL);
  return 0;  
}