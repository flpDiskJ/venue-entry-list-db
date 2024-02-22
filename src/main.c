#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

// Code by Jake Aigner Feb, 2024

#define INPUT_MAX 500
#define ENTRY_LEN 12
#define DB_SIZE 10000

struct eList{
    char first_name[ENTRY_LEN];
    char last_name[ENTRY_LEN];
    uint16_t id;
    bool paid;
    bool checked_in;
    bool empty;
};

typedef struct configFile{
    uint16_t size;
    char path[INPUT_MAX];
} configFile;

struct eList entry_list[DB_SIZE+1];

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
    printf("> Database path: ");
    fgets(settings.path, INPUT_MAX, stdin);
    settings.size = strlen(settings.path) - 1;
    char file[INPUT_MAX];
    strcpy(file, getenv("HOME"));
    strncat(file, "/.entry_list_config", 20);
    system("rm ~/.entry_list_config");
    FILE *fp = fopen(file, "wb");
    if (!fp)
    {
        printf("> Error creating config file!\n");
        return;
    }
    fwrite(&settings.size, 2, 1, fp);
    fwrite(&settings.path, 1, settings.size, fp);
    fclose(fp);
}

void erase_entry(int x) // clear entry_list[x]
{
    entry_list[x].first_name[0] = '\0';
    entry_list[x].last_name[0] = '\0';
    entry_list[x].id = 0;
    entry_list[x].paid = false;
    entry_list[x].checked_in = false;
    entry_list[x].empty = true;
}

void init()
{
    load_config();
    for (int x = 0; x < DB_SIZE; x++)
    {
        erase_entry(x);
    }
    char file[INPUT_MAX];
    file[0] = '\0';
    strcpy(file, settings.path);
    strncat(file, "entry_list_database.dtbs", 25);
    FILE *fp = fopen(file, "rb");
    if (fp)
    {
        uint16_t d_size;
        fread(&d_size, 2, 1, fp);
        fread(&entry_list, sizeof(struct eList), d_size, fp);
        fclose(fp);
    }
}

void save_db()
{
    char file[INPUT_MAX];
    file[0] = '\0';
    strcpy(file, settings.path);
    strncat(file, "entry_list_database.dtbs", 25);
    FILE *fp = fopen(file, "wb");
    uint16_t d_size = DB_SIZE;
    fwrite(&d_size, 2, 1, fp);
    fwrite(&entry_list, sizeof(struct eList), d_size, fp);
    fclose(fp);
    printf("~ Database Saved\n");
}

int find_id(uint16_t id) // get entry position from ID. Returns -1 if ID is not found
{
    int entry = -1;
    for (int x = 0; x < DB_SIZE; x++)
    {
        if (id == entry_list[x].id)
        {
            entry = entry_list[x].id;
        }
    }
    return entry;
}

void rm_all()
{
    printf("~ Are you sure? > ");
    char answer[INPUT_MAX];
    answer[0] = '\0';
    fgets(answer, INPUT_MAX, stdin);
    if (strComp(answer, "yes\0") || strComp(answer, "y\0"))
    {
        for (int x = 0; x < DB_SIZE; x++)
        {
            erase_entry(x);
        }
        printf("~ Erased entire database\n");
    }
}

void add()
{
    int entry_id = 0;
    bool found = false;
    while (entry_id < DB_SIZE && !found)
    {
        if (entry_list[entry_id].empty)
        {
            found = true;
        } else {
            entry_id++;
        }
    }
    if (found)
    {
        printf("> First Name: ");
        fgets(entry_list[entry_id].first_name, ENTRY_LEN, stdin);
        entry_list[entry_id].first_name[strlen(entry_list[entry_id].first_name)-1] = '\0';
        printf("> Last Name: ");
        fgets(entry_list[entry_id].last_name, ENTRY_LEN, stdin);
        entry_list[entry_id].last_name[strlen(entry_list[entry_id].last_name)-1] = '\0';
        uint16_t random_id = rand() % (65536 - 1000) + 1000;
        while (find_id(random_id) != -1)
        {
            random_id = rand() % (65536 - 1000) + 1000;
        }
        entry_list[entry_id].id = random_id;
        entry_list[entry_id].empty = false;
        printf("> ID: %d\n", random_id);
    } else {
        printf("~ DB Full!\n");
    }
}

void list_entry(int x)
{
    if (entry_list[x].empty){return;}
    printf("~ %s %s\n", entry_list[x].first_name, entry_list[x].last_name);
    printf("  ID: %d\n", entry_list[x].id);
    if (entry_list[x].paid)
    {
        printf("  PAID\n");
    } else {
        printf("  NOT PAID\n");
    }
    if (entry_list[x].checked_in)
    {
        printf("  CHECKED IN\n");
    } else {
        printf("  ABSENT\n");
    }
    printf("\n");
}

void list_name(char *name)
{
    printf("\n");
    // check first names
    for (int x = 0; x < DB_SIZE; x++)
    {
        if (strComp(&entry_list[x].first_name[0], name))
        {
            list_entry(x);
        }
    }
    // check last names
    for (int x = 0; x < DB_SIZE; x++)
    {
        if (strComp(&entry_list[x].last_name[0], name))
        {
            list_entry(x);
        }
    }
}

void list_id(char *arg)
{
    printf("\n");
    int id = atoi(arg);
    for (int x = 0; x < DB_SIZE; x++)
    {
        if (id == entry_list[x].id)
        {
            list_entry(x);
        }
    }
}

void check_in(char *arg)
{
    int id = atoi(arg);
    for (int x = 0; x < DB_SIZE; x++)
    {
        if (id == entry_list[x].id)
        {
            entry_list[x].checked_in = true;
            entry_list[x].paid = true;
            printf("~ Checked in %s %s\n", entry_list[x].first_name, entry_list[x].last_name);
        }
    }
}

void paid(char *arg)
{
    int id = atoi(arg);
    for (int x = 0; x < DB_SIZE; x++)
    {
        if (id == entry_list[x].id)
        {
            entry_list[x].paid = true;
            printf("~ %s %s paid in advance\n", entry_list[x].first_name, entry_list[x].last_name);
        }
    }
}

void reset_all()
{
    for (int x = 0; x < DB_SIZE; x++)
    {
        entry_list[x].checked_in = false;
        entry_list[x].paid = false;
    }
    printf("~ All entries reset\n");
}

void reset(char *arg)
{
    int id = atoi(arg);
    for (int x = 0; x < DB_SIZE; x++)
    {
        if (id == entry_list[x].id)
        {
            entry_list[x].checked_in = false;
            entry_list[x].paid = false;
            printf("~ %s %s has been reset\n", entry_list[x].first_name, entry_list[x].last_name);
        }
    }
}

void remove_id(char *arg)
{
    int id = atoi(arg);
    for (int x = 0; x < DB_SIZE; x++)
    {
        if (id == entry_list[x].id)
        {
            printf("~ Removed %s %s\n", entry_list[x].first_name, entry_list[x].last_name);
            erase_entry(x);
        }
    }
}

void list_all()
{
    for (int x = 0; x < DB_SIZE; x++)
    {
        list_entry(x);
    }
}

void help()
{
    printf("Commands:\n");
    printf("exit - save and quit\n");
    printf("name (first or last) - list entry(s) with matching name\n");
    printf("ls - list all entries\n");
    printf("id (id) - list entry with matching id\n");
    printf("in (id) - check in\n");
    printf("clear - clear screen\n");
    printf("save - save database\n");

    printf("\n: Mod use ONLY! :\n");
    printf("add - add entry and generate ID\n");
    printf("paid (id) - paid in advance\n");
    printf("reset (id or all) - resets advance pay and check in\n");
    printf("rm (id) - removes entry with matching id\n");
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
        if (token != NULL)
        {
            strcpy(arg, token);
        }

        if (strComp(command, "config\0"))
        {
            config();
        } else if (strComp(command, "exit\0"))
        {
            save_db();
            run = false;
        } else if (strComp(command, "add\0"))
        {
            add();
        } else if (strComp(command, "help\0"))
        {
            help();
        } else if (strComp(command, "name\0"))
        {
            list_name(&arg[0]);
        } else if (strComp(command, "id\0"))
        {
            list_id(&arg[0]);
        } else if (strComp(command, "clear\0"))
        {
            system("clear");
        } else if (strComp(command, "in\0"))
        {
            check_in(&arg[0]);
        } else if (strComp(command, "paid\0"))
        {
            paid(&arg[0]);
        } else if (strComp(command, "reset\0"))
        {
            if (strComp(arg, "all\0"))
            {
                reset_all();
            } else {
                reset(&arg[0]);
            }
        } else if (strComp(command, "rm\0"))
        {
            if (strComp(arg, "all\0"))
            {
                rm_all();
            } else {
                remove_id(&arg[0]);
            }
        } else if (strComp(command, "save\0"))
        {
            save_db();
        } else if (strComp(command, "ls\0"))
        {
            list_all();
        }
    }
}

int main()
{
    srand((unsigned int)time(NULL)); // inits random number gen
    printf(":Entry List:\n");
    init();
    while (run)
    {
        shell();
    }
    return 0;
}
