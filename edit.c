#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "edit.h"
#include "help.h" 

void edit(const char *file_str, const char *option, const char *new_data) 
{
    char given_tag[5] = "";

    // Map user shortcut options to official 4-byte tags
    if (strcmp(option, "-t") == 0) strcpy(given_tag, "TIT2");
    else if (strcmp(option, "-a") == 0) strcpy(given_tag, "TPE1");
    else if (strcmp(option, "-y") == 0) strcpy(given_tag, "TYER");
    else if (strcmp(option, "-A") == 0) strcpy(given_tag, "TALB");
    else if (strcmp(option, "-g") == 0) strcpy(given_tag, "TCON");
    else if (strcmp(option, "-c") == 0) strcpy(given_tag, "COMM");
    else 
    {
        printf("====================================================\n");
        printf("      Error: Invalid tag option specified.\n");
        printf("====================================================\n");
        return;
    }

    FILE *f_src = fopen(file_str, "rb");       // Source file (sample.mp3)
    FILE *f_tmp = fopen("temp.mp3", "wb");     // Duplicate helper file
    
    if (f_src == NULL || f_tmp == NULL) 
    {
        printf("====================================================\n");
        printf("        Error: File stream opening failed.\n");
        printf("====================================================\n");
        if (f_src) 
           fclose(f_src);
        if (f_tmp) 
           fclose(f_tmp);
        return;
    }

    // Step 1: Copy main 10-byte header from original file to duplicate file
    char main_header[10];
    fread(main_header, 1, 10, f_src);
    fwrite(main_header, 1, 10, f_tmp);

    char tag[5];
    tag[4] = '\0';

    // Parse tag items
    while (1) 
    {
        if (fread(tag, 1, 4, f_src) != 4) 
        {
            break;
        }

        // Read original Big Endian Size
        unsigned char size_bytes[4];
        fread(size_bytes, 1, 4, f_src);
        unsigned int size = (size_bytes[0] << 24) | (size_bytes[1] << 16) | (size_bytes[2] << 8) | size_bytes[3];

        // Read 3 bytes of Flag
        char flag[3];
        fread(flag, 1, 3, f_src);

        // Allocate and read core data frame contents
        char *data = (char *)malloc(size - 1);
        fread(data, 1, size - 1, f_src);

        // Check if the current tag matches the target given_tag
        if (strcmp(tag, given_tag) == 0) 
        {
            // Write the matching tag id
            fwrite(tag, 1, 4, f_tmp);

            // Compute and construct new text field size value (+1 inside Big Endian space)
            unsigned int new_size = strlen(new_data) + 1;
            unsigned char new_size_bytes[4];
            new_size_bytes[0] = (new_size >> 24) & 0xFF;
            new_size_bytes[1] = (new_size >> 16) & 0xFF;
            new_size_bytes[2] = (new_size >> 8) & 0xFF;
            new_size_bytes[3] = new_size & 0xFF;
            fwrite(new_size_bytes, 1, 4, f_tmp);

            // Write matching unchanged flag properties
            fwrite(flag, 1, 3, f_tmp);

            // Write modified new data payload
            fwrite(new_data, 1, new_size - 1, f_tmp);

            free(data);
            break; // Stop parsing loop to pass remaining streams straight down
        } 
        else 
        {
            // Not a match: copy structural properties forward identically
            fwrite(tag, 1, 4, f_tmp);
            fwrite(size_bytes, 1, 4, f_tmp);
            fwrite(flag, 1, 3, f_tmp);
            fwrite(data, 1, size - 1, f_tmp);
        }
        free(data);
    }

    // Step 2: Stream copy all remaining underlying raw audio structural data block fragments
    char buffer;
    while (fread(&buffer, 1, 1, f_src) == 1) 
    {
        fwrite(&buffer, 1, 1, f_tmp);
    }

    fclose(f_src);
    fclose(f_tmp);

    // Step 3: Swap original asset files safely
    remove(file_str);
    rename("temp.mp3", file_str);
    printf("====================================================\n");
    printf("         Tag updated successfully!\n");
    printf("====================================================\n");
}