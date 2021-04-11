#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "exemple.h"
#define true 1
#define false 0


int est_un_symbole(int _x) {
  return _x < 0;
}

void affiche_formule(int _formule, int (*_ensemble_de_formules)[3] ) {
  if (est_un_symbole(_formule)) {
    affiche_symbole(_formule);
    return;
  }
  switch(_ensemble_de_formules[_formule][0]) {
  case FAIT :
    affiche_symbole(_ensemble_de_formules[_formule][1]);
    break;
  case NEGATION :
    printf("~");
    affiche_formule(_ensemble_de_formules[_formule][1], _ensemble_de_formules);
    break;
  case ET :
    printf("(");
    affiche_formule(_ensemble_de_formules[_formule][1], _ensemble_de_formules);
    printf("&");
    affiche_formule(_ensemble_de_formules[_formule][2], _ensemble_de_formules);
    printf(")");
    break;
  case OU :
    printf("(");
    affiche_formule(_ensemble_de_formules[_formule][1], _ensemble_de_formules);
    printf("|");
    affiche_formule(_ensemble_de_formules[_formule][2], _ensemble_de_formules);
    printf(")");
    break;
  case IMP :
    printf("(");
    affiche_formule(_ensemble_de_formules[_formule][1], _ensemble_de_formules);
    printf("->");
    affiche_formule(_ensemble_de_formules[_formule][2], _ensemble_de_formules);
    printf(")");
    break;
  case EQU :
    printf("(");
    affiche_formule(_ensemble_de_formules[_formule][1], _ensemble_de_formules);
    printf("<->");
    affiche_formule(_ensemble_de_formules[_formule][2], _ensemble_de_formules);
    printf(")");
    break;
  }
}

void affiche_consequence()
{
  printf("{");
  for (int k=1; k < NB_FORMULES; k++) {
    affiche_formule(k, ensemble_de_formules);
    if (k+1 < NB_FORMULES) printf(",");
  }
  printf("} |= ");
  affiche_formule(0, ensemble_de_formules);
  printf("\n");
}

int verifie_formule(int _formule, int (*_ensemble_de_formules)[3] , int *tableau) {
  if(est_un_symbole(_formule)) {
  	int nbr = fabs(_formule)-1;//prend la valeur absolue
    return tableau[nbr];
  }
  switch(_ensemble_de_formules[_formule][0]) {
	  case FAIT :
	  	if(est_un_symbole(_ensemble_de_formules[_formule][1])){
	  		int nbr = fabs(_ensemble_de_formules[_formule][1])-1;  	//prend la valeur absolue
	    	return tableau[nbr];
	  	}
	    break;
	  case NEGATION :{
	    int valeurNegative = (verifie_formule(_ensemble_de_formules[_formule][1], _ensemble_de_formules, tableau));
	    return !valeurNegative;
		}
	    break;
	  case ET :{
	    int valeurEt1 = verifie_formule(_ensemble_de_formules[_formule][1], _ensemble_de_formules, tableau);
	    int valeurEt2 = verifie_formule(_ensemble_de_formules[_formule][2], _ensemble_de_formules, tableau);
	    return valeurEt1 && valeurEt2;
		}
	    break;
	  case OU :{
	    int valeurOu1 = verifie_formule(_ensemble_de_formules[_formule][1], _ensemble_de_formules, tableau);
	    int valeurOu2 = verifie_formule(_ensemble_de_formules[_formule][2], _ensemble_de_formules, tableau);
	    return valeurOu1 || valeurOu2;
		}
	    break;
	  case IMP :{
	    int valeurImp1 = verifie_formule(_ensemble_de_formules[_formule][1], _ensemble_de_formules, tableau);
	    int valeurImp2 = verifie_formule(_ensemble_de_formules[_formule][2], _ensemble_de_formules, tableau);
	    return !(valeurImp1) || valeurImp2;
		}
	    break;
	  case EQU :{
	    int valeurEqu1 = verifie_formule(_ensemble_de_formules[_formule][1], _ensemble_de_formules, tableau);
	    int valeurEqu2 = verifie_formule(_ensemble_de_formules[_formule][2], _ensemble_de_formules, tableau);
		return valeurEqu1 == valeurEqu2;
		}
	    break;
	}
	return 0;
}

int parcours_les_formules(int *tableau){
	int valeurBooleanFormule;
 	for(int k=1 ; k < NB_FORMULES; k++) {
	    valeurBooleanFormule = verifie_formule(k, ensemble_de_formules, tableau);
	    if(valeurBooleanFormule == false)//si ma formule est fausse je retourne faux
	    	return false;
	}
    return true;
}

void InverseTableau(int *T, int N){
	int i, j, temp;
	for (i = 0, j = N-1 ; i < j ; i++, j--)
	{
		temp = T[i];
		T[i] = T[j];
		T[j] = temp;
	}
}

void boucle_possibilite(){
	int tab[NB_SYMBOLES];
	int resultat = 0;
	int compteur = 0;
	for(int n = 0; n < pow(2, NB_SYMBOLES); ++n){							
		///remplie le tableau de 0
	  	for(int i = 0 ; i < NB_SYMBOLES; i++)
	  		tab[i] = 0;
	  	int nbr = n;
	  	//convertie mon nombre en binaire
	    for(int i = 0 ; nbr > 0 ; i++){
	  		tab[i] = nbr%2;
	  		nbr = nbr/2;
	    }
	    //j'inverse car la méthode pour convertir en binaire doit ce lire de la fin vers le début
	    InverseTableau(tab, NB_SYMBOLES);
	    parcours_les_formules(tab);
	   	if(parcours_les_formules(tab) == false){//si une de mes formule de gauche est fausse je passe a l'itération suivante
	   		compteur++;
	   		if(compteur == pow(2, NB_SYMBOLES))
	   			resultat = 1;
	   		continue;
	   }
	    else{
	    	int valeurBooleanFormule = verifie_formule(0, ensemble_de_formules, tab);//teste ma formule de droite si toutes met formule de gauche sont vrai
	    	if(valeurBooleanFormule == true)
	    		resultat = 1;
	    	else{
	    		resultat = 0;//stop ma boucle si ma formule de droite est fausse quand mes formule de gauche sont vrai
	    		break;
	    	}
	    }
	}
		printf("conséquance : %d \n", resultat);
}


int main(int argc, char **argv){
  	affiche_consequence();//affiche la consequence
	boucle_possibilite();//retourne 1(vrai) s'il y a conséquence sémantique
	return 0;
}