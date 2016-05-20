#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "lib_surface.h"
#include "lib_2d.h"
#include "lib_3d.h"
#include "lib_objet3d.h"
#include "lib_scene3d.h"


//#define T2D
//#define T3D
//#define O3D
#define S3D

int main(int argc,char** argv)
{
  srand(time(NULL));
  t_surface *surface=NULL;
  int cpt = 0;
  int timestart;
  char buf[50];
  SDL_Event event;
  t_bool fin=true;
  int turnlr=0,turnhb=0;
  int db1,db2;
#ifdef T2D

#endif

#ifdef T3D
 
#endif

#ifdef O3D
 
#endif
  
#ifdef S3D
  t_bool is_button_down=false;
  double x,y;
  t_point3d *origine = definirPoint3d(0,0,0);
  t_point3d *vecteur,*centre;
  t_point3d *unitaire = definirPoint3d(1,1,1);
  t_point3d *translation;
  double Tx=0,Ty=0,Tz=0;
  double Rx=0,Ry=0,Rz=0;

  int dx=0,dy=0,dz=0;
  int drx=0,dry=0,drz=0;

  double mat[4][4];
  double inv[4][4];

  double tmpmat[4][4];
  double tmpinv[4][4];

  double h = -500;
  
  t_objet3d*c1=camera();

  t_objet3d*p1=objet_vide();//damier(800,800,16,16,NULL,NULL);
  
  t_scene3d*scene;

  t_scene3d*sc1=creerScene3d(c1);
  ajoutObjet3d(sc1,p1);
  t_scene3d*sp1=sc1->fils;

  scene=sc1;



  //dragon ?
  int cdragon=0;
  t_scene3d**tabd[5];
  t_scene3d*drag=dragon(tabd);
  
  ajoutfils(sp1,drag);
  

  //la mer !
  t_scene3d*tabm1[20][20];
  t_scene3d*tabm2[20][20];
  t_scene3d*tabm3[20][20];
  Uint32 tabc[20][20];
  mer_init(20,20,tabc);
  t_scene3d*rive1=mer(400,400,20,20,tabc,tabm1);
  t_scene3d*rive2=mer(400,400,20,20,tabc,tabm2);
  t_scene3d*rive3=mer(400,400,20,20,tabc,tabm3);
  int Crive=0;

  
  ajoutfils(sp1,rive1); 
  ajoutfils(sp1,rive2);
  ajoutfils(sp1,rive3);
  

  //le pont !
  t_bool btabp[3]={false,true,false}; //haut, move, anim
  t_scene3d*tabp[5];



  //soleil
  int csolp=0;
  Uint32 stabc[3]={ROUGEF,VERTF,BLEUF};
  t_bool btabs[3]={false,false,true}; //bas, move, anim
  t_objet3d*n1=n64(80);
  t_objet3d*t1=tore(85,5,10,50,&stabc[0]);//r1 r2 nlat nlong
  t_objet3d*t2=tore(100,5,10,50,&stabc[1]);
  t_objet3d*t3=tore(115,5,10,50,&stabc[2]);
  ajoutObjet3d(sp1,n1);
  t_scene3d*sn1=sp1->fils;
  ajoutObjet3d(sn1,t1);
  t_scene3d*st1=sn1->fils;
  ajoutObjet3d(st1,t2);
  t_scene3d*st2=st1->fils;
  ajoutObjet3d(st2,t3);
  t_scene3d*st3=st2->fils;


  //kraken !
  int Ctent=0;
  int ckapp=0;
  t_bool btabk[3]={false,false,true}; //haut, move, anim
  t_scene3d*tabt[8][10];
  Uint32 tabct[8][10];
  int kinfo[8][10];
  t_scene3d*baset=tentacle(8,10,tabct,tabt,20,0.7,50,0.85);
  ajoutfils(rive3,baset); 
  kraken_init(8,10,tabct);
  

  //cam 2
  t_objet3d*c2=camera();
  ajoutObjet3d(baset,c2);
  t_scene3d*sc2=baset->fils;  

  vecteur=definirPoint3d(0,150,-500);
  translationScene3d(sp1,vecteur);
  free(vecteur);
  
  vecteur=definirPoint3d(-200,-20,200);
  translationScene3d(rive1,vecteur);
  free(vecteur);

  vecteur=definirPoint3d(200,-20,200);
  translationScene3d(rive2,vecteur);
  free(vecteur);

  vecteur=definirPoint3d(200,-20,-200);
  translationScene3d(rive3,vecteur);
  free(vecteur);
  
  vecteur=definirPoint3d(0,270,0);
  translationScene3d(baset,vecteur);
  free(vecteur);

  vecteur=definirPoint3d(0,-900,0);
  translationScene3d(sn1,vecteur);
  free(vecteur);

  vecteur=definirPoint3d(0,0,0);
  rotationScene3d(st2,vecteur,0,0,90);
  free(vecteur);

  vecteur=definirPoint3d(0,0,0);
  rotationScene3d(st3,vecteur,90,0,0);
  free(vecteur);

  vecteur=definirPoint3d(0,0,0);
  rotationScene3d(sc2,vecteur,0,180,0);
  free(vecteur);

  vecteur=definirPoint3d(0,0,-400);
  translationScene3d(sc2,vecteur);
  free(vecteur);

  
  vecteur=definirPoint3d(0,-100,-500);
  translationScene3d(drag,vecteur);
  free(vecteur);

  /*
  vecteur=definirPoint3d(500,-150,0);
  translationScene3d(sc2,vecteur);
  free(vecteur);

  vecteur=definirPoint3d(200,-50,0);
  translationScene3d(sn1,vecteur);
  free(vecteur);

  vecteur=GetcentreR(sc2);
  rotationScene3d(sc2,vecteur,0,90,0);
  free(vecteur);
  */



  // affscene();


#endif


 


  int i=0;

  surface=creerFenetre(RX,RY);
  timestart = SDL_GetTicks();

  while(i<10000 && fin)
    {
      //printf("\n%deme passe:\n",i);
      init();
      effacerFenetre(surface,0);

#ifdef T2D

#endif
  
#ifdef T3D

#endif

#ifdef O3D

#endif

#ifdef S3D

      dessinerScene3d(surface,scene,h);

      SDL_PollEvent(&event);
      switch(event.type){

      case SDL_KEYDOWN:
	switch(event.key.keysym.sym){
	case SDLK_F1:
	  Racine(sc1);
	  scene=sc1;
	  break;
	case SDLK_F2:
	  Racine(sc2);
	  scene=sc2;
	  break;	  
	case SDLK_KP9:
	  dy++;
	  break;
	case SDLK_KP3:
	  dy--;
	  break;
	case SDLK_KP6:
	  dx--;
	  break;
	case SDLK_KP4:
	  dx++;
	  break;
	case SDLK_KP8:
	  dz++;
	  break;
	case SDLK_KP2:
	  dz--;
	  break;
	case SDLK_a:
	  drz++;
	  break;
	case SDLK_e:
	  drz--;
	  break;
	case SDLK_q:
	  dry--;
	  break;
	case SDLK_d:
	  dry++;
	  break;
	case SDLK_z:
	  drx++;
	  break;
	case SDLK_s:
	  drx--;
	  break;       
	case SDLK_g:
	  btabk[1]=true;
	  break;	
	case SDLK_h:
	  btabs[1]=true;
	  break;	
	case SDLK_SPACE:
	  dx=0;
	  dy=0;
	  dz=0;
	  drx=0;
	  dry=0;
	  drz=0;
	  break;
	case SDLK_ESCAPE:
	  fin=false;
	  break;
	}
	break;
      }    
      
      translation=definirPoint3d(dx,dy,dz);
      translationScene3d(scene->fils,translation);      
      free(translation);

    if(drx<0)
	{	
	  rotationScene3d(scene->fils,origine,-1,0,0);
	}
      if(dry<0)
	{
	  rotationScene3d(scene->fils,origine,0,-1,0);
	}
      if(drz<0)
	{
	  rotationScene3d(scene->fils,origine,0,0,-1);
	}
      if(drx>0)
	{
	  rotationScene3d(scene->fils,origine,1,0,0);
	}
      if(dry>0)
	{
	  rotationScene3d(scene->fils,origine,0,1,0);
	}
      if(drz>0)
	{
	  rotationScene3d(scene->fils,origine,0,0,1);
	}

      //bula=echelle_de_couleur(i);
      
      /*      
      Umer(20,20,10,10,tabm1,Crive,tabc);
      Umer(20,20,10,10,tabm2,Crive,tabc);
      Umer(20,20,10,10,tabm3,Crive,tabc);
      Crive++;
      */

      Udragon(tabd,cdragon);
      cdragon++;

      if(btabk[2] && (btabk[0] || btabk[1])){
      Ukraken(8,10,tabt,kinfo,60,0.85,Ctent);
      Ctent++;
      }

      if(btabk[1]){
	ckapp++;
	kapp(baset,btabk,ckapp);
      }

      if(btabs[2] && (btabs[0] || btabs[1])){
	centre=GetcentreR(sn1);
	rotationScene3d(sn1,centre,0,-5,0);
	free(centre);
	centre=GetcentreR(st1);
	rotationScene3d(st1,centre,5,0,0);
	free(centre);
	centre=GetcentreR(st2);
	rotationScene3d(st2,centre,0,5,0);
	free(centre);
	centre=GetcentreR(st3);
	rotationScene3d(st3,centre,0,0,-5);
	free(centre);
      }
      if(btabs[1]){
	csolp++;
	ksolp(sn1,btabs,csolp);
      }
      
#endif

      afficherFenetre(surface,screen);
      majEcran(surface);
      SDL_Delay(5);
      i++;

      cpt++;
      sprintf(buf,"%d FPS",(int)(cpt*1000.0/(SDL_GetTicks()-timestart)));
      SDL_WM_SetCaption(buf,NULL);
    }
  SDL_Quit();
  return 0;
}
