#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define HASH_TABLE_SIZE 10007

struct HashItem {
  int key;
  int value;
  struct HashItem *next;
};

int hashFunction(int hash_input) {
  if (hash_input < 0) {
    hash_input = -hash_input;
  }
  return hash_input % HASH_TABLE_SIZE;
}

/*
 * If the key for the hash function has already been added,
 * we overwrite the value. Otherwise, we create a new Node.
 */
void insert_and_maybe_overwrite(
  struct HashItem** hash_table,
  int hash_index,
  int key,
  int value
) {
  struct HashItem* node_to_add = (struct HashItem*)malloc(sizeof(struct HashItem));
  node_to_add->key = key;
  node_to_add->value = value;
  node_to_add->next = NULL;

  if (hash_table[hash_index] != NULL) {
    struct HashItem* current = hash_table[hash_index];
    while (current->next != NULL) {
      if (current->key == key) {
        current->value = value;
        free(node_to_add);
        return;
      }
      current = current->next;
    }
    current->next = node_to_add;
  } else {
    hash_table[hash_index] = node_to_add;
  }
}

/*
 * In practice, returns the index of the last occurence of a value in
 * the input.
 */
int check_hash_map(struct HashItem** hash_table, int key) {

  int hash_index = hashFunction(key);
  if (hash_table[hash_index] != NULL) {
    struct HashItem* found_node = hash_table[hash_index];
    while (found_node->key != key) {
      if (found_node->next != NULL) {
        found_node = found_node->next;
      } else {
        return -1;
      }
    }
    return found_node->value;
  } else {
    return -1;
  }
}

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
  struct HashItem* hash_map[HASH_TABLE_SIZE] = {NULL};

  // create hash map mapping value to index
  for (unsigned int i = 0; i < numsSize; i++) {
    int hash_index = hashFunction(nums[i]);  
    insert_and_maybe_overwrite(hash_map, hash_index, nums[i], i);
  }

  // check for the pairs that add to target
  for (unsigned int i = 0; i < numsSize; i++) {
    int current_num = nums[i];
    int num_to_find = target - current_num;

    int index_of_num_to_find = check_hash_map(hash_map, num_to_find);
    if (index_of_num_to_find != -1 && index_of_num_to_find != i) {
      int* rv = (int *)malloc(sizeof(int *) * 2);
      rv[0] = index_of_num_to_find;
      rv[1] = i;
      printf("returning %d, %d\n", rv[0], rv[1]);
      *returnSize = 2;
      return rv;
    }
  }
  int* error_rv = (int *)malloc(sizeof(int *) * 2);
  error_rv[0] = -1;
  error_rv[1] = -1;
  return error_rv;
}

int main() { 
  int* returnSize = (int *)malloc(sizeof(int));
  twoSum((int[]){2, 7, 11, 15}, 4, 9, returnSize);
  return 0;
}
