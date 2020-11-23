#!/bin/sh

CURRDIR=`pwd`

case `uname` in
  SunOS) ECHO="/usr/ucb/echo" ;;
  *) ECHO="echo" ;;
esac

readln () {
    $ECHO -n "$1"
    IFS='@' read ans || exit 1
    [ -z "$ans" ] && ans=$2
}

if [ $# -lt 2 ]; then
    $ECHO "wrong number of arguments"
    $ECHO ""
    exit 1
fi

INSTALLTO="$1"
INSTALL="$2"
ARGS="$#"
ARGS=`expr $ARGS - 2`

while [ $# -ne $ARGS ]
do
    shift
done

PARAMS=""
while [ $# -ne 0 ]
do
    if [ $# -eq 1 ]; then
      PARAMS="$PARAMS$1"
    else
      PARAMS="$PARAMS$1 "
    fi
    shift
done

$ECHO "Install tool is $INSTALL (params: $PARAMS)"
$ECHO ""
readln "Install oer binary into [$INSTALLTO] " "$INSTALLTO"
INSTALLTO=$ans
if [ ! -d $INSTALLTO ]; then
    $ECHO "Creating directory $INSTALLTO"
    mkdir -p $INSTALLTO/scripts
fi
$ECHO "  Installing oer binary into directory $INSTALLTO"
$INSTALL $PARAMS -m 755 ./oer $INSTALLTO/oer

DEFANS1="Y"
DEFANS2="n"
DEFANS="Y"
if [ -f $INSTALLTO/oer.conf -a -f $INSTALLTO/oer.admins ]; then
    DEFANS1="N"
    DEFANS2="y"
    DEFANS="N"
fi

$ECHO ""
readln "Do you want to create initial configuration for oer? [$DEFANS1/$DEFANS2] " "$DEFANS"
CREATECONF=$ans

if [ "$CREATECONF" = "Y" -o "$CREATECONF" = "y" ]; then
    $ECHO ""
    readln "Please specify default IRC nick [oer] " "oer"
    NICK=$ans
    readln "Please specify alternate IRC nick [oeroer] " "oeroer"
    ALTNICK=$ans
    readln "Please specify user ID [oer] " "oer"
    USERID=$ans
    readln "Please specify user modes [+i] " "+i"
    USERMODES=$ans
    readln "Please specify bot flags [f] " "f"
    BOTFLAGS=$ans
    readln "Please specify command prefix [!] " "!"
    PREFIX=$ans
    readln "Please specify IRC REALNAME [http://oer.equnet.org] " "http://oer.equnet.org"
    REALNAME=$ans
    readln "Please specify optional virtual host [] " ""
    VIRTHOST=$ans
    readln "Please specify optional proxy setup string [] " ""
    PROXYSETUP=$ans
    readln "Please specify default IRC server to connect to [irc.equnet.org] " "irc.equnet.org"
    SERVER=$ans
    readln "Please specify port [6667] " "6667"
    PORT=$ans
    readln "Please specify amount of allowed server modes [6] " "6"
    MODES=$ans
    readln "Please specify ping frequency in seconds on $SERVER [120] " "120"
    PINGFREQ=$ans
    readln "Please specify whether IRC operators are protected on $SERVER (0 = no, 1 = yes) [0] " "0"
    PROT=$ans
    readln "Please specify whether you want line noise for $SERVER (0 = off, >>1 = on) [0] " "0"
    NOISE=$ans
    readln "Please specify optional server password for $SERVER [] " ""
    SERVERPW=$ans
    
    $ECHO ""
    $ECHO -n "Creating configuration/oer.conf ... "
    $ECHO "nick::$NICK" > configuration/oer.conf
    $ECHO "altnick::$ALTNICK" >> configuration/oer.conf
    $ECHO "user::$USERID" >> configuration/oer.conf
    $ECHO "usermode::$USERMODES" >> configuration/oer.conf
    $ECHO "flags::$BOTFLAGS" >> configuration/oer.conf
    $ECHO "prefix::$PREFIX" >> configuration/oer.conf
    $ECHO "realname::$REALNAME" >> configuration/oer.conf
    if [ x$VIRTHOST != x ]; then
	$ECHO "vhost::$VIRTHOST" >> configuration/oer.conf
    fi
    if [ x$PROXYSETUP != x ]; then
	$ECHO "proxysetup::$PROXYSETUP" >> configuration/oer.conf
    fi
    $ECHO "server::$SERVER::$PORT::$MODES::$PINGFREQ::$PROT::$NOISE::$SERVERPW" >> configuration/oer.conf
    
    RC=$?
    
    if [ $RC -eq 1 ]; then
	$ECHO "failed"
	$ECHO ""
	exit 1
    fi
    
    $ECHO "ok"
    $ECHO ""
    readln "Please specify bot admin handle [EQU] " "EQU"
    ADMIN=$ans
    readln "Please specify bot admin flags (n = unremovable, d = dynamic) [n] " "n"
    ADMINFLAGS=$ans
    ISDYN=`$ECHO "$ADMINFLAGS" | sed 's/[0-9A-Za-ce-z]//g'`
    if [ "$ISDYN" = "d" ]; then
	stty -$ECHO
	readln "Please specify your password [] " ""
	PW=$ans
	stty $ECHO
	$ECHO ""
	$ECHO ""
	$ECHO -n "Crypting password ... "
        CRYPT=`./mycrypt $PW`

	if [ $RC -eq 1 ]; then
		$ECHO "failed"
		$ECHO ""
	    exit 1
	fi
	
	$ECHO "ok"
	$ECHO ""
    fi
    readln "Please specify bot admin hostmask [equ@*.equnet.org] " "equ@*.equnet.org"
    ADMINHOSTMASK=$ans
    
    $ECHO ""
    $ECHO -n "Creating configuration/oer.admins ... "
    $ECHO "admin::$ADMIN::$ADMINFLAGS" > configuration/oer.admins
    $ECHO "mask::$ADMIN::$ADMINHOSTMASK" >> configuration/oer.admins
    if [ "$ISDYN" = "d" ]; then
	$ECHO "password::$ADMIN::$CRYPT" >> configuration/oer.admins
    fi
    
    RC=$?
    
    if [ $RC -eq 1 ]; then
	$ECHO "failed"
	$ECHO ""
	exit 1
    fi

    $ECHO "ok"

    $ECHO ""
    SUCCESS=1
    $ECHO "Copying configuration files into $INSTALLTO"
    if [ ! -f $INSTALLTO/oer.conf ]; then
	$INSTALL $PARAMS -m 644 configuration/oer.conf $INSTALLTO
    else
        SUCCESS=0
	$ECHO "oer.conf exists, not copying over"
    fi
    if [ ! -f $INSTALLTO/oer.admins ]; then \
	$INSTALL $PARAMS -m 644 configuration/oer.admins $INSTALLTO
    else
        SUCCESS=0
	$ECHO "oer.admins exists, not copying over"
    fi
    if [ ! -f $INSTALLTO/oer.kickreasons ]; then \
	$INSTALL $PARAMS -m 644 sample-configuration/oer.kickreasons $INSTALLTO
    else
        SUCCESS=0
	$ECHO "oer.kickreasons exists, not copying over"
    fi
    if [ "$SUCCESS" = "1" ]; then
	$ECHO ""
	$ECHO "  oer configuration installed succesfully"
    fi
fi

$ECHO ""
$ECHO "  Type \"./oer\" in $INSTALLTO to start oer"
$ECHO ""

exit 0
