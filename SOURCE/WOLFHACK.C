// WOLFHACK.C

// Green Sleeves: 125

#include "WL_DEF.H"

#define	MAXVIEWHEIGHT	200

int         spanstart[MAXVIEWHEIGHT/2];

fixed       psin,pcos;
unsigned    mr_rowofs;
int         mr_count;
fixed       mr_xstep;
fixed       mr_ystep;
fixed       mr_xfrac;
fixed       mr_yfrac;
unsigned    mr_dest;
#ifdef USE_SHADE
unsigned    mr_shade;
#endif

#ifdef PLANE4SHIT

#define VERYDARK 1 // You can read the values of macros, idiot
#define QUITEDARK VERYDARK+1 // 2
// Skip Number 3 because it's supposed to be the default one :3
#define BRIGHT QUITEDARK+2 // 4
#define MY_EYES BRIGHT+1 // 5

/*
==============
=
= GetTileShade
=
= If a certain tile is at a certain position,
= very likely for it to be on the top left of the map...
= You'll be able to have different Shadings per map!
= Though, Sadly... that means no Bright tiles :<
=
= Made with NO AI SHITSTAINS and most importantly: made with Hopes and Dreams
= Programmed by: JackfeldSeinfeld, 2026
= Fixed by: KS-Presto @ DieHard Wolfers, 2026
=
==============
*/

void GetTileShade (int Uzi) // yes, this var is named after the character Uzi from Murder Drones (i really like MD btw)
{
     unsigned tile = MAPSPOT(0,0,2);

     if (gamestate.mapon != FINALMAP)
     {
         if (tile >= BRIGHT)
             tile = (tile + 4) >> 1;
         else if (tile < 3 && tile > 0)
             tile = (tile + 1) >> 1;
         else
             tile = 3;
     }
     else
         tile = (2 + 1) >> 1;

     mr_shade = FP_OFF(GetShade(Uzi << tile));
}

#endif

/*
==============
=
= DrawSpans
=
= Height ranges from 0 (infinity) to centery (nearest)
=
==============
*/

void DrawSpans (int x1, int x2, int height)
{
    fixed    basedist;
    int      prestep;
    fixed    xstep,ystep;
    int      startx,plane,startplane;
    unsigned toprow;

    toprow = bufferofs + ylookup[centery - 1 - height];
    mr_rowofs = ylookup[(height << 1) + 1];
#ifdef USE_SHADE
    if (shadetoggle)
#ifndef PLANE4SHIT
       mr_shade = FP_OFF(GetShade(height << 3));
#else
       GetTileShade (height);
#endif
    else
       mr_shade = FP_OFF(GetShade(255));
#endif

    height++;

    xstep = (psin >> 1) / height;
    ystep = (pcos >> 1) / height;
    basedist = (scale << 15) / height;

    prestep = centerx + 1 - x1;

    mr_xfrac = (viewx + FixedByFrac(basedist,viewcos)) - (xstep * prestep);
    mr_yfrac = (viewy - FixedByFrac(basedist,viewsin)) - (ystep * prestep);

    mr_xstep = xstep << 2;
    mr_ystep = ystep << 2;

//
// draw two spans simultaneously
//
    plane = startplane = x1 & 3;

    do
    {
        outportb (SC_INDEX + 1,1 << plane);

        startx = x1 >> 2;
        mr_dest = toprow + startx;
        mr_count = ((x2 - plane) >> 2) - startx + 1;
        x1++;

        if (mr_count)
            MapRow ();

        mr_xfrac += xstep;
        mr_yfrac += ystep;

        plane = (plane + 1) & 3;

    } while (plane != startplane);
}


/*
===================
=
= DrawPlanes
=
===================
*/

void DrawPlanes (void)
{
    int height,lastheight;
    int x;

    psin = viewsin;
    pcos = viewcos;

    if (psin < 0)
        psin = -(psin & 0xffff);

    if (pcos < 0)
        pcos = -(pcos & 0xffff);

//
// loop over all columns
//
    lastheight = centery;

    for (x =0; x < viewwidth; x++)
    {
        height = wallheight[x] >> 3;

        if (height < lastheight)
        {
            //
            // more starts
            //
            do
            {
                spanstart[--lastheight] = x;
            } while (lastheight > height);
        }
        else if (height > lastheight)
        {
            //
            // draw clipped spans
            //
            if (height > centery)
                height = centery;

            while (lastheight < height)
            {
                DrawSpans (spanstart[lastheight],x - 1,lastheight);

                lastheight++;
            }
        }
    }

    height = centery;

    while (lastheight < height)
    {
        DrawSpans (spanstart[lastheight],x - 1,lastheight);

        lastheight++;
    }
}



