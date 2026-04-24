// WL_AGENT.C

#include "WL_DEF.H"
#pragma hdrstop


/*
=============================================================================

						 LOCAL CONSTANTS

=============================================================================
*/

#define MAXMOUSETURN	10

#define MOVESCALE		150l
#define BACKMOVESCALE	100l
#define ANGLESCALE		20

/*
=============================================================================

						 GLOBAL VARIABLES

=============================================================================
*/



//
// player state info
//
boolean		running;
long		thrustspeed;

unsigned	plux,pluy;			// player coordinates scaled to unsigned

int			anglefrac;

boolean     gotgatgun;  // BJ grin fix
int         grincount;

objtype		*LastAttacker;

/*
=============================================================================

						 LOCAL VARIABLES

=============================================================================
*/

extern   statetype s_fire1;
extern   statetype s_rocket;

void	T_Player (objtype *ob);
void	T_Attack (objtype *ob);

statetype s_player = {false,0,0,T_Player,NULL,NULL};
statetype s_attack = {false,0,0,T_Attack,NULL,NULL};


long	playerxmove,playerymove;

struct atkinf
{
	char	tics,attack,frame;		// attack is 1 for gun, 2 for knife
} attackinfo[NUMWEAPONS][4] =

{
{ {6,0,1},{6,2,2},{6,0,3},{6,-1,4} },
{ {6,0,1},{6,1,2},{6,0,3},{6,-1,4} },
{ {6,0,1},{6,1,2},{6,3,3},{6,-1,4} },
{ {6,0,1},{6,1,2},{6,4,3},{6,-1,4} },
{ {6,0,1},{6,1,2},{6,4,3},{6,-1,4} },
{ {6,0,1},{6,1,2},{6,0,3},{6,-1,4} },
};


//int	strafeangle[9] = {0,90,180,270,45,135,225,315,0};

void DrawWeapon (void);
void GiveWeapon (int weapon);
void GiveAmmo   (int weapon,char ammo);

//===========================================================================

//----------

void Attack (void);
void Use (void);
void Search (objtype *ob);
void SelectWeapon (void);
void SelectItem (void);

//----------

boolean TryMove (objtype *ob);
void T_Player (objtype *ob);

void ClipMove (objtype *ob, long xmove, long ymove);

/*
=============================================================================

						CONTROL STUFF

=============================================================================
*/

int CheckAmmo (int weapon)
{
    switch (weapon)
    {
    case wp_knife:
        return 1;
    case wp_pistol:
    case wp_machinegun:
    case wp_chaingun:
        if (gamestate.ammo > 0) return 1;
        break;
    case wp_flamethrower:
        if (gamestate.fuel > 0) return 1;
        break;
    case wp_rocketlauncher:
        if (gamestate.rockets > 0) return 1;
        break;
   }
   return 0;
}

/*
======================
=
= CheckWeaponChange
=
= Keys 1-4 change weapons
=
======================
*/

void CheckWeaponChange (void)
{
	int i, kbuttons[NUMWEAPONS]={sc_1,sc_2,sc_3,sc_4,sc_5,sc_6}; 

   for (i=0;i<NUMWEAPONS;i++)
     if ((Keyboard[kbuttons[i]]) && (i != gamestate.weapon))
     { 
        if (gamestate.bestweapon & (1<<i) && CheckAmmo(i)) 
        {
            if (gamestate.chosenweapon != i && fancyweaps)
            {
                SD_PlaySound(CHANGEWEAPSND);
            }
            gamestate.chosenweapon = gamestate.nextweapon = i;
            gamestate.changingweapon = gamestate.goingdown = true;  
            DrawWeapon (); 
            DrawAmmo (); 
            return; 
        }
     }
}


/*
=======================
=
= ControlMovement
=
= Takes controlx,controly, and buttonstate[bt_strafe]
=
= Changes the player's angle and position
=
= There is an angle hack because when going 70 fps, the roundoff becomes
= significant
=
=======================
*/

void ControlMovement (objtype *ob)
{
	long	oldx,oldy;
	int		angle,maxxmove;
	int		angleunits;

	thrustspeed = 0;

	oldx = player->x;
	oldy = player->y;

    if (Keyboard[sc_A] || Keyboard[sc_D]) 
	{
	//
	// strafing
	//
	//
		if (strafe > 0)
		{
			angle = ob->angle - ANGLES/4;
			if (angle < 0)
				angle += ANGLES;
			Thrust (angle,strafe*MOVESCALE);	// move to left
		}
		else if (strafe < 0)
		{
			angle = ob->angle + ANGLES/4;
			if (angle >= ANGLES)
				angle -= ANGLES;
			Thrust (angle,-strafe*MOVESCALE);	// move to right
		}
	}


//
// side to side move
//
	if (buttonstate[bt_strafe])
	{
	//
	// strafing
	//
	//
		if (strafe > 0)
		{
			angle = ob->angle - ANGLES/4;
			if (angle < 0)
				angle += ANGLES;
			Thrust (angle,strafe*MOVESCALE);	// move to left
		}
		else if (strafe < 0)
		{
			angle = ob->angle + ANGLES/4;
			if (angle >= ANGLES)
				angle -= ANGLES;
			Thrust (angle,-strafe*MOVESCALE);	// move to right
		}                   
    }
    else
    {
	//
	// not strafing
	//
		anglefrac += controlx;
		angleunits = anglefrac/ANGLESCALE;
		anglefrac -= angleunits*ANGLESCALE;
		ob->angle -= angleunits;

		if (ob->angle >= ANGLES)
			ob->angle -= ANGLES;
		if (ob->angle < 0)
			ob->angle += ANGLES;

	}

//
// forward/backwards move
//
	if (controly < 0)
	{
		Thrust (ob->angle,-controly*MOVESCALE);	// move forwards
	}
	else if (controly > 0)
	{
		angle = ob->angle + ANGLES/2;
		if (angle >= ANGLES)
			angle -= ANGLES;
		Thrust (angle,controly*BACKMOVESCALE);		// move backwards
	}

	if (gamestate.victoryflag)		// watching the BJ actor
		return;

//
// calculate total move
//
	playerxmove = player->x - oldx;
	playerymove = player->y - oldy;
}

/*
=============================================================================

					STATUS WINDOW STUFF

=============================================================================
*/


/*
==================
=
= StatusDrawPic
=
==================
*/

void StatusDrawPic (unsigned x, unsigned y, unsigned picnum)
{
	unsigned	temp;

	temp = bufferofs;
	bufferofs = 0;

	bufferofs = PAGE1START+(200-STATUSLINES)*SCREENWIDTH;
	LatchDrawPic (x,y,picnum);
	bufferofs = PAGE2START+(200-STATUSLINES)*SCREENWIDTH;
	LatchDrawPic (x,y,picnum);
	bufferofs = PAGE3START+(200-STATUSLINES)*SCREENWIDTH;
	LatchDrawPic (x,y,picnum);

	bufferofs = temp;
}


/*
==================
=
= DrawFace
=
==================
*/

void DrawFace (void)
{
	if (gamestate.health)
	{
		#ifdef SPEAR
		if (godmode || quadmode)
			StatusDrawPic (17,4,GODMODEFACE1PIC+gamestate.faceframe);
#ifndef EXTRAHLTH
		else
#else
        else if (gamestate.health < 101)
#endif
		#endif
		StatusDrawPic (17,4,FACE1APIC+3*((100-gamestate.health)/16)+gamestate.faceframe);
#ifdef EXTRAHLTH
        else
            StatusDrawPic(17,4,FACE1APIC+gamestate.faceframe);
#endif
	}
	else
	{
#ifndef SPEAR
	 if (LastAttacker->obclass == needleobj)
	   StatusDrawPic (17,4,MUTANTBJPIC);
	 else
#endif
	   StatusDrawPic (17,4,FACE8APIC);
	}
}


/*
===============
=
= UpdateFace
=
= Calls draw face if time to change
=
===============
*/

#define FACETICS	70

int	facecount;

void	UpdateFace (void)
{
    
    if (gotgatgun)                       // use for BJ grin pic hold
        {
          if ((grincount += tics) >= 290)    
              {                                     // keep returning until count      
                grincount = 0;                // reaches 290, then reset and     
                gotgatgun = false;          // allow normal face updates
              }
           else
              return;
        }

	facecount += tics;
	if (facecount > US_RndT())
	{
		gamestate.faceframe = (US_RndT()>>6);
		if (gamestate.faceframe==3)
			gamestate.faceframe = 1;

		facecount = 0;
		DrawFace ();
	}
}



/*
===============
=
= LatchNumber
=
= right justifies and pads with blanks
=
===============
*/

void	LatchNumber (int x, int y, int width, long number)
{
	unsigned	length,c;
	char	str[20];

	ltoa (number,str,10);

	length = (unsigned) strlen (str);

	while (length<width)
	{
		StatusDrawPic (x,y,N_BLANKPIC);
		x++;
		width--;
	}

	c= length <= width ? 0 : length-width;

	while (c<length)
	{
		StatusDrawPic (x,y,str[c]-'0'+ N_0PIC);
		x++;
		c++;
	}
}


/*
===============
=
= DrawHealth
=
===============
*/

void	DrawHealth (void)
{
	LatchNumber (21,16,3,gamestate.health);
}


/*
===============
=
= TakeDamage
=
===============
*/

void	TakeDamage (int points,objtype *attacker)
{
	LastAttacker = attacker;

	if (gamestate.victoryflag)
		return;
	if (gamestate.difficulty==gd_baby)
	  points>>=2;
	else if (gamestate.difficulty == gd_extreme)
	  points<<=2;

	if (!godmode)
		gamestate.health -= points;
	
    gotgatgun = false;     // BJ grin fix
    grincount = 0; 

	if (gamestate.health<=0)
	{
		gamestate.health = 0;
		playstate = ex_died;
		killerobj = attacker;
	}

	StartDamageFlash (points);

	gotgatgun=0;

	DrawHealth ();
	DrawFace ();

	//
	// MAKE BJ'S EYES BUG IF MAJOR DAMAGE!
	//
	#ifdef SPEAR
	if (points > 30 && gamestate.health!=0 && !godmode)
	{
		StatusDrawPic (17,4,BJOUCHPIC);
		facecount = 0;
	}
	#endif

}


/*
===============
=
= HealSelf
=
===============
*/

void	HealSelf (int points)
{
	gamestate.health += points;
	if (gamestate.health>100)
		gamestate.health = 100;

	DrawHealth ();
    gotgatgun = false;     // BJ grin fix
    grincount = 0; 

	DrawFace ();
}


//===========================================================================


/*
===============
=
= DrawLevel
=
===============
*/

void	DrawLevel (void)
{
#ifdef SPEAR
	if (gamestate.mapon == FINALMAP)
		LatchNumber (2,16,2,18);
	else
#endif
	LatchNumber (2,16,2,gamestate.mapon+1);
}

//===========================================================================


/*
===============
=
= DrawLives
=
===============
*/

void	DrawLives (void)
{
	LatchNumber (14,16,1,gamestate.lives);
}


/*
===============
=
= GiveExtraMan
=
===============
*/

void	GiveExtraMan (void)
{
	if (gamestate.lives<9)
		gamestate.lives++;
	DrawLives ();
	SD_PlaySound (BONUS1UPSND);
}

//===========================================================================

/*
===============
=
= DrawScore
=
===============
*/

void	DrawScore (void)
{
	LatchNumber (6,16,6,gamestate.score);
}

/*
===============
=
= GivePoints
=
===============
*/

void	GivePoints (long points)
{
	gamestate.score += points;
	while (gamestate.score >= gamestate.nextextra)
	{
		gamestate.nextextra += EXTRAPOINTS;
		GiveExtraMan ();
	}
	DrawScore ();
}

//===========================================================================

/*
==================
=
= DrawWeapon
=
==================
*/

void DrawWeapon (void)
{
	StatusDrawPic (32,8,KNIFEPIC+gamestate.weapon);
}


/*
==================
=
= DrawKeys
=
==================
*/

void DrawKeys (void)
{
	if (gamestate.keys & 1)
		StatusDrawPic (30,4,GOLDKEYPIC);
	else
		StatusDrawPic (30,4,NOKEYPIC);

	if (gamestate.keys & 2)
		StatusDrawPic (30,20,SILVERKEYPIC);
	else
		StatusDrawPic (30,20,NOKEYPIC);
}



/*
==================
=
= GiveWeapon
=
==================
*/

void GiveWeapon (int weapon)
{
	switch (weapon)
	{
	case wp_machinegun:
	    GiveAmmo (1,6);
		break;
	case wp_chaingun:
	    GiveAmmo (1,12);
		break;
	case wp_flamethrower:
	    GiveAmmo (2,24);
		break;
	case wp_rocketlauncher:
	    GiveAmmo (3,3);
		break;
	}

	gamestate.bestweapon |= (1<<weapon);
	if (gamestate.chosenweapon != weapon) // Hope it fixes it :)
	{
       gamestate.changingweapon = gamestate.goingdown = true;
       gamestate.nextweapon = gamestate.chosenweapon = weapon;
    }

    DrawAmmo ();
	DrawWeapon ();
}


//===========================================================================

/*
===============
=
= DrawAmmo
=
===============
*/

void	DrawAmmo (void)
{
    switch (gamestate.chosenweapon)
    {
    case wp_knife:
        LatchNumber (26,16,3,0);
        break;
	case wp_pistol:
	case wp_machinegun:
	case wp_chaingun:
		LatchNumber (26,16,3,gamestate.ammo);
		break;
    case wp_flamethrower:
        LatchNumber (26,16,3,gamestate.fuel);
        break;
    case wp_rocketlauncher:
        LatchNumber (26,16,3,gamestate.rockets);
        break;
    }
}


/*
===============
=
= GiveAmmo
=
===============
*/

void GiveAmmo (int weapon,char ammo)
{
	if (!gamestate.ammo &&!gamestate.fuel && !gamestate.rockets )	// knife was out
	{
		if (!gamestate.attackframe)
		{
			gamestate.weapon = gamestate.chosenweapon;
			DrawWeapon ();
		}
	}
     switch (weapon)
     {
        case 1:
            gamestate.ammo += ammo;
            if (gamestate.ammo > gamestate.maxammo) gamestate.ammo = gamestate.maxammo;
            break;
        case 2:
            gamestate.fuel += ammo;
            if (gamestate.fuel > gamestate.maxfuel) gamestate.fuel = gamestate.maxfuel;
            break;
        case 3:
            gamestate.rockets += ammo;
            if (gamestate.rockets > gamestate.maxrockets) gamestate.rockets = gamestate.maxrockets;
            break;
     }
	DrawAmmo ();
}

//===========================================================================

/*
==================
=
= GiveKey
=
==================
*/

void GiveKey (int key)
{
	gamestate.keys |= (1<<key);
	DrawKeys ();
}



/*
=============================================================================

							MOVEMENT

=============================================================================
*/




/*
===================
=
= GetBonus
=
===================
*/
void GetBonus (statobj_t *check)
{
    boolean bonusflash = true;
	switch (check->itemnumber)
	{
	case	bo_firstaid:
#ifdef EXTRAHLTH       
		if (gamestate.health >= 100)
#else
		if (gamestate.health == 100)
#endif
			return;

		SD_PlaySound (HEALTH2SND);
		HealSelf (25);
		GetMessage ("Picked up a first aid kit.");
		break;

	case	bo_key1:
	case	bo_key2:
		GiveKey (check->itemnumber - bo_key1);
		SD_PlaySound (GETKEYSND);
		break;

	case	bo_cross:
		SD_PlaySound (BONUS1SND);
		GivePoints (100);
		gamestate.treasurecount++;
		break;
	case	bo_chalice:
		SD_PlaySound (BONUS2SND);
		GivePoints (500);
		gamestate.treasurecount++;
		break;
	case	bo_bible:
		SD_PlaySound (BONUS3SND);
		GivePoints (1000);
		gamestate.treasurecount++;
		break;
	case	bo_crown:
		SD_PlaySound (BONUS4SND);
		GivePoints (5000);
		gamestate.treasurecount++;
		break;

	case	bo_clip:
		if (gamestate.ammo == gamestate.maxammo)
			return;

		SD_PlaySound (GETAMMOSND);
		GiveAmmo (1,8);
		GetMessage ("Picked up a clip.");
		break;
	case	bo_clip2:
		if (gamestate.ammo == gamestate.maxammo)
			return;

		SD_PlaySound (GETAMMOSND);
		GiveAmmo (1,4);
		GetMessage ("Picked up a used clip.");
		break;
	
	case    bo_fuel:
		if (gamestate.fuel == gamestate.maxfuel)
			return;

		SD_PlaySound (GETAMMOSND);
		GiveAmmo (2,14);
		GetMessage ("Picked up a fuel can");
		break;
	
	case    bo_rockets:
        if (gamestate.rockets == gamestate.maxrockets)
			return;
        
		SD_PlaySound (GETAMMOSND);
		GiveAmmo (3,3);
		GetMessage ("Got some rockets.");
		break;
	
	case   bo_backpack:       
        
        if (gamestate.difficulty < gd_medium)
            HealSelf (25);
        
        gamestate.maxammo = 150;
        gamestate.maxfuel = 200;
        gamestate.maxrockets = 100;
        
        SD_PlaySound (GETBACKPACKSND);
        GetMessage ("Picked up a BackPack!");
		GiveAmmo (1,10);
		GiveAmmo (2,10);
		GiveAmmo (3,5);
		break;

#ifdef SPEAR
	case	bo_25clip:
		if (gamestate.ammo == gamestate.maxammo)
		  return;

        GetMessage ("Ammo box.");
		SD_PlaySound (GETAMMOBOXSND);
		GiveAmmo (1,25);
		break;
#endif

	case	bo_machinegun:
		SD_PlaySound (GETMACHINESND);
		GiveWeapon (wp_machinegun);
		GetMessage ("You got the Machine-Gun!");
		break;
	case	bo_chaingun:
		if (DigiMode != sds_Off) SD_PlaySound(YEAHSND);
        SD_PlaySound (GETGATLINGSND);
		GiveWeapon (wp_chaingun);

		StatusDrawPic (17,4,GOTGATLINGPIC);
		facecount = 0;
        gotgatgun = true;     // BJ grin fix
        grincount = 0;
        GetMessage ("You got the Gatling-Gun!");
		break;

	case	bo_flamethrower:
		SD_PlaySound (GETFLAMERSND);
		GiveWeapon (wp_flamethrower);
		GetMessage ("You got the Flamethrower!");
		break;

	case	bo_rocketlauncher:
		if (DigiMode != sds_Off) SD_PlaySound(YEAHSND);
        SD_PlaySound (GETLAUNCHERSND);
		GiveWeapon (wp_rocketlauncher);

		StatusDrawPic (17,4,GOTGATLINGPIC);
		facecount = 0;
        gotgatgun = true;     // BJ grin fix
        grincount = 0;
        GetMessage ("You got the Rocket Launcher!");
		break;

	case	bo_fullheal:
		SD_PlaySound (BONUS1UPSND);
#ifndef EXTRAHLTH
		HealSelf (99);
#else
        if(gamestate.health < 125)
            gamestate.health = 125;
        else if(gamestate.health < 150)
            gamestate.health = 150;
        else if(gamestate.health < 175)
            gamestate.health = 175;
        else
            gamestate.health = 200;   
#endif
		GiveAmmo (1,25);
		GiveAmmo (2,25);
		GiveAmmo (3,10);
		GiveExtraMan ();
#ifdef EXTRAHLTH
		DrawHealth ();
#endif
#ifndef NEEDLE
		gamestate.treasurecount++;
#endif
		GetMessage ("What in the bloody hell is THIS?"); // KRONC!!!
		break;

	case	bo_food:
#ifdef EXTRAHLTH       
		if (gamestate.health >= 100)
#else
		if (gamestate.health == 100)
#endif
			return;

		SD_PlaySound (HEALTH1SND);
		HealSelf (10);
		GetMessage ("Ate some chicken dinner.");
		break;

	case	bo_alpo:
#ifdef EXTRAHLTH       
		if (gamestate.health >= 100)
#else
		if (gamestate.health == 100)
#endif
			return;

#ifndef SPEAR
		SD_PlaySound (HEALTH1SND);
#else
        SD_PlaySound (HEALTH0SND);
#endif
		HealSelf (4);
		GetMessage ("Tastes like shit... BLEURGH!");
		break;

	case	bo_gibs:
		if (gamestate.health >10)
			return;

		SD_PlaySound (SLURPIESND);
		HealSelf (1);
		break;

	case    bo_god:
        SD_PlaySound (GETPOWSND); // enter any sound you like here
        GivePoints (10000);
        godmode = 1;
        gamestate.godmode = true;
        gamestate.godmodecount++;
        break;
    
	case    bo_quad:
        SD_PlaySound (GETPOWSND);
        GivePoints (10000);
        quadmode = 1;
        gamestate.quadmode = true;
        gamestate.quadmodecount++;
        break;

	case    bo_secret:
        gamestate.secretcount++;
        GetMessage ("Found a secret!");
        bonusflash = false;
        break;

	case	bo_spear:
		GetMessage ("Uh oh...");
        spearflag = true;
		spearx = player->x;
		speary = player->y;
		spearangle = player->angle;
		playstate = ex_completed;
	}

    // Flash the screen
    if (bonusflash) StartBonusFlash ();

	check->shapenum = -1;			// remove from list
}


/*
===================
=
= TryMove
=
= returns true if move ok
= debug: use pointers to optimize
===================
*/

boolean TryMove (objtype *ob)
{
	int			xl,yl,xh,yh,x,y;
	objtype		*check;
	long		deltax,deltay;

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

//
// check for actors
//
	if (yl>0)
		yl--;
	if (yh<MAPSIZE-1)
		yh++;
	if (xl>0)
		xl--;
	if (xh<MAPSIZE-1)
		xh++;

	for (y=yl;y<=yh;y++)
		for (x=xl;x<=xh;x++)
		{
			check = actorat[x][y];
			if (check > objlist
			&& (check->flags & FL_SHOOTABLE) )
			{
				deltax = ob->x - check->x;
				if (deltax < -MINACTORDIST || deltax > MINACTORDIST)
					continue;
				deltay = ob->y - check->y;
				if (deltay < -MINACTORDIST || deltay > MINACTORDIST)
					continue;

				return false;
			}
		}

	return true;
}


/*
===================
=
= ClipMove
=
===================
*/

void ClipMove (objtype *ob, long xmove, long ymove)
{
	long	basex,basey;

	basex = ob->x;
	basey = ob->y;

	ob->x = basex+xmove;
	ob->y = basey+ymove;
	if (TryMove (ob))
		return;

	if (noclip && ob->x > 2*TILEGLOBAL && ob->y > 2*TILEGLOBAL &&
	ob->x < (((long)(mapwidth-1))<<TILESHIFT)
	&& ob->y < (((long)(mapheight-1))<<TILESHIFT) )
		return;		// walk through walls

	if (!SD_SoundPlaying())
		SD_PlaySound (HITWALLSND);

	ob->x = basex+xmove;
	ob->y = basey;
	if (TryMove (ob))
		return;

	ob->x = basex;
	ob->y = basey+ymove;
	if (TryMove (ob))
		return;

	ob->x = basex;
	ob->y = basey;
}

//==========================================================================

/*
===================
=
= VictoryTile
=
===================
*/

void VictoryTile (void)
{
#ifndef SPEAR
	SpawnBJVictory ();
#endif

	gamestate.victoryflag = true;
}


/*
===================
=
= Thrust
=
===================
*/

void Thrust (int angle, long speed)
{
	long xmove,ymove;
	unsigned	offset;

	thrustspeed += speed;
//
// moving bounds speed
//
	if (speed >= MINDIST*2)
		speed = MINDIST*2-1;

	xmove = FixedByFrac(speed,costable[angle]);
	ymove = -FixedByFrac(speed,sintable[angle]);

	ClipMove(player,xmove,ymove);

	player->tilex = player->x >> TILESHIFT;		// scale to tile values
	player->tiley = player->y >> TILESHIFT;

	offset = farmapylookup[player->tiley]+player->tilex;
	player->areanumber = *(mapsegs[0] + offset) -AREATILE;

	if (*(mapsegs[1] + offset) == EXITTILE)
		VictoryTile ();
	if (*(mapsegs[1] + offset) == EXITLEVEL)
		playstate = ex_completed;
}


/*
=============================================================================

								ACTIONS

=============================================================================
*/


/*
===============
=
= Cmd_Fire
=
===============
*/

void Cmd_Fire (void)
{
    
#ifdef WEAPONC
    if(gamestate.changingweapon == true) 
        return; 
#endif
                    
	buttonheld[bt_attack] = true;

	gamestate.weaponframe = 0;

	player->state = &s_attack;

	gamestate.attackframe = 0;
	gamestate.attackcount =
		attackinfo[gamestate.weapon][gamestate.attackframe].tics;
	gamestate.weaponframe =
		attackinfo[gamestate.weapon][gamestate.attackframe].frame;
}

//===========================================================================

/*
===============
=
= Cmd_Use
=
===============
*/

void Cmd_Use (void)
{
	objtype 	*check;
	int			checkx,checky,doornum,dir;
	boolean		elevatorok;


//
// find which cardinal direction the player is facing
//
	if (player->angle < ANGLES/8 || player->angle > 7*ANGLES/8)
	{
		checkx = player->tilex + 1;
		checky = player->tiley;
		dir = di_east;
		elevatorok = true;
	}
	else if (player->angle < 3*ANGLES/8)
	{
		checkx = player->tilex;
		checky = player->tiley-1;
		dir = di_north;
		elevatorok = false;
	}
	else if (player->angle < 5*ANGLES/8)
	{
		checkx = player->tilex - 1;
		checky = player->tiley;
		dir = di_west;
		elevatorok = true;
	}
	else
	{
		checkx = player->tilex;
		checky = player->tiley + 1;
		dir = di_south;
		elevatorok = false;
	}

	doornum = tilemap[checkx][checky];
	if (*(mapsegs[1]+farmapylookup[checky]+checkx) == PUSHABLETILE)
	{
	//
	// pushable wall
	//

		PushWall (checkx,checky,dir);
		return;
	}
	if (!buttonheld[bt_use] && doornum == ELEVATORTILE && elevatorok)
	{
	//
	// use elevator
	//
		buttonheld[bt_use] = true;

		tilemap[checkx][checky]++;		// flip switch
		if (*(mapsegs[0]+farmapylookup[player->tiley]+player->tilex) == ALTELEVATORTILE)
			playstate = ex_secretlevel;
		else
			playstate = ex_completed;
		SD_PlaySound (LEVELDONESND);
		SD_WaitSoundDone();
	}
#if NOTVER > 0
	else if (!buttonheld[bt_use] && (doornum == SWITCHTILE || doornum == SWITCHTILE+1) && elevatorok) // if you want the switch to be on both sides of the wall, remove the "&& elevatorok" part
	{
	//
	// use switch
	//
		buttonheld[bt_use] = true;

		if (doornum==SWITCHTILE)
		{
		tilemap[checkx][checky]++;		// flip switch
		}
		else
		{
		tilemap[checkx][checky]--;		// flip switch
		}
		gamestate.electricity^=1;	// instead of using this. This one changes from the actual state to the second one, the others change to on, when switch is in position A, off when in position B
		SD_PlaySound (LEVELDONESND);
		SD_WaitSoundDone();
	}

#endif
	else if (!buttonheld[bt_use] && doornum & 0x80)
	{
		buttonheld[bt_use] = true;
		OperateDoor (doornum & ~0x80);
	}
#ifdef NEEDLEDEMO
#ifndef UZISFIXES
	else
		SD_PlaySound (DONOTHINGSND);
#endif
#else
#ifndef NEEDLE
	else
		SD_PlaySound (DONOTHINGSND);
#endif
#endif

}

/*
=============================================================================

						   PLAYER CONTROL

=============================================================================
*/



/*
===============
=
= SpawnPlayer
=
===============
*/

void SpawnPlayer (int tilex, int tiley, int dir)
{
	player->obclass = playerobj;
	player->active = true;
	player->tilex = tilex;
	player->tiley = tiley;
	player->areanumber =
		*(mapsegs[0] + farmapylookup[player->tiley]+player->tilex);
	player->x = ((long)tilex<<TILESHIFT)+TILEGLOBAL/2;
	player->y = ((long)tiley<<TILESHIFT)+TILEGLOBAL/2;
	player->state = &s_player;
	player->angle = (1-dir)*90;
	if (player->angle<0)
		player->angle += ANGLES;
	player->flags = FL_NEVERMARK;
	Thrust (0,0);				// set some variables

	InitAreas ();
}


//===========================================================================

/*
===============
=
= T_KnifeAttack
=
= Update player hands, and try to do damage when the proper frame is reached
=
===============
*/

void	KnifeAttack (objtype *ob)
{
	objtype *check,*closest;
	long	dist;

	SD_PlaySound (ATKKNIFESND);
// actually fire
	dist = 0x7fffffff;
	closest = NULL;
	for (check=ob->next ; check ; check=check->next)
		if ( (check->flags & FL_SHOOTABLE)
		&& (check->flags & FL_VISABLE)
		&& abs (check->viewx-centerx) < shootdelta
		)
		{
			if (check->transx < dist)
			{
				dist = check->transx;
				closest = check;
			}
		}

	if (!closest || dist> 0x18000l)
	{
	// missed

		return;
	}

// hit something
	DamageActor (closest,US_RndT() >> 4);
}

/*
==========================
=
= T_FlameThrowerAttack
=
==========================
*/

void FlameThrowerAttack (objtype *ob)
{
   if (!gamestate.fuel)
       return;

   SD_PlaySound (FLAMETHROWERSND);

   GetNewActor ();
   new->state = &s_fire1;
   new->ticcount = 1;

   new->tilex = ob->tilex;
   new->tiley = ob->tiley;
   new->x = ob->x;
   new->y = ob->y;

   new->obclass = fireobj;
   new->dir = nodir;
   new->angle = ob->angle;
   new->speed = 0x2000l;
   new->flags = FL_NEVERMARK;
   new->active = true;
   
   SetSpawner(new,ob);
   
}

/*
==========================
=
= T_RocketLauncherAttack
=
==========================
*/

void RocketLauncherAttack (objtype *ob)
{
   if (!gamestate.rockets)
       return;

   SD_PlaySound (MISSILEFIRESND);

   GetNewActor ();
   new->state = &s_rocket;
   new->ticcount = 1;

   new->tilex = ob->tilex;
   new->tiley = ob->tiley;
   new->x = ob->x;
   new->y = ob->y;

   new->obclass = rocketobj;
   new->dir = nodir;
   new->angle = ob->angle;
   new->speed = 0x4200l;
   new->flags = FL_NEVERMARK;
   new->active = true;
   
   SetSpawner(new,ob);
}

void	GunAttack (objtype *ob)
{
	objtype *check,*closest,*oldclosest;
	int      dmgmod; // damage modifier
	int		damage;
	int		dx,dy,dist;
	long	viewdist;

	switch (gamestate.weapon)
	{
	case wp_pistol:
		SD_PlaySound (ATKPISTOLSND);
		dmgmod = 70;
		break;
	case wp_machinegun:
		SD_PlaySound (ATKMACHINEGUNSND);
		dmgmod = 75;
		break;
	case wp_chaingun:
		SD_PlaySound (ATKGATLINGSND);
		dmgmod = 80;
		break;
    case wp_flamethrower:
        FlameThrowerAttack (ob);
        madenoise = true;
        return;
    case wp_rocketlauncher:
        RocketLauncherAttack (ob);
        madenoise = true;
        return;
	}

	madenoise = true;

//
// find potential targets
//
	viewdist = 0x7fffffffl;
	closest = NULL;

	while (1)
	{
		oldclosest = closest;

		for (check=ob->next ; check ; check=check->next)
			if ( (check->flags & FL_SHOOTABLE)
			&& (check->flags & FL_VISABLE)
			&& abs (check->viewx-centerx) < shootdelta
			)
			{
				if (check->transx < viewdist)
				{
					viewdist = check->transx;
					closest = check;
				}
			}

		if (closest == oldclosest)
			return;						// no more targets, all missed

	//
	// trace a line from player to enemey
	//
		if (CheckLine(closest))
			break;

	}

//
// hit something
//
	dx = abs(closest->tilex - player->tilex);
	dy = abs(closest->tiley - player->tiley);
	dist = dx>dy ? dx:dy;

	if (dist<2)
		damage = US_RndT() / 4;
	else if (dist<4)
		damage = US_RndT() / 6;
	else
	{
		if ( (US_RndT() / 12) < dist)		// missed
			return;
		damage = US_RndT() / 6;
	}

    if (quadmode != true)
        damage = damage * dmgmod / 100; // scale damage based on weapon modifier
    else
        damage = damage * dmgmod / 80;
        
	DamageActor (closest,damage);
}

//===========================================================================

/*
===============
=
= VictorySpin
=
===============
*/

#ifndef SPEAR

void VictorySpin (void)
{
	long	desty;

	if (player->angle > 270)
	{
		player->angle -= tics * 3;
		if (player->angle < 270)
			player->angle = 270;
	}
	else if (player->angle < 270)
	{
		player->angle += tics * 3;
		if (player->angle > 270)
			player->angle = 270;
	}

	desty = (((long)player->tiley-5)<<TILESHIFT)-0x3000;

	if (player->y > desty)
	{
		player->y -= tics*4096;
		if (player->y < desty)
			player->y = desty;
	}
}

#endif

//===========================================================================

/*
===============
=
= T_Attack
=
===============
*/

void	T_Attack (objtype *ob)
{
	struct	atkinf	*cur;
	int i;

	UpdateFace ();

    #ifndef SPEAR
	if (gamestate.victoryflag)		// watching the BJ actor
	{
		VictorySpin ();
		return;
	}
	#endif

	if ( buttonstate[bt_use] && !buttonheld[bt_use] )
		buttonstate[bt_use] = false;

	if ( buttonstate[bt_attack] && !buttonheld[bt_attack])
		buttonstate[bt_attack] = false;

	ControlMovement (ob);
	if (gamestate.victoryflag)		// watching the BJ actor
		return;

	plux = player->x >> UNSIGNEDSHIFT;			// scale to fit in unsigned
	pluy = player->y >> UNSIGNEDSHIFT;
	player->tilex = player->x >> TILESHIFT;		// scale to tile values
	player->tiley = player->y >> TILESHIFT;

//
// change frame and fire
//
	gamestate.attackcount -= tics;
	while (gamestate.attackcount <= 0)
	{
		cur = &attackinfo[gamestate.weapon][gamestate.attackframe];
		switch (cur->attack)
		{
		case -1:
			ob->state = &s_player;
			if (!CheckAmmo (gamestate.weapon) && gamestate.chosenweapon != wp_knife)
            {
                for(i=0; i<NUMWEAPONS; i++)
                {
                    if(CheckAmmo (i) && gamestate.bestweapon & (1<<i))
                    {
#ifdef WEAPONC
                        gamestate.changingweapon = true; 
                        gamestate.goingdown = true; 
                        gamestate.nextweapon = gamestate.chosenweapon = i;
#else
                        gamestate.weapon = i;
#endif
                    }
                }
                DrawWeapon ();
                DrawAmmo ();
            }
			else
			{
				if (gamestate.weapon != gamestate.chosenweapon)
				{
                    if(CheckAmmo(gamestate.chosenweapon))
                    {
#ifndef WEAPONC
                        gamestate.weapon = gamestate.chosenweapon;
#else
                        gamestate.changingweapon = gamestate.goingdown = true;
                        gamestate.nextweapon = gamestate.chosenweapon;
#endif
                    }
                    DrawWeapon ();
				}
			}
			gamestate.attackframe = gamestate.weaponframe = 0;
			return;
		case 4:
			if (!CheckAmmo(gamestate.weapon))
				break;
			if (buttonstate[bt_attack])
				gamestate.attackframe -= 2;
		case 1:
			if (!CheckAmmo(wp_chaingun) && gamestate.weapon == wp_chaingun)
			{	// can only happen with chain gun
				gamestate.attackframe++;
				break;
			}
			if (!CheckAmmo(wp_flamethrower) && gamestate.weapon == wp_flamethrower)
			{	// can only happen with flamethrower
				gamestate.attackframe++;
				break;
			}
            if (CheckAmmo(gamestate.weapon))
            {
                GunAttack (ob);
                switch (gamestate.weapon)
                {
                    case wp_pistol:
                    case wp_machinegun:
                    case wp_chaingun:
                        gamestate.ammo--;
                        break;
                    case wp_flamethrower:
                        gamestate.fuel--;
                        break;
                    case wp_rocketlauncher:
                        gamestate.rockets--;
                        break;
                }
                DrawAmmo ();
                break;
            }
            else
            {
                return;
            }
		case 2:
			KnifeAttack (ob);
			break;

		case 3:
			if (CheckAmmo(gamestate.weapon) && buttonstate[bt_attack])
				gamestate.attackframe -= 2;
			break;
		}
		gamestate.attackcount += cur->tics;
		gamestate.attackframe++;
		gamestate.weaponframe =
			attackinfo[gamestate.weapon][gamestate.attackframe].frame;
	}

}



//===========================================================================

/*
===============
=
= T_Player
=
===============
*/

void	T_Player (objtype *ob)
{
    
    #ifndef SPEAR
	if (gamestate.victoryflag)		// watching the BJ actor
	{
		VictorySpin ();
		return;
	}
	#endif

	UpdateFace ();
	CheckWeaponChange ();

	if ( buttonstate[bt_use] )
		Cmd_Use ();

	if ( buttonstate[bt_attack] && !buttonheld[bt_attack])
		Cmd_Fire ();

	ControlMovement (ob);
	if (gamestate.victoryflag)		// watching the BJ actor
		return;


	plux = player->x >> UNSIGNEDSHIFT;			// scale to fit in unsigned
	pluy = player->y >> UNSIGNEDSHIFT;
	player->tilex = player->x >> TILESHIFT;		// scale to tile values
	player->tiley = player->y >> TILESHIFT;
}


