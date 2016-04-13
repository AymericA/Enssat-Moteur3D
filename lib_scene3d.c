#include <math.h>
#include "lib_scene3d.h"



//need fonction get centre de gravité -> point (0,0,0) auquel on applique la relation racine-objet
//utiliser la fonction recherche v2






matrice * creerMatrice(double m[4][4])
{
  matrice * res=malloc(sizeof(matrice));
  int i,j;
  for(i=0;i<4;i++){
    for(j=0;j<4;j++){
      res->mat[i][j]=m[i][j];
    }}
  return res;
}


matrice * matTranslation(t_point3d *vecteur){
  double mat[4][4]={{1,0,0,vecteur->xyzt[0]},	\
		    {0,1,0,vecteur->xyzt[1]},	\
		    {0,0,1,vecteur->xyzt[2]},	\
		    {0,0,0,1}};
  return creerMatrice(mat);
}

matrice * matTranslationinv(t_point3d *vecteur){
  double mat[4][4]={{1,0,0,-vecteur->xyzt[0]},	\
		    {0,1,0,-vecteur->xyzt[1]},	\
		    {0,0,1,-vecteur->xyzt[2]},	\
		    {0,0,0,1}};
  return creerMatrice(mat);
}


matrice * matRotation(t_point3d *centre,float degreX, float degreY, float degreZ)
{
  float x,y,z;
  x=degreX*M_PI/180;
  y=degreY*M_PI/180;
  z=degreZ*M_PI/180;
  double mat[4][4];
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
  return creerMatrice(mat);
} 

matrice * matRotationinv(t_point3d *centre,float degreX, float degreY, float degreZ){
  return matRotation(centre,-degreX,-degreY,-degreZ);
}

t_point3d * getCentre(t_scene3d* pt_scene3d,int pos)
{
  double mat[4][4]={{1,0,0,0},			\
		    {0,1,0,0},			\
		    {0,0,1,0},			\
		    {0,0,0,1}};
  t_scene3d*tmp= rechercheScene3dv2(pos,pt_scene3d,mat);
  t_point3d * res= definirPoint3d(0,0,0);
  multiplicationVecteur3d(res,mat,res);
  return res;
}

void translationScene3d(t_scene3d* pt_scene3d,t_point3d *vecteur)
{
  if(vecteur!=NULL){
    double mat[4][4]={{1,0,0,vecteur->xyzt[0]},	\
		      {0,1,0,vecteur->xyzt[1]},	\
		      {0,0,1,vecteur->xyzt[2]},	\
		      {0,0,0,1}};
    transformationScene3d(pt_scene3d,mat);
  }
}

void rotationScene3d(t_scene3d* pt_scene3d,t_point3d *centre, float degreX, float degreY, float degreZ)
{
  if(centre!=NULL){
    float x,y,z;
    x=degreX*M_PI/180;
    y=degreY*M_PI/180;
    z=degreZ*M_PI/180;
    double mat[4][4];
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
    transformationScene3d(pt_scene3d,mat);
  }
}



void transformationScene3d(t_scene3d *pt_scene3d,double mat[4][4])
{
  if(pt_scene3d!=NULL){
    double m[4][4];
    multiplicationMatrice3d(m,mat,pt_scene3d->mat->mat);
    matrice*tmp=creerMatrice(m);
    free(pt_scene3d->mat);
    pt_scene3d->mat=tmp;
    transformationObjet3d(pt_scene3d->objet3d,mat);
    transformationFils(mat,pt_scene3d->lifils);
  }
}

void transformationFils(double mat[4][4],m_noeud * lifils)
{
  if(lifils!=NULL){
    transformationScene3d(lifils->fils,mat);
    transformationFils(mat,lifils->pt_suiv);
  }
}


t_scene3d * creerScene3d(t_objet3d * pt_objet3d,matrice *m1,matrice *m2,int nom)
{
  int i,j;
  t_scene3d * pt_scene3d=malloc(sizeof(t_scene3d));
  pt_scene3d->objet3d=pt_objet3d;
  pt_scene3d->nom=nom; 
  pt_scene3d->mat=m1;
  pt_scene3d->inv=m2;
  pt_scene3d->lifils=NULL;
  return pt_scene3d;
}

m_noeud * __creerMNoeud(t_scene3d * elem){
  m_noeud * res = malloc(sizeof(m_noeud));
  res->pt_suiv=NULL;
  res->fils=elem;
}

t_scene3d * rechercheScene3d(int val,t_scene3d* pt_scene3d)
{
  t_scene3d *res=NULL;
  if(pt_scene3d!=NULL){
    if(pt_scene3d->nom==val){
      return pt_scene3d;
    }
    else{
      res=rechercheFils(val,pt_scene3d->lifils);
    }
  }
  return res;
}

t_scene3d * rechercheFils(int val,m_noeud * lifils){
  t_scene3d *res=NULL;
  if(lifils!=NULL){
    res=rechercheScene3d(val,lifils->fils);
    if(res==NULL){
      res=rechercheFils(val,lifils->pt_suiv);
    }
  }
  return res;
}


t_scene3d * rechercheScene3dv2(int val,t_scene3d* pt_scene3d,double mat[4][4])
{
  t_scene3d *res=NULL;
  if(pt_scene3d!=NULL){
    if(pt_scene3d->nom==val){
      multiplicationMatrice3d(mat,mat,pt_scene3d->mat->mat);
      return pt_scene3d;
    }
    else{
      res=rechercheFilsv2(val,pt_scene3d->lifils,mat);
      if(res!=NULL){
	multiplicationMatrice3d(mat,mat,pt_scene3d->mat->mat);
      }
    }
  }
  return res;
}

t_scene3d * rechercheFilsv2(int val,m_noeud * lifils,double mat[4][4]){
  t_scene3d *res=NULL;
  if(lifils!=NULL){
    res=rechercheScene3dv2(val,lifils->fils,mat);
    if(res==NULL){
      res=rechercheFilsv2(val,lifils->pt_suiv,mat);
    }
  }
  return res;
}


void ajoutFilsNoeud(t_scene3d *pt_scene3d,t_scene3d *elem,int pos)
{
  double mat[4][4]={{1,0,0,0},			\
		    {0,1,0,0},			\
		    {0,0,1,0},			\
		    {0,0,0,1}};
  
  t_scene3d* pere=rechercheScene3dv2(pos,pt_scene3d,mat);
  if(pere!=NULL){
    multiplicationMatrice3d(mat,mat,elem->mat->mat);
    transformationObjet3d(elem->objet3d,mat);
    m_noeud *noeud=malloc(sizeof(m_noeud));
    noeud->fils=elem;
    noeud->pt_suiv=pere->lifils;
    pere->lifils=noeud;
  }
  else{
    printf("insersion fail ! bouwia \n");
  }
}


void dessinerScene3d(t_surface *surface,t_scene3d * pt_scene3d,double h){
  if(pt_scene3d!=NULL){
    //printf("dessin de objet\n");
    dessinerObjet3d(surface,pt_scene3d->objet3d,h);
    //printf("dessin de ses fils\n");
    dessinerFils(surface,pt_scene3d->lifils,h);
  }
}
  
void dessinerFils(t_surface * surface,m_noeud * lifils,double h){
  if(lifils!=NULL){
    //printf("dessin de scene\n");
    dessinerScene3d(surface,lifils->fils,h);
    //printf("dessin de ses fils\n");
    dessinerFils(surface,lifils->pt_suiv,h);
  }
}
