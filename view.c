#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "view.h"

void view(const char *file_str) 
{
    FILE *fptr = fopen(file_str, "rb"); 
    if (fptr == NULL) 
    {
        printf("====================================================\n");
        printf("  Error: Could not open file %s\n", file_str);
        printf("====================================================\n");
        return;
    }

    // Skip the initial 10-byte main ID3 header
    fseek(fptr, 10, SEEK_SET);

    int count = 0;
    char tag[5];
    tag[4] = '\0'; // Guarantee string null termination
    printf("====================================================\n");
    printf("         --- Reading MP3 Tags ---\n");
    printf("====================================================\n");

    while (count < 6) 
    {
        // Read 4 bytes of Tag ID
        if (fread(tag, 1, 4, fptr) != 4) 
        {
            break;
        }

        // Stop if we hit padding or empty data
        if (tag[0] == '\0') 
        {
            break;
        }

        // Read 4 bytes of Size
        unsigned char size_bytes[4];
        if (fread(size_bytes, 1, 4, fptr) != 4) 
        {
            break;
        }

        // Convert Big Endian size bytes to a Little Endian integer
        unsigned int size = (size_bytes[0] << 24) | 
                            (size_bytes[1] << 16) | 
                            (size_bytes[2] << 8)  | 
                            size_bytes[3];

        // Skip 3 bytes of Flag (includes the internal \0 byte text identifier prefix)
        fseek(fptr, 3, SEEK_CUR);

        // Read size - 1 bytes of plain text Data
        unsigned int data_size = size - 1;
        char *data = (char *)malloc(data_size + 1);
        if (data == NULL) 
        {
            break;
        }

        if (fread(data, 1, data_size, fptr) != data_size) 
        {
            free(data);
            break;
        }
        data[data_size] = '\0'; // Ensure string safe ending

        // Print matching tags based on user whiteboard list
        if (strcmp(tag, "TIT2") == 0) 
        {
            printf("Title   : %s\n", data);
            count++;
        } 
        else if (strcmp(tag, "TPE1") == 0) 
        {
            printf("Artist  : %s\n", data);
            count++;
        } 
        else if (strcmp(tag, "TYER") == 0) 
        {
            printf("Year    : %s\n", data);
            count++;
        } 
        else if (strcmp(tag, "TALB") == 0) 
        {
            printf("Album   : %s\n", data);
            count++;
        }
        else if (strcmp(tag, "TCON") == 0) 
        {
            printf("Genre   : %s\n", data);
            count++;
        } 
        else if (strcmp(tag, "COMM") == 0) 
        {
            printf("Comment : %s\n", data);
            count++;
        } 
        else 
        {
            // Skips unrecognized tag strings automatically
            printf("====================================================\n");
            printf("            No tags were matched\n");
            printf("====================================================\n"); 
        }

        free(data);
    }

    fclose(fptr);
}