/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte funkci letter_count. Výstupní strom může být značně degradovaný (až na úroveň lineárního seznamu) 
 * a tedy implementujte i druhou funkci (bst_balance), která strom, na požadavek uživatele, vybalancuje.
 * Funkce jsou na sobě nezávislé a tedy automaticky NEVOLEJTE bst_balance v letter_count.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
void letter_count(bst_node_t **tree, char *input) {
    if (tree == NULL || input == NULL) return; 
    
    bst_init(tree);

    for (int i = 0; input[i] != '\0'; i++) {
        char character = input[i];

        if (character >= 'A' && character <= 'Z') {
            character = character + 32; 
        }

        if (character >= 'a' && character <= 'z') {
            int value = 1;
            if (bst_search(*tree, character, &value)) {
                value++; 
                bst_delete(tree, character); 
            }
            bst_insert(tree, character, value);
        }else if (character == ' ') {
            int value = 1;
            if (bst_search(*tree, ' ', &value)) {
                value++;
                bst_delete(tree, ' ');
            }
            bst_insert(tree, ' ', value);
        } else {
            int value = 1;
            if (bst_search(*tree, '_', &value)) {
                value++;
                bst_delete(tree, '_');
            }
            bst_insert(tree, '_', value);
        }
    }
}

 // additional function for balancing tree
bst_node_t* additional_func_for_balanced_bst(bst_node_t **nodes, int start, int end) {
    if (start > end) {
        return NULL;
    }

    int middle = (start + end) / 2;
    bst_node_t *root = nodes[middle];
    root->left = additional_func_for_balanced_bst(nodes, start, middle - 1);
    root->right = additional_func_for_balanced_bst(nodes, middle + 1, end);

    return root;
}

/**
 * Vyvážení stromu.
 * 
 * Vyvážený binární vyhledávací strom je takový binární strom, kde hloubka podstromů libovolného uzlu se od sebe liší maximálně o jedna.
 * 
 * Předpokládejte, že strom je alespoň inicializován. K získání uzlů stromu využijte vhodnou verzi vámi naimplmentovaného průchodu stromem.
 * Následně můžete například vytvořit nový strom, kde pořadím vkládaných prvků zajistíte vyváženost.
 *  
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce. Není nutné, aby funkce fungovala *in situ* (in-place).
*/
void bst_balance(bst_node_t **tree) {
    // bst_items_t *items = bst_init_items(); doesnt seem to work somehow so inititialized by myself
    bst_items_t items;
    items.nodes = malloc(sizeof(bst_node_t*));
    items.capacity = 0;
    items.size = 0;
    bst_inorder(*tree, &items);
    *tree = additional_func_for_balanced_bst(items.nodes, 0, items.size - 1);
    free(items.nodes);
}