#ifndef __LEXER_H
#define __LEXAR_H 

#define SUPPORTED_OPERATOR_COUNT 2
#define SUPPORTED_OPERATORS {"|","|&"}
#define MAX_TRIE_DEPTH 3

typedef enum {
    no_op, //This is what is returned via search when operator is not implmented or DNE
    op_pipe, //Implmented "|"
    pipe_stdout_stderr, // Implmented "|&"
    
    //UnImplmented
    logical_and,
    logical_or,
    seq_exec,
    bg_proc,
    test_proc,
} operator_e;

typedef struct trie_node
{
    char token;
    operator_e operator_id;
    struct trie_node *children[SUPPORTED_OPERATOR_COUNT];
    int word_end;
} trie_node;


// Adds a supported operator and its operation ID to the trie.
trie_node *trieAddOperator(trie_node *root,const char *op_token,operator_e operator_id);

// Searches the trie for an operator and returns its operation ID.
operator_e trieSearch(trie_node *root, const char *op_token);

// Recursively frees all nodes allocated in the trie.
void trieFree(trie_node *node);
#endif
