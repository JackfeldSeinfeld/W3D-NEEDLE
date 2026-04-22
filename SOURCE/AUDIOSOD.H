/////////////////////////////////////////////////
//
// MUSE Header for .SOD
// Created Thu Aug 13 09:25:58 1992
//
/////////////////////////////////////////////////

#define NUMSOUNDS		LASTSOUND
#define NUMSNDCHUNKS	NUMSOUNDS*3+LASTMUSIC

//
// Sound names & indexes
//
typedef enum {
		HITWALLSND,              // 0
		MISSILEHITSND,           // 1
		BITEMESND,               // 2
		GHOSTSIGHTSND,           // 3
		MOVEGUN2SND,             // 4
		MOVEGUN1SND,             // 5
		NOWAYSND,                // 6
		NAZIHITPLAYERSND,        // 7
		MISSILEFIRESND,          // 8
		PLAYERDEATHSND,          // 9
		DOGDEATHSND,             // 10
		ATKGATLINGSND,           // 11
		GETKEYSND,               // 12
		HEARTBEATSND,            // 13
		WALK1SND,                // 14
		WALK2SND,                // 15
		TAKEDAMAGESND,           // 16
		GAMEOVERSND,             // 17
		OPENDOORSND,             // 18
		CLOSEDOORSND,            // 19
		DONOTHINGSND,            // 20
		HALTSND,                 // 21
		DEATHSCREAM2SND,         // 22
		ATKKNIFESND,             // 23
		ATKPISTOLSND,            // 24
		DEATHSCREAM3SND,         // 25
		ATKMACHINEGUNSND,        // 26
		CHANGEWEAPSND,           // 27
		SHOOTDOORSND,            // 28
		DEATHSCREAM1SND,         // 29
		GETMACHINESND,           // 30
		GETAMMOSND,              // 31
		SHOOTSND,                // 32
		HEALTH1SND,              // 33
		HEALTH2SND,              // 34
		BONUS1SND,               // 35
		BONUS2SND,               // 36
		BONUS3SND,               // 37
		GETGATLINGSND,           // 38
		ESCPRESSEDSND,           // 39
		LEVELDONESND,            // 40
		DOGBARKSND,              // 41
		ENDBONUS1SND,            // 42
		ENDBONUS2SND,            // 43
		BONUS1UPSND,             // 44
		BONUS4SND,               // 45
		PUSHWALLSND,             // 46
		NOBONUSSND,              // 47
		PERCENT100SND,           // 48
		BOSSACTIVESND,           // 49
		DEATHSCREAM4SND,         // 50
		SCHUTZADSND,             // 51
		AHHHGSND,                // 52
		DEATHSCREAM5SND,         // 53
		DEATHSCREAM7SND,         // 54
		DEATHSCREAM8SND,         // 55
		LEBENSND,                // 56
		DEATHSCREAM6SND,         // 57
		NAZIFIRESND,             // 58
		BOSSFIRESND,             // 59
		SSFIRESND,               // 60
		SLURPIESND,              // 61
		GHOSTFADESND,            // 62
		DEATHSCREAM9SND,         // 63
		GETAMMOBOXSND,           // 64
		ANGELSIGHTSND,           // 65
		SPIONSND,                // 66
		NEINSOVASSND,            // 67
		DOGATTACKSND,            // 68
		ANGELFIRESND,            // 69
		MANSSIGHTSND,            // 70
		MANSDEATHSND,            // 71
		WILHELMSIGHTSND,         // 72
		WILHELMDEATHSND,         // 73
		SAKURAIDEATHSND,         // 74
		KNIGHTSIGHTSND,          // 75
		KNIGHTDEATHSND,          // 76
		ANGELDEATHSND,           // 77
		KNIGHTMISSILESND,        // 78
		GETSPEARSND,             // 79
		ANGELTIREDSND,           // 80
		GETFLAMERSND,            // 81
		GETLAUNCHERSND,          // 82
		FLAMETHROWERSND,         // 83
		SATISFYINGSND,           // 84
		FLAMEHITSND,             // 85
		OPENWDOORSND,            // 86
		CLOSEWDOORSND,           // 87
		GETBACKPACKSND,          // 88
		SPEARFALLSND,            // 89
		GETPOWSND,               // 90
		BACHTUNGSND,             // 91
		SCHAISERSND,             // 92
		YEAHSND,                 // 93
		HEALTH0SND,              // 94
		SAKURAISIGHTSND,         // 95
		LASTSOUND
	     } soundnames;

//
// Base offsets
//
#define STARTPCSOUNDS		0
#define STARTADLIBSOUNDS	NUMSOUNDS
#define STARTDIGISOUNDS		NUMSOUNDS*2
#define STARTMUSIC		    NUMSOUNDS*3

//
// Music names & indexes
//
typedef enum
{
		XFUNKIE_MUS,             // 0
		DUNGEON_MUS,             // 1
		XDEATH_MUS,              // 2
		GETTHEM_MUS,             // 3
		XTIPTOE_MUS,             // 4
		GOINGAFT_MUS,            // 5
		URAHERO_MUS,             // 6
		XTHEEND_MUS,             // 7
		NAZI_OMI_MUS,            // 8
		POW_MUS,                 // 9
		TWELFTH_MUS,             // 10
		SEARCHN_MUS,             // 11
		SUSPENSE_MUS,            // 12
		ZEROHOUR_MUS,            // 13
		WONDERIN_MUS,            // 14
		ULTIMATE_MUS,            // 15
		ENDLEVEL_MUS,            // 16
		XEVIL_MUS,               // 17
		GREEDY_MUS,            // 18
		COPYPRO_MUS,             // 19
		XAWARD_MUS,              // 20
		XPUTIT_MUS,              // 21
		XGETYOU_MUS,             // 22
		XTOWER2_MUS,             // 23
		URDEAD_MUS,              // 24
		GRUNGE_MUS,              // 25
		BITEME_MUS,              // 26
		DANGER_MUS,              // 27
		SAKURAI_MUS,             // 28
		ENDOFDEMO_MUS,           // 29
		LASTMUSIC
} musicnames;

/////////////////////////////////////////////////
//
// Thanks for playing with MUSE!
//
/////////////////////////////////////////////////
