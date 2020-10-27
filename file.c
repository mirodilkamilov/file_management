#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEF_MODE S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH

void getFname(char fname[]);
void relase(char buf[]);
void writeHandle(int fd, char buf[], char fname[]);
void readHandle(int fd, char buf[], unsigned int buf_size, char fname[]);

int main(void)
{
    int fd, offset, menu;
    unsigned int buf_size;
    char buf[520], fname[25];

    do
    {
        menu = 0;
        printf("\n1. Read a file\n");
        printf("2. Write a file\n");
        printf("3. Change a file position\n");
        printf("0. Exit\n");
        scanf("%d", &menu);
        if (menu == 0)
        {
            printf("Exitting...\n");
            exit(1);
        }

        switch (menu)
        {
        case 1:
            getFname(fname);
            fd = open(fname, O_CREAT | O_RDONLY, DEF_MODE);
            buf_size = sizeof(buf) / sizeof(buf[0]); //* n=520 as buf is declared buf[520]
            readHandle(fd, buf, buf_size, fname);
            break;

        case 2:
            getFname(fname);
            fd = open(fname, O_CREAT | O_WRONLY | O_APPEND, DEF_MODE);
            writeHandle(fd, buf, fname);
            break;

        case 3:
            getFname(fname);
            printf("Input an offset from beginning of the file: ");
            scanf("%d", &offset);
            fd = open(fname, O_CREAT | O_RDWR, DEF_MODE); //? note there is no O_APPEND bitmask (in order to show lseek)
            int positon = lseek(fd, offset, 0);
            if (positon < 0)
                printf("Error occured while performing lseek()");

            writeHandle(fd, buf, fname);
            break;
        default:
            printf("Wrong input");
            break;
        }
    } while (menu != 0);
}

void relase(char buf[])
{
    for (size_t i = 0; i < 520; i++)
    {
        buf[i] = '\0';
    }
}
void getFname(char fname[])
{
    printf("Now input a filename: ");
    scanf("%s", fname);
    if (access(fname, F_OK) == -1)
        printf("Creating %s\n", fname);
}

void writeHandle(int fd, char buf[], char fname[])
{
    if (fd > 0)
    {
        relase(buf);
        int i = 0;
        printf("\tWrite something in to %s\n", fname);
        char ch = getchar();
        while ((ch = getchar()) != EOF)
            buf[i++] = ch;
        int fsize = i;
        buf[fsize] = '\0';

        if (write(fd, buf, fsize) < 0)
        {
            printf("Error occured while writing");
            return 1;
        }
        close(fd);
    }
    else
    {
        printf("Cannot write %s\n", fname);
        return 1;
    }
}

void readHandle(int fd, char buf[], unsigned int buf_size, char fname[])
{
    if (fd > 0)
    {
        relase(buf);
        int nbytes = read(fd, buf, buf_size);
        if (nbytes < 0)
        {
            printf("Error occured while reading");
            exit(1);
        }

        printf("\n\tContents of %s\n", fname);
        buf[nbytes] = '\0';
        puts(buf);
        close(fd);
    }
    else
    {
        printf("Cannot read %s\n", fname);
        return 1;
    }
}