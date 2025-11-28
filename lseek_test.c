#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char *argv[])
{
  int fd, n;
  char buf[32];

  //
  // Test 1: Seek forward
  //
  printf(1, "Test 1: Seek forward\n");

  fd = open("t1", O_CREATE | O_RDWR);
  write(fd, "Hello", 5);

  int off = lseek(fd, 5);   // move from 5 to 10
  printf(1, "Offset after +5: %d (expected 10)\n", off);

  write(fd, "World", 5);    // should write at position 10

  close(fd);

  fd = open("t1", O_RDONLY);
  n = read(fd, buf, 15);
  buf[n] = 0;
  printf(1, "Read back: ");
  for(int i=0; i<n; i++)
    buf[i] ? printf(1, "%c", buf[i]) : printf(1, "[0]");
  printf(1, "\n");
  close(fd);
  unlink("t1");

  //
  // Test 2: Seek backward
  //
  printf(1, "\nTest 2: Seek backward\n");

  fd = open("t2", O_CREATE | O_RDWR);
  write(fd, "abcdefghij", 10);   // offsets 0–9

  off = lseek(fd, -5);       // 10 to 5
  printf(1, "Offset after -5: %d (expected 5)\n", off);

  write(fd, "***", 3);        // overwrite positions 5,6,7

  close(fd);

  fd = open("t2", O_RDONLY);
  n = read(fd, buf, 10);
  printf(1, "Read back: ");
  for(int i=0; i<n; i++)
    printf(1, "%c", buf[i]);
  printf(1, "\nExpected: abcde***ij\n");
  close(fd);
  unlink("t2");

  //
  // Test 3: Large hole
  //
  printf(1, "\nTest 3: Large hole\n");

  fd = open("t3", O_CREATE | O_RDWR);
  write(fd, "FIRST", 5);

  off = lseek(fd, 1024);   // move from 5 to 1029
  printf(1, "Offset after +1024: %d (expected 1029)\n", off);

  write(fd, "LAST", 4);
  close(fd);

  fd = open("t3", O_RDONLY);
  struct stat st;
  fstat(fd, &st);
  printf(1, "File size: %d (expected 1033)\n", st.size);
  close(fd);
  unlink("t3");

  exit();
}
