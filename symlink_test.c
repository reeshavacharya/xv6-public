#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define BSIZE 512

int
main(int argc, char *argv[])
{
  int fd;
  char buf[BSIZE];

  printf(1, "symlink test start\n");

  // 1. Create a regular file "a" and write some data into it.
  fd = open("a", O_CREATE | O_RDWR);
  if(fd < 0){
    printf(1, "symlinktest: failed to create file a\n");
    exit();
  }
  write(fd, "hello via symlink\n", 19);
  close(fd);

  // 2. Create a symlink "b" that points to "a".
  if(symlink("a", "b") < 0){
    printf(1, "symlinktest: symlink(a -> b) failed\n");
    exit();
  }
  printf(1, "created symlink b -> a\n");

  // 3. Open "b" normally (should follow the link to "a" and read file content).
  fd = open("b", O_RDONLY);
  if(fd < 0){
    printf(1, "symlinktest: open(b) failed\n");
    exit();
  }
  memset(buf, 0, sizeof(buf));
  read(fd, buf, sizeof(buf));
  close(fd);
  printf(1, "read via b (following link): %s", buf);

  // 4. Open "b" with O_NOFOLLOW (should open the symlink itself).
  fd = open("b", O_RDONLY | O_NOFOLLOW);
  if(fd < 0){
    printf(1, "symlinktest: open(b, O_NOFOLLOW) failed\n");
    exit();
  }
  memset(buf, 0, sizeof(buf));
  read(fd, buf, sizeof(buf));
  close(fd);
  printf(1, "symlink content (target path) via O_NOFOLLOW: %s\n", buf);

  // 5. Chain symlinks: c -> b -> a
  if(symlink("b", "c") < 0){
    printf(1, "symlinktest: symlink(b -> c) failed\n");
    exit();
  }
  printf(1, "created symlink c -> b\n");

  // Open "c" (should follow c -> b -> a)
  fd = open("c", O_RDONLY);
  if(fd < 0){
    printf(1, "symlinktest: open(c) failed\n");
    exit();
  }
  memset(buf, 0, sizeof(buf));
  read(fd, buf, sizeof(buf));
  close(fd);
  printf(1, "read via c (following c -> b -> a): %s", buf);

  printf(1, "symlink test done\n");
  exit();
}
