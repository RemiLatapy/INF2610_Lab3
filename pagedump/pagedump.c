/*
 * pagedump.c
 *
 *  Created on: 2013-10-23
 *      Author: francis
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


/*
 * Savegarder la page contenant ptr dans le fichier fname
 */
void save_page(char *fname, void *ptr) {
    (void) fname;
    (void) ptr;
    /*
     * TODO:
     * 1 - calculer l'adresse du début de la page ** done
     * 2 - ouvrir le fichier de destination
     * 3 - écrire la page dans le fichier
     * 4 - fermer le fichier
     */
         
    int *startAddr = ptr-((intptr_t)ptr%getpagesize());

    // hex output
    FILE *dest = fopen(fname, "w");
    
    fwrite(startAddr, 1 , getpagesize() , dest);
    
    fclose(dest);
    
    
    
    // txt output
    char str[80] = "readable.";
    strcat(str,fname);

    FILE *destReadable = fopen(str, "w");
    
    for(int *i = startAddr; i < startAddr+(getpagesize()/sizeof(int)); i++) {
        fprintf(destReadable, "0x%8.8X\t:\t0x%8.8X\n", (unsigned int)(intptr_t)i, *i); // double cast to use %X
    }
    
    fclose(destReadable);


    return;
}

int main(int argc, char **argv) {
    /*
     * L'utilisation de volatile empêche le compilateur
     * d'optimiser (i.e. supprimer) les variables.
     *
     * (void) var; évite l'avertissement concernant une variable non-utilisée
     *
     */

    /*
     * Variables sur la pile (stack)
     */
    volatile int cafe1 = 0x1111CAFE; (void) cafe1;
    volatile int cafe2 = 0x2222CAFE; (void) cafe2;
    volatile int cafe3[2] = { 0x3333CAFE, 0x4444CAFE }; (void) cafe3;

    /*
     * Variables sur le monceau (heap)
     */
    volatile int *beef1 = malloc(sizeof(int)); (void) beef1;
    volatile int *beef2 = malloc(sizeof(int)); (void) beef2;
    volatile int *beef3 = malloc(2 * sizeof(int)); (void) beef3;
    *beef1 = 0x1111BEEF;
    *beef2 = 0x2222BEEF;
    beef3[0] = 0x3333BEEF;
    beef3[1] = 0x4444BEEF;

    save_page("beef.page", (void *)beef1);
    save_page("cafe.page", (void *)&cafe1);
        
    return 0;
}
