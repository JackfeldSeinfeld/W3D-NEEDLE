// WL_ACT2.C

#include "WL_DEF.H"
#pragma hdrstop

/*
=============================================================================

						 LOCAL CONSTANTS

=============================================================================
*/

#define PROJECTILESIZE	0xc000l

#define BJRUNSPEED	2048
#define BJJUMPSPEED	680

#define BJROCKETSIZE 0x6000l
#define BJFLAMESIZE  0x6000l

/*
=============================================================================

						 GLOBAL VARIABLES

=============================================================================
*/

void A_Gib (objtype *ob)
{
 PlaySoundLocActor(SATISFYINGSND,ob);
}

#pragma argsused
void A_Gibbed (objtype *ob)
{
    gamestate.gibbed = false;
}

void A_SpearFall (objtype *ob)
{
	int	dx,dy,dist;

	dx = abs(ob->tilex - player->tilex);
	dy = abs(ob->tiley - player->tiley);
	dist = dx>dy ? dx : dy;
	if (dist < 7) PlaySoundLocActor(SPEARFALLSND,ob);
}

/*
=============================================================================

						 LOCAL VARIABLES

=============================================================================
*/


dirtype dirtable[9] = {northwest,north,northeast,west,nodir,east,
	southwest,south,southeast};

int	starthitpoints[5][NUMENEMIES] =
	 //
	 // BABY MODE
	 //
	 {
	 {25,	// guards
	  50,	// officer
	  100,	// SS
	  1,	// dogs
	  45,	// mutants
#ifndef SPEAR
	  850,	// Hans
	  850,	// Schabbs
	  200,	// fake hitler
	  800,	// mecha hitler
	  25,	// ghosts
	  25,	// ghosts
	  25,	// ghosts
	  25,	// ghosts

	  850,	// Gretel
	  850,	// Gift
	  850	// Fat
#else
	  5,	// en_spectre,
	  1450,	// en_angel,
	  850,	// en_mans,
	  1050,	// en_uber,
	  950,	// en_will,
	  1250,	// en_death
	  1050  // en_fried
#endif
	  },
	 //
	 // DON'T HURT ME MODE
	 //
	 {25,	// guards
	  50,	// officer
	  100,	// SS
	  1,	// dogs
	  55,	// mutants
#ifndef SPEAR
	  950,	// Hans
	  950,	// Schabbs
	  300,	// fake hitler
	  950,	// mecha hitler
	  25,	// ghosts
	  25,	// ghosts
	  25,	// ghosts
	  25,	// ghosts

	  950,	// Gretel
	  950,	// Gift
	  950	// Fat
#else
	  10,	// en_spectre,
	  1550,	// en_angel,
	  950,	// en_mans,
	  1150,	// en_uber,
	  1050,	// en_will,
	  1350,	// en_death
	  1150  // en_fried
#endif
	  },
	 //
	 // BRING 'EM ON MODE
	 //
	 {25,	// guards
	  50,	// officer
	  100,	// SS
	  1,	// dogs
	  55,	// mutants
	  
#ifndef SPEAR
	  1050,	// Hans
	  1550,	// Schabbs
	  400,	// fake hitler
	  1050,	// mecha hitler
	  25,	// ghosts
	  25,	// ghosts
	  25,	// ghosts
	  25,	// ghosts

	  1050,	// Gretel
	  1050,	// Gift
	  1050	// Fat
#else
	  15,	// en_spectre,
	  1650,	// en_angel,
	  1050,	// en_mans,
	  1375,	// en_uber,
	  1150,	// en_will,
	  1450,	// en_death
	  1300  // en_fried
#endif
	  },
	 //
	 // DEATH INCARNATE MODE
	 //
	 {25,	// guards
	  50,	// officer
	  100,	// SS
	  1,	// dogs

	  65,	// mutants
	  
#ifndef SPEAR
	  1200,	// Hans
	  2400,	// Schabbs
	  500,	// fake hitler
	  1200,	// mecha hitler
	  25,	// ghosts
	  25,	// ghosts
	  25,	// ghosts
	  25,	// ghosts

	  1200,	// Gretel
	  1200,	// Gift
	  1200	// Fat
#else
	  25,	// en_spectre,
	  2000,	// en_angel,
	  1200,	// en_mans,
	  1400,	// en_uber,
	  1300,	// en_will,
	  1600,	// en_death
	  1400  // en_fried
#endif
	  },
	 //
	 // MEIN LEBEN MODE
	 //
	 {50,	// guards
	  75,	// officer
	  125,	// SS
	  5,	// dogs

	  75,	// mutants
	  
#ifndef SPEAR
	  2400,	// Hans
	  4800,	// Schabbs
	  1000,	// fake hitler
	  2400,	// mecha hitler
	  25,	// ghosts
	  25,	// ghosts
	  25,	// ghosts
	  25,	// ghosts

	  2400,	// Gretel
	  2400,	// Gift
	  2400	// Fat
#else
	  25,	// en_spectre,
	  4000,	// en_angel,
	  2400,	// en_mans,
	  2800,	// en_uber,
	  2600,	// en_will,
	  3200,	// en_death
	  2800  // en_fried
#endif
	  }
      }
	  ;

void	A_StartDeathCam (objtype *ob);

void	T_Path (objtype *ob);
void	T_Shoot (objtype *ob);
void	T_Bite (objtype *ob);
void	T_DogChase (objtype *ob);
void	T_Chase (objtype *ob);
void    T_Will (objtype *ob);
void	T_Projectile (objtype *ob);
void	T_Stand (objtype *ob);

void A_DeathScream (objtype *ob);
void A_Explode (objtype *ob);

extern	statetype s_rocket;
extern	statetype s_smoke1;
extern	statetype s_smoke2;
extern	statetype s_smoke3;
extern	statetype s_smoke4;
extern	statetype s_boom2;
extern	statetype s_boom3;

extern	statetype s_fire1;
extern	statetype s_fire2;
extern	statetype s_fhit1;
extern	statetype s_fhit2;
extern	statetype s_fhit3;

extern  statetype s_blood1;
extern  statetype s_blood2;
extern  statetype s_blood3;
extern  statetype s_blood4;

void A_Smoke (objtype *ob);
void A_Bleed (objtype *ob);
void T_Ghosts (objtype *ob);

statetype s_rocket	 	= {true,SPR_ROCKET_1,3,T_Projectile,A_Smoke,&s_rocket};
statetype s_smoke1	 	= {false,SPR_SMOKE_1,3,NULL,NULL,&s_smoke2};
statetype s_smoke2	 	= {false,SPR_SMOKE_2,3,NULL,NULL,&s_smoke3};
statetype s_smoke3	 	= {false,SPR_SMOKE_3,3,NULL,NULL,&s_smoke4};
statetype s_smoke4	 	= {false,SPR_SMOKE_4,3,NULL,NULL,NULL};

statetype s_boom1	 	= {false,SPR_BOOM_1,6,NULL,A_Explode,&s_boom2};
statetype s_boom2	 	= {false,SPR_BOOM_2,6,NULL,NULL,&s_boom3};
statetype s_boom3	 	= {false,SPR_BOOM_3,6,NULL,NULL,NULL};

statetype s_fhit1 	= {false,SPR_FHIT1,6,NULL,NULL,&s_fhit2};
statetype s_fhit2 	= {false,SPR_FHIT2,6,NULL,NULL,&s_fhit3};
statetype s_fhit3 	= {false,SPR_FHIT3,6,NULL,NULL,NULL};

statetype s_blood1	= {false,SPR_BLOOD_1,6,NULL,NULL,&s_blood2};
statetype s_blood2	= {false,SPR_BLOOD_2,6,NULL,NULL,&s_blood3};
statetype s_blood3	= {false,SPR_BLOOD_3,6,NULL,NULL,&s_blood4};
statetype s_blood4	= {false,SPR_BLOOD_4,6,NULL,NULL,NULL};


#ifdef SPEAR

void A_Smoke (objtype *ob);

#ifndef NEEDLE
extern	statetype s_hrocket;
extern	statetype s_hsmoke1;
extern	statetype s_hsmoke2;
extern	statetype s_hsmoke3;
extern	statetype s_hsmoke4;
extern	statetype s_hboom2;
extern	statetype s_hboom3;

statetype s_hrocket	 	= {true,SPR_ROCKET_1,3,T_Projectile,A_Smoke,&s_hrocket};
statetype s_hsmoke1	 	= {false,SPR_SMOKE_1,3,NULL,NULL,&s_hsmoke2};
statetype s_hsmoke2	 	= {false,SPR_SMOKE_2,3,NULL,NULL,&s_hsmoke3};
statetype s_hsmoke3	 	= {false,SPR_SMOKE_3,3,NULL,NULL,&s_hsmoke4};
statetype s_hsmoke4	 	= {false,SPR_SMOKE_4,3,NULL,NULL,NULL};

statetype s_hboom1	 	= {false,SPR_BOOM_1,6,NULL,A_Explode,&s_hboom2};
statetype s_hboom2	 	= {false,SPR_BOOM_2,6,NULL,NULL,&s_hboom3};
statetype s_hboom3	 	= {false,SPR_BOOM_3,6,NULL,NULL,NULL};
#endif

extern	statetype s_spear1;
extern	statetype s_spear2;
extern	statetype s_spear3;
extern	statetype s_spear4;
extern	statetype s_spear5;
extern	statetype s_spear5d;
extern	statetype s_spear6;
extern	statetype s_spear7;
extern	statetype s_spear8;

statetype s_spear1	 	= {false,SPR_SPEAR_1,30,NULL,NULL,&s_spear2};
statetype s_spear2	 	= {false,SPR_SPEAR_2,3,NULL,A_SpearFall,&s_spear3};
statetype s_spear3	 	= {false,SPR_SPEAR_3,3,NULL,NULL,&s_spear4};
statetype s_spear4	 	= {false,SPR_SPEAR_4,3,NULL,NULL,&s_spear5};
statetype s_spear5	 	= {false,SPR_SPEAR_5,3,T_Ghosts,NULL,&s_spear5d};
statetype s_spear5d	 	= {false,SPR_SPEAR_5,21,T_Ghosts,NULL,&s_spear6};
statetype s_spear6	 	= {false,SPR_SPEAR_4,3,NULL,NULL,&s_spear7};
statetype s_spear7	 	= {false,SPR_SPEAR_3,3,NULL,NULL,&s_spear8};
statetype s_spear8	 	= {false,SPR_SPEAR_2,3,NULL,NULL,&s_spear1};

#endif

void	T_Schabb (objtype *ob);
void	T_SchabbThrow (objtype *ob);
void	T_Fake (objtype *ob);
void	T_FakeFire (objtype *ob);

void A_Slurpie (objtype *ob);
void A_HitlerMorph (objtype *ob);
void A_MechaSound (objtype *ob);

/*
=================
=
= A_Smoke
=
=================
*/

void A_Smoke (objtype *ob)
{
	GetNewActor ();
#ifdef SPEAR
#ifndef NEEDLE
	if (ob->obclass == hrocketobj)
		new->state = &s_hsmoke1;
	else
#endif
#endif
		new->state = &s_smoke1;
	new->ticcount = 6;

	new->tilex = ob->tilex;
	new->tiley = ob->tiley;
	new->x = ob->x;
	new->y = ob->y;
	new->obclass = inertobj;
	new->active = true;

	new->flags = FL_NEVERMARK;
}

/*
=================
=
= A_Bleed
=
=================
*/

void A_Bleed (objtype *ob)
{   
    GetNewActor ();
    
    new->state = &s_blood1;
	new->ticcount = 6;

	new->tilex = ob->tilex;
	new->tiley = ob->tiley;
	new->x = ob->x;
	new->y = ob->y;
	new->obclass = inertobj;
	new->active = true;

	new->flags = FL_NEVERMARK;
}

#if NOTVER > 0
boolean CheckIfBoss (objtype *ob)
{
    if (ob->is_boss == true) return 1;
    return 0;
}
#endif

void A_Explode (objtype *ob)
{
	objtype *op;
	long dist;
#ifndef UZISFIXES
	int damage;
#else
    byte damage;
#endif

	madenoise = true;
	dist = sqrt(pow(player->x-ob->x,2)+pow(player->y-ob->y,2));
	if (dist < (TILEGLOBAL*2))
		if (CheckLine(ob))
		{
			damage = (int)((double)30 / pow(((double)dist / (double)TILEGLOBAL),2));
			if (damage > 50)
				damage = 50;
			TakeDamage(damage,ob);
		}
	for (op = player->next; op; op = op->next)
	{
	#if NOTVER == 0
		if (op->flags & FL_SHOOTABLE && !op->flags & FL_BOSS )
		{
			dist = sqrt(pow(op->x-ob->x,2)+pow(op->y-ob->y,2));
			if (dist < (TILEGLOBAL*4))
				if (CheckLine2(op,ob))
				{
					damage = (int)((double)100 / pow(((double)dist / (double)TILEGLOBAL),2));
					if (damage > 130)
						damage = 130;
					DamageActor(op,damage);
				}
		}
	#else
        if (op->flags & FL_SHOOTABLE)
 		{
            // Is it a boss?
            if (!CheckIfBoss(op)) // If not, then kick their ass!!
            {
			dist = sqrt(pow(op->x-ob->x,2)+pow(op->y-ob->y,2));
			if (dist < (TILEGLOBAL*4))
				if (CheckLine2(op,ob))
				{
					damage = (int)((double)100 / pow(((double)dist / (double)TILEGLOBAL),2));
					if (damage > 130)
						damage = 130;
					DamageActor(op,damage);
				}
            }
            else // Otherwise... Assume the worst :3
            	return; // trolololololololololololololo!!!!!!!
		}
	#endif
    }
}

//
// The Following code was made by Codetech84
//

/*
=======================================================
=  ClipAngle:
=
=  Keeps angle in range
=======================================================
*/
int ClipAngle (int angle)
{
    while (angle>=ANGLES){ angle-=ANGLES; }
    while (angle<0){ angle+=ANGLES; }
   
   return angle;
}


/*
=======================================================
=  ObjAngle:
=
=  Returns angle from ob1 to ob2
=======================================================
*/
int ObjAngle (objtype *ob1, objtype *ob2)
{
    int deltax,deltay;
    float   angle;
    int     iangle;

    deltax = ob2->x - ob1->x;
    deltay = ob1->y - ob2->y;
   
    if (deltax == 0 && deltay == 0)
       return 0;

    angle = (float) atan2 ((float) deltay, (float) deltax);
    if (angle<0)
        angle = (float) (M_PI*2+angle);

    iangle = ClipAngle ((int)(angle/(M_PI*2)*ANGLES));

    return iangle;
}

/*
=======================================================
=  TurnToAngle:
=
=  Rotates an actor towards desired angle, as efficiently
=  as possible
=
=  dest     = Destination angle
=  turnrate = Turning speed
=======================================================
*/
void TurnToAngle (objtype *ob, int dest, int turnrate)
{
   long	deltax,deltay;
   int	iangle;
   int	countercw, clockwise, change, curangle;

   // No need to turn we are heading at the desired direction
   if (ob->angle == dest) return;

   //
   // Check most efficient turning angle
   //

   // Check which way we should turn
   if (ob->angle > dest)
   {
      countercw = ob->angle - dest;
	  clockwise = ANGLES-ob->angle + dest;
   }
   else
   {
	  clockwise = dest - ob->angle;
	  countercw = ob->angle + ANGLES-dest;
   }

   curangle = ob->angle;

   if (clockwise < countercw)
   {
      //
	  // Rotate clockwise
	  //

	  if (curangle > dest)
	  curangle -= ANGLES;

	  change = tics*turnrate;
	  if (curangle + change > dest)
         change = dest-curangle;

      curangle += change;
      ob->angle += change;

      ob->angle = ClipAngle (ob->angle);
   }
   else
   {
      //
      // Rotate counterclockwise
      //

      if (curangle < dest)
         curangle += ANGLES;

      change = -tics*turnrate;
      if (curangle + change < dest)
         change = dest-curangle;

      curangle += change;
      ob->angle += change;

      ob->angle = ClipAngle (ob->angle);
   }
}

/*
=======================================================
=  HomeToPlayer:
=
=  Rocket homing in to player
=======================================================
*/
void HomeToPlayer (objtype *ob, byte Uzi)
{
   int a = ObjAngle (ob, player);
   TurnToAngle (ob, a, Uzi);
}

/*
===================
=
= ProjectileTryMove
=
= returns true if move ok
===================
*/

#define PROJSIZE	0x2000

boolean ProjectileTryMove (objtype *ob)
{
	int			xl,yl,xh,yh,x,y;
	objtype		*check;

	xl = (ob->x-PROJSIZE) >>TILESHIFT;
	yl = (ob->y-PROJSIZE) >>TILESHIFT;

	xh = (ob->x+PROJSIZE) >>TILESHIFT;
	yh = (ob->y+PROJSIZE) >>TILESHIFT;

//
// check for solid walls
//
	for (y=yl;y<=yh;y++)
		for (x=xl;x<=xh;x++)
		{
			check = actorat[x][y];
			if (check && check<objlist)
				return false;
		}

	return true;
}



/*
=================
=
= T_Projectile
=
=================
*/

void T_Projectile (objtype *ob)
{
	long	deltax,deltay;
	int		damage;
	long	speed;
    objtype *check;

    if (ob->obclass == hrocketobj)
        HomeToPlayer (ob, 2);

	speed = (long)ob->speed*tics;

	deltax = FixedByFrac(speed,costable[ob->angle]);
	deltay = -FixedByFrac(speed,sintable[ob->angle]);

	if (deltax>0x10000l)
		deltax = 0x10000l;
	if (deltay>0x10000l)
		deltay = 0x10000l;

	ob->x += deltax;
	ob->y += deltay;

	deltax = LABS(ob->x - player->x);
	deltay = LABS(ob->y - player->y);

	if (!ProjectileTryMove (ob))
	{
#ifndef NEEDLE
		if (ob->obclass == rocketobj)
#else
		if (ob->obclass == rocketobj || ob->obclass == hrocketobj)
#endif
		{
			PlaySoundLocActor(MISSILEHITSND,ob);
			ob->state = &s_boom1;
		}
#ifdef SPEAR
#ifndef NEEDLE
		else if (ob->obclass == hrocketobj)
		{
			PlaySoundLocActor(MISSILEHITSND,ob);
			ob->state = &s_hboom1;
		}
#endif
#endif
        // Thanks KS-Presto! 27/05/2025
		else if (ob->obclass == fireobj)
		{
			PlaySoundLocActor(FLAMEHITSND,ob);
			ob->state = &s_fhit1;
		}
		else
			ob->state = NULL;		// mark for removal

		return;
	}
    if (ob->obclass == rocketobj)
    {
        for (check = player->next; check; check = check->next)
        {
            if (!CheckSpawner(check,ob) && (check->flags & FL_SHOOTABLE))
            {
                deltax = LABS(ob->x - check->x);
                deltay = LABS(ob->y - check->y);

                if (deltax < BJROCKETSIZE && deltay < BJROCKETSIZE)
                {
                    ob->state = &s_boom1;
#if NOTVER == 0
                    DamageActor (check, 150);
#else
                    if (!CheckIfBoss(check))
                       DamageActor (check, 150);
#endif
                    return;
                }
            }
        }
    }
    else if (ob->obclass == fireobj)
    {
        for (check = player->next; check; check = check->next)
        {
            if (!CheckSpawner(check,ob) && (check->flags & FL_SHOOTABLE))
            {
                deltax = LABS(ob->x - check->x);
                deltay = LABS(ob->y - check->y);

                if (deltax < BJFLAMESIZE && deltay < BJFLAMESIZE)
                {
                    ob->state = NULL;
                    DamageActor (check, 15);
                    return;
                }
            }
        }
    }
    else
    {
        deltax = LABS(ob->x - player->x);
        deltay = LABS(ob->y - player->y);

        if (deltax < PROJECTILESIZE && deltay < PROJECTILESIZE)
        {	// hit the player
            switch (ob->obclass)
            {
#ifndef SPEAR
                case needleobj:
                    damage = (US_RndT() >>3) + 20;
                    break;
#endif
                case rocketobj:
                case hrocketobj:
                    damage = (US_RndT() >>3) + 30;
                    break;
                case fireobj:
                    damage = (US_RndT() >>3);
                    break;
            }
            TakeDamage (damage,ob);
            ob->state = NULL;		// mark for removal
            return;
        }
    }
    ob->tilex = ob->x >> TILESHIFT;
    ob->tiley = ob->y >> TILESHIFT;
}




/*
=============================================================================

							GUARD

=============================================================================
*/

extern	statetype s_hgib1;
extern	statetype s_hgib2;
extern	statetype s_hgib3;
extern	statetype s_hgib4;

//
// guards
//

extern	statetype s_grdstand;

extern	statetype s_grdpath1;
extern	statetype s_grdpath1s;
extern	statetype s_grdpath2;
extern	statetype s_grdpath3;
extern	statetype s_grdpath3s;
extern	statetype s_grdpath4;

extern	statetype s_grdpain;
extern	statetype s_grdpain1;

extern	statetype s_grdshoot1;
extern	statetype s_grdshoot2;
extern	statetype s_grdshoot3;
extern	statetype s_grdshoot4;

extern	statetype s_grdchase1;
extern	statetype s_grdchase1s;
extern	statetype s_grdchase2;
extern	statetype s_grdchase3;
extern	statetype s_grdchase3s;
extern	statetype s_grdchase4;

extern	statetype s_grddie1;
extern	statetype s_grddie2;
extern	statetype s_grddie3;
extern	statetype s_grddie4;

statetype s_grdstand	= {true,SPR_GRD_S_1,0,T_Stand,NULL,&s_grdstand};

statetype s_grdpath1 	= {true,SPR_GRD_W1_1,20,T_Path,NULL,&s_grdpath1s};
statetype s_grdpath1s 	= {true,SPR_GRD_W1_1,5,NULL,NULL,&s_grdpath2};
statetype s_grdpath2 	= {true,SPR_GRD_W2_1,15,T_Path,NULL,&s_grdpath3};
statetype s_grdpath3 	= {true,SPR_GRD_W3_1,20,T_Path,NULL,&s_grdpath3s};
statetype s_grdpath3s 	= {true,SPR_GRD_W3_1,5,NULL,NULL,&s_grdpath4};
statetype s_grdpath4 	= {true,SPR_GRD_W4_1,15,T_Path,NULL,&s_grdpath1};

statetype s_grdpain 	= {2,SPR_GRD_PAIN_1,10,NULL,A_Bleed,&s_grdchase1};
statetype s_grdpain1 	= {2,SPR_GRD_PAIN_2,10,NULL,A_Bleed,&s_grdchase1};

statetype s_grdshoot1 	= {false,SPR_GRD_SHOOT1,20,NULL,NULL,&s_grdshoot2};
statetype s_grdshoot2 	= {false,SPR_GRD_SHOOT2,20,NULL,T_Shoot,&s_grdshoot3};
statetype s_grdshoot3 	= {false,SPR_GRD_SHOOT3,20,NULL,NULL,&s_grdchase1};

statetype s_grdchase1 	= {true,SPR_GRD_W1_1,10,T_Chase,NULL,&s_grdchase1s};
statetype s_grdchase1s 	= {true,SPR_GRD_W1_1,3,NULL,NULL,&s_grdchase2};
statetype s_grdchase2 	= {true,SPR_GRD_W2_1,8,T_Chase,NULL,&s_grdchase3};
statetype s_grdchase3 	= {true,SPR_GRD_W3_1,10,T_Chase,NULL,&s_grdchase3s};
statetype s_grdchase3s 	= {true,SPR_GRD_W3_1,3,NULL,NULL,&s_grdchase4};
statetype s_grdchase4 	= {true,SPR_GRD_W4_1,8,T_Chase,NULL,&s_grdchase1};

statetype s_grddie1		= {false,SPR_GRD_DIE_1,15,NULL,A_DeathScream,&s_grddie2};
statetype s_grddie2		= {false,SPR_GRD_DIE_2,15,NULL,NULL,&s_grddie3};
statetype s_grddie3		= {false,SPR_GRD_DIE_3,15,NULL,NULL,&s_grddie4};
statetype s_grddie4		= {false,SPR_GRD_DEAD,0,NULL,NULL,&s_grddie4};

statetype s_hgib1		= {false,SPR_HGIB1,10,A_Bleed,A_Gib,&s_hgib2};
statetype s_hgib2		= {false,SPR_HGIB2,10,NULL,A_Gibbed,&s_hgib3};
statetype s_hgib3		= {false,SPR_HGIB3,10,NULL,NULL,&s_hgib4};
statetype s_hgib4		= {false,SPR_HGIBBED,0,NULL,NULL,&s_hgib4};

#ifndef SPEAR
//
// ghosts
//
extern	statetype s_blinkychase1;
extern	statetype s_blinkychase2;
extern	statetype s_inkychase1;
extern	statetype s_inkychase2;
extern	statetype s_pinkychase1;
extern	statetype s_pinkychase2;
extern	statetype s_clydechase1;
extern	statetype s_clydechase2;

statetype s_blinkychase1 	= {false,SPR_BLINKY_W1,10,T_Ghosts,NULL,&s_blinkychase2};
statetype s_blinkychase2 	= {false,SPR_BLINKY_W2,10,T_Ghosts,NULL,&s_blinkychase1};

statetype s_inkychase1 		= {false,SPR_INKY_W1,10,T_Ghosts,NULL,&s_inkychase2};
statetype s_inkychase2 		= {false,SPR_INKY_W2,10,T_Ghosts,NULL,&s_inkychase1};

statetype s_pinkychase1 	= {false,SPR_PINKY_W1,10,T_Ghosts,NULL,&s_pinkychase2};
statetype s_pinkychase2 	= {false,SPR_PINKY_W2,10,T_Ghosts,NULL,&s_pinkychase1};

statetype s_clydechase1 	= {false,SPR_CLYDE_W1,10,T_Ghosts,NULL,&s_clydechase2};
statetype s_clydechase2 	= {false,SPR_CLYDE_W2,10,T_Ghosts,NULL,&s_clydechase1};
#endif

//
// dogs
//

extern	statetype s_dogpath1;
extern	statetype s_dogpath1s;
extern	statetype s_dogpath2;
extern	statetype s_dogpath3;
extern	statetype s_dogpath3s;
extern	statetype s_dogpath4;

extern	statetype s_dogjump1;
extern	statetype s_dogjump2;
extern	statetype s_dogjump3;
extern	statetype s_dogjump4;
#ifndef NEEDLE
extern	statetype s_dogjump5;
#endif

extern	statetype s_dogchase1;
extern	statetype s_dogchase1s;
extern	statetype s_dogchase2;
extern	statetype s_dogchase3;
extern	statetype s_dogchase3s;
extern	statetype s_dogchase4;

extern	statetype s_dogdie1;
extern	statetype s_dogdie2;
extern	statetype s_dogdie3;
extern	statetype s_dogdead;

statetype s_dogpath1 	= {true,SPR_DOG_W1_1,20,T_Path,NULL,&s_dogpath1s};
statetype s_dogpath1s 	= {true,SPR_DOG_W1_1,5,NULL,NULL,&s_dogpath2};
statetype s_dogpath2 	= {true,SPR_DOG_W2_1,15,T_Path,NULL,&s_dogpath3};
statetype s_dogpath3 	= {true,SPR_DOG_W3_1,20,T_Path,NULL,&s_dogpath3s};
statetype s_dogpath3s 	= {true,SPR_DOG_W3_1,5,NULL,NULL,&s_dogpath4};
statetype s_dogpath4 	= {true,SPR_DOG_W4_1,15,T_Path,NULL,&s_dogpath1};

statetype s_dogjump1 	= {false,SPR_DOG_JUMP1,10,NULL,NULL,&s_dogjump2};
statetype s_dogjump2 	= {false,SPR_DOG_JUMP2,10,NULL,T_Bite,&s_dogjump3};
statetype s_dogjump3 	= {false,SPR_DOG_JUMP3,10,NULL,NULL,&s_dogjump4};
#ifndef NEEDLE
statetype s_dogjump4 	= {false,SPR_DOG_JUMP1,10,NULL,NULL,&s_dogjump5};
statetype s_dogjump5 	= {false,SPR_DOG_W1_1,10,NULL,NULL,&s_dogchase1};
#else
statetype s_dogjump4 	= {false,SPR_DOG_JUMP1,15,NULL,NULL,&s_dogchase1};
#endif

statetype s_dogchase1 	= {true,SPR_DOG_W1_1,10,T_DogChase,NULL,&s_dogchase1s};
statetype s_dogchase1s 	= {true,SPR_DOG_W1_1,3,NULL,NULL,&s_dogchase2};
statetype s_dogchase2 	= {true,SPR_DOG_W2_1,8,T_DogChase,NULL,&s_dogchase3};
statetype s_dogchase3 	= {true,SPR_DOG_W3_1,10,T_DogChase,NULL,&s_dogchase3s};
statetype s_dogchase3s 	= {true,SPR_DOG_W3_1,3,NULL,NULL,&s_dogchase4};
statetype s_dogchase4 	= {true,SPR_DOG_W4_1,8,T_DogChase,NULL,&s_dogchase1};

statetype s_dogdie1		= {false,SPR_DOG_DIE_1,15,NULL,A_DeathScream,&s_dogdie2};
statetype s_dogdie2		= {false,SPR_DOG_DIE_2,15,NULL,NULL,&s_dogdie3};
statetype s_dogdie3		= {false,SPR_DOG_DIE_3,15,NULL,NULL,&s_dogdead};
statetype s_dogdead		= {false,SPR_DOG_DEAD,15,NULL,NULL,&s_dogdead};


//
// officers
//

extern	statetype s_ofcstand;

extern	statetype s_ofcpath1;
extern	statetype s_ofcpath1s;
extern	statetype s_ofcpath2;
extern	statetype s_ofcpath3;
extern	statetype s_ofcpath3s;
extern	statetype s_ofcpath4;

extern	statetype s_ofcpain;
extern	statetype s_ofcpain1;

extern	statetype s_ofcshoot1;
extern	statetype s_ofcshoot2;
extern	statetype s_ofcshoot3;
extern	statetype s_ofcshoot4;

extern	statetype s_ofcchase1;
extern	statetype s_ofcchase1s;
extern	statetype s_ofcchase2;
extern	statetype s_ofcchase3;
extern	statetype s_ofcchase3s;
extern	statetype s_ofcchase4;

extern	statetype s_ofcdie1;
extern	statetype s_ofcdie2;
extern	statetype s_ofcdie3;
extern	statetype s_ofcdie4;
extern	statetype s_ofcdie5;

statetype s_ofcstand	= {true,SPR_OFC_S_1,0,T_Stand,NULL,&s_ofcstand};

statetype s_ofcpath1 	= {true,SPR_OFC_W1_1,20,T_Path,NULL,&s_ofcpath1s};
statetype s_ofcpath1s 	= {true,SPR_OFC_W1_1,5,NULL,NULL,&s_ofcpath2};
statetype s_ofcpath2 	= {true,SPR_OFC_W2_1,15,T_Path,NULL,&s_ofcpath3};
statetype s_ofcpath3 	= {true,SPR_OFC_W3_1,20,T_Path,NULL,&s_ofcpath3s};
statetype s_ofcpath3s 	= {true,SPR_OFC_W3_1,5,NULL,NULL,&s_ofcpath4};
statetype s_ofcpath4 	= {true,SPR_OFC_W4_1,15,T_Path,NULL,&s_ofcpath1};

statetype s_ofcpain 	= {2,SPR_OFC_PAIN_1,10,NULL,A_Bleed,&s_ofcchase1};
statetype s_ofcpain1 	= {2,SPR_OFC_PAIN_2,10,NULL,A_Bleed,&s_ofcchase1};

statetype s_ofcshoot1 	= {false,SPR_OFC_SHOOT1,6,NULL,NULL,&s_ofcshoot2};
statetype s_ofcshoot2 	= {false,SPR_OFC_SHOOT2,20,NULL,T_Shoot,&s_ofcshoot3};
statetype s_ofcshoot3 	= {false,SPR_OFC_SHOOT3,10,NULL,NULL,&s_ofcchase1};

statetype s_ofcchase1 	= {true,SPR_OFC_W1_1,10,T_Chase,NULL,&s_ofcchase1s};
statetype s_ofcchase1s 	= {true,SPR_OFC_W1_1,3,NULL,NULL,&s_ofcchase2};
statetype s_ofcchase2 	= {true,SPR_OFC_W2_1,8,T_Chase,NULL,&s_ofcchase3};
statetype s_ofcchase3 	= {true,SPR_OFC_W3_1,10,T_Chase,NULL,&s_ofcchase3s};
statetype s_ofcchase3s 	= {true,SPR_OFC_W3_1,3,NULL,NULL,&s_ofcchase4};
statetype s_ofcchase4 	= {true,SPR_OFC_W4_1,8,T_Chase,NULL,&s_ofcchase1};

statetype s_ofcdie1		= {false,SPR_OFC_DIE_1,11,NULL,A_DeathScream,&s_ofcdie2};
statetype s_ofcdie2		= {false,SPR_OFC_DIE_2,11,NULL,NULL,&s_ofcdie3};
statetype s_ofcdie3		= {false,SPR_OFC_DIE_3,11,NULL,NULL,&s_ofcdie4};
statetype s_ofcdie4		= {false,SPR_OFC_DIE_4,11,NULL,NULL,&s_ofcdie5};
statetype s_ofcdie5		= {false,SPR_OFC_DEAD,0,NULL,NULL,&s_ofcdie5};


//
// mutant
//

extern	statetype s_mutstand;

extern	statetype s_mutpath1;
extern	statetype s_mutpath1s;
extern	statetype s_mutpath2;
extern	statetype s_mutpath3;
extern	statetype s_mutpath3s;
extern	statetype s_mutpath4;

extern	statetype s_mutpain;
extern	statetype s_mutpain1;

extern	statetype s_mutshoot1;
extern	statetype s_mutshoot2;
extern	statetype s_mutshoot3;
extern	statetype s_mutshoot4;

extern	statetype s_mutchase1;
extern	statetype s_mutchase1s;
extern	statetype s_mutchase2;
extern	statetype s_mutchase3;
extern	statetype s_mutchase3s;
extern	statetype s_mutchase4;

extern	statetype s_mutdie1;
extern	statetype s_mutdie2;
extern	statetype s_mutdie3;
extern	statetype s_mutdie4;
extern	statetype s_mutdie5;

statetype s_mutstand	= {true,SPR_MUT_S_1,0,T_Stand,NULL,&s_mutstand};

statetype s_mutpath1 	= {true,SPR_MUT_W1_1,20,T_Path,NULL,&s_mutpath1s};
statetype s_mutpath1s 	= {true,SPR_MUT_W1_1,5,NULL,NULL,&s_mutpath2};
statetype s_mutpath2 	= {true,SPR_MUT_W2_1,15,T_Path,NULL,&s_mutpath3};
statetype s_mutpath3 	= {true,SPR_MUT_W3_1,20,T_Path,NULL,&s_mutpath3s};
statetype s_mutpath3s 	= {true,SPR_MUT_W3_1,5,NULL,NULL,&s_mutpath4};
statetype s_mutpath4 	= {true,SPR_MUT_W4_1,15,T_Path,NULL,&s_mutpath1};

statetype s_mutpain 	= {2,SPR_MUT_PAIN_1,10,NULL,NULL,&s_mutchase1};
statetype s_mutpain1 	= {2,SPR_MUT_PAIN_2,10,NULL,NULL,&s_mutchase1};

statetype s_mutshoot1 	= {false,SPR_MUT_SHOOT1,6,NULL,T_Shoot,&s_mutshoot2};
statetype s_mutshoot2 	= {false,SPR_MUT_SHOOT2,20,NULL,NULL,&s_mutshoot3};
statetype s_mutshoot3 	= {false,SPR_MUT_SHOOT3,10,NULL,T_Shoot,&s_mutshoot4};
statetype s_mutshoot4 	= {false,SPR_MUT_SHOOT4,20,NULL,NULL,&s_mutchase1};

statetype s_mutchase1 	= {true,SPR_MUT_W1_1,10,T_Chase,NULL,&s_mutchase1s};
statetype s_mutchase1s 	= {true,SPR_MUT_W1_1,3,NULL,NULL,&s_mutchase2};
statetype s_mutchase2 	= {true,SPR_MUT_W2_1,8,T_Chase,NULL,&s_mutchase3};
statetype s_mutchase3 	= {true,SPR_MUT_W3_1,10,T_Chase,NULL,&s_mutchase3s};
statetype s_mutchase3s 	= {true,SPR_MUT_W3_1,3,NULL,NULL,&s_mutchase4};
statetype s_mutchase4 	= {true,SPR_MUT_W4_1,8,T_Chase,NULL,&s_mutchase1};

statetype s_mutdie1		= {false,SPR_MUT_DIE_1,7,NULL,A_DeathScream,&s_mutdie2};
statetype s_mutdie2		= {false,SPR_MUT_DIE_2,7,NULL,NULL,&s_mutdie3};
statetype s_mutdie3		= {false,SPR_MUT_DIE_3,7,NULL,NULL,&s_mutdie4};
statetype s_mutdie4		= {false,SPR_MUT_DIE_4,7,NULL,NULL,&s_mutdie5};
statetype s_mutdie5		= {false,SPR_MUT_DEAD,0,NULL,NULL,&s_mutdie5};


//
// SS
//

extern	statetype s_ssstand;

extern	statetype s_sspath1;
extern	statetype s_sspath1s;
extern	statetype s_sspath2;
extern	statetype s_sspath3;
extern	statetype s_sspath3s;
extern	statetype s_sspath4;

extern	statetype s_sspain;
extern	statetype s_sspain1;

extern	statetype s_ssshoot1;
extern	statetype s_ssshoot2;
extern	statetype s_ssshoot3;
extern	statetype s_ssshoot4;
extern	statetype s_ssshoot5;
extern	statetype s_ssshoot6;
extern	statetype s_ssshoot7;
extern	statetype s_ssshoot8;
extern	statetype s_ssshoot9;

extern	statetype s_sschase1;
extern	statetype s_sschase1s;
extern	statetype s_sschase2;
extern	statetype s_sschase3;
extern	statetype s_sschase3s;
extern	statetype s_sschase4;

extern	statetype s_ssdie1;
extern	statetype s_ssdie2;
extern	statetype s_ssdie3;
extern	statetype s_ssdie4;

statetype s_ssstand	= {true,SPR_SS_S_1,0,T_Stand,NULL,&s_ssstand};

statetype s_sspath1 	= {true,SPR_SS_W1_1,20,T_Path,NULL,&s_sspath1s};
statetype s_sspath1s 	= {true,SPR_SS_W1_1,5,NULL,NULL,&s_sspath2};
statetype s_sspath2 	= {true,SPR_SS_W2_1,15,T_Path,NULL,&s_sspath3};
statetype s_sspath3 	= {true,SPR_SS_W3_1,20,T_Path,NULL,&s_sspath3s};
statetype s_sspath3s 	= {true,SPR_SS_W3_1,5,NULL,NULL,&s_sspath4};
statetype s_sspath4 	= {true,SPR_SS_W4_1,15,T_Path,NULL,&s_sspath1};

statetype s_sspain 		= {2,SPR_SS_PAIN_1,10,NULL,A_Bleed,&s_sschase1};
statetype s_sspain1 	= {2,SPR_SS_PAIN_2,10,NULL,A_Bleed,&s_sschase1};

statetype s_ssshoot1 	= {false,SPR_SS_SHOOT1,20,NULL,NULL,&s_ssshoot2};
statetype s_ssshoot2 	= {false,SPR_SS_SHOOT2,20,NULL,T_Shoot,&s_ssshoot3};
statetype s_ssshoot3 	= {false,SPR_SS_SHOOT3,5,NULL,NULL,&s_ssshoot4};
statetype s_ssshoot4 	= {false,SPR_SS_SHOOT2,8,NULL,T_Shoot,&s_ssshoot5};
statetype s_ssshoot5 	= {false,SPR_SS_SHOOT3,5,NULL,NULL,&s_ssshoot6};
statetype s_ssshoot6 	= {false,SPR_SS_SHOOT2,8,NULL,T_Shoot,&s_ssshoot7};
statetype s_ssshoot7  	= {false,SPR_SS_SHOOT3,5,NULL,NULL,&s_ssshoot8};
statetype s_ssshoot8  	= {false,SPR_SS_SHOOT2,8,NULL,T_Shoot,&s_ssshoot9};
statetype s_ssshoot9  	= {false,SPR_SS_SHOOT3,5,NULL,NULL,&s_sschase1};

statetype s_sschase1 	= {true,SPR_SS_W1_1,10,T_Chase,NULL,&s_sschase1s};
statetype s_sschase1s 	= {true,SPR_SS_W1_1,3,NULL,NULL,&s_sschase2};
statetype s_sschase2 	= {true,SPR_SS_W2_1,8,T_Chase,NULL,&s_sschase3};
statetype s_sschase3 	= {true,SPR_SS_W3_1,10,T_Chase,NULL,&s_sschase3s};
statetype s_sschase3s 	= {true,SPR_SS_W3_1,3,NULL,NULL,&s_sschase4};
statetype s_sschase4 	= {true,SPR_SS_W4_1,8,T_Chase,NULL,&s_sschase1};

statetype s_ssdie1		= {false,SPR_SS_DIE_1,15,NULL,A_DeathScream,&s_ssdie2};
statetype s_ssdie2		= {false,SPR_SS_DIE_2,15,NULL,NULL,&s_ssdie3};
statetype s_ssdie3		= {false,SPR_SS_DIE_3,15,NULL,NULL,&s_ssdie4};
statetype s_ssdie4		= {false,SPR_SS_DEAD,0,NULL,NULL,&s_ssdie4};


#ifndef SPEAR
//
// hans
//
extern	statetype s_bossstand;

extern	statetype s_bosschase1;
extern	statetype s_bosschase1s;
extern	statetype s_bosschase2;
extern	statetype s_bosschase3;
extern	statetype s_bosschase3s;
extern	statetype s_bosschase4;

extern	statetype s_bossdie1;
extern	statetype s_bossdie2;
extern	statetype s_bossdie3;
extern	statetype s_bossdie4;

extern	statetype s_bossshoot1;
extern	statetype s_bossshoot2;
extern	statetype s_bossshoot3;
extern	statetype s_bossshoot4;
extern	statetype s_bossshoot5;
extern	statetype s_bossshoot6;
extern	statetype s_bossshoot7;
extern	statetype s_bossshoot8;


statetype s_bossstand	= {false,SPR_BOSS_W1,0,T_Stand,NULL,&s_bossstand};

statetype s_bosschase1 	= {false,SPR_BOSS_W1,10,T_Chase,NULL,&s_bosschase1s};
statetype s_bosschase1s	= {false,SPR_BOSS_W1,3,NULL,NULL,&s_bosschase2};
statetype s_bosschase2 	= {false,SPR_BOSS_W2,8,T_Chase,NULL,&s_bosschase3};
statetype s_bosschase3 	= {false,SPR_BOSS_W3,10,T_Chase,NULL,&s_bosschase3s};
statetype s_bosschase3s	= {false,SPR_BOSS_W3,3,NULL,NULL,&s_bosschase4};
statetype s_bosschase4 	= {false,SPR_BOSS_W4,8,T_Chase,NULL,&s_bosschase1};

statetype s_bossdie1	= {false,SPR_BOSS_DIE1,15,NULL,A_DeathScream,&s_bossdie2};
statetype s_bossdie2	= {false,SPR_BOSS_DIE2,15,NULL,NULL,&s_bossdie3};
statetype s_bossdie3	= {false,SPR_BOSS_DIE3,15,NULL,NULL,&s_bossdie4};
statetype s_bossdie4	= {false,SPR_BOSS_DEAD,0,NULL,NULL,&s_bossdie4};

statetype s_bossshoot1 	= {false,SPR_BOSS_SHOOT1,30,NULL,NULL,&s_bossshoot2};
statetype s_bossshoot2 	= {false,SPR_BOSS_SHOOT2,10,NULL,T_Shoot,&s_bossshoot3};
statetype s_bossshoot3 	= {false,SPR_BOSS_SHOOT3,10,NULL,T_Shoot,&s_bossshoot4};
statetype s_bossshoot4 	= {false,SPR_BOSS_SHOOT2,10,NULL,T_Shoot,&s_bossshoot5};
statetype s_bossshoot5 	= {false,SPR_BOSS_SHOOT3,10,NULL,T_Shoot,&s_bossshoot6};
statetype s_bossshoot6 	= {false,SPR_BOSS_SHOOT2,10,NULL,T_Shoot,&s_bossshoot7};
statetype s_bossshoot7 	= {false,SPR_BOSS_SHOOT3,10,NULL,T_Shoot,&s_bossshoot8};
statetype s_bossshoot8 	= {false,SPR_BOSS_SHOOT1,10,NULL,NULL,&s_bosschase1};


//
// gretel
//
extern	statetype s_gretelstand;

extern	statetype s_gretelchase1;
extern	statetype s_gretelchase1s;
extern	statetype s_gretelchase2;
extern	statetype s_gretelchase3;
extern	statetype s_gretelchase3s;
extern	statetype s_gretelchase4;

extern	statetype s_greteldie1;
extern	statetype s_greteldie2;
extern	statetype s_greteldie3;
extern	statetype s_greteldie4;

extern	statetype s_gretelshoot1;
extern	statetype s_gretelshoot2;
extern	statetype s_gretelshoot3;
extern	statetype s_gretelshoot4;
extern	statetype s_gretelshoot5;
extern	statetype s_gretelshoot6;
extern	statetype s_gretelshoot7;
extern	statetype s_gretelshoot8;


statetype s_gretelstand	= {false,SPR_GRETEL_W1,0,T_Stand,NULL,&s_gretelstand};

statetype s_gretelchase1 	= {false,SPR_GRETEL_W1,10,T_Chase,NULL,&s_gretelchase1s};
statetype s_gretelchase1s	= {false,SPR_GRETEL_W1,3,NULL,NULL,&s_gretelchase2};
statetype s_gretelchase2 	= {false,SPR_GRETEL_W2,8,T_Chase,NULL,&s_gretelchase3};
statetype s_gretelchase3 	= {false,SPR_GRETEL_W3,10,T_Chase,NULL,&s_gretelchase3s};
statetype s_gretelchase3s	= {false,SPR_GRETEL_W3,3,NULL,NULL,&s_gretelchase4};
statetype s_gretelchase4 	= {false,SPR_GRETEL_W4,8,T_Chase,NULL,&s_gretelchase1};

statetype s_greteldie1	= {false,SPR_GRETEL_DIE1,15,NULL,A_DeathScream,&s_greteldie2};
statetype s_greteldie2	= {false,SPR_GRETEL_DIE2,15,NULL,NULL,&s_greteldie3};
statetype s_greteldie3	= {false,SPR_GRETEL_DIE3,15,NULL,NULL,&s_greteldie4};
statetype s_greteldie4	= {false,SPR_GRETEL_DEAD,0,NULL,NULL,&s_greteldie4};

statetype s_gretelshoot1 	= {false,SPR_GRETEL_SHOOT1,30,NULL,NULL,&s_gretelshoot2};
statetype s_gretelshoot2 	= {false,SPR_GRETEL_SHOOT2,10,NULL,T_Shoot,&s_gretelshoot3};
statetype s_gretelshoot3 	= {false,SPR_GRETEL_SHOOT3,10,NULL,T_Shoot,&s_gretelshoot4};
statetype s_gretelshoot4 	= {false,SPR_GRETEL_SHOOT2,10,NULL,T_Shoot,&s_gretelshoot5};
statetype s_gretelshoot5 	= {false,SPR_GRETEL_SHOOT3,10,NULL,T_Shoot,&s_gretelshoot6};
statetype s_gretelshoot6 	= {false,SPR_GRETEL_SHOOT2,10,NULL,T_Shoot,&s_gretelshoot7};
statetype s_gretelshoot7 	= {false,SPR_GRETEL_SHOOT3,10,NULL,T_Shoot,&s_gretelshoot8};
statetype s_gretelshoot8 	= {false,SPR_GRETEL_SHOOT1,10,NULL,NULL,&s_gretelchase1};
#endif


/*
===============
=
= SpawnStand
=
===============
*/

void SpawnStand (enemy_t which, int tilex, int tiley, int dir)
{
	unsigned	far *map,tile;

	switch (which)
	{
	case en_guard:
		SpawnNewObj (tilex,tiley,&s_grdstand);
		new->speed = SPDPATROL;
		if (!loadedgame)
		  gamestate.killtotal++;
		break;

	case en_officer:
		SpawnNewObj (tilex,tiley,&s_ofcstand);
		new->speed = SPDPATROL;
		if (!loadedgame)
		  gamestate.killtotal++;
		break;

	case en_mutant:
		SpawnNewObj (tilex,tiley,&s_mutstand);
		new->speed = SPDPATROL;
		if (!loadedgame)
		  gamestate.killtotal++;
		break;

	case en_ss:
		SpawnNewObj (tilex,tiley,&s_ssstand);
		new->speed = SPDPATROL;
		if (!loadedgame)
		  gamestate.killtotal++;
		break;
	}


	map = mapsegs[0]+farmapylookup[tiley]+tilex;
	if (*map == AMBUSHTILE)
	{
		tilemap[tilex][tiley] = 0;

		if (*(map+1) >= AREATILE)
			tile = *(map+1);
		if (*(map-mapwidth) >= AREATILE)
			tile = *(map-mapwidth);
		if (*(map+mapwidth) >= AREATILE)
			tile = *(map+mapwidth);
		if ( *(map-1) >= AREATILE)
			tile = *(map-1);

		*map = tile;
		new->areanumber = tile-AREATILE;

		new->flags |= FL_AMBUSH;
	}

	new->obclass = guardobj+which;
	new->hitpoints = starthitpoints[gamestate.difficulty][which];
	new->dir = dir*2;
	new->flags |= FL_SHOOTABLE;
}

#ifdef SPEAR

#define SPSPEAR     1
void SpawnSpear (int tilex, int tiley)
{
	unsigned	far *map,tile;

	SpawnNewObj (tilex,tiley,&s_spear1);

	new->obclass = spearobj;
	new->speed = SPSPEAR;

	new->dir = east;
	new->flags |= FL_AMBUSH;
	if (!loadedgame)
	{
	  gamestate.killtotal++;
	  gamestate.killcount++;
	}
}

#endif

/*
===============
=
= SpawnDeadGuard
=
===============
*/

void SpawnDeadGuard (int tilex, int tiley)
{
	SpawnNewObj (tilex,tiley,&s_grddie4);
	new->obclass = inertobj;
}

/*
===============
=
= SpawnPatrol
=
===============
*/

void SpawnPatrol (enemy_t which, int tilex, int tiley, int dir)
{
	switch (which)
	{
	case en_guard:
		SpawnNewObj (tilex,tiley,&s_grdpath1);
		new->speed = SPDPATROL;
		if (!loadedgame)
		  gamestate.killtotal++;
		break;

	case en_officer:
		SpawnNewObj (tilex,tiley,&s_ofcpath1);
		new->speed = SPDPATROL;
		if (!loadedgame)
		  gamestate.killtotal++;
		break;

	case en_ss:
		SpawnNewObj (tilex,tiley,&s_sspath1);
		new->speed = SPDPATROL;
		if (!loadedgame)
		  gamestate.killtotal++;
		break;

	case en_mutant:
		SpawnNewObj (tilex,tiley,&s_mutpath1);
		new->speed = SPDPATROL;
		if (!loadedgame)
		  gamestate.killtotal++;
		break;

	case en_dog:
		SpawnNewObj (tilex,tiley,&s_dogpath1);
		new->speed = SPDDOG;
		if (!loadedgame)
		  gamestate.killtotal++;
		break;
	}

	new->obclass = guardobj+which;
	new->dir = dir*2;
	new->hitpoints = starthitpoints[gamestate.difficulty][which];
	new->distance = tileglobal;
	new->flags |= FL_SHOOTABLE;
	new->active = true;

	actorat[new->tilex][new->tiley] = NULL;		// don't use original spot

	switch (dir)
	{
	case 0:
		new->tilex++;
		break;
	case 1:
		new->tiley--;
		break;
	case 2:
		new->tilex--;
		break;
	case 3:
		new->tiley++;
		break;
	}

	actorat[new->tilex][new->tiley] = new;
}



/*
==================
=
= A_DeathScream
=
==================
*/

void A_DeathScream (objtype *ob)
{
#ifndef UPLOAD
#ifndef SPEAR
	if (mapon==9 && !US_RndT())
#else
#ifndef NEEDLE
	if ((mapon==18 || mapon==19) && !US_RndT())
#else
	if (mapon<TESMAP && mapon >= SECRET1)	
#endif
	{
	 switch(ob->obclass)
	 {
	  case mutantobj:
	  case guardobj:
	  case officerobj:
	  case ssobj:
	  case dogobj:
		PlaySoundLocActor(DEATHSCREAM6SND,ob);
		return;
	 }
	}
#endif
#endif

	switch (ob->obclass)
	{
	case mutantobj:
		PlaySoundLocActor(AHHHGSND,ob);
		break;

	case guardobj:
		{
		 int sounds[9]={ DEATHSCREAM1SND,
				 DEATHSCREAM2SND,
				 DEATHSCREAM3SND,
				 DEATHSCREAM4SND,
				 DEATHSCREAM5SND,
				 DEATHSCREAM7SND,
				 DEATHSCREAM8SND,
				 DEATHSCREAM9SND
				 };

		 #ifndef UPLOAD
		 PlaySoundLocActor(sounds[US_RndT()%8],ob);
		 #else
		 PlaySoundLocActor(sounds[US_RndT()%2],ob);
		 #endif
		}
		break;
	case officerobj:
		PlaySoundLocActor(NEINSOVASSND,ob);
		break;
	case ssobj:
		PlaySoundLocActor(LEBENSND,ob);	// JAB
		break;
	case dogobj:
		PlaySoundLocActor(DOGDEATHSND,ob);	// JAB
		break;
#ifndef SPEAR
	case bossobj:
		SD_PlaySound(MUTTISND);				// JAB
		break;
	case schabbobj:
		SD_PlaySound(MEINGOTTSND);
		break;
	case fakeobj:
		SD_PlaySound(HITLERHASND);
		break;
	case mechahitlerobj:
		SD_PlaySound(SCHEISTSND);
		break;
	case realhitlerobj:
		SD_PlaySound(EVASND);
		break;
	case gretelobj:
		SD_PlaySound(MEINSND);
		break;
	case giftobj:
		SD_PlaySound(DONNERSND);
		break;
	case fatobj:
		SD_PlaySound(ROSESND);
		break;
#else
	case spectreobj:
		SD_PlaySound(GHOSTFADESND);
		break;
	case angelobj:
		SD_PlaySound(ANGELDEATHSND);
		break;
	case mansobj:
		SD_PlaySound(MANSDEATHSND);
		break;
	case uberobj:
		SD_PlaySound(SAKURAIDEATHSND);
		break;
	case willobj:
		SD_PlaySound(WILHELMDEATHSND);
		break;
	case deathobj:
		SD_PlaySound(KNIGHTDEATHSND);
		break;
	case friedobj:
        SD_PlaySound(SCHAISERSND);
        break;
#endif
	}
}


/*
=============================================================================

						 SPEAR ACTORS

=============================================================================
*/

#ifdef SPEAR

void T_Launch (objtype *ob);

extern	statetype s_angelshoot1;
extern	statetype s_deathshoot1;
extern	statetype s_spark1;

//
// mans
//
extern	statetype s_mansstand;

extern	statetype s_manschase1;
extern	statetype s_manschase1s;
extern	statetype s_manschase2;
extern	statetype s_manschase3;
extern	statetype s_manschase3s;
extern	statetype s_manschase4;

extern	statetype s_mansdie0;
extern	statetype s_mansdie01;
extern	statetype s_mansdie1;
extern	statetype s_mansdie2;
extern	statetype s_mansdie3;
extern	statetype s_mansdie4;

extern	statetype s_mansshoot1;
extern	statetype s_mansshoot2;
extern	statetype s_mansshoot3;
extern	statetype s_mansshoot4;
extern	statetype s_mansshoot5;
extern	statetype s_mansshoot6;
extern	statetype s_mansshoot7;
extern	statetype s_mansshoot8;


statetype s_mansstand	= {false,SPR_MANS_W1,0,T_Stand,NULL,&s_mansstand};

statetype s_manschase1 	= {false,SPR_MANS_W1,10,T_Chase,NULL,&s_manschase1s};
statetype s_manschase1s	= {false,SPR_MANS_W1,3,NULL,NULL,&s_manschase2};
statetype s_manschase2 	= {false,SPR_MANS_W2,8,T_Chase,NULL,&s_manschase3};
statetype s_manschase3 	= {false,SPR_MANS_W3,10,T_Chase,NULL,&s_manschase3s};
statetype s_manschase3s	= {false,SPR_MANS_W3,3,NULL,NULL,&s_manschase4};
statetype s_manschase4 	= {false,SPR_MANS_W4,8,T_Chase,NULL,&s_manschase1};

statetype s_mansdie0	= {false,SPR_MANS_W1,1,NULL,A_DeathScream,&s_mansdie01};
statetype s_mansdie01	= {false,SPR_MANS_W1,1,NULL,NULL,&s_mansdie1};
statetype s_mansdie1	= {false,SPR_MANS_DIE1,15,NULL,NULL,&s_mansdie2};
statetype s_mansdie2	= {false,SPR_MANS_DIE2,15,NULL,NULL,&s_mansdie3};
statetype s_mansdie3	= {false,SPR_MANS_DIE3,15,NULL,NULL,&s_mansdie4};
statetype s_mansdie4	= {false,SPR_MANS_DEAD,0,NULL,NULL,&s_mansdie4};

statetype s_mansshoot1 	= {false,SPR_MANS_SHOOT1,30,NULL,NULL,&s_mansshoot2};
statetype s_mansshoot2 	= {false,SPR_MANS_SHOOT2,10,NULL,T_Shoot,&s_mansshoot3};
statetype s_mansshoot3 	= {false,SPR_MANS_SHOOT3,10,NULL,T_Shoot,&s_mansshoot4};
statetype s_mansshoot4 	= {false,SPR_MANS_SHOOT2,10,NULL,T_Shoot,&s_mansshoot5};
statetype s_mansshoot5 	= {false,SPR_MANS_SHOOT3,10,NULL,T_Shoot,&s_mansshoot6};
statetype s_mansshoot6 	= {false,SPR_MANS_SHOOT2,10,NULL,T_Shoot,&s_mansshoot7};
statetype s_mansshoot7 	= {false,SPR_MANS_SHOOT3,10,NULL,T_Shoot,&s_mansshoot8};
statetype s_mansshoot8 	= {false,SPR_MANS_SHOOT1,10,NULL,NULL,&s_manschase1};

//
// uber
//

void T_UShoot (objtype *ob);

extern	statetype s_uberstand;

extern	statetype s_uberchase1;
extern	statetype s_uberchase1s;
extern	statetype s_uberchase2;
extern	statetype s_uberchase3;
extern	statetype s_uberchase3s;
extern	statetype s_uberchase4;

extern	statetype s_uberdie0;
extern	statetype s_uberdie01;
extern	statetype s_uberdie1;
extern	statetype s_uberdie2;
extern	statetype s_uberdie3;
extern	statetype s_uberdie4;
extern	statetype s_uberdie5;

#ifndef NEEDLE

extern	statetype s_ubershoot1;
extern	statetype s_ubershoot2;
extern	statetype s_ubershoot3;
extern	statetype s_ubershoot4;
extern	statetype s_ubershoot5;
extern	statetype s_ubershoot6;
extern	statetype s_ubershoot7;

statetype s_uberstand	= {false,SPR_UBER_W1,0,T_Stand,NULL,&s_uberstand};

statetype s_uberchase1 	= {false,SPR_UBER_W1,10,T_Chase,NULL,&s_uberchase1s};
statetype s_uberchase1s	= {false,SPR_UBER_W1,3,NULL,NULL,&s_uberchase2};
statetype s_uberchase2 	= {false,SPR_UBER_W2,8,T_Chase,NULL,&s_uberchase3};
statetype s_uberchase3 	= {false,SPR_UBER_W3,10,T_Chase,NULL,&s_uberchase3s};
statetype s_uberchase3s	= {false,SPR_UBER_W3,3,NULL,NULL,&s_uberchase4};
statetype s_uberchase4 	= {false,SPR_UBER_W4,8,T_Chase,NULL,&s_uberchase1};

statetype s_uberdie0	= {false,SPR_UBER_W1,1,NULL,A_DeathScream,&s_uberdie01};
statetype s_uberdie01	= {false,SPR_UBER_W1,1,NULL,NULL,&s_uberdie1};
statetype s_uberdie1	= {false,SPR_UBER_DIE1,15,NULL,NULL,&s_uberdie2};
statetype s_uberdie2	= {false,SPR_UBER_DIE2,15,NULL,NULL,&s_uberdie3};
statetype s_uberdie3	= {false,SPR_UBER_DIE3,15,NULL,NULL,&s_uberdie4};
statetype s_uberdie4	= {false,SPR_UBER_DIE4,15,NULL,NULL,&s_uberdie5};
statetype s_uberdie5	= {false,SPR_UBER_DEAD,0,NULL,NULL,&s_uberdie5};

statetype s_ubershoot1 	= {false,SPR_UBER_SHOOT1,30,NULL,NULL,&s_ubershoot2};
statetype s_ubershoot2 	= {false,SPR_UBER_SHOOT2,12,NULL,T_UShoot,&s_ubershoot3};
statetype s_ubershoot3 	= {false,SPR_UBER_SHOOT3,12,NULL,T_UShoot,&s_ubershoot4};
statetype s_ubershoot4 	= {false,SPR_UBER_SHOOT4,12,NULL,T_UShoot,&s_ubershoot5};
statetype s_ubershoot5 	= {false,SPR_UBER_SHOOT3,12,NULL,T_UShoot,&s_ubershoot6};
statetype s_ubershoot6 	= {false,SPR_UBER_SHOOT2,12,NULL,T_UShoot,&s_ubershoot7};
statetype s_ubershoot7 	= {false,SPR_UBER_SHOOT1,12,NULL,NULL,&s_uberchase1};

/*
===============
=
= T_UShoot
=
===============
*/

void T_UShoot (objtype *ob)
{
	int	dx,dy,dist;

	T_Shoot (ob);

	dx = abs(ob->tilex - player->tilex);
	dy = abs(ob->tiley - player->tiley);
	dist = dx>dy ? dx : dy;
	if (dist <= 1)
		TakeDamage (10,ob);
}

#else

void T_Sakurai (objtype *ob);
void T_SakuraiThink (objtype *ob);

extern	statetype s_ubershoot1;
extern	statetype s_ubershoot1s;
extern	statetype s_ubershoot2;
extern	statetype s_ubershoot3;
extern	statetype s_ubershoot4;
extern	statetype s_ubershoot5;
extern	statetype s_ubershoot6;
extern	statetype s_ubershoot7;
extern	statetype s_ubershoot8;
extern	statetype s_ubershoot9;

statetype s_uberstand	= {false,SPR_UBER_STAND,0,T_Stand,NULL,&s_uberstand};

statetype s_uberchase1 	= {false,SPR_UBER_W1,10,T_Will,NULL,&s_uberchase2};
statetype s_uberchase2 	= {false,SPR_UBER_W2,8,T_Will,NULL,&s_uberchase3};
statetype s_uberchase3 	= {false,SPR_UBER_W3,10,T_Will,NULL,&s_uberchase4};
statetype s_uberchase4 	= {false,SPR_UBER_W4,8,T_Will,NULL,&s_uberchase1};

statetype s_uberdie0	= {false,SPR_UBER_STAND,1,NULL,A_DeathScream,&s_uberdie01};
statetype s_uberdie01	= {false,SPR_UBER_STAND,1,NULL,NULL,&s_uberdie1};
statetype s_uberdie1	= {false,SPR_UBER_DIE1,15,NULL,NULL,&s_uberdie2};
statetype s_uberdie2	= {false,SPR_UBER_DIE2,15,NULL,NULL,&s_uberdie3};
statetype s_uberdie3	= {false,SPR_UBER_DIE3,15,NULL,NULL,&s_uberdie4};
statetype s_uberdie4	= {false,SPR_UBER_DEAD,0,NULL,NULL,&s_uberdie4};

statetype s_ubershoot1 	= {false,SPR_UBER_STAND,8,NULL,NULL,&s_ubershoot1s};
statetype s_ubershoot1s = {false,SPR_UBER_SHOOT1,30,NULL,NULL,&s_ubershoot2};
statetype s_ubershoot2 	= {false,SPR_UBER_SHOOT2,12,NULL,T_Launch,&s_ubershoot3};
statetype s_ubershoot3 	= {false,SPR_UBER_SHOOT1,12,NULL,NULL,&s_ubershoot4};
statetype s_ubershoot4 	= {false,SPR_UBER_SHOOT3,12,NULL,T_Launch,&s_ubershoot5};
statetype s_ubershoot5 	= {false,SPR_UBER_SHOOT1,20,T_SakuraiThink,NULL,&s_uberchase1};
statetype s_ubershoot6 	= {false,SPR_UBER_SHOOT4,3,NULL,T_Launch,&s_ubershoot7};
statetype s_ubershoot7 	= {false,SPR_UBER_SHOOT4,3,NULL,T_Launch,&s_ubershoot8};
statetype s_ubershoot8 	= {false,SPR_UBER_SHOOT4,6,NULL,NULL,&s_ubershoot9};
statetype s_ubershoot9 	= {false,SPR_UBER_SHOOT1,12,NULL,NULL,&s_uberchase1};

void T_SakuraiThink (objtype *ob)
{
	int dx,dy,dist;
	
	dx = abs(ob->tilex - player->tilex);
	dy = abs(ob->tiley - player->tiley);
	dist = dx>dy ? dx : dy;

	if (ob->state == &s_ubershoot5)
	{
		if ( US_RndT() < (tics<<3) && dist <= 8)
			NewState (ob,&s_ubershoot6);
		else
		    NewState (ob,&s_uberchase1);
		return;
	}
	else
	    return;
}

#endif

//
// will
//
extern	statetype s_willstand;

extern	statetype s_willchase1;
extern	statetype s_willchase1s;
extern	statetype s_willchase2;
extern	statetype s_willchase3;
extern	statetype s_willchase3s;
extern	statetype s_willchase4;

extern	statetype s_willdie1;
extern	statetype s_willdie2;
extern	statetype s_willdie3;
extern	statetype s_willdie4;
extern	statetype s_willdie5;
extern	statetype s_willdie6;

extern	statetype s_willshoot1;
extern	statetype s_willshoot2;
extern	statetype s_willshoot3;
extern	statetype s_willshoot4;
extern	statetype s_willshoot5;
extern	statetype s_willshoot6;


statetype s_willstand	= {false,SPR_WILL_W1,0,T_Stand,NULL,&s_willstand};

statetype s_willchase1 	= {false,SPR_WILL_W1,10,T_Will,NULL,&s_willchase1s};
statetype s_willchase1s	= {false,SPR_WILL_W1,3,NULL,NULL,&s_willchase2};
statetype s_willchase2 	= {false,SPR_WILL_W2,8,T_Will,NULL,&s_willchase3};
statetype s_willchase3 	= {false,SPR_WILL_W3,10,T_Will,NULL,&s_willchase3s};
statetype s_willchase3s	= {false,SPR_WILL_W3,3,NULL,NULL,&s_willchase4};
statetype s_willchase4 	= {false,SPR_WILL_W4,8,T_Will,NULL,&s_willchase1};

statetype s_willdeathcam	= {false,SPR_WILL_W1,1,NULL,NULL,&s_willdie1};

statetype s_willdie1	= {false,SPR_WILL_W1,1,NULL,A_DeathScream,&s_willdie2};
statetype s_willdie2	= {false,SPR_WILL_W1,10,NULL,NULL,&s_willdie3};
statetype s_willdie3	= {false,SPR_WILL_DIE1,10,NULL,NULL,&s_willdie4};
statetype s_willdie4	= {false,SPR_WILL_DIE2,10,NULL,NULL,&s_willdie5};
statetype s_willdie5	= {false,SPR_WILL_DIE3,10,NULL,NULL,&s_willdie6};
statetype s_willdie6	= {false,SPR_WILL_DEAD,20,NULL,NULL,&s_willdie6};

statetype s_willshoot1 	= {false,SPR_WILL_SHOOT1,30,NULL,NULL,&s_willshoot2};
statetype s_willshoot2 	= {false,SPR_WILL_SHOOT2,10,NULL,T_Launch,&s_willshoot3};
statetype s_willshoot3 	= {false,SPR_WILL_SHOOT3,10,NULL,T_Shoot,&s_willshoot4};
statetype s_willshoot4 	= {false,SPR_WILL_SHOOT4,10,NULL,T_Shoot,&s_willshoot5};
statetype s_willshoot5 	= {false,SPR_WILL_SHOOT3,10,NULL,T_Shoot,&s_willshoot6};
statetype s_willshoot6 	= {false,SPR_WILL_SHOOT4,10,NULL,T_Shoot,&s_willchase1};

/*
================
=
= T_Will
=
================
*/

void T_Will (objtype *ob)
{
	long move;
	int	dx,dy,dist;
	boolean	dodge,wuss;

	dodge = false;
	wuss = false;
	dx = abs(ob->tilex - player->tilex);
	dy = abs(ob->tiley - player->tiley);
	dist = dx>dy ? dx : dy;

	if (ob->obclass == uberobj)
	{
		if (ob->hitpoints <= 750) wuss = true;
		else wuss = false;
	}
	else if (ob->obclass == officerobj)
	{
		if (gamestate.difficulty <= gd_hard)
		{
        	if (ob->hitpoints <= 25) wuss = true;
			else wuss = false;
    	}
    	else wuss = false;
	}
    
    if (CheckLine(ob))						// got a shot at player?
	{
		if ( US_RndT() < (tics<<3) )
		{
		//
		// go into attack frame
		//
			if (ob->obclass == willobj)
				NewState (ob,&s_willshoot1);
			else if (ob->obclass == angelobj)
				NewState (ob,&s_angelshoot1);
			else if (ob->obclass == uberobj)
			{
				if (dist < 7) NewState (ob,&s_ubershoot1);
            }
            else if (ob->obclass == officerobj)
				NewState (ob,&s_ofcshoot1);
			else
				NewState (ob,&s_deathshoot1);
			return;
		}
		if (!wuss) dodge = true;
		else dodge = false;
	}

	if (ob->dir == nodir)
	{
		if (dodge)
			SelectDodgeDir (ob);
		else
			SelectChaseDir (ob);
		if (ob->dir == nodir)
			return;							// object is blocked in
	}

	move = ob->speed*tics;

	while (move)
	{
		if (ob->distance < 0)
		{
		//
		// waiting for a door to open
		//
			OpenDoor (-ob->distance-1);
			if (doorobjlist[-ob->distance-1].action != dr_open)
				return;
			ob->distance = TILEGLOBAL;	// go ahead, the door is now opoen
		}

		if (move < ob->distance)
		{
			MoveObj (ob,move);
			break;
		}

		//
		// reached goal tile, so select another one
		//

		//
		// fix position to account for round off during moving
		//
		ob->x = ((long)ob->tilex<<TILESHIFT)+TILEGLOBAL/2;
		ob->y = ((long)ob->tiley<<TILESHIFT)+TILEGLOBAL/2;

		move -= ob->distance;

		if (dist < 4 || wuss)
			SelectRunDir (ob);
		else if (dodge)
			SelectDodgeDir (ob);
		else
			SelectChaseDir (ob);

		if (ob->dir == nodir)
			return;							// object is blocked in
	}

}


//
// death
//
extern	statetype s_deathstand;

extern	statetype s_deathchase1;
extern	statetype s_deathchase1s;
extern	statetype s_deathchase2;
extern	statetype s_deathchase3;
extern	statetype s_deathchase3s;
extern	statetype s_deathchase4;

extern	statetype s_deathdie1;
extern	statetype s_deathdie2;
extern	statetype s_deathdie3;
extern	statetype s_deathdie4;
extern	statetype s_deathdie5;
extern	statetype s_deathdie6;
extern	statetype s_deathdie7;
extern	statetype s_deathdie8;
extern	statetype s_deathdie9;

extern	statetype s_deathshoot1;
extern	statetype s_deathshoot2;
extern	statetype s_deathshoot3;
extern	statetype s_deathshoot4;
extern	statetype s_deathshoot5;


statetype s_deathstand	= {false,SPR_DEATH_W1,0,T_Stand,NULL,&s_deathstand};

statetype s_deathchase1 	= {false,SPR_DEATH_W1,10,T_Will,NULL,&s_deathchase1s};
statetype s_deathchase1s	= {false,SPR_DEATH_W1,3,NULL,NULL,&s_deathchase2};
statetype s_deathchase2 	= {false,SPR_DEATH_W2,8,T_Will,NULL,&s_deathchase3};
statetype s_deathchase3 	= {false,SPR_DEATH_W3,10,T_Will,NULL,&s_deathchase3s};
statetype s_deathchase3s	= {false,SPR_DEATH_W3,3,NULL,NULL,&s_deathchase4};
statetype s_deathchase4 	= {false,SPR_DEATH_W4,8,T_Will,NULL,&s_deathchase1};

statetype s_deathdeathcam	= {false,SPR_DEATH_W1,1,NULL,NULL,&s_deathdie1};

statetype s_deathdie1	= {false,SPR_DEATH_W1,1,NULL,A_DeathScream,&s_deathdie2};
statetype s_deathdie2	= {false,SPR_DEATH_W1,10,NULL,NULL,&s_deathdie3};
statetype s_deathdie3	= {false,SPR_DEATH_DIE1,10,NULL,NULL,&s_deathdie4};
statetype s_deathdie4	= {false,SPR_DEATH_DIE2,10,NULL,NULL,&s_deathdie5};
statetype s_deathdie5	= {false,SPR_DEATH_DIE3,10,NULL,NULL,&s_deathdie6};
statetype s_deathdie6	= {false,SPR_DEATH_DIE4,10,NULL,NULL,&s_deathdie7};
statetype s_deathdie7	= {false,SPR_DEATH_DIE5,10,NULL,NULL,&s_deathdie8};
statetype s_deathdie8	= {false,SPR_DEATH_DIE6,10,NULL,NULL,&s_deathdie9};
statetype s_deathdie9	= {false,SPR_DEATH_DEAD,0,NULL,NULL,&s_deathdie9};

statetype s_deathshoot1 	= {false,SPR_DEATH_SHOOT1,30,NULL,NULL,&s_deathshoot2};
statetype s_deathshoot2 	= {false,SPR_DEATH_SHOOT2,10,NULL,T_Launch,&s_deathshoot3};
statetype s_deathshoot3 	= {false,SPR_DEATH_SHOOT4,10,NULL,T_Shoot,&s_deathshoot4};
statetype s_deathshoot4 	= {false,SPR_DEATH_SHOOT3,10,NULL,T_Launch,&s_deathshoot5};
statetype s_deathshoot5 	= {false,SPR_DEATH_SHOOT4,10,NULL,T_Shoot,&s_deathchase1};

// Friedrich

extern	statetype s_friedstand;

extern	statetype s_friedchase1;
extern	statetype s_friedchase1s;
extern	statetype s_friedchase2;
extern	statetype s_friedchase3;
extern	statetype s_friedchase3s;
extern	statetype s_friedchase4;

extern	statetype s_frieddie0;
extern	statetype s_frieddie01;
extern	statetype s_frieddie1;
extern	statetype s_frieddie2;
extern	statetype s_frieddie3;
extern	statetype s_frieddie4;
extern	statetype s_frieddie5;

extern	statetype s_friedshoot1;
extern	statetype s_friedshoot2;
extern	statetype s_friedshoot3;
extern	statetype s_friedshoot4;
extern	statetype s_friedshoot5;
extern	statetype s_friedshoot6;
extern	statetype s_friedshoot7;


statetype s_friedstand	= {false,SPR_FRIED_W1,0,T_Stand,NULL,&s_friedstand};

statetype s_friedchase1 	= {false,SPR_FRIED_W1,10,T_Chase,NULL,&s_friedchase1s};
statetype s_friedchase1s	= {false,SPR_FRIED_W1,3,NULL,NULL,&s_friedchase2};
statetype s_friedchase2 	= {false,SPR_FRIED_W2,8,T_Chase,NULL,&s_friedchase3};
statetype s_friedchase3 	= {false,SPR_FRIED_W3,10,T_Chase,NULL,&s_friedchase3s};
statetype s_friedchase3s	= {false,SPR_FRIED_W3,3,NULL,NULL,&s_friedchase4};
statetype s_friedchase4 	= {false,SPR_FRIED_W4,8,T_Chase,NULL,&s_friedchase1};

statetype s_frieddie0	= {false,SPR_FRIED_W1,1,NULL,A_DeathScream,&s_frieddie01};
statetype s_frieddie01	= {false,SPR_FRIED_W1,1,NULL,NULL,&s_frieddie1};
statetype s_frieddie1	= {false,SPR_FRIED_DIE1,15,NULL,NULL,&s_frieddie2};
statetype s_frieddie2	= {false,SPR_FRIED_DIE2,15,NULL,NULL,&s_frieddie3};
statetype s_frieddie3	= {false,SPR_FRIED_DIE3,15,NULL,NULL,&s_frieddie4};
statetype s_frieddie4	= {false,SPR_FRIED_DEAD,0,NULL,NULL,&s_frieddie4};

statetype s_friedshoot1 	= {false,SPR_FRIED_SHOOT1,30,NULL,NULL,&s_friedshoot2};
statetype s_friedshoot2 	= {false,SPR_FRIED_SHOOT2,12,NULL,T_Launch,&s_friedshoot3};
statetype s_friedshoot3 	= {false,SPR_FRIED_SHOOT3,8,NULL,T_Launch,&s_friedshoot4};
statetype s_friedshoot4 	= {false,SPR_FRIED_SHOOT4,8,NULL,T_Launch,&s_friedshoot5};
statetype s_friedshoot5 	= {false,SPR_FRIED_SHOOT3,8,NULL,T_Launch,&s_friedshoot6};
statetype s_friedshoot6 	= {false,SPR_FRIED_SHOOT2,8,NULL,T_Launch,&s_friedshoot7};
statetype s_friedshoot7 	= {false,SPR_FRIED_SHOOT1,8,NULL,T_Launch,&s_friedchase1};

/*
===============
=
= T_Launch
=
===============
*/

void T_Launch (objtype *ob)
{
	long	deltax,deltay;
	float	angle;
	int		iangle;

	deltax = player->x - ob->x;
	deltay = ob->y - player->y;
	angle = atan2 (deltay,deltax);
	if (angle<0)
		angle = M_PI*2+angle;
	iangle = angle/(M_PI*2)*ANGLES;
	if (ob->obclass == deathobj)
	{
		T_Shoot (ob);
		if (ob->state == &s_deathshoot2)
		{
			iangle-=4;
			if (iangle<0)
				iangle+=ANGLES;
		}
		else
		{
			iangle+=4;
			if (iangle>=ANGLES)
				iangle-=ANGLES;
		}
	}
	else if (ob->obclass == friedobj)
	{
		if (ob->state == &s_friedshoot2)
		{
			iangle-=4;
			if (iangle<0)
				iangle+=ANGLES;
		}
		else
		{
			iangle+=4;
			if (iangle>=ANGLES)
				iangle-=ANGLES;
		}
    }
	else if (ob->obclass == uberobj)
	{
		if (ob->state == &s_ubershoot2 || ob->state == &s_ubershoot6 )
		{
			iangle-=4;
			if (iangle<0)
				iangle+=ANGLES;
		}
		else
		{
			iangle+=4;
			if (iangle>=ANGLES)
				iangle-=ANGLES;
		}
    }

	GetNewActor ();
	new->state = &s_rocket;
	new->ticcount = 1;

	new->tilex = ob->tilex;
	new->tiley = ob->tiley;
	new->x = ob->x;
	new->y = ob->y;
	new->obclass = rocketobj;
	
	SetSpawner(new,ob);
	
	switch(ob->obclass)
	{
	case deathobj:
		new->state = &s_rocket;
		new->obclass = hrocketobj;
		PlaySoundLocActor (KNIGHTMISSILESND,new);
		break;
	case angelobj:
		new->state = &s_spark1;
		new->obclass = sparkobj;
		PlaySoundLocActor (ANGELFIRESND,new);
		break;
	case friedobj:
        if (ob->state != &s_friedshoot2)
        {
    		new->state = &s_fire1;
    		new->obclass = fireobj;
    		PlaySoundLocActor (FLAMETHROWERSND,new);
        }
        else
        {
            new->state = &s_rocket;
            new->obclass = rocketobj;
            PlaySoundLocActor (MISSILEFIRESND,new);
        }
        break;
	case uberobj:
        if (ob->state == &s_ubershoot2)
        {
            new->state = &s_rocket;
            new->obclass = rocketobj;
            PlaySoundLocActor (MISSILEFIRESND,new);
        }
        else
        {
		    new->state = &s_rocket;
		    new->obclass = hrocketobj;
		    PlaySoundLocActor (KNIGHTMISSILESND,new);
        }
        break;
	default:
		PlaySoundLocActor (MISSILEFIRESND,new);
		break;
	}

	new->dir = nodir;
	new->angle = iangle;
	new->speed = 0x2000l;
	new->flags = FL_NONMARK;
	new->active = true;
}



//
// angel
//
void A_Relaunch (objtype *ob);
void A_Victory (objtype *ob);
void A_StartAttack (objtype *ob);
void A_Breathing (objtype *ob);

extern	statetype s_angelstand;

extern	statetype s_angelchase1;
extern	statetype s_angelchase1s;
extern	statetype s_angelchase2;
extern	statetype s_angelchase3;
extern	statetype s_angelchase3s;
extern	statetype s_angelchase4;

extern	statetype s_angeldie1;
extern	statetype s_angeldie11;
extern	statetype s_angeldie2;
extern	statetype s_angeldie3;
extern	statetype s_angeldie4;
extern	statetype s_angeldie5;
extern	statetype s_angeldie6;
extern	statetype s_angeldie7;
extern	statetype s_angeldie8;
extern	statetype s_angeldie9;

extern	statetype s_angelshoot1;
extern	statetype s_angelshoot2;
extern	statetype s_angelshoot3;
extern	statetype s_angelshoot4;
extern	statetype s_angelshoot5;
extern	statetype s_angelshoot6;

extern	statetype s_angeltired;
extern	statetype s_angeltired2;
extern	statetype s_angeltired3;
extern	statetype s_angeltired4;
extern	statetype s_angeltired5;
extern	statetype s_angeltired6;
extern	statetype s_angeltired7;

extern	statetype s_spark1;
extern	statetype s_spark2;
extern	statetype s_spark3;
extern	statetype s_spark4;


statetype s_angelstand	= {false,SPR_ANGEL_W1,0,T_Stand,NULL,&s_angelstand};

statetype s_angelchase1 	= {false,SPR_ANGEL_W1,10,T_Will,NULL,&s_angelchase1s};
statetype s_angelchase1s	= {false,SPR_ANGEL_W1,3,NULL,NULL,&s_angelchase2};
statetype s_angelchase2 	= {false,SPR_ANGEL_W2,8,T_Will,NULL,&s_angelchase3};
statetype s_angelchase3 	= {false,SPR_ANGEL_W3,10,T_Will,NULL,&s_angelchase3s};
statetype s_angelchase3s	= {false,SPR_ANGEL_W3,3,NULL,NULL,&s_angelchase4};
statetype s_angelchase4 	= {false,SPR_ANGEL_W4,8,T_Will,NULL,&s_angelchase1};

statetype s_angeldie1	= {false,SPR_ANGEL_W1,1,NULL,A_DeathScream,&s_angeldie11};
statetype s_angeldie11	= {false,SPR_ANGEL_W1,1,NULL,NULL,&s_angeldie2};
statetype s_angeldie2	= {false,SPR_ANGEL_DIE1,10,NULL,A_Slurpie,&s_angeldie3};
statetype s_angeldie3	= {false,SPR_ANGEL_DIE2,10,NULL,NULL,&s_angeldie4};
statetype s_angeldie4	= {false,SPR_ANGEL_DIE3,10,NULL,NULL,&s_angeldie5};
statetype s_angeldie5	= {false,SPR_ANGEL_DIE4,10,NULL,NULL,&s_angeldie6};
statetype s_angeldie6	= {false,SPR_ANGEL_DIE5,10,NULL,NULL,&s_angeldie7};
statetype s_angeldie7	= {false,SPR_ANGEL_DIE6,10,NULL,NULL,&s_angeldie8};
statetype s_angeldie8	= {false,SPR_ANGEL_DIE7,10,NULL,NULL,&s_angeldie9};
statetype s_angeldie9	= {false,SPR_ANGEL_DEAD,130,NULL,A_Victory,&s_angeldie9};

statetype s_angelshoot1 	= {false,SPR_ANGEL_SHOOT1,10,NULL,A_StartAttack,&s_angelshoot2};
statetype s_angelshoot2 	= {false,SPR_ANGEL_SHOOT2,20,NULL,T_Launch,&s_angelshoot3};
statetype s_angelshoot3 	= {false,SPR_ANGEL_SHOOT1,10,NULL,A_Relaunch,&s_angelshoot2};

statetype s_angeltired 	= {false,SPR_ANGEL_TIRED1,40,NULL,A_Breathing,&s_angeltired2};
statetype s_angeltired2	= {false,SPR_ANGEL_TIRED2,40,NULL,NULL,&s_angeltired3};
statetype s_angeltired3	= {false,SPR_ANGEL_TIRED1,40,NULL,A_Breathing,&s_angeltired4};
statetype s_angeltired4	= {false,SPR_ANGEL_TIRED2,40,NULL,NULL,&s_angeltired5};
statetype s_angeltired5	= {false,SPR_ANGEL_TIRED1,40,NULL,A_Breathing,&s_angeltired6};
statetype s_angeltired6	= {false,SPR_ANGEL_TIRED2,40,NULL,NULL,&s_angeltired7};
statetype s_angeltired7	= {false,SPR_ANGEL_TIRED1,40,NULL,A_Breathing,&s_angelchase1};

statetype s_spark1 	= {false,SPR_SPARK1,6,T_Projectile,NULL,&s_spark2};
statetype s_spark2 	= {false,SPR_SPARK2,6,T_Projectile,NULL,&s_spark3};
statetype s_spark3 	= {false,SPR_SPARK3,6,T_Projectile,NULL,&s_spark4};
statetype s_spark4 	= {false,SPR_SPARK4,6,T_Projectile,NULL,&s_spark1};


#pragma argsused
void A_Slurpie (objtype *ob)
{
 SD_PlaySound(SLURPIESND);
}

#pragma argsused
void A_Breathing (objtype *ob)
{
 SD_PlaySound(ANGELTIREDSND);
}

/*
=================
=
= A_Victory
=
=================
*/

#pragma argsused
void A_Victory (objtype *ob)
{
	playstate = ex_victorious;
}


/*
=================
=
= A_StartAttack
=
=================
*/

void A_StartAttack (objtype *ob)
{
	ob->temp1 = 0;
}


/*
=================
=
= A_Relaunch
=
=================
*/

void A_Relaunch (objtype *ob)
{
	if (++ob->temp1 == 3)
	{
		NewState (ob,&s_angeltired);
		return;
	}

	if (US_RndT()&1)
	{
		NewState (ob,&s_angelchase1);
		return;
	}
}




//
// spectre
//
void T_SpectreWait (objtype *ob);
void A_Dormant (objtype *ob);

extern	statetype s_spectrewait1;
extern	statetype s_spectrewait2;
extern	statetype s_spectrewait3;
extern	statetype s_spectrewait4;

extern	statetype s_spectrechase1;
extern	statetype s_spectrechase2;
extern	statetype s_spectrechase3;
extern	statetype s_spectrechase4;

extern	statetype s_spectredie1;
extern	statetype s_spectredie2;
extern	statetype s_spectredie3;
extern	statetype s_spectredie4;

extern	statetype s_spectrewake;

statetype s_spectrewait1	= {false,SPR_SPECTRE_W1,10,T_Stand,NULL,&s_spectrewait2};
statetype s_spectrewait2	= {false,SPR_SPECTRE_W2,10,T_Stand,NULL,&s_spectrewait3};
statetype s_spectrewait3	= {false,SPR_SPECTRE_W3,10,T_Stand,NULL,&s_spectrewait4};
statetype s_spectrewait4	= {false,SPR_SPECTRE_W4,10,T_Stand,NULL,&s_spectrewait1};

statetype s_spectrechase1	= {false,SPR_SPECTRE_W1,10,T_Ghosts,NULL,&s_spectrechase2};
statetype s_spectrechase2	= {false,SPR_SPECTRE_W2,10,T_Ghosts,NULL,&s_spectrechase3};
statetype s_spectrechase3	= {false,SPR_SPECTRE_W3,10,T_Ghosts,NULL,&s_spectrechase4};
statetype s_spectrechase4	= {false,SPR_SPECTRE_W4,10,T_Ghosts,NULL,&s_spectrechase1};

statetype s_spectredie1	= {false,SPR_SPECTRE_F1,10,NULL,NULL,&s_spectredie2};
statetype s_spectredie2	= {false,SPR_SPECTRE_F2,10,NULL,NULL,&s_spectredie3};
statetype s_spectredie3	= {false,SPR_SPECTRE_F3,10,NULL,NULL,&s_spectredie4};
statetype s_spectredie4	= {false,SPR_SPECTRE_F4,300,NULL,NULL,&s_spectrewake};
statetype s_spectrewake	= {false,SPR_SPECTRE_F4,10,NULL,A_Dormant,&s_spectrewake};

/*
===============
=
= SpawnSpectre
=
===============
*/

void SpawnSpectre (int tilex, int tiley)
{
	unsigned	far *map,tile;

	SpawnNewObj (tilex,tiley,&s_spectrewait1);
	new->obclass = spectreobj;
	new->hitpoints = starthitpoints[gamestate.difficulty][en_spectre];
	new->flags |= FL_SHOOTABLE|FL_AMBUSH; // |FL_NEVERMARK|FL_NONMARK;
	if (!loadedgame)
	  gamestate.killtotal++;
}


/*
===============
=
= A_Dormant
=
===============
*/

void A_Dormant (objtype *ob)
{
	long	deltax,deltay;
	int	xl,xh,yl,yh;
	int	x,y;

	deltax = ob->x - player->x;
	if (deltax < -MINACTORDIST || deltax > MINACTORDIST)
		goto moveok;
	deltay = ob->y - player->y;
	if (deltay < -MINACTORDIST || deltay > MINACTORDIST)
		goto moveok;

	return;
moveok:

	xl = (ob->x-MINDIST) >> TILESHIFT;
	xh = (ob->x+MINDIST) >> TILESHIFT;
	yl = (ob->y-MINDIST) >> TILESHIFT;
	yh = (ob->y+MINDIST) >> TILESHIFT;

	for (y=yl ; y<=yh ; y++)
		for (x=xl ; x<=xh ; x++)
		{
			if (!actorat[x][y])
				continue;
			if (actorat[x][y])
				return;
			if (((objtype *)actorat[x][y])->flags&FL_SHOOTABLE)
				return;
		}

	ob->flags |= FL_AMBUSH | FL_SHOOTABLE;
	ob->flags &= ~FL_ATTACKMODE;
	ob->dir = nodir;
	NewState (ob,&s_spectrewait1);
}


#endif

/*
=============================================================================

						 SCHABBS / GIFT / FAT

=============================================================================
*/

statetype s_fire1 	= {false,SPR_FIRE1,6,T_Projectile,NULL,&s_fire2};
statetype s_fire2 	= {false,SPR_FIRE2,6,T_Projectile,NULL,&s_fire1};

#ifndef SPEAR
/*
===============
=
= SpawnGhosts
=
===============
*/

void SpawnGhosts (int which, int tilex, int tiley)
{
	unsigned	far *map,tile;

	switch(which)
	{
	 case en_blinky:
	   SpawnNewObj (tilex,tiley,&s_blinkychase1);
	   break;
	 case en_clyde:
	   SpawnNewObj (tilex,tiley,&s_clydechase1);
	   break;
	 case en_pinky:
	   SpawnNewObj (tilex,tiley,&s_pinkychase1);
	   break;
	 case en_inky:
	   SpawnNewObj (tilex,tiley,&s_inkychase1);
	   break;
	}

	new->obclass = ghostobj;
	new->speed = SPDDOG;

	new->dir = east;
	new->flags |= FL_AMBUSH;
	if (!loadedgame)
	  gamestate.killtotal++;
}



void	T_Gift (objtype *ob);
void	T_GiftThrow (objtype *ob);

void	T_Fat (objtype *ob);
void	T_FatThrow (objtype *ob);

//
// schabb
//
extern	statetype s_schabbstand;

extern	statetype s_schabbchase1;
extern	statetype s_schabbchase1s;
extern	statetype s_schabbchase2;
extern	statetype s_schabbchase3;
extern	statetype s_schabbchase3s;
extern	statetype s_schabbchase4;

extern	statetype s_schabbdie1;
extern	statetype s_schabbdie2;
extern	statetype s_schabbdie3;
extern	statetype s_schabbdie4;
extern	statetype s_schabbdie5;
extern	statetype s_schabbdie6;

extern	statetype s_schabbshoot1;
extern	statetype s_schabbshoot2;

extern	statetype s_needle1;
extern	statetype s_needle2;
extern	statetype s_needle3;
extern	statetype s_needle4;

extern	statetype s_schabbdeathcam;


statetype s_schabbstand	= {false,SPR_SCHABB_W1,0,T_Stand,NULL,&s_schabbstand};

statetype s_schabbchase1 	= {false,SPR_SCHABB_W1,10,T_Schabb,NULL,&s_schabbchase1s};
statetype s_schabbchase1s	= {false,SPR_SCHABB_W1,3,NULL,NULL,&s_schabbchase2};
statetype s_schabbchase2 	= {false,SPR_SCHABB_W2,8,T_Schabb,NULL,&s_schabbchase3};
statetype s_schabbchase3 	= {false,SPR_SCHABB_W3,10,T_Schabb,NULL,&s_schabbchase3s};
statetype s_schabbchase3s	= {false,SPR_SCHABB_W3,3,NULL,NULL,&s_schabbchase4};
statetype s_schabbchase4 	= {false,SPR_SCHABB_W4,8,T_Schabb,NULL,&s_schabbchase1};

statetype s_schabbdeathcam	= {false,SPR_SCHABB_W1,1,NULL,NULL,&s_schabbdie1};

statetype s_schabbdie1	= {false,SPR_SCHABB_W1,10,NULL,A_DeathScream,&s_schabbdie2};
statetype s_schabbdie2	= {false,SPR_SCHABB_W1,10,NULL,NULL,&s_schabbdie3};
statetype s_schabbdie3	= {false,SPR_SCHABB_DIE1,10,NULL,NULL,&s_schabbdie4};
statetype s_schabbdie4	= {false,SPR_SCHABB_DIE2,10,NULL,NULL,&s_schabbdie5};
statetype s_schabbdie5	= {false,SPR_SCHABB_DIE3,10,NULL,NULL,&s_schabbdie6};
statetype s_schabbdie6	= {false,SPR_SCHABB_DEAD,20,NULL,A_StartDeathCam,&s_schabbdie6};

statetype s_schabbshoot1 	= {false,SPR_SCHABB_SHOOT1,30,NULL,NULL,&s_schabbshoot2};
statetype s_schabbshoot2 	= {false,SPR_SCHABB_SHOOT2,10,NULL,T_SchabbThrow,&s_schabbchase1};

statetype s_needle1 	= {false,SPR_HYPO1,6,T_Projectile,NULL,&s_needle2};
statetype s_needle2 	= {false,SPR_HYPO2,6,T_Projectile,NULL,&s_needle3};
statetype s_needle3 	= {false,SPR_HYPO3,6,T_Projectile,NULL,&s_needle4};
statetype s_needle4 	= {false,SPR_HYPO4,6,T_Projectile,NULL,&s_needle1};


//
// gift
//
extern	statetype s_giftstand;

extern	statetype s_giftchase1;
extern	statetype s_giftchase1s;
extern	statetype s_giftchase2;
extern	statetype s_giftchase3;
extern	statetype s_giftchase3s;
extern	statetype s_giftchase4;

extern	statetype s_giftdie1;
extern	statetype s_giftdie2;
extern	statetype s_giftdie3;
extern	statetype s_giftdie4;
extern	statetype s_giftdie5;
extern	statetype s_giftdie6;

extern	statetype s_giftshoot1;
extern	statetype s_giftshoot2;

extern	statetype s_needle1;
extern	statetype s_needle2;
extern	statetype s_needle3;
extern	statetype s_needle4;

extern	statetype s_giftdeathcam;

extern	statetype s_boom1;
extern	statetype s_boom2;
extern	statetype s_boom3;


statetype s_giftstand	= {false,SPR_GIFT_W1,0,T_Stand,NULL,&s_giftstand};

statetype s_giftchase1 	= {false,SPR_GIFT_W1,10,T_Gift,NULL,&s_giftchase1s};
statetype s_giftchase1s	= {false,SPR_GIFT_W1,3,NULL,NULL,&s_giftchase2};
statetype s_giftchase2 	= {false,SPR_GIFT_W2,8,T_Gift,NULL,&s_giftchase3};
statetype s_giftchase3 	= {false,SPR_GIFT_W3,10,T_Gift,NULL,&s_giftchase3s};
statetype s_giftchase3s	= {false,SPR_GIFT_W3,3,NULL,NULL,&s_giftchase4};
statetype s_giftchase4 	= {false,SPR_GIFT_W4,8,T_Gift,NULL,&s_giftchase1};

statetype s_giftdeathcam	= {false,SPR_GIFT_W1,1,NULL,NULL,&s_giftdie1};

statetype s_giftdie1	= {false,SPR_GIFT_W1,1,NULL,A_DeathScream,&s_giftdie2};
statetype s_giftdie2	= {false,SPR_GIFT_W1,10,NULL,NULL,&s_giftdie3};
statetype s_giftdie3	= {false,SPR_GIFT_DIE1,10,NULL,NULL,&s_giftdie4};
statetype s_giftdie4	= {false,SPR_GIFT_DIE2,10,NULL,NULL,&s_giftdie5};
statetype s_giftdie5	= {false,SPR_GIFT_DIE3,10,NULL,NULL,&s_giftdie6};
statetype s_giftdie6	= {false,SPR_GIFT_DEAD,20,NULL,A_StartDeathCam,&s_giftdie6};

statetype s_giftshoot1 	= {false,SPR_GIFT_SHOOT1,30,NULL,NULL,&s_giftshoot2};
statetype s_giftshoot2 	= {false,SPR_GIFT_SHOOT2,10,NULL,T_GiftThrow,&s_giftchase1};


//
// fat
//
extern	statetype s_fatstand;

extern	statetype s_fatchase1;
extern	statetype s_fatchase1s;
extern	statetype s_fatchase2;
extern	statetype s_fatchase3;
extern	statetype s_fatchase3s;
extern	statetype s_fatchase4;

extern	statetype s_fatdie1;
extern	statetype s_fatdie2;
extern	statetype s_fatdie3;
extern	statetype s_fatdie4;
extern	statetype s_fatdie5;
extern	statetype s_fatdie6;

extern	statetype s_fatshoot1;
extern	statetype s_fatshoot2;
extern	statetype s_fatshoot3;
extern	statetype s_fatshoot4;
extern	statetype s_fatshoot5;
extern	statetype s_fatshoot6;

extern	statetype s_needle1;
extern	statetype s_needle2;
extern	statetype s_needle3;
extern	statetype s_needle4;

extern	statetype s_fatdeathcam;


statetype s_fatstand	= {false,SPR_FAT_W1,0,T_Stand,NULL,&s_fatstand};

statetype s_fatchase1 	= {false,SPR_FAT_W1,10,T_Fat,NULL,&s_fatchase1s};
statetype s_fatchase1s	= {false,SPR_FAT_W1,3,NULL,NULL,&s_fatchase2};
statetype s_fatchase2 	= {false,SPR_FAT_W2,8,T_Fat,NULL,&s_fatchase3};
statetype s_fatchase3 	= {false,SPR_FAT_W3,10,T_Fat,NULL,&s_fatchase3s};
statetype s_fatchase3s	= {false,SPR_FAT_W3,3,NULL,NULL,&s_fatchase4};
statetype s_fatchase4 	= {false,SPR_FAT_W4,8,T_Fat,NULL,&s_fatchase1};

statetype s_fatdeathcam	= {false,SPR_FAT_W1,1,NULL,NULL,&s_fatdie1};

statetype s_fatdie1	= {false,SPR_FAT_W1,1,NULL,A_DeathScream,&s_fatdie2};
statetype s_fatdie2	= {false,SPR_FAT_W1,10,NULL,NULL,&s_fatdie3};
statetype s_fatdie3	= {false,SPR_FAT_DIE1,10,NULL,NULL,&s_fatdie4};
statetype s_fatdie4	= {false,SPR_FAT_DIE2,10,NULL,NULL,&s_fatdie5};
statetype s_fatdie5	= {false,SPR_FAT_DIE3,10,NULL,NULL,&s_fatdie6};
statetype s_fatdie6	= {false,SPR_FAT_DEAD,20,NULL,A_StartDeathCam,&s_fatdie6};

statetype s_fatshoot1 	= {false,SPR_FAT_SHOOT1,30,NULL,NULL,&s_fatshoot2};
statetype s_fatshoot2 	= {false,SPR_FAT_SHOOT2,10,NULL,T_GiftThrow,&s_fatshoot3};
statetype s_fatshoot3 	= {false,SPR_FAT_SHOOT3,10,NULL,T_Shoot,&s_fatshoot4};
statetype s_fatshoot4 	= {false,SPR_FAT_SHOOT4,10,NULL,T_Shoot,&s_fatshoot5};
statetype s_fatshoot5 	= {false,SPR_FAT_SHOOT3,10,NULL,T_Shoot,&s_fatshoot6};
statetype s_fatshoot6 	= {false,SPR_FAT_SHOOT4,10,NULL,T_Shoot,&s_fatchase1};

/*
=================
=
= T_SchabbThrow
=
=================
*/

void T_SchabbThrow (objtype *ob)
{
	long	deltax,deltay;
	float	angle;
	int		iangle;

	deltax = player->x - ob->x;
	deltay = ob->y - player->y;
	angle = atan2 (deltay,deltax);
	if (angle<0)
		angle = M_PI*2+angle;
	iangle = angle/(M_PI*2)*ANGLES;

	GetNewActor ();
	new->state = &s_needle1;
	new->ticcount = 1;

	new->tilex = ob->tilex;
	new->tiley = ob->tiley;
	new->x = ob->x;
	new->y = ob->y;
	new->obclass = needleobj;
	new->dir = nodir;
	new->angle = iangle;
	new->speed = 0x2000l;

	new->flags = FL_NONMARK;
	new->active = true;

	PlaySoundLocActor (SCHABBSTHROWSND,new);
}

/*
=================
=
= T_GiftThrow
=
=================
*/

void T_GiftThrow (objtype *ob)
{
	long	deltax,deltay;
	float	angle;
	int		iangle;

	deltax = player->x - ob->x;
	deltay = ob->y - player->y;
	angle = atan2 (deltay,deltax);
	if (angle<0)
		angle = M_PI*2+angle;
	iangle = angle/(M_PI*2)*ANGLES;

	GetNewActor ();
	new->state = &s_rocket;
	new->ticcount = 1;

	new->tilex = ob->tilex;
	new->tiley = ob->tiley;
	new->x = ob->x;
	new->y = ob->y;
	new->obclass = rocketobj;
	new->dir = nodir;
	new->angle = iangle;
	new->speed = 0x2000l;
	new->flags = FL_NONMARK;
	new->active = true;

	PlaySoundLocActor (MISSILEFIRESND,new);
}



/*
=================
=
= T_Schabb
=
=================
*/

void T_Schabb (objtype *ob)
{
	long move;
	int	dx,dy,dist;
	boolean	dodge;

	dodge = false;
	dx = abs(ob->tilex - player->tilex);
	dy = abs(ob->tiley - player->tiley);
	dist = dx>dy ? dx : dy;

	if (CheckLine(ob))						// got a shot at player?
	{

		if ( US_RndT() < (tics<<3) )
		{
		//
		// go into attack frame
		//
			NewState (ob,&s_schabbshoot1);
			return;
		}
		dodge = true;
	}

	if (ob->dir == nodir)
	{
		if (dodge)
			SelectDodgeDir (ob);
		else
			SelectChaseDir (ob);
		if (ob->dir == nodir)
			return;							// object is blocked in
	}

	move = ob->speed*tics;

	while (move)
	{
		if (ob->distance < 0)
		{
		//
		// waiting for a door to open
		//
			OpenDoor (-ob->distance-1);
			if (doorobjlist[-ob->distance-1].action != dr_open)
				return;
			ob->distance = TILEGLOBAL;	// go ahead, the door is now opoen
		}

		if (move < ob->distance)
		{
			MoveObj (ob,move);
			break;
		}

		//
		// reached goal tile, so select another one
		//

		//
		// fix position to account for round off during moving
		//
		ob->x = ((long)ob->tilex<<TILESHIFT)+TILEGLOBAL/2;
		ob->y = ((long)ob->tiley<<TILESHIFT)+TILEGLOBAL/2;

		move -= ob->distance;

		if (dist <4)
			SelectRunDir (ob);
		else if (dodge)
			SelectDodgeDir (ob);
		else
			SelectChaseDir (ob);

		if (ob->dir == nodir)
			return;							// object is blocked in
	}

}




/*
=================
=
= T_Gift
=
=================
*/

void T_Gift (objtype *ob)
{
	long move;
	int	dx,dy,dist;
	boolean	dodge;

	dodge = false;
	dx = abs(ob->tilex - player->tilex);
	dy = abs(ob->tiley - player->tiley);
	dist = dx>dy ? dx : dy;

	if (CheckLine(ob))						// got a shot at player?
	{

		if ( US_RndT() < (tics<<3) )
		{
		//
		// go into attack frame
		//
			NewState (ob,&s_giftshoot1);
			return;
		}
		dodge = true;
	}

	if (ob->dir == nodir)
	{
		if (dodge)
			SelectDodgeDir (ob);
		else
			SelectChaseDir (ob);
		if (ob->dir == nodir)
			return;							// object is blocked in
	}

	move = ob->speed*tics;

	while (move)
	{
		if (ob->distance < 0)
		{
		//
		// waiting for a door to open
		//
			OpenDoor (-ob->distance-1);
			if (doorobjlist[-ob->distance-1].action != dr_open)
				return;
			ob->distance = TILEGLOBAL;	// go ahead, the door is now opoen
		}

		if (move < ob->distance)
		{
			MoveObj (ob,move);
			break;
		}

		//
		// reached goal tile, so select another one
		//

		//
		// fix position to account for round off during moving
		//
		ob->x = ((long)ob->tilex<<TILESHIFT)+TILEGLOBAL/2;
		ob->y = ((long)ob->tiley<<TILESHIFT)+TILEGLOBAL/2;

		move -= ob->distance;

		if (dist <4)
			SelectRunDir (ob);
		else if (dodge)
			SelectDodgeDir (ob);
		else
			SelectChaseDir (ob);

		if (ob->dir == nodir)
			return;							// object is blocked in
	}

}




/*
=================
=
= T_Fat
=
=================
*/

void T_Fat (objtype *ob)
{
	long move;
	int	dx,dy,dist;
	boolean	dodge;

	dodge = false;
	dx = abs(ob->tilex - player->tilex);
	dy = abs(ob->tiley - player->tiley);
	dist = dx>dy ? dx : dy;

	if (CheckLine(ob))						// got a shot at player?
	{

		if ( US_RndT() < (tics<<3) )
		{
		//
		// go into attack frame
		//
			NewState (ob,&s_fatshoot1);
			return;
		}
		dodge = true;
	}

	if (ob->dir == nodir)
	{
		if (dodge)
			SelectDodgeDir (ob);
		else
			SelectChaseDir (ob);
		if (ob->dir == nodir)
			return;							// object is blocked in
	}

	move = ob->speed*tics;

	while (move)
	{
		if (ob->distance < 0)
		{
		//
		// waiting for a door to open
		//
			OpenDoor (-ob->distance-1);
			if (doorobjlist[-ob->distance-1].action != dr_open)
				return;
			ob->distance = TILEGLOBAL;	// go ahead, the door is now opoen
		}

		if (move < ob->distance)
		{
			MoveObj (ob,move);
			break;
		}

		//
		// reached goal tile, so select another one
		//

		//
		// fix position to account for round off during moving
		//
		ob->x = ((long)ob->tilex<<TILESHIFT)+TILEGLOBAL/2;
		ob->y = ((long)ob->tiley<<TILESHIFT)+TILEGLOBAL/2;

		move -= ob->distance;

		if (dist <4)
			SelectRunDir (ob);
		else if (dodge)
			SelectDodgeDir (ob);
		else
			SelectChaseDir (ob);

		if (ob->dir == nodir)
			return;							// object is blocked in
	}

}



/*
=============================================================================

							HITLERS

=============================================================================
*/


//
// fake
//
extern	statetype s_fakestand;

extern	statetype s_fakechase1;
extern	statetype s_fakechase1s;
extern	statetype s_fakechase2;
extern	statetype s_fakechase3;
extern	statetype s_fakechase3s;
extern	statetype s_fakechase4;

extern	statetype s_fakedie1;
extern	statetype s_fakedie2;
extern	statetype s_fakedie3;
extern	statetype s_fakedie4;
extern	statetype s_fakedie5;
extern	statetype s_fakedie6;

extern	statetype s_fakeshoot1;
extern	statetype s_fakeshoot2;
extern	statetype s_fakeshoot3;
extern	statetype s_fakeshoot4;
extern	statetype s_fakeshoot5;
extern	statetype s_fakeshoot6;
extern	statetype s_fakeshoot7;
extern	statetype s_fakeshoot8;
extern	statetype s_fakeshoot9;

statetype s_fakestand	= {false,SPR_FAKE_W1,0,T_Stand,NULL,&s_fakestand};

statetype s_fakechase1 	= {false,SPR_FAKE_W1,10,T_Fake,NULL,&s_fakechase1s};
statetype s_fakechase1s	= {false,SPR_FAKE_W1,3,NULL,NULL,&s_fakechase2};
statetype s_fakechase2 	= {false,SPR_FAKE_W2,8,T_Fake,NULL,&s_fakechase3};
statetype s_fakechase3 	= {false,SPR_FAKE_W3,10,T_Fake,NULL,&s_fakechase3s};
statetype s_fakechase3s	= {false,SPR_FAKE_W3,3,NULL,NULL,&s_fakechase4};
statetype s_fakechase4 	= {false,SPR_FAKE_W4,8,T_Fake,NULL,&s_fakechase1};

statetype s_fakedie1	= {false,SPR_FAKE_DIE1,10,NULL,A_DeathScream,&s_fakedie2};
statetype s_fakedie2	= {false,SPR_FAKE_DIE2,10,NULL,NULL,&s_fakedie3};
statetype s_fakedie3	= {false,SPR_FAKE_DIE3,10,NULL,NULL,&s_fakedie4};
statetype s_fakedie4	= {false,SPR_FAKE_DIE4,10,NULL,NULL,&s_fakedie5};
statetype s_fakedie5	= {false,SPR_FAKE_DIE5,10,NULL,NULL,&s_fakedie6};
statetype s_fakedie6	= {false,SPR_FAKE_DEAD,0,NULL,NULL,&s_fakedie6};

statetype s_fakeshoot1 	= {false,SPR_FAKE_SHOOT,8,NULL,T_FakeFire,&s_fakeshoot2};
statetype s_fakeshoot2 	= {false,SPR_FAKE_SHOOT,8,NULL,T_FakeFire,&s_fakeshoot3};
statetype s_fakeshoot3 	= {false,SPR_FAKE_SHOOT,8,NULL,T_FakeFire,&s_fakeshoot4};
statetype s_fakeshoot4 	= {false,SPR_FAKE_SHOOT,8,NULL,T_FakeFire,&s_fakeshoot5};
statetype s_fakeshoot5 	= {false,SPR_FAKE_SHOOT,8,NULL,T_FakeFire,&s_fakeshoot6};
statetype s_fakeshoot6 	= {false,SPR_FAKE_SHOOT,8,NULL,T_FakeFire,&s_fakeshoot7};
statetype s_fakeshoot7 	= {false,SPR_FAKE_SHOOT,8,NULL,T_FakeFire,&s_fakeshoot8};
statetype s_fakeshoot8 	= {false,SPR_FAKE_SHOOT,8,NULL,T_FakeFire,&s_fakeshoot9};
statetype s_fakeshoot9 	= {false,SPR_FAKE_SHOOT,8,NULL,NULL,&s_fakechase1};

//
// hitler
//
extern	statetype s_mechachase1;
extern	statetype s_mechachase1s;
extern	statetype s_mechachase2;
extern	statetype s_mechachase3;
extern	statetype s_mechachase3s;
extern	statetype s_mechachase4;

extern	statetype s_mechadie1;
extern	statetype s_mechadie2;
extern	statetype s_mechadie3;
extern	statetype s_mechadie4;

extern	statetype s_mechashoot1;
extern	statetype s_mechashoot2;
extern	statetype s_mechashoot3;
extern	statetype s_mechashoot4;
extern	statetype s_mechashoot5;
extern	statetype s_mechashoot6;


extern	statetype s_hitlerchase1;
extern	statetype s_hitlerchase1s;
extern	statetype s_hitlerchase2;
extern	statetype s_hitlerchase3;
extern	statetype s_hitlerchase3s;
extern	statetype s_hitlerchase4;

extern	statetype s_hitlerdie1;
extern	statetype s_hitlerdie2;
extern	statetype s_hitlerdie3;
extern	statetype s_hitlerdie4;
extern	statetype s_hitlerdie5;
extern	statetype s_hitlerdie6;
extern	statetype s_hitlerdie7;
extern	statetype s_hitlerdie8;
extern	statetype s_hitlerdie9;
extern	statetype s_hitlerdie10;

extern	statetype s_hitlershoot1;
extern	statetype s_hitlershoot2;
extern	statetype s_hitlershoot3;
extern	statetype s_hitlershoot4;
extern	statetype s_hitlershoot5;
extern	statetype s_hitlershoot6;

extern	statetype s_hitlerdeathcam;

statetype s_mechastand	= {false,SPR_MECHA_W1,0,T_Stand,NULL,&s_mechastand};

statetype s_mechachase1 	= {false,SPR_MECHA_W1,10,T_Chase,A_MechaSound,&s_mechachase1s};
statetype s_mechachase1s	= {false,SPR_MECHA_W1,6,NULL,NULL,&s_mechachase2};
statetype s_mechachase2 	= {false,SPR_MECHA_W2,8,T_Chase,NULL,&s_mechachase3};
statetype s_mechachase3 	= {false,SPR_MECHA_W3,10,T_Chase,A_MechaSound,&s_mechachase3s};
statetype s_mechachase3s	= {false,SPR_MECHA_W3,6,NULL,NULL,&s_mechachase4};
statetype s_mechachase4 	= {false,SPR_MECHA_W4,8,T_Chase,NULL,&s_mechachase1};

statetype s_mechadie1	= {false,SPR_MECHA_DIE1,10,NULL,A_DeathScream,&s_mechadie2};
statetype s_mechadie2	= {false,SPR_MECHA_DIE2,10,NULL,NULL,&s_mechadie3};
statetype s_mechadie3	= {false,SPR_MECHA_DIE3,10,NULL,A_HitlerMorph,&s_mechadie4};
statetype s_mechadie4	= {false,SPR_MECHA_DEAD,0,NULL,NULL,&s_mechadie4};

statetype s_mechashoot1 	= {false,SPR_MECHA_SHOOT1,30,NULL,NULL,&s_mechashoot2};
statetype s_mechashoot2 	= {false,SPR_MECHA_SHOOT2,10,NULL,T_Shoot,&s_mechashoot3};
statetype s_mechashoot3 	= {false,SPR_MECHA_SHOOT3,10,NULL,T_Shoot,&s_mechashoot4};
statetype s_mechashoot4 	= {false,SPR_MECHA_SHOOT2,10,NULL,T_Shoot,&s_mechashoot5};
statetype s_mechashoot5 	= {false,SPR_MECHA_SHOOT3,10,NULL,T_Shoot,&s_mechashoot6};
statetype s_mechashoot6 	= {false,SPR_MECHA_SHOOT2,10,NULL,T_Shoot,&s_mechachase1};


statetype s_hitlerchase1 	= {false,SPR_HITLER_W1,6,T_Chase,NULL,&s_hitlerchase1s};
statetype s_hitlerchase1s	= {false,SPR_HITLER_W1,4,NULL,NULL,&s_hitlerchase2};
statetype s_hitlerchase2 	= {false,SPR_HITLER_W2,2,T_Chase,NULL,&s_hitlerchase3};
statetype s_hitlerchase3 	= {false,SPR_HITLER_W3,6,T_Chase,NULL,&s_hitlerchase3s};
statetype s_hitlerchase3s	= {false,SPR_HITLER_W3,4,NULL,NULL,&s_hitlerchase4};
statetype s_hitlerchase4 	= {false,SPR_HITLER_W4,2,T_Chase,NULL,&s_hitlerchase1};

statetype s_hitlerdeathcam	= {false,SPR_HITLER_W1,10,NULL,NULL,&s_hitlerdie1};

statetype s_hitlerdie1	= {false,SPR_HITLER_W1,1,NULL,A_DeathScream,&s_hitlerdie2};
statetype s_hitlerdie2	= {false,SPR_HITLER_W1,10,NULL,NULL,&s_hitlerdie3};
statetype s_hitlerdie3	= {false,SPR_HITLER_DIE1,10,NULL,A_Slurpie,&s_hitlerdie4};
statetype s_hitlerdie4	= {false,SPR_HITLER_DIE2,10,NULL,NULL,&s_hitlerdie5};
statetype s_hitlerdie5	= {false,SPR_HITLER_DIE3,10,NULL,NULL,&s_hitlerdie6};
statetype s_hitlerdie6	= {false,SPR_HITLER_DIE4,10,NULL,NULL,&s_hitlerdie7};
statetype s_hitlerdie7	= {false,SPR_HITLER_DIE5,10,NULL,NULL,&s_hitlerdie8};
statetype s_hitlerdie8	= {false,SPR_HITLER_DIE6,10,NULL,NULL,&s_hitlerdie9};
statetype s_hitlerdie9	= {false,SPR_HITLER_DIE7,10,NULL,NULL,&s_hitlerdie10};
statetype s_hitlerdie10	= {false,SPR_HITLER_DEAD,20,NULL,A_StartDeathCam,&s_hitlerdie10};

statetype s_hitlershoot1 	= {false,SPR_HITLER_SHOOT1,30,NULL,NULL,&s_hitlershoot2};
statetype s_hitlershoot2 	= {false,SPR_HITLER_SHOOT2,10,NULL,T_Shoot,&s_hitlershoot3};
statetype s_hitlershoot3 	= {false,SPR_HITLER_SHOOT3,10,NULL,T_Shoot,&s_hitlershoot4};
statetype s_hitlershoot4 	= {false,SPR_HITLER_SHOOT2,10,NULL,T_Shoot,&s_hitlershoot5};
statetype s_hitlershoot5 	= {false,SPR_HITLER_SHOOT3,10,NULL,T_Shoot,&s_hitlershoot6};
statetype s_hitlershoot6 	= {false,SPR_HITLER_SHOOT2,10,NULL,T_Shoot,&s_hitlerchase1};

/*
===============
=
= A_HitlerMorph
=
===============
*/

void A_HitlerMorph (objtype *ob)
{
	unsigned	far *map,tile,hitpoints[5]={500,700,800,900,900};


	SpawnNewObj (ob->tilex,ob->tiley,&s_hitlerchase1);
	new->speed = SPDPATROL*5;

	new->x = ob->x;
	new->y = ob->y;

	new->distance = ob->distance;
	new->dir = ob->dir;
	new->flags = ob->flags | FL_SHOOTABLE;

	new->obclass = realhitlerobj;
	new->hitpoints = hitpoints[gamestate.difficulty];
}


////////////////////////////////////////////////////////
//
// A_MechaSound
// A_Slurpie
//
////////////////////////////////////////////////////////
void A_MechaSound (objtype *ob)
{
	if (areabyplayer[ob->areanumber])
		PlaySoundLocActor (MECHSTEPSND,ob);
}

#pragma argsused
void A_Slurpie (objtype *ob)
{
 SD_PlaySound(SLURPIESND);
}

/*
=================
=
= T_FakeFire
=
=================
*/

void T_FakeFire (objtype *ob)
{
	long	deltax,deltay;
	float	angle;
	int		iangle;

	deltax = player->x - ob->x;
	deltay = ob->y - player->y;
	angle = atan2 (deltay,deltax);
	if (angle<0)
		angle = M_PI*2+angle;
	iangle = angle/(M_PI*2)*ANGLES;

	GetNewActor ();
	new->state = &s_fire1;
	new->ticcount = 1;

	new->tilex = ob->tilex;
	new->tiley = ob->tiley;
	new->x = ob->x;
	new->y = ob->y;
	new->dir = nodir;
	new->angle = iangle;
	new->obclass = fireobj;
	new->speed = 0x1200l;
	new->flags = FL_NEVERMARK;
	new->active = true;
	
	SetSpawner(new,ob);

	PlaySoundLocActor (FLAMETHROWERSND,new);
}



/*
=================
=
= T_Fake
=
=================
*/

void T_Fake (objtype *ob)
{
	long move;
	int	dx,dy,dist;
	boolean	dodge;

	if (CheckLine(ob))			// got a shot at player?
	{
		if ( US_RndT() < (tics<<1) )
		{
		//
		// go into attack frame
		//
			NewState (ob,&s_fakeshoot1);
			return;
		}
	}

	if (ob->dir == nodir)
	{
		SelectDodgeDir (ob);
		if (ob->dir == nodir)
			return;							// object is blocked in
	}

	move = ob->speed*tics;

	while (move)
	{
		if (move < ob->distance)
		{
			MoveObj (ob,move);
			break;
		}

		//
		// reached goal tile, so select another one
		//

		//
		// fix position to account for round off during moving
		//
		ob->x = ((long)ob->tilex<<TILESHIFT)+TILEGLOBAL/2;
		ob->y = ((long)ob->tiley<<TILESHIFT)+TILEGLOBAL/2;

		move -= ob->distance;

		SelectDodgeDir (ob);

		if (ob->dir == nodir)
			return;							// object is blocked in
	}

}

/*
=================
=
= T_Gift
=
=================
*/

void T_Gift (objtype *ob)
{
	long move;
	int	dx,dy,dist;
	boolean	dodge;

	dodge = false;
	dx = abs(ob->tilex - player->tilex);
	dy = abs(ob->tiley - player->tiley);
	dist = dx>dy ? dx : dy;

	if (CheckLine(ob))						// got a shot at player?
	{

		if ( US_RndT() < (tics<<3) )
		{
		//
		// go into attack frame
		//
			NewState (ob,&s_giftshoot1);
			return;
		}
		dodge = true;
	}

	if (ob->dir == nodir)
	{
		if (dodge)
			SelectDodgeDir (ob);
		else
			SelectChaseDir (ob);
		if (ob->dir == nodir)
			return;							// object is blocked in
	}

	move = ob->speed*tics;

	while (move)
	{
		if (ob->distance < 0)
		{
		//
		// waiting for a door to open
		//
			OpenDoor (-ob->distance-1);
			if (doorobjlist[-ob->distance-1].action != dr_open)
				return;
			ob->distance = TILEGLOBAL;	// go ahead, the door is now opoen
		}

		if (move < ob->distance)
		{
			MoveObj (ob,move);
			break;
		}

		//
		// reached goal tile, so select another one
		//

		//
		// fix position to account for round off during moving
		//
		ob->x = ((long)ob->tilex<<TILESHIFT)+TILEGLOBAL/2;
		ob->y = ((long)ob->tiley<<TILESHIFT)+TILEGLOBAL/2;

		move -= ob->distance;

		if (dist <4)
			SelectRunDir (ob);
		else if (dodge)
			SelectDodgeDir (ob);
		else
			SelectChaseDir (ob);

		if (ob->dir == nodir)
			return;							// object is blocked in
	}

}

#endif

/*
============================
=
= SpawnBoss
=
= Credit: Wolf3DGuy
=
============================
*/
void SpawnBoss (enemy_t which, int tilex, int tiley)
{
	switch (which)
	{
#ifndef SPEAR
        case en_boss:
           SpawnNewObj (tilex,tiley,&s_bossstand);
	        break;

    	case en_schabbs:
            if (DigiMode != sds_Off)
                s_schabbdie2.tictime = 140;
            else
                s_schabbdie2.tictime = 5;

        	SpawnNewObj(tilex,tiley,&s_schabbstand);
        	break;

    	case en_fake:
        	SpawnNewObj(tilex,tiley,&s_fakestand);
        	break;

    	case en_hitler:
        	if (DigiMode != sds_Off)
                s_hitlerdie2.tictime = 140;
        	else
                s_hitlerdie2.tictime = 5;

        	SpawnNewObj(tilex,tiley,&s_mechastand);
        	break;

        case en_gretel:
            SpawnNewObj (tilex,tiley,&s_gretelstand);
            break;

    	case en_gift:
        	if (DigiMode != sds_Off)
                s_giftdie2.tictime = 140;
            else
                s_giftdie2.tictime = 5;

        	SpawnNewObj(tilex,tiley,&s_giftstand);
        	break;

        case en_fat:
            if (DigiMode != sds_Off)
                s_fatdie2.tictime = 140;
            else
                s_fatdie2.tictime = 5;

            SpawnNewObj(tilex,tiley,&s_fatstand);
            break;
#else
        case en_spectre:
            SpawnNewObj (tilex,tiley,&s_spectrewait1);
            break;

        case en_angel:
            if (SoundBlasterPresent && DigiMode != sds_Off)
                s_angeldie11.tictime = 105;

            SpawnNewObj (tilex,tiley,&s_angelstand);
            break;

        case en_mans:
            if (SoundBlasterPresent && DigiMode != sds_Off)
                s_mansdie01.tictime = 105;

            SpawnNewObj (tilex,tiley,&s_mansstand);
            break;

        case en_uber:
            if (SoundBlasterPresent && DigiMode != sds_Off)
                s_uberdie01.tictime = 95;

            SpawnNewObj (tilex,tiley,&s_uberstand);
            break;

        case en_will:
            if (SoundBlasterPresent && DigiMode != sds_Off)
                s_willdie2.tictime = 70;

            SpawnNewObj (tilex,tiley,&s_willstand);
            break;

        case en_death:
            if (SoundBlasterPresent && DigiMode != sds_Off)
                s_deathdie2.tictime = 105;

            SpawnNewObj (tilex,tiley,&s_deathstand);
            break;
        
        case en_fried:
            if (SoundBlasterPresent && DigiMode != sds_Off)
                s_frieddie2.tictime = 105;

            SpawnNewObj (tilex,tiley,&s_friedstand);
            break;    
#endif
	}
	new->obclass = guardobj+which;
	new->hitpoints = starthitpoints[gamestate.difficulty][which];
	new->dir = nodir;
#if NOTVER == 0
	new->flags |= FL_SHOOTABLE|FL_AMBUSH|FL_BOSS;
#else
	new->flags |= FL_SHOOTABLE|FL_AMBUSH;
	new->is_boss = true;
#endif
   new->speed = SPDPATROL;
   if (!loadedgame)
        gamestate.killtotal++;
}

/*
============================================================================

							STAND

============================================================================
*/


/*
===============
=
= T_Stand
=
===============
*/

void T_Stand (objtype *ob)
{
	SightPlayer (ob);
}


/*
============================================================================

								CHASE

============================================================================
*/

/*
=================
=
= T_Chase
=
=================
*/

void T_Chase (objtype *ob)
{
	long move;
	int	dx,dy,dist,chance;
	boolean	dodge;

	if (gamestate.victoryflag)
		return;

	dodge = false;
	if (CheckLine(ob))	// got a shot at player?
	{
		dx = abs(ob->tilex - player->tilex);
		dy = abs(ob->tiley - player->tiley);
		dist = dx>dy ? dx : dy;
		if (!dist || (dist==1 && ob->distance<0x4000) )
			chance = 300;
		else
			chance = (tics<<4)/dist;

		if ( US_RndT()<chance)
		{
		//
		// go into attack frame
		//
			switch (ob->obclass)
			{
			case guardobj:
				NewState (ob,&s_grdshoot1);
				break;
			case officerobj:
				NewState (ob,&s_ofcshoot1);
				break;
			case mutantobj:
				NewState (ob,&s_mutshoot1);
				break;
			case ssobj:
				NewState (ob,&s_ssshoot1);
				break;
#ifndef SPEAR
			case bossobj:
				NewState (ob,&s_bossshoot1);
				break;
			case gretelobj:
				NewState (ob,&s_gretelshoot1);
				break;
			case mechahitlerobj:
				NewState (ob,&s_mechashoot1);
				break;
			case realhitlerobj:
				NewState (ob,&s_hitlershoot1);
				break;
#else
			case angelobj:
				NewState (ob,&s_angelshoot1);
				break;
			case mansobj:
				NewState (ob,&s_mansshoot1);
				break;
			case uberobj:
				NewState (ob,&s_ubershoot1);
				break;
			case willobj:
				NewState (ob,&s_willshoot1);
				break;
			case deathobj:
				NewState (ob,&s_deathshoot1);
				break;
			case friedobj:
                NewState (ob,&s_friedshoot1);
                break;
#endif
			}
			return;
		}
		dodge = true;
	}

	if (ob->dir == nodir)
	{
		if (dodge)
			SelectDodgeDir (ob);
		else
			SelectChaseDir (ob);
		if (ob->dir == nodir)
			return;							// object is blocked in
	}

	move = ob->speed*tics;

	while (move)
	{
		if (ob->distance < 0)
		{
		//
		// waiting for a door to open
		//
			OpenDoor (-ob->distance-1);
			if (doorobjlist[-ob->distance-1].action != dr_open)
				return;
			ob->distance = TILEGLOBAL;	// go ahead, the door is now opoen
		}

		if (move < ob->distance)
		{
			MoveObj (ob,move);
			break;
		}

		//
		// reached goal tile, so select another one
		//

		//
		// fix position to account for round off during moving
		//
		ob->x = ((long)ob->tilex<<TILESHIFT)+TILEGLOBAL/2;
		ob->y = ((long)ob->tiley<<TILESHIFT)+TILEGLOBAL/2;

		move -= ob->distance;

		if (dodge)
			SelectDodgeDir (ob);
		else
			SelectChaseDir (ob);

		if (ob->dir == nodir)
			return;							// object is blocked in
	}

}


/*
=================
=
= T_Ghosts
=
=================
*/

void T_Ghosts (objtype *ob)
{
	long move;


	if (ob->dir == nodir)
	{
		SelectChaseDir (ob);
		if (ob->dir == nodir)
			return;							// object is blocked in
	}

	move = ob->speed*tics;

	while (move)
	{
		if (move < ob->distance)
		{
			MoveObj (ob,move);
			break;
		}

		//
		// reached goal tile, so select another one
		//

		//
		// fix position to account for round off during moving
		//
		ob->x = ((long)ob->tilex<<TILESHIFT)+TILEGLOBAL/2;
		ob->y = ((long)ob->tiley<<TILESHIFT)+TILEGLOBAL/2;

		move -= ob->distance;

		SelectChaseDir (ob);

		if (ob->dir == nodir)
			return;							// object is blocked in
	}

}

/*
=================
=
= T_DogChase
=
=================
*/

void T_DogChase (objtype *ob)
{
	long 	move;
	int		dist,chance;
	long	dx,dy;


	if (ob->dir == nodir)
	{
		SelectDodgeDir (ob);
		if (ob->dir == nodir)
			return;							// object is blocked in
	}

	move = ob->speed*tics;

	while (move)
	{
	//
	// check for byte range
	//
		dx = player->x - ob->x;
		if (dx<0)
			dx = -dx;
		dx -= move;
		if (dx <= MINACTORDIST)
		{
			dy = player->y - ob->y;
			if (dy<0)
				dy = -dy;
			dy -= move;
			if (dy <= MINACTORDIST)
			{
				NewState (ob,&s_dogjump1);
				return;
			}
		}

		if (move < ob->distance)
		{
			MoveObj (ob,move);
			break;
		}

		//
		// reached goal tile, so select another one
		//

		//
		// fix position to account for round off during moving
		//
		ob->x = ((long)ob->tilex<<TILESHIFT)+TILEGLOBAL/2;
		ob->y = ((long)ob->tiley<<TILESHIFT)+TILEGLOBAL/2;

		move -= ob->distance;

		SelectDodgeDir (ob);

		if (ob->dir == nodir)
			return;							// object is blocked in
	}

}



/*
============================================================================

								PATH

============================================================================
*/


/*
===============
=
= SelectPathDir
=
===============
*/

void SelectPathDir (objtype *ob)
{
	unsigned spot;

	spot = MAPSPOT(ob->tilex,ob->tiley,1)-ICONARROWS;

	if (spot<8)
	{
	// new direction
		ob->dir = spot;
	}

	ob->distance = TILEGLOBAL;

	if (!TryWalk (ob))
		ob->dir = nodir;
}


/*
===============
=
= T_Path
=
===============
*/

void T_Path (objtype *ob)
{
	long 	move;
	long 	deltax,deltay,size;

	if (SightPlayer (ob))
		return;

	if (ob->dir == nodir)
	{
		SelectPathDir (ob);
		if (ob->dir == nodir)
			return;					// all movement is blocked
	}


	move = ob->speed*tics;

	while (move)
	{
		if (ob->distance < 0)
		{
		//
		// waiting for a door to open
		//
			OpenDoor (-ob->distance-1);
			if (doorobjlist[-ob->distance-1].action != dr_open)
				return;
			ob->distance = TILEGLOBAL;	// go ahead, the door is now opoen
		}

		if (move < ob->distance)
		{
			MoveObj (ob,move);
			break;
		}

		if (ob->tilex>MAPSIZE || ob->tiley>MAPSIZE)
		{
			sprintf (str,"T_Path hit a wall at %u,%u, dir %u"
			,ob->tilex,ob->tiley,ob->dir);
			Quit (str);
		}



		ob->x = ((long)ob->tilex<<TILESHIFT)+TILEGLOBAL/2;
		ob->y = ((long)ob->tiley<<TILESHIFT)+TILEGLOBAL/2;
		move -= ob->distance;

		SelectPathDir (ob);

		if (ob->dir == nodir)
			return;					// all movement is blocked
	}
}


/*
=============================================================================

								FIGHT

=============================================================================
*/


/*
===============
=
= T_Shoot
=
= Try to damage the player, based on skill level and player's speed
=
===============
*/

void T_Shoot (objtype *ob)
{
	int	dx,dy,dist;
	int	hitchance,damage;

	hitchance = 128;

	if (!areabyplayer[ob->areanumber])
		return;

	if (!CheckLine (ob))			// player is behind a wall
	  return;

	dx = abs(ob->tilex - player->tilex);
	dy = abs(ob->tiley - player->tiley);
	dist = dx>dy ? dx:dy;

#ifndef SPEAR
	if (ob->obclass == ssobj || ob->obclass == bossobj)
#else
	if (ob->obclass == ssobj)
#endif
		dist = dist*2/3;					// ss are better shots

	if (thrustspeed >= RUNSPEED)
	{
		if (ob->flags&FL_VISABLE)
			hitchance = 160-dist*16;		// player can see to dodge
		else
			hitchance = 160-dist*8;
	}
	else
	{
		if (ob->flags&FL_VISABLE)
			hitchance = 256-dist*16;		// player can see to dodge
		else
			hitchance = 256-dist*8;
	}

// see if the shot was a hit

	if (US_RndT()<hitchance)
	{
        if (ob->obclass != ssobj)
        {
    		if (dist<2)
    			damage = US_RndT()>>2;
    		else if (dist<4)
    			damage = US_RndT()>>3;
    		else
    			damage = US_RndT()>>4;
        }
        else
        {
    		if (dist<2)
    			damage = US_RndT()>>3;
    		else if (dist<4)
    			damage = US_RndT()>>4;
    		else
    			damage = US_RndT()>>5;
        }

		TakeDamage (damage,ob);
	}

	switch(ob->obclass)
	{
	 case ssobj:
	   PlaySoundLocActor(SSFIRESND,ob);
	   break;
#ifndef SPEAR
	 case giftobj:
	 case fatobj:
	   PlaySoundLocActor(MISSILEFIRESND,ob);
	   break;
	 case mechahitlerobj:
	 case realhitlerobj:
	 case bossobj:
	   PlaySoundLocActor(BOSSFIRESND,ob);
	   break;
	 case schabbobj:
	   PlaySoundLocActor(SCHABBSTHROWSND,ob);
	   break;
	 case fakeobj:
	   PlaySoundLocActor(FLAMETHROWERSND,ob);
	   break;
#else
#ifndef NEEDLE
     case uberobj:
#endif     
     case willobj:
     case deathobj:       
     case mansobj:
	   PlaySoundLocActor(BOSSFIRESND,ob);
	   break;
#endif
	 default:
	   PlaySoundLocActor(ATKPISTOLSND,ob);
	}

}


/*
===============
=
= T_Bite
=
===============
*/

void T_Bite (objtype *ob)
{
	long	dx,dy;
	int	hitchance,damage;


	PlaySoundLocActor(DOGATTACKSND,ob);	// JAB

	dx = player->x - ob->x;
	if (dx<0)
		dx = -dx;
	dx -= TILEGLOBAL;
	if (dx <= MINACTORDIST)
	{
		dy = player->y - ob->y;
		if (dy<0)
			dy = -dy;
		dy -= TILEGLOBAL;
		if (dy <= MINACTORDIST)
		{
		   if (US_RndT()<180)
		   {
			   TakeDamage (US_RndT()>>4,ob);
			   return;
		   }
		}
	}

	return;
}


#ifndef SPEAR
/*
============================================================================

							BJ VICTORY

============================================================================
*/


//
// BJ victory
//

void T_BJRun (objtype *ob);
void T_BJJump (objtype *ob);
void T_BJDone (objtype *ob);
void T_BJYell (objtype *ob);

void T_DeathCam (objtype *ob);

extern	statetype s_bjrun1;
extern	statetype s_bjrun1s;
extern	statetype s_bjrun2;
extern	statetype s_bjrun3;
extern	statetype s_bjrun3s;
extern	statetype s_bjrun4;

extern	statetype s_bjjump1;
extern	statetype s_bjjump2;
extern	statetype s_bjjump3;
extern	statetype s_bjjump4;


statetype s_bjrun1 	= {false,SPR_BJ_W1,12,T_BJRun,NULL,&s_bjrun1s};
statetype s_bjrun1s	= {false,SPR_BJ_W1,3, NULL,NULL,&s_bjrun2};
statetype s_bjrun2 	= {false,SPR_BJ_W2,8,T_BJRun,NULL,&s_bjrun3};
statetype s_bjrun3 	= {false,SPR_BJ_W3,12,T_BJRun,NULL,&s_bjrun3s};
statetype s_bjrun3s	= {false,SPR_BJ_W3,3, NULL,NULL,&s_bjrun4};
statetype s_bjrun4 	= {false,SPR_BJ_W4,8,T_BJRun,NULL,&s_bjrun1};


statetype s_bjjump1	= {false,SPR_BJ_JUMP1,14,T_BJJump,NULL,&s_bjjump2};
statetype s_bjjump2	= {false,SPR_BJ_JUMP2,14,T_BJJump,T_BJYell,&s_bjjump3};
statetype s_bjjump3	= {false,SPR_BJ_JUMP3,14,T_BJJump,NULL,&s_bjjump4};
statetype s_bjjump4	= {false,SPR_BJ_JUMP4,300,NULL,T_BJDone,&s_bjjump4};


statetype s_deathcam = {false,0,0,NULL,NULL,NULL};


/*
===============
=
= SpawnBJVictory
=
===============
*/

void SpawnBJVictory (void)
{
	unsigned	far *map,tile;

	SpawnNewObj (player->tilex,player->tiley+1,&s_bjrun1);
	new->x = player->x;
	new->y = player->y;
	new->obclass = bjobj;
	new->dir = north;
	new->temp1 = 6;			// tiles to run forward
}



/*
===============
=
= T_BJRun
=
===============
*/

void T_BJRun (objtype *ob)
{
	long 	move;

	move = BJRUNSPEED*tics;

	while (move)
	{
		if (move < ob->distance)
		{
			MoveObj (ob,move);
			break;
		}


		ob->x = ((long)ob->tilex<<TILESHIFT)+TILEGLOBAL/2;
		ob->y = ((long)ob->tiley<<TILESHIFT)+TILEGLOBAL/2;
		move -= ob->distance;

		SelectPathDir (ob);

		if ( !(--ob->temp1) )
		{
			NewState (ob,&s_bjjump1);
			return;
		}
	}
}


/*
===============
=
= T_BJJump
=
===============
*/

void T_BJJump (objtype *ob)
{
	long 	move;

	move = BJJUMPSPEED*tics;
	MoveObj (ob,move);
}


/*
===============
=
= T_BJYell
=
===============
*/

void T_BJYell (objtype *ob)
{
	PlaySoundLocActor(YEAHSND,ob);	// JAB
}


/*
===============
=
= T_BJDone
=
===============
*/

#pragma argsused
void T_BJDone (objtype *ob)
{
	playstate = ex_victorious;				// exit castle tile
}



//===========================================================================


/*
===============
=
= CheckPosition
=
===============
*/

boolean	CheckPosition (objtype *ob)
{
	int	x,y,xl,yl,xh,yh;
	objtype *check;

	xl = (ob->x-PLAYERSIZE) >>TILESHIFT;
	yl = (ob->y-PLAYERSIZE) >>TILESHIFT;

	xh = (ob->x+PLAYERSIZE) >>TILESHIFT;
	yh = (ob->y+PLAYERSIZE) >>TILESHIFT;

	//
	// check for solid walls
	//
	for (y=yl;y<=yh;y++)
		for (x=xl;x<=xh;x++)
		{
			check = actorat[x][y];
			if (check && check<objlist)
				return false;
		}

	return true;
}


/*
===============
=
= A_StartDeathCam
=
===============
*/

void	A_StartDeathCam (objtype *ob)
{
	long	dx,dy;
	float	fangle;
	long    xmove,ymove;
	long	dist;
	int		temp,i;

	FinishPaletteShifts ();

	VW_WaitVBL (100);

	if (gamestate.victoryflag)
	{
		playstate = ex_victorious;				// exit castle tile
		return;
	}

	gamestate.victoryflag = true;
	VW_Bar (0,0,320,200-STATUSLINES,127);
	FizzleFade(bufferofs,displayofs,320,200-STATUSLINES,70,false);

	PM_UnlockMainMem ();
	CA_UpLevel ();
	CacheLump(LEVELEND_LUMP_START,LEVELEND_LUMP_END);
	#ifdef JAPAN
	#ifndef JAPDEMO
	CA_CacheScreen(C_LETSSEEPIC);
	#endif
	#else
	Write(0,7,STR_SEEAGAIN);
	#endif
	CA_DownLevel ();
	PM_CheckMainMem ();

	VW_UpdateScreen ();

	IN_UserInput(300);

//
// line angle up exactly
//
	NewState (player,&s_deathcam);

	player->x = gamestate.killx;
	player->y = gamestate.killy;

	dx = ob->x - player->x;
	dy = player->y - ob->y;

	fangle = atan2(dy,dx);			// returns -pi to pi
	if (fangle<0)
		fangle = M_PI*2+fangle;

	player->angle = fangle/(M_PI*2)*ANGLES;

//
// try to position as close as possible without being in a wall
//
	dist = 0x14000l;
	do
	{
		xmove = FixedByFrac(dist,costable[player->angle]);
		ymove = -FixedByFrac(dist,sintable[player->angle]);

		player->x = ob->x - xmove;
		player->y = ob->y - ymove;
		dist += 0x1000;

	} while (!CheckPosition (player));
	plux = player->x >> UNSIGNEDSHIFT;			// scale to fit in unsigned
	pluy = player->y >> UNSIGNEDSHIFT;
	player->tilex = player->x >> TILESHIFT;		// scale to tile values
	player->tiley = player->y >> TILESHIFT;

//
// go back to the game
//
	temp = bufferofs;
	for (i=0;i<3;i++)
	{
		bufferofs = screenloc[i];
		DrawPlayBorder ();
	}
	bufferofs = temp;

	fizzlein = true;
	switch (ob->obclass)
	{
#ifndef SPEAR
	case schabbobj:
		NewState (ob,&s_schabbdeathcam);
		break;
	case realhitlerobj:
		NewState (ob,&s_hitlerdeathcam);
		break;
	case giftobj:
		NewState (ob,&s_giftdeathcam);
		break;
	case fatobj:
		NewState (ob,&s_fatdeathcam);
		break;
#endif
	}

}

#endif

