#include <stdio.h>
#include <stdlib.h>
#include "24_tree.c"

int main(){
    int key, option;

    while(option != 5){
        printf("[ 1 ] Print Tree\n");
        printf("[ 2 ] Insert Value\n");
        printf("[ 3 ] Search Value\n");
        printf("[ 4 ] Delete\n");
        printf("[ 5 ] Exit\n");

        printf("Enter option: ");
        scanf("%d", &option);
        
        switch(option){
            case 1:
                printf("The tree is:\n");
                print(root, 0);
                break;
            
            case 2:
                printf("Enter the key: ");
                scanf("%d", &key); 
                insert_24_tree(key);
                break;
            case 3:
                printf("Enter the key: ");
                scanf("%d", &key); 
                search(key);
                break;
                
            case 4:
                printf("Enter the key: ");
                scanf("%d", &key); 
                delete_24_tree(key);
                break;
                
            case 5:
                exit(1);    
                
        } 
    } 	
    return 0;
}
