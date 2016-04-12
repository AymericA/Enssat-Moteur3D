#include "lib_scene3d.h"


t_scene3d * creerVide()
{
  t_scene3d * pt_scene3d=malloc(sizeof(t_scene3d));
  pt_scene3d->objet3d=NULL;
  pt_scene3d->lifils=NULL;
  return pt_scene3d;
}

void ajoutNoeud(t_scene3d *elem,t_scene3d *noeud,int pos)
{
  noeud->lifils=ajoutFilsNoeud(elem,noeud->lifils,pos);
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

//incorrect !
m_noeud * ajoutFilsNoeud(t_scene3d * elem,m_noeud *pm_noeud,int pos)
{
  if(pos==0){
    m_noeud*tmp=pm_noeud->pt_suiv;
    pm_noeud->pt_suiv=__creerMNoeud(elem);
    pm_noeud->pt_suiv->pt_suiv=tmp;
  }
  else
    pm_noeud->pt_suiv=ajoutFilsNoeud(elem,pm_noeud,pos-1);
  return pm_noeud;
}


void dessinerScene3d(t_surface *surface,t_scene3d * pt_scene3d,double h){
  if(pt_scene3d!=NULL){
    dessinerObjet3d(surface,pt_scene3d->objet3d,h);
    dessinerFils(surface,pt_scene3d->lifils,h);
  }
}
  
void dessinerFils(t_surface * surface,m_noeud * lifils,double h){
  if(lifils!=NULL){
    dessinerScene3d(surface,lifils->fils,h);
    dessinerFils(surface,lifils->pt_suiv,h);
  }
}
