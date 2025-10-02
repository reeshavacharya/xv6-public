// Simple grep.  Only supports ^ . * $ operators.

#include "types.h"
#include "stat.h"
#include "user.h"

int flag_i = 0;
int flag_n = 0;
int flag_v = 0;

char buf[1024];
int match(char*, char*);

char
lower(char c) {
  if('A' <= c && c <= 'Z')
    return c - 'A' + 'a';
  return c;
}

void itoa(int n, char *s) {
  char buf[16];
  int i = 0, j = 0;
  if(n == 0){
    s[0] = '0'; s[1] = '\0';
    return;
  }
  while(n > 0){
    buf[i++] = (n % 10) + '0';
    n /= 10;
  }
  while(i > 0) s[j++] = buf[--i];
  s[j] = '\0';
}

void
grep(char *pattern, int fd)
{
  int n, m, lineno = 0;
  char *p, *q;
  char line[1024];

  m = 0;
  while((n = read(fd, buf+m, sizeof(buf)-m-1)) > 0){
    m += n;
    buf[m] = '\0';
    p = buf;
    while((q = strchr(p, '\n')) != 0){
      *q = 0;
      lineno++;

      // Copy line into buffer for case-insensitive comparison
      strcpy(line, p);

      int matched;
      if(flag_i){
        // Lowercase pattern + line
        char pat[128], lbuf[1024];
        int i;
        for(i=0; pattern[i]; i++) pat[i] = lower(pattern[i]);
        pat[i] = 0;
        for(i=0; line[i]; i++) lbuf[i] = lower(line[i]);
        lbuf[i] = 0;
        matched = match(pat, lbuf);
      } else {
        matched = match(pattern, line);
      }

      if(flag_v) matched = !matched;

      if(matched){
        if(flag_n){
          char numbuf[16];
          itoa(lineno, numbuf);
          write(1, numbuf, strlen(numbuf));
          write(1, ":", 1);
        }
        *q = '\n';
        write(1, p, q+1 - p);
      }
      p = q+1;
    }
    if(p == buf)
      m = 0;
    if(m > 0){
      m -= p - buf;
      memmove(buf, p, m);
    }
  }
}

int
main(int argc, char *argv[])
{
  int fd, i, argi;
  char *pattern;

  if(argc <= 1){
    printf(2, "usage: grep [-inv] pattern [file ...]\n");
    exit();
  }

  argi = 1;
  while(argi < argc && argv[argi][0] == '-'){
    char *opt = argv[argi];
    for(int j = 1; opt[j]; j++){
      if(opt[j] == 'i') flag_i = 1;
      else if(opt[j] == 'n') flag_n = 1;
      else if(opt[j] == 'v') flag_v = 1;
    }
    argi++;
  }

  if(argi >= argc){
    printf(2, "usage: grep [-inv] pattern [file ...]\n");
    exit();
  }

  pattern = argv[argi++];
  if(argi >= argc){
    grep(pattern, 0); 
    exit();
  }

  for(i = argi; i < argc; i++){
    if((fd = open(argv[i], 0)) < 0){
      printf(1, "grep: cannot open %s\n", argv[i]);
      exit();
    }
    grep(pattern, fd);
    close(fd);
  }
  exit();
}

// Regexp matcher from Kernighan & Pike,
// The Practice of Programming, Chapter 9.

int matchhere(char*, char*);
int matchstar(int, char*, char*);

int
match(char *re, char *text)
{
  if(re[0] == '^')
    return matchhere(re+1, text);
  do{  // must look at empty string
    if(matchhere(re, text))
      return 1;
  }while(*text++ != '\0');
  return 0;
}

// matchhere: search for re at beginning of text
int matchhere(char *re, char *text)
{
  if(re[0] == '\0')
    return 1;
  if(re[1] == '*')
    return matchstar(re[0], re+2, text);
  if(re[0] == '$' && re[1] == '\0')
    return *text == '\0';
  if(*text!='\0' && (re[0]=='.' || re[0]==*text))
    return matchhere(re+1, text+1);
  return 0;
}

// matchstar: search for c*re at beginning of text
int matchstar(int c, char *re, char *text)
{
  do{  // a * matches zero or more instances
    if(matchhere(re, text))
      return 1;
  }while(*text!='\0' && (*text++==c || c=='.'));
  return 0;
}

