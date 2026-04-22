//////////////////////////////////////
//
// Graphics .H file for .SOD
// IGRAB-ed on Thu Oct 08 20:38:29 1992
//
//////////////////////////////////////

typedef enum {
		// Lump Start
		C_BACKDROPPIC=3,
		C_BACKDROPPIC2,
		C_MOUSELBACKPIC,                     // 4
		C_CURSOR1PIC,                        // 5
		C_CURSOR2PIC,                        // 6
		C_NOTSELECTEDPIC,                    // 7
		C_SELECTEDPIC,                       // 8
		// Lump Start
		H_LOGOPIC,
		H_NOWHEREISLESPIC,
		H_WEAPONSPIC,
		H_ENEMIESPIC,
		H_TOPWINDOWPIC,                      // 6
		H_LEFTWINDOWPIC,                     // 7
		H_RIGHTWINDOWPIC,                    // 8
		H_BOTTOMINFOPIC,                     // 9
		// Lump Start
		C_CUSTOMIZEPIC,                      // 9
		C_JOY1PIC,                           // 10
		C_JOY2PIC,                           // 11
		C_MOUSEPIC,                          // 12
		C_JOYSTICKPIC,                       // 13
		C_KEYBOARDPIC,                       // 14
		C_CONTROLPIC,                        // 15
		// Lump Start
		C_OPTIONSPIC,                        // 16
		// Lump Start
		C_FXTITLEPIC,                        // 17
		C_DIGITITLEPIC,                      // 18
		C_MUSICTITLEPIC,                     // 19
		// Lump Start
		C_HOWTOUGHPIC,                       // 20
		C_BABYMODEPIC,                       // 21
		C_EASYPIC,                           // 22
		C_NORMALPIC,                         // 23
		C_HARDPIC,                           // 24
		C_EXTREMEPIC,
		// Lump Start
		C_DISKLOADING1PIC,                   // 25
		C_DISKLOADING2PIC,                   // 26
		C_LOADGAMEPIC,                       // 27
		C_SAVEGAMEPIC,                       // 28
		// Lump Start
		HIGHSCORESPIC,                       // 29
		C_WONSPEARPIC,                       // 30
		// Lump Start
		BJCOLLAPSE1PIC,                      // 31
		BJCOLLAPSE2PIC,                      // 32
		BJCOLLAPSE3PIC,                      // 33
		BJCOLLAPSE4PIC,                      // 34
		ENDPICPIC,                           // 35
		// Lump Start
		L_GUYPIC,                            // 36
		L_COLONPIC,                          // 37
		L_NUM0PIC,                           // 38
		L_NUM1PIC,                           // 39
		L_NUM2PIC,                           // 40
		L_NUM3PIC,                           // 41
		L_NUM4PIC,                           // 42
		L_NUM5PIC,                           // 43
		L_NUM6PIC,                           // 44
		L_NUM7PIC,                           // 45
		L_NUM8PIC,                           // 46
		L_NUM9PIC,                           // 47
		L_PERCENTPIC,                        // 48
		L_APIC,                              // 49
		L_BPIC,                              // 50
		L_CPIC,                              // 51
		L_DPIC,                              // 52
		L_EPIC,                              // 53
		L_FPIC,                              // 54
		L_GPIC,                              // 55
		L_HPIC,                              // 56
		L_IPIC,                              // 57
		L_JPIC,                              // 58
		L_KPIC,                              // 59
		L_LPIC,                              // 60
		L_MPIC,                              // 61
		L_NPIC,                              // 62
		L_OPIC,                              // 63
		L_PPIC,                              // 64
		L_QPIC,                              // 65
		L_RPIC,                              // 66
		L_SPIC,                              // 67
		L_TPIC,                              // 68
		L_UPIC,                              // 69
		L_VPIC,                              // 70
		L_WPIC,                              // 71
		L_XPIC,                              // 72
		L_YPIC,                              // 73
		L_ZPIC,                              // 74
		L_EXPOINTPIC,                        // 75
		L_APOSTROPHEPIC,                     // 76
		L_GUY2PIC,                           // 77
		L_BJWINSPIC,                         // 78
		// Lump Start
		TITLE1PIC,                           // 79
		TITLE2PIC,                           // 80
		// Lump Start
		ENDSCREEN11PIC,                      // 81
		// Lump Start
		ENDSCREEN12PIC,                      // 82
		ENDSCREEN3PIC,                       // 83
		ENDSCREEN4PIC,                       // 84
		ENDSCREEN5PIC,                       // 85
		ENDSCREEN6PIC,                       // 86
		ENDSCREEN7PIC,                       // 87
		ENDSCREEN8PIC,                       // 88
		ENDSCREEN9PIC,                       // 89
		STATUSBARPIC,                        // 90
		PG13PIC,                             // 91
		CREDITSPIC1,                          // 92
		CREDITSPIC2,
		// Lump Start
		IDGUYS1PIC,                          // 93
		IDGUYS2PIC,                          // 94
		// Lump Start
		SIGNONPIC,                          // 93
		// Lump Start
		COPYPROTTOPPIC,                      // 95
		COPYPROTBOXPIC,                      // 96
		BOSSPIC1PIC,                         // 97
		BOSSPIC2PIC,                         // 98
		BOSSPIC3PIC,                         // 99
		BOSSPIC4PIC,                         // 100
		// Lump Start
		KNIFEPIC,                            // 101
		GUNPIC,                              // 102
		MACHINEGUNPIC,                       // 103
		GATLINGGUNPIC,                       // 104
		FLAMETHROWERPIC,                     // 105
		ROCKETLAUNCHERPIC,                   // 106
		NOKEYPIC,                            // 107
		GOLDKEYPIC,                          // 108
		SILVERKEYPIC,                        // 109
		N_BLANKPIC,                          // 110
		N_0PIC,                              // 111
		N_1PIC,                              // 112
		N_2PIC,                              // 113
		N_3PIC,                              // 114
		N_4PIC,                              // 115
		N_5PIC,                              // 116
		N_6PIC,                              // 117
		N_7PIC,                              // 118
		N_8PIC,                              // 119
		N_9PIC,                              // 120
		FACE1APIC,                           // 121
		FACE1BPIC,                           // 122
		FACE1CPIC,                           // 123
		FACE2APIC,                           // 124
		FACE2BPIC,                           // 125
		FACE2CPIC,                           // 126
		FACE3APIC,                           // 127
		FACE3BPIC,                           // 128
		FACE3CPIC,                           // 129
		FACE4APIC,                           // 130
		FACE4BPIC,                           // 131
		FACE4CPIC,                           // 132
		FACE5APIC,                           // 133
		FACE5BPIC,                           // 134
		FACE5CPIC,                           // 135
		FACE6APIC,                           // 136
		FACE6BPIC,                           // 137
		FACE6CPIC,                           // 138
		FACE7APIC,                           // 139
		FACE7BPIC,                           // 140
		FACE7CPIC,                           // 141
		FACE8APIC,                           // 142
		GOTGATLINGPIC,                       // 143
		GODMODEFACE1PIC,                     // 144
		GODMODEFACE2PIC,                     // 145
		GODMODEFACE3PIC,                     // 146
		BJWAITING1PIC,                       // 147
		BJWAITING2PIC,                       // 148
		BJOUCHPIC,                           // 149
		PAUSEDPIC,                           // 150
		GETPSYCHEDPIC,                       // 151

        TILE8,

		ORDERSCREEN,
		ERRORSCREEN,                         // 152
		TITLEPALETTE,                        // 153
		END1PALETTE,                         // 154
		END2PALETTE,                         // 155
		END3PALETTE,                         // 156
		END4PALETTE,                         // 157
		END5PALETTE,                         // 158
		END6PALETTE,                         // 159
		END7PALETTE,                         // 160
		END8PALETTE,                         // 161
		END9PALETTE,                         // 162
		IDGUYSPALETTE,                       // 163
		T_DEMO0,                             // 164
		T_DEMO1,                             // 165
		T_DEMO2,                             // 166
		T_DEMO3,                             // 167
		T_ENDART1,                           // 168
		ENUMEND
	     } graphicnums;

//
// Data LUMPs
//

#define BACKDROP_LUMP_START		C_BACKDROPPIC
#define BACKDROP_LUMP_END		C_SELECTEDPIC

#define README_LUMP_START		H_LOGO
#define README_LUMP_END			H_BOTTOMINFOPIC

#define CONTROL_LUMP_START      C_CUSTOMIZEPIC
#define CONTROL_LUMP_END        C_CONTROLPIC

#define OPTIONS_LUMP_START		C_OPTIONSPIC
#define OPTIONS_LUMP_END		C_OPTIONSPIC

#define SOUND_LUMP_START		C_FXTITLEPIC
#define SOUND_LUMP_END			C_MUSICTITLEPIC

#define NEWGAME_LUMP_START		C_HOWTOUGHPIC
#define NEWGAME_LUMP_END		C_EXTREMEPIC

#define LOADSAVE_LUMP_START		C_DISKLOADING1PIC
#define LOADSAVE_LUMP_END		C_SAVEGAMEPIC

#define HIGHSCORES_LUMP_START	HIGHSCORESPIC
#define HIGHSCORES_LUMP_END		C_WONSPEARPIC

#define ENDGAME_LUMP_START		BJCOLLAPSE1PIC
#define ENDGAME_LUMP_END		ENDPICPIC

#define LEVELEND_LUMP_START     L_GUYPIC
#define LEVELEND_LUMP_END       L_BJWINSPIC

#define TITLESCREEN_LUMP_START		TITLE1PIC
#define TITLESCREEN_LUMP_END		TITLE2PIC

#define ENDGAME1_LUMP_START		ENDSCREEN11PIC
#define ENDGAME1_LUMP_END		ENDSCREEN11PIC

#define ENDGAME2_LUMP_START		ENDSCREEN12PIC
#define ENDGAME2_LUMP_END		ENDSCREEN12PIC

#define EASTEREGG_LUMP_START	IDGUYS1PIC
#define EASTEREGG_LUMP_END		IDGUYS2PIC

#define SIGNON_LUMP_START		SIGNONPIC
#define SIGNON_LUMP_END			SIGNONPIC

#define COPYPROT_LUMP_START		COPYPROTTOPPIC
#define COPYPROT_LUMP_END		BOSSPIC4PIC

#define LATCHPICS_LUMP_START    KNIFEPIC
#define LATCHPICS_LUMP_END      GETPSYCHEDPIC


//
// Amount of each data item
//
#define NUMCHUNKS    ENUMEND
#define NUMFONT      2
#define NUMFONTM     0
#define NUMPICS      ((TILE8 - 1) - NUMFONT)
#define NUMPICM      0
#define NUMSPRITES   0
#define NUMTILE8     72
#define NUMTILE8M    0
#define NUMTILE16    0
#define NUMTILE16M   0
#define NUMTILE32    0
#define NUMTILE32M   0
#define NUMEXTERNS   13
//
// File offsets for data items
//
#define STRUCTPIC    0

#define STARTFONT    1
#define STARTFONTM   3
#define STARTPICS    3
#define STARTPICM    TILE8
#define STARTSPRITES TILE8
#define STARTTILE8   TILE8
#define STARTTILE8M  ORDERSCREEN
#define STARTTILE16  ORDERSCREEN
#define STARTTILE16M ORDERSCREEN
#define STARTTILE32  ORDERSCREEN
#define STARTTILE32M ORDERSCREEN
#define STARTEXTERNS ORDERSCREEN

//
// Thank you for using IGRAB!
//
