#include "contiki.h"
#include "cfs/cfs.h"
#include "lib/random.h"
#include <stdio.h>
#include <string.h>

#ifdef R1
#define RATE1 CLOCK_SECOND * R1
#else
#define RATE1 CLOCK_SECOND * 3
#endif

#ifdef R2
#define RATE2 CLOCK_SECOND * R2
#else 
#define RATE2 CLOCK_SECOND * 17
#endif

#ifdef R3
#define RATE3 CLOCK_SECOND * R3
#else 
#define RATE3 CLOCK_SECOND * 100
#endif

// #define RAND1 2934
// #define RAND2 1234
// #define RAND3 9999
#define CANARY1 {0xe7, 0x1d, 0xe5, 0xce}
#define CANARY2 {0xca, 0xfe, 0xba, 0xbe}
#define CANARY3 {0xde, 0xad, 0xbe, 0xef}

PROCESS(write_process1, "Write to flash memory");
PROCESS(write_process2, "Write to flash memory");
PROCESS(write_process3, "Write to flash memory");
AUTOSTART_PROCESSES(&write_process1, &write_process2, &write_process3);

PROCESS_THREAD(write_process1, ev, data){
  int fd;
  const char file[16] = "file0001\0";
  char msg[16];
  char canary[4] = CANARY1;
  //unsigned short seed = RAND1, tim=RATE1;
  unsigned long tim=RATE1;
  //cfs_offset_t offs;
  static struct etimer et1;
  
  PROCESS_BEGIN();

  bzero(msg, 16);
  //random_init(seed);
  
  /* Test GC for file modification */
  //sprintf(file, "file0000");
  fd = cfs_open(file, CFS_WRITE);
  
  memcpy(msg, canary, 4);
  cfs_write(fd, msg, strlen(msg)+1);
  cfs_close(fd);

  etimer_set(&et1, tim);
  while (1) {
    PROCESS_YIELD_UNTIL(etimer_expired(&et1));
    etimer_reset(&et1);

    fd = cfs_open(file, CFS_WRITE);
    memcpy(msg, canary, 4);
    
    cfs_write(fd, msg, strlen(msg)+1);
    cfs_close(fd);
  }
  
  PROCESS_END();
}

PROCESS_THREAD(write_process2, ev, data){
  int fd;
  const char file[16] = "file0002\0";
  char msg[16];
  char canary[4] = CANARY2;
  //unsigned short seed = RAND2, tim=RATE2;
  unsigned long tim=RATE2;
  //cfs_offset_t offs;
  static struct etimer et2;
  
  PROCESS_BEGIN();

  bzero(msg, 16);
  //random_init(seed);
  
  fd = cfs_open(file, CFS_WRITE);
  
  memcpy(msg, canary, 4);
  cfs_write(fd, msg, strlen(msg)+1);
  cfs_close(fd);

  etimer_set(&et2, tim);
  while (1) {
    PROCESS_YIELD_UNTIL(etimer_expired(&et2));
    etimer_reset(&et2);
    
    fd = cfs_open(file, CFS_WRITE);
    memcpy(msg, canary, 4);
    
    cfs_write(fd, msg, strlen(msg)+1);
    cfs_close(fd);
  }
  
  PROCESS_END();
}

PROCESS_THREAD(write_process3, ev, data){
  int fd;
  const char file[16] = "file0003\0";
  char msg[16];
  char canary[4] = CANARY3;
  //unsigned short seed = RAND3, tim=RATE3;
  unsigned long tim=RATE3;
  //cfs_offset_t offs;
  static struct etimer et3;
  
  PROCESS_BEGIN();

  bzero(msg, 16);
  //random_init(seed);
  
  fd = cfs_open(file, CFS_WRITE);
  
  memcpy(msg, canary, 4);
  cfs_write(fd, msg, strlen(msg)+1);
  cfs_close(fd);

  etimer_set(&et3, tim);
  while (1) {
    PROCESS_YIELD_UNTIL(etimer_expired(&et3));
    etimer_reset(&et3);
    
    fd = cfs_open(file, CFS_WRITE);
    memcpy(msg, canary, 4);
    
    cfs_write(fd, msg, strlen(msg)+1);
    cfs_close(fd);
  }
  
  PROCESS_END();
}
