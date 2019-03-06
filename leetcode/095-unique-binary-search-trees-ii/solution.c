#include <stdio.h>
#include <stdlib.h>

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

void print_tree(struct TreeNode *node)
{
    printf("%d ", node->val);
    if (node->left != NULL) {
        print_tree(node->left);
    } else {
        printf("# ");
    }
    if (node->right != NULL) {
        print_tree(node->right);
    } else {
        printf("# ");
    }
}

void free_tree(struct TreeNode *node)
{
    if (node == NULL) {
        return;
    }

    if (node->left != NULL) {
        free_tree(node->left);
    }
    if (node->right != NULL) {
        free_tree(node->right);
    }
    free(node);
}

/**
 *
 *         i=1
 *        /   \
 *    (1,0)   (2,3)   // 2 possibilities
 *
 *
 *         i=1
 *        /   \
 *    (1,0)    i=2
 *            /   \
 *        (2,1)   (3,3)
 *
 *
 *         i=1
 *        /   \
 *    (1,0)    i=3
 *            /   \
 *        (2,2)   (4,3)
 *
 *
 *         i=2
 *        /   \
 *    (1,1)   (3,3)   // 1 possiblity
 *
 *
 *         i=3
 *        /   \
 *    (1,2)   (4,3)   // 2 possibilities
 *
 *
 *          i=3
 *         /   \
 *      i=1    (4,3)
 *     /   \
 * (2,1)   (3,3)
 *
 *
 *         i=3
 *        /   \
 *      i=2   (4,3)
 *     /   \
 * (1,1)   (3,2)
 *
 */
struct TreeNode *dfs(int low, int high, int *count)
{
    int i, j, k;

    if (low > high) {
        *count = 0;
        return NULL;
    } else if (low == high) {
        struct TreeNode *node = malloc(sizeof(struct TreeNode));
        node->val = low;
        node->left = NULL;
        node->right = NULL;
        *count = 1;
        return node;
    } else {
        *count = 0;
        int capacity = 5;
        struct TreeNode *roots = malloc(capacity * sizeof(struct TreeNode));
        for (i = low; i <= high; i++) {
            // Possibilities of roots with different values.
            int left_cnt, right_cnt;
            struct TreeNode *left_subs = dfs(low, i - 1, &left_cnt);
            struct TreeNode *right_subs = dfs(i + 1, high, &right_cnt);
            // Total number = left sub possibilities * right sub possibilities.
            if (left_cnt == 0) left_cnt = 1;
            if (right_cnt == 0) right_cnt = 1;
            if (*count + (left_cnt * right_cnt) >= capacity) {
                capacity *= 2;
                capacity += left_cnt * right_cnt;
                roots = realloc(roots, capacity * sizeof(struct TreeNode));
            }
            for (j = 0; j < left_cnt; j++) {
                for (k = 0; k < right_cnt; k++) {
                    // Possibilities of roots with same value but different sub-trees.
                    roots[*count].val = i;
                    roots[*count].left = left_subs == NULL ? NULL : &left_subs[j];
                    roots[*count].right = right_subs == NULL ? NULL : &right_subs[k];
                    (*count)++;
                }
            }
        }
        return roots;
    }
}

struct TreeNode **generate_trees(int n, int *return_size)
{
    int i, count = 0;
    struct TreeNode *roots = dfs(1, n, &count);
    struct TreeNode **results = malloc(count * sizeof(struct TreeNode *));
    for (i = 0; i < count; i++) {
        results[i] = &roots[i];
    }
    *return_size = count;
    return results;
}

int main(int argc, char **argv)
{
    int inputs[] = { 3 }, i, len = sizeof(inputs) / sizeof(int);
    int j, return_size;

    for (i = 0; i < len; i++) {
        int n = inputs[i];

        printf("\n Input: %d\n", n);

        struct TreeNode **trees = generate_trees(n, &return_size);
        for (j = 0; j < return_size; j++) {
            print_tree(trees[j]);
            printf("\n");
        }
        
        /*
        for (j = 0; j < return_size; j++) {
            free(trees[j]);
        }
        */
        free(trees);
    }

    return EXIT_SUCCESS;
}
