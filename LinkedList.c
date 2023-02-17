#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

struct LinkedList *createLinkedList()
{
	struct LinkedList *ll = (struct LinkedList *)malloc(sizeof(struct LinkedList));
	ll->page = NULL;
	ll->head = NULL;
	ll->size = 0;
	return ll;
}

bool insert(struct LinkedList *ll, int pin, char *fname, char *lname, int zip, char status, int bucketentries)
{
	/*printf("Inserting: %d %s %s %d\n", pin, fname, lname, zip);*/
	bool split = false;
	while (ll->size == bucketentries && ll->page != NULL)
	{
		ll = ll->page;
	}
	if (ll->size == bucketentries)
	{
		struct LinkedList *newPage = createLinkedList();
		ll->page = newPage;
		ll = newPage;
		split = true;
	}
	struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
	newNode->pin = pin;
	newNode->next = ll->head;

	struct Info *newInfo = (struct Info *)malloc(sizeof(struct Info));
	newInfo->pin = pin;
	newInfo->fname = (char *)malloc(100 * sizeof(char));
	newInfo->lname = (char *)malloc(100 * sizeof(char));
	strcpy(newInfo->fname, fname);
	strcpy(newInfo->lname, lname);
	newInfo->zip = zip;
	newInfo->status = status;

	newNode->data = newInfo;
	ll->head = newNode;
	ll->size++;

	return split;
}

int printList(struct LinkedList *list)
{
	int num_people = 0;
	struct Node *curr = list->head;
	while (list->size > 0)
	{

		num_people += list->size;
		while (curr != NULL)
		{
			printf("%d ", curr->data->pin);
			printf("%s ", curr->data->fname);
			printf("%s ", curr->data->lname);
			printf("%d ", curr->data->zip);
			printf("%c \n", curr->data->status);
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
	return num_people;
}

void freeList(struct LinkedList *list)
{
	struct LinkedList *templist = list;
	struct Node *curr = list->head;
	while (list->size > 0)
	{

		while (curr != NULL)
		{
			struct Node *temp = curr;
			curr = curr->next;
			free(temp->data->fname);
			free(temp->data->lname);
			free(temp->data);
			free(temp);
			list->size--;
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
	templist->head = NULL;
}

struct Info *lookUp(struct LinkedList *list, int pin)
{
	struct Node *curr = list->head;
	while (list->size > 0)
	{
		while (curr != NULL)
		{
			if (curr->data->pin == pin)
			{

				printf("%d ", curr->data->pin);
				printf("%s ", curr->data->fname);
				printf("%s ", curr->data->lname);
				printf("%d ", curr->data->zip);
				printf("%c ", curr->data->status);
				printf("\n");
				return curr->data;
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
	return NULL;
}
