/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h 
 * a připravených koster funkcí implementujte binární vyhledávací 
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci. 
 */
void bst_init(bst_node_t **tree) {
    *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    while (tree != NULL) {
        if (key < tree->key) {
            tree = tree->left;  
        } else if (key > tree->key) {
            tree = tree->right;  
        } else {
            *value = tree->value;
            return true;
        }
    }
    return false;
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší. 
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    bst_node_t *new_node = (bst_node_t*)malloc(sizeof(bst_node_t));
     
    new_node->key = key;     
    new_node->value = value;                     
    new_node->left = NULL;       
    new_node->right = NULL;  
             
    if (*tree == NULL) {
        *tree = new_node;   
    } else {         
        bst_node_t *current_node = *tree;        
        while (true) {               
             if (key > current_node->key) {          
                if (current_node->right == NULL) {       
                    current_node->right = new_node;          
                    break;       
                } else {         
                    current_node = current_node->right;          
                }        
            }else if (key < current_node->key) {
                if (current_node->left == NULL) {        
                    current_node->left = new_node;               
                    break;               
                } else {                 
                    current_node = current_node->left;                   
                }            
            } else {         
                current_node->value = value;         
                free(new_node);                 
                break;           
            }            
        }
    }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    bst_node_t *current_node = *tree;
    bst_node_t *prev_node = NULL;

    while (current_node->right != NULL) {
        prev_node = current_node;
        current_node = current_node->right;
    }

    target->value = current_node->value;
    target->key = current_node->key;
    

    if (prev_node == NULL) {
        *tree = current_node->left; 
    } else {
        prev_node->right = current_node->left;
    }
    free(current_node);
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 * 
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 * 
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
    if (*tree == NULL) return;             

    bst_node_t *current_node = *tree;         
    bst_node_t *prev_node = NULL;
                 
    while (current_node != NULL) {
        if (key < current_node->key) {                   
            prev_node = current_node;
            current_node = current_node->left;                 
        } else if (key > current_node->key) {             
            prev_node = current_node;            
            current_node = current_node->right; 
        } else {         
            if (current_node->left != NULL && current_node->right != NULL) {    
                bst_replace_by_rightmost(current_node, &(current_node->left));             
            }else if (current_node->left == NULL && current_node->right == NULL) {       
                if (prev_node == NULL) {            
                    *tree = NULL;        
                } else if (prev_node->left == current_node) {       
                    prev_node->left = NULL;
                } else {
                    prev_node->right = NULL;
                }
                free(current_node);       
            }   else {         
                bst_node_t *child_node;
                if (current_node->left != NULL) {
                    child_node = current_node->left;
                } else {
                    child_node = current_node->right;
                }            
                if (prev_node == NULL) {           
                    *tree = child_node;       
                } else if (prev_node->left == current_node) {            
                    prev_node->left = child_node;
                } else {             
                    prev_node->right = child_node;   
                }
                free(current_node);
            }
            break;
        }
    }
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
    stack_bst_t stack_for_nodes;     
    stack_bst_init(&stack_for_nodes);      

    stack_bst_push(&stack_for_nodes, *tree);
    while(!stack_bst_empty(&stack_for_nodes)){
        bst_node_t *temp_node = stack_bst_pop(&stack_for_nodes);
        if(temp_node != NULL){
            stack_bst_push(&stack_for_nodes, temp_node->left);
            stack_bst_push(&stack_for_nodes, temp_node->right);
            free(temp_node);
        }
    }
    *tree = NULL;
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {
    bst_node_t *temp_node = tree;

    while(temp_node){
        stack_bst_push(to_visit, temp_node);
        bst_add_node_to_items(temp_node, items);
        temp_node = temp_node->left;
    }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
    stack_bst_t stack_for_nodes;
    stack_bst_init(&stack_for_nodes);
    bst_node_t *temp_node = tree;

    bst_leftmost_preorder(temp_node, &stack_for_nodes, items);

    while(!stack_bst_empty(&stack_for_nodes)){
        temp_node = stack_bst_top(&stack_for_nodes);
        stack_bst_pop(&stack_for_nodes);
        bst_leftmost_preorder(temp_node->right, &stack_for_nodes, items);
    }
}

/*
 * Pomocná funkce pro iterativní inorder.
 * 
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
    bst_node_t *temp_node = tree;

    while(temp_node){
        stack_bst_push(to_visit, temp_node);
        temp_node = temp_node->left;
    }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
    bst_node_t *temp_node = tree;
    stack_bst_t stack_for_nodes;
    stack_bst_init(&stack_for_nodes);

    bst_leftmost_inorder(temp_node, &stack_for_nodes);

    while (!stack_bst_empty(&stack_for_nodes)){
        temp_node = stack_bst_top(&stack_for_nodes);
        stack_bst_pop(&stack_for_nodes);
        bst_add_node_to_items(temp_node, items);
        bst_leftmost_inorder(temp_node->right, &stack_for_nodes);
    }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
    bst_node_t *temp_node = tree;

    while(temp_node != NULL){
        stack_bst_push(to_visit, temp_node);
        stack_bool_push(first_visit, true);
        temp_node = temp_node->left;
    }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
    bst_node_t *temp_node = tree;


    bool is_left;



    stack_bool_t stack_for_first;
    stack_bool_init(&stack_for_first);

    stack_bst_t stack_for_nodes;
    stack_bst_init(&stack_for_nodes);

    bst_leftmost_postorder(temp_node, &stack_for_nodes, &stack_for_first);

    while(!stack_bst_empty(&stack_for_nodes)){
        is_left = stack_bool_pop(&stack_for_first);
        temp_node = stack_bst_top(&stack_for_nodes);
        stack_bst_pop(&stack_for_nodes);
        if(!is_left){
            bst_add_node_to_items(temp_node, items);
        }
        else{
            stack_bst_push(&stack_for_nodes, temp_node);
            stack_bool_push(&stack_for_first, false);
            bst_leftmost_postorder(temp_node->right, &stack_for_nodes, &stack_for_first);
        }
    }
}
