AutoPAL 1.12b - Copyright �1993 C.Laurence Gonsalves

DISCLAIMER
=

Ahem...

THIS PROGRAM AND ACCOMPANYING MATERIALS ARE PROVIDED WITHOUT WARRANTY OF ANY
KIND, EITHER EXPRESS OR IMPLIED. IN NO EVENT WILL WE BE LIABLE FOR ANY DAMAGES,
INCLUDING LOSS OF DATA, LOSS OF PROFIT, LOSS OF HAIR, OR ANY OTHER DIRECT,
INDIRECT, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING FROM THE USE OR ABUSE OF
THIS PROGRAM, HOWEVER CAUSED, AND ON ANY THEORY OF LIABILITY. TO USE THIS
PROGRAM YOU MUST AGREE WITH THE ABOVE AND ACCEPT AND CONSEQUENCES ARISING FROM
THE USE OF THIS PROGRAM AS YOUR OWN RESPONSIBILITY. USE THIS PROGRAM AT YOUR
OWN RISK.

Hey, it's free.

REQUIREMENTS
=

AutoPAL requires:

- an ECS Agnus. Should work with Alice (AGA) too.
- System 2.0 (or higher)
- Oh, and of course, an Amiga... :-)

INTRODUCTION
=

When I installed 2.1 upgrade in my Amiga one of the first things I noticed was
the collection of monitor files included. When first installed, "NTSC" was in
my devices drawer, and many others were in storage. I decided that since I
could use PAL on my machine I ought to drag the "PAL" monitor to devices also.

Later, when playing with the Prefs, I noticed that I could set the Workbench
for either PAL or NTSC. Oddly enough, this setting had no effect on the other
screens. I could have PAL and NTSC screens open at the same time, and the
computer would automatically adjust to the mode of the top screen!

Unfortunately, most of my older software didn't know about this nifty feature
of 2.1. MED, for instance, which looks much nicer on a PAL screen, did not
switch to PAL. So how to remedy this oversight? Make the OS even smarter!

AutoPAL is a simple but useful patch for Intuition. It patches Intuition so
that when screens are opened they will be automatically placed in PAL mode if
they are PAL screens. This means that all of those PAL programs you have will
automatically switch to 50Hz, and NTSC software will automatically switch to
60Hz. This is all provided that they open their screens through Intuition. Most
application programs, and even a few games, do this. 

INSTALLATION
=

To install AutoPAL, first of all make sure you've got PAL and NTSC in your
Monitors drawer. You MUST do this, or AutoPAL will actually prevent most
programs from working!!! MAKE SURE NTSC AND PAL ARE IN YOUR MONITORS DRAWER!!!

It seems that this drawer moves around with different versions of the OS, so if
your version isn't listed you might have to look around.

For 2.0 to 2.05:
-

Look for the "Monitors" drawer in your system disk.  (Sys:Monitors) If either
of NTSC or PAL is missing, copy the file(s) from the monitor storage drawer
(Sys:MonitorStore), or from your system disks.

For 2.1 and higher:
-

Look for "Monitors" in the "Devs" drawer of your system disk.
(Sys:Devs/Monitors) If either of NTSC or PAL is missing, copy the file(s) from
the storage drawer (Sys:Storage/Monitors), or from your system disks.

Note for AGA users: If you've got an AGA machine (1200 or 4000 right now...)
and you want to use the "doubling" feature of AutoPAL be sure to also copy the
"double" monitors to your monitor drawer. Some monitors can't handle the double
modes - read your system manual for details! You'll also have to use the
"double" option when running AutoPAL, which means you must run it from the CLI,
or tack it onto the end of your `user-startup`. Also please note that veryt
little testing of this mode has been done, as I don't have an AGA machine
myself. If you want more AGA support send me some cash and I'll buy a 1200 or a
4000/030... :-)

For all OS's (2.0 and higher):
-

Once the above is done you actually have to run AutoPAL. To run from Workbench
just use the dandy little icon included. From the CLI just type "AutoPAL" at
the prompt. Note that AutoPAL is auto-detaching, so you won't have to use "run"
or anything like that.

To install it so it automatically starts up either drag it to the `WBStartup`
drawer, or add a line like:

    AutoPAL

to the end of your `user-startup`.

OPTIONS

AutoPAL now has some options you can specify from the command line. Sorry WB
users, you're out of luck here. Maybe in the next release I'll add tooltypes.
Note that all options below are case sensitive, so "D" is not acceptable, but
"d", "double", and "dioxyribo-nucleic-acid" are. :-)

The options are:

- `double`: doubles screens. This only works on AGA machines, and I believe you must have additional monitors in your monitors drawer. They are probably called DoubleNTSC and DoublePAL, or something like that. These screens are equivalent to the NTSC and PAL screens, except that they have no flicker in "interlaced" modes, and they require a special monitor (multi-sync, I believe). I can't test this option, as I don't have an AGA machine.

- `quit`: will suppress the "AutoPAL already running..." requestor, if you want to quit from a script. Note that it will NOT supress the warning requestor, so it's just as safe. Also, if AutoPAL is not already running, this version will not run either. In other words: if this goes okay, there won't be an AutoPAL running.

- `pal`: sets auto mode to PAL.

- `ntsc`: sets auto mode to NTSC.

- `hardware`: sets auto mode to hardware setting.

- `PAL`: sets default mode to PAL.

- `NTSC`: sets default mode to NTSC.

- `Hardware`: sets default mode to hardware setting.

See section below for explanation of auto and default modes.

WEIRD SCREENS
=

Some screens use weird heights, which AutoPAL can't interpret. AutoPAL uses the
NTSC and PAL extents (maximum and minimum heights) to determine which mode a
screen should be in, and sometimes the screen height is outside of both of
these extents.

If this happens, AutoPAL set's the screen to the "default mode" as set by the
user (if you don't set it, it uses the hardware setting). If you're using the
double option be sure to set the default mode to NTSC or PAL manually, or these
weird screens won't be doubled!!!

There is one screen height that's special. It's "-1". When a programmer uses
this he's either 

1. A lazy @$#%!

2. (or hopefully) Trying to find out what kind of machine you're using (PAL or NTSC).

See, if you ask the computer for a screen of height -1, it will automatically
give you the height for that type of screen. Since most programmer's aren't as
lazy (or sloppy) as to assume that a height of -1 will result in a certain
height, AutoPAL assumes they're trying to find your favorite screen size. This
is what "auto" mode is. It's the mode AutoPAL sets screens to when their height
is -1.

Lots of great programs use this, by the way. MED, OctaMED, Hex and Q-Blue all
use this "auto" mode. By default it's set to pal, as I'm guessing most people
will want the maximum resolution. Also, some programs that use the -1 option
properly will give you more graphics in PAL mode (MED and Hex in
particular...).

If you want, you can set it to ntsc. "hardware" is also avaialble, but was
really just added for consistency.

As above, if you're using the "double" option you should not set the auto mode
to "hardware", or these screens WILL NOT BE DOUBLED.

Here are some example command lines:

    AutoPAL p H - the default settings
    AutoPAL p N d - great for NTSC 4000 users*
    AutoPAL p P d - great for PAL 4000 users*
    AutoPAL h H d - BAD SETTINGS. Not all screens will be doubled!
    AutoPAL d - BAD SETTINGS. Not all screens will be doubled!

\* Note: see installation for details on setting these up. Not all AGA systems
can use these modes (depends on your monitor).

TESTING
=

To test out how AutoPAL works, run it (or if you've installed it in your
`WBStartup` or `user-startup`, reset your machine). Then try the following:

Run a program that opens an NTSC sized screen, and then a program that opens a
PAL sized screen. On a PAL machine, the NTSC screen would look squashed, and on
an NTSC machine the PAL screen would look stretched (not to mention about a
quarter of the picture gets chopped off). With AutoPAL running however, this
should not happen. Also, unlike normal PAL/NTSC switchers that require you to
switch modes manually each time you switch applications, with AutoPAL you
simply switch screens (try tapping <kbd>left Amiga N</kbd>).

BTW, I've had AutoPAL running on my system for a a while now (over a year),
and have had very few problems with it. In fact, since the last two versions I
haven't had ANY problems with it. This version has no more "bug fixes"... I'm
just enhancing it now.

EXITING
=

To leave AutoPAL simply run it again. If you haven't specified the q option a
requester will pop up asking if you're sure, and if you are it'll attempt to
remove the wedges by resetting the vectors and then clear itself from RAM. If
it finds that someone else has already changed the vectors it'll give you a
warning requester. There are three options on this requester:

- OK - still tries to remove AutoPAL. In this situation this can be very
dangerous, as something may still have AutoPAL's vectors. If this program
replaces AutoPAL's vectors when AutoPAL is gone, `OpenScreen()` and
`OpenScreenTagList()` may go to never never land. In other words: don't do this
unless you know what you're doing, or at least don't open any screens (closing
screens is okay though).

- Reboot - Reboots the system. Note: this is the same as control- amiga-amiga.
Make sure all drive activity has stopped before selecting this option!

- CANCEL - Cancel's your request. You can now go and remove whatever program is
causing the problem, if you know what it is.  AutoPAL will remain installed,
and you can later repeat your request to remove it.

HOW DOES IT WORK?
=

AutoPAL "patches" the system routines `OpenScreen` and `OpenScreenTags`. These
are the only two routes through Intuition that may be taken to open a screen.

The `OpenScreen` patch basically reformats the parameters and then calls the
`OpenScreenTags` patch.

The `OpenScreenTags` patch is where the magic happens. First AutoPAL checks to
see if the screen has a set MONITOR_ID that is neither PAL or NTSC. If so, it
just passes it on to the real `OpenScreenTags` unmodified.

If there's no MONITOR_ID at all, AutoPAL uses the Height, TopEdge, and
ViewModes/SA_DisplayID of the screen to determine what MONITOR_ID it should
have. If it can't make up it's mind it just leaves the MONITOR_ID blank,
meaning "default monitor". In most situations however, it can decide on either
NTSC or PAL by comparing the height with the maximum and minimum values allowed
for these monitors.

If there already was a MONITOR_ID, but it was PAL or NTSC, AutoPAL will use the
mode already passed, but may "tweak" it, if you have certain options set.
Currently the only option that does this is d - double mode.

The PAL/NTSC monitor switching is not actually done by AutoPAL. It's done by
the OS, so if you install AutoPAL, open some screens which AutoPAL sets to PAL
or NTSC, and then remove AutoPAL, the screens opened will still switch to the
correct scan rate until they are closed.

BUGS
=

There's an elusive bug regarding the centre mode, and there may be bugs in the
double mode.

HISTORY
=

- 1.00 - First working version.

- 1.02 - Changed PAL-check algorithm so overscan is understood. Added switching
  to NTSC, as well as to PAL. Made quitting the program relatively easy. Fixed
  exit routine so it warns before destroying system, not after.

- 1.03 - Tweaked PAL-check algorithm as a bug was noticed when using MuchMore.
  This was caused by their hack for interleaved didsplays (I believe).
  Algorithm should be perfect now. Program auto-detaching, as suggested by
  Darrell Grainger.

- 1.04 - Fixed bug in manual. Version number was wrong... :-) Modified PAL check
  algorithm so it would use the BOTTOM of a screen to pick a mode, not its
  height. This fixes weirdness caused by things like palette in DPAintIV. Fixed
  `ViewModes -> SA_DisplayID` filter so it's a bit more strict.

- 1.05 - NOT RELEASED. Was to be the first version with a "double option".

- 1.06 - NOT RELEASED. Decided to add a quit option while I was at it.

- 1.10 - I got really carried away, so I added a whole tonne of new options, and
re-wrote a lot of the code. Also tweaked compiler options, so code is REALLY
tiny now (slightly over 5K!). See the "Options" section for details.

- 1.12b - Cleaned up the code a bit... it's now 100% C, and doesn't use any
  "borrowed" code. Previous versions used a small stubb written by Steve
  Tibbett (well, I changed it a bit... :-). I found away around this, so the
  code is more legible. You might be wondering what the "b" is for (in the
  version number). Well... there's a bug in this version, but I haven't found
  the cause yet. The centre mode will make some screens go kerblangos. Oddly,
  this has only happened with programs I've written... other software seems to
  be immune to it. Also, once I completely debug my software the kerblangoness
  dissapears... is it real, or is it AutoPAL? I did find a few flaws, BTW,
  which is why the program's now a bit bigger.

COMING IN THE FUTURE:
=
I really want to add tool-types support. I also want to
add a "force list". This will be a list (probably in the tooltypes) of the
names of screens that you want to force to a certain mode (PAL or NTSC). This
will be for programs that do stupid things like opening a screen without
setting a size, and then assuming that it's 200 lines high (is the author of
Elements III out there?). with the force list you'd say something like
NTSC=Elements III screen (or whatever). 

CREDITS
=

AutoPAL was written by C.Laurence Gonsalves.

A lot of advice regarding SetFunction and the Amiga OS was provided by Darrell
Grainger. He also explained how a vital piece of assembly worked. AutoPAL was
written in C, BTW, but a small piece of asm was necessary. Since I don't know
assembly code, the explanation of the code was invaluable.

I also have to thank Steve Tibbett. I examined the source for NoReq extensively
to get an idea of how SetFunction worked, and the bit of assembly mentioned
above was based on "asmbit.a", a part of NoReq (and AssignX). This code
(asmbit.a) is no longer used, but it was crucial to the early stages of
AutoPAL's development.

Also, thanks go to Mike Haba, who beta tested AutoPAL under 2.0, and confirmed
a few suspicions of mine - such as the fact that you MUST have the monitor
files in the Monitors drawer.

Finally, thanks to Peter Feigl for the info I needed to get the "double" modes
working on AGA machines, and for his great help in beta-testing this beast.

DONATIONS?
=

No, I don't really expect donations, but I'll accept them. You can think of
this program as GiftWare. If you really like it, send me something in return.
Here are a few suggestions: C source code, software, programming documentation,
a 4000, or if you're really stuck - money. (any currency accepted) :-)

Oh, yes. This file can be freely distributed for non-commercial use, provided
ALL of the files in the archive travel with it, and are left unmodified. If you
want to use AutoPAL commercially, you must contact me to register it.

The files that must be in the AutoPAL archive are:

    AutoPAL.info - drawer icon
    AutoPAL/AutoPAL- executable
    AutoPAL/AutoPAL.info - icon for executable
    AutoPAL/AutoPAL.doc- this file
    AutoPAL/AutoPAL.doc.info - icon for this file

I can be reached at the following address:

    C.Laurence Gonsalves
    208 Forsyth Road
    Newmarket, ONTARIO CANADA
    L3Y 7Y1
