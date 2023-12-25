#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

int euidaccess(const char *pathname, int mode)
{
   errno = 0;
   int allMode =  F_OK | R_OK | W_OK | X_OK;
   if (allMode & mode != mode)
   {
      printf("Mode check error, all mode %d value %d\n", allMode, mode);
      errno = EINVAL;
      return -1;
   }

   uid_t eeuid = geteuid();
   uid_t eguid = getegid();

   printf("Current eeuid: %d , eguid: %d\n", eeuid, eguid);

   if (eeuid == 0 || eguid == 0)
   {
      // Privileged effective id
      return 0;
   }

   struct stat statbuf;
   if (stat(pathname, &statbuf) == -1)
   {
      // TODO handle error more careful
      perror("Cannot check stat of file");
      return -1;
   }

   printf("User id of file: %d\n",statbuf.st_uid); 
   printf("Group id of file: %d\n",statbuf.st_gid);

   

   if (statbuf.st_uid != eeuid && statbuf.st_gid != eguid)
   {
      return -1; // No permission
   }

   printf("File st_mode: %d\n", statbuf.st_mode);

   int permissionBit =  statbuf.st_mode & __S_IFMT;
   
   printf("Permission bit: %d\n", permissionBit);

   int groupPermissionBit = (statbuf.st_mode >> 3); // Bit shift to get group permissions bit at the end

   printf("Current groupPermissionBit: %d\n", groupPermissionBit);

// Check group permission bit with mode
   if ((groupPermissionBit & mode) == mode)
   {
      return 0;
   }

// Check user permission bit with mode
   if ((groupPermissionBit >> 3) & mode == mode) {
      return 0;
   }

   return -1;
}

int main(int argc, char *argv[])
{
   char *filePath;
   int opt;
   while ((opt = getopt(argc, argv, ":f:")) != -1)
   {
      switch (opt)
      {
      case 'f':
         filePath = optarg;
         break;
      }
   }

   printf("Check effective permission of file %s \n", filePath);
   int mode = R_OK | X_OK;

   if (euidaccess(filePath, mode) == -1) {
      perror("Không có permission");
      return -1;
   }
   
   printf("Have permission %d by effective id\n", mode);
   return 0;
}
