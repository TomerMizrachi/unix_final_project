/*This is the sample program to notify us for the file creation and file deletion takes place in “/tmp” directory*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <linux/inotify.h>
#include <time.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

void* inotify( void *arg )
{
  int length, i = 0;
  int fd;
  int wd;
  char buffer[EVENT_BUF_LEN];
  char* dir_path = (char *) arg;
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  char months[] = {"Jan", "Feb", "Mar", "Apr", "May",
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

  length = read( fd, buffer, EVENT_BUF_LEN ); 

  /*checking for error*/
  if ( length < 0 ) {
    perror( "read" );
  }  

  /*actually read return the list of change events happens. Here, read the change event one by one and process it accordingly.*/
  while ( i < length ) {     
    struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];     
    if ( event->len ) {
        if ( event->mask & IN_ACCESS ) {
            bzero( temp_buf, 256 );
            fprintf(temp_buf, "FILE ACCESSED: %s\nACCESS: READ\nTIME OF ACCESS: %02d %s %d: %02d:%02d\n", event->name, tm.tm_mday, months[tm.tm_mon], tm.tm_year + 1900,  tm.tm_hour, tm.tm_min);
          }
         else if ( event->mask & IN_MODIFY ) {
            bzero( temp_buf, 256 );
            fprintf(temp_buf, "FILE ACCESSED: %s\nACCESS: WRITE\nTIME OF ACCESS: %02d %s %d: %02d:%02d\n", event->name, tm.tm_mday, months[tm.tm_mon], tm.tm_year + 1900,  tm.tm_hour, tm.tm_min);
          }
        } 
      i += EVENT_SIZE + event->len;
  }
  /*removing the “/tmp” directory from the watch list.*/
   inotify_rm_watch( fd, wd );

  /*closing the INOTIFY instance*/
   close( fd );

}