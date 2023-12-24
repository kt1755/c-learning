# include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int euidaccess(const char *pathname, int mode) {
   uid_t eeuid = geteuid();
   uid_t eguid = getegid();

   struct stat *statbuf;
   if (stat(pathname, statbuf) == -1) {
      // TODO handle error more careful
      return -1; 
   }

   if (statbuf.st_uid != eeuid && statbuf->st_gid) {
      return -1; // No permission
   }

   if (statbuf.st_mode & F_OK) {

   }
}

int main(int argc, char const *argv[])
{
   /* code */
   return 0;
}
