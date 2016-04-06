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
