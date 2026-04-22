// WL_SHADE.C
//
// Based on code from Wolf4SDL by Moritz "Ripper" Kroll
//
// Uses fixed point arithmetic and checks for early returns
// to speed up and reduce the *** 2 million *** iterations
//

#include "WL_DEF.H"
#pragma hdrstop

#ifdef USE_SHADE

byte far shadetable[MAXSHADE][256];    // must be at an offset of 0 or evenly divisible by 256


/*
=======================
=
= GetBestColor
=
= Finds the palette index of the nearest matching color of the
= given RGB color
=
=======================
*/

int GetBestColor (int red, int green, int blue)
{
    int      bestcolor,color;
    fixed    bestdist,dist;
    fixed    destr,destg,destb;
    byte far *palette;

    bestdist = MAXLONG;
    bestcolor = 0;

    palette = &gamepal;

    for (color = 0; color < 256; color++, palette += 3)
    {
        destr = red - palette[0];
        destg = green - palette[1];
        destb = blue - palette[2];

        dist = (destr * destr) + (destg * destg) + (destb * destb);

        if (!dist)
            return color;       // perfect match!
        else if (dist < bestdist)
        {
            bestdist = dist;
            bestcolor = color;
        }
    }

    return bestcolor;
}

#ifdef SPEAR

void LevelShadeTableBuilder (int r, int g, int b)
{
    if (playstate != ex_died)
       BuildShadeTable(r,g,b);
}

#endif

/*
=======================
=
= BuildShadeTable
=
=======================
*/

void BuildShadeTable (int red, int green, int blue)
{
    int      i;
    int      color,newcolor;
    int      newred,newblue,newgreen;
    fixed    srcr,srcg,srcb;
    fixed    deltar,deltag,deltab;
    fixed    stepr,stepg,stepb;
    long     key,lastkey;
    byte far *palette;

    palette = &gamepal;

    for (color = 0; color < 256; color++, palette += 3)
    {
        lastkey = MAXLONG;

        srcr = (fixed)palette[0] << 16;
        srcg = (fixed)palette[1] << 16;
        srcb = (fixed)palette[2] << 16;

        deltar = ((fixed)red << 16) - srcr;
        deltag = ((fixed)green << 16) - srcg;
        deltab = ((fixed)blue << 16) - srcb;

        stepr = deltar / (fixed)(MAXSHADE + 8 );
        stepg = deltag / (fixed)(MAXSHADE + 8 );
        stepb = deltab / (fixed)(MAXSHADE + 8 );

        for (i = 0; i < MAXSHADE; i++)
        {
            newred = srcr >> 16;
            newgreen = srcg >> 16;
            newblue = srcb >> 16;

            key = ((long)newred << 16) | (newgreen << 8 ) | newblue;

            if (lastkey != key)
            {
                newcolor = GetBestColor(newred,newgreen,newblue);

                lastkey = key;
            }

            shadetable[ i ][color] = newcolor;

            srcr += stepr;
            srcg += stepg;
            srcb += stepb;
        }
    }
}


/*
=======================
=
= GetShade
=
=======================
*/

byte far *GetShade (int height)
{
    int shade;

    shade = (height >> 1) / (((viewwidth * 3) >> 8 ) + 1);

    if (shade > MAXSHADE)
        shade = MAXSHADE;
    else if (shade <= 0)
        shade = 1;

    return shadetable[MAXSHADE - shade];
}

#endif
