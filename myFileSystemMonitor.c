#include <execinfo.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <pthread.h> // and link with -lpthread
#include <libcli.h> // and link with -lcli.
#include <semaphore.h>
#include "declerations.h"

#define BT_BUF_SIZE 100

sem_t sem; 
int backtrace_flag = 0;
int nptrs;
void *buffer[BT_BUF_SIZE];
char **strings;

int socketfd = 0;

#include "telnet_thread.c"
#include "inotify_thread.c"

void  __attribute__ ((no_instrument_function))  __cyg_profile_func_enter (void *this_fn,
                                         void *call_site)
{
  if(backtrace_flag == 1){
    backtrace_flag =0;
   
  	nptrs = backtrace(buffer, BT_BUF_SIZE);

	  strings = backtrace_symbols(buffer, nptrs);
  	if (strings == NULL) {
  		perror("backtrace_symbols");
	  	exit(EXIT_FAILURE);
	  }
    sem_post(&sem);
   
  
  }
}

void  __attribute__ ((no_instrument_function))  __cyg_profile_func_exit (void *this_fn,
                                         void *call_site)
{
      
}

int main(int argc, char *argv[])
{
  char opt;
  char dir_path[100];
  char target_ip[100];
  char* global_buffer;
  if (sem_init(&sem, 0, 0) == -1){
    printf("sem_init failed\n");
    return 1;
  }
    
  global_buffer = (char*)malloc(10*sizeof(char));
  if(global_buffer == NULL){
    printf("global_buffer malloc failed\n");
    return 1;
  }
  strcat(global_buffer,"the inotify list:");

  pthread_t tid_telnet;
  pthread_t tid_inotify;

  struct args *arguments = (struct args *)malloc(sizeof(struct args));
  if(arguments == NULL){
    printf("arguments malloc failed\n");
    return 1;
  }
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
      socketfd = udp((void*)arguments);
      break;
    default:
      break;
    }
  }
  if (pthread_create(&tid_inotify, NULL, inotify, (void*)arguments))
	 	return 1;
  pthread_join(tid_telnet, NULL);
  pthread_join(tid_inotify, NULL);
  free(global_buffer);
  free(arguments);
  close(socketfd);
  return 0;  
}