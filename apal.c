/*
**  $ File: autopal.c               $
**  $ Version: 1.12                 $
**  $ Author: C.Laurence Gonsalves  $
**  $ Revision: 40                  $
**
**  Well, that's a rough estimate...
**
*/

#include <clib/exec_protos.h>
#include <clib/intuition_protos.h>
#include <clib/utility_protos.h>
#include <clib/graphics_protos.h>

#include <exec/types.h>
#include <exec/memory.h>

#include <intuition/intuition.h>
#include <intuition/screens.h>

#include <graphics/displayinfo.h>

#include <dos/dos.h>

#ifdef _beta_
#include <stdio.h>
#endif

#define DisplayID_MASK (0x00008ce4)
#define ANSI "\x1b["

#define NTSC_MIN 200
#define NTSC_MAX 241
#define PAL_MIN 256
#define PAL_MAX 290    /* Should be 283, but DPaint has a slight "glitch", so
                          I've compromised a bit.                             */

extern struct IntuitionBase *IntuitionBase;
extern struct Library *UtilityBase;

#define version "1.12b"

static UBYTE ver[]="$VER: AutoPAL "version" (18.04.93)\n";

#ifdef _beta_
            FILE    *file;
#endif

static union
    {
        struct
        {
            unsigned
            dbl:1,
            auto_mon:2,
            norm_mon:2,
            quit:1,
            cent:1;
        }bits;
        ULONG num;
    }
    options;    /* this is where the user options are stored. Note that they
                   are only written to at startup, so screens may be opened
                   asynchronously. */

char *oldtaskname;
struct Task *mytask;

APTR newVectorTagList, newVector;
__asm struct Screen *(*oldOpenScreenTagList)(register __a0 struct NewScreen *n, register __a1 struct TagItem *);
__asm struct Screen *(*oldOpenScreen)(register __a0 struct NewScreen *);

#define NORM_MONITOR_ID ((options.bits.norm_mon==1)?(PAL_MONITOR_ID):((options.bits.norm_mon==2)?(NTSC_MONITOR_ID):(DEFAULT_MONITOR_ID)))
#define AUTO_MONITOR_ID ((options.bits.auto_mon==0)?(PAL_MONITOR_ID):((options.bits.auto_mon==1)?(NTSC_MONITOR_ID):(DEFAULT_MONITOR_ID)))

struct Screen * __saveds __interrupt __asm
newOpenScreenTags(register __a0 struct NewScreen *ns, register __a1 struct TagItem *old_tags)
{
                    /* These are the indices of the tags in our array */

#define cent_tag 0  /* used when centre mode is on */
#define nmon_tag 1  /* used for replacing monitor */
#define last_tag 2  /* used either for ending tag-list or jumping to app's */

    struct Screen *res;
    struct TagItem new_tags[last_tag+1]={{TAG_IGNORE}}; /* default to no cent */
    ULONG sa_dispid=0,old_mon;
    WORD height=-1,width;

#ifdef _beta_

    WORD topp;

#endif

    WORD bottom=0;
    #define top bottom
         /* ^^^ this is an alias!!! */

    ULONG new_mon=~MONITOR_ID_MASK; /* Note that this isn't a valid ID, but is
                                       a non-zero (ie: true) value.
                                    */

    if(ns)
    {
        sa_dispid=ns->ViewModes&DisplayID_MASK;
        top=ns->TopEdge;
        height=ns->Height;
        width=ns->Width;
    }

    if(old_tags)
    {
        new_tags[last_tag].ti_Tag=TAG_MORE;
        new_tags[last_tag].ti_Data=(ULONG)(old_tags);

        if(sa_dispid=GetTagData(SA_DisplayID,sa_dispid,old_tags))
            if(old_mon=sa_dispid&MONITOR_ID_MASK)
                if((old_mon!=NTSC_MONITOR_ID)&&(old_mon!=PAL_MONITOR_ID))
                    new_mon=NULL;
                else
                    new_mon=old_mon;

        /*
        The above code steals the old monitor ID, if it was NTSC or PAL. If not
        then we'll forget about the whole thing and leave the screen thing
        alone. This is done just in case double-mode is set, so we can double
        not only screens WE change, but screens that were already set to NTSC
        or PAL.

        BTW, why not steal DBLNTSC or DBLPAL also? Simply because we only
        need NTSC and PAL if we plan on setting DBLPAL and DBLNTSC anyways...
        ie: it would be pointless.

        In other words, we are attempting to ascertain the intelligence of the
        calling program. Some have no idea what's going on (1.0-1.3), some
        know about monitor ID's (2.0-2.1), and some know about the double modes
        (3.0 and up).

        BTW, I wrote this under 2.1, with slightly hacked 2.04 includes...
        (I added a bit of 3.0 support!)
        */

        height=GetTagData(SA_Height,height,old_tags);
        width=GetTagData(SA_Width,width,old_tags);
        top=GetTagData(SA_Top,top,old_tags);
    }
    else
        new_tags[last_tag].ti_Tag=TAG_DONE;

    /*
    This is partially to decide whether our tags should call the user tags
    or not. If user tags are there our tag will call 'em. Good thing C= was
    nice enough to create tags like "TAG_MORE"... :-)
    */

#ifdef _beta_

    topp=top;   /* for the printf that comes later */

#endif

    /*
    In the next block we actually assign our new monitor using the pal/ntsc
    switch algorithm, if one hasn't already been decided (ie: either NTSC,
    PAL, or "NULL", meaning no switching at all.
    */

    if(new_mon==~MONITOR_ID_MASK)
    {
        if(height!=-1)
        {
            bottom+=height;     /* Note: "top" was really bottom. This is where
                                   it starts to live up to its name. */
            if(sa_dispid&LACE)
            {
                if((bottom>=NTSC_MIN*2)&&(bottom<=NTSC_MAX*2))
                    new_mon=NTSC_MONITOR_ID;
                else
                if((bottom>=PAL_MIN*2)&&(bottom<=PAL_MAX*2))
                    new_mon=PAL_MONITOR_ID;
                else
                    new_mon=NORM_MONITOR_ID;
            }
            else
            {
                if((bottom>=NTSC_MIN)&&(bottom<=NTSC_MAX))
                    new_mon=NTSC_MONITOR_ID;
                else
                if((bottom>=PAL_MIN)&&(bottom<=PAL_MAX))
                    new_mon=PAL_MONITOR_ID;
                else
                    new_mon=NORM_MONITOR_ID;
            }
        }
        else new_mon=AUTO_MONITOR_ID;
    }

    if(new_mon)
    {
        if((options.bits.dbl)&&(new_mon&LACE))
            new_mon|=DBL_MONITOR_ID;

        new_tags[nmon_tag].ti_Tag=SA_DisplayID;
        new_tags[nmon_tag].ti_Data=sa_dispid|new_mon;

#ifdef _beta_
        fprintf( file, ANSI"32mNewScr:"ANSI"31m%s "ANSI"32mTags:"ANSI"31m%s "ANSI"32mOld ID:"ANSI"31m0x%08lX "ANSI"32mNew ID:"ANSI"31m0x%08lX "ANSI"32mHeight:"
            ANSI"31m%3ld "ANSI"32mTop:"ANSI"31m%3ld \n",((ns)?"Y":"N"),((old_tags)?"Y":"N"),sa_dispid,sa_dispid|new_mon,height,topp);
#endif
    }
    else
        new_tags[nmon_tag].ti_Tag=TAG_IGNORE;

    if(options.bits.cent)
        {
            struct DimensionInfo di;

            GetDisplayInfoData(NULL,(UBYTE *)&di,sizeof(struct DimensionInfo),DTAG_DIMS,sa_dispid);
            if(di.TxtOScan.MaxX>width)
            {
                new_tags[cent_tag].ti_Tag= SA_Left;
                new_tags[cent_tag].ti_Data=(di.TxtOScan.MaxX-width)/2;
            }
        }

    res=oldOpenScreenTagList(ns,new_tags);

    return(res);
}

int __saveds __interrupt __asm
newOpenScreen(register __a0 struct ExtNewScreen *ns)
{
    struct Screen *sptr;
    struct TagItem *nsext=NULL;

    /*
    This patch really just converts OpenScreen's into OpenScreenTagList's. The
    monitor fiddling is done there. Oddly, the real OpenScreenTagList and
    OpenScreen are completely independant! You can test this by patching one,
    and trying to use the other. Either way, the un-patched routine remains
    un-affected! Thing is- if the big C decides to change it so
    OpenScreenTagList calls OpenScreen then AutoPAL will cease to work. This
    would be silly, however, 'cause OpenScreenTagList is much better! :-)
    */

    if(ns->Type&NS_EXTENDED)
    {
        nsext=ns->Extension;
        ns->Extension=NULL;
        ns->Type^=NS_EXTENDED;
    }

    sptr=(struct Screen *)newOpenScreenTags((struct NewScreen *)ns,nsext);

    if(nsext)
    {
        ns->Extension=nsext;
        ns->Type^=NS_EXTENDED;
    }

    /*
    The above is for those weird V36 coders who used OpenScreen with a
    ExtNewScreen. Note that similar support is not provided in the
    OpenScreenTagList function (ie: if you use an ExtNewScreen it's treated
    as a NewScreen). I was going to fix this, but the autodocs don't
    say you can do this. Who would, anyways?
    */

    return((int)(sptr));
}

LONG ezreq(UBYTE *Text, UBYTE *Gads) /* very simply request routine */
{
    struct EasyStruct ezreq=
    {
        sizeof(struct EasyStruct),
        0,
        "AutoPAL "version" Request",
        NULL,
        NULL
    };

    ezreq.es_TextFormat=Text;
    ezreq.es_GadgetFormat=Gads;

    return(EasyRequest(NULL,&ezreq,NULL));
}

int leave(int x) /* this is for restoring the task name. Necessary? I dunno. */
{
    Forbid();
    mytask->tc_Node.ln_Name=oldtaskname;
    Permit();
    return(x);
}

main(int argc,char *argv[])
{
    char taskname[]="AutoPAL Task";
    UBYTE optnum=1;             /* option counter. */
    #define not_quit optnum     /* also used as negative quit flag (0=quit) */

    options.num=0;
#ifdef _beta_
    file = fopen( "PIPE:apal", "wb" );
#endif

    if(!(IntuitionBase=(struct IntuitionBase *)OpenLibrary("intuition.library", 36)))
        return(10);

    while(argc>optnum) /* not_quit is masquerading as an arg counter. */
    {                     /* I'm a memory miser, so sue me... :-)        */
        switch((char)*(argv[optnum]))
        {
            case 'd':
                options.bits.dbl=1;         /* these parse the command line     */
                break;                      /* note that the options are case   */
            case 'p':                       /* sensitive, and contradicting     */
                options.bits.auto_mon=0;    /* options are allowed, with the    */
                break;                      /* later option over-riding those   */
            case 'n':                       /* preceding. ie: in "p n", "n"     */
                options.bits.auto_mon=1;    /* over-rides "p".                  */
                break;
            case 'h':                       /* defaults are:                    */
                options.bits.auto_mon=2;    /* p H                              */
                break;
            case 'P':
                options.bits.norm_mon=1;
                break;
            case 'N':
                options.bits.norm_mon=2;
                break;
            case 'H':
                options.bits.norm_mon=0;
                break;
            case 'q':
                options.bits.quit=1;
                break;
            case 'c':
                options.bits.cent=1;
                break;
            default:
                ezreq(
                    "  Copyright ©1993 C.Laurence Gonsalves\n\n"
                    "           Many thanks go to:\n"
                    "Darrell Grainger - asm & SetFunction help\n"
                    "    Steve Tibbett - source for NoReq\n"
                    "       Mike Haba - 2.0 Beta tester\n"
                    "      Peter Feigl - 3.0 Beta tester\n\n"
                    "          See docs for details!\n"
                    ," OK ");
                CloseLibrary((struct Library *)IntuitionBase);
                return(leave(0));   /* if an unknown option is given a help
                                       window is shown, and then prg quits. */
        }
        optnum++;
    }

    /*
    We can assume not_quit has a non-zero value. This should be true so
    long as Mr. User doesn't go and enter a ridiculous number of args (ie:
    about 256 or so). From here-on it lives up to it's name.
    */

    mytask=(struct Task *)FindTask((STRPTR)&taskname); /* Find AutoPAL */

    if(mytask)
    {
        /* I'm already running! */

        if(!options.bits.quit)
            options.bits.quit=ezreq("  AutoPAL already\nrunning. Remove it?","OK|CANCEL");

        if(options.bits.quit)
        {
            CloseLibrary((struct Library *)IntuitionBase);
            Forbid();
            mytask=(struct Task *)FindTask((STRPTR)&taskname);
            Signal(mytask,SIGBREAKF_CTRL_C);
            Permit();
        }
        return(0);
    }

    if(options.bits.quit)return(0);

    mytask=(struct Task *)FindTask(NULL);       /* Find me...                 */
    oldtaskname=mytask->tc_Node.ln_Name;        /* I restore the name later   */
                                                /* Is this necessary?         */

    Forbid();                                   /* I'm not sure if I need the */
    mytask->tc_Node.ln_Name=(char *)&taskname;  /* Forbid/Permit, but don't   */
    Permit();                                   /* want to take the chance    */

    /* This task is now called "AutoPAL Task", so I stick out in a crowd */

    /*
    We need utility.library to do all the neat tag-tricks. It's a ROM lib,
    so it's no extra overhead.
    */

    if(!(UtilityBase=(struct Library *)OpenLibrary("utility.library", 36)))
    {
        ezreq("Can't open utility.library V36."," OK ");
        CloseLibrary((struct Library *)IntuitionBase);
        return(leave(10));
    }

    /* We now patch the functions. Whoo-hoo! */

    if(!(oldOpenScreenTagList=(struct Screen *(*))SetFunction(
                        (struct Library *)IntuitionBase, -0x264,
                        (void *)newOpenScreenTags)))
    {
        ezreq("Couldn't install wedge\nfor OpenScreenTagList()!"," OK ");
        CloseLibrary(UtilityBase);
        CloseLibrary((struct Library *)IntuitionBase);
        return(leave(15));
    }

    if(!(oldOpenScreen=SetFunction((struct Library *)IntuitionBase, -0xc6, (APTR)newOpenScreen)))
    {
        ezreq("Couldn't install wedge\nfor OpenScreen()!"," OK ");
        if(&newOpenScreenTags!=SetFunction((struct Library *)IntuitionBase, -0x264, (APTR)oldOpenScreenTagList))
            if(!ezreq("*** Error removing wedge ***","OK|Reboot"))
                ColdReboot();
        CloseLibrary(UtilityBase);
        CloseLibrary((struct Library *)IntuitionBase);
        return(leave(15));
    }

#ifdef _beta_

    fprintf( file, "\x0c"ANSI"43;32m AutoPAL "version" - Copyright ©1993 C.Laurence Gonsalves                     \n"
                    ANSI"30m Note: This is a beta test version. End-user versions should be available \n"
                           " on your local BBS.                                                       \n"ANSI"40m");

#endif

    /*
    Now we sit around twiddling our thumbs... No, we don't even do that. We just
    do a nice, 0 processor use, Wait(). The loop is for people who wake us up,
    and then let us go back to sleep.
    */

    do
    {
        Wait(SIGBREAKF_CTRL_C);
        Forbid();
        if(&newOpenScreen!=(newVector=SetFunction((struct Library *)IntuitionBase, -0xc6, (APTR)oldOpenScreen)))
            SetFunction((struct Library *)IntuitionBase, -0xc6, newVector);
        else
        if((void *)&newOpenScreenTags!=(void *)(newVectorTagList=SetFunction((struct Library *)IntuitionBase, -0x264, (APTR)oldOpenScreenTagList)))
        {
            SetFunction((struct Library *)IntuitionBase, -0xc6, newVector);
            SetFunction((struct Library *)IntuitionBase, -0x264, newVectorTagList);
        }
        else not_quit=FALSE;
        Permit();

        if(not_quit)
        {
            switch (ezreq("      *** WARNING ***\n\n  Removing wedges now may\ncompromise system integrity","OK|Reboot|CANCEL"))
            {
                case 1:
                    SetFunction((struct Library *)IntuitionBase, -0xc6, (APTR)oldOpenScreen);
                    SetFunction((struct Library *)IntuitionBase, -0x264, (APTR)oldOpenScreenTagList);
                    not_quit=FALSE;
                    break;
                case 2:
                    ColdReboot();
            }
        }
    }
    while(not_quit);

    CloseLibrary(UtilityBase);     /* Note we close in reverse order. :-) */
    CloseLibrary((struct Library *)IntuitionBase);

#ifdef _beta_
    fclose( file );
#endif

    return(leave(0));
}
