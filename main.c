#include <stdio.h>
#include <string.h>
#include "view.h"
#include "edit.h"
#include "help.h"

int main(int argc, char *argv[]) 
{
    // Check minimal arguments
    if (argc < 3) 
    {
        printf("Usage to view: %s -v <filename.mp3>\n", argv[0]);
        printf("Usage to edit: %s -e <filename.mp3> <option> \"New Data\"\n", argv[0]);
        printf("Usage to help: %s -h\n", argv[0]);
        return 1;
    }

    // 1. View Mode Check
    if (strcmp(argv[1], "-v") == 0) 
    {
        view(argv[2]); 
    } 
    // 2. Edit Mode Check
    else if (strcmp(argv[1], "-e") == 0) 
    {
        if (argc < 5) 
        {
            printf("Error: Missing parameters for edit.\n");
            printf("Format: %s -e <filename.mp3> <option> \"New Data\"\n", argv[0]);
            return 1;
        }
        // Calls edit function with: filename, tag option (like -a), new data string
        edit(argv[2], argv[3], argv[4]);
    } 
    // 3. Help Mode Check
    else if(strcmp(argv[1], "-h") == 0) 
    {
        help(); 
    } 
    else 
    {
        printf("Invalid option. Use -v to view or -e to edit or -h to help.\n");
    }

    return 0;
}