#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "TP04.h"


T_Element * creerInscription( char * code) {

    T_Element * newinscription = malloc(sizeof(T_Element));

    if (newinscription != NULL) {
        // l'allocation memoire s'est bien passee
        // Important : malloc permet d'avoir l'espace memoire dedie pour notre champ de structure
        newinscription->code_uv = malloc(strlen(code) + 1);
        strcpy(newinscription->code_uv, code);

        newinscription->suivant = NULL;
    }

    return newinscription;
}

//ajouter un élément dans une liste d’inscriptions à des UV
T_Element * ajouterInscription(T_Element *liste, char* code){
    T_Element *ajout = creerInscription(code);
    if (!ajout) {
        printf("Erreur d'allocation de mémoire.\n");
        return NULL;
    }

    T_Element *tete = liste;

    if (tete == NULL || strcmp(tete->code_uv, code) > 0) {
        ajout->suivant = tete;
        return ajout;  // changer le point tete;
    }

    int compare = strcmp(tete->code_uv, code);

    while (tete->suivant != NULL && compare < 0) {
        tete = tete->suivant;
        compare = strcmp(tete->code_uv, code);
    }

    if (compare == 0) {
        printf("UV %s deja existe!\n" , tete->code_uv );
        free(ajout->code_uv);
        free(ajout);
    } else {
        ajout->suivant = tete->suivant;
        tete->suivant = ajout;
    }

    return liste;
}
/*----------------------------------------------------------------------------------------------------*/

int comparer_Etu ( T_Noeud* etu , char* nom, char* prenom)
{
    int prenomcmp = strcmp( etu->prenom , prenom) ;
    if (prenomcmp == 0)
        return strcmp(etu->nom , nom);
    return prenomcmp;//return 1:ajout est avant ; -1:apres
}

/*----------------------------------------------------------------------------------------------------*/

T_Noeud * chercher_Etu(T_Arbre abr, char* nom, char* prenom)
{
    if (abr == NULL || comparer_Etu(abr , nom , prenom) == 0)
        return abr;

    if ( comparer_Etu(abr , nom , prenom) < 0 )
        return chercher_Etu(abr->filsDroit , nom , prenom);
    else
        return chercher_Etu(abr->filsGauche , nom , prenom);
}

/*----------------------------------------------------------------------------------------------------*/

T_Noeud * creerEtu(char * prenom , char * nom , char * code){
    T_Noeud * ajout_etu = (T_Noeud*)malloc(sizeof(T_Noeud));

    if(ajout_etu != NULL){
        ajout_etu->nom = malloc(strlen(nom) + 1);
        strcpy( ajout_etu->nom , nom );

        ajout_etu->prenom = malloc(strlen(prenom) + 1);
        strcpy( ajout_etu->prenom , prenom );

        ajout_etu->listeInscriptions = creerInscription(code);

        ajout_etu->filsDroit = NULL;
        ajout_etu->filsGauche = NULL;
    }

    return ajout_etu;
}

T_Arbre ajouterEtuRec(T_Noeud* arbre, T_Noeud* ajoutetu) {
    if (arbre == NULL) {
        return ajoutetu;
    }

    int compare = comparer_Etu(arbre, ajoutetu->nom, ajoutetu->prenom);

    if (compare == 0) {
        printf("L'etudiant existe deja!!!\n");
        return arbre;
    }

    if (compare > 0) {
        ajoutetu->filsGauche = ajouterEtuRec(arbre->filsGauche, ajoutetu);
    } else {
        ajoutetu->filsDroit = ajouterEtuRec(arbre->filsDroit, ajoutetu);
    }

    return arbre;
}

//inscrire un étudiant à une UV
T_Arbre inscrire(T_Arbre abr, char* nom, char* prenom, char* code)
{
    T_Noeud * etu = chercher_Etu(abr , nom , prenom); //pour chercher l'etudiant

    if (etu == NULL) //l’étudiant n’est pas encoreprésent dans l’ABR
    {
        //comparer le position et le etu_ajout
        etu = creerEtu(prenom , nom ,code);

        abr = ajouterEtuRec(abr , etu);
    }
    else //l’étudiant est dans l’ABR
    {
        etu->listeInscriptions = ajouterInscription(etu->listeInscriptions , code);
    }
    return abr ;
}

/*----------------------------------------------------------------------------------------------------*/

//charger dans l’ABR un fichier texte comportant une liste d’inscriptions
T_Arbre chargerFichier(T_Arbre abr, char *filename){
    FILE * fichier = fopen(filename , "r");

    char nom[20] , prenom[20] , code[10];
    int nbbloc , nbligne;
    if(fichier == NULL){
        perror("Erreur d'ouvrir\n");
        return abr;
    }

    char ligne[100];
    nbligne = 1;
    while(fgets(ligne , sizeof(ligne) , fichier) != NULL){

        nbbloc = sscanf(ligne , "«%[^;];%[^;];%[^»]" , prenom , nom , code);
        if(nbbloc == 3){
            T_Noeud * ajoutetu = creerEtu(prenom , nom ,code);
            //检查这个文档里要添加的同学是否存在
            if(chercher_Etu(abr, nom , prenom) == NULL){//该同学不存在
                ajouterEtuRec(abr, ajoutetu);
            }
            else{//该同学存在，所以只更新uvliste
                T_Noeud * existe_etu = chercher_Etu( abr , nom , prenom);
                ajouterInscription( existe_etu->listeInscriptions , code);
            }
        }
        else{
            printf("le %d eme ligne n'est pas valide!\n" , nbligne);
        }
        nbligne++;


    }
    return abr;
}






/*----------------------------------------------------------------------------------------------------*/

//afficher la liste d'un étudiant « etu »
void afficherEtuInfo(T_Noeud* etu)
{
    if(etu) //si le nœud de T_Noeud « etu » n'est pas vide
    {
        T_Element* UV = etu->listeInscriptions;
        printf("%s %s : ", etu->nom, etu->prenom);
        while(UV) //tant que le nœud de T_Element « UV » n'est pas vide
        {
            printf("%s ", UV->code_uv);
            UV = UV->suivant;
        }
    }
}

/*----------------------------------------------------------------------------------------------------*/

//afficher la liste de tous les étudiants, classée par ordre alphabétique
void afficherInscriptions(T_Arbre abr)
{
    if(abr) //si le nœud de T_Arbre « abr » n'est pas vide
    {
        afficherInscriptions(abr->filsGauche);
        afficherEtuInfo(abr);
        afficherInscriptions(abr->filsDroit);
    }
}

/*----------------------------------------------------------------------------------------------------*/

//déterminer si un étudiant « etu » est inscrit à un UV de code « code »
int rechercherUVdEtus(T_Noeud* etu, char* code)
{
    if(etu) //si le nœud de T_Noeud « etu » n'est pas vide
    {
        T_Element *UV = etu->listeInscriptions;
        while(UV) //tant que le nœud de T_Element « UV » n'est pas vide
        {
            if(!strcmp(UV->code_uv, code)) //si les chaînes de caractères « UV->code_uv » et « code » sont identiques
            {
                return 1;
            }
            UV = UV->suivant;
        }
    }
    return 0;
}

/*----------------------------------------------------------------------------------------------------*/

//afficher l’ensemble des étudiants inscrits à une UV de code « code »
void afficherInscriptionsUV(T_Arbre abr, char* code)
{
    if(abr) //si  le nœud de T_Arbre « abr » n'est pas vide
    {
        int compteur = 0;
        afficherInscriptionsUV(abr->filsGauche, code);
        if(rechercherUVdEtus(abr, code))
        {
            compteur += 1;
            printf("%s %s    ", abr->nom, abr->prenom);
            if(!(compteur %5)) //si on a imprimé cinq étudiants dans cette ligne
            {
                compteur = 0;
                printf("\n");
            }
        }
        if(compteur) //si « compteur » n'est pas vide
        {
            printf("\n");
        }
        afficherInscriptionsUV(abr->filsDroit, code);
    }
}



/*----------------------------------------------------------------------------------------------------*/

//renvoyer le nœud d'un étudiant
T_Noeud* obtenirEtu(T_Arbre abr, char* nom, char* prenom) ///////////////////////对比chercher_Etu
{
    if(abr) //si le nœud de T_Arbre « abr » n'est pas vide
    {
        obtenirEtu(abr->filsGauche, nom, prenom);
        if(!(abs(strcmp(abr->nom, nom))+abs(strcmp(abr->prenom, prenom))))
        {
            return abr;
        }
        obtenirEtu(abr->filsDroit, nom, prenom);
    }
    return NULL;
}

/*----------------------------------------------------------------------------------------------------*/

//renvoyer le nœud d'un étudiant minimum d'un sous-arbre
T_Noeud* obtenirMinimumEtu(T_Noeud* etu)
{
    if(!etu) //si le nœud de T_Noeud « etu » n'est pas vide
    {
        while(etu->filsGauche)
        {
            etu = etu->filsGauche;
        }
    }
    return etu;
}

/*----------------------------------------------------------------------------------------------------*/

//renvoyer le nœud de parent d'un étudiant
T_Noeud* obtenirParent(T_Arbre abr, char* nom, char* prenom)
{
    if(!abr) //si le nœud de T_Arbre « abr » est vide
    {
        return NULL;
    }
    //Vérification du nœud racine
    int jugementNomGauche = strcmp(abr->filsGauche->nom, nom);
    int jugementPrenomGauche = strcmp(abr->filsGauche->prenom, prenom);
    int jugementNomDroit = strcmp(abr->filsDroit->nom, nom);
    int jugementPrenomDroit = strcmp(abr->filsDroit->prenom, prenom);
    if(((jugementNomGauche == 0) && (jugementPrenomGauche == 0)) || ((jugementNomDroit == 0) && (jugementPrenomDroit == 0)))
    {
        return abr;
    }
    //Vérification récursive des sous-arbres de gauche et de droite
    T_Noeud* resultatGauche = obtenirParent(abr->filsGauche, nom, prenom);
    T_Noeud* resultatDroit = obtenirParent(abr->filsDroit, nom, prenom);
    // Renvoi du résultat des recherches dans les sous-arbres de gauche et de droite
    return (resultatGauche) ? resultatGauche : resultatDroit;
}

/*----------------------------------------------------------------------------------------------------*/

//renvoyer le successeur d'un étudiant
T_Noeud* obtenirSuccesseur(T_Arbre abr, T_Noeud* etu)
{
    if(etu->filsDroit) //si le nœud de T_Noeud « etu » possède un fils droit
    {
        return obtenirMinimumEtu(etu->filsDroit);
    }
    T_Noeud* parent = obtenirParent(abr, etu->nom, etu->prenom);
    while(parent && (etu == parent->filsDroit))
    {
        etu = parent;
        parent = obtenirParent(abr, parent->nom, parent->prenom);
    }
    return parent;
}

/*----------------------------------------------------------------------------------------------------*/

//supprimer l’inscription d’un étudiant à une UV
T_Arbre supprimerInscription(T_Arbre abr, char* nom, char* prenom, char* code)
{
    if(abr) //si le nœud de T_Arbre « abr » n'est pas vide
    {

        T_Noeud* etu = obtenirEtu(abr, nom, prenom);
        if(etu) //si le nœud de T_Noeud « etu » n'est pas vide
        {
            T_Element* UV = etu->listeInscriptions;
            if(UV) //si le nœud de T_Element « UV » n'est pas vide
            {
                while(UV) //si  le nœud de T_Element « UV » n'est pas vide
                {
                    if(!strcmp(UV->code_uv, code)) //si  les chaînes de caractères « UV->code_uv » et « code » sont identiques
                    {
                        free(UV);
                        return abr;
                    }
                    UV = UV->suivant;
                }
            }
            else //si cet étudiant n'a plus aucune inscription
            {
                T_Noeud* parent = obtenirParent(abr, etu->nom, etu->prenom);
                int position = -1; //-1 : « etu » n'a pas de parent //0 : « etu » est fils gauche de « parent » //1 : « etu » est fils droit de « parent »
                if(!parent) //si le nœud de T_Noeud « etu » possède un parent « parent »
                {
                     position = (parent->filsGauche == etu) ? 0 : 1;
                }
                if(!(etu->filsGauche || etu->filsDroit)) //si le nœud de T_Noeud « etu » n'a pas de fils
                {
                    if(parent->filsGauche == etu)
                    {
                        parent->filsGauche = NULL;
                    }
                    else
                    {
                        parent->filsDroit = NULL;
                    }
                    free(etu);
                }
                else if((etu->filsGauche && !etu->filsDroit) || (!etu->filsGauche && etu->filsDroit)) //si le nœud de T_Noeud « etu » n'a qu'un seul fils
                {
                    T_Noeud* fils = (etu->filsGauche) ? etu->filsGauche : etu->filsDroit;
                    if(position) //si « etu » est fils gauche de « parent »
                    {
                        parent->filsGauche = fils;
                    }
                    else //si « etu » est fils droit de « parent »
                    {
                        parent->filsDroit = fils;
                    }
                    free(etu);
                }
                else //si le nœud de T_Noeud « etu » possède deux fils
                {
                    T_Noeud* successeur = obtenirSuccesseur(abr, etu);
                    T_Noeud* parent_successeur = obtenirParent(abr, successeur->nom, successeur->prenom);
                    if(parent_successeur->filsGauche == successeur)
                    {
                        parent_successeur->filsGauche = successeur->filsDroit;
                    }
                    else
                    {
                        parent_successeur->filsDroit = successeur->filsDroit;
                    }
                    etu->nom = successeur->nom;
                    etu->prenom = successeur->prenom;
                    etu->listeInscriptions = successeur->listeInscriptions;
                    free(successeur);
                }
            }
        }
    }
    return abr;
}

/*----------------------------------------------------------------------------------------------------*/

