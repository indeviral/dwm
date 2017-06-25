#include <X11/XF86keysym.h>

/* appearance */
#define NUMCOLORS 9
static const char colors[NUMCOLORS][MAXCOLORS][9] = {
  // border     foreground background
  { "#1A1A1A", "#808080", "#111111" },  // 1 = normal
  { "#005577", "#FFFFFF", "#111111" },  // 2 = selected
  { "#B3B3B3", "#BABDB6", "#111111" },  // 3 = white
  { "#93d44f", "#93d44f", "#111111" },  // 4 = green
  { "#111111", "#F32626", "#111111" },  // 5 = urgent/warning
  { "#F32626", "#FFFFFF", "#F32626" },  // 6 = error
  { "#FFC123", "#ffc123", "#111111" },  // 7 = yelow
  { "#89b6e2", "#89b6e2", "#111111" },  // 8 = blue
  { "#89b6e2", "#89b6e2", "#111111" },  // 8 = blue
};

static const char *fonts[] = { "terminus:size=8" };
static const char dmenufont[] = "terminus:size=8";

static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */

/* tagging */
static const char *tags[] = { "1:main","2:term","3:web","4:null","5:null" };

static const Rule rules[] = {
/* class      instance    title       tags mask     isfloating   monitor */
  { NULL,       NULL,       "sxiv",     0,            True,        -1 },
  { "mpv",      NULL,       NULL,       ~0,           True,        -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ ".t",      tile },    /* first entry is default */
	{ ".f",      NULL },    /* no layout function means floating behavior */
	{ ".m",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/bash", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", colors[0][2], "-nf", colors[0][1], "-sb", colors[0][2], "-sf", colors[1][1], NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *apluscmd[] = { "ponymix", "increase", "5", NULL };
static const char *aminuscmd[] = { "ponymix", "decrease", "5", NULL };
static const char *bpluscmd[] = { "xbacklight", "-inc", "10", NULL };
static const char *bminuscmd[] = { "xbacklight", "-dec", "10", NULL };
static const char *translate[] = { "translate", "-t", NULL };
static const char *musplay[] = { "mpc", "toggle", NULL };
static const char *musstop[] = { "mpc", "stop", NULL };
static const char *musnext[] = { "mpc", "next", NULL };
static const char *musprev[] = { "mpc", "prev", NULL };
static const char *printscreen[] = { "scrot", NULL };
static const char *dwmclip[] = { "dmenu_clip","-i","-m", dmenumon, "-fn", dmenufont, "-nb", colors[0][2], "-nf", colors[0][1], "-sb", colors[0][2], "-sf", colors[1][1],"-l","10", NULL };

static Key keys[] = {
  /* modifier                     key        function        argument */
  { MODKEY,                       XK_q,      spawn,          {.v = dmenucmd } },
  { MODKEY,                       XK_x,      spawn,          {.v = termcmd} },
  { MODKEY,                       XK_b,      togglebar,      {0} },
  { MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
  { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
  { MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
  { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
  { MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
  { MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
  { MODKEY,                       XK_Return, zoom,           {0} },
  { MODKEY,                       XK_Tab,    view,           {0} },
  { MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
  { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
  { MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
  { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
  { MODKEY,                       XK_space,  setlayout,      {0} },
  { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
  { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
  { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
  { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
  { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
  { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
  { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
  TAGKEYS(                        XK_1,                      0)
  TAGKEYS(                        XK_2,                      1)
  TAGKEYS(                        XK_3,                      2)
  TAGKEYS(                        XK_4,                      3)
  TAGKEYS(                        XK_5,                      4)
  TAGKEYS(                        XK_6,                      5)
  TAGKEYS(                        XK_7,                      6)
  TAGKEYS(                        XK_8,                      7)
  TAGKEYS(                        XK_9,                      8)
  { MODKEY|ShiftMask,            0xff57,      quit,           {0} },
  { 0,                       0x1008ff13,     spawn,         {.v = apluscmd} },
  { 0,                       0x1008ff11,     spawn,         {.v = aminuscmd } },
  { 0,                       0x1008ff02,     spawn,         {.v = bpluscmd } },
  { 0,                       0x1008ff03,     spawn,         {.v = bminuscmd } },
  { MODKEY,                       XK_r,      spawn,         {.v = translate } },
  { 0,                       0x1008ff14,     spawn,         {.v = musplay } },
  { 0,                       0x1008ff15,     spawn,         {.v = musstop } },
  { 0,                       0x1008ff16,     spawn,         {.v = musprev } },
  { 0,                       0x1008ff17,     spawn,         {.v = musnext } },
  { 0,                           0xff61,     spawn,         {.v = printscreen } },
  { MODKEY,                        XK_v,     spawn,         {.v = dwmclip}},
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
  /* click                event mask      button          function        argument */
  { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
  { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
  { ClkWinTitle,          0,              Button2,        zoom,           {0} },
  { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
  { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
  { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
  { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
  { ClkTagBar,            0,              Button1,        view,           {0} },
  { ClkTagBar,            0,              Button3,        toggleview,     {0} },
  { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
  { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
