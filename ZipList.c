/* Zip List */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "LinkedList.h"

struct ZipNode
{
    int zip;
    struct ZipNode *next;
    struct LinkedList *ptr_list;
};

struct ZipList
{
    struct ZipNode *head;
    int size;
};
struct ZipList *createZipList()
{
    struct ZipList *list = (struct ZipList *)malloc(sizeof(struct ZipList));
    list->head = NULL;
    list->size = 0;
    return list;
}

int voted(struct ZipList *list)
{
    struct ZipNode *curr = list->head;
    int voted_num = 0;
    while (curr != NULL)
    {
        voted_num += curr->ptr_list->size;
        curr = curr->next;
    }
    return voted_num;
}
int cmp(const void *a, const void *b)
{
    return (*(int *)b > *(int *)a) - (*(int *)b < *(int *)a);
}
void printZipList(struct ZipList *list)
{
    struct ZipNode *curr = list->head;
    /* create a 2D array storing the size and zip*/
    if (list->size == 0)
    {
        printf("No one has voted yet!\n");
        return;
    }
    int a[list->size][2];
    int i = 0;
    while (curr != NULL)
    {
        /*printf("Zip: %d, Size: %d\n", curr->zip, curr->ptr_list->size);*/
        a[i][0] = curr->ptr_list->size;
        a[i][1] = curr->zip;
        curr = curr->next;
        i++;
    }

    int n = sizeof a / sizeof *a;

    qsort(a, n, sizeof *a, cmp); /* qsort array of pointers */

    for (int(*p)[2] = a; p < a + n; p++) /* output results */
        printf("%d %d\n", (*p)[1], (*p)[0]);
}

void printYesInZip(struct ZipList *list, int zip)
{
    struct ZipNode *curr = list->head;
    while (curr != NULL)
    {
        if (curr->zip == zip)
        {
            printList(curr->ptr_list);
            return;
        }
        curr = curr->next;
    }
    printf("Zip code not found\n");
}
void insertPointerList(struct LinkedList *ll, int pin, struct Info *info)
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->pin = pin;
    newNode->next = ll->head;
    newNode->data = info;
    ll->head = newNode;
    ll->size++;
}

void insertZipList(struct LinkedList *list, struct ZipList *ziplist, int pin)
{
    struct Info *info = lookUp(list, pin);
    if (info == NULL)
    {
        printf("Person code not found\n");
        return;
    }

    int zip = info->zip;
    if (ziplist->head == NULL)
    {
        struct ZipNode *newNode = malloc(sizeof(struct ZipNode));
        newNode->zip = zip;
        newNode->next = NULL;
        newNode->ptr_list = createLinkedList();
        ziplist->head = newNode;
        ziplist->size++;
        insertPointerList(ziplist->head->ptr_list, zip, info);
        return;
    }
    struct ZipNode *curr = ziplist->head;
    while (curr)
    {
        if (curr->zip == zip)
        {

            insertPointerList(curr->ptr_list, zip, info);
            return;
        }
        if (curr->next == NULL)
        {
            /* append a new zip node to the end of ziplist*/
            struct ZipNode *newNode = malloc(sizeof(struct ZipNode));
            newNode->zip = zip;
            newNode->next = NULL;
            newNode->ptr_list = createLinkedList();
            curr->next = newNode;
            ziplist->size++;
            insertPointerList(curr->next->ptr_list, zip, info);
            return;
        }
        curr = curr->next;
    }
}

void markYes(struct LinkedList *list, int pin, struct ZipList *zlist)
{
    struct Node *curr = list->head;
    while (list->size > 0)
    {
        while (curr != NULL)
        {
            if (curr->data->pin == pin)
            {
                if (curr->data->status == 'Y')
                {
                    printf("Already marked yes\n");
                    return;
                }
                else if (curr->data->status == 'N')
                {
                    curr->data->status = 'Y';
                    insertZipList(list, zlist, curr->data->pin);
                    return;
                }
            }
            curr = curr->next;
        }
        if (list->page != NULL)
        {
            list = list->page;
            curr = list->head;
        }
        else
        {
            break;
        }
    }
    printf("Person not found\n");
}
void freeZipList(struct ZipList *list)
{
    struct ZipNode *temp = list->head;
    while (temp)
    {
        freeList(temp->ptr_list);
        struct ZipNode *next = temp->next;
        temp = next;
    }
    free(list);
}