/*This is the sample program to notify us for the file creation and file deletion takes place in “/tmp” directory*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <time.h>
#include "udp_socket.c"
#include "declerations.h"


#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

void update_webserver(char* buf){
  static int flag = 0;
  if(flag == 0){
    printf("web server are connect, connect your browser with URL: 127.0.0.1 to see inpotify deatails\n");
    flag++;
  }
  char* tmp;
  FILE* fp = fopen("/var/www/html/index.html","w+");
  if(fp == NULL){
    printf("fopen in update_webserver function failed\n");
    return;
  }
  fprintf(fp,"<html><body>");
  tmp = strtok(buf,"\n");
  while (tmp != NULL)
  {
    fprintf(fp,"<li> %s </li>",tmp);
    tmp = strtok(NULL, "\n");
  }
  fprintf(fp, "</body></html>");

  fclose(fp);
}

void* inotify( void *arg )
{
  size_t size;
  int length, n, i = 0;
  int fd;
  int wd;
  char buffer[EVENT_BUF_LEN];
  char* dir_path = ((struct args*)arg)->dir_path;
  char* globalbuff = ((struct args*)arg)->buffer;
  char* tmp;

  pthread_t tid_udp;

  time_t t;
  struct tm tm;
  char months[12][4] = {"Jan", "Feb", "Mar", "Apr", "May",
                       "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  char temp_buf[256];

  /*creating the INOTIFY instance*/
  fd = inotify_init();

  /*checking for error*/
  if ( fd < 0 ) {
    perror( "inotify_init" );
  }

  /*adding the “/tmp” directory into watch list. Here, the suggestion is to validate the existence of the directory before adding into monitoring list.*/
  wd = inotify_add_watch( fd, dir_path , IN_ACCESS | IN_MODIFY );

  /*read to determine the event change happens on path.dir. Actually this read blocks until the change event occurs*/ 
  while (1){
    bzero(buffer, sizeof(buffer));
    length = read( fd, buffer, EVENT_BUF_LEN ); 

    /*checking for error*/
    if ( length < 0 ) {
      perror( "read" );
    }  
    t = time(NULL);
    tm = *localtime(&t); 
    /*actually read return the list of change events happens. Here, read the change event one by one and process it accordingly.*/
    while ( i < length ) {     
      struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];  
       
      if ( event->len ) {
        if ( event->mask & IN_ACCESS ) {
          bzero( temp_buf, 256 );
          snprintf(temp_buf, sizeof(temp_buf), "\nFILE ACCESSED: %s\nACCESS: READ\nTIME OF ACCESS: %02d %s %d: %02d:%02d:%02d\n", event->name, tm.tm_mday, months[tm.tm_mon], tm.tm_year + 1900,  tm.tm_hour, tm.tm_min, tm.tm_sec);
        } else if ( event->mask & IN_MODIFY ) {
          bzero( temp_buf, 256 );
          snprintf(temp_buf, sizeof(temp_buf), "\nFILE ACCESSED: %s\nACCESS: WRITE\nTIME OF ACCESS: %02d %s %d: %02d:%02d:%02d\n", event->name, tm.tm_mday, months[tm.tm_mon], tm.tm_year + 1900,  tm.tm_hour, tm.tm_min, tm.tm_sec);
        }  
      }
      size = strlen(globalbuff) + strlen(temp_buf);

      globalbuff = (char *)realloc(globalbuff, size);
        
      strcat(globalbuff,temp_buf);
  
      i += EVENT_SIZE + event->len;
    }
    i = 0;
    tmp = (char *)malloc(sizeof(char)*(strlen(globalbuff)+1));
    if(tmp == NULL){
      printf("in inotify thred tmp malloc failed\n");
    }
    strcpy(tmp,globalbuff);

    ((struct args*)arg)->buffer = (char*)malloc(sizeof(char)*strlen(globalbuff)+1);
    if(((struct args*)arg)->buffer == NULL){
      printf("in inotify thred arg->buffer malloc failed\n");
    }
    strcpy(((struct args*)arg)->buffer,globalbuff);

    update_webserver(tmp);
    free(tmp);

    if((n = send(socketfd, ((struct args*)arg)->buffer, strlen(((struct args*)arg)->buffer), 0)) < 0){
        printf("UDP sending ERROR, please initilize netcat server, command: netcat -l -u -p 10000 \n");
    }
   
    free(((struct args*)arg)->buffer);
  }

  /*removing the “/tmp” directory from the watch list.*/
  inotify_rm_watch( fd, wd );


  /*closing the INOTIFY instance*/
  close( fd );

}