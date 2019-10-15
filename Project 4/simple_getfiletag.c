#include "types.h"
#include "user.h"

#define O_RDONLY 0x000
#define O_WRONLY 0x001
#define O_RDWR 0x002
#define O_CREATE 0x200

int
main(int argc, char *argv[])
{
 int fd = open("ls", O_RDONLY);

 char* key = "type";
 char buffer[18];
 int res = getFileTag(fd, key, buffer, 18);
 if (res == -1) {
   printf(1, "tag doesn't exist\n");
 }
 else if(res < 18){
   printf(1, "%s: %s\n", key, buffer);  // prints "type: utility" (assuming tagFile 
                                        // was previously used to set the tag value as "utility"
 } else{
   printf(1, "buffer too small. res = %d\n", res);
 }

 close(fd);

 exit();
}
