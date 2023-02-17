#ifndef ZIPLIST_H
#define ZIPLIST_H
#include "LinkedList.h"
#include <stdbool.h>
#define bool _Bool

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

void insertZipList(struct LinkedList *list, struct ZipList *ziplist, int zip);
void freeZipList(struct ZipList *list);
void printZipList(struct ZipList *list);
int voted(struct ZipList *list);
void bulkVote(struct LinkedList *list, struct ZipList *zlist, int *bulkofkeys);
void printYesInZip(struct ZipList *list, int zip);
void markYes(struct LinkedList *list, int pin, struct ZipList *zlist);
void insertPointerList(struct LinkedList *ll, int pin, struct Info *info);
#endif
