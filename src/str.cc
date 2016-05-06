// string assigning
#include <iostream>
#include <string>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

int ls()
{
DIR *dir;
struct dirent *ent;
if ((dir = opendir ("/home/mcarter/.sifi/yahoo")) != NULL) {
  /* print all the files and directories within directory */
  while ((ent = readdir (dir)) != NULL) {
    printf ("%s\n", ent->d_name);
  }
  closedir (dir);
} else {
  /* could not open directory */
  perror ("");
  return EXIT_FAILURE;
}
}

int main ()
{
  std::string str1, str2, str3;
  str1 = "Test string: ";   // c-string
  str2 = 'x';               // single character
  str3 = str1 + str2;       // string

  std::cout << str3  << '\n';
  ls();
  return 0;
}

