#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define INPUT_MAX 500

typedef struct eList{
    char first_name[12];
    char last_name[12];
    uint16_t id;
    bool paid;
    bool checked_in;
    bool empty;
} eList;

eList *entry_list;
int position = 0;

typedef struct configFile{
    uint16_t size;
    char path[INPUT_MAX];
} configFile;

configFile settings;

bool run = true;

bool strComp(char *input, char *comp) // compares strings
{
    bool same = true;
    for (int x = 0; x < strlen(comp); x++)
    {
        if (input[x] != comp[x])
        {
            same = false;
        }
    }
    return same;
}

void load_config()
{
    char file[INPUT_MAX];
    strcpy(file, getenv("HOME"));
    strncat(file, "/.entry_list_config", 20);
    FILE *fp = fopen(file, "rb");
    if (!fp)
    {
        printf("Failed to read config file or no config file exists!\nrun 'config' to create file\n");
        return;
    }
    fread(&settings.size, 2, 1, fp);
    fread(&settings.path, 1, settings.size, fp);
    fclose(fp);
    settings.path[settings.size] = '\0';
    printf("Database Path: %s\n", settings.path);
}

void config()
{
    settings.path[0] = '\0';
    printf("        Database path: ");
    fgets(settings.path, INPUT_MAX, stdin);
    settings.size = strlen(settings.path) - 1;
    char file[INPUT_MAX];
    strcpy(file, getenv("HOME"));
    strncat(file, "/.entry_list_config", 20);
    system("rm ~/.entry_list_config");
    FILE *fp = fopen(file, "wb");
    if (!fp)
    {
        printf("        Error creating config file!\n");
        return;
    }
    fwrite(&settings.size, 2, 1, fp);
    fwrite(&settings.path, 1, settings.size, fp);
    fclose(fp);
}

void shell()
{
    char input[INPUT_MAX];
    printf("> ");
    fgets(input, INPUT_MAX, stdin);
    if (strlen(input) > 1)
    {
        char command[INPUT_MAX];
        char arg[INPUT_MAX];
        command[0] = '\0';
        arg[0] = '\0';
        char *token = strtok(input, " \n\t\r");
        strcpy(command, token);
        token = strtok(NULL, " \n\t\r");

        if (strComp(command, "config\0"))
        {
            config();
        } else if (strComp(command, "exit\0"))
        {
            run = false;
        }
    }
}

int main()
{
    printf(":Entry List:\n");
    load_config();
    while (run)
    {
        shell();
    }
    return 0;
}
