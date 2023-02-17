#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"

int main(int argc, char *argv[])
{
    char *filename = "";
    int bucketentries = 4;
    int i;
    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-f") == 0)
        {
            filename = argv[i + 1];
        }
        else if (strcmp(argv[i], "-b") == 0)
        {
            bucketentries = atoi(argv[i + 1]);
        }
    }

    /* create the hash table here */
    int table_size = 2;
    struct LinkedList *table = (struct LinkedList *)malloc(sizeof(struct LinkedList) * table_size);
    for (i = 0; i < table_size; i++)
    {
        table[i].head = NULL;
    }

    printf("%s\n", filename);
    printf("%d\n", bucketentries);

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return 1;
    }
    char line[1024];
    int pin;
    char lname[100];
    char fname[100];
    int zip;
    int round = 0;
    int mod_num = 2;
    int p = 0;
    int old_table_size = table_size;

    /* Create the zip list here */
    struct ZipList *ziplist = malloc(sizeof(struct ZipList));
    ziplist->head = NULL;
    ziplist->size = 0;

    while (fgets(line, 1024, file))
    {
        sscanf(line, "%d %s %s %d", &pin, fname, lname, &zip);
        int index = pin % mod_num;
        if (index < p)
        {
            index = pin % (mod_num * 2);
        }
        if (insert(&table[index], pin, fname, lname, zip, 'N', bucketentries))
        {
            /* need to split*/
            /*
            printf("Splitting bucket %d\n", p);
            printf("Table size: %d\n", table_size);
            */
            table_size++;
            struct Node *curr = table[p].head;
            int len = 0;
            struct Node *arr = (struct Node *)malloc(sizeof(struct Node) * len);

            struct LinkedList *toClean = &table[p];
            while (table[p].size > 0)
            {
                while (curr != NULL)
                {
                    len++;
                    arr = (struct Node *)realloc(arr, len * sizeof(struct Node));

                    /* make a copy of the current node */
                    struct Node *copy = malloc(sizeof(struct Node));
                    copy->data = malloc(sizeof(struct Info));
                    copy->data->pin = curr->data->pin;
                    copy->data->fname = (char *)malloc(100 * sizeof(char));
                    copy->data->lname = (char *)malloc(100 * sizeof(char));
                    strcpy(copy->data->fname, curr->data->fname);
                    strcpy(copy->data->lname, curr->data->lname);
                    copy->data->zip = curr->data->zip;
                    copy->data->status = curr->data->status;
                    copy->next = NULL;
                    arr[len - 1] = *copy;
                    curr = curr->next;
                }
                if (table[p].page != NULL)
                {
                    table[p] = *table[p].page;
                    curr = table[p].head;
                }
                else
                {

                    freeList(toClean);
                    toClean = createLinkedList();
                    break;
                }
            }
            table = (struct LinkedList *)realloc(table, sizeof(struct LinkedList) * (table_size));
            table[table_size - 1] = *createLinkedList();
            for (i = 0; i < len; i++)
            {
                int index = arr[i].data->pin % (mod_num * 2);
                int pin = arr[i].data->pin;
                char *fname = (char *)malloc(100 * sizeof(char));
                char *lname = (char *)malloc(100 * sizeof(char));
                strcpy(fname, arr[i].data->fname);
                strcpy(lname, arr[i].data->lname);
                int zip = arr[i].data->zip;

                insert(&table[index], pin, fname, lname, zip, 'N', bucketentries);
                /*printf("Moved to bucket %d\n", index);*/
            }
            printf("\n");
            p++;
            if (p == old_table_size)
            {
                p = 0;
                round++;
                mod_num = mod_num * 2;
                old_table_size = table_size;
                /*
                printf("Table size: %d\n", table_size);
                printf("Enter round: %d\n", round);
                */
            }
            free(arr);
        }
    }

    fclose(file);

    int num_people = 0;

    for (i = 0; i < table_size; i++)
    {
        printf("Bucket: %d\n", i);
        num_people += printList(&table[i]);
    }
    printf("Total number of people: %d\n", num_people);

    char input[100];
    fgets(input, sizeof(input), stdin);

    while (strcmp(input, "exit\n") != 0)
    {
        int pin;
        char *lname;
        char *fname;
        int zip;
        char *token = strtok(input, " ");

        if (strcmp(token, "l") == 0)
        {
            token = strtok(NULL, " ");
            while (token != NULL)
            {
                pin = atoi(token);

                token = strtok(NULL, " ");
                int index = pin % mod_num;
                if (index < p)
                {
                    index = pin % (mod_num * 2);
                }
                struct Info *info = lookUp(&table[index], pin);
                if (info == NULL)
                {
                    printf("Person Not Found\n");
                }
            }
        }
        else if (strcmp(token, "i") == 0)
        {
            int j = 0;
            token = strtok(NULL, " ");
            while (token != NULL)
            {
                switch (j)
                {
                case 0:
                    pin = atoi(token);
                    j++;
                    break;
                case 1:
                    fname = token;
                    j++;
                    break;
                case 2:
                    lname = token;
                    j++;
                    break;
                case 3:
                    zip = atoi(token);
                    j++;
                    break;

                default:
                    break;
                }
                token = strtok(NULL, " ");
            }
            int index = pin % mod_num;
            if (index < p)
            {
                index = pin % (mod_num * 2);
            }
            struct Info *info = lookUp(&table[index], pin);
            if (info == NULL)
            {
                num_people++;
                if (insert(&table[index], pin, fname, lname, zip, 'N', bucketentries))
                {
                    /* need to split*/
                    printf("Splitting bucket %d\n", p);
                    printf("Table size: %d\n", table_size);
                    table_size++;
                    struct Node *curr = table[p].head;
                    int len = 0;
                    struct Node *arr = (struct Node *)malloc(sizeof(struct Node) * len);

                    struct LinkedList *toClean = &table[p];
                    while (table[p].size > 0)
                    {
                        while (curr != NULL)
                        {
                            len++;
                            arr = (struct Node *)realloc(arr, len * sizeof(struct Node));

                            /* make a copy of the current node */
                            struct Node *copy = malloc(sizeof(struct Node));
                            copy->data = malloc(sizeof(struct Info));
                            copy->data->pin = curr->data->pin;
                            copy->data->fname = (char *)malloc(100 * sizeof(char));
                            copy->data->lname = (char *)malloc(100 * sizeof(char));
                            strcpy(copy->data->fname, curr->data->fname);
                            strcpy(copy->data->lname, curr->data->lname);
                            copy->data->zip = curr->data->zip;
                            copy->data->status = curr->data->status;
                            copy->next = NULL;
                            arr[len - 1] = *copy;
                            curr = curr->next;
                        }
                        if (table[p].page != NULL)
                        {
                            table[p] = *table[p].page;
                            curr = table[p].head;
                        }
                        else
                        {
                            printf("len: %d\n", len);
                            freeList(toClean);
                            toClean = createLinkedList();
                            break;
                        }
                    }
                    table = (struct LinkedList *)realloc(table, sizeof(struct LinkedList) * (table_size));
                    table[table_size - 1] = *createLinkedList();
                    for (i = 0; i < len; i++)
                    {
                        printf("Array: %d ", arr[i].data->pin);
                        printf("%s ", arr[i].data->fname);
                        printf("%s ", arr[i].data->lname);
                        printf("%d ", arr[i].data->zip);
                        printf("%c \n", arr[i].data->status);
                        int index = arr[i].data->pin % (mod_num * 2);
                        int pin = arr[i].data->pin;
                        char *fname = (char *)malloc(100 * sizeof(char));
                        char *lname = (char *)malloc(100 * sizeof(char));
                        strcpy(fname, arr[i].data->fname);
                        strcpy(lname, arr[i].data->lname);
                        int zip = arr[i].data->zip;

                        insert(&table[index], pin, fname, lname, zip, 'N', bucketentries);
                        printf("Moved to bucket %d\n", index);
                    }
                    printf("\n");
                    p++;
                    if (p == old_table_size)
                    {
                        p = 0;
                        round++;
                        mod_num = mod_num * 2;
                        old_table_size = table_size;
                        printf("Table size: %d\n", table_size);
                        printf("Enter round: %d\n", round);
                    }
                    free(arr);
                }
            }
            else
            {
                printf("Person Already Exists\n");
            }
        }
        else if (strcmp(token, "r") == 0)
        {
            token = strtok(NULL, " ");
            while (token != NULL)
            {
                pin = atoi(token);

                int index = pin % mod_num;
                if (index < p)
                {
                    index = pin % (mod_num * 2);
                }
                markYes(&table[index], pin, ziplist);
                token = strtok(NULL, " ");
            }
        }
        else if (strcmp(token, "bv") == 0)
        {
            token = strtok(NULL, " ");

            size_t len = strlen(token) - 1;
            char *fileofkeys = (char *)malloc(len);
            memcpy(fileofkeys, token, len);

            FILE *keys = fopen(fileofkeys, "r");
            if (keys == NULL)
            {
                printf("Error opening file!\n");
            }
            else
            {
                while (fgets(line, 1024, keys))
                {
                    pin = atoi(line);

                    int index = pin % mod_num;
                    if (index < p)
                    {
                        index = pin % (mod_num * 2);
                    }
                    markYes(&table[index], pin, ziplist);
                }
                fclose(keys);
            }
            free(fileofkeys);
        }
        else if (strcmp(token, "v\n") == 0)
        {
            printf("Number of people voted: %d\n", voted(ziplist));
        }
        else if (strcmp(token, "perc\n") == 0)
        {
            /*printf("Number of people voted: %d\n", voted(ziplist));*/
            float percentage = ((float)voted(ziplist) / num_people) * 100;
            printf("Percentage of people who voted: %.2f%%\n", percentage);
        }
        else if (strcmp(token, "z") == 0)
        {
            token = strtok(NULL, " ");
            while (token != NULL)
            {
                zip = atoi(token);
                printYesInZip(ziplist, zip);
                token = strtok(NULL, " ");
            }
        }
        else if (strcmp(token, "o\n") == 0)
        {
            printZipList(ziplist);
        }
        else
        {
            printf("Invalid command\n");
        }
        fgets(input, sizeof(input), stdin);
    }
    for (i = 0; i < table_size; i++)
    {
        freeList(&table[i]);
    }
    free(ziplist);
    free(table);
    return 0;
}
