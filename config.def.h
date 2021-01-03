/* See LICENSE file for copyright and license details. */

/* functions */
static void modmove(const Arg *arg);

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char statussep         = ';';      /* separator between status bars */
static const int barheight          = 25;       /* height of a bar; 0 equals autocalculation */
static const int attachdirection    = 0;        /* 0 default, 1 above, 2 aside, 3 below, 4 bottom, 5 top */
static const char *fonts[]          = { "monospace:size=8", "JoyPixels:pixelsize=10:antialias=true:autohint=true" };
static const char dmenufont[]       = "monospace:size=8";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#aaaaaa";
static const char col_gray4[]       = "#ffffff";
static const char col_acc1[]        = "#a46600";
static const char col_acc2[]        = "#d79921";

/* static const char col_stat0[]       = "#000000"; */
/* static const char col_stat1[]       = "#cc241d"; */
/* static const char col_stat2[]       = "#98971a"; */
/* static const char col_stat3[]       = "#d79921"; */
/* static const char col_stat4[]       = "#458588"; */
/* static const char col_stat5[]       = "#b16286"; */
/* static const char col_stat6[]       = "#689d6a"; */
/* static const char col_stat7[]       = "#a89984"; */

/* static const char col_stat0[]       = "#928374"; */
/* static const char col_stat1[]       = "#fb4934"; */
/* static const char col_stat2[]       = "#b8bb26"; */
/* static const char col_stat3[]       = "#fabd2f"; */
/* static const char col_stat4[]       = "#83a598"; */
/* static const char col_stat5[]       = "#d3869b"; */
/* static const char col_stat6[]       = "#8ec07c"; */
/* static const char col_stat7[]       = "#ebdbb2"; */

/* gruvbox */
static const char col_stat0[]       = "#000000";
static const char col_stat1[]       = "#fb4934";
static const char col_stat2[]       = "#98971a";
static const char col_stat3[]       = "#d79921";
static const char col_stat4[]       = "#458588";
static const char col_stat5[]       = "#b16286";
static const char col_stat6[]       = "#689d6a";
static const char col_stat7[]       = "#a89984";

static const char *colors[][3]      = {
	/*                       fg         bg         border   */
	[SchemeNorm]         = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]          = { col_gray4, col_acc2,  col_acc2  },
	[SchemeStatus]       = { col_gray3, col_gray1, "#000000" }, /* Status              */
	[SchemeTagsNorm]     = { col_gray3, col_gray1, "#000000" }, /* Tagbar l unselected */
	[SchemeTagsSel]      = { col_gray4, col_gray2, "#000000" }, /* Tagbar l selected   */
	[SchemeTagLnNorm]    = { col_gray2, col_gray2, "#000000" }, /* Top Line normal     */
	[SchemeTagLnOcc]     = { col_acc1,  col_acc1,  "#000000" }, /* Top Line occupied   */
	[SchemeTagLnSel]     = { col_acc2,  col_acc2,  "#000000" }, /* Top Line selected   */
	[SchemeInfoNorm]     = { col_gray3, col_gray1, "#000000" }, /* Infbar m unselected */
	[SchemeInfoSel]      = { col_gray3, col_gray1, "#000000" }, /* Infbar m selected   */

	/* Terminal colors, see drw.c for customizing @term */
	[SchemeTermNorm]     = { col_gray3, col_gray1, "@term"   },
	[SchemeTermSel]      = { col_gray4, col_acc2,  col_acc2  },

	/* Statusbar colors */
	[StatusLn]           = { col_gray2, col_gray1, "#000000" },
	[StatusBlack]        = { col_stat0, col_gray1, "#000000" },
	[StatusRed]          = { col_stat1, col_gray1, "#000000" },
	[StatusGreen]        = { col_stat2, col_gray1, "#000000" },
	[StatusYellow]       = { col_stat3, col_gray1, "#000000" },
	[StatusBlue]         = { col_stat4, col_gray1, "#000000" },
	[StatusMagenta]      = { col_stat5, col_gray1, "#000000" },
	[StatusCyan]         = { col_stat6, col_gray1, "#000000" },
	[StatusWhite]        = { col_stat7, col_gray1, "#000000" },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class              instance    title       tags mask     isfloating   isterminal   noswallow    monitor */
	{ "st",               NULL,       NULL,       0 << 0,       0,           1,           0,           -1 },
	{ "St",               NULL,       NULL,       0 << 0,       0,           1,           0,           -1 },
	{ "Gimp",             NULL,       NULL,       1 << 8,       0,           0,           0,           -1 },
	{ "discord",          NULL,       NULL,       1 << 7,       0,           1,           0,           -1 },
	{ "st-256color",      NULL,       NULL,       0 << 0,       0,           1,           0,           -1 },
	{ "Pulseeffects",     NULL,       NULL,       1 << 8,       0,           0,           0,            1 },
	{ "TelegramDesktop",  NULL,       NULL,       1 << 6,       0,           0,           0,           -1 },
	{ NULL,               NULL,   "Event Tester", 0,            0,           0,           1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55;     /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;        /* number of clients in master area */
static const int resizehints = 0;        /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "[M]",      monocle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
};

static const Direction directions[] = {
	/* symbol     arrange function */
	{ "replace",  attach },    /* first entry is default */
	{ "aside",    attachaside },
	{ "-^-",      attachabove },
	{ "-v-",      attachbelow },
	{ "vvv",      attachbottom },
	{ "^^^",      attachtop },
	/* { "left",        attach },    /1* first entry is default *1/ */
	/* { "up",          attachabove }, */
	/* { "right",       attachaside }, */
	/* { "down",        attachbelow }, */
	/* { "bottom",      attachbottom }, */
	/* { "top",         attachtop }, */
};

/* key definitions */
#define MODKEY Mod4Mask /* WinKey; Change to Mod1Mask if you want AltKey as Modifier */
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask|Mod1Mask,    KEY,      swaptags,       {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", /* "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_acc, "-sf", col_gray4, */ NULL };
static const char *termcmd[]  = { "st", NULL };

#include "movestack.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ShiftMask,             XK_F1,     quit,           {1} }, /* restart */
	{ MODKEY|ControlMask|ShiftMask, XK_F1,     quit,           {0} }, /* quit */
	{ MODKEY,                       XK_F4,     spawn,          SHCMD("st -e pulsemixer; kill -44 $(pidof dwmblocks)") },

	{ MODKEY,                       XK_F5,     setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_F5,     setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_F6,     setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_F6,     setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_F7,     setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ShiftMask,             XK_F7,     setlayout,      {.v = &layouts[5]} },

	{ MODKEY,                       XK_F9,     spawn,          SHCMD("dmenumount") },
	{ MODKEY,                       XK_F10,    spawn,          SHCMD("dmenuumount") },
	{ MODKEY,                       XK_F11,    spawn,          SHCMD("mpv --no-cache --no-osc --no-input-default-bindings --input-conf=/dev/null --title=webcam $(ls /dev/video[0,2,4,6,8] | tail -n 1)") },
	{ MODKEY,                       XK_F12,    spawn,          SHCMD("sudo notifytog") },

	{ MODKEY,                       XK_grave,  spawn,          SHCMD("dmenuunicode") },
	{ MODKEY,                       XK_minus,  spawn,          SHCMD("pamixer --allow-boost -d 5; kill -44 $(pidof dwmblocks)") },
	{ MODKEY|ShiftMask,             XK_minus,  spawn,          SHCMD("pamixer --allow-boost -d 15; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,                       XK_equal,  spawn,          SHCMD("pamixer --allow-boost -i 5; kill -44 $(pidof dwmblocks)") },
	{ MODKEY|ShiftMask,             XK_equal,  spawn,          SHCMD("pamixer --allow-boost -i 15; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,                       XK_BackSpace, spawn,       SHCMD("sysact") },
	{ MODKEY|ShiftMask,             XK_BackSpace, spawn,       SHCMD("sysact") },

	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_w,      spawn,          SHCMD("$BROWSER") },
	{ MODKEY|ShiftMask,             XK_w,      spawn,          SHCMD("st -e sudo nmtui") },
	{ MODKEY,                       XK_e,      spawn,          SHCMD("st -e neomutt ; pkill -RTMIN+12 dwmblocks; rmdir ~/.abook") },
	{ MODKEY|ShiftMask,             XK_e,      spawn,          SHCMD("st -e abook -C ~/.config/abook/abookrc --datafile ~/.config/abook/addressbook") },
	{ MODKEY,                       XK_r,      reorganizetags, {0} },
	{ MODKEY|ShiftMask,             XK_r,      setcfact,       {.f =  0.00} },

	{ MODKEY,                       XK_o,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_o,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_p,      spawn,          SHCMD("mpc toggle") },
	{ MODKEY|ShiftMask,             XK_p,      spawn,          SHCMD("mpc pause ; pauseallmpv") },
	{ MODKEY,                       XK_bracketleft, spawn,     SHCMD("mpc seek -10") },
	{ MODKEY|ShiftMask,             XK_bracketleft, spawn,     SHCMD("mpc seek -60") },
	{ MODKEY,                       XK_bracketright, spawn,    SHCMD("mpc seek +10") },
	{ MODKEY|ShiftMask,             XK_bracketright, spawn,    SHCMD("mpc seek +60") },

	{ MODKEY,                       XK_a,      spawn,          SHCMD("st -e lf") },
	{ MODKEY|ShiftMask,             XK_a,      spawn,          SHCMD("st -e htop") },
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_d,      spawn,          SHCMD("dmenize") },
	{ MODKEY,                       XK_f,      togglefullscr,  {0} },

	/* bits: [hk/jl], [jk/hl], [withControl] */

	{ MODKEY|ShiftMask,             XK_j,      modmove,        {.i = (0 + (0 << 1) + (0 << 2))} },
	{ MODKEY|ShiftMask,             XK_k,      modmove,        {.i = (0 + (0 << 1) + (1 << 2))} },
	{ MODKEY|ShiftMask,             XK_h,      modmove,        {.i = (0 + (1 << 1) + (0 << 2))} },
	{ MODKEY|ShiftMask,             XK_l,      modmove,        {.i = (0 + (1 << 1) + (1 << 2))} },
	{ MODKEY|ControlMask,           XK_j,      modmove,        {.i = (1 + (0 << 1) + (0 << 2))} },
	{ MODKEY|ControlMask,           XK_k,      modmove,        {.i = (1 + (0 << 1) + (1 << 2))} },
	{ MODKEY|ControlMask,           XK_h,      modmove,        {.i = (1 + (1 << 1) + (0 << 2))} },
	{ MODKEY|ControlMask,           XK_l,      modmove,        {.i = (1 + (1 << 1) + (1 << 2))} },

	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|Mod1Mask,              XK_j,      focusmon,       {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_k,      focusmon,       {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_j,      tagmon,         {.i = -1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_k,      tagmon,         {.i = +1 } },

	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },

	{ MODKEY,                       XK_z,      setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_z,      resetlayout,    {0} },
	{ MODKEY,                       XK_x,      setattach,      {0} },
	{ MODKEY|ShiftMask,             XK_x,      setattach,      {.i = -1} },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY|ShiftMask,             XK_b,      spawn,          SHCMD("setbg") },
	{ MODKEY,                       XK_n,      spawn,          SHCMD("st -e newsboat; pkill -RTMIN+6 dwmblocks") },
	{ MODKEY|ShiftMask,             XK_n,      spawn,          SHCMD("st -e nvim -c VimwikiIndex") },
	{ MODKEY,                       XK_m,      spawn,          SHCMD("st -e ncmpcpp") },
	{ MODKEY|ShiftMask,             XK_m,      spawn,          SHCMD("pamixer -t; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,                       XK_comma,  spawn,          SHCMD("mpc prev") },
	{ MODKEY|ShiftMask,             XK_comma,  spawn,          SHCMD("mpc seek 0%") },
	{ MODKEY,                       XK_period, spawn,          SHCMD("mpc next") },
	{ MODKEY|ShiftMask,             XK_period, spawn,          SHCMD("mpc repeat") },

	{ MODKEY,                       XK_space,  zoom,           {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },

	{ 0,                            XK_Print,  spawn,          SHCMD("maim /usr/kocotian/pix/screen/pic-full-$(date '+%y%m%d-%H%M-%S').png") },
	{ ShiftMask,                    XK_Print,  spawn,          SHCMD("maimpick") },
	{ MODKEY,                       XK_Print,  spawn,          SHCMD("dmenurecord") },
	{ MODKEY|ShiftMask,             XK_Print,  spawn,          SHCMD("dmenurecord kill") },
	{ MODKEY,                       XK_Delete, spawn,          SHCMD("dmenurecord kill") },
	{ MODKEY,                       XK_Scroll_Lock, spawn,     SHCMD("killall screenkey || screenkey &") },

	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY|ShiftMask, Button1,      movemouse,      {.i = 1} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,         MODKEY|ShiftMask, Button3,      resizemouse,    {.i = 1} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkRootWin,           0,              Button1,        spawn,          SHCMD("st") },
	{ ClkRootWin,           MODKEY,         Button1,        spawn,          { .v = dmenucmd} },
	{ ClkRootWin,           MODKEY,         Button3,        spawn,          SHCMD("dmenize") },
};

static void modmove(const Arg *arg)
{
	int f;
	f = selmon->sel->isfloating;
	Arg a;

	/* bits: [hk/jl], [jk/hl], [withControl] */
	if ((arg->i >> 1) & 1) { /* hl */
		if ((arg->i >> 2) & 1) { /* l */
			if (f) a.v = (arg->i & 1 ? "0x 0y 25w 0h" : "25x 0y 0w 0h"), moveresize(&a);
			else a.f = +0.25, setcfact(&a);
		} else { /* h */
			if (f) a.v = (arg->i & 1 ? "0x 0y -25w 0h" : "-25x 0y 0w 0h"), moveresize(&a);
			else a.f = -0.25, setcfact(&a);
		}
	} else { /* jk */
		if ((arg->i >> 2) & 1) { /* k */
			if (f) a.v = (arg->i & 1 ? "0x 0y 0w -25h" : "0x -25y 0w 0h"), moveresize(&a);
			else a.f = -1, movestack(&a);
		} else { /* j */
			if (f) a.v = (arg->i & 1 ? "0x 0y 0w 25h" : "0x 25y 0w 0h"), moveresize(&a);
			else a.f = +1, movestack(&a);
		}
	}
}
