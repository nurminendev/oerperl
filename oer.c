/*

oer - IRC bot

See ftp://nic.funet.fi/pub/unix/irc/docs/FAQ.gz section 11 for the
definition of the word bot.

Copyright (C) 2000-2004 EQU <equ@equnet.org>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "oer-common.h"
#include "ds.h"
#include "network.h"
#include "misc.h"
#include "perl.h"

/* global (to all) variables */

int do_console;
int do_debug;
int do_output;
int do_tempoutput;
int do_altlogging;

char salt_chars[] = { "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" };
char timed_str[WRITE_BUFFER_LENGTH + 1];

struct state *mystate;
PerlInterpreter *my_perl;

/* global (to oer.c) variables */
int do_signals;

/* prototype definitions */
static void handlesignal(int);

int main(int argc, char *argv[])
{
	extern char *optarg;
        
        extern int optind;
        extern int opterr;
        extern int optopt;
#ifdef HAVE_LOCALE_H
	char *locptr;
#endif
	char stringbuffer[WRITE_BUFFER_LENGTH + 1];
	char altloggingdir[STRINGLEN + 1];
	char basedir[STRINGLEN + 1];
	int retval;
	int c;
	int do_help;
	int do_version;
	fd_set rsock;
	FILE *pidfile;
	pid_t newpid;
	struct timeval tv;
	struct stat st;
#ifdef NEW_SIGNALS
        struct sigaction act;
	struct sigaction old;
#endif
	do_console = 0;
	do_debug = OER_DEBUG_INFO;
	do_help = 0;
	do_output = 0;
	do_signals = 1;
	do_altlogging = 0;
	do_version = 0;
	do_tempoutput = 1;
	if(getcwd(&basedir[0], STRINGLEN) == NULL) {
		oer_debug(OER_DEBUG_FATAL, "couldn't get current directory\n");
		exit(EXIT_FAILURE);
	}
	while((c = getopt(argc, argv, "cd:f:hl:osv")) > 0) {
		switch(c) {
		case 'c':
			do_console = 1;
			break;
		case 'd':
			do_debug = atoi(optarg);
			break;
		case 'f':
			strncpy(basedir, optarg, STRINGLEN);
			break;
		case 'h':
			do_help = 1;
			break;
		case 'l':
			strncpy(altloggingdir, optarg, STRINGLEN);
			do_altlogging = 1;
			break;
		case 'o':
			do_output = 1;
			break;
		case 's':
			do_signals = 0;
			break;
		case 'v':
			do_version = 1;
			break;
		default:
			exit(EXIT_FAILURE);
		}
	}
	if(do_help) {
		oer_debug(OER_DEBUG_INFO, "%s: oer [-c] [-d <level>] [-f <basedir>] [-h] [-l <logging dir>] [-o] [-s] [-v]\n", argv[0]);
		exit(EXIT_SUCCESS);
	}
	oer_debug(OER_DEBUG_INFO, "%s, %s\n\n", OER_VERSION, OER_COPYRIGHT1);
	oer_debug(OER_DEBUG_INFO, "%s\n", OER_COPYRIGHT2);
	if(do_version) {
		exit(EXIT_SUCCESS);
	}
	/* check if there is already a oer running */
	snprintf(stringbuffer, STRINGLEN, "%s/oer.pid", basedir);
	if((pidfile = fopen(stringbuffer, "r")) != NULL) {
		if(fgets(stringbuffer, WRITE_BUFFER_LENGTH, pidfile)) {
			/* exists, check if process is running */
			if(!kill(atoi(stringbuffer), SIGHUP)) {
				oer_debug(OER_DEBUG_FATAL, "main->oer seems to be running, if this is not the case remove oer.pid from %s\n", basedir);
				fclose(pidfile);
				exit(EXIT_FAILURE);
			}
		}
		fclose(pidfile);
	}
#ifdef HAVE_LOCALE_H
	if((locptr = setlocale(LC_CTYPE, "")) == NULL) {
		oer_debug(OER_DEBUG_FATAL, "main->setlocale() failed\n");
                exit(EXIT_FAILURE);
        }
#endif
	if(!do_console) {
		oer_debug(OER_DEBUG_INFO, "main->oer calling fork()\n");
		newpid = fork();
		if(newpid < 0) {
			oer_debug(OER_DEBUG_INFO, "main->couldn't fork()\n");
			exit(EXIT_FAILURE);
		}
		if(newpid) {
			/* the parent */
			oer_debug(OER_DEBUG_INFO, "main->fork successful, oer running in background (child pid is %ld, parent exiting)\n", newpid);
			exit(EXIT_SUCCESS);
		}
		/* necessary for some remote progs */
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		do_console = 0;
	}
	/* create new pidfile */
	snprintf(stringbuffer, STRINGLEN, "%s/oer.pid", basedir);
	if((pidfile = fopen(stringbuffer, "w")) == NULL) {
		oer_debug(OER_DEBUG_FATAL, "main->couldn't open oer.pid: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}	
	snprintf(stringbuffer, WRITE_BUFFER_LENGTH, "%d", getpid());
	if(fputs(stringbuffer, pidfile) == EOF) {
		oer_debug(OER_DEBUG_FATAL, "main->couldn't write to oer.pid: %s\n", strerror(errno));
		fclose(pidfile);
		exit(EXIT_FAILURE);
	}
	fclose(pidfile);
#ifdef NEW_SIGNALS
	sigemptyset(&act.sa_mask);
        act.sa_flags = SA_RESTART;
        act.sa_handler = handlesignal;
        if(sigaction(SIGHUP, &act, &old) < 0) {
                oer_debug(OER_DEBUG_FATAL, "main->failed to set SIGHUP: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
	}
        if(sigaction(SIGINT, &act, &old) < 0) {
                oer_debug(OER_DEBUG_FATAL, "main->failed to set SIGINT: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
	}
        if(sigaction(SIGTERM, &act, &old) < 0) {
                oer_debug(OER_DEBUG_FATAL, "main->failed to set SIGTERM: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
	}
        if(sigaction(SIGPIPE, &act, &old) < 0) {
                oer_debug(OER_DEBUG_FATAL, "main->failed to set SIGPIPE: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
	}
        if(sigaction(SIGUSR1, &act, &old) < 0) {
                oer_debug(OER_DEBUG_FATAL, "main->failed to set SIGUSR1: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
	}
#else
	if(signal(SIGHUP, handlesignal) == SIG_ERR) {
                oer_debug(OER_DEBUG_FATAL, "main->failed to set SIGHUP: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
        }
	if(signal(SIGINT, handlesignal) == SIG_ERR) {
                oer_debug(OER_DEBUG_FATAL, "main->failed to set SIGINT: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
        }
	if(signal(SIGTERM, handlesignal) == SIG_ERR) {
                oer_debug(OER_DEBUG_FATAL, "main->failed to set SIGTERM: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
        }
	if(signal(SIGPIPE, handlesignal) == SIG_ERR) {
                oer_debug(OER_DEBUG_FATAL, "main->failed to set SIGPIPE: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
        }
	if(signal(SIGUSR1, handlesignal) == SIG_ERR) {
                oer_debug(OER_DEBUG_FATAL, "main->failed to set SIGUSR1: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
        }
#endif
	if((mystate = emptystate()) == NULL) {
		oer_debug(OER_DEBUG_INFO, "main->failed to allocate memory for irc state\n");
		exit(EXIT_FAILURE);
	}
	if(!oerperl_init()) {
		oer_debug(OER_DEBUG_FATAL, "main->failed to initialize perl interpreter\n");
		exit(EXIT_FAILURE);
	}
	atexit(oerperl_shutdown);
	strncpy(mystate->basedir, basedir, STRINGLEN);
	oer_debug(OER_DEBUG_INFO, "main->basedir is %s\n", mystate->basedir);
	strncpy(mystate->progname, mybasename(argv[0]), STRINGLEN);
	oer_debug(OER_DEBUG_INFO, "main->progname is %s\n", mystate->progname);
	if(do_altlogging) {
		if(!stat(altloggingdir, &st)) {
			if(S_IFDIR & st.st_mode) {
				strncpy(mystate->altloggingdir, altloggingdir, STRINGLEN);
				oer_debug(OER_DEBUG_INFO, "main->using alternative logging directory %s\n", mystate->altloggingdir);
			}
		}
	}
	if(do_output) {
		if(strlen(mystate->altloggingdir)) {
			snprintf(stringbuffer, WRITE_BUFFER_LENGTH, "%s/oer-%d.output", mystate->altloggingdir, getpid());
		} else {
			snprintf(stringbuffer, WRITE_BUFFER_LENGTH, "oer-%d.output", getpid());
		}
		if((mystate->logto = fopen(stringbuffer, "a+")) == NULL) {
			oer_debug(OER_DEBUG_INFO, "main->couldn't open output file %s: %s\n", stringbuffer, strerror(errno));
			exit(EXIT_FAILURE);
		}
		oer_debug(OER_DEBUG_INFO, "main->debug output gets logged to %s\n", stringbuffer);
	}
	/* from now on we write to console and/or file */
	do_tempoutput = 0;
	oer_debug(OER_DEBUG_INFO, "main->%s (%s %s) starting up\n", OER_VERSION, __DATE__, __TIME__);
	if(!loadall()) {
		oer_debug(OER_DEBUG_INFO, "main->loadall() failed\n");
		exit(EXIT_FAILURE);
	}
	if(do_console) {
                oer_debug(OER_DEBUG_INFO, "console mode is on (debug messages will be shown on screen)\n");
        }
	if(index(mystate->flags, (int)'l') != NULL) {
		if(strlen(mystate->altloggingdir)) {
			snprintf(stringbuffer, WRITE_BUFFER_LENGTH, "%s/oer-%d.raw", mystate->altloggingdir, getpid());
		} else {
			snprintf(stringbuffer, WRITE_BUFFER_LENGTH, "oer-%d.raw", getpid());
		}
		if((mystate->logfp = fopen(stringbuffer, "a+")) == NULL) {
			oer_debug(OER_DEBUG_INFO, "main->couldn't open raw file %s: %s\n", stringbuffer, strerror(errno));
			exit(EXIT_FAILURE);
		}
		oer_debug(OER_DEBUG_INFO, "main->IRC traffic gets logged to %s\n", stringbuffer);
	}
	establishconnection();
        if(mystate->bailout) {
                exit(EXIT_FAILURE);
        }
	mystate->loopforever = 1;
	while(mystate->loopforever) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_FLOOD, "main->mainloop\n");
#endif
		FD_ZERO(&rsock);
		FD_SET(mystate->sockfd, &rsock);
		tv.tv_sec = OER_MAINLOOP_TIMEOUT_SECS;
		tv.tv_usec = OER_MAINLOOP_TIMEOUT_USECS;
		retval = select(mystate->sockfd + 1, &rsock, NULL, NULL, &tv);
		mystate->now = time(NULL);
		if(retval < 0) {
			if(errno != EINTR) {
				oer_debug(OER_DEBUG_ERROR, "main->select() failed: %s\n", strerror(errno));
				mystate->reconnect = OER_RECONNECT_ERROR;
				reconnect();
			}
			continue;
		}
		if(retval > 0) {
			/* we have server I/O */
			if(FD_ISSET(mystate->sockfd, &rsock)) {
				switch(handleserverdata()) {
				case OER_HANDLESERVERDATA_ERR_WRITE:
				case OER_HANDLESERVERDATA_ERR_READ:
				case OER_HANDLESERVERDATA_ERR_NOTHING_READ:
					mystate->reconnect = OER_RECONNECT_ERROR;
					reconnect();
					break;
				}
			}
		}
		reconnect();
		/* retval == 0, timeout */
		switch(registerconnection()) {
		case OER_REGISTERCONNECTION_ERR_GETPWUID:
			exit(EXIT_FAILURE);
		case OER_REGISTERCONNECTION_ERR_WRITE:
		case OER_REGISTERCONNECTION_ERR_HANDLESERVERDATA:
		case OER_REGISTERCONNECTION_TIMEOUT:
			mystate->reconnect = OER_RECONNECT_ERROR;
			reconnect();
			break;
		case OER_REGISTERCONNECTION_ALREADY_REGISTERED:
		case OER_REGISTERCONNECTION_EVERYTHING_OK:
			break;
		}
		/* do some housekeeping */
		processenv();	
		/* convert all mmodes to timeds */
                mmodes2timeds();
		/* process timeds */
		switch(processtimeds()) {
		case OER_PROCESSTIMEDS_ERR_WRITE:
			mystate->reconnect = OER_RECONNECT_ERROR;
			reconnect();
			break;
		}
	}
	exit(EXIT_SUCCESS);
}

static void handlesignal(int signal)
{
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_NOISE, "handlesignal(%d)\n", signal);
#endif
	switch(signal) {
	case SIGHUP:
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "handlesignal->received SIGHUP, flushing disk I/O\n");
#endif
		flushall();
		break;
	case SIGINT:
	case SIGTERM:
		if(do_signals) {
			snprintf(mystate->signoff, STRINGLEN, "received %s, leaving for now but I will return!", (signal == SIGINT) ? "SIGINT" : "SIGTERM");
			quit();
		} else {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_INFO, "handlesignal->%s received, ignoring (do_signals = 0)\n", (signal == SIGINT) ? "SIGINT" : "SIGTERM");
#endif
		}
		break;
	case SIGPIPE:
		mystate->reconnect = OER_RECONNECT_ERROR;
		strncpy(mystate->signoff, "received SIGPIPE, connecting to next server", STRINGLEN);
		break;
	case SIGUSR1:
		do_debug = (do_debug < OER_DEBUG_FLOOD) ? do_debug + 1 : OER_DEBUG_NONE;
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_NONE, "handlesignal->SIGUSR1 received, new debug level is %d\n", do_debug);
#endif
		break;
	}
}
