#include "lib_scene3d.h"
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
  double mat[4][4];
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
  t_point3d * vect=definirPoint3d(0,0,0);

  pt_scene3d->objet3d=pt_objet3d;
  pt_scene3d->pere=NULL;
  pt_scene3d->fils=NULL;
  pt_scene3d->frere=NULL;

  matTranslation(vect,pt_scene3d->mat);
  matTranslation(vect,pt_scene3d->inv);
  //free(vect);  
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
printf("scene : %p(\n",scene);
  printf("bula\n");
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

t_scene3d*dragon(t_scene3d*** tab)
{
  t_scene3d* tete[1];
  t_scene3d* cou[5];
  t_scene3d* queue[12];
  t_scene3d* ailed[12];
  t_scene3d* aileg[12];
  
  t_point3d*tmp;
 
  tab[0]=tete;
  tab[1]=cou;
  tab[2]=ailed;
  tab[3]=aileg;
  tab[4]=queue;

  t_objet3d*corp=parallelepipede(640,240,240);
  t_objet3d*a1=parallelepipede(120,60,20);
  t_objet3d*a2=parallelepipede(120,60,20);
  t_objet3d*a3=parallelepipede(120,60,20);

  t_scene3d*body=creerScene3d(corp);

  ajoutObjet3d(body,a1);
  t_scene3d*ba1=body->fils;
  ajoutObjet3d(body,a2);
  t_scene3d*ba2=body->fils;
  ajoutObjet3d(body,a3);
  t_scene3d*ba3=body->fils;

  tmp=definirPoint3d(-200,-150,0);
  translationScene3d(ba1,tmp);
  //free(tmp);

  tmp=definirPoint3d(0,-150,0);
  translationScene3d(ba2,tmp);
  //free(tmp);

  tmp=definirPoint3d(200,-150,0);
  translationScene3d(ba3,tmp);
  //free(tmp);

  return body;
}
