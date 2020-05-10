#include <stdio.h>
#include <stdlib.h>
#include <libTableauNoir.h>
#include <math.h>

int main(){
  //EtatSourisClavier esc = lireEtatSourisClavier();
  int rouge,vert,bleu;
  int a =-10;
  int b =60;
  creerTableau();
  fixerModeBufferisation(1);
  tableauPleinEcran();
  Image I = chargerImage("MarioKart8.png");
  afficherImage(I,0-tn_largeur(I)/2,0+tn_hauteur(I)/2);
  choisirTypeStylo(10,0,0,0);
  //tracerPoint(a,b);
  chargerPixelsEnMemoire();
  lirePixel(a,b,&rouge,&vert,&bleu);
  printf("lirePixel(%d,%d,%d,%d,%d);\n",a,b,rouge,vert,bleu);
  //attendreTouche();
  //printf("%lf\n",fabs(b));
  printf("%d\n",attendreTouche());
  //printf ("debut de boucle\n") ;
  //esc = lireEtatSourisClavier();
  //while(a<20){
    //printf("%d\n",attendreTouche());
    //printf ("test %d\n", esc.touchesClavier[44]);
  //	a++ ;
  //	sleep (1) ;
  //}
  return EXIT_SUCCESS;
}
