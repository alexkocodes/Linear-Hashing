#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <stdbool.h>

struct Info
{
    int pin;
    char *fname;
    char *lname;
    int zip;
    char status;
};
struct Node
{
    int pin;
    struct Node *next;
    struct Info *data;
};

struct LinkedList
{
    struct Node *head;
    struct LinkedList *page;
    int size;
};
struct Node *head;

struct LinkedList *createLinkedList();
int printList(struct LinkedList *list);
bool insert(struct LinkedList *ll, int pin, char *fname, char *lname, int zip, char status, int bucketentries);
void freeList(struct LinkedList *list);
struct Info *lookUp(struct LinkedList *list, int pin);

#endif
