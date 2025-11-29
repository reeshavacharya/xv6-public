#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define BSIZE 512  // match the kernel block size

int
main(int argc, char *argv[])
{
  int fd, i, nblocks;
  char buf[BSIZE];

  if(argc != 3){
    printf(1, "usage: bigfile filename nblocks\n");
    exit();
  }

  nblocks = atoi(argv[2]);

  fd = open(argv[1], O_CREATE | O_WRONLY);
  if(fd < 0){
    printf(1, "bigfile: cannot open %s\n", argv[1]);
    exit();
  }

  // Fill buffer with 'A'
  memset(buf, 'A', sizeof(buf));

  for(i = 0; i < nblocks; i++){
    if(write(fd, buf, sizeof(buf)) != sizeof(buf)){
      printf(1, "bigfile: write failed at block %d\n", i);
      close(fd);
      exit();
    }
  }

  printf(1, "bigfile: wrote %d blocks to %s\n", nblocks, argv[1]);

  close(fd);
  exit();
}
