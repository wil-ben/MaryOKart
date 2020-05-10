#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <libTableauNoir.h>
#include <time.h>

/********************/
/*Defintion de types*/
/********************/

typedef struct {double x ;double y;}coord_t;

typedef struct {coord_t p ; coord_t v;}mobile_t;

typedef struct {mobile_t k; double m;double n;}kart_t;

typedef struct {tStylo kart1;tStylo kart2;tStylo ligne;tStylo cube;}stylos_t;

typedef struct {Image image_tab[100];Image im_tab[100];Image im_k[100];Image im_m;}image_t;

typedef struct {Son son_tab[100];Son s_tab[100];PisteAudio p[100];Son s_k1;Son s_k2;}son_t;

typedef struct {int c1;int c2;int d1;int d2;int f1;int f2;int s1;int s2;int a;int b;int n;int l;int g;int h;int i1;int i2;int j1;int j2;int cab;int map;int u;int v;int t;int w;int perso1;int perso2;int pos1;int pos2;int sc1;int sc2;}var_t;

typedef struct {kart_t kart1;kart_t kart2;mobile_t ligne;mobile_t cube1;mobile_t cube2;mobile_t rea;}modele_t;

typedef struct {modele_t modele;stylos_t stylos;image_t image;son_t son;int fini;int x[100];int y[100];FILE * f;double dt;double t;} donnees_t;

typedef struct { FILE * f;int mode;double t;double dt;double tps;double tps1;double delai1;double delai2;int t_pi[100];double c_pi1[100];double c_pi2[100];double c_pi3[100];} etat_t;


/***************************/
/*Declaration des fonctions*/
/***************************/

//initialisation
modele_t init_modele(var_t);
stylos_t init_stylos();
donnees_t init_donnees(donnees_t*,var_t);
var_t init_var(donnees_t);

//fonction de la boucle d'animation
void afficher_donnees(donnees_t,var_t);
void afficher_menu(donnees_t*,var_t*);
void afficher_map_cab(donnees_t*,var_t *);
void afficher_score_fin(donnees_t*,var_t*);
void afficher_objet(donnees_t,var_t);
void charger_image(donnees_t*);
void charger_im_kart(donnees_t*,var_t*);
void charger_im_map(donnees_t*,var_t*);
void charger_son_kart(donnees_t*,var_t*);
void charger_son(donnees_t*);
void mettre_a_jour(donnees_t *,etat_t *,var_t *);
void recup_objet(donnees_t*,etat_t*,var_t*);
void nb_tours(donnees_t*,var_t*);
void respawn(donnees_t*,etat_t*,var_t*);
int fini(donnees_t);

int main(){
  /*Declaration*/
  donnees_t donnees;
  etat_t etat;
  var_t var;
  
  /* ouverture de la fenêtre graphique avec quelques réglages */
  creerTableau();
  fixerModeBufferisation(1);
  tableauPleinEcran();
  //fixerTaille(1000,800);
  
  /* initialisation des donnees et variable et chargement des images et sons*/
  var = init_var(donnees);
  charger_image(&donnees);
  charger_son(&donnees);
  afficher_menu(&donnees,&var);//affiche le menu d'acceuil
  charger_im_map(&donnees,&var);
  donnees = init_donnees(&donnees,var);
  charger_im_kart(&donnees,&var);
  /* boucle d'animation */
  while (!fini(donnees)){
    
    /*affiche la map et le compte a rebours*/
    afficher_map_cab(&donnees,&var);
    
    /*affiche les scores et l'ecran de fin de partie*/
    afficher_score_fin(&donnees,&var);
    
    /* mettre à jour le programme*/
    mettre_a_jour(&donnees,&etat,&var);
    
    /*affiche les objets obtenus*/
    afficher_objet(donnees,var);
    
    /* créer l'image sur le buffer */
    afficher_donnees(donnees,var);
    
    /* afficher le buffer à l'écran */
    tamponner();
    
    /* effacer le buffer */
    effacerTableau();
  }
  return EXIT_SUCCESS;
}


/* initialisation des données */
donnees_t init_donnees(donnees_t *donnees,var_t var){
  donnees->fini=0;
  donnees->modele = init_modele(var);
  donnees->stylos = init_stylos();
  return *donnees;
}


//initialisation des modeles
modele_t init_modele(var_t var){
  donnees_t donnees;
  int o=0;
  if(var.map==2){
    donnees.f=fopen("coord2.c","r");
  }else if(var.map == 3 || var.map == 5){
    donnees.f=fopen("coord3.c","r");
  }else if(var.map==4){
    donnees.f=fopen("coord4.c","r");
  }else if(var.map==6){
    donnees.f=fopen("coord5.c","r");
  }else if(var.map==7){
    donnees.f=fopen("coord6.c","r");
  }else if(var.map==8){
    donnees.f =fopen("coord7.c","r");
  }else{
    donnees.f=fopen("coord.c","r");
  }
  while(!feof(donnees.f)){
    fscanf(donnees.f,"%d %d\n", &donnees.x[o],&donnees.y[o]);
    o++;
  }
  fclose(donnees.f);
  modele_t m = {kart1:{k: {p: {donnees.x[0],donnees.y[0]}, v: {donnees.x[6],donnees.y[6]}},m: 0,n :0},
		kart2:{k: {p: {donnees.x[1],donnees.y[1]}, v: {donnees.x[7],donnees.y[7]}},m: 0,n :0},
		ligne: {p: {donnees.x[2],donnees.y[2]}, v: {donnees.x[3],donnees.y[3]}},//la vitesse est utilisé comme 2ème vecteur position
		cube1: {p: {donnees.x[4],donnees.y[4]}, v: {donnees.x[8],donnees.y[8]}},
		cube2: {p: {donnees.x[5],donnees.y[5]}, v: {donnees.x[9],donnees.y[9]}},
		rea: {p: {donnees.x[10],donnees.y[10]}, v: {donnees.x[11],donnees.y[11]}}};//la vitesse est utilisé comme 2ème vecteur position
  return m;
}

//initialisation des variables
var_t init_var(donnees_t donnees){
  var_t v={c1 :0,c2 :0,d1 :0,d2 :0,f1 :0,f2 :0,s1 :0,s2 :0,a :0,b :4,n :0,l :0,g :0,h :0,i1 :donnees.modele.rea.p.x,i2 :donnees.modele.rea.p.x, j1 :donnees.modele.rea.p.y,j2 :donnees.modele.rea.p.y,cab :0,map :0,u :0,v :0,t :0,w :0,perso1 :-1,perso2 :-1,pos1 :0,pos2 :0,sc1 :0,sc2 :0};
  return v;
}

//initialisation des stylos
stylos_t init_stylos(){
  stylos_t stylos;
  etat_t etat;
  int o=0;
  etat.f = fopen("pix.c","r");
  while(!feof(etat.f)){
    fscanf(etat.f,"%d %lf %lf %lf\n",&etat.t_pi[o],&etat.c_pi1[o],&etat.c_pi2[o],&etat.c_pi3[o]);
    o++;
  }
  fclose(etat.f);
  choisirTypeStylo(etat.t_pi[0],etat.c_pi1[0],etat.c_pi2[0],etat.c_pi3[0]); stylos.kart1 = stockerStylo();
  choisirTypeStylo(etat.t_pi[1],etat.c_pi1[1],etat.c_pi2[1],etat.c_pi3[1]); stylos.kart2 = stockerStylo();
  choisirTypeStylo(etat.t_pi[2],etat.c_pi1[2],etat.c_pi2[2],etat.c_pi3[2]); stylos.ligne = stockerStylo();
  choisirTypeStylo(etat.t_pi[3],etat.c_pi1[3],etat.c_pi2[3],etat.c_pi3[3]); stylos.cube = stockerStylo();
  return stylos;
}

//retourne 1 si le programme doit se finir, 0 sinn
int fini(donnees_t donnees){
  return donnees.fini;
}

//afficher les objets mouvants
void afficher_donnees(donnees_t donnees,var_t var){
  if(var.u == 1){
    fixerCouleurDeTransparence(donnees.image.im_k[var.pos1],0,0,0);
  }else{
  /*selectionnerStylo(donnees.stylos.kart1);
    tracerPoint(donnees.modele.kart1.k.p.x,donnees.modele.kart1.k.p.y);*/
  afficherImage(donnees.image.im_k[var.pos1],donnees.modele.kart1.k.p.x-16,donnees.modele.kart1.k.p.y+19);
  }
   if(var.v == 1){
    fixerCouleurDeTransparence(donnees.image.im_k[var.pos2],0,0,0);
  }else{
   /*selectionnerStylo(donnees.stylos.kart2);
     tracerPoint(donnees.modele.kart2.k.p.x,donnees.modele.kart2.k.p.y);*/
  afficherImage(donnees.image.im_k[var.pos2],donnees.modele.kart2.k.p.x-16,donnees.modele.kart2.k.p.y+19);
   }
  selectionnerStylo(donnees.stylos.ligne);
  fixerOpacite(255);
  tracerRectangle(donnees.modele.ligne.p.x,donnees.modele.ligne.p.y,donnees.modele.ligne.v.x,donnees.modele.ligne.v.y);
  selectionnerStylo(donnees.stylos.cube);
  if(var.c1==0){
    fixerOpacite(150);
    tracerPoint(donnees.modele.cube1.p.x,donnees.modele.cube1.p.y);
  }
  if(var.c2==0){
    fixerOpacite(150);  
    tracerPoint(donnees.modele.cube2.p.x,donnees.modele.cube2.p.y);
  }
}

//mettre à jour l'etat du programme
void mettre_a_jour(donnees_t *donnees,etat_t *etat,var_t *var){
  EtatSourisClavier esc = lireEtatSourisClavier();
  donnees->dt=delta_temps();
  int o=0,i=0;
  
  etat->f = fopen("maj.c","r");
  while(!feof(etat->f)){
    fscanf(etat->f,"%d %lf %lf %lf\n",&etat->t_pi[o],&etat->c_pi1[o],&etat->c_pi2[o],&etat->c_pi3[o]);
    o++;
  }
  fclose(etat->f);
  if(var->map==3 || var->map==5 || var->map==6){
    donnees->f = fopen("colors2.c","r");
  }else if(var->map==4){
    donnees->f = fopen("colors3.c","r");
  }else if(var->map==7 || var->map==8){
    donnees->f=fopen("colors4.c","r");
  }else{
    donnees->f = fopen("colors.c","r");
  }
  while(!feof(donnees->f)){
    fscanf(donnees->f,"%d %d\n",&donnees->x[i],&donnees->y[i]);
    i++;
  }
  fclose(donnees->f);
  
  //Les kart reviennent au chekpoint lorsqu'ils sortent de la route
  respawn(donnees,etat,var);
  //Compte le nombre de tour
  nb_tours(donnees,var);
  //gain aleatoire des objets lorsqu'un kart prend un cube
  recup_objet(donnees,etat,var);
  
  //delacement kart 1
  if(var->d1==1||var->f1==1 || var->s1==1){
    if(esc.touchesClavier[44]==1){//touche espace
      etat->t=0;
      etat->mode=1;//lorsque que etat->mode vaut 1 cela signifie que le kart1 a activer l'avantage
      if(var->d1==1){
	donnees->modele.kart2.k.v.x=etat->c_pi1[0];/*ralenti le*/
	donnees->modele.kart2.k.v.y=etat->c_pi1[0];/*  kart 2  */
      }else if(var->f1==1){
	donnees->modele.kart1.k.v.x=etat->c_pi3[0];/*boost le  */
	donnees->modele.kart1.k.v.y=etat->c_pi3[0];/*   kart 1 */
      }else{
	var->v=1;
	donnees->son.p[7]=jouerSon(donnees->son.s_tab[7],0);
	volumePiste(donnees->son.p[7],1);
      }
    }
  }
  if(esc.touchesClavier[4]==1){//touche Q
    if(donnees->modele.kart1.k.v.x!=etat->c_pi1[0]&&donnees->modele.kart1.k.v.x!=etat->c_pi3[0]&&donnees->modele.kart1.k.v.x!=0.001){
      donnees->modele.kart1.k.v.x = etat->c_pi2[0];
    }
    donnees->son.p[4]=jouerSon(donnees->son.s_tab[4],0);
    volumePiste(donnees->son.p[4],0.1);
    donnees->modele.kart1.k.p.x = donnees->modele.kart1.k.p.x - donnees->modele.kart1.k.v.x;
    var->pos1=3;
  }
  if(esc.touchesClavier[7]==1){//touche D
    if(donnees->modele.kart1.k.v.x!=etat->c_pi1[0]&&donnees->modele.kart1.k.v.x!=etat->c_pi3[0]&&donnees->modele.kart1.k.v.x!=0.001){
      donnees->modele.kart1.k.v.x = etat->c_pi2[0];
    }
    donnees->son.p[4]=jouerSon(donnees->son.s_tab[4],0);
    volumePiste(donnees->son.p[4],0.1);
    donnees->modele.kart1.k.p.x = donnees->modele.kart1.k.p.x + donnees->modele.kart1.k.v.x;
    var->pos1=2;
  }
  if(esc.touchesClavier[26]==1){//touche Z
    if(donnees->modele.kart1.k.v.y!=etat->c_pi1[0]&&donnees->modele.kart1.k.v.x!=etat->c_pi3[0]&&donnees->modele.kart1.k.v.x!=0.001){
      donnees->modele.kart1.k.v.y = etat->c_pi2[0];
    }
    donnees->son.p[4]=jouerSon(donnees->son.s_tab[4],0);
    volumePiste(donnees->son.p[4],0.1);
    donnees->modele.kart1.k.p.y = donnees->modele.kart1.k.p.y+donnees->modele.kart1.k.v.y;
    var->pos1=0;
    var->n=0;
  }
  if(esc.touchesClavier[22]==1){//touche S
    if(donnees->modele.kart1.k.v.y!=etat->c_pi1[0]&&donnees->modele.kart1.k.v.x!=etat->c_pi3[0]&&donnees->modele.kart1.k.v.x!=0.001){
      donnees->modele.kart1.k.v.y = etat->c_pi2[0];
    }
    donnees->son.p[4]=jouerSon(donnees->son.s_tab[4],0);
    volumePiste(donnees->son.p[4],0.1);
    donnees->modele.kart1.k.p.y = donnees->modele.kart1.k.p.y-donnees->modele.kart1.k.v.y;
    var->pos1=1;
    var->n=1;
  }
  
  //deplacement kart 2
  if(var->d2==1||var->f2==1 || var->s2 == 1){
    if(esc.touchesClavier[100]==1){//touche '>'ou'<'
      etat->dt=0;
      etat->mode=2;//lorsque que etat->mode vaut 2 cela signifie que le kart2 a activer l'avantage
      if(var->d2==1){
	donnees->modele.kart1.k.v.x=etat->c_pi1[0];/*ralenti le*/
	donnees->modele.kart1.k.v.y=etat->c_pi1[0];/*   kart 1 */
      }else if(var->f2==1){
	donnees->modele.kart2.k.v.x=etat->c_pi3[0];/*boost le  */
	donnees->modele.kart2.k.v.y=etat->c_pi3[0];/*   kart 2 */
      }else{
	var->u=1;
	donnees->son.p[7]=jouerSon(donnees->son.s_tab[7],0);
	volumePiste(donnees->son.p[7],1);
      }
    }
  }
  if(esc.touchesClavier[80]==1){//touche fleche de gauche
    if(donnees->modele.kart2.k.v.x!=etat->c_pi1[0]&&donnees->modele.kart2.k.v.x!=etat->c_pi3[0]&&donnees->modele.kart2.k.v.x!=0.001){
      donnees->modele.kart2.k.v.x = etat->c_pi2[0];
    }
    donnees->son.p[4]=jouerSon(donnees->son.s_tab[4],0);
    volumePiste(donnees->son.p[4],0.1);
    donnees->modele.kart2.k.p.x=donnees->modele.kart2.k.p.x-donnees->modele.kart2.k.v.x;
    var->pos2=7;
  }
  if(esc.touchesClavier[79]==1){//touche fleche de droite
    if(donnees->modele.kart2.k.v.x!=etat->c_pi1[0]&&donnees->modele.kart2.k.v.x!=etat->c_pi3[0]&&donnees->modele.kart2.k.v.x!=0.001){
      donnees->modele.kart2.k.v.x = etat->c_pi2[0];
    }
    donnees->son.p[4]=jouerSon(donnees->son.s_tab[4],0);
    volumePiste(donnees->son.p[4],0.1);
    donnees->modele.kart2.k.p.x=donnees->modele.kart2.k.p.x+donnees->modele.kart2.k.v.x;
    var->pos2=6;
  }
  
  if(esc.touchesClavier[82]==1){//touche fleche du haut
    if(donnees->modele.kart2.k.v.y!=etat->c_pi1[0]&&donnees->modele.kart2.k.v.x!=etat->c_pi3[0]&&donnees->modele.kart2.k.v.x!=0.001){
      donnees->modele.kart2.k.v.y = etat->c_pi2[0];
    }
    donnees->son.p[4]=jouerSon(donnees->son.s_tab[4],0);
    volumePiste(donnees->son.p[4],0.1);
    donnees->modele.kart2.k.p.y = donnees->modele.kart2.k.p.y+donnees->modele.kart2.k.v.y;
    var->pos2=4;
    var->l=0;
  }
  if(esc.touchesClavier[81]==1){//touche fleche du bas
    if(donnees->modele.kart2.k.v.y!=etat->c_pi1[0]&&donnees->modele.kart2.k.v.x!=etat->c_pi3[0]&&donnees->modele.kart2.k.v.x!=0.001){
      donnees->modele.kart2.k.v.y =etat->c_pi2[0];
    }
    donnees->son.p[4]=jouerSon(donnees->son.s_tab[4],0);
    volumePiste(donnees->son.p[4],0.1);
    donnees->modele.kart2.k.p.y = donnees->modele.kart2.k.p.y-donnees->modele.kart2.k.v.y;
    var->pos2=5;
    var->l=1;
  }
  
  if(etat->mode==1){
    etat->t=etat->t + donnees->dt;//etat->t contient le temps qui s'est ecoulé depuis que le ralentissement du kart2 a été activé
    if(etat->t >=etat->t_pi[0]){
      if(var->d1==1){
	donnees->modele.kart2.k.v.x=etat->c_pi2[0];
	donnees->modele.kart2.k.v.y=etat->c_pi2[0];
	var->d1=0;
	etat->t=0;
      }
      else if(var->f1==1){
	donnees->modele.kart1.k.v.x=etat->c_pi2[0];
	donnees->modele.kart1.k.v.y=etat->c_pi2[0];
	var->f1=0;
	etat->t=0;
      }else{
	var->v=0;
	var->s1=0;
	etat->t=0;
      }
      etat->mode=0;
      var->t=0;
    }
  }
  if(etat->mode==2){
    etat->dt= etat->dt+donnees->dt;//etat->dt contient le temps qui s'est ecoulé depuis que le ralentissement du kart1 a été activé
    if(etat->dt >=etat->t_pi[0]){
      if(var->d2==1){
	donnees->modele.kart1.k.v.x=etat->c_pi2[0];
	donnees->modele.kart1.k.v.y=etat->c_pi2[0];
	var->d2=0;
	etat->dt=0;
      }else if(var->f2==1){
	donnees->modele.kart2.k.v.x=etat->c_pi2[0];
	donnees->modele.kart2.k.v.y=etat->c_pi2[0];
	var->f2=0;
	etat->dt=0;
      }else{
	var->u=0;
	var->s2=0;
	etat->dt=0;
      }
      etat->mode=0;
      var->w=0;
    }
  }
  if (fermetureDemandee()) {
    donnees->fini = 1; 
  }
}    

//chargement des images en memoire
void charger_image(donnees_t *donnees){
  
  donnees->image.im_tab[0] = chargerImage("LancementMK1.png");
  donnees->image.im_tab[2] = chargerImage("ObjetChampiRouge.png");
  donnees->image.im_tab[3] = chargerImage("ObjetChampiBlanc.png");
  donnees->image.im_tab[4] = chargerImage("ObjetChampiBleu.png");
  donnees->image.im_tab[5] = chargerImage("LancementMK.png");
  donnees->image.im_tab[6] = chargerImage("P_MK1.png");
  donnees->image.im_tab[7] = chargerImage("P_MK2.png");
  donnees->image.im_tab[8] = chargerImage("Commandes.png");
  donnees->image.im_tab[9] = chargerImage("CAB_MK.png");
  donnees->image.im_tab[10] = chargerImage("CAB_MK1.png");
  donnees->image.im_tab[11] = chargerImage("CAB_MK2.png");
  donnees->image.im_tab[12] = chargerImage("CAB_MK3.png");
  
  donnees->image.image_tab[0] = chargerImage("BlueZero.png");
  donnees->image.image_tab[1] = chargerImage("BlueOne.png");
  donnees->image.image_tab[2] = chargerImage("BlueTwo.png");
  donnees->image.image_tab[3] = chargerImage("BlueThree.png");
  donnees->image.image_tab[4] = chargerImage("Win.png");
  donnees->image.image_tab[5] = chargerImage("RedZero.png");
  donnees->image.image_tab[6] = chargerImage("RedOne.png");
  donnees->image.image_tab[7] = chargerImage("RedTwo.png");
  donnees->image.image_tab[8] = chargerImage("RedThree.png");
  donnees->image.image_tab[9] = chargerImage("Final_MK.jpg");
  
}

void charger_im_kart(donnees_t *donnees,var_t *var){
  char nom_fichier[60];
  int i=0;
  while(i<=3){
    sprintf(nom_fichier, "K_MK_%d.png", var->perso1);
    donnees->image.im_k[i] = chargerImage(nom_fichier);
    ++var->perso1;
    i++;
  }
  char nom_fichier2[60];
  while(i<=7){
    sprintf(nom_fichier2, "K_MK_%d.png", var->perso2);
    donnees->image.im_k[i] = chargerImage(nom_fichier2);
    ++var->perso2;
    i++;
  }
}

void charger_im_map(donnees_t *donnees,var_t *var){
  char nom_fichier[60];
  sprintf(nom_fichier, "MarioKart%d.png", var->map+1);
  donnees->image.im_m = chargerImage(nom_fichier);
}

void charger_son_kart(donnees_t *donnees,var_t *var){
  char nom_fichier[60];
  if(var->perso1!=-1&&var->perso2==-1){
    sprintf(nom_fichier, "V_MK_%d.wav", var->perso1);
  }else{
     sprintf(nom_fichier, "V_MK_%d.wav", var->perso2);
  }
  donnees->son.s_k1 = chargerAudio(nom_fichier);
  donnees->son.p[11]=jouerSon(donnees->son.s_k1,0);
  volumePiste(donnees->son.p[11],1);
}

//chargement des sons en memoires
void charger_son(donnees_t *donnees){
  donnees->son.son_tab[0] = chargerAudio("S_MK1.mp3");
  donnees->son.son_tab[1] = chargerAudio("S_MK2.mp3");
  donnees->son.son_tab[2] = chargerAudio("S_MK3.mp3");
  donnees->son.son_tab[3] = chargerAudio("Son_MK6.mp3");
  donnees->son.son_tab[4] = chargerAudio("Son_MK5.mp3");
  donnees->son.son_tab[5] = chargerAudio("Son_MK6.mp3");
  donnees->son.son_tab[6] = chargerAudio("Son_MK7.mp3");
  donnees->son.son_tab[7] = chargerAudio("Son_MK8.mp3");
  donnees->son.son_tab[8] = chargerAudio("Son_MK9.mp3");
  donnees->son.son_tab[9] = chargerAudio("S_F_Lap_MK.mp3");

  donnees->son.s_tab[0] = chargerAudio("Son_menu.mp3");
  donnees->son.s_tab[1] = chargerAudio("S_MK_Start.mp3");
  donnees->son.s_tab[2] = chargerAudio("CAB_MK.wav");
  donnees->son.s_tab[3] = chargerAudio("S_MK_moteur.mp3");
  donnees->son.s_tab[4] = chargerAudio("S_MK_Acceleration.mp3");
  donnees->son.s_tab[5] = chargerAudio("S_MK_Cube.mp3");
  donnees->son.s_tab[6] = chargerAudio("Son_MK_Win.mp3");
  donnees->son.s_tab[7] = chargerAudio("S_MK_boo.wav");
  donnees->son.s_tab[8] = chargerAudio("Welcome.wav");
  donnees->son.s_tab[9] = chargerAudio("SelectPlayer.wav");
  donnees->son.s_tab[10] = chargerAudio("SelectMap.wav");
  donnees->son.s_tab[11] = chargerAudio("Congratulation.wav");
}

//affiche le menu où on choisit les maps
void afficher_menu(donnees_t *donnees,var_t *var){
  int i=0,o=0,u=0;
  EtatSourisClavier esc;
  afficherImage(donnees->image.im_tab[5],0-tn_largeur(donnees->image.im_tab[5])/2,0+tn_hauteur(donnees->image.im_tab[5])/2);
  tamponner();
  donnees->son.p[0]=jouerSon(donnees->son.s_tab[0],5);
  donnees->son.p[9]=jouerSon(donnees->son.s_tab[8],0);
  volumePiste(donnees->son.p[9],1);
  volumePiste(donnees->son.p[0],1);
  attendreTouche();
  while(u==0){
    esc = lireEtatSourisClavier();
    if(esc.touchesClavier[40]==1){
      u++;
      effacerTableau();
      afficherImage(donnees->image.im_tab[6],0-tn_largeur(donnees->image.im_tab[6])/2,0+tn_hauteur(donnees->image.im_tab[6])/2);
      donnees->son.p[8]=jouerSon(donnees->son.s_tab[9],0);
      volumePiste(donnees->son.p[8],1);
      tamponner();
      while(o!=2){
	esc = lireEtatSourisClavier();
	if(esc.touchesClavier[30+i]==1){//touche 1
	  if(o==0){
	    if(i==9){
	      var->perso1=0;
	    }else if(i==-10){
	      var->perso1=40;
	    }else if(i==-4){
	      var->perso1=44;
	    }else if(i==-22){
	      var->perso1=48;
	    }else if(i==-9){
	      var->perso1=52;
	    }else if(i==-7){
	      var->perso1=56;
	    }else{
	      var->perso1=(i+1)*4;
	    }
	    charger_son_kart(donnees,var);
	    effacerTableau();
	    afficherImage(donnees->image.im_tab[7],0-tn_largeur(donnees->image.im_tab[7])/2,0+tn_hauteur(donnees->image.im_tab[7])/2);
	    tamponner();
	    var->pos1=0;
	    o++;
	    attendreNms(300);
	  }else{
	    if(i==9){
	      var->perso2=0;
	    }else if(i==-10){
	      var->perso2=40;
	    }else if(i==-4){
	      var->perso2=44;
	    }else if(i==-22){
	      var->perso2=48;
	    }else if(i==-9){
	      var->perso2=52;
	    }else if(i==-7){
	      var->perso2=56;
	    }else{
	      var->perso2=(i+1)*4;
	    }
	    charger_son_kart(donnees,var);
	    var->pos2=4;
	    o++;
	  }
	}
	
	if(i==9){
	  i=-10;
	}else if(i==-10){
	  i=-4;
	}else if(i==-4){
	  i=-22;
	}else if(i==-22){
	  i=-9;
	}else if(i==-9){
	  i=-7;
	}else if(i==-7){
	  i=0;
	}else{
	  i++;
	}
      }
      
      attendreNms(700);
      if(var->perso2!=-1){
	o=0;
	i=0;
	effacerTableau();
	afficherImage(donnees->image.im_tab[0],0-tn_largeur(donnees->image.im_tab[0])/2,0+tn_hauteur(donnees->image.im_tab[0])/2);
	jouerSon(donnees->son.s_tab[10],0);
	tamponner();
	attendreTouche();
	while(o==0){
	  esc = lireEtatSourisClavier();
	  if(esc.touchesClavier[30+i] ==1){//touche 1
	    var->map=i;
	    o++;
	  }
	  if(i==8){
	    i=0;
	  }else{
	    i++;
	  }
	}
      }
    }else if(esc.touchesClavier[224]==1){
      effacerTableau();
      u++;
      afficherImage(donnees->image.im_tab[8],0-tn_largeur(donnees->image.im_tab[8])/2,0+tn_hauteur(donnees->image.im_tab[8])/2);
      tamponner();
      while(o!=1){
	esc = lireEtatSourisClavier();
	if(esc.touchesClavier[42]==1){
	  supprimerPiste(donnees->son.p[0]);
	  effacerTableau();
	  afficher_menu(donnees,var);
	  o++;
	}
      }
    }
  }
  effacerTableau();
}

//affiche les scores en direct et l'écran de fin de jeu
void afficher_score_fin(donnees_t *donnees,var_t *var){
  EtatSourisClavier esc = lireEtatSourisClavier();
  int u=0,i=0,o=1,y=0,s=0,j=1;
  if((var->a == 4 || var->b==8)){
    while(i==0){
      if(u==0){
	donnees->son.p[10]=jouerSon(donnees->son.s_tab[11],0);
	volumePiste(donnees->son.p[10],1);
	donnees->son.p[1]=jouerSon(donnees->son.s_tab[6],5);
	volumePiste(donnees->son.p[1],1);
	u++;
	supprimerPiste(donnees->son.p[2]);
	if(var->a==4){
	  donnees->image.im_k[var->pos1+o]=rotozoomImage(donnees->image.im_k[var->pos1+o],0,3,3);
	  donnees->image.im_k[5]=rotozoomImage(donnees->image.im_k[5],0,2,2);
	}else{
	  donnees->image.im_k[1]=rotozoomImage(donnees->image.im_k[1],0,2,2);
	  donnees->image.im_k[var->pos2+j]=rotozoomImage(donnees->image.im_k[var->pos2+j],0,3,3);
	}
      }
      afficherImage(donnees->image.image_tab[4],0-tn_largeur(donnees->image.image_tab[4])/2,0+tn_hauteur(donnees->image.image_tab[4])/2);
      if(y==0){
	donnees->t=0;
	y++;
      }
      donnees->t=donnees->t+donnees->dt;
      if(donnees->t>=2){
	if(var->a==4){
	  if(o==2){
	    o=0;
	  }else if(o==0){
	    o=3;
	  }else if(o==3){
	    o=1;
	    s=1;
	  }else{
	    o++;
	  }
	  if(o!=1&&s==0){
	    donnees->image.im_k[var->pos1+o]=rotozoomImage(donnees->image.im_k[var->pos1+o],0,3,3);
	  }
	}else{
	  if(j==2){
	    j=0;
	  }else if(j==0){
	    j=3;
	  }else if(j==3){
	    j=1;
	    s=1;
	  }else{
	    j++;
	  }
	  if(j!=1&&s==0){
	    donnees->image.im_k[var->pos2+j]=rotozoomImage(donnees->image.im_k[var->pos2+j],0,3,3);
	  }
	}
	y=0;
      }
      if(var->a==4){
	afficherImage(donnees->image.im_k[var->pos1+o],-190,120);
	afficherImage(donnees->image.im_k[5],140,80);
      }else{
	afficherImage(donnees->image.im_k[1],140,80);
	afficherImage(donnees->image.im_k[var->pos2+j],-190,120);
      }
      tamponner();
      esc = lireEtatSourisClavier();
      if(esc.touchesClavier[41]){//touche echap
	donnees->fini =1;
	i++;
      }else if(esc.touchesClavier[40]){//touche entrée
	supprimerPiste(donnees->son.p[1]);
	*var=init_var(*donnees);
	afficher_menu(donnees,var);
	charger_im_map(donnees,var);
	*donnees=init_donnees(donnees,*var);
	charger_im_kart(donnees,var);
	u--;
	i++;
      }
    }
    i--;
  }
  // affiche les scores en direct
  if(var->a<=3){
    afficherImage(donnees->image.image_tab[var->a],-400,180);
    if(var->a==3){
      if(var->sc1==0){
	++var->sc1;
	donnees->son.p[12]=jouerSon(donnees->son.son_tab[9],0);
	volumePiste( donnees->son.p[12],1);
      }
      var->sc1=var->sc1 +donnees->dt;
      if(var->sc1<=2){
	afficherImage(donnees->image.image_tab[9],-400,80);
      }
      
    }
  }if(var->b<=7){
    afficherImage(donnees->image.image_tab[var->b+1],300,180);
    if(var->b==7){
      if(var->sc2==0){
	++var->sc2;
	donnees->son.p[12]=jouerSon(donnees->son.son_tab[9],0);
	volumePiste( donnees->son.p[12],1);
      }
      var->sc2=var->sc2 +donnees->dt;
      if(var->sc2<=2){
	afficherImage(donnees->image.image_tab[9],300,80);
      }
    }
  }
}
//affiche la map choisie et le compte a rebours
void afficher_map_cab(donnees_t *donnees, var_t *var){
  //affiche la map
  supprimerPiste(donnees->son.p[0]);
  afficherImage( donnees->image.im_m,0-tn_largeur( donnees->image.im_m)/2,0+tn_hauteur( donnees->image.im_m)/2);
  if(var->cab==0){
    tamponner();
    donnees->son.p[3]=jouerSon(donnees->son.s_tab[1],0);
    attendreNms(4000);
  }
  
  //affiche le compte à rebours
  if(var->cab == 0){
    afficher_donnees(*donnees,*var);
    jouerSon(donnees->son.s_tab[4],3);
    jouerSon(donnees->son.s_tab[2],0);
    attendreNms(600);
    afficherImage(donnees->image.im_tab[12],300,50);
    afficherImage(donnees->image.im_tab[12],-400,50);
    tamponner();
    attendreNms(1000);
    afficherImage(donnees->image.im_tab[11],300,50);
    afficherImage(donnees->image.im_tab[11],-400,50);
    tamponner();
    attendreNms(1000);
    afficherImage(donnees->image.im_tab[10],300,50);
    afficherImage(donnees->image.im_tab[10],-400,50);
    tamponner();
    attendreNms(1000);
    afficherImage(donnees->image.im_tab[9],300,50);
    afficherImage(donnees->image.im_tab[9],-400,50);
    tamponner();
    ++var->cab;
    donnees->son.p[2]=jouerSon(donnees->son.son_tab[var->map],3);
  }
}

//affiche les objets obtenus
void afficher_objet(donnees_t donnees,var_t var){
  if(var.d1==1){
    afficherImage(donnees.image.im_tab[4],-400,-20);
  }else{
    fixerCouleurDeTransparence(donnees.image.im_tab[8],0,0,0);
  }
  if(var.d2==1){
    afficherImage(donnees.image.im_tab[4],300,-20);
  }else{
    fixerCouleurDeTransparence(donnees.image.im_tab[8],0,0,0);
  }
  if(var.f1==1){
    afficherImage(donnees.image.im_tab[2],-400,-20);;
  }else{
    fixerCouleurDeTransparence(donnees.image.im_tab[6],0,0,0);
  }
  if(var.f2==1){
    afficherImage(donnees.image.im_tab[2],300,-20);
  }else{
    fixerCouleurDeTransparence(donnees.image.im_tab[6],0,0,0);
  }
  if(var.s1==1){
    afficherImage(donnees.image.im_tab[3],-400,-20);
  }else{
    fixerCouleurDeTransparence(donnees.image.im_tab[7],0,0,0);
  }
  if(var.s2==1){
    afficherImage(donnees.image.im_tab[3],300,-20);
  }else{
    fixerCouleurDeTransparence(donnees.image.im_tab[7],0,0,0);
  }
}

void recup_objet(donnees_t *donnees,etat_t *etat,var_t *var){
  int e;
  if(abs(donnees->modele.kart1.k.p.x-donnees->modele.cube1.p.x)<15  &&  abs(donnees->modele.kart1.k.p.y- donnees->modele.cube1.p.y)<15 && var->c1 == 0&& var->c2==0&& var->d1==0 && var->f1==0 && var->s1==0){
    donnees->son.p[6]=jouerSon(donnees->son.s_tab[5],0);
    volumePiste(donnees->son.p[6],1);
    var->c1 = var->c1+1;//Avantage obtenu pour le kart1
    etat->tps=0;
    ++var->t;
  }
  if(var->t==1 && var->c1==1 && var->d1==0 && var->f1==0 && var->s1==0){
    etat->tps=etat->tps + donnees->dt;
    if(etat->tps >=3){
      srand(time(NULL));
      e=rand()%3;
      if(e == 0){
	if(var->d1 == 0){
	  var->d1 = var->d1+1;
	}
      }else if(e == 1){
	if(var->f1 == 0){
	  var->f1 = var->f1+1;
	}
      }else{
	if (var->s1==0){
	  var->s1=var->s1+1;
	}
      }
      --var->c1;
    }
  }
  if(abs(donnees->modele.kart2.k.p.x-donnees->modele.cube1.p.x)<15  &&  abs(donnees->modele.kart2.k.p.y- donnees->modele.cube1.p.y)<15 && var->c1 == 0 && var->c2==0 && var->d2==0 && var->f2==0 && var->s2==0){
    donnees->son.p[6]=jouerSon(donnees->son.s_tab[5],0);
    volumePiste(donnees->son.p[6],1);
    var->c1 = var->c1+1;
    etat->tps1=0;
    ++var->w;
  }
  if(var->w==1 &&  var->c1 == 1 && var->d2==0 && var->f2==0 && var->s2==0){
    etat->tps1 = etat->tps1 + donnees->dt;
    if(etat->tps1>=3){
      srand(time(NULL));
      e=rand()%3;
      if(e == 0){//Avantage obtenu pour le kart2
	if(var->d2 == 0){
	  var->d2 = var->d2+1;
	}
      }else if(e == 1){
	if(var->f2 == 0){
	  var->f2 = var->f2+1;
	}
      }else{
	if(var->s2==0){
	  var->s2=var->s2+1;
	}
      }
      --var->c1;
    }
  }
  if((abs(donnees->modele.kart1.k.p.x-donnees->modele.cube2.p.x)<15 &&  abs(donnees->modele.kart1.k.p.y- donnees->modele.cube2.p.y)<15 &&  var->c2==0 && var->c1==0 && var->d1==0 && var->f1==0 && var->s1==0)){
    donnees->son.p[6]=jouerSon(donnees->son.s_tab[5],0);
    volumePiste(donnees->son.p[6],1);
    var->c2 = var->c2+1;//Avantage obtenu pour le kart1
    etat->tps=0;
    ++var->t;
  }
  if(var->t==1 && var->c2==1 && var->d1==0 && var->f1==0 && var->s1==0){
    etat->tps=etat->tps + donnees->dt;
    if(etat->tps >=3){
      srand(time(NULL));
      e=rand()%3;
      if(e == 0){
	if(var->d1 == 0){
	  var->d1 = var->d1+1;
	}
      }else if(e == 1){
	if(var->f1 == 0){
	  var->f1 = var->f1+1;
	}
      }else{
	if (var->s1==0){
	  var->s1=var->s1+1;
	}
      }
      --var->c2;
    }
  }
  if((abs(donnees->modele.kart2.k.p.x-donnees->modele.cube2.p.x)<15 &&  abs(donnees->modele.kart2.k.p.y- donnees->modele.cube2.p.y)<15 &&  var->c2==0 && var->c1==0 && var->d2==0 && var->f2==0 && var->s2==0)){
    donnees->son.p[6]=jouerSon(donnees->son.s_tab[5],0);
    volumePiste(donnees->son.p[6],1);
    var->c2 = var->c2+1;
    etat->tps1=0;
    ++var->w;
  }
  if( var->w==1 && var->c2 == 1 && var->d2==0 && var->f2==0 && var->s2==0){
    etat->tps1 = etat->tps1 + donnees->dt;
    if(etat->tps1>=3){
      srand(time(NULL));
      e=rand()%3;
      if(e == 0){//Avantage obtenu pour le kart2
	if(var->d2 == 0){
	  var->d2 = var->d2+1;
	}
      }else if(e == 1){
	if(var->f2 == 0){
	  var->f2 = var->f2+1;
	}
      }else{
	if(var->s2==0){
	  var->s2=var->s2+1;
	}
      }
      --var->c2;
    }
  }
}

void nb_tours(donnees_t *donnees, var_t *var){
  if(fabs(donnees->modele.kart1.k.p.x-donnees->modele.ligne.p.x)<50 && fabs(donnees->modele.kart1.k.p.y-donnees->modele.ligne.p.y)<12 && donnees->modele.kart1.m==0){
    if(var->n==1)var->g=1;
    if(var->n==0 && var->g==0){
      var->a=var->a+1;
    }
    if(var->n==0&&var->g==1)var->g=0;
    var->i1=donnees->modele.rea.p.x;
    var->j1=donnees->modele.rea.p.y;
    donnees->modele.kart1.m=1;
  }
  if(donnees->modele.kart1.k.p.y > donnees->modele.ligne.p.y+20){
    donnees->modele.kart1.m=0;
  }
  if(fabs(donnees->modele.kart2.k.p.x-donnees->modele.ligne.p.x)<50 && fabs(donnees->modele.kart2.k.p.y-donnees->modele.ligne.p.y)<12  && donnees->modele.kart2.m==0){
    if(var->l==1)var->h=1;
    if(var->l==0 && var->h==0){
      var->b=var->b+1;
    }
    if(var->l==0 && var->h==1)var->h=0;
    var->i2=donnees->modele.rea.p.x;
    var->j2=donnees->modele.rea.p.y;
    donnees->modele.kart2.m=1;
  }
  if(donnees->modele.kart2.k.p.y > donnees->modele.rea.p.y){
    donnees->modele.kart2.m=0;
  }
}

void respawn(donnees_t *donnees,etat_t *etat,var_t *var){
  int r1,v1,b1,r2,v2,b2;
  //Lit les pixels sur lesquels roulent les 2 kart
  chargerPixelsEnMemoire();
  lirePixel(donnees->modele.kart1.k.p.x,donnees->modele.kart1.k.p.y,&r1,&v1,&b1);
  lirePixel(donnees->modele.kart2.k.p.x,donnees->modele.kart2.k.p.y,&r2,&v2,&b2);
  
  if(abs(donnees->modele.kart1.k.p.x-donnees->modele.rea.v.x)<30 && abs(donnees->modele.kart1.k.p.y-donnees->modele.rea.v.y)<30){
    var->i1=donnees->modele.rea.v.x;
    var->j1=donnees->modele.rea.v.y;
  }
  if(abs(donnees->modele.kart2.k.p.x-donnees->modele.rea.v.x)<30 && abs(donnees->modele.kart2.k.p.y-donnees->modele.rea.v.y)<30){
    var->i2=donnees->modele.rea.v.x;
    var->j2=donnees->modele.rea.v.y;
  }
  if((r1 >= donnees->x[0] && r1 <= donnees->y[0])&&(v1 >= donnees->x[1] && v1<= donnees->y[1])&&(b1 >= donnees->x[2] && b1<= donnees->y[2])&& donnees->modele.kart1.n!=1){//pixel r1 et v1 pour les différentes teintes de marron
    etat->delai1=0;
    donnees->modele.kart1.n=1;
    donnees->modele.kart1.k.v.x= 0.001;
    donnees->modele.kart1.k.v.y= 0.001;
  }
  if( donnees->modele.kart1.n==1){
    etat->delai1=etat->delai1+donnees->dt;
    if(etat->delai1 >=0.5){
      donnees->modele.kart1.k.v.x= etat->c_pi2[0];
      donnees->modele.kart1.k.v.y= etat->c_pi2[0];
      donnees->modele.kart1.k.p.x= var->i1;
      donnees->modele.kart1.k.p.y= var->j1;
      donnees->modele.kart1.n=0;
    }
  }
  if((r2 >= donnees->x[0] && r2 <= donnees->y[0])&&(v2 >= donnees->x[1] && v2<= donnees->y[1])&&(b2 >= donnees->x[2] && b2<= donnees->y[2])&& donnees->modele.kart2.n!=1){
    etat->delai2=0;
    donnees->modele.kart2.n=1;
    donnees->modele.kart2.k.v.x= 0.001;
    donnees->modele.kart2.k.v.y= 0.001;
  }
  if( donnees->modele.kart2.n==1){
    etat->delai2=etat->delai2+donnees->dt;
    if(etat->delai2 >=0.5){
      donnees->modele.kart2.k.v.x= etat->c_pi2[0];
      donnees->modele.kart2.k.v.y= etat->c_pi2[0];
      donnees->modele.kart2.k.p.x= var->i2;
      donnees->modele.kart2.k.p.y= var->j2;
      donnees->modele.kart2.n=0;
    }
  }
}
