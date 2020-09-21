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
#include "declerations.h"

int main(int argc, char *argv[])
{
  char opt;
  char dir_path[100];
  char target_ip[100];
  char* global_buffer;

  global_buffer = (char*)malloc(10*sizeof(char));
  strcat(global_buffer,"the list:\n");

  pthread_t tid_telnet;
  pthread_t tid_inotify;

  struct args *arguments = (struct args *)malloc(sizeof(struct args));
  arguments->buffer = global_buffer;
  
  if (pthread_create(&tid_telnet, NULL, telnet, NULL))
		return 1;
  
  while ((opt = getopt(argc, argv, "d:i:")) != -1)
  {
    switch (opt)
    {
    case 'd':
      memset(dir_path, '\0', sizeof(dir_path));
      strcat(dir_path, optarg);
      arguments->dir_path = dir_path;
     
      break;
    case 'i':
      memset(target_ip, '\0', sizeof(target_ip));
      strcat(target_ip, optarg);
      arguments->ip_addr = target_ip;
      // if (pthread_create(&tid_udp, NULL, udp, (void*)arguments))
		  //   return 1;
      break;
    default:
      break;
    }

  }
  if (pthread_create(&tid_inotify, NULL, inotify, (void*)arguments))
	 	return 1;
  pthread_join(tid_telnet, NULL);
  pthread_join(tid_inotify, NULL);
  free(arguments);
  
  return 0;  
}