#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(void)
{
  int fd;
  char buf[100];

  printf(1, "Testing symbolic links...\n");

  unlink("target.txt");
  unlink("link.txt");
  unlink("a");
  unlink("b");

  fd = open("target.txt", O_CREATE | O_RDWR);
  if(fd < 0){
    printf(1, "FAIL: could not create target.txt\n");
    exit();
  }

  write(fd, "hello symlink", 13);
  close(fd);

  if(symlink("target.txt", "link.txt") < 0){
    printf(1, "FAIL: symlink creation failed\n");
    exit();
  }

  fd = open("link.txt", O_RDONLY);
  if(fd < 0){
    printf(1, "FAIL: could not open symlink\n");
    exit();
  }

  memset(buf, 0, sizeof(buf));
  read(fd, buf, sizeof(buf));
  close(fd);

  if(strcmp(buf, "hello symlink") == 0)
    printf(1, "PASS: reading through symlink works\n");
  else
    printf(1, "FAIL: symlink contents wrong: %s\n", buf);

  symlink("b", "a");
  symlink("a", "b");

  fd = open("a", O_RDONLY);
  if(fd < 0)
    printf(1, "PASS: symlink cycle failed safely\n");
  else{
    printf(1, "FAIL: symlink cycle opened unexpectedly\n");
    close(fd);
  }

  printf(1, "Symbolic link tests complete.\n");
  exit();
}