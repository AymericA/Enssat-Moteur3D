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
    multiplicationMatrice3d(pt_scene3d->inv,inv,pt_scene3d->inv);
    transformationFils(pt_scene3d->fils,mat,inv);
  }
}


void transformationFils(t_scene3d *pt_scene3d,double mat[4][4],double inv[4][4])
{
  if(pt_scene3d!=NULL){
    multiplicationMatrice3d(pt_scene3d->mat,mat,pt_scene3d->mat);
    multiplicationMatrice3d(pt_scene3d->inv,inv,pt_scene3d->inv);
    transformationFils(pt_scene3d->fils,mat,inv);
    transformationFils(pt_scene3d->frere,mat,inv);
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
    multiplicationMatrice3d(tmpinv,inv,pt_scene3d->inv);

    transformationObjet3d(pt_scene3d->objet3d,tmpmat);
    dessinerObjet3d(surface,pt_scene3d->objet3d,h);
    transformationObjet3d(pt_scene3d->objet3d,tmpinv);

    dessinerScene3d_rec(surface,pt_scene3d->fils,htmpmat,tmpinv);
    dessinerScene3d_rec(surface,pt_scene3d->frere,h,mat,inv);
  }
}
 
