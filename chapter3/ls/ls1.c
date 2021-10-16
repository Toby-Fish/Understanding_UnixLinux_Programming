/*
 * ls1.c
 *  purpose list contents of directory ordirectories
 *  action if no args, use . else list files in args
 */
#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>

void do_ls(char[]);

int main(int argc, char* argv[])
{
    if (argc == 1)
        do_ls(".");
    else
        while (--argc) {
            printf("%s:\n", *(++argv));
            do_ls(*argv);
        }

    return 0;
}

void do_ls(char dirname[])
/*
 * list files in directory called dirname
 */
{
    DIR* dir_ptr;                   /* the directory */
    struct dirent* direntp;         /* each entry */

    if ((dir_ptr = opendir(dirname)) == NULL)
        fprintf(stderr, "ls1: cannot open %s\n", dirname);
    else
    {
        while ((direntp = readdir(dir_ptr)) != NULL)
            printf("%s\n", direntp->d_name);
        closedir(dir_ptr);
    }
}
