/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   SYSTEM.H                                                              */
/*                                                                         */
/*   defines the classes THWMouse, TMouse, TEventQueue, TDisplay,          */
/*   TScreen, and TSystemError                                             */
/*                                                                         */
/* ------------------------------------------------------------------------*/
/*
 *      Turbo Vision - Version 2.0
 *
 *      Copyright (c) 1994 by Borland International
 *      All Rights Reserved.
 *
 */

#pragma option -Vo-
#if defined( __BCOPT__ ) && !defined (__FLAT__)
#pragma option -po-
#endif

#if !defined( __EVENT_CODES )
#define __EVENT_CODES

/* Event codes */

const evMouseDown = 0x0001;
const evMouseUp   = 0x0002;
const evMouseMove = 0x0004;
const evMouseAuto = 0x0008;
const evKeyDown   = 0x0010;
const evCommand   = 0x0100;
const evBroadcast = 0x0200;

/* Event masks */

const evNothing   = 0x0000;
const evMouse     = 0x000f;
const evKeyboard  = 0x0010;
const evMessage   = 0xFF00;

/* Mouse button state masks */

const mbLeftButton  = 0x01;
const mbRightButton = 0x02;

/* Mouse event flags */

#if !defined( __FLAT__ )
const meMouseMoved = 0x01;
const meDoubleClick = 0x02;
#else
#if !defined( __WINDOWS_H )
#include <windows.h>
#endif
const meMouseMoved = MOUSE_MOVED;       // NT values from WINDOWS.H
const meDoubleClick = DOUBLE_CLICK;
#endif

#endif  // __EVENT_CODES


#if defined( Uses_TEvent ) && !defined( __TEvent )
#define __TEvent

struct MouseEventType
{
    TPoint where;
    ulong eventFlags;           // Replacement for doubleClick.
    ulong controlKeyState;
    uchar buttons;
};

class THWMouse
{

protected:

    THWMouse();
    THWMouse( const THWMouse& ) {};
    ~THWMouse();

    static void show();
    static void hide();

    static void setRange( ushort, ushort );
    static void getEvent( MouseEventType& );
    static Boolean present();

#if !defined( __FLAT__ )
    static void registerHandler( unsigned, void (_FAR *)() );
#endif

    static void suspend();
    static void resume();
    static void inhibit();

protected:

    static uchar _NEAR buttonCount;

private:

    static Boolean _NEAR handlerInstalled;
    static Boolean _NEAR noMouse;

};

inline Boolean THWMouse::present()
{
    return Boolean( buttonCount != 0 );
}

inline void THWMouse::inhibit()
{
    noMouse = True;
}

class TMouse : public THWMouse
{

public:

    TMouse();
    ~TMouse();

    static void show();
    static void hide();

    static void setRange( ushort, ushort );
    static void getEvent( MouseEventType& );
    static Boolean present();

#if !defined( __FLAT__ )
    static void registerHandler( unsigned, void (_FAR *)() );
#endif

    static void suspend() { THWMouse::suspend(); }
    static void resume() { THWMouse::resume(); }

};

inline void TMouse::show()
{
    THWMouse::show();
}

inline void TMouse::hide()
{
    THWMouse::hide();
}

inline void TMouse::setRange( ushort rx, ushort ry )
{
    THWMouse::setRange( rx, ry );
}

inline void TMouse::getEvent( MouseEventType& me )
{
    THWMouse::getEvent( me );
}

inline Boolean TMouse::present()
{
    return THWMouse::present();
}

#if !defined( __FLAT__ )
inline void TMouse::registerHandler( unsigned mask, void (_FAR *func)() )
{
    THWMouse::registerHandler( mask, func );
}
#endif

struct CharScanType
{
    uchar charCode;
    uchar scanCode;
};

struct KeyDownEvent
{
    union
        {
        ushort keyCode;
        CharScanType charScan;
        };
    ulong controlKeyState;
};

struct MessageEvent
{
    ushort command;
    union
        {
        void *infoPtr;
        long infoLong;
        ushort infoWord;
        short infoInt;
        uchar infoByte;
        char infoChar;
        };
};

struct TEvent
{

    ushort what;
    union
    {
        MouseEventType mouse;
        KeyDownEvent keyDown;
        MessageEvent message;
    };
    void getMouseEvent();
    void getKeyEvent();

};

#endif  // Uses_TEvent

#if defined( Uses_TEventQueue ) && !defined( __TEventQueue )
#define __TEventQueue

class TEventQueue
{
public:
    TEventQueue();
    ~TEventQueue();

    static void getMouseEvent( TEvent& );
    static void suspend();
    static void resume();

    friend class TView;
    friend class TProgram;
    friend void genRefs();

    static ushort _NEAR doubleDelay;
    static Boolean _NEAR mouseReverse;

private:

    static TMouse _NEAR mouse;
    static Boolean getMouseState( TEvent& );

#if !defined( __FLAT__ )
#if !defined( __DPMI16__ )
#define __MOUSEHUGE huge
#else
#define __MOUSEHUGE
#endif
    static void __MOUSEHUGE mouseInt();
#endif

    static void setLast( TEvent& );

    static MouseEventType _NEAR lastMouse;
    static MouseEventType _NEAR curMouse;

    static MouseEventType _NEAR downMouse;
    static ushort _NEAR downTicks;

#if !defined( __FLAT__ )
    static TEvent _NEAR eventQueue[ eventQSize ];
    static TEvent * _NEAR eventQHead;
    static TEvent * _NEAR eventQTail;
    static Boolean _NEAR mouseIntFlag;
    static ushort _NEAR eventCount;
#endif

    static Boolean _NEAR mouseEvents;

    static ushort _NEAR repeatDelay;
    static ushort _NEAR autoTicks;
    static ushort _NEAR autoDelay;

};

inline void TEvent::getMouseEvent()
{
    TEventQueue::getMouseEvent( *this );
}

#endif  // Uses_TEventQueue

#if defined( Uses_TScreen ) && !defined( __TScreen )
#define __TScreen


class TDisplay
{

public:

    friend class TView;

    enum videoModes
        {
        smBW80      = 0x0002,
        smCO80      = 0x0003,
        smMono      = 0x0007,
        smFont8x8   = 0x0100
        };

    static void clearScreen( uchar, uchar );

    static void setCursorType( ushort );
    static ushort getCursorType();

    static ushort getRows();
    static ushort getCols();

    static void setCrtMode( ushort );
    static ushort getCrtMode();

#if !defined( __FLAT__ )
    static int isEGAorVGA();
#endif

protected:

    TDisplay() { updateIntlChars(); };
    TDisplay( const TDisplay& ) { updateIntlChars(); };
    ~TDisplay() {};

private:

#if !defined( __FLAT__ )
    static void videoInt();
#endif

    static void updateIntlChars();

};

class TScreen : public TDisplay
{

public:

    TScreen();
    ~TScreen();

    static void setVideoMode( ushort mode );
    static void clearScreen();

    static ushort _NEAR startupMode;
    static ushort _NEAR startupCursor;
    static ushort _NEAR screenMode;
    static uchar _NEAR screenWidth;
    static uchar _NEAR screenHeight;
    static Boolean _NEAR hiResScreen;
    static Boolean _NEAR checkSnow;
    static ushort * _NEAR screenBuffer;
    static ushort _NEAR cursorLines;
    static Boolean _NEAR clearOnSuspend;

    static void setCrtData();
    static ushort fixCrtMode( ushort );

    static void suspend();
    static void resume();

};

#endif  // Uses_TScreen

#if defined( Uses_TSystemError ) && !defined( __TSystemError )
#define __TSystemError

class _FAR TDrawBuffer;

struct TPMRegs
{
    unsigned long di, si, bp, dummy, bx, dx, cx, ax;
    unsigned flags, es, ds, fs, gs, ip, cs, sp, ss;
};

class TSystemError
{

public:

    TSystemError();
    ~TSystemError();

    static Boolean _NEAR ctrlBreakHit;

    static void suspend();
    static void resume();

#if !defined( __FLAT__ )
    static short ( _FAR *sysErrorFunc )( short, uchar );
#endif

private:

    static Boolean _NEAR saveCtrlBreak;

#if !defined( __FLAT__ )
    static ushort _NEAR sysColorAttr;
    static ushort _NEAR sysMonoAttr;
    static Boolean _NEAR sysErrActive;

    static void swapStatusLine( TDrawBuffer _FAR & );
    static ushort selectKey();
    static short sysErr( short, uchar );

    static const char * const _NEAR errorString[16];
    static const char * _NEAR sRetryOrCancel;

    static Boolean _NEAR inIDE;

    static void interrupt Int24PMThunk();
    static void setupDPMI();
    static void shutdownDPMI();

    static TPMRegs Int24Regs;
    static void (interrupt far *Int24RMThunk)();
    static void (interrupt far *Int24RMCallback)();
    static unsigned Int24RMThunkSel;

    friend class Int11trap;
#endif

};

#if !defined( __FLAT__ )
class Int11trap
{

public:

    Int11trap();
    ~Int11trap();

private:

    static void interrupt handler(...);
    static void interrupt (_FAR * _NEAR oldHandler)(...);

};
#endif

#endif  // Uses_TSystemError

#pragma option -Vo.
#if defined( __BCOPT__ ) && !defined (__FLAT__)
#pragma option -po.
#endif


