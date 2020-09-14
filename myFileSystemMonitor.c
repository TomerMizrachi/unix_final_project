#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <pthread.h> // and link with -lpthread
#include <libcli.h> // and link with -lcli.
#include "telnet_thread.c"



int main(int argc, char *argv[])
{
  pthread_t tid_telnet;
  if (pthread_create(&tid_telnet, NULL, telnet, NULL))
		return 1;
  
  	pthread_join(tid_telnet, NULL);

  char opt;
  
  while ((opt = getopt(argc, argv, "d:i:")) != -1)
  {
    switch (opt)
    {
    case 'd':
    
    break;
    case 'i':

    break;
    }
        
  }
  
}