#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Usage check
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // Read memory input_file
    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL)
    {
        printf("Err: Cannot open file.\n");
        return 1;
    }

    // Create jpg files
    int n = 0;
    FILE *output_file = NULL;
    char *output_name = malloc(8 * sizeof(char));
    uint8_t buffer[512];
    while (fread(buffer, 1, 512, input_file) == 512)
    {
        // Check file header is 0xff 0xd8 0xff 0xex (JPEG Header)
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Close previous file if open
            if (output_file)
            {
                fclose(output_file);
            }
            // Open new file
            sprintf(output_name, "%03d.jpg", n++);
            output_file = fopen(output_name, "w+");
        }
        // Write to file if exists
        if (output_file)
        {
            fwrite(buffer, 512, 1, output_file);
        }
    }
    free(output_name);
    fclose(output_file);
    fclose(input_file);
    return 0;
}
