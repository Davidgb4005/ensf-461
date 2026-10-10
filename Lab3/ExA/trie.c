#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "trie.h"
//#define SUPPORTED_OPERATOR_COUNT 7
//#define SUPPORTED_OPERATORS {'<','>','|','2','&',';','!'}

static const char * supported_operators[SUPPORTED_OPERATOR_COUNT] =
    SUPPORTED_OPERATORS;

int charToTrieIndex(const char c)
{
    switch (c)
    {
    case '|':
        return 0;
        break;
    case '&':
        return 1;
        break;
    case '<':
        return 2;
        break;
    case '2':
        return 3;
        break;
    case '>':
        return 4;
        break;
    case ';':
        return 5;
        break;
    case '!':
        return 6;
        break;
    default:
        return 254;
    }
}

trie_node *trieAddOperator(
    trie_node *root,
    const char *op_token,
    operator_e operator_id)
{
    trie_node *cursor = root;
    while (*op_token)
    {
        int child_index = charToTrieIndex(*op_token);

        if (cursor->children[child_index])
        {
            cursor = cursor->children[child_index];
        }
        else
        {
            trie_node *new_node = calloc(1, sizeof(trie_node));

            if (!new_node)
            {
                exit(1);
            }

            new_node->token = *op_token;

            cursor->children[child_index] = new_node;
            cursor = new_node;
        }

        op_token++;
    }
    cursor->word_end = 1;
    cursor->operator_id = operator_id;

    return root;
}

operator_e trieSearch(trie_node *root, const char *op_token)
{
    trie_node *cursor = root;
    while (*op_token)
    {
        int child_index = charToTrieIndex(*op_token);
        if (child_index > SUPPORTED_OPERATOR_COUNT){
            return no_op;
        }
        if (cursor->children[child_index])
        {
            cursor = cursor->children[child_index];
        }
        else
        {
            return no_op;
        }
        op_token++;
    }
    if (!cursor->word_end)
    {
        return no_op;
    }
    return cursor->operator_id;
}
void trieFree(trie_node *node)
{
    if (node == NULL)
        return;

    for (int i = 0; i < SUPPORTED_OPERATOR_COUNT; i++) {
        trieFree(node->children[i]);
    }

    free(node);
}