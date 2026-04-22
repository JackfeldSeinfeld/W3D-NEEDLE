// WL_DEBUG.C

#include "WL_DEF.H"
#pragma hdrstop
#include <BIOS.H>

/*
=============================================================================

						 LOCAL CONSTANTS

=============================================================================
*/

#define VIEWTILEX	(viewwidth/16)
#define VIEWTILEY	(viewheight/16)

/*
=============================================================================

						 GLOBAL VARIABLES

=============================================================================
*/


int DebugKeys (void);

/*
=============================================================================

						 LOCAL VARIABLES

=============================================================================
*/


byte	maporgx;
byte	maporgy;
enum {mapview,tilemapview,actoratview,visview}	viewtype;

void ViewMap (void);

//===========================================================================

/*
==================
=
= DebugMemory
=
==================
*/

void DebugMemory (void)
{
	int	i;
	char    scratch[80],str[10];
	long	mem;
	spritetype _seg	*block;

	CenterWindow (16,7);

	US_CPrint ("Memory Usage");
	US_CPrint ("------------");
	US_Print ("Total     :");
	US_PrintUnsigned (mminfo.mainmem/1024);
	US_Print ("k\nFree      :");
	US_PrintUnsigned (MM_UnusedMemory()/1024);
	US_Print ("k\nWith purge:");
	US_PrintUnsigned (MM_TotalFree()/1024);
	US_Print ("k\n");
	VH_UpdateScreen();
	IN_Ack ();
}

//===========================================================================

/*
================
=
= DebugKeys
=
================
*/

int DebugKeys (void)
{
	boolean esc;
	int level;

	if (Keyboard[sc_E])		// E = quit level
	{
		playstate = ex_completed;
	}
	if (Keyboard[sc_G])		// G = god mode
	{
		CenterWindow (12,2);
		if (godmode)
		  US_PrintCentered ("God mode OFF");
		else
		  US_PrintCentered ("God mode ON");
		VH_UpdateScreen();
		IN_Ack();
		godmode ^= 1;
		return 1;
	}
	if (Keyboard[sc_H])		// H = hurt self
	{
		IN_ClearKeysDown ();
		TakeDamage (16,NULL);
	}
	else if (Keyboard[sc_I])			// I = item cheat
	{
		CenterWindow (12,3);
		US_PrintCentered ("Free items!");
		VH_UpdateScreen();
		GivePoints (100000);
		HealSelf (99);
		GiveWeapon (wp_chaingun);
		GiveWeapon (wp_flamethrower);
		GiveWeapon (wp_rocketlauncher);
		gamestate.ammo = 150;
		gamestate.fuel = 200;
		gamestate.rockets = 100;
		DrawAmmo ();
		IN_Ack ();
		return 1;
	}
	else if (Keyboard[sc_M])			// M = memory info
	{
		DebugMemory();
		return 1;
	}
#ifdef SPEAR
	else if (Keyboard[sc_N])			// N = no clip
	{
		noclip^=1;
		CenterWindow (18,3);
		if (noclip)
			US_PrintCentered ("No clipping ON");
		else
			US_PrintCentered ("No clipping OFF");
		VH_UpdateScreen();
		IN_Ack ();
		return 1;
	}
#endif
	else if (Keyboard[sc_Q])			// Q = fast quit
		Quit (NULL);
	else if (Keyboard[sc_W])			// W = warp to level
	{
		CenterWindow(26,3);
		PrintY+=6;
#ifndef SPEAR
		US_Print("  Warp to which level(1-10):");
#else
#ifdef NEEDLE

#ifdef NEEDLEDEMO
		US_Print("  Warp to which level(1-7):");
#else
		US_Print("  Warp to which level(1-21):");
#endif //NEEDLEDEMO

#else
		US_Print("  Warp to which level(1-21):");
#endif
#endif
		VH_UpdateScreen();
		esc = !US_LineInput (px,py,str,NULL,true,2,0);
		if (!esc)
		{
			level = atoi (str);
#ifndef SPEAR
			if (level>0 && level<11)
#else
#ifdef NEEDLEDEMO
            #define MAXMAP 7
#else
            #define MAXMAP FINALMAP+1
#endif
			if (level>0 && level<=MAXMAP)
#endif
			{
#ifndef NEEDLEDEMO
				gamestate.mapon = level-1;
#else
				if (level < MAXMAP) gamestate.mapon = level-1;
				else if (level == MAXMAP) gamestate.mapon = SECRET3;
#endif
                playstate = ex_warped;
			}
		}
		return 1;
	}
	else if (Keyboard[sc_X])			// X = item cheat
	{
		CenterWindow (12,3);
		US_PrintCentered ("Extra stuff!");
		VH_UpdateScreen();
		IN_Ack ();
		return 1;
	}

	return 0;
}


#if 0
/*
===================
=
= OverheadRefresh
=
===================
*/

void OverheadRefresh (void)
{
	unsigned	x,y,endx,endy,sx,sy;
	unsigned	tile;


	endx = maporgx+VIEWTILEX;
	endy = maporgy+VIEWTILEY;

	for (y=maporgy;y<endy;y++)
		for (x=maporgx;x<endx;x++)
		{
			sx = (x-maporgx)*16;
			sy = (y-maporgy)*16;

			switch (viewtype)
			{
#if 0
			case mapview:
				tile = *(mapsegs[0]+farmapylookup[y]+x);
				break;

			case tilemapview:
				tile = tilemap[x][y];
				break;

			case visview:
				tile = spotvis[x][y];
				break;
#endif
			case actoratview:
				tile = (unsigned)actorat[x][y];
				break;
			}

			if (tile<MAXWALLTILES)
				LatchDrawTile(sx,sy,tile);
			else
			{
				LatchDrawChar(sx,sy,NUMBERCHARS+((tile&0xf000)>>12));
				LatchDrawChar(sx+8,sy,NUMBERCHARS+((tile&0x0f00)>>8));
				LatchDrawChar(sx,sy+8,NUMBERCHARS+((tile&0x00f0)>>4));
				LatchDrawChar(sx+8,sy+8,NUMBERCHARS+(tile&0x000f));
			}
		}

}
#endif

#if 0
/*
===================
=
= ViewMap
=
===================
*/

void ViewMap (void)
{
	boolean		button0held;

	viewtype = actoratview;
//	button0held = false;


	maporgx = player->tilex - VIEWTILEX/2;
	if (maporgx<0)
		maporgx = 0;
	if (maporgx>MAPSIZE-VIEWTILEX)
		maporgx=MAPSIZE-VIEWTILEX;
	maporgy = player->tiley - VIEWTILEY/2;
	if (maporgy<0)
		maporgy = 0;
	if (maporgy>MAPSIZE-VIEWTILEY)
		maporgy=MAPSIZE-VIEWTILEY;

	do
	{
//
// let user pan around
//
		PollControls ();
		if (controlx < 0 && maporgx>0)
			maporgx--;
		if (controlx > 0 && maporgx<mapwidth-VIEWTILEX)
			maporgx++;
		if (controly < 0 && maporgy>0)
			maporgy--;
		if (controly > 0 && maporgy<mapheight-VIEWTILEY)
			maporgy++;

#if 0
		if (c.button0 && !button0held)
		{
			button0held = true;
			viewtype++;
			if (viewtype>visview)
				viewtype = mapview;
		}
		if (!c.button0)
			button0held = false;
#endif

		OverheadRefresh ();

	} while (!Keyboard[sc_Escape]);

	IN_ClearKeysDown ();
}
#endif

