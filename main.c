#include <stdio.h>
#include <stdlib.h>
#include "tp3.h"

int main(){

    BlockChain ma_chaine_de_blocs = NULL;

    // ============= MENU UTILISATEUR ============= */
    char choix = '0';
    while (choix != '8'){
        printf("\n======================================");
        printf("\n1. Afficher la liste des blocs de la BlockChain");
        printf("\n2. Afficher toutes les transactions d'un bloc");
        printf("\n3. Afficher toutes les transactions du jour pour un etudiant");
        printf("\n4. Afficher l'historique pour un etudiant");
        printf("\n5. Crediter un compte");
        printf("\n6. Payer un repas");
        printf("\n7. Transferer des EATCoins entre deux etudiants");
        printf("\n8. Quitter");
        printf("\n======================================");
        printf("\n   Votre choix ? ");

        choix = getchar();

        switch (choix) {
            case '1' :
            {
                    printf("***** Affichage de la blockchain *****\n\n");

                    T_Block* p = ma_chaine_de_blocs;
                    T_Transaction* pp = p->listeTransactions;
                    while(p!= NULL)
                    {
                        printf("numero block = %d   date = %s\n",p->idBloc, p->dateBloc);
                        while (pp!= NULL)
                        {
                            printf("etudiant %d montant %.2f \n",pp->idEtu, pp->montant);
                            pp = pp->suivant;
                        }
                        printf("\n");
                        p = p->suivant;
                    }
                    printf("\n***** Fin affichage *****");

                break;
            }

            case '2' :
            {
                    char date[12];
                    printf("vous voudrez voir bloc de quel jour? (20231009 pour le 09/10/2020)");
                    scanf("%s",date);
                    T_Block* p = ma_chaine_de_blocs;
                    int count_block = 0;//verifier si le block existe;
                    while ( p != NULL )
                    {
                        if ( strcmp(date ,p->dateBloc)==0 )
                        {
                            printf("numero block = %d   date = %s\n",p->idBloc, p->dateBloc);\
                            T_Transaction* pp = p->listeTransactions;
                             while (pp != NULL)
                            {
                                printf("etudiant %d montant %.2f \n",pp->idEtu, pp->montant);
                                pp = pp->suivant;
                            }
                            count_block++;
                        }


                        p = p->suivant;
                    }

                    if (count_block == 0)
                    {
                        printf("le jour %s n'a pas ete trouve ", date);
                    }
                    break;
            }

            case '3' :
            {
                    char date[12];
                    printf("vous voudrez voir bloc de quel jour? (20231009 pour le 09/10/2020)");
                    scanf("%s",date);
                    int count_block = 0;//verifier si le block existe;
                    int count_etu = 0;//verifier si l'etu existe;
                    int id;
                    printf("quel est le ID numbre d'etudiant que vous voudrez voir ? ");
                    scanf("%d",&id);
                    T_Block* p = ma_chaine_de_blocs;

                    while ( p != NULL)
                    {
                        if ( strcmp(date ,p->dateBloc)==0 )
                        {
                            printf("numero block = %d   date = %s\n",p->idBloc, p->dateBloc);
                            T_Transaction* pp = p->listeTransactions;
                            while ( pp != NULL)
                            {
                                if ( pp->idEtu == id )
                                {
                                    printf("etudiant %d montant %.2f \n",pp->idEtu, pp->montant);
                                    count_etu++;
                                }

                                pp = pp->suivant;

                            }
                            printf("\n");
                            count_block++;

                        }
                        p = p->suivant;
                    }

                    if(count_block == 0)
                        printf("le jour %s n'a pas ete trouve", date);
                    if(count_etu == 0)
                        printf("le etudiant (ID==%d) n'a pas ete trouve", id);
                    break;
            }

            case '4' :
            {
                    int id;
                    printf("quel est le ID numbre d'etudiant que vous voudrez voir ? ");
                    scanf("%d",&id);
                    int count_etu = 0;
                    T_Block* p = ma_chaine_de_blocs;
                    while ( p != NULL)
                    {
                        printf("numero block = %d   date = %s\n",p->idBloc, p->dateBloc);
                        T_Transaction* pp = p->listeTransactions;
                         while ( pp != NULL )
                        {
                            if ( pp->idEtu == id )
                            {
                                printf("etudiant %d montant %.2f \n",pp->idEtu, pp->montant);
                                pp = pp->suivant;
                                count_etu++;
                            }

                        }
                        printf("\n");
                        p = p->suivant;
                    }
                    if(count_etu == 0)
                        printf("le etudiant (ID==%d) n'a pas ete trouve", id);
                    break;
            }

            case '5' :
            {
                    int id;
                    float montant;
                    char descr[20];

                    printf("Entrez ID de l'étudiant : ");
                    scanf("%d", &id);
                    printf("Entrez le montant à créditer : ");
                    scanf("%f", &montant);
                    printf("Entrez une description : ");
                    viderBuffer(); // 清空输入缓冲区
                    fgets(descr, sizeof(descr), stdin);

                    // 调用 crediter 函数来增加学生的余额
                    crediter(id, montant, descr, ma_chaine_de_blocs);

                    printf("Crédit ajouté avec succès.\n");
                    break;
            }

            case '6' ://n6. Payer un repas
            {
                break;
            }

            case '7' :
            {
                    break;
            }

            case '8' :
            {
                printf("\n======== PROGRAMME TERMINE ========\n");
                break;
            }

            default : printf("\n\nERREUR : votre choix n'est valide ! ");
        }
        printf("\n\n\n");
        viderBuffer(); // supprimer de l'entrée standard le retour charriot et les éventuels caractères supplémentaires tapés par l'utilisateur
    }

    return 0;
}
