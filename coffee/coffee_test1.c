#include "contiki.h"
#include "cfs/cfs.h"
#include <stdio.h>
#include <string.h>

PROCESS(coffee_test_process, "Testing Coffee file system");
AUTOSTART_PROCESSES(&coffee_test_process);

PROCESS_THREAD(coffee_test_process, ev, data){
  int n, i = 0, j, fd;
  char msg[100];
  char file1[] = "file001.txt";
  char file2[] = "file002.txt";
  char filen[50];
  
  PROCESS_BEGIN();

  /* Write a file */
  fd = cfs_open(file1, CFS_WRITE);
  if (fd != -1) {
    printf("Writing file %s\n", file1);
    for (i=0, n=0; i<16; i++){
      sprintf(msg, "File1   #%06d\n", i);
      n += cfs_write(fd, msg, strlen(msg));
    }
    cfs_close(fd);
    printf("Wrote %d bytes\n", n);
  } else {
    printf("Could not write %s\n", file1);
  }

  /* Append to file */
  fd = cfs_open(file1, CFS_WRITE | CFS_APPEND);
  if (fd != -1) {
    printf("Appending to %s\n", file1);
    for (i=16, n=0; i<=40; i++){
      sprintf(msg, "File1  A#%06d\n", i);
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
    for (i=0, n=0; i<=16; i++) {
      sprintf(msg, "File2  A#%06d\n", i);
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
    for (i=16, n=0; i<=24; i++) {
      sprintf(msg, "File2  B#%06d\n", i);
      n+= cfs_write(fd, msg, strlen(msg));
    }
    cfs_close(fd);
    printf("Wrote %d bytes\n", n);
  } else {
    printf("Could not overwrite %s\n", file2);
  }

  /* Create several files, then delete half, and write more, and delete half again */
  for (j=3; j<13; j++){
    sprintf(filen, "file%03d.txt", j);
    fd = cfs_open(filen, CFS_WRITE);
    if (fd != -1) {
      printf("Creating %s\n", filen);
      for (i=0, n=0; i<=32; i++) {
	sprintf(msg, "File%03d #%06d\n", j, i);
	n+= cfs_write(fd, msg, strlen(msg));
      }
      cfs_close(fd);
      printf("Wrote %d bytes\n", n);
    } else {
      printf("Could not write %s\n", filen);
    }
  }
  for (j=3; j<13; j+=2){
    sprintf(filen, "file%03d.txt", j);
    printf("Delete file; %s\n", filen);
    cfs_remove(filen);
  }
  for (j=13; j<23; j++){
    sprintf(filen, "file%03d.txt", j);
    fd = cfs_open(filen, CFS_WRITE);
    if (fd != -1) {
      printf("Creating %s\n", filen);
      for (i=0, n=0; i<=16; i++) {
	sprintf(msg, "File%03d #%06d\n", j, i);
	n+= cfs_write(fd, msg, strlen(msg));
      }
      cfs_close(fd);
      printf("Wrote %d bytes\n", n);
    } else {
      printf("Could not write %s\n", filen);
    }
  }
  for (j=13; j<23; j+=2){
    sprintf(filen, "file%03d.txt", j);
    printf("Delete file; %s\n", filen);
    cfs_remove(filen);
  }
  
  PROCESS_END();
}
