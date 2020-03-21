#include "contiki.h"
#include "cfs/cfs.h"
#include <stdio.h>
#include <string.h>

PROCESS(coffee_test_process, "Testing Coffee file system");
AUTOSTART_PROCESSES(&coffee_test_process);

PROCESS_THREAD(coffee_test_process, ev, data){
  int n, i = 0, fd;
  char msg[100];
  char file1[] = "hello.txt";
  char file2[] = "yadayada.txt";
  char file3[] = "deleted.txt";
  
  PROCESS_BEGIN();

  /* Write a file */
  fd = cfs_open(file1, CFS_WRITE);
  if (fd != -1) {
    printf("Writing file %s\n", file1);
    sprintf(msg, "Hello #%d\n", i);
    n = cfs_write(fd, msg, strlen(msg));
    cfs_close(fd);
    printf("Wrote %d bytes\n", n);
  } else {
    printf("Could not write %s\n", file1);
  }

  /* Append to file */
  fd = cfs_open(file1, CFS_WRITE | CFS_APPEND);
  if (fd != -1) {
    printf("Appending to %s\n", file1);
    for (i=1, n=0; i<=10; i++){
      sprintf(msg, "Hello #%d\n", i);
      n += cfs_write(fd, msg, strlen(msg));
    }
    cfs_close(fd);
    printf("Wrote %d bytes\n", n);
  } else {
    printf("Could not append %s\n", file1);
  }

  /* Write a file, then overwrite */
  fd = cfs_open(file2, CFS_WRITE);
  if (fd != -1) {
    printf("Writing %s\n", file2);
    for (i=0, n=0; i<=10; i++) {
      sprintf(msg, "This will be overwritten in a short while: %d\n", i);
      n+= cfs_write(fd, msg, strlen(msg));
    }
    cfs_close(fd);
    printf("Wrote %d bytes\n", n);
  } else {
    printf("Could not write %s\n", file2);
  }

  fd = cfs_open(file2, CFS_WRITE);
  if (fd != -1) {
    printf("Overwriting %s\n", file2);
    for (i=0, n=0; i<=5; i++) {
      sprintf(msg, "This a new text that overwrites the previous: %d\n", i);
      n+= cfs_write(fd, msg, strlen(msg));
    }
    cfs_close(fd);
    printf("Wrote %d bytes\n", n);
  } else {
    printf("Could not overwrite %s\n", file2);
  }

  /* Create a file, then delete */
  fd = cfs_open(file3, CFS_WRITE);
  if (fd != -1) {
    printf("Creating %s\n", file2);
    for (i=0, n=0; i<=10; i++) {
      sprintf(msg, "This text will be deleted: %d", i);
      n+= cfs_write(fd, msg, strlen(msg));
    }
    cfs_close(fd);
    printf("Wrote %d bytes\n", n);
  } else {
    printf("Could not write %s\n", file3);
  }

  printf("Removing %s\n", file3);
  cfs_remove(file3);
  
}
