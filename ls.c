#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "stdbool.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

char *format_directory_name(char *fmtname){
  for(int i = DIRSIZ-1; i>=0; i--){
    if(fmtname[i]!=' '){
      fmtname[i+1]='/';
      break;
    }
  }
  return fmtname;
}

struct file_entry{
  char name[DIRSIZ+1];
  struct stat st;
};

struct file_entry files[100];

void swap_file_entry(struct file_entry *a, struct file_entry *b){
  struct file_entry temp = *a;
  *a = *b;
  *b = temp;
}

void bubbleSortFiles(struct file_entry files[], int n) {
  for(int i = 0; i < n - 1; i++){
      int swapped = 0;
      for(int j = 0; j < n - i - 1; j++){
        if(files[j].st.size < files[j + 1].st.size){
            swap_file_entry(&files[j], &files[j + 1]);
            swapped = 1;
        }
      }
      if(swapped == 0){ 
        break;
      }
  }
}

int count_files(){
  int file_count = 0;
  for(int i = 0; i< 100; i ++){
    if(files[i].st.size!=0){
      file_count++;
    }
  }
  return file_count;
}

void print_files_sorted(){
  for (int i = 0; i< 100; i++){
    if(files[i].st.type!=0){
      printf(1, "%s %d %d %d\n", files[i].name, files[i].st.type, files[i].st.ino, files[i].st.size);
    }
  }
}

void
ls(char *path, bool sort_size)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    printf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    printf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    printf(1, "%s %d %d %d\n", fmtname(path), st.type, st.ino, st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    int i = 0;
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf(1, "ls: cannot stat %s\n", buf);
        continue;
      }
      if(fmtname(buf)[0]!='.'){
        if(!sort_size){
          if(st.type==1){
            printf(1, "%s %d %d %d\n", format_directory_name(fmtname(buf)), st.type, st.ino, st.size);
          }else {
            printf(1, "%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
          }
        }
        else{
          char *name_to_copy;
          if(st.type == 1){
              name_to_copy = format_directory_name(fmtname(buf));
          } else {
              name_to_copy = fmtname(buf);
          }
          int j;
          for (j = 0; j< DIRSIZ && name_to_copy[j] !='\0'; j++ ){
            files[i].name[j] = name_to_copy[j];
          }
          for (int i =0; i< 100; i++){
            if(files[i].name[0]=='y' && files[i].name[0]=='o'){
              printf(1, "files[%d]: %s", i, files[i].name);
            }
          }
          files[i].name[j]='\0';
          files[i].st = st;
          i++;
        }
      }
    }
    if(sort_size){
      int n = count_files();
      // for (int i = 0; i< n ; i++){
      //   printf(1, "File[%d]: %s \tsize: %d\n", i, files[i].name, files[i].st.size);
      // }
      bubbleSortFiles(files, n);
      print_files_sorted();
    }
    break;
  }
  close(fd);
}

int main(int argc, char *argv[])
{
  int i;
  bool sort_size = false;

  for(int i = 1; i<argc; i++){
    if(argv[i][0] == '-' && argv[i][1] == 's' && argv[i][2] == '\0'){
      sort_size=true;
      break;
    }
  }

  if(argc < 2 || (argc ==2 && sort_size)){
    ls(".", sort_size);
    exit();
  }

  for(i=1; i<argc; i++){
    if(argv[i][0] == '-' && argv[i][1] == 's' && argv[i][2] == '\0'){
      continue;
    }
    ls(argv[i], sort_size);
  }
    
  exit();
}
