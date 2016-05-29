#include "lib_scene3d.h"
#include "lib_objet3d.h"
#include <math.h>

void matTranslation(t_point3d*vecteur,double mat[4][4])
{
  if(vecteur!=NULL)
    {
      mat[0][0]=1;
      mat[0][1]=0;
      mat[0][2]=0;
      mat[0][3]=vecteur->xyzt[0];

      mat[1][0]=0;
      mat[1][1]=1;
      mat[1][2]=0;
      mat[1][3]=vecteur->xyzt[1];

      mat[2][0]=0;
      mat[2][1]=0;
      mat[2][2]=1;
      mat[2][3]=vecteur->xyzt[2];

      mat[3][0]=0;
      mat[3][1]=0;
      mat[3][2]=0;
      mat[3][3]=1;
    }
}


void matTranslationinv(t_point3d*vecteur,double mat[4][4])
{
  if(vecteur!=NULL)
    {
      mat[0][0]=1;
      mat[0][1]=0;
      mat[0][2]=0;
      mat[0][3]=-vecteur->xyzt[0];

      mat[1][0]=0;
      mat[1][1]=1;
      mat[1][2]=0;
      mat[1][3]=-vecteur->xyzt[1];

      mat[2][0]=0;
      mat[2][1]=0;
      mat[2][2]=1;
      mat[2][3]=-vecteur->xyzt[2];

      mat[3][0]=0;
      mat[3][1]=0;
      mat[3][2]=0;
      mat[3][3]=1;
    }
}


void matRotation(t_point3d*centre,float degreX,float degreY,float degreZ,double mat[4][4])
{
  if(centre!=NULL)
    {
      float x,y,z;
      x=degreX*M_PI/180;
      y=degreY*M_PI/180;
      z=degreZ*M_PI/180;
      double m[4][4]={{1,0,0,centre->xyzt[0]},\
		      {0,1,0,centre->xyzt[1]},\
		      {0,0,1,centre->xyzt[2]},\
		      {0,0,0,1}};
      double mx[4][4]={{1,0,0,0},\
		       {0,cos(x),-sin(x),0},\
		       {0,sin(x),cos(x),0},\
		       {0,0,0,1}};
      double my[4][4]={{cos(y),0,sin(y),0},\
		       {0,1,0,0},\
		       {-sin(y),0,cos(y),0},\
		       {0,0,0,1}};
      double mz[4][4]={{cos(z),-sin(z),0,0},\
		       {sin(z),cos(z),0,0},\
		       {0,0,1,0},\
		       {0,0,0,1}};
      double minv[4][4]={{1,0,0,-centre->xyzt[0]},\
			 {0,1,0,-centre->xyzt[1]},\
			 {0,0,1,-centre->xyzt[2]},\
			 {0,0,0,1}};

      multiplicationMatrice3d(mat,m,mx);
      multiplicationMatrice3d(mat,mat,my);
      multiplicationMatrice3d(mat,mat,mz);
      multiplicationMatrice3d(mat,mat,minv);
    } 
}


void matRotationinv(t_point3d*centre,float degreX,float degreY,float degreZ,double mat[4][4]){
  matRotation(centre,-degreX,-degreY,-degreZ,mat);
}


t_point3d*Getcentre(t_scene3d*pt_scene3d)
{
  t_point3d*origine=definirPoint3d(0,0,0);
  double mat[4][4];
  matTranslation(origine,mat);
  Getcentre_rec(mat,pt_scene3d);
  multiplicationVecteur3d(origine,mat,origine);
  return origine;
}


void Getcentre_rec(double mat[4][4],t_scene3d*pt_scene3d)
{
  if(pt_scene3d->pere!=NULL)
    {
      Getcentre_rec(mat,pt_scene3d->pere);
    }
  multiplicationMatrice3d(mat,pt_scene3d->inv,mat);
}


t_point3d*GetcentreR(t_scene3d*pt_scene3d)
{
  t_point3d*res=definirPoint3d(0,0,0);
  multiplicationVecteur3d(res,pt_scene3d->mat,res);
  return res;
}


void translationScene3d(t_scene3d*pt_scene3d,t_point3d*vecteur)
{
  double mat[4][4];
  double inv[4][4];
  matTranslation(vecteur,mat);
  matTranslationinv(vecteur,inv);
  transformationScene3d(pt_scene3d,mat,inv);
}

 
void rotationScene3d(t_scene3d*pt_scene3d,t_point3d*centre,float degreX,float degreY,float degreZ)
{
  double mat[4][4];
  double inv[4][4];
  matRotation(centre,degreX,degreY,degreZ,mat);
  matRotationinv(centre,degreX,degreY,degreZ,inv);   
  transformationScene3d(pt_scene3d,mat,inv);
}


void transformationScene3d(t_scene3d *pt_scene3d,double mat[4][4],double inv[4][4])
{
  if(pt_scene3d!=NULL){
    multiplicationMatrice3d(pt_scene3d->mat,mat,pt_scene3d->mat);
    multiplicationMatrice3d(pt_scene3d->inv,pt_scene3d->inv,inv);
  }
}


t_scene3d*creerScene3d(t_objet3d*pt_objet3d)
{
  t_scene3d * pt_scene3d=malloc(sizeof(t_scene3d));
  t_point3d * vecteur=definirPoint3d(0,0,0);
  pt_scene3d->objet3d=pt_objet3d;
  pt_scene3d->pere=NULL;
  pt_scene3d->fils=NULL;
  pt_scene3d->frere=NULL;
  matTranslation(vecteur,pt_scene3d->mat);
  matTranslation(vecteur,pt_scene3d->inv);
  free(vecteur);  
  return pt_scene3d;
}


void ajoutObjet3d(t_scene3d*pt_scene3d,t_objet3d*pt_objet3d)
{
  t_scene3d * fils=creerScene3d(pt_objet3d);
  fils->pere=pt_scene3d;
  fils->frere=pt_scene3d->fils;
  pt_scene3d->fils=fils;
}

void dessinerScene3d(t_surface*surface,t_scene3d*pt_scene3d,double h)
{
  double mat[4][4]={{1,0,0,0},			\
		    {0,1,0,0},			\
		    {0,0,1,0},			\
		    {0,0,0,1}};
  double inv[4][4]={{1,0,0,0},			\
		    {0,1,0,0},			\
		    {0,0,1,0},			\
		    {0,0,0,1}};
  dessinerScene3d_rec(surface,pt_scene3d,h,mat,inv);
}

 
void dessinerScene3d_rec(t_surface*surface,t_scene3d*pt_scene3d,double h,double mat[4][4],double inv[4][4])
{
  if(pt_scene3d!=NULL){
    double tmpmat[4][4];
    double tmpinv[4][4];
 
    multiplicationMatrice3d(tmpmat,mat,pt_scene3d->mat);
    multiplicationMatrice3d(tmpinv,pt_scene3d->inv,inv);

    t_objet3d* copie=copierObjet3d(pt_scene3d->objet3d);

    transformationObjet3d(copie,tmpmat);
    dessinerObjet3d(surface,copie,h);

    //transformationObjet3d(pt_scene3d->objet3d,tmpinv);

    libererObjet3d(copie);

    dessinerScene3d_rec(surface,pt_scene3d->fils,h,tmpmat,tmpinv);
    dessinerScene3d_rec(surface,pt_scene3d->frere,h,mat,inv);
  }
}
 


void extract(t_scene3d*pt_scene3d)
{
  t_scene3d*tmp=pt_scene3d->frere;
  t_scene3d*var=pt_scene3d->pere->fils;
  if(var!=pt_scene3d)
    {
      while(var->frere!=pt_scene3d)
	{
	  var=var->frere;
	}
      var->frere=tmp;
    }
  else{
    pt_scene3d->pere->fils=tmp; 
  }
}


void ajoutfils(t_scene3d*pere,t_scene3d*fils)
{
  fils->pere=pere;
  fils->frere=pere->fils;
  pere->fils=fils;
}



void Racine(t_scene3d*scene)
{
  if(scene->pere!=NULL)
    {
      Racine(scene->pere);
      double mat[4][4];
      double inv[4][4];
      copierMatrice3d(mat,scene->pere->mat);
      copierMatrice3d(inv,scene->pere->inv);

      copierMatrice3d(scene->pere->mat,scene->inv);
      copierMatrice3d(scene->pere->inv,scene->mat);

      copierMatrice3d(scene->inv,mat);
      copierMatrice3d(scene->mat,inv);

      extract(scene);
      ajoutfils(scene,scene->pere);
      scene->pere=NULL;
      scene->frere=NULL;
    }
}

void affscene(t_scene3d*scene)
{
  if(scene==NULL)
    printf("NULL\n");
  else
    {
      printf("scene : %p(\n",scene);
      if(scene->pere!=NULL)
	printf("pere : %p\n",scene->pere);
      else
	printf("pere : NULL\n");

      printf("frere :\n(");
      affscene(scene->frere);
      printf(")\n");

      printf("fils :\n(");
      affscene(scene->fils);
      printf(")\n");
      printf(")\n");

    }
}

void dragon_init(Uint32* tabc[6])
{
  int i;
  for(i=0;i<7;i++){//tete
    switch(rand()%5){
    case 0:
      tabc[0][i]=0x42157c;
      break;
    case 1:
      tabc[0][i]=0x6820a7;
      break;
    case 2:
      tabc[0][i]=0x8825ce;
      break;
    case 3:
      tabc[0][i]=0x450172;
      break;
    case 4:
      tabc[0][i]=0x3a005c;
      break;
    }
  }
  tabc[0][7]=0xff0000;
  tabc[0][8]=0xff0000;

  for(i=0;i<10;i++){//cou
    switch(rand()%5){
    case 0:
      tabc[1][i]=0x42157c;
      break;
    case 1:
      tabc[1][i]=0x6820a7;
      break;
    case 2:
      tabc[1][i]=0x8825ce;
      break;
    case 3:
      tabc[1][i]=0x450172;
      break;
    case 4:
      tabc[1][i]=0x3a005c;
      break;
    }
  }
  for(i=0;i<7;i++){//ailed
    switch(rand()%5){
    case 0:
      tabc[2][i]=0x42157c;
      break;
    case 1:
      tabc[2][i]=0x6820a7;
      break;
    case 2:
      tabc[2][i]=0x8825ce;
      break;
    case 3:
      tabc[2][i]=0x450172;
      break;
    case 4:
      tabc[2][i]=0x3a005c;
      break;
    }
  }
  for(i=0;i<7;i++){//aileg
    switch(rand()%5){
    case 0:
      tabc[3][i]=0x42157c;
      break;
    case 1:
      tabc[3][i]=0x6820a7;
      break;
    case 2:
      tabc[3][i]=0x8825ce;
      break;
    case 3:
      tabc[3][i]=0x450172;
      break;
    case 4:
      tabc[3][i]=0x3a005c;
      break;
    }
  }
  for(i=0;i<24;i++){//queue
    switch(rand()%5){
    case 0:
      tabc[4][i]=0x42157c;
      break;
    case 1:
      tabc[4][i]=0x6820a7;
      break;
    case 2:
      tabc[4][i]=0x8825ce;
      break;
    case 3:
      tabc[4][i]=0x450172;
      break;
    case 4:
      tabc[4][i]=0x3a005c;
      break;
    }
  }
  for(i=0;i<4;i++){//corp
    switch(rand()%5){
    case 0:
      tabc[5][i]=0x42157c;
      break;
    case 1:
      tabc[5][i]=0x6820a7;
      break;
    case 2:
      tabc[5][i]=0x8825ce;
      break;
    case 3:
      tabc[5][i]=0x450172;
      break;
    case 4:
      tabc[5][i]=0x3a005c;
      break;
    }
  }
}


t_scene3d*dragon(t_scene3d** tab[5],Uint32*tabdc[6]) //tete, cou, queue, ailed, ailg 
{
  t_scene3d**tete=malloc(sizeof(t_scene3d*)*3);
  t_scene3d**cou=malloc(sizeof(t_scene3d*)*5);
  t_scene3d**queue=malloc(sizeof(t_scene3d*)*12);
  t_scene3d**aileg=malloc(sizeof(t_scene3d*)*5);
  t_scene3d**ailed=malloc(sizeof(t_scene3d*)*5);
 
  tab[0]=tete;
  tab[1]=cou;
  tab[2]=ailed;
  tab[3]=aileg;
  tab[4]=queue;

  Uint32*tetec=malloc(sizeof(Uint32)*9);
  Uint32*couc=malloc(sizeof(Uint32)*10);
  Uint32*queuec=malloc(sizeof(Uint32)*24);
  Uint32*ailegc=malloc(sizeof(Uint32)*7);
  Uint32*ailedc=malloc(sizeof(Uint32)*7);
  Uint32*corpc=malloc(sizeof(Uint32)*4);

  tabdc[0]=tetec;
  tabdc[1]=couc;
  tabdc[2]=ailedc;
  tabdc[3]=ailegc;
  tabdc[4]=queuec;
  tabdc[5]=corpc;

  dragon_init(tabdc);


  t_scene3d* tmps;
  t_objet3d* tmpo1;
  t_objet3d* tmpo2;
  int i;  
  t_point3d*tmp;
 
  //body + aileron

  t_objet3d*corp=parallelepipede(320,120,120,&(tabdc[5][0]));
  t_objet3d*a1=parallelepipede(60,30,10,&(tabdc[5][1]));
  t_objet3d*a2=parallelepipede(60,30,10,&(tabdc[5][2]));
  t_objet3d*a3=parallelepipede(60,30,10,&(tabdc[5][3]));

  t_scene3d*body=creerScene3d(corp);

  ajoutObjet3d(body,a1);
  t_scene3d*ba1=body->fils;
  ajoutObjet3d(body,a2);
  t_scene3d*ba2=body->fils;
  ajoutObjet3d(body,a3);
  t_scene3d*ba3=body->fils;

  tmp=definirPoint3d(-100,-75,0);
  translationScene3d(ba1,tmp);
  free(tmp);

  tmp=definirPoint3d(0,-75,0);
  translationScene3d(ba2,tmp);
  free(tmp);

  tmp=definirPoint3d(100,-75,0);
  translationScene3d(ba3,tmp);
  free(tmp);

  //queue
  
  tmpo1=parallelepipede(50,50,50,&(tabdc[4][0]));
  tmpo2=parallelepipede(30,20,10,&(tabdc[4][12]));
  
  ajoutObjet3d(body,tmpo1);
  queue[0]=body->fils;
  
  ajoutObjet3d(queue[0],tmpo2);
  tmps=queue[0]->fils;
      
  tmp=definirPoint3d(-185,0,0);
  translationScene3d(queue[0],tmp);
  free(tmp);

  tmp=definirPoint3d(0,-35,0);
  translationScene3d(tmps,tmp);
  free(tmp);

  for(i=1;i<12;i++)
    {
      tmpo1=parallelepipede(50,50,50,&(tabdc[4][i]));
      tmpo2=parallelepipede(30,20,10,&(tabdc[4][i+12]));

      ajoutObjet3d(queue[i-1],tmpo1);
      queue[i]=queue[i-1]->fils;

      ajoutObjet3d(queue[i],tmpo2);
      tmps=queue[i]->fils;
      
      tmp=definirPoint3d(-50,0,0);
      translationScene3d(queue[i],tmp);
      free(tmp);

      tmp=definirPoint3d(0,-35,0);
      translationScene3d(tmps,tmp);
      free(tmp);
    }

  //cou
  
  tmpo1=parallelepipede(50,50,50,&(tabdc[1][0]));
  tmpo2=parallelepipede(30,20,10,&(tabdc[1][5]));
  
  ajoutObjet3d(body,tmpo1);
  cou[0]=body->fils;
  
  ajoutObjet3d(cou[0],tmpo2);
  tmps=cou[0]->fils;
      
  tmp=definirPoint3d(185,20,0);
  translationScene3d(cou[0],tmp);
  free(tmp);

  tmp=definirPoint3d(0,-35,0);
  translationScene3d(tmps,tmp);
  free(tmp);
  
  for(i=1;i<5;i++)
    {
      tmpo1=parallelepipede(50,50,50,&(tabdc[1][i]));
      tmpo2=parallelepipede(30,20,10,&(tabdc[1][i+5]));

      ajoutObjet3d(cou[i-1],tmpo1);
      cou[i]=cou[i-1]->fils;

      ajoutObjet3d(cou[i],tmpo2);
      tmps=cou[i]->fils;
      
      tmp=definirPoint3d(50,0,0);
      translationScene3d(cou[i],tmp);
      free(tmp);

      tmp=definirPoint3d(0,-35,0);
      translationScene3d(tmps,tmp);
      free(tmp);
    }
  
  //tete

  tmpo1=parallelepipede(80,80,80,&(tabdc[0][0]));//tete
  ajoutObjet3d(cou[4],tmpo1);
  tete[0]=cou[4]->fils;

  tmp=definirPoint3d(65,-10,0);
  translationScene3d(tete[0],tmp);
  free(tmp);
  
  tmpo1=parallelepipede(30,20,10,&(tabdc[0][1]));//aileron gauche
  ajoutObjet3d(tete[0],tmpo1);
  tmps=tete[0]->fils;

  tmp=definirPoint3d(-5,-50,-20);
  translationScene3d(tmps,tmp);
  free(tmp);
  
  tmpo1=parallelepipede(30,20,10,&(tabdc[0][2]));//aileron droit
  ajoutObjet3d(tete[0],tmpo1);
  tmps=tete[0]->fils;
  
  tmp=definirPoint3d(-5,-50,20);
  translationScene3d(tmps,tmp);
  free(tmp);
  
  tmpo1=parallelepipede(80,25,60,&(tabdc[0][3]));//machoire sup
  ajoutObjet3d(tete[0],tmpo1);
  tete[1]=tete[0]->fils;

  tmp=definirPoint3d(80,7,0);
  translationScene3d(tete[1],tmp);
  free(tmp);

  tmpo1=parallelepipede(20,10,10,&(tabdc[0][4]));//narine gauche
  ajoutObjet3d(tete[1],tmpo1);
  tmps=tete[1]->fils;
  
  tmp=definirPoint3d(20,-17,-15);
  translationScene3d(tmps,tmp);
  free(tmp);

  tmpo1=parallelepipede(20,10,10,&(tabdc[0][5]));//narine droite
  ajoutObjet3d(tete[1],tmpo1);
  tmps=tete[1]->fils;
  
  tmp=definirPoint3d(20,-17,15);
  translationScene3d(tmps,tmp);
  free(tmp); 

  tmpo1=parallelepipede(80,20,60,&(tabdc[0][6]));//machoire inf
  ajoutObjet3d(tete[0],tmpo1);
  tete[2]=tete[0]->fils;

  tmp=definirPoint3d(80,30,0);
  translationScene3d(tete[2],tmp);
  free(tmp);

  //rajouter des carré/tile pour la couleur des yeux

  tmpo1=parallelepipede(10,10,10,&(tabdc[0][7]));//oeil gauche
  ajoutObjet3d(tete[0],tmpo1);
  tmps=tete[0]->fils;

  tmp=definirPoint3d(45,-20,15);
  translationScene3d(tmps,tmp);
  free(tmp); 

  tmpo1=parallelepipede(10,10,10,&(tabdc[0][8]));//oeil droit
  ajoutObjet3d(tete[0],tmpo1);
  tmps=tete[0]->fils;

  tmp=definirPoint3d(45,-20,-15);
  translationScene3d(tmps,tmp);
  free(tmp); 

  //aile droite

  t_point3d *p0d = definirPoint3d(0,0,140);
  t_point3d *p1d = definirPoint3d(0,0,-140);
  t_point3d *p2d = definirPoint3d(-235,0,85);

  t_point3d *p3d = definirPoint3d(0,0,-140);
  t_point3d *p4d = definirPoint3d(0,0,140);
  t_point3d *p5d = definirPoint3d(-205,0,10);

  t_point3d *p6d = definirPoint3d(140,0,0);
  t_point3d *p7d = definirPoint3d(-95,0,225);
  t_point3d *p8d = definirPoint3d(-65,0,-130);
  t_point3d *p9d = definirPoint3d(-140,0,0);
  
  t_triangle3d*t1d=definirTriangle3d(p0d,p1d,p2d);
  t_triangle3d*t2d=definirTriangle3d(p3d,p4d,p5d);
  t_triangle3d*t3d=definirTriangle3d(p6d,p7d,p9d);
  t_triangle3d*t4d=definirTriangle3d(p6d,p8d,p9d);
  
  t_objet3d*b1d=parallelepipede(40,40,280,&(tabdc[2][0]));
  t_objet3d*b2d=parallelepipede(20,20,280,&(tabdc[2][1]));
  t_objet3d*b3d=parallelepipede(280,20,20,&(tabdc[2][2]));

  __insere_tete_chaine(b2d,__cree_chaine(p0d));
  __insere_tete_chaine(b2d,__cree_chaine(p1d));
  __insere_tete_chaine(b2d,__cree_chaine(p2d));

  __insere_tete_chaine(b3d,__cree_chaine(p6d));
  __insere_tete_chaine(b3d,__cree_chaine(p7d));
  __insere_tete_chaine(b3d,__cree_chaine(p8d));
  __insere_tete_chaine(b3d,__cree_chaine(p9d));

  __insere_tete_chaine(b1d,__cree_chaine(p3d));
  __insere_tete_chaine(b1d,__cree_chaine(p4d));
  __insere_tete_chaine(b1d,__cree_chaine(p5d));

  __insere_tete(b2d, __cree_maillon(t1d,&(tabdc[2][3]))); 
  __insere_tete(b1d, __cree_maillon(t2d,&(tabdc[2][4])));
  __insere_tete(b3d, __cree_maillon(t3d,&(tabdc[2][5])));
  __insere_tete(b3d, __cree_maillon(t4d,&(tabdc[2][6])));

  ajoutObjet3d(body,b1d);
  ailed[0]=body->fils;

  tmp=definirPoint3d(80,-10,200);
  translationScene3d(ailed[0],tmp);
  free(tmp);

  ajoutObjet3d(ailed[0],b2d);
  ailed[1]=ailed[0]->fils;

  tmp=definirPoint3d(0,0,280);
  translationScene3d(ailed[1],tmp);
  free(tmp);

  ajoutObjet3d(ailed[0],b3d);
  ailed[2]=ailed[0]->fils;

  tmp=definirPoint3d(-140,0,140);
  translationScene3d(ailed[2],tmp);
  free(tmp);
  
  //aile gauche
  t_point3d *p0g = definirPoint3d(0,0,-140);
  t_point3d *p1g = definirPoint3d(0,0,140);
  t_point3d *p2g = definirPoint3d(-235,0,-85);

  t_point3d *p3g = definirPoint3d(0,0,140);
  t_point3d *p4g = definirPoint3d(0,0,-140);
  t_point3d *p5g = definirPoint3d(-205,0,-10);

  t_point3d *p6g = definirPoint3d(140,0,0);
  t_point3d *p7g = definirPoint3d(-95,0,-225);
  t_point3d *p8g = definirPoint3d(-65,0,130);
  t_point3d *p9g = definirPoint3d(-140,0,0);
  
  t_triangle3d*t1g=definirTriangle3d(p0g,p1g,p2g);
  t_triangle3d*t2g=definirTriangle3d(p3g,p4g,p5g);
  t_triangle3d*t3g=definirTriangle3d(p6g,p7g,p9g);
  t_triangle3d*t4g=definirTriangle3d(p6g,p8g,p9g);
  
  t_objet3d*b1g=parallelepipede(40,40,280,&(tabdc[3][0]));
  t_objet3d*b2g=parallelepipede(20,20,280,&(tabdc[3][1]));
  t_objet3d*b3g=parallelepipede(280,20,20,&(tabdc[3][2]));

  __insere_tete_chaine(b2g,__cree_chaine(p0g));
  __insere_tete_chaine(b2g,__cree_chaine(p1g));
  __insere_tete_chaine(b2g,__cree_chaine(p2g));

  __insere_tete_chaine(b3g,__cree_chaine(p6g));
  __insere_tete_chaine(b3g,__cree_chaine(p7g));
  __insere_tete_chaine(b3g,__cree_chaine(p8g));
  __insere_tete_chaine(b3g,__cree_chaine(p9g));

  __insere_tete_chaine(b1g,__cree_chaine(p3g));
  __insere_tete_chaine(b1g,__cree_chaine(p4g));
  __insere_tete_chaine(b1g,__cree_chaine(p5g));

  __insere_tete(b2g, __cree_maillon(t1g,&(tabdc[3][3]))); 
  __insere_tete(b1g, __cree_maillon(t2g,&(tabdc[3][4])));
  __insere_tete(b3g, __cree_maillon(t3g,&(tabdc[3][5])));
  __insere_tete(b3g, __cree_maillon(t4g,&(tabdc[3][6])));

  ajoutObjet3d(body,b1g);
  aileg[0]=body->fils;

  tmp=definirPoint3d(80,-10,-200);
  translationScene3d(aileg[0],tmp);
  free(tmp);

  ajoutObjet3d(aileg[0],b2g);
  aileg[1]=aileg[0]->fils;

  tmp=definirPoint3d(0,0,-280);
  translationScene3d(aileg[1],tmp);
  free(tmp);

  ajoutObjet3d(aileg[0],b3g);
  aileg[2]=aileg[0]->fils;

  tmp=definirPoint3d(-140,0,-140);
  translationScene3d(aileg[2],tmp);
  free(tmp);

 
  return body;
}

void Udragon(t_scene3d**tabd[5],int cycle)
{
  int i;
  float inv;
  int dec=3;
  t_point3d*tmp;
  int T1=55;
  float omega1=2*M_PI/T1;
  int T2=100;
  float omega2=2*M_PI/T2;

  //anim tete 0 (3)

  inv=(cycle/500)%3-1;
  tmp=definirPoint3d(0,0,0);
  rotationScene3d(tabd[0][1],tmp,0,0,-0.5*sin(cycle*omega2));
  rotationScene3d(tabd[0][2],tmp,0,0,0.5*sin(cycle*omega2));
  free(tmp); 

  //anim cou 1 (5)
  for(i=0;i<5;i++)
    {
      tmp=definirPoint3d(0,1.5*sin(omega1*(cycle-((7+i)*dec))),0); //valeur 7 = distance du corp en element de cou pour faire le raccord du sinus
      translationScene3d(tabd[1][i],tmp);
      free(tmp); 
    }

  //anim ailed 2 (3)

  tmp=definirPoint3d(80,-10,50);
  rotationScene3d(tabd[2][0],tmp,pow(-1,1+((cycle+20)/60)%2),0,0);
  free(tmp); 

  tmp=definirPoint3d(0,0,140);
  rotationScene3d(tabd[2][1],tmp,pow(-1,1+((cycle+20)/60)%2),0,0);
  free(tmp); 

  tmp=definirPoint3d(0,0,140);
  rotationScene3d(tabd[2][2],tmp,pow(-1,1+((cycle+20)/60)%2),0,0);
  free(tmp); 
  
  //anim aileg 3 (3)

  tmp=definirPoint3d(80,-10,-50);
  rotationScene3d(tabd[3][0],tmp,pow(-1,((cycle+20)/60)%2),0,0);
  free(tmp); 

  tmp=definirPoint3d(0,0,-140);
  rotationScene3d(tabd[3][1],tmp,pow(-1,((cycle+20)/60)%2),0,0);
  free(tmp); 

  tmp=definirPoint3d(0,0,-140);
  rotationScene3d(tabd[3][2],tmp,pow(-1,((cycle+20)/60)%2),0,0);
  free(tmp); 

 
  //anim queue 4 (12)
  for(i=0;i<12;i++)
    {
      tmp=definirPoint3d(0,1.5*sin(omega1*(cycle-i*dec)),0);
      translationScene3d(tabd[4][i],tmp);
      free(tmp);
    }
  
}

void plage_init(int nx,int nz,Uint32 tabc[nx][nz])
{
  int c,i,j;
  for(i=0;i<nx;i++){
    for(j=0;j<nz;j++){
      switch(rand()%5){
      case 0:
	tabc[i][j]=0xcc9f29;
	break;
      case 1:
	tabc[i][j]=0xc9a84f;
	break;
      case 2:
	tabc[i][j]=0xffc52e;
	break;
      case 3:
	tabc[i][j]=0xf6b301;
	break;
      case 4:
	tabc[i][j]=0xf0dca6;
	break;
      }
    }
  }
}


void mer_init(int nx,int nz,Uint32 tabc[nx][nz])
{
  int c,i,j;
  for(i=0;i<nx;i++){
    for(j=0;j<nz;j++){
      switch(rand()%5){
      case 0:
	tabc[i][j]=0x00fff9;
	break;
      case 1:
	tabc[i][j]=0x00afb4;
	break;
      case 2:
	tabc[i][j]=0x009489;
	break;
      case 3:
	tabc[i][j]=0x086c74;
	break;
      case 4:
	tabc[i][j]=0x074d5c;
	break;
      }
    }
  }
}

Uint32 next(int lon,Uint32 val,const Uint32 tab[lon])
{
  int i;
  for(i=0;i<lon;i++)
    if(val==tab[i])
      return tab[i+1];
}

const Uint32 colmer[37]={0x00FFF9,0x00f2ed,0x00e4e2,0x00d7d7,0x00cacb,0x00bcc0, \
			 0x00AFB4,0x00abad,0x00a6a6,0x00a19e,0x009d97,0x009890, \
			 0x009489,0x018d85,0x038782,0x04807e,0x05797b,0x077378, \
			 0x086C74,0x086770,0x08626c,0x085d68,0x075764,0x075260, \
			 0x074D5C,0x065c69,0x066b76,0x057a83,0x058890,0x04979d, \
			 0x04a6ab,0x03b5b8,0x02c4c5,0x02d3d2,0x01e1df,0x01f0ec, \
			 0x00FFF9};



void Umer(int nx,int nz,int freqx,int freqz,t_scene3d*tab[nx][nz],int cycle,Uint32 tabc[nx][nz])
{
  t_point3d*tmp;
  int i,j;
  for(i=0;i<nx;i++){
    for(j=0;j<nz;j++){
      tmp=definirPoint3d(0,sin(2*M_PI*((cycle/2-i)%freqx)/freqx)+cos(2*M_PI*((cycle/2-j)%freqz)/freqz),0);
      translationScene3d(tab[i][j],tmp);
      free(tmp);
      //if(cycle%2)
      tabc[i][j]=next(37,tabc[i][j],colmer);
    }
  }
}



t_scene3d*mer(double lx,double lz,int nx,int nz,Uint32 tabc[nx][nz],t_scene3d*tab[nx][nz])
{
  t_objet3d*plan=objet_vide();
  
  t_scene3d*main=creerScene3d(plan);
  int i,j;
  t_objet3d*tmpo;
  t_point3d*tmp;
  double ux=lx/nx,uz=lz/nz;

  for(i=0;i<nx;i++)
    {
      for(j=0;j<nz;j++)
	{
	  tmpo=parallelepipede(ux,5,uz,&(tabc[i][j]));
	  ajoutObjet3d(main,tmpo);
	  tab[i][j]=main->fils;
	  
	  tmp=definirPoint3d((i+0.5)*ux-lx/2,0,(j+0.5)*uz-lz/2);
	  translationScene3d(tab[i][j],tmp);
	  free(tmp);
	}
    }

  return main;
}


void deco_init(int nx,Uint32 tabc[nx])
{
  int c,i;
  for(i=0;i<nx;i++){
    switch(rand()%5){
    case 0:
      tabc[i]=0x856539;
      break;
    case 1:
      tabc[i]=0x8d5622;
      break;
    case 2:
      tabc[i]=0x80461b;
      break;
    case 3:
      tabc[i]=0x5d2c04;
      break;
    case 4:
      tabc[i]=0x4e3119;
      break;
    }
  }
}

void feuill_init(int nx,Uint32 tabc[nx])
{
  int c,i;
  for(i=0;i<nx;i++){
    switch(rand()%3){
    case 0:
      tabc[i]=0x69b64f;
      break;
    case 1:
      tabc[i]=0x497f37;
      break;
    case 2:
      tabc[i]=0x345b27;
      break;
    }
  }
}



void deco(int sec,Uint32 tabdec[sec],t_scene3d*tab[sec],double r,double rapr,double lh,double raph)
{
  int i;
  t_objet3d*tmpo;
  t_point3d*tmp;

  tmpo=cylindre(lh,r,20,&(tabdec[0]));
  tab[0]=creerScene3d(tmpo);
  
  for(i=1;i<sec;i++)
    {
      tmpo=cylindre(lh*pow(raph,i),r*pow(rapr,i),20,&(tabdec[i]));
      ajoutObjet3d(tab[i-1],tmpo);
      tab[i]=tab[i-1]->fils;
          
      tmp=definirPoint3d(0,pow(raph,i)-(lh*(pow(raph,i)+pow(raph,i-1)))/2.0,0);
      translationScene3d(tab[i],tmp);
      free(tmp);
    }
}



void Uplage(t_scene3d*plage,t_bool btab[4],int cycle)
{
  t_point3d*tmp;
  tmp=GetcentreR(plage);
  rotationScene3d(plage,tmp,0,0,-2);
  rotationScene3d(plage->fils,tmp,0,0.333,0);
  free(tmp);
  if(cycle%90==0){
    btab[3]=false;   //mouvement fini
    if(btab[2])      //actualisation de la position avec un toggle
      btab[2]=false; 
    else
      btab[2]=true;
  }
}

void palmier_init(int sec,t_scene3d*tab[sec],double lh,double raph)
{
  int i=0;
  t_point3d*tmp;
  tmp=definirPoint3d(0,-lh/2,0);
  rotationScene3d(tab[i],tmp,5,0,0);
  free(tmp);
  for(i=1;i<sec;i++){
    tmp=definirPoint3d(0,-lh*pow(raph,i-1)/2.0,0);
    rotationScene3d(tab[i],tmp,-0.75,0.5,0.95);
    free(tmp);
  }
}


void Upalmier(t_bool btab[4],int n1,int n2,t_scene3d*tab1[n1],t_scene3d*tab2[n2],int cycle)
{
  int i;
  int inv;
  t_point3d*tmp;
  t_point3d*origine=definirPoint3d(0,0,0);
  double omega=2*M_PI/500;

  if(btab[1]){//version fun !
    for(i=0;i<n2;i++){
      rotationScene3d(tab2[i],origine,0,15,0);
    }
    
    for(i=1;i<n1;i++){
      rotationScene3d(tab1[i],origine,(inv+2)%3*0.25*sin(omega*2*(cycle+200)),(inv+1)%3*0.05*sin(omega*2*(cycle+400)),inv*0.25*sin(omega*2*cycle));
    }
  }
  else{
    for(i=0;i<n2;i++){
      inv=(cycle/500)%3-1;
      rotationScene3d(tab2[i],origine,(inv+2)%3*0.05*sin(omega*(cycle+200)),(inv+1)%3*0.05*sin(omega*(cycle+400)),inv*0.05*sin(omega*cycle));
    }
    for(i=1;i<n1;i++){
      rotationScene3d(tab1[i],origine,(inv+2)%3*0.01*sin(omega*(cycle+200)),(inv+1)%3*0.01*sin(omega*(cycle+400)),inv*0.01*sin(omega*cycle));
    }
  }
}


void ksolp(t_scene3d*sn,t_bool btabs[3],int csolp)
{
  t_point3d*tmp;
  if(btabs[0])
    tmp=definirPoint3d(0,-5,0);
  else
    tmp=definirPoint3d(0,5,0);
  translationScene3d(sn,tmp);
  if(csolp%100==0){
    btabs[1]=false;   //mouvement fini
    if(btabs[0])      //actualisation de la position avec un toggle
      btabs[0]=false; 
    else
      btabs[0]=true;
  }
}

void kapp(t_scene3d*baset,t_bool btabk[3],int ckapp)
{
  t_point3d*tmp;
  if(btabk[0])
    tmp=definirPoint3d(0,5,0);
  else
    tmp=definirPoint3d(0,-5,0);
  translationScene3d(baset,tmp);
  if(ckapp%50==0){
    btabk[1]=false;   //mouvement fini
    if(btabk[0])      //actualisation de la position avec un toggle
      btabk[0]=false; 
    else
      btabk[0]=true;
  }
}

t_scene3d*tentacle(int nb,int sec,Uint32 tabc[nb][sec],t_scene3d*tab[nb][sec],double r,double rapr,double lh,double raph)
{
  t_objet3d*plan=objet_vide();
  t_scene3d*main=creerScene3d(plan);
  int i,j;
  t_objet3d*tmpo;
  t_point3d*tmp;
  int rayon=150;
  
  for(i=0;i<nb;i++)
    {
      j=0;
      tmpo=cylindre(lh,r,20,&(tabc[i][j]));
      ajoutObjet3d(main,tmpo);
      tab[i][j]=main->fils;
      
      tmp=definirPoint3d(-rayon*sin(i*2*M_PI/nb),0,rayon*cos(i*2*M_PI/nb));
      translationScene3d(tab[i][j],tmp);
      free(tmp);
      for(j=1;j<sec;j++)
	{
	  tmpo=cylindre(lh*pow(raph,j),r*pow(rapr,j),20,&(tabc[i][j]));
	  ajoutObjet3d(tab[i][j-1],tmpo);
	  tab[i][j]=tab[i][j-1]->fils;
	  
	  tmp=definirPoint3d(0,5*pow(raph,j)-(lh*(pow(raph,j)+pow(raph,j-1)))/2.0,0);
	  translationScene3d(tab[i][j],tmp);
	  free(tmp);
	 
	}
    }
  return main;	  
}

void kraken_init(int nb,int sec,Uint32 tabc[nb][sec])
{
  int c,i,j;
  for(i=0;i<nb;i++){
    for(j=0;j<sec;j++){
      switch(rand()%5){
      case 0:
	tabc[i][j]=0xc62828;
	break;
      case 1:
	tabc[i][j]=0xb10f0f;
	break;
      case 2:
	tabc[i][j]=0x951212;
	break;
      case 3:
	tabc[i][j]=0x731010;
	break;
      case 4:
	tabc[i][j]=0x5b0707;
	break;
      }
    }
  }
}



void Ukraken(int nb,int sec,t_scene3d*tab[nb][sec],int kinfo[nb][sec],double lh,double raph,int cycle)
{
  int i,j;
  t_point3d*tmp,*dy;
  float rx,rz;
  int T=100;
  float omega=2*M_PI/T;
  float omega2=omega/1.4;
  int dec1=16;
  int dec2=50;
  int inv=1;
  float somsin;
  float roty=0;
  for(i=0;i<nb;i++)
    {

      if((cycle-i*dec2)%T==0)
	{
	  kinfo[i][0]=(kinfo[i][0]+rand()%101-50)%360;
	}
      if((cycle-i*dec2)%dec1==0)
	{
	  for(j=sec-1;j>0;j--)
	    {
	      kinfo[i][j]=kinfo[i][j-1];
	    }
	}
      somsin=sin(omega2*((cycle-i*dec2)/2+1))*sin(omega2*(cycle-i*dec2)/2)/sin(omega2/2); //formule de la somme de sinus by constance !
      for(j=0;j<sec;j++)
	{
	  if(j==0){
	    dy=definirPoint3d(0,sin(omega2*(cycle-i*dec2)),0);
	    translationScene3d(tab[i][j],dy);
	    free(dy);
	    tmp=definirPoint3d(-120*sin(i*2*M_PI/nb),lh/2.0+somsin,120*cos(i*2*M_PI/nb));
	    roty=0;
	  }
	  else{
	    roty=0;
	    tmp=definirPoint3d(0,-lh*pow(raph,j-1)/2.0,0);
	  }
	  if(cycle-i*dec2-dec1*j>=0){
	    inv=pow(-1,((cycle-i*dec2-dec1*j)/T)%2);

	    rotationScene3d(tab[i][j],tmp,inv*cos(kinfo[i][j]*M_PI/180)*sin(omega*(cycle-i*dec2-dec1*j)),roty,inv*sin(kinfo[i][j]*M_PI/180)*sin(omega*(cycle-i*dec2-dec1*j)));
	  }
	  free(tmp);
	}
    }
}
