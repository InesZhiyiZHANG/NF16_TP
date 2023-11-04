#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tp3.h"





/* ********************************
 * Création et initialisation Bloc
 ******************************** */
T_Block *creerBloc(int id, char* date){
    T_Block *nouveauBloc = malloc(sizeof(T_Block));
    if (nouveauBloc != NULL) {
        // l'allocation mémoire s'est bien passée
        nouveauBloc->dateBloc = malloc(strlen(date) + 1);
        // reserver la space pour '\0'
        strcpy( nouveauBloc->dateBloc,date );
        nouveauBloc->idBloc = id;
        nouveauBloc->listeTransactions = NULL; //la tête d'une liste de transactions
        nouveauBloc->suivant = NULL;//le nouveaubloc est le dernier
    }

    return nouveauBloc;
}



/* **************************************
 * Création et initialisation Transaction
 ************************************** */
T_Transaction *creerTransaction(int id, float montant, char *descr) {

    T_Transaction *nouvelleTransaction = malloc(sizeof(T_Transaction));
    if (nouvelleTransaction != NULL) {
        // l'allocation mémoire s'est bien passée
        nouvelleTransaction->description = malloc(strlen(descr) + 1); // Important : malloc permet d'avoir l'espace mémoire dédié pour notre champ de structure
        strcpy(nouvelleTransaction->description,descr);
        nouvelleTransaction->idEtu = id;
        nouvelleTransaction->montant = montant;
        nouvelleTransaction->suivant = NULL;
    }

    return nouvelleTransaction;
}


/* ********************************
 * 1.	Ajout d'une transaction en tête d'une liste de transactions :
 ******************************** */
T_Transaction *ajouterTransaction(int idEtu, float montant, char *descr, T_Transaction *listeTransaction) {
    T_Transaction *tempT = creerTransaction(idEtu, montant, descr); //créer une nouvel transaction « tempT »
    tempT->suivant = listeTransaction; //ajouter la nouvel transaction
    //listeTransaction = tempT; //faire de « tempT » la tête de listeTransaction
    return tempT;
}



/* ********************************
 * 2.	Ajout d'un bloc en tête de la BlockChain :
 ******************************** */
BlockChain ajouterBlock(BlockChain bc){
    char date[12];
    printf("Entrer la date :\n");
    scanf("%s", date);
    //vérifier s'il existe une date identique
    T_Block *pB = bc;
    while(pB != NULL){
        if(strcmp(date, pB->dateBloc) == 0){
            printf("Cette date existe déjà !\n");
            return bc;
        }
        pB = pB->suivant;
    }
    T_Block *tempB = creerBloc(bc->idBloc+1, date); //créer un nouveau bloc « tempB »
    tempB->suivant = bc; //ajouter le nouveau bloc
    //bc = tempB; //faire de « tempB » la tête de BlockChain
    return tempB;
}


/* ********************************
 * 3.	Calcul de la somme des EATCoin crédités et dépensés par un étudiant sur une journée :
 ******************************** */
float totalTransactionEtudiantBlock(int idEtu, T_Block b){
    float total = 0;
    T_Transaction *pT = b.listeTransactions; //« pT » pointe sur la tête de listeTransactions
    while (pT != NULL){
        if (pT->idEtu == idEtu) {
                total += pT->montant;
        }
        pT = pT->suivant;
    }
    return total;
}


/* ********************************
 * 4.	Calcul du solde total d'un étudiant:
 ******************************** */
float soldeEtudiant(int idEtu, BlockChain bc){
    float solde = 0;
    T_Block *pB = bc; //« pB » pointe sur la tête de BlockChain
    while (pB != NULL){
        solde += totalTransactionEtudiantBlock(idEtu, *pB);
        pB = pB->suivant;
    }
    return solde;
}


/* ********************************
 * 5.	Rechargement du compte d’un étudiant :
 ******************************** */
void crediter(int idEtu, float montant, char *descr, BlockChain bc){
    T_Block *p = bc;//确定指针指向该天,且不改变
    p->listeTransactions = ajouterTransaction(idEtu, montant, descr, bc->listeTransactions);
//??????怎么确定是该天的 //应该是我们只会在最新的那一天交易，而不会在之前的日期这么做，即只会在BlockChain的头节点更新数据
}




T_Transaction *ajouterTransaction(int idEtu, float montant, char *descr, T_Transaction *listeTransaction) {
    T_Transaction *tempT = creerTransaction(idEtu, montant, descr);
    tempT->suivant = listeTransaction;//ajouter le nouveau transaction
    listeTransaction = tempT;
    //printf("rtudiant %d montant %.2f \n",idEtu, montant)
    return NULL;
}














/* ********************************
 * 6.	Paiement d'un repas :
 ******************************** */
int payer(int idEtu, float montant, char *descr, BlockChain bc){
    ajouterTransaction(idEtu, montant, descr, bc->listeTransactions);
    return 1;
}


/* ********************************
 * 7.	Historique d’un étudiant :
 ******************************** */
void consulter(int idEtu, BlockChain bc){
    BlockChain pB = bc;
    T_Transaction *pT = bc->listeTransactions;
    while(pB != NULL){
        while(pT != NULL && pT->idEtu == idEtu){
            printf("%d", idEtu); //imprimer l'identité d'étudiant
            printf("%s", pB->dateBloc); //imprimer la date
            printf("%f", pT->montant); //imprimer le montant
            print("%s", pT->description); //imprimer la description
            pT = pT->suivant;
        }
    pB = pB->suivant;
    pT = pB->listeTransactions;
    }
}


/* ********************************
 * 8.	Transfert de EATCoins entre deux étudiants :
 ******************************** */
int transfert(int idSource, int idDestination, float montant, char *descr, BlockChain bc){
    ajouterTransaction(idSource, -montant, descr, bc->listeTransactions);
    ajouterTransaction(idDestination, montant, descr, bc->listeTransactions);
    return 1;
}







/* *********************
 * Fonctions utilitaires
 ********************* */
// Vider le buffer (utile quand on utlise des getchar() )
void viderBuffer() {
    int c = 0;
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
}
