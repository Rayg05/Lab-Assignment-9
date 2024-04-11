#include <stdio.h>
#include <stdlib.h> // Include for malloc
#include <string.h> // Include for memcpy

#define HASH_SIZE 23

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order;
	struct RecordType* next; // Pointer for chaining
};

// Compute the hash function
int hash(int x)
{
    return x % HASH_SIZE;
}

int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
			pRecord->next = NULL; // Initialize next pointer
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct RecordType pData[], int dataSz)
{
    struct RecordType* hashTable[HASH_SIZE] = { NULL }; // Initialize hash table to NULL

    printf("Hash Table:\n");
    for (int i = 0; i < dataSz; ++i)
    {
        struct RecordType* record = &pData[i];
        int index = hash(record->id);

        // If the slot is empty, create a new linked list node
        if (hashTable[index] == NULL)
        {
            hashTable[index] = (struct RecordType*) malloc(sizeof(struct RecordType));
            memcpy(hashTable[index], record, sizeof(struct RecordType));
            hashTable[index]->next = NULL;
        }
        else
        {
            // Collision detected, traverse the linked list and append the record
            struct RecordType* current = hashTable[index];
            while (current->next != NULL)
            {
                current = current->next;
            }
            current->next = (struct RecordType*) malloc(sizeof(struct RecordType));
            memcpy(current->next, record, sizeof(struct RecordType));
            current->next->next = NULL;
        }
    }

    // Display the hash table
    for (int i = 0; i < HASH_SIZE; ++i)
    {
        if (hashTable[i] != NULL)
        {
            printf("Index %d -> ", i);
            struct RecordType* current = hashTable[i];
            while (current != NULL)
            {
                printf("%d %c %d -> ", current->id, current->name, current->order);
                current = current->next;
            }
            printf("NULL\n");
        }
    }
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	displayRecordsInHash(pRecords, recordSz);
}
