#include <stdio.h>
#include <stdlib.h>
#include "TP04.h"

int main()
{
/*测试添加liste
//***************************************************************
    T_Element *liste = NULL;


    liste = ajouterInscription(liste, "CSE101");
    liste = ajouterInscription(liste, "MAT201");
    liste = ajouterInscription(liste, "PHY301");
    liste = ajouterInscription(liste, "CSE101");  // 重复的 UV

    // 打印列表内容
    T_Element *courant = liste;
    while (courant != NULL) {
        printf("%s ", courant->code_uv);
        courant = courant->suivant;
    }

    // 释放列表内存
    courant = liste;
    while (courant != NULL) {
        T_Element *temp = courant;
        courant = courant->suivant;
        free(temp);
    }
//*********************************************************************/
   T_Arbre abr = NULL; // 初始化一个空树

    // 测试1：添加学生及其注册
    abr = inscrire(abr, "Doe", "John", "CSE101");
    //abr = inscrire(abr, "Smith", "Jane", "MAT201");
    abr = inscrire(abr, "Doe", "John", "PHY301");
    abr = inscrire(abr, "Johnson", "Alice", "CSE101");

    // 输出学生信息
    T_Noeud *etudiant = chercher_Etu(abr, "Doe", "John");
    if (etudiant != NULL) {
        printf("Nom: %s\n", etudiant->nom);
        printf("Prenom: %s\n", etudiant->prenom);
        printf("Inscriptions:\n%s  %s",etudiant->listeInscriptions->code_uv,etudiant->listeInscriptions->code_uv);

    }


    return 0;

}
