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

t_scene3d*dragon(t_scene3d** tab[5])
{
  t_scene3d* tete[3];
  t_scene3d* cou[5];
  t_scene3d* queue[12];
  t_scene3d* ailed[12];
  t_scene3d* aileg[12];

  t_scene3d* tmps;
  t_objet3d* tmpo1;
  t_objet3d* tmpo2;
  int i;  
  t_point3d*tmp;
 
  tab[0]=tete;
  tab[1]=cou;
  tab[2]=ailed;
  tab[3]=aileg;
  tab[4]=queue;

  //body + aileron

  t_objet3d*corp=parallelepipede(320,120,120,NULL);
  t_objet3d*a1=parallelepipede(60,30,10,NULL);
  t_objet3d*a2=parallelepipede(60,30,10,NULL);
  t_objet3d*a3=parallelepipede(60,30,10,NULL);

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
  
  tmpo1=parallelepipede(50,50,50,NULL);
  tmpo2=parallelepipede(30,20,10,NULL);
  
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
      tmpo1=parallelepipede(50,50,50,NULL);
      tmpo2=parallelepipede(30,20,10,NULL);

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
  
  tmpo1=parallelepipede(50,50,50,NULL);
  tmpo2=parallelepipede(30,20,10,NULL);
  
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
      tmpo1=parallelepipede(50,50,50,NULL);
      tmpo2=parallelepipede(30,20,10,NULL);

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

  tmpo1=parallelepipede(80,80,80,NULL);//tete
  ajoutObjet3d(cou[4],tmpo1);
  tete[1]=cou[4]->fils;

  tmp=definirPoint3d(65,-10,0);
  translationScene3d(tete[1],tmp);
  free(tmp);
  
  tmpo1=parallelepipede(30,20,10,NULL);//aileron gauche
  ajoutObjet3d(tete[1],tmpo1);
  tmps=tete[1]->fils;

  tmp=definirPoint3d(-5,-50,-20);
  translationScene3d(tmps,tmp);
  free(tmp);
  
  tmpo1=parallelepipede(30,20,10,NULL);//aileron droit
  ajoutObjet3d(tete[1],tmpo1);
  tmps=tete[1]->fils;
  
  tmp=definirPoint3d(-5,-50,20);
  translationScene3d(tmps,tmp);
  free(tmp);
  
  tmpo1=parallelepipede(80,25,60,NULL);//machoire sup
  ajoutObjet3d(tete[1],tmpo1);
  tete[2]=tete[1]->fils;

  tmp=definirPoint3d(80,7,0);
  translationScene3d(tete[2],tmp);
  free(tmp);

  tmpo1=parallelepipede(20,10,10,NULL);//narine gauche
  ajoutObjet3d(tete[2],tmpo1);
  tmps=tete[2]->fils;
  
  tmp=definirPoint3d(20,-17,-15);
  translationScene3d(tmps,tmp);
  free(tmp);

  tmpo1=parallelepipede(20,10,10,NULL);//narine droite
  ajoutObjet3d(tete[2],tmpo1);
  tmps=tete[2]->fils;
  
  tmp=definirPoint3d(20,-17,15);
  translationScene3d(tmps,tmp);
  free(tmp); 

  tmpo1=parallelepipede(80,20,60,NULL);//machoire inf
  ajoutObjet3d(tete[1],tmpo1);
  tete[3]=tete[1]->fils;

  tmp=definirPoint3d(80,30,0);
  translationScene3d(tete[3],tmp);
  free(tmp);

  //aile droite

  //aile gauche

  
  return body;
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



t_scene3d*tentacle(int nb,int sec,Uint32 tabc[nb][sec],t_scene3d*tab[nb][sec],double r,double rapr,double lh,double raph)
{
  t_objet3d*plan=objet_vide();
  t_scene3d*main=creerScene3d(plan);
  int i,j;
  t_objet3d*tmpo;
  t_point3d*tmp;
  
  for(i=0;i<nb;i++)
    {
      j=0;
      tmpo=cylindre(lh,r,20,&(tabc[i][j]));
      ajoutObjet3d(main,tmpo);
      tab[i][j]=main->fils;
      
      tmp=definirPoint3d(-120*sin(i*2*M_PI/nb),0,120*cos(i*2*M_PI/nb));
      translationScene3d(tab[i][j],tmp);
      free(tmp);
      for(j=1;j<sec;j++)
	{
	  tmpo=cylindre(lh*pow(raph,j),r*pow(rapr,j),20,&(tabc[i][j]));
	  ajoutObjet3d(tab[i][j-1],tmpo);
	  tab[i][j]=tab[i][j-1]->fils;
	  
	  tmp=definirPoint3d(0,5-(lh*(pow(raph,j)+pow(raph,j-1)))/2.0,0);
	  translationScene3d(tab[i][j],tmp);
	  free(tmp);
	 
	}
    }
  return main;	  
}


void Ukraken(int nb,int sec,t_scene3d*tab[nb][sec],float trtabk[nb][sec][2],double lh,double raph,int cycle)
{
  int i,j;
  t_point3d*tmp;
  int a=3;
  float rx,rz;
  for(i=0;i<nb;i++)
    {
      if(!(cycle%5)){
	rx=1;
	rz=1;

	trtabk[i][0][0]=rx;
	trtabk[i][0][1]=rz;
	/*
	trtabk[i][0][0]=trtabk[i][0][0]+rx;
	trtabk[i][0][1]=trtabk[i][0][1]+rz;
	*/
      }
      for(j=0;j<sec;j++)
	{

if(j>1){
	    trtabk[i][j][0]=trtabk[i][j-1][0];
	    trtabk[i][j][1]=trtabk[i][j-1][1];
 }
	  if(j==0){
	    tmp=definirPoint3d(-120*sin(i*2*M_PI/nb),lh/2.0,120*cos(i*2*M_PI/nb));}
	  else{
	    tmp=definirPoint3d(0,-lh*pow(raph,j-1)/2.0,0);}
	  rotationScene3d(tab[i][j],tmp,trtabk[i][(j+cycle)%sec][0],0,trtabk[i][(j+cycle)%sec][1]);
	  free(tmp);
	  
	  	}}
}
