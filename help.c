#include <stdio.h>
#include "help.h"

void help(void)
{
    printf("\n");
    printf("====================================================\n");
    printf("                 MP3 TAG READER                     \n");
    printf("====================================================\n");

    printf("\n[ COPY / FRESH EDIT ]\n");
    printf(" ./a.out -copy <destination> <source>\n");
    printf("       Copy an MP3 file for fresh editing\n");

    printf("\n[ VIEW ]\n");
    printf(" ./a.out -v <filename>\n");
    printf("       View MP3 tag information\n");

    printf(" ./a.out -o <filename>\n");
    printf("       View original MP3 tag information\n");

    printf("\n[ EDIT ]\n");
    printf(" ./a.out -e -t <title>       Edit Title\n");
    printf(" ./a.out -e -a <artist>      Edit Artist\n");
    printf(" ./a.out -e -A <album>       Edit Album\n");
    printf(" ./a.out -e -y <year>        Edit Year\n");
    printf(" ./a.out -e -g <genre>       Edit Genre\n");
    printf(" ./a.out -e -c <comment>     Edit Comment\n");

    printf("\n[ HELP ]\n");
    printf(" ./a.out -h\n");
    printf("       Display help and usage information\n");

    printf("\n====================================================\n");
}
