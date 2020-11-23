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
#include "oer.h"
#include "misc.h"
#include "reg.h"
#include "perl.h"

/* prototype definitions */
char *safeban(struct channel *, char *, char *);
char *getfirsthostless(struct channel *, char *);
char *getkickreason(char *);
char getmodepol(char *, char);
int partcount(struct channel *, char *, char *);
int loadall(void);
int loadconf(void);
int loadchannels(void);
int loadtrusted(void);
int loadservices(void);
int loadscripts(void);
int loadadmins(void);
int loadkickreasons(void);
int loadchaninfo(void);
int saveall(void);
int saveconf(void);
int savechannels(void);
int savetrusted(void);
int saveservices(void);
int savescripts(void);
int saveadmins(void);
int savekickreasons(void);
int savechaninfo(void);
int parseconf(char *);
int parsechannels(char *);
int parsetrusted(char *);
int parseservices(char *);
int parsescripts(char *);
int parseadmins(char *);
int parsekickreasons(char *);
int parsechaninfo(char *);
int burstjoins(struct channel *);
int isq(char *);
int isservice(char *);
int istrusted(char *);
int isme(char *);
int isatleastopnow(struct channel *, char *, char *);
int isatleastop(struct channel *, char *, char *);
int isallowedop(struct channel *, char *);
int isop(struct channel *, char *);
int isvoice(struct channel *, char *);
int isvoicenow(struct channel *, char *);
int isvalidlogon(struct channel *, char *);
int isopa(char *);
int ishostless(char *);
int delopa(char *);
int isadmin(struct channel *, char *);
int isonchan(struct channel *, char *);
int issameuser(struct channel *, char *, char *);
int isopnow(struct channel *, char *);
int isflood(struct channel *, char *, char *, char *);
int ispermban(struct channel *, char *);
int permbancount(struct channel *);
int topiccount(struct channel *);
int haschanflags(struct channel *, char *);
int hasadminflags(char *, char *);
int nickchange(struct channel *, char *, char *);
int nthmode(char *, int);
int userleft(struct channel *, char *, char *);
int changeuser(struct channel *, char *, int, int, int, int);
int whichctcp(char *);
int whichcommand(char *, int);
int gettobek(struct channel *, char *);
int checkforansi(struct channel *, char *, char *, char *);
int checkforflood(struct channel *, char *, char *, char *);
int checkfornickflood(struct channel *, char *, char *);
int checkforbadword(struct channel *, char *, char *, char *);
int checkforbadnick(struct channel *, char *, char *);
int checkforautorejoin(struct channel *, char *, char *);
int checkforpartrejoin(struct channel *, char *, char *);
int checkforaction(struct channel *, char *, char *, int);
int delchankey(struct channel *);
int delchanlimit(struct channel *);
int delchanmode(struct channel *);
int delpermbans(struct channel *, char *, int);
int delnickbks(struct channel *, char *, int);
int delwordbks(struct channel *, char *, int);
int uptime(time_t *, time_t *);
int deladmin(char *);
int deladminmask(char *, char *);
int deluser(struct channel *, char *);
int delusermasks(struct channel *, char *, char *, int);
int deltrusted(char *);
int delservice(char *);
int delscript(char *);
int delchannel(struct channel *);
int delkickreason(char *);
int deladverts(struct channel *, char *, int);
int getondiskmsgcount(struct channel *, char *);
int getondiskjoincount(struct channel *, char *);
int getjoincount(struct channel *, char *);
int getrandommsg(struct channel *, char *, int, int, char *, int);
int delserver(char *, int, int, int, int, int);
int setpassword(struct channel *, char *, char *);
int logoff(struct channel *, char *);
int logon(struct channel *, struct botuser *, char *, char *, int);
int timed_cmp(struct timed *, struct timed *);
int clonechannel(char *, char *);
int isfriend(struct channel *, char *, char *);
int deltaflags(struct channel *, char *, char);
int isvalidchannel(char *);
int hasuserflags(struct channel *, char *, char *, char *);
int noexpiredlogons(struct channel *);
int delchannelban(struct channel *, char *);
int ischanopless(struct channel *);
int joininprogress(void);
int getmmodecount(struct channel *);
int gettimedcount(void);
int cleanautheds(struct channel *, int);
unsigned int getrandom(unsigned int);
long getfilesize(char *);
struct authed *addnewauthed(struct channel *, time_t, char *, char *);
struct botuser *addbotuser(char *, char *);
struct botuser *addnewadmin(char *, char *);
struct botuser *addnewuser(struct channel *, char *, char *);
struct botuser *usercopy(struct channel *, char *, struct channel *, char *);
struct channel *addnewchannel(char *);
struct channel *getchptr(char *);
struct channelban *addnewchannelban(struct channel *, char *, char *, time_t);
struct chanuser *userjoined(struct channel *, char *, char *, int, int, int, int);
struct chanuser *getcuptr(struct channel *, char *);
struct state *emptystate(void);
struct server *getserver(void);
struct server *addnewserver(char *, int, int, int, int, int, char *);
struct advert *addnewadvert(struct channel *, char *, char *);
struct maskstruct *addnewpermban(struct channel *, char *, char *, char *);
struct maskstruct *addnewnickbk(struct channel *, char *, char *, char *);
struct maskstruct *addnewwordbk(struct channel *, char *, char *, char *);
struct maskstruct *addnewtrusted(char *);
struct maskstruct *addnewservice(char *);
struct script *addnewscript(char *);
struct eventhandler *addneweventhandler(struct script *, char *, SV *);
struct commandhandler *addnewcommandhandler(struct script *, char *, SV *);
struct scripttimer *addnewscripttimer(struct script *, time_t, SV *);
struct maskstruct *addnewkickreason(char *);
struct maskstruct *addnewusermask(struct channel *, char *, char *);
struct maskstruct *addnewadminmask(char *, char *);
struct maskstruct *editmask(struct maskstruct *, char *, char *);
struct maskstruct *isnickbk(struct channel *, char *);
struct maskstruct *iswordbk(struct channel *, char *);
struct pubmsg *addnewpubmsg(struct channel *, time_t, char *, char *, char *);
struct nickchange *addnewnickchange(struct channel *, char *);
struct part *addnewpart(struct channel *, time_t, char *, char *);
struct join *addnewjoin(struct channel *, time_t, char *, char *);
struct topic *addnewtopic(struct channel *, char *, char *);
struct topic *insertnewtopic(struct channel *, char *, char *, int);
struct topic *edittopic(struct channel *, char *, char *, int);
struct chanuser *getrandomuser(struct channel *);
struct ls_wrapstruct *addnew_ls(struct ls_wrapstruct *, int, time_t, char *, char *, char *);
struct timed *timed_new(struct channel *, time_t, int, int, char *);
struct mmode *mmode_new(struct channel *, time_t, char *, char *);
time_t lastoff(struct channel *, char *, char *);
void sendadverts(struct channel *, char *, char *);
void resetparts(struct channel *, char *, char *);
void initpubmsgs(struct channel *);
void freepubmsguser(struct channel *, char *, char *);
void freenickchanges(struct channel *, char *);
void lockchan(struct channel *, char *, int, char *, char *);
void unlockchan(struct channel *, char *, char *);
void initchannel(struct channel *);
void initall(void);
void initnicks(struct channel *);
void initmmodes(struct channel *);
void initmmodesfornick(struct channel *, char *);
void changemmodesfornick(struct channel *, char *, char *);
void initparts(struct channel *);
void initjoins(struct channel *);
void initnickbks(struct channel *);
void initwordbks(struct channel *);
void initpermbans(struct channel *);
void initchannelbans(struct channel *);
void initusers(struct channel *);
void delalltopics(struct channel *);
void swaptopic(struct channel *, int, int);
void gettopic(struct channel *, char *);
void listtopic(struct channel *, char *, int);
void setuserhost(char *, char *);
void sethostquerystatus(char *, int);
void banuser(struct channel *, time_t, char *);
void unbanuser(struct channel *, time_t, char *);
void kickuser(struct channel *, time_t, char *, char *);
void sendreply(char *, int, int, int, char *);
void sendwall(char *, struct channel *, int, char *);
void sendchannelnotice(struct channel *, int, char *);
void listcommand(struct channel *, char *, int, char *, char *, char *);
void addcommand(struct channel *, char *, int, char *, char *, char *);
void delcommand(struct channel *, char *, int, char *, char *, char *);
void editcommand(struct channel *, char *, int, char *, char *, char *);
void extcommand(struct channel *, char *, int, char *, char *, char *);
void logoffcommand(struct channel *, char *, int, char *, char *, char *);
void logoncommand(struct channel *, char *, int, char *, char *, char *);
void isjoined(struct channel *);
void changetobek(struct channel *, char *, int);
void syncvoices(struct channel *);
void syncops(struct channel *);
void syncbans(struct channel *);
void syncnickbks(struct channel *);
void syncuserhosts(void);
void settopic(struct channel *);
void processenv(void);
void processnetjoin(struct channel *);
void mmodes2timeds(void);
void processlock(struct channel *);
void joinchannel(struct channel *);
void updatelast(struct channel *, char *, char *, char *);
void updateseen(struct channel *, char *, char *);
void showlast(struct channel *, char *, int, char *);
void showseen(struct channel *, char *, int, char *);
void setchanmode(struct channel *);
void setchankey(struct channel *, char *);
void setchanlimit(struct channel *, char *);
void channelsync(struct channel *);
void cleartobeks(struct channel *);
void getnthmode(char *, int, char *);
void massmessage(char *, char *);
void flushall(void);
void quit(void);
void parsectcp(char *, char *, int, char *);
void nstats(struct channel *, char *, int, char *);
void wall(struct channel *, char *, char *, char *);
void listchanusers(void);
void timed_del(struct timed *);
void mmode_del(struct channel *, struct mmode *);
void validateflags(char *, char *, int);
void resetfloodcounters(struct channel *, char *);
void resetcommand(struct channel *, char *, int, char *, char *, char *);
void deltopic(struct channel *, char *, int, char *, char *, char *);

void listchanusers()
{
#ifdef OER_DEBUG
        struct channel *this;
        struct chanuser *cu;

	oer_debug(OER_DEBUG_NOISE, "listchanusers()\n");

        for(this = mystate->channels; this != NULL; this = this->next) {
		oer_debug(OER_DEBUG_INFO, "there are %d users on %s%s\n", this->nickcount, this->name, this->i_am_op ? "" : " *");
		if(do_debug >= OER_DEBUG_NOISE) {
			/* optimization */
			cu = this->nicks;
			while(cu != NULL) {
				oer_debug(OER_DEBUG_NOISE, "channel: %s, nick: %s, host: %s, oper: %s, op: %s, voice: %s\n", this->name, cu->nick, (cu->userhost == NULL) ? "(null)" : cu->userhost, (cu->ircop) ? "YES" : "NO", (cu->chanop) ? "YES" : "NO", (cu->voice) ? "YES" : "NO");
				cu = cu->next;
			}
		}
	}
#endif
}

void quit(void)
{
	struct script *s;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "quit()\n");
#endif
	for(s = mystate->scripts; s != NULL; s = s->next) {
		oerperl_deletescript(s);
	}
	if(mystate->current_server == NULL || !mystate->current_server->registered.done) {
		close(mystate->sockfd);
		exit(EXIT_SUCCESS);
	}
	snprintf(timed_str, WRITE_BUFFER_LENGTH, "QUIT :%s", mystate->signoff);
	timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CHANNEL_HANDLING, timed_str);
	mystate->quitting = 1;
}

void flushall(void)
{
	struct channel *this;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "flushall()\n");
#endif
	for(this = mystate->channels; this != NULL; this = this->next) {
		if(this->seen != NULL) {
			if(fflush(this->seen) == EOF) {
#ifdef OER_DEBUG
				oer_debug(OER_DEBUG_ERROR, "flushall->fflush() failed: %s\n", strerror(errno));
#endif
			}
		}
		if(this->last != NULL) {
			if(fflush(this->last) == EOF) {
#ifdef OER_DEBUG
				oer_debug(OER_DEBUG_ERROR, "flushall->fflush() failed: %s\n", strerror(errno));
#endif
			}
		}
	}
	if(mystate->logfp != NULL) {
		if(fflush(mystate->logfp) == EOF) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_ERROR, "flushall->fflush() failed: %s\n", strerror(errno));
#endif
		}
	}
	if(mystate->logto != NULL) {
		if(fflush(mystate->logto) == EOF) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_ERROR, "flushall->fflush() failed: %s\n", strerror(errno));
#endif
		}
	}
}

int uptime(time_t *uptime, time_t *idle)
{
	char upt_str[TINYSTRINGLEN + 1];
	char idl_str[TINYSTRINGLEN + 1];
	FILE *fp;
        char stringbuffer[STRINGLEN + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "uptime()\n");
#endif
	/* read uptime & idle from /proc */
        if((fp = fopen("/proc/uptime", "r")) == NULL) {
		return 0;
	}
        while(fgets(stringbuffer, STRINGLEN, fp) != NULL) {
                sscanf(stringbuffer, "%s %s", upt_str, idl_str);
        }
	fclose(fp);
	*uptime = atol(upt_str);
	*idle = atol(idl_str);
	return 1;
}

int delpermbans(struct channel *this, char *mask, int force)
{
	int deleted;
	struct maskstruct *ms;
	struct maskstruct *ms2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "delpermbans(\"%s\", \"%s\", %d)\n", this->name, mask, force);
#endif
	/* find & remove mask */
	ms = this->permbans;
	deleted = 0;
	while(ms != NULL) {
		ms2 = ms->next;
		if(force || !strcasecmp(ms->mask, mask)) {
			if(ms->prev == NULL) {
				/* first mask */
				if(ms->next == NULL) {
					/* only mask */
					this->permbans = NULL;
				} else {
					ms->next->prev = NULL;
					this->permbans = ms->next;
				}
			} else if(ms->next == NULL) {
				/* last mask */
				ms->prev->next = NULL;
			} else {
				/* between 2 or more masks */
				ms->prev->next = ms->next;
				ms->next->prev = ms->prev;
			}
			if(force && this->i_am_op) {
				mmode_new(this, mystate->now, "-b", ms->mask);
			}
			free(ms->mask);
			free(ms->optstring2);
			free(ms->optstring1);
			free(ms);
			deleted++;
		}
		ms = ms2;
	}
	return deleted;
}

int delkickreason(char *kickreason)
{
	struct maskstruct *ms;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "delkickreason(\"%s\")\n", kickreason);
#endif
	/* find & remove mask */
	for(ms = mystate->kickreasons; ms != NULL; ms = ms->next) {
		if(!strcasecmp(ms->mask, kickreason)) {
			break;
		}
	}
	if(ms == NULL) {
		return 0;
	}
	if(ms->prev == NULL) {
                /* first */
                if(ms->next == NULL) {
                        /* only */
                        mystate->kickreasons = NULL;
                } else {
                        ms->next->prev = NULL;
                        mystate->kickreasons = ms->next;
                }
        } else if(ms->next == NULL) {
                /* last */
                ms->prev->next = NULL;
        } else {
                /* between 2 or more */
                ms->prev->next = ms->next;
                ms->next->prev = ms->prev;
        }
        free(ms->mask);
        free(ms);
	mystate->kickrs--;
        return 1;
}

int delserver(char *serverhost, int serverport, int servermodes, int pingfrequency, int protected_ircops, int linenoise)
{
	struct server *s;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "delserver(\"%s\", %d, %d, %d, %d, %d)\n", serverhost, serverport, servermodes, pingfrequency, protected_ircops, linenoise);
#endif
	/* find & remove server */
	for(s = mystate->servers; s != NULL; s = s->next) {
                if(!strcasecmp(s->serverhost, serverhost) && s->serverport == serverport && s->servermodes == servermodes && s->pingfrequency == pingfrequency && s->protected_ircops == protected_ircops && s->linenoise == linenoise) {
			break;
                }
        }
	if(s == NULL) {
		/* no such server */
		return 0;
	}
	if(s == mystate->current_server) {
		/* we won't remove the current server */
		return 0;
	}
	if(s->prev == NULL && s->next != NULL) {
                /* first server, we won't remove the only one */
		s->next->prev = NULL;
		mystate->servers = s->next;
        } else if(s->next == NULL) {
                /* last server */
                s->prev->next = NULL;
        } else {
                /* between 2 or more servers */
                s->prev->next = s->next;
                s->next->prev = s->prev;
        }
        free(s);
	return 1;
}

int delnickbks(struct channel *this, char *mask, int force)
{
	int deleted;
	struct maskstruct *ms;
	struct maskstruct *ms2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "delnickbks(\"%s\", \"%s\", %d)\n", this->name, mask, force);
#endif
	/* find & remove mask */
	ms = this->nickbks;
	deleted = 0;
	while(ms != NULL) {
                ms2 = ms->next;
                if(force || !strcasecmp(ms->mask, mask)) {
                        if(ms->prev == NULL) {
                                /* first mask */
                                if(ms->next == NULL) {
                                        /* only mask */
                                        this->nickbks = NULL;
                                } else {
                                        ms->next->prev = NULL;
                                        this->nickbks = ms->next;
                                }
                        } else if(ms->next == NULL) {
                                /* last mask */
                                ms->prev->next = NULL;
                        } else {
                                /* between 2 or more masks */
                                ms->prev->next = ms->next;
                                ms->next->prev = ms->prev;
                        }
                        free(ms->mask);
                        free(ms->optstring2);
                        free(ms->optstring1);
                        free(ms);
                        deleted++;
                }
                ms = ms2;
        }
	return deleted;
}

int delwordbks(struct channel *this, char *mask, int force) 
{
	int deleted;
	struct maskstruct *ms;
	struct maskstruct *ms2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "delwordbks(\"%s\", \"%s\", %d)\n", this->name, mask, force);
#endif
	/* find & remove mask */
	ms = this->wordbks;
	deleted = 0;
	while(ms != NULL) {
                ms2 = ms->next;
                if(force || !strcasecmp(ms->mask, mask)) {
                        if(ms->prev == NULL) {
                                /* first mask */
                                if(ms->next == NULL) {
                                        /* only mask */
                                        this->wordbks = NULL;
                                } else {
                                        ms->next->prev = NULL;
                                        this->wordbks = ms->next;
                                }
                        } else if(ms->next == NULL) {
                                /* last mask */
                                ms->prev->next = NULL;
                        } else {
                                /* between 2 or more masks */
                                ms->prev->next = ms->next;
                                ms->next->prev = ms->prev;
                        }
                        free(ms->mask);
                        free(ms->optstring2);
                        free(ms->optstring1);
                        free(ms);
                        deleted++;
                }
                ms = ms2;
        }
	return deleted;
}

int getondiskmsgcount(struct channel *this, char *thisnick)
{
	int i;
	int msgcount;
	int len;
	char outstring[TXT_BUFFER + 1];
	char nick[NICKLEN + 1];
	char *p;
	char *q;
	FILE *fp;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "getondiskmsgcount(\"%s\", \"%s\")\n", this->name, thisnick);
#endif
	if(this->last != NULL) {
		if(fflush(this->last) == EOF) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_ERROR, "getondiskmsgcount->fflush() failed: %s\n", strerror(errno));
#endif
		}
	}
	if(strlen(mystate->altloggingdir)) {
		snprintf(outstring, HUGESTRINGLEN, "%s/last-%s", mystate->altloggingdir, this->name);
	} else {
		snprintf(outstring, HUGESTRINGLEN, "last-%s", this->name);
	}
	if((fp = fopen(outstring, "r")) == NULL) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "getondiskmsgcount->failed to open %s: %s\n", outstring, strerror(errno));
#endif
		return -1;
	}
	msgcount = 0;
	while(fgets(outstring, TXT_BUFFER, fp)) {
		p = outstring;
		/* striplf() */
		for(i = strlen(p) - 1; isspace((int)p[i]) && i >= 0; i--) {
			p[i] = '\0';
		}
		/* first field is the timestamp, skip to next */
		for(;*p != '\0'; p++) {
			if(!strncasecmp(p, OER_DEFAULT_DELIM, OER_DEFAULT_DELIM_LEN)) {
				p += 2;
				break;
			}
		}
		if(*p == '\0') {
			continue;
		}
		q = p;
		/* 2nd field is the nick */
		for(;*p != '\0'; p++) {
			if(!strncasecmp(p, OER_DEFAULT_DELIM, OER_DEFAULT_DELIM_LEN)) {
				p += 2;
				break;
			}
		}
		if(*p == '\0') {
			continue;
		}
		len = (p - q) - OER_DEFAULT_DELIM_LEN;
		len = (len > NICKLEN) ? NICKLEN : len;
		strncpy(nick, q, len);
		nick[len] = '\0';
		if(!strcasecmp(thisnick, nick)) {
			msgcount++;
		}
	}
	fclose(fp);
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "getondiskmsgcount->returning %d\n", msgcount);
#endif
	return msgcount;
}

int getondiskjoincount(struct channel *this, char *thisnick)
{
	int i;
	int joincount;
	int len;
	char outstring[TXT_BUFFER + 1];
	char nick[NICKLEN + 1];
	char *p;
	char *q;
	FILE *fp;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "getondiskjoincount(\"%s\", \"%s\")\n", this->name, thisnick);
#endif
	if(this->seen != NULL) {
		if(fflush(this->seen) == EOF) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_ERROR, "getondiskjoincount->fflush() failed: %s\n", strerror(errno));
#endif
		}
	}
	if(strlen(mystate->altloggingdir)) {
		snprintf(outstring, HUGESTRINGLEN, "%s/seen-%s", mystate->altloggingdir, this->name);
	} else {
		snprintf(outstring, HUGESTRINGLEN, "seen-%s", this->name);
	}
	if((fp = fopen(outstring, "r")) == NULL) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "getondiskjoincount->failed to open %s: %s\n", outstring, strerror(errno));
#endif
		return -1;
	}
	joincount = 0;
	while(fgets(outstring, TXT_BUFFER, fp)) {
		p = outstring;
		/* striplf() */
		for(i = strlen(p) - 1; isspace((int)p[i]) && i >= 0; i--) {
			p[i] = '\0';
		}
		/* first field is the timestamp, skip to next */
		for(;*p != '\0'; p++) {
			if(!strncasecmp(p, OER_DEFAULT_DELIM, OER_DEFAULT_DELIM_LEN)) {
				p += 2;
				break;
			}
		}
		if(*p == '\0') {
			continue;
		}
		q = p;
		/* 2nd field is the nick */
		for(;*p != '\0'; p++) {
			if(!strncasecmp(p, OER_DEFAULT_DELIM, OER_DEFAULT_DELIM_LEN)) {
				p += 2;
				break;
			}
		}
		if(*p == '\0') {
			continue;
		}
		len = (p - q) - OER_DEFAULT_DELIM_LEN;
		len = (len > NICKLEN) ? NICKLEN : len;
		strncpy(nick, q, len);
		nick[len] = '\0';
		if(!strcasecmp(thisnick, nick)) {
			joincount++;
		}
	}
	fclose(fp);
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "getondiskjoincount->returning %d\n", joincount);
#endif
	return joincount;
}

int getrandommsg(struct channel *this, char *thisnick, int nicklen, int which, char *msgstore, int msgstorelen)
{
	int i;
	int pos;
	int strpos;
	int len;
	int rc;
	char outstring[TXT_BUFFER + 1];
	char nick[NICKLEN + 1];
	char message[HUGESTRINGLEN + 1];
	char *p;
	char *q;
	FILE *fp;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "getrandommsg(\"%s\", \"%s\", %d, %d, %d)\n", this->name, thisnick, nicklen, which, msgstorelen);
#endif
	if(this->last != NULL) {
		if(fflush(this->last) == EOF) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_ERROR, "getrandommsg->fflush() failed: %s\n", strerror(errno));
#endif
		}
	}
	if(strlen(mystate->altloggingdir)) {
		snprintf(outstring, TXT_BUFFER, "%s/last-%s", mystate->altloggingdir, this->name);
	} else {
		snprintf(outstring, TXT_BUFFER, "last-%s", this->name);
	}
	if((fp = fopen(outstring, "r")) == NULL) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "getrandommsg->failed to open %s: %s\n", outstring, strerror(errno));
#endif
		return -1;
	}
	pos = 0;
	rc = -1;
	while(fgets(outstring, TXT_BUFFER, fp)) {
		p = outstring;
		/* striplf() */
		for(i = strlen(p) - 1; isspace((int)p[i]) && i >= 0; i--) {
			p[i] = '\0';
		}
		/* first field is the timestamp, skip to next */
		for(;*p != '\0'; p++) {
			if(!strncasecmp(p, OER_DEFAULT_DELIM, OER_DEFAULT_DELIM_LEN)) {
				p += 2;
				break;
			}
		}
		if(*p == '\0') {
			continue;
		}
		q = p;
		/* 2nd field is the nick */
		for(;*p != '\0'; p++) {
			if(!strncasecmp(p, OER_DEFAULT_DELIM, OER_DEFAULT_DELIM_LEN)) {
				p += 2;
				break;
			}
		}
		if(*p == '\0') {
			continue;
		}
		len = (p - q) - OER_DEFAULT_DELIM_LEN;
		len = (len > NICKLEN) ? NICKLEN : len;
		strncpy(nick, q, len);
		nick[len] = '\0';
		if(strcasecmp(thisnick, nick)) {
			/* not the nick we are after */
			continue;
		}
		pos++;
		if(pos < which) {
			/* not the msg we are after */
			continue;
		}
		/* 3rd field is the host */
		for(;*p != '\0'; p++) {
			if(!strncasecmp(p, OER_DEFAULT_DELIM, OER_DEFAULT_DELIM_LEN)) {
				p += 2;
				break;
			}
		}
		if(*p == '\0') {
			continue;
		}
		/* 4th field is the message */
		strpos = 0;
		while(*p != '\0' && strpos < HUGESTRINGLEN) {
			message[strpos++] = *p++;
		}
		message[strpos] = '\0';
		strncpy(thisnick, nick, nicklen);
		strncpy(msgstore, message, msgstorelen);
		rc = 0;
		break;
	}
	fclose(fp);
	return rc;
}

void showlast(struct channel *this, char *to, int tochan, char *commandline)
{
	int i;
	int strpos;
	int len;
	int ppos;
	int params;
	int last_show;
	char outstring[TXT_BUFFER + 1];
	char pattern[NICKLEN + 1];
	char at[NICKLEN + 1];
	char ts[TINYSTRINGLEN + 1];
	char nick[NICKLEN + 1];
	char host[USERHOSTLEN + 1];
	char message[HUGESTRINGLEN + 1];
        char p1[STRINGLEN + 1];
	char *p;
	char *q;
	struct ls_wrapstruct *first;
	struct ls_wrapstruct *ls;
	struct ls_wrapstruct *ls2;
	FILE *fp;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "showlast(\"%s\", \"%s\", %d, \"%s\")\n", this->name, to, tochan, commandline);
#endif
	last_show = OER_LAST;
	strncpy(pattern, "*", NICKLEN);
	params = wordcount(commandline);
	switch(params) {
		/* case 0 is default */
	case 1:
		ppos = 0;
		if((ppos = parse(commandline, ppos, " ", p1, STRINGLEN, 0)) < 0) {
			return;
		}
		strncpy(pattern, p1, NICKLEN);
		last_show = OER_LAST;
		break;
	case 2:
		ppos = 0;
		if((ppos = parse(commandline, ppos, " ", p1, STRINGLEN, 0)) < 0) {
			return;
		}
		strncpy(pattern, p1, NICKLEN);
		if((ppos = parse(commandline, ppos, " ", p1, STRINGLEN, 0)) < 0) {
			return;
		}
		last_show = atoi(p1);
		if(!isnumbw(last_show, OER_LIMIT_SHOWLAST_MIN, OER_LIMIT_SHOWLAST_MAX)) {
			return;
		}
		break;
	}
	if(this->last != NULL) {
		if(fflush(this->last) == EOF) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_ERROR, "showlast->fflush() failed: %s\n", strerror(errno));
#endif
		}
	}
	if(strlen(mystate->altloggingdir)) {
		snprintf(outstring, TXT_BUFFER, "%s/last-%s", mystate->altloggingdir, this->name);
	} else {
		snprintf(outstring, TXT_BUFFER, "last-%s", this->name);
	}
	if((fp = fopen(outstring, "r")) == NULL) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "showlast->failed to open %s: %s\n", outstring, strerror(errno));
#endif
		return;
	}
        first = NULL;
	while(fgets(outstring, TXT_BUFFER, fp)) {
		p = outstring;
		/* striplf() */
		for(i = strlen(p) - 1; isspace((int)p[i]) && i >= 0; i--) {
			p[i] = '\0';
		}
		/* first field is the timestamp */
		q = p;
		for(;*p != '\0'; p++) {
			if(!strncasecmp(p, OER_DEFAULT_DELIM, OER_DEFAULT_DELIM_LEN)) {
				p += 2;
				break;
			}
		}
		if(*p == '\0') {
			continue;
		}
		len = (p - q) - OER_DEFAULT_DELIM_LEN;
		len = (len > NICKLEN) ? NICKLEN : len;
		strncpy(at, q, len);
		at[len] = '\0';
		/* 2nd field is the nick */
		q = p;
		for(;*p != '\0'; p++) {
			if(!strncasecmp(p, OER_DEFAULT_DELIM, OER_DEFAULT_DELIM_LEN)) {
				p += 2;
				break;
			}
		}
		if(*p == '\0') {
			continue;
		}
		len = (p - q) - OER_DEFAULT_DELIM_LEN;
		len = (len > NICKLEN) ? NICKLEN : len;
		strncpy(nick, q, len);
		nick[len] = '\0';
		if(!wild_match(pattern, nick)) {
			/* not the nick we are after */
			continue;
		}
		/* 3rd field is the host */
		q = p;
		for(;*p != '\0'; p++) {
			if(!strncasecmp(p, OER_DEFAULT_DELIM, OER_DEFAULT_DELIM_LEN)) {
				p += 2;
				break;
			}
		}
		if(*p == '\0') {
			continue;
		}
		len = (p - q) - OER_DEFAULT_DELIM_LEN;
		len = (len > USERHOSTLEN) ? USERHOSTLEN : len;
		strncpy(host, q, len);
		host[len] = '\0';
		/* last field is the message */
		strpos = 0;
		while(*p != '\0' && strpos < HUGESTRINGLEN) {
			message[strpos++] = *p++;
		}
		message[strpos] = '\0';
		first = addnew_ls(first, last_show, atoi(at), nick, host, message);
	}
	fclose(fp);
	ls = first;
	while(ls != NULL) {
		strncpy(ts, ctime(&ls->at), TINYSTRINGLEN);
		striplf(ts);
		tzset();
		memset(outstring, 0, TXT_BUFFER + 1);
		snprintf(outstring, HUGESTRINGLEN, "\"%s\" (%s, %s, %s %s %s)", ls->str3, ls->str1, ls->str2, ts, tzname[0], tzname[1]);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, outstring);
		ls2 = ls->next;
		free(ls->str1);
		free(ls->str2);
		free(ls->str3);
		free(ls);
		ls = ls2;
	}
}

void updatelast(struct channel *this, char *nick, char *userhost, char *message)
{
	char outstring[WRITE_BUFFER_LENGTH + 1];

	if(this->last == NULL) {
		return;
	}
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "updatelast(\"%s\", \"%s\", \"%s\", \"%s\")\n", this->name, nick, userhost, message);
#endif
	snprintf(outstring, WRITE_BUFFER_LENGTH, "%lu::%s::%s::%s\n", mystate->now, nick, userhost, message);
	if(fputs(outstring, this->last) == EOF) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_ERROR, "updatelast->fputs() failed: %s\n", strerror(errno));
#endif
	}
	if(this->lastcounter >= OER_FORCE_FLUSH) {
		if(fflush(this->last) == EOF) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_ERROR, "updatelast->fflush() failed: %s\n", strerror(errno));
#endif
		}
		this->lastcounter = 0;
	}
}

void showseen(struct channel *this, char *to, int tochan, char *commandline)
{
	int i;
	int strpos;
	int len;
	int ppos;
	int params;
	int seen_show;
	char outstring[TXT_BUFFER + 1];
	char pattern[NICKLEN + 1];
	char at[NICKLEN + 1];
	char ts[TINYSTRINGLEN + 1];
	char nick[NICKLEN + 1];
	char host[USERHOSTLEN + 1];
        char p1[STRINGLEN + 1];
	char *p;
	char *q;
	struct ls_wrapstruct *first;
	struct ls_wrapstruct *ls;
	struct ls_wrapstruct *ls2;
	FILE *fp;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "showseen(\"%s\", \"%s\", %d, \"%s\")\n", this->name, to, tochan, commandline);
#endif
	seen_show = OER_SEEN;
	strncpy(pattern, "*", NICKLEN);
	params = wordcount(commandline);
	switch(params) {
		/* case 0 is default */
	case 1:
		ppos = 0;
		if((ppos = parse(commandline, ppos, " ", p1, STRINGLEN, 0)) < 0) {
			return;
		}
		strncpy(pattern, p1, NICKLEN);
		seen_show = OER_SEEN;
		break;
	case 2:
		ppos = 0;
		if((ppos = parse(commandline, ppos, " ", p1, STRINGLEN, 0)) < 0) {
			return;
		}
		strncpy(pattern, p1, NICKLEN);
		if((ppos = parse(commandline, ppos, " ", p1, STRINGLEN, 0)) < 0) {
			return;
		}
		seen_show = atoi(p1);
		if(!isnumbw(seen_show, OER_LIMIT_SHOWSEEN_MIN, OER_LIMIT_SHOWSEEN_MAX)) {
                        return;
                }
		break;
	}
	if(this->seen != NULL) {
		if(fflush(this->seen) == EOF) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_ERROR, "showseen->fflush() failed: %s\n", strerror(errno));
#endif
		}
	}
	if(strlen(mystate->altloggingdir)) {
		snprintf(outstring, BIGSTRINGLEN, "%s/seen-%s", mystate->altloggingdir, this->name);
	} else {
		snprintf(outstring, BIGSTRINGLEN, "seen-%s", this->name);
	}
	if((fp = fopen(outstring, "r")) == NULL) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "showseen->failed to open %s: %s\n", outstring, strerror(errno));
#endif
		return;
	}
        first = NULL;
	while(fgets(outstring, TXT_BUFFER, fp)) {
		p = outstring;
		/* striplf() */
		for(i = strlen(p) - 1; isspace((int)p[i]) && i >= 0; i--) {
			p[i] = '\0';
		}
		/* first field is the timestamp */
		q = p;
		for(;*p != '\0'; p++) {
			if(!strncasecmp(p, OER_DEFAULT_DELIM, OER_DEFAULT_DELIM_LEN)) {
				p += 2;
				break;
			}
		}
		if(*p == '\0') {
			continue;
		}
		len = (p - q) - OER_DEFAULT_DELIM_LEN;
		len = (len > NICKLEN) ? NICKLEN : len;
		strncpy(at, q, len);
		at[len] = '\0';
		/* 2nd field is the nick */
		q = p;
		for(;*p != '\0'; p++) {
			if(!strncasecmp(p, OER_DEFAULT_DELIM, OER_DEFAULT_DELIM_LEN)) {
				p += 2;
				break;
			}
		}
		if(*p == '\0') {
			continue;
		}
		len = (p - q) - OER_DEFAULT_DELIM_LEN;
		len = (len > NICKLEN) ? NICKLEN : len;
		strncpy(nick, q, len);
		nick[len] = '\0';
		if(!wild_match(pattern, nick)) {
			/* not the nick we are after */
			continue;
		}
		/* last field is the host */
		strpos = 0;
		while(*p != '\0' && strpos < USERHOSTLEN) {
			host[strpos++] = *p++;
		}
		host[strpos] = '\0';
		first = addnew_ls(first, seen_show, atoi(at), nick, host, NULL);
	}
	fclose(fp);
	ls = first;
	while(ls != NULL) {
		strncpy(ts, ctime(&ls->at), TINYSTRINGLEN);
		striplf(ts);
		tzset();
		snprintf(outstring, HUGESTRINGLEN, "I saw %s with host %s last %s %s %s", ls->str1, ls->str2, ts, tzname[0], tzname[1]);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, outstring);
		ls2 = ls->next;
		free(ls->str1);
		free(ls->str2);
		free(ls->str3);
		free(ls);
		ls = ls2;
	}
}

void updateseen(struct channel *this, char *nick, char *userhost)
{
	char outstring[BIGSTRINGLEN + 1];

	if(this->seen == NULL) {
		return;
	}
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "updateseen(\"%s\", \"%s\", \"%s\")\n", this->name, nick, userhost);
#endif
	snprintf(outstring, BIGSTRINGLEN, "%lu::%s::%s\n", mystate->now, nick, userhost);
	if(fputs(outstring, this->seen) == EOF) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_ERROR, "updateseen->fputs() failed: %s\n", strerror(errno));
#endif
	}
	if(this->seencounter >= OER_FORCE_FLUSH) {
		if(fflush(this->seen) == EOF) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_ERROR, "updateseen->fflush() failed: %s\n", strerror(errno));
#endif
		}
		this->seencounter = 0;
	}
}

void nstats(struct channel *this, char *to, int tochan, char *nick)
{
	int msgs;
	int joins;
	char outstring[BIGSTRINGLEN + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "nstats(\"%s\", \"%s\", %d, \"%s\")\n", this->name, to, tochan, nick);
#endif
	msgs = getondiskmsgcount(this, nick);
	joins = getondiskjoincount(this, nick);
	if(!msgs && !joins) {
		return;
	}
	snprintf(outstring, BIGSTRINGLEN, "I have seen %d joins and %d messages from %s on channel %s", joins, msgs, nick, this->name);
	sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, outstring);
}

int checkforpartrejoin(struct channel *this, char *nick, char *userhost)
{
	int parts;
        char ban[USERHOSTLEN + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "checkforpartrejoin(\"%s\", \"%s\", \"%s\")\n", this->name, nick, userhost);
#endif
	if(!haschanflags(this, "r")) {
                return 0;
        }
	if(isq(nick)) {
		return 0;
	}
        if(isadmin(this, userhost)) {
                return 0;
        }
        if(isop(this, userhost)) {
                return 0;
        }
        if(!haschanflags(this, "N") && isopnow(this, nick)) {
                return 0;
        }
	if(haschanflags(this, "F") && isfriend(this, nick, userhost)) {
		return 0;
	}
	parts = partcount(this, nick, userhost);
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_INFO, "checkforpartrejoin->credited parts on %s for %s!%s: %d\n", this->name, nick, userhost, parts);
#endif
        if(parts < OER_ALLOWED_PARTS) {
                return 0;
        }
        strncpy(ban, userhost, USERHOSTLEN);
        if(safeban(this, ban, nick) == NULL) {
                return 0;
        }
	mmode_new(this, mystate->now - 1, "+b", ban);
        snprintf(timed_str, WRITE_BUFFER_LENGTH, "KICK %s %s :part-rejoin protection triggered, come back when you have made up your mind", this->name, nick);
        timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CHANNEL_PROTECTION, timed_str);
	if(this->banvars.part_rejoin > 0) {
		mmode_new(this, mystate->now + this->banvars.part_rejoin, "-b", ban);
	} else {
		mmode_new(this, mystate->now + OER_BANVARS_PART_REJOIN, "-b", ban);
	}
        return 1;
}

int checkforautorejoin(struct channel *this, char *nick, char *userhost)
{
        char ban[USERHOSTLEN + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "checkforautorejoin(\"%s\", \"%s\", \"%s\")\n", this->name, nick, userhost);
#endif
        if(!haschanflags(this, "r")) {
                return 0;
        }
	if(isq(nick)) {
                return 0;
        }
        if(isadmin(this, userhost)) {
                return 0;
        }
        if(isop(this, userhost)) {
                return 0;
        }
        if(!haschanflags(this, "N") && isopnow(this, nick)) {
                return 0;
        }
        if(haschanflags(this, "F") && isfriend(this, nick, userhost)) {
		return 0;
	}
        if(lastoff(this, nick, userhost) > OER_AUTO_REJOIN_TIME) {
                return 0;
        }
        strncpy(ban, userhost, USERHOSTLEN);
        if(safeban(this, ban, nick) == NULL) {
                return 0;
        }
	mmode_new(this, mystate->now - 1, "+b", ban);
        snprintf(timed_str, WRITE_BUFFER_LENGTH, "KICK %s %s :auto-rejoin, your prize: a %d second ban", this->name, nick, (this->banvars.auto_rejoin > 0) ? this->banvars.auto_rejoin : OER_BANVARS_AUTO_REJOIN);
        timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CHANNEL_PROTECTION, timed_str);
	if(this->banvars.auto_rejoin > 0) {
		mmode_new(this, mystate->now + this->banvars.auto_rejoin, "-b", ban);
	} else {
		mmode_new(this, mystate->now + OER_BANVARS_AUTO_REJOIN, "-b", ban);
	}
        return 1;
}

int checkforansi(struct channel *this, char *nick, char *userhost, char *message)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "checkforansi(\"%s\", \"%s\", \"%s\")\n", this->name, nick, userhost);
#endif
        if(!haschanflags(this, "a")) {
                return 0;
        }
	if(isq(nick)) {
                return 0;
        }
        if(isadmin(this, userhost)) {
                return 0;
        }
        if(isop(this, userhost)) {
                return 0;
        }
        if(!haschanflags(this, "N") && isopnow(this, nick)) {
                return 0;
        }
        if(haschanflags(this, "F") && isfriend(this, nick, userhost)) {
		return 0;
	}
        if(!isansi(message)) {
                return 0;
        }
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_INFO, "checkforansi->ansi/control-codes detected on %s from %s\n", this->name, nick);
#endif
	if(isonchan(this, nick)) {
		snprintf(timed_str, WRITE_BUFFER_LENGTH, "KICK %s %s :mIRC or non-standard control codes not permitted on this channel", this->name, nick);
		timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CHANNEL_PROTECTION, timed_str);
	}
        return 1;
}

int checkforflood(struct channel *this, char *nick, char *userhost, char *message)
{
        int floodtype;
        char ban[USERHOSTLEN + 1];
        char outstring[STRINGLEN + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "checkforflood(\"%s\", \"%s\", \"%s\", \"%s\")\n", this->name, nick, userhost, message);
#endif
        if(!haschanflags(this, "f")) {
                return 0;
        }
	if(isq(nick)) {
                return 0;
        }
        if(isadmin(this, userhost)) {
                return 0;
        }
        if(!haschanflags(this, "G")) {
		if(isop(this, userhost)) {
			return 0;
		}
		if(!haschanflags(this, "N") && isopnow(this, nick)) {
			return 0;
		}
		if(haschanflags(this, "F") && isfriend(this, nick, userhost)) {
			return 0;
		}
	}
        floodtype = isflood(this, nick, userhost, message);
        if(floodtype != OER_PUBMSG_FLOOD_NORMAL && floodtype != OER_PUBMSG_FLOOD_REPEAT) {
                return 0;
        }
        strncpy(ban, userhost, USERHOSTLEN);
        if(safeban(this, ban, nick) == NULL) {
                return 0;
        }
        switch(floodtype) {
        case OER_PUBMSG_FLOOD_NORMAL:
		if(haschanflags(this, "G") && isopnow(this, nick) && this->banvars.public_flood > 0 && isonchan(this, nick)) {
			mmode_new(this, mystate->now, "-o", nick);
			mmode_new(this, mystate->now + this->banvars.public_flood + 1, "+o", nick);
		}
		if(isvoicenow(this, nick) && this->banvars.public_flood > 0 && isonchan(this, nick)) {
			mmode_new(this, mystate->now, "-v", nick);
			mmode_new(this, mystate->now + this->banvars.public_flood + 1, "+v", nick);
		}
		if(this->banvars.public_flood > 0) {
			mmode_new(this, mystate->now, "+b", ban);
			if(isonchan(this, nick)) {
				snprintf(outstring, STRINGLEN, "flood protection triggered, you can talk again in %d seconds", this->banvars.public_flood);
				sendreply(nick, 0, 0, OER_TIMED_PRIORITY_NORMAL, outstring);
			}
			mmode_new(this, mystate->now + this->banvars.public_flood, "-b", ban);
		}
		resetfloodcounters(this, nick);
		freepubmsguser(this, nick, userhost);
                return 1;
        case OER_PUBMSG_FLOOD_REPEAT:
		if(haschanflags(this, "G") && isopnow(this, nick) && isonchan(this, nick)) {
			mmode_new(this, mystate->now, "-o", nick);
		}
		if(this->banvars.public_flood_repeat > 0) {
			mmode_new(this, mystate->now - 1, "+b", ban);
		}
		if(isonchan(this, nick)) {
			snprintf(timed_str, WRITE_BUFFER_LENGTH, "KICK %s %s :bad choice of channel for your repeating fetish", this->name, nick);
			timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CHANNEL_PROTECTION, timed_str);
			changetobek(this, nick, 1);
		}
		if(this->banvars.public_flood_repeat > 0) {
			mmode_new(this, mystate->now + this->banvars.public_flood_repeat, "-b", ban);
		}
		resetfloodcounters(this, nick);
		freepubmsguser(this, nick, userhost);
                return 1;
        case OER_PUBMSG_FLOOD_NONE:
                return 0;
        }
        return 0;
}

int checkfornickflood(struct channel *this, char *nick, char *userhost)
{
        int changes;
        char ban[USERHOSTLEN + 1];
	struct nickchange *nc1;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "checkfornickflood(\"%s\", \"%s\", \"%s\")\n", this->name, nick, userhost);
#endif
        if(!haschanflags(this, "k")) {
                return 0;
        }
	if(isq(nick)) {
                return 0;
        }
        if(isadmin(this, userhost)) {
                return 0;
        }
        if(!haschanflags(this, "G")) {
		if(isop(this, userhost)) {
			return 0;
		}
		if(!haschanflags(this, "N") && isopnow(this, nick)) {
			return 0;
		}
		if(haschanflags(this, "F") && isfriend(this, nick, userhost)) {
			return 0;
		}
	}
	for(nc1 = this->nickchanges, changes = 0; nc1 != NULL; nc1 = nc1->next) {
		if((mystate->now - nc1->at) > this->floodvars.nickflood_expire) {
			continue;
		}
		if(!strcasecmp(nc1->userhost, userhost)) {
			changes++;
		}
	}
	if(changes < this->floodvars.nickflood_changes) {
		return 0;
	}
        strncpy(ban, userhost, USERHOSTLEN);
        if(safeban(this, ban, nick) == NULL) {
                return 0;
        }
	if(this->banvars.public_flood_repeat > 0) {
		mmode_new(this, mystate->now - 1, "+b", ban);
	}
	snprintf(timed_str, WRITE_BUFFER_LENGTH, "KICK %s %s :bad choice of channel to have a identity crisis", this->name, nick);
	timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CHANNEL_PROTECTION, timed_str);
	if(this->banvars.public_flood_repeat > 0) {
		mmode_new(this, mystate->now + this->banvars.public_flood_repeat, "-b", ban);
	}
	changetobek(this, nick, 1);
	freenickchanges(this, userhost);
	return 1;
}

int checkforbadword(struct channel *this, char *nick, char *userhost, char *message)
{
        char ban[USERHOSTLEN + 1];
	char outstring1[STRINGLEN + 1];
	char outstring2[STRINGLEN + 1];
	struct maskstruct *ms;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "checkforbadword(\"%s\", \"%s\", \"%s\", \"%s\")\n", this->name, nick, userhost, message);
#endif
        if(!haschanflags(this, "w")) {
                return 0;
        }
	if(isq(nick)) {
                return 0;
        }
        if(isadmin(this, userhost)) {
                return 0;
        }
        if(isop(this, userhost)) {
                return 0;
        }
        if(!haschanflags(this, "N") && isopnow(this, nick)) {
                return 0;
        }
        if(haschanflags(this, "F") && isfriend(this, nick, userhost)) {
		return 0;
	}
        if((ms = iswordbk(this, message)) == NULL) {
                return 0;
        }
        strncpy(ban, userhost, USERHOSTLEN);
        if(safeban(this, ban, nick) == NULL) {
                return 0;
        }
	if(this->banvars.bad_word > 0) {
		mmode_new(this, mystate->now - 1, "+b", ban);
	}
	if(ms->optstring1 != NULL) {
		/* we got nick */
		snprintf(outstring1, STRINGLEN, " (%s)", ms->optstring1);
	} else {
		memset(outstring1, 0, STRINGLEN + 1);
	}
	if(ms->optstring2 != NULL) {
		/* we got reason */
		snprintf(outstring2, STRINGLEN, "%s", ms->optstring2);
	} else {
		snprintf(outstring2, STRINGLEN, "%s", DEFAULT_WORDBK_MESSAGE);
	}
	if(isonchan(this, nick)) {
		snprintf(timed_str, WRITE_BUFFER_LENGTH, "KICK %s %s :%s%s", this->name, nick, outstring2, strlen(outstring1) ? outstring1 : "");
		timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CHANNEL_PROTECTION, timed_str);
	}
	if(this->banvars.bad_word > 0) {
		mmode_new(this, mystate->now + this->banvars.bad_word, "-b", ban);
	}
        return 1;
}

int checkforbadnick(struct channel *this, char *nick, char *userhost)
{
        char ban[USERHOSTLEN + 1];
	char outstring1[STRINGLEN + 1];
	char outstring2[STRINGLEN + 1];
	struct maskstruct *ms;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "checkforbadnick(\"%s\", \"%s\", \"%s\")\n", this->name, nick, userhost);
#endif
        if(!haschanflags(this, "n")) {
                return 0;
        }
	if(isq(nick)) {
                return 0;
        }
        if(isadmin(this, userhost)) {
                return 0;
        }
        if(isop(this, userhost)) {
                return 0;
        }
        if(!haschanflags(this, "N") && isopnow(this, nick)) {
                return 0;
        }
        if(haschanflags(this, "F") && isfriend(this, nick, userhost)) {
		return 0;
	}
	if((ms = isnickbk(this, nick)) == NULL) {
                return 0;
        }
        strncpy(ban, userhost, USERHOSTLEN);
        if(safeban(this, ban, nick) == NULL) {
                return 0;
        }
	if(this->banvars.bad_nick > 0) {
		mmode_new(this, mystate->now - 1, "+b", ban);
	}
	if(ms->optstring1 != NULL) {
		/* we got nick */
		snprintf(outstring1, STRINGLEN, " (%s)", ms->optstring1);
	} else {
		memset(outstring1, 0, STRINGLEN + 1);
	}
	if(ms->optstring2 != NULL) {
		/* we got reason */
		snprintf(outstring2, STRINGLEN, "%s", ms->optstring2);
	} else {
		snprintf(outstring2, STRINGLEN, "%s", DEFAULT_NICKBK_MESSAGE);
	}
	snprintf(timed_str, WRITE_BUFFER_LENGTH, "KICK %s %s :%s%s", this->name, nick, outstring2, strlen(outstring1) ? outstring1 : "");
        timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CHANNEL_PROTECTION, timed_str);
	if(this->banvars.bad_nick > 0) {
		mmode_new(this, mystate->now + this->banvars.bad_nick, "-b", ban);
	}
        return 1;
}

void processenv()
{
        struct channel *this;
	char outstring[STRINGLEN + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_FLOOD, "processenv()\n");
#endif
        /* check if connected */
        if(mystate->current_server == NULL) {
                return;
        }
	/* check if registered */
        if(!mystate->current_server->registered.done) {
                /* no, check that we aren't stuck in the registration phase */
                if((mystate->now - mystate->current_server->registerconnection_start) > OER_REGISTERCONNECTION_TIMEOUT) {
                        mystate->reconnect = OER_RECONNECT_ERROR;
                }
                return;
        }
	/* check if registered */
        if(!mystate->current_server->registered.done) {
                return;
        }
	/* these are channel independent actions and 
           are processed dependless of netjoin, linenoise */
	if(mystate->current_server->linenoise) {
		if((mystate->now - mystate->last_linenoise) > mystate->current_server->linenoise) {
			snprintf(timed_str, WRITE_BUFFER_LENGTH, "ISON %s", mystate->nick);
			timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CHANNEL_HANDLING, timed_str);
			mystate->last_linenoise = mystate->now;
		}
	}
	/* check for stoned server */
	if(index(mystate->flags, (int)'p') == NULL && mystate->current_server->lastping) {
		/* wait a while before showing to the user */
		if(mystate->now > (mystate->startup + 10)) {
			if(!(mystate->now & 0xF)) {
				snprintf(outstring, STRINGLEN, "processenv->last known server activity %s", ctime(&mystate->current_server->lastping));
				striplf(outstring);
				tzset();
				/* this is the only one not enclosed in #ifdef OER_DEBUG */
				oer_debug(OER_DEBUG_INFO, "%s %s %s\n", outstring, tzname[0], tzname[1]);
			}
			if((mystate->now - mystate->current_server->lastping) > (mystate->current_server->pingfrequency * 3)) {
				snprintf(timed_str, WRITE_BUFFER_LENGTH, "QUIT :%s", OER_SERVER_IS_STONED);
				timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CHANNEL_HANDLING, timed_str);
				mystate->current_server->stoned = mystate->now;
				mystate->reconnect = OER_RECONNECT_STONED;
			}
		}
	}
	/* set unset user modes */
	if(mystate->newmode && (mystate->now - mystate->current_server->linkup) >= 10) {
		snprintf(timed_str, WRITE_BUFFER_LENGTH, "MODE %s %s", mystate->nick, mystate->mode);
		timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CHANNEL_HANDLING, timed_str);
		mystate->newmode = 0;
	}
	/* query Q if not queried before */
	if(mystate->qauth.hasauth && !mystate->qauth.isonline && (mystate->now - mystate->qauth.lastquery) > OER_Q_QUERY_INTERVAL) {
		snprintf(timed_str, WRITE_BUFFER_LENGTH, "USERHOST %s", Q_NICK);
                timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CHANNEL_PROTECTION, timed_str);
		mystate->qauth.lastquery = mystate->now;
	}
	/* send unsent qauth */
	if(mystate->qauth.hasauth && !mystate->qauth.authed && mystate->qauth.isonline) {
		snprintf(timed_str, WRITE_BUFFER_LENGTH, "PRIVMSG %s :AUTH %s %s", mystate->qauth.q, mystate->qauth.name, mystate->qauth.password);
		timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CHANNEL_HANDLING, timed_str);
		mystate->qauth.authed = 1;
	}
	/* set unset nick */
	if(mystate->newnick) {
		/* a new nick was given, let's try to get it */
		snprintf(timed_str, WRITE_BUFFER_LENGTH, "NICK %s", mystate->getnick);
		timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CHANNEL_HANDLING, timed_str);
		mystate->newnick = 0;
	} else if(!joininprogress() && strcasecmp(mystate->nick, mystate->getnick) && index(mystate->flags, (int)'g') != NULL && (mystate->now > (mystate->last_gn + OER_GN_INTERVAL))) {
		snprintf(timed_str, WRITE_BUFFER_LENGTH, "NICK %s", mystate->getnick);
		timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CHANNEL_HANDLING, timed_str);
		mystate->last_gn = mystate->now;
	}
	/* check when configs were last saved */
	if(mystate->now -  mystate->last_saved > OER_SAVE_INTERVAL) {
		saveall();
		mystate->last_saved = mystate->now;
	}
	/* check if we are in netjoin */
        if(mystate->netjoining && mystate->now < mystate->postnj_checks_at) {
		/* don't process anything until netjoin is over */
		return;
	}
	/* get missing userhosts of all nicks on all channels
	   limited by OER_NICKS_PER_USERHOST */
	syncuserhosts();
	/* main loop, process everything for all channels
           NOTE: all functions in the following loop pass a 
           pointer to the channel structure, a optimization */
	for(this = mystate->channels; this != NULL; this = this->next) {
		/* join unjoined channel */
		if(!joininprogress() && !this->joined && !this->joining && mystate->now >= this->rejoin_at && !haschanflags(this, "!")) {
			joinchannel(this);
		}
		/* check if we actually have joined the channel */
		if(!this->joined) {
			continue;
		}
		/* process post netjoin actions */
		if(mystate->netjoining) {
			processnetjoin(this);
		}
		/* sync channel if all hosts are known */
		if(!this->synced && this->allhostsknown && this->i_am_op) {
			channelsync(this);
		}
		/* check that there are no expired logons */
		if(this->i_am_op && this->allhostsknown && !noexpiredlogons(this)) {
			syncvoices(this);
			syncops(this);
			/* remove expired logons, otherwise we would
			   flood the server with +v's in some cases */
			cleanautheds(this, 0);
		}
		/* check for unset channel modes, wait for 324 */
		if(this->i_am_op && this->setchanmode && (mystate->now - this->join_ts) > OER_DELAY_CHANMODES_AFTER_JOIN) {
			setchanmode(this);
		}
		/* process possible lock for channel */
		if(this->locked.locked) {
			processlock(this);
		}
		/* set topics for this channel */
		if(this->topic_change) {
			settopic(this);
		}
		/* send requestop only if 1) requestop rules are kept 2) the channel is opless
                   3) the "R" flag is set */
                if(this->nickcount <= 20 && (this->requestop_now || mystate->now > (this->requestop + 300)) && ischanopless(this) && haschanflags(this, "R")) {
                        snprintf(timed_str, WRITE_BUFFER_LENGTH, "PRIVMSG O :requestop %s", this->name);
                        timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CHANNEL_HANDLING, timed_str);
                        snprintf(timed_str, WRITE_BUFFER_LENGTH, "PRIVMSG %s :I have requested O to give me channel operator status", this->name);
                        timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_NORMAL, timed_str);
                        if(this->nickcount > 5) {
                                snprintf(timed_str, WRITE_BUFFER_LENGTH, "PRIVMSG %s :Since there are more than 5 users on %s one of you has to do the same (/msg O requestop %s %s)", this->name, this->name, this->name, mystate->nick);
                                timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_NORMAL, timed_str);
                        }
			this->requestop_now = 0;
                        this->requestop = mystate->now;
                }
	}
	/* reset netjoin flag if post netjoin actions were processed */
	if(mystate->netjoining && mystate->now >= mystate->postnj_checks_at) {
		mystate->netjoining = 0;
		mystate->postnj_checks_at = 0;
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "processenv->end of netjoin\n");
#endif
	}
	/* process script timers */
	oerperl_process_scripttimers();

}

void setchanmode(struct channel *this)
{
	char tempmode[CHANLEN + 1];
	char modeline[STRINGLEN + 1];
	char targets[STRINGLEN + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "setchanmode->%s chankeyisset is %d and chanlimitisset is %d\n", this->name, this->chankeyisset, this->chanlimitisset);
#endif
	memset(modeline, 0, STRINGLEN + 1);
	memset(targets, 0, STRINGLEN + 1);
	if(strlen(this->mode)) {
		/* remove +/- k and +/- l from channel modes */
		filteroutchanmodes(this->mode, "kl", modeline, STRINGLEN);
	}
	if(strlen(this->key) && !this->chankeyisset) {
		if(getmodepol(this->mode, 'k') != '+' && getmodepol(this->mode, 'k') != '-') {
			/* there is no +k/-k in the modeline, add one */
			strncat(modeline, "+k", STRINGLEN - strlen(modeline));
			strncat(targets, this->key, STRINGLEN - strlen(targets));
		} else {
			snprintf(tempmode, CHANLEN, "%c%c", getmodepol(this->mode, 'k'), 'k');
			strncat(modeline, tempmode, STRINGLEN - strlen(modeline));
			strncat(targets, this->key, STRINGLEN - strlen(targets));
			if(getmodepol(this->mode, 'k') == '-') {
				/* dynamically set, empty key */
				memset(this->key, 0, CHANLEN + 1);
			}
		}
		this->chankeyisset = 1;
	}
	if(strlen(this->limit) && !this->chanlimitisset) {
		if(getmodepol(this->mode, 'l') != '+' && getmodepol(this->mode, 'l') != '-') {
			/* there is no +l/-l in the modeline, add one */
			strncat(modeline, "+l", STRINGLEN - strlen(modeline));
		} else {
			snprintf(tempmode, CHANLEN, "%c%c", getmodepol(this->mode, 'l'), 'l');
			strncat(modeline, tempmode, STRINGLEN - strlen(modeline));
		}
		if(strlen(targets)) {
			strncat(targets, " ", STRINGLEN - strlen(targets));
		}
		strncat(targets, this->limit, STRINGLEN - strlen(targets));
		if(getmodepol(this->mode, 'l') == '-') {
			/* dynamically set, empty limit */
			memset(this->limit, 0, CHANLEN + 1);
		}
		this->chanlimitisset = 1;
	}
	mmode_new(this, mystate->now, modeline, targets);
	this->setchanmode = 0;
}

void joinchannel(struct channel *this)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "joinchannel(\"%s\")\n", this->name);
#endif
	this->joined = 0;
	this->joining = 1;
	this->rejoin_at = 0;
	this->last_quote = 0;
	this->last_advert = 0;
	this->synced = 0;
	if(strlen(this->key)) {
		snprintf(timed_str, WRITE_BUFFER_LENGTH, "JOIN %s %s", this->name, this->key);
	} else {
		snprintf(timed_str, WRITE_BUFFER_LENGTH, "JOIN %s", this->name);
	}
        timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CHANNEL_HANDLING, timed_str);
}

void processnetjoin(struct channel *this)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "processnetjoin(\"%s\")\n", this->name);
#endif
	if(!haschanflags(this, "p") || !this->i_am_op) {
		return;
	}
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "processnetjoin->netjoin actions for %s\n", this->name);
#endif
	syncvoices(this);
	syncops(this);
	syncbans(this);
	syncnickbks(this);
}

void channelsync(struct channel *this)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "channelsync(\"%s\")\n", this->name);
#endif
	syncvoices(this);
	syncops(this);
	syncbans(this);
	syncnickbks(this);
	this->synced = 1;
}

void processlock(struct channel *this)
{
	int nicks;
        char nick[NICKLEN + 1];
        char stringbuffer[BIGSTRINGLEN + 1];
        char stringbuffer2[BIGSTRINGLEN + 1];
        struct chanuser *cu;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "processlock(\"%s\")\n", this->name);
#endif
	if(this->locked.unlocked) {
		/* channel has been unlocked due to manual prevention */
		this->locked.locked = 0;
		return;
	}
	if(mystate->now < this->locked.lastkick + OER_KICK_INTERVAL) {
		return;
	}
	memset(nick, 0, NICKLEN + 1);
	memset(stringbuffer, 0, BIGSTRINGLEN + 1);
	memset(stringbuffer2, 0, BIGSTRINGLEN + 1);
	nicks = 0;
	while((nicks < OER_NICKS_PER_KICK) && ((cu = getrandomuser(this)) != NULL)) {
		strncpy(nick, cu->nick, NICKLEN);
		strncat(stringbuffer, nick, BIGSTRINGLEN - strlen(stringbuffer));
		strncat(stringbuffer2, this->name, BIGSTRINGLEN - strlen(stringbuffer2));
		if((nicks + 1) < OER_NICKS_PER_KICK) {
			strncat(stringbuffer, ",", BIGSTRINGLEN - strlen(stringbuffer));
			strncat(stringbuffer2, ",", BIGSTRINGLEN - strlen(stringbuffer2));
		}
		nicks++;
        }
	if(nicks) {
		snprintf(timed_str, WRITE_BUFFER_LENGTH, "KICK %s %s :locking channel", stringbuffer2, stringbuffer);
		if(strlen(this->locked.reason)) {
			snprintf(timed_str, WRITE_BUFFER_LENGTH, "KICK %s %s :%s", stringbuffer2, stringbuffer, this->locked.reason);
		}
		timed_new(this, mystate->now, OER_TIMED_TYPE_KICK, OER_TIMED_PRIORITY_CHANNEL_PROTECTION, timed_str);
		this->locked.lastkick = mystate->now;
	} else {
		/* no more users to kick, unlocking if auto */
		if(this->locked.auto_unlock) {
			mmode_new(this, mystate->now, "-i", NULL);
			this->locked.locked = 0;
		}
	}
}

void settopic(struct channel *this)
{
        char stringbuffer[WRITE_BUFFER_LENGTH + 1];
        struct topic *t;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "settopic(\"%s\")\n", this->name);
#endif
	memset(stringbuffer, 0, WRITE_BUFFER_LENGTH + 1);
	for(t = this->topics; t != NULL; t = t->next) {
		if((strlen(stringbuffer) + strlen(t->topic) + strlen(TOPIC_SEPARATOR)) > WRITE_BUFFER_LENGTH) {
			break;
		}
		strncat(stringbuffer, t->topic, WRITE_BUFFER_LENGTH - strlen(stringbuffer));
		if(t->next != NULL) {
			strncat(stringbuffer, TOPIC_SEPARATOR, WRITE_BUFFER_LENGTH - strlen(stringbuffer));
		}
	}
	if(strlen(stringbuffer)) {
		snprintf(timed_str, WRITE_BUFFER_LENGTH, "TOPIC %s :%s", this->name, stringbuffer);
		timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CHANNEL_HANDLING, timed_str);
	}
	this->topic_change = 0;
}

void syncvoices(struct channel *this)
{
        struct chanuser *cu;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "syncvoices(\"%s\")\n", (this == NULL) ? "(null)" : this->name);
#endif
	if(this == NULL) {
		return;
	}
	for(cu = this->nicks; cu != NULL; cu = cu->next) {
		/* check if channel has auto-voice set */
		if(haschanflags(this, "v")) {
			/* check for non-voiced & to be voiced */
			if(!cu->voice && isvoice(this, cu->userhost)) {
				mmode_new(this, mystate->now, "+v", cu->nick);
			}
		}
		/* auto-voice global admins? */
		if(haschanflags(this, "V")) {
			/* check for non-voiced & to be voiced */
			if(!cu->voice && isopa(cu->userhost)) {
				mmode_new(this, mystate->now, "+v", cu->nick);
			}
		}
        }
}

void syncops(struct channel *this)
{
        struct chanuser *cu;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "syncops(\"%s\")\n", (this == NULL) ? "(null)" : this->name);
#endif
	if(this == NULL) {
                return;
        }
	for(cu = this->nicks; cu != NULL; cu = cu->next) {
		/* check if channel has auto-op set */
		if(haschanflags(this, "o")) {
			/* check for non-opped & to be opped */
			if(!cu->chanop && isop(this, cu->userhost)) {
				mmode_new(this, mystate->now, "+o", cu->nick);
				continue;
			}
		}
		/* auto-op global admins? */
		if(haschanflags(this, "O")) {
			/* check for non-opped & to be opped */
			if(!cu->chanop && isopa(cu->userhost)) {
				mmode_new(this, mystate->now, "+o", cu->nick);
				continue;
			}
		}
                /* nevertheless, check for opped & to be de-opped */
                if(cu->chanop && (haschanflags(this, "u") && !isallowedop(this, cu->nick))) {
			mmode_new(this, mystate->now, "-o", cu->nick);
                }
        }
}

void syncbans(struct channel *this)
{
        struct maskstruct *ms;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "syncbans(\"%s\")\n", this->name);
#endif
        for(ms = this->permbans; ms != NULL; ms = ms->next) {
		/* bans are set each time, doing a query would be too heavy */
		mmode_new(this, mystate->now, "+b", ms->mask);
        }
}

void syncnickbks(struct channel *this)
{
        struct chanuser *cu;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "syncnickbks(\"%s\")\n", this->name);
#endif
	for(cu = this->nicks; cu != NULL; cu = cu->next) {
		if(cu->userhost != NULL && !cu->tobek) {
			checkforbadnick(this, cu->nick, cu->userhost);
		}
	}
}

void syncuserhosts(void)
{
	int nicks;
        char nick[NICKLEN + 1];
        char stringbuffer[BIGSTRINGLEN + 1];
        struct channel *this;
        struct chanuser *cu;
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_NOISE, "syncuserhosts()\n");
#endif
        for(this = mystate->channels; this != NULL; this = this->next) {
                if(!this->joined || !this->whoquery) {
                        continue;
                }
                if(!this->allhostsknown) {
                        /* found a channel with unknown userhosts */
                        break;
                }
        }
        if(this == NULL) {
                return;
        }
        nicks = 0;
        memset(stringbuffer, 0, BIGSTRINGLEN + 1);
        while((nicks < OER_NICKS_PER_USERHOST) && (getfirsthostless(this, nick) != NULL)) {
                sethostquerystatus(nick, 1);
                strncat(stringbuffer, nick, BIGSTRINGLEN - strlen(stringbuffer));
                if((nicks + 1) < OER_NICKS_PER_USERHOST) {
                        strncat(stringbuffer, " ", BIGSTRINGLEN - strlen(stringbuffer));
                }
                nicks++;
        }
        if(nicks) {
                snprintf(timed_str, WRITE_BUFFER_LENGTH, "USERHOST %s", stringbuffer);
                timed_new(NULL, mystate->now, OER_TIMED_TYPE_USERHOST, OER_TIMED_PRIORITY_FLOOD, timed_str);
                return;
        }
        /* no more USERHOST queries, do we have all userhosts? */
        for(cu = this->nicks; cu != NULL; cu = cu->next) {
                if(cu->userhost == NULL) {
                        /* no, return immediatly */
                        return;
                }
        }
        this->allhostsknown = 1;
}

struct server *getserver()
{
        struct server *s;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "getserver()\n");
	oer_debug(OER_DEBUG_NOISE, "getserver->current server is: %s\n", (mystate->current_server) ? mystate->current_server->serverhost : "(null)");
#endif
	/* if admin specified a server, we'll take that */
	if(strlen(mystate->preferredserver)) {
		for(s = mystate->servers; s != NULL; s = s->next) {
			if(!strcasecmp(s->serverhost, mystate->preferredserver)) {
				memset(mystate->preferredserver, 0, HOSTLEN + 1);
				return s;
			}
		}
	}
	/* check if we already have a server, take next if available */
	if(mystate->current_server && mystate->current_server->next) {
		s = mystate->current_server->next;
	} else {
		/* start from top of list */
		s = mystate->servers;
	}
        for(; s != NULL; s = s->next) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_NOISE, "getserver->server %s\n", s->serverhost);
#endif
		if(s->used) {
			/* skip this server */
			continue;
		}
		/* if server is stoned see if it should be unstoned */
                if(s->stoned) {
			if(mystate->now >= (s->stoned + OER_SERVER_IS_STONED_FOR)) {
				s->stoned = 0;
				break;
			} else {
				continue;
			}
		} else {
			/* server is fine, take it */
			break;
		}
        }
	if(s == NULL) {
		/* reset used bits to avoid endlessly looping */
		for(s = mystate->servers; s != NULL; s = s->next) {
			s->used = 0;
		}
	}
        return s;
}

void massmessage(char *from, char *message)
{
	int delay;
        char stringbuffer[WRITE_BUFFER_LENGTH + 1];
	struct channel *this;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "massmessage(\"%s\", \"%s\")\n", from, message);
#endif
	snprintf(stringbuffer, WRITE_BUFFER_LENGTH, "notice from admin %s: %s", from, message);
	for(this = mystate->channels, delay = 0; this != NULL; this = this->next, delay++) {
		sendchannelnotice(this, delay, stringbuffer);
	}
}

void initchannel(struct channel *this)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "initchannel(\"%s\")\n", this->name);
#endif
	this->join_ts = 0;
	this->joined = 0;
	this->joining = 0;
	this->setchanmode = 1;
	this->chankeyisset = 0;
	this->chanlimitisset = 0;
	this->rejoin_at = 0;
	this->last_quote = 0;
	this->last_advert = 0;
	this->i_am_op = 0;
	this->synced = 0;
	this->allhostsknown = 0;
	this->whoquery = 0;
	this->requestop_now = 1;
	this->requestop = mystate->now + 300;
	if(this->last != NULL) {
		/* have to be closed because they are re-opened on join */
		if(fclose(this->last) == EOF) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_ERROR, "initchannel->fclose() failed: %s\n", strerror(errno));
#endif
		}
		this->last = NULL;
	}
	if(this->seen != NULL) {
		if(fclose(this->seen) == EOF) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_ERROR, "initchannel->fclose() failed: %s\n", strerror(errno));
#endif
		}
		this->seen = NULL;
	}
	initjoins(this);
	initparts(this);
	initpubmsgs(this);
	memset(&this->locked, 0, sizeof(struct locked));
	initnicks(this);
	initmmodes(this);
	initchannelbans(this);
}

void initall(void)
{
	struct channel *this;
	struct timed *t1;
	struct timed *t2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "initall()\n");
#endif
	cleanautheds(NULL, 1);
        for(t1 = mystate->timeds; t1 != NULL; t1 = t2) {
		t2 = t1->next;
                timed_del(t1);
	}
	mystate->last_gn = mystate->now;
	mystate->newmode = 1;
	mystate->qauth.authed = 0;
	mystate->qauth.isonline = 0;
	mystate->qauth.lastquery = 0;
	for(this = mystate->channels; this != NULL; this = this->next) {
		initchannel(this);
	}
	if(index(mystate->flags, (int)'d') != NULL) {
		/* remove internal dynamic admin, it will be always added
		   when registering to the server */
		deladmin(OER_INTERNAL_DYNAMIC_ADMIN);
	}
}

void initnicks(struct channel *this)
{
	struct chanuser *cu;
	struct chanuser *cu2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "initnicks(\"%s\")\n", this->name);
#endif
	cu = this->nicks;
	while(cu != NULL) {
		cu2 = cu->next;
		free(cu->userhost);
		free(cu->nick);
		free(cu);
		cu = cu2;
	}
	this->nicks = NULL;
	this->nickcount = 0;
}

void initmmodes(struct channel *this)
{
	struct mmode *m1;
	struct mmode *m2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "initmmodes(\"%s\")\n", this->name);
#endif
	m1 = this->mmodes;
	while(m1 != NULL) {
		m2 = m1->next;
		mmode_del(this, m1);
		m1 = m2;
	}
	this->mmodes = NULL;
}

void initpubmsgs(struct channel *this)
{
	struct pubmsg *pm;
	struct pubmsg *pm2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "initpubmsgs(\"%s\")\n", this->name);
#endif
	pm = this->pubmsgs;
	while(pm != NULL) {
		pm2 = pm->next;
		free(pm->message);
		free(pm->nick);
		free(pm->userhost);
		free(pm);
		pm = pm2;
	}
	this->pubmsgs = NULL;
}

void initparts(struct channel *this)
{
	struct part *p;
	struct part *p2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "initparts(\"%s\")\n", this->name);
#endif
	p = this->parts;
	while(p != NULL) {
		p2 = p->next;
		free(p->nick);
		free(p->userhost);
		free(p);
		p = p2;
	}
	this->parts = NULL;
}

void initjoins(struct channel *this)
{
	struct join *j;
	struct join *j2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "initnicks(\"%s\")\n", this->name);
#endif
	j = this->joins;
	while(j != NULL) {
		j2 = j->next;
		free(j->nick);
		free(j->userhost);
		free(j);
		j = j2;
	}
	this->joins = NULL;
}

void initpermbans(struct channel *this)
{
	struct maskstruct *ms;
	struct maskstruct *ms2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "initpermbans(\"%s\")\n", this->name);
#endif
	ms = this->permbans;
	while(ms != NULL) {
		ms2 = ms->next;
		free(ms->optstring2);
		free(ms->optstring1);
		free(ms->mask);
		free(ms);
		ms = ms2;
	}
	this->permbans = NULL;
}

void initchannelbans(struct channel *this)
{
	struct channelban *cb;
	struct channelban *cb2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "initchannelbans(\"%s\")\n", this->name);
#endif
	cb = this->channelbans;
	cb2 = cb;
	while(cb != NULL) {
		cb2 = cb->next;
		free(cb->ban);
		free(cb->setby);
		free(cb);
		cb = cb2;
	}
	this->channelbans = NULL;
}

void initnickbks(struct channel *this)
{
	struct maskstruct *ms;
	struct maskstruct *ms2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "initnicks(\"%s\")\n", this->name);
#endif
	ms = this->nickbks;
	while(ms != NULL) {
		ms2 = ms->next;
		free(ms->mask);
		free(ms->optstring2);
		free(ms->optstring1);
		free(ms);
		ms = ms2;
	}
	this->nickbks = NULL;
}

void initwordbks(struct channel *this)
{
	struct maskstruct *ms;
	struct maskstruct *ms2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "initwordbks(\"%s\")\n", this->name);
#endif
	ms = this->wordbks;
	while(ms != NULL) {
		ms2 = ms->next;
		free(ms->mask);
		free(ms->optstring2);
		free(ms->optstring1);
		free(ms);
		ms = ms2;
	}
	this->wordbks = NULL;
}

void initusers(struct channel *this)
{
	struct botuser *u;
	struct botuser *u2;
	struct maskstruct *ms;
	struct maskstruct *ms2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "initusers(\"%s\")\n", this->name);
#endif
	u = this->users;
	while(u != NULL) {
		u2 = u->next;
		ms = u->firstmask;
		while(ms != NULL) {
			ms2 = ms->next;
			free(ms->mask);
			free(ms);
			ms = ms2;
		}
		free(u->handle);
		free(u->options);
		free(u->password);
		free(u);
		u = u2;
	}
	this->users = NULL;
}

struct state *emptystate()
{
	struct state *state;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "emptystate()\n");
#endif
	if((state = (struct state *) malloc(sizeof(struct state))) == NULL) {
		return NULL;
	}
	memset(state->nick, 0, NICKLEN + 1);
	memset(state->altnick, 0, NICKLEN + 1);
	memset(state->getnick, 0, NICKLEN + 1);
	memset(state->user, 0, USERLEN + 1);
	memset(state->host, 0, HOSTLEN + 1);
	memset(state->mode, 0, CHANLEN + 1);
	memset(state->signoff, 0, STRINGLEN + 1);
	memset(state->preferredserver, 0, HOSTLEN + 1);
	memset(state->realname, 0, STRINGLEN + 1);
	memset(state->flags, 0, FLAGLEN + 1);
	memset(state->prefix, 0, MICROSTRINGLEN + 1);
	memset(&state->qauth, 0, sizeof(struct qauth));
	memset(state->proxysetup, 0, STRINGLEN + 1);
	memset(state->vhost, 0, HOSTLEN + 1);
	state->netjoining = 0;
	state->reconnect = 0;
	state->now = time(NULL);
	memset(&state->last_action, 0, sizeof(struct last_action));
	state->startup = time(NULL);
	state->postnj_checks_at = 0;
	state->last_saved = time(NULL);
	state->last_gn = time(NULL);
	state->sockfd = 0;
	state->loopforever = 0;
	state->customuser = 0;
	state->use_altnick = 0;
	state->newnick = 0;
	state->newmode = 0;
	state->bailout = 0;
	state->quitting = 0;
	state->rawcounter = 0;
	state->outputcounter = 0;
	state->logfp = NULL;
	state->logto = NULL;
	state->trusted = NULL;
	state->services = NULL;
	state->admins = NULL;
	state->kickrs = 0;
	state->kickreasons = NULL;
	state->channels = NULL;
	state->timeds = NULL;
	state->current_server = NULL;
	state->servers = NULL;
	state->scripts = NULL;
	return state;
}

struct topic *addnewtopic(struct channel *this, char *setby, char *topic)
{
	int amount;
	struct topic *t;
	struct topic *t2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "addnewtopic(\"%s\", \"%s\", \"%s\")\n", this->name, setby, topic);
#endif
	if(topiccount(this) >= OER_TOPICS) {
		return NULL;
	}
	/* strip mIRC colors */
        stripmirc(topic);
        /* strip all control chars from message */
        stripcntrl(topic);
	t = this->topics;
	t2 = t;
	amount = 0;
	while(t != NULL) {
                t2 = t;
                amount++;
                t = t2->next;
        }
	if((t = (struct topic *) malloc(sizeof(struct topic))) == NULL) {
		return NULL;
	}
	if((t->setby = (char *) malloc(strlen(setby) + 1)) == NULL) {
		return NULL;
	}
	if((t->topic = (char *) malloc(strlen(topic) + 1)) == NULL) {
		return NULL;
	}
	strcpy(t->setby, setby);
	strcpy(t->topic, topic);
	t->at = mystate->now;
	t->deleted = 0;
	t->list = 0;
	t->next = NULL;
	t->prev = NULL;
	this->topic_list = 0;
	if(strcasecmp(setby, "gettopic()")) {
		this->topic_change = 1;
	}
	if(this->topics == NULL) {
		/* first topic, special case */
		this->topics = t;
		return t;
	}
	/* >=1 topics, normal processing */
        t2->next = t;
        t->prev = t2;
        return t;
}

struct topic *insertnewtopic(struct channel *this, char *setby, char *topic, int pos)
{
	int amount;
	struct topic *t;
	struct topic *t2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "insertnewtopic(\"%s\", \"%s\", \"%s\", %d)\n", this->name, setby, topic, pos);
#endif
	if(!isnumbw(pos, OER_LIMIT_TOPICS_MIN, OER_LIMIT_TOPICS_MAX)) {
		return NULL;
	}
	if(topiccount(this) >= OER_TOPICS) {
		return NULL;
	}
	/* strip mIRC colors */
        stripmirc(topic);
        /* strip all control chars from message */
        stripcntrl(topic);
	t = this->topics;
	t2 = t;
	amount = 1;
	while((t != NULL) && (amount != pos)) {
                t2 = t;
                amount++;
                t = t2->next;
        }
	if((t = (struct topic *) malloc(sizeof(struct topic))) == NULL) {
		return NULL;
	}
	if((t->setby = (char *) malloc(strlen(setby) + 1)) == NULL) {
		return NULL;
	}
	if((t->topic = (char *) malloc(strlen(topic) + 1)) == NULL) {
		return NULL;
	}
	strcpy(t->setby, setby);
	strcpy(t->topic, topic);
	t->at = mystate->now;
	t->list = 0;
	t->deleted = 0;
	t->next = NULL;
	t->prev = NULL;
	this->topic_list = 0;
	/* there are 4 possibilities: 1st (new), 1st, last and
	   between 2 other topics */
	if(pos == 1 && this->topics == NULL) {
		/* first */
		this->topic_change = 1;
		this->topics = t;
		return t;
	}
	if(pos == 1 && this->topics != NULL) {
		/* first */
		this->topic_change = 1;
		this->topics = t;
		t->next = t2;
		t2->prev = t;
		return t;
	}
	if(t2->next == NULL) {
		/* last */
		this->topic_change = 1;
		t2->next = t;
		t->prev = t2;
		return t;
	}
	if(t2 != NULL && t2->next != NULL) {
		/* between 2 */
		this->topic_change = 1;
		t->next = t2->next;
		t->prev = t2;
		t2->next->prev = t;
		t2->next = t;
		return t;
	}
	free(t->topic);
	free(t->setby);
	free(t);
	return NULL;
}

struct topic *edittopic(struct channel *this, char *setby, char *topic, int pos)
{
	int amount;
	struct topic *t;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "edittopic(\"%s\", \"%s\", \"%s\", %d)\n", this->name, setby, topic, pos);
#endif
	if(!isnumbw(pos, OER_LIMIT_TOPICS_MIN, OER_LIMIT_TOPICS_MAX)) {
		return NULL;
	}
	/* strip mIRC colors */
        stripmirc(topic);
        /* strip all control chars from message */
        stripcntrl(topic);
	amount = 1;
	t = this->topics;
	while((t != NULL) && (amount != pos)) {
		amount++;
		t = t->next;
	}
	if(t == NULL) {
		return NULL;
	}
	free(t->setby);
	free(t->topic);
	if((t->setby = (char *) malloc(strlen(setby) + 1)) == NULL) {
		return NULL;
	}
	if((t->topic = (char *) malloc(strlen(topic) + 1)) == NULL) {
		return NULL;
	}
	strcpy(t->setby, setby);
	strcpy(t->topic, topic);
	t->at = mystate->now;
	t->list = 0;
	t->deleted = 0;
	this->topic_change = 1;
	this->topic_list = 0;
	return t;
}

void gettopic(struct channel *this, char *topic)
{
	int i;
	int k;
	int length;
	char thistopic[TOPICLEN + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "gettopic(\"%s\", \"%s\")\n", this->name, topic);
#endif
	delalltopics(this);
	i = 0;
	k = 0;
	/* topic is at most of (READ_BUFFER_LENGTH - some) length */
	length = strlen(topic);
	while(i < length) {
		if(!strncasecmp(topic + i, TOPIC_SEPARATOR, strlen(TOPIC_SEPARATOR)) || k >= TOPICLEN) {
			/* we found the first topic separator or we ran out of
			   space for one topic */
			thistopic[k++] = '\0';
			/* strip mIRC colors */
			stripmirc(thistopic);
			/* strip all control characters */
			stripcntrl(thistopic);
			addnewtopic(this, "gettopic()", thistopic);
			memset(thistopic, 0, TOPICLEN + 1);
			k = 0;
			i += strlen(TOPIC_SEPARATOR);
		} else {
			thistopic[k++] = topic[i++];
		}
	}
	if(k > 0) {
		/* end of topic */
		thistopic[k++] = '\0';
		/* strip mIRC colors */
		stripmirc(thistopic);
		/* strip all control characters */
		stripcntrl(thistopic);
		addnewtopic(this, "gettopic()", thistopic);
	}
}

void listtopic(struct channel *this, char *to, int tochan)
{
        int amount;
        struct topic *t;
        char stringbuffer[BIGSTRINGLEN + 1];
        char ts[STRINGLEN + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "listtopic(\"%s\", \"%s\", %d)\n", this->name, to, tochan);
#endif
	t = this->topics;
	amount = 1;
	while(t != NULL) {
		striplf(t->topic);
		strncpy(ts, ctime(&t->at), STRINGLEN);
		striplf(ts);
		if(!strcasecmp(t->setby, "gettopic()")) {
			tzset();
			snprintf(stringbuffer, BIGSTRINGLEN, "Topic #%d: %s (%s %s %s)", amount, t->topic, ts, tzname[0], tzname[1]);
		} else {
			tzset();
			snprintf(stringbuffer, BIGSTRINGLEN, "Topic #%d: %s (%s %s %s, %s)", amount, t->topic, ts, tzname[0], tzname[1], t->setby);
		}
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		amount++;
		t = t->next;
	}
}

void deltopic(struct channel *this, char *to, int tochan, char *nick, char *userhost, char *commandline)
{
	int amount;
	int ppos;
	int nppos;
	int which;
	char p1[STRINGLEN + 1];
	char outstring[WRITE_BUFFER_LENGTH + 1];
	struct topic *t;
	struct topic *t2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "deltopic(\"%s\", \"%s\", %d, \"%s\", \"%s\", \"%s\")\n", (this == NULL) ? "(null)" : this->name, to, tochan, nick, userhost, commandline);
#endif
	/* command line parsing begins here, first parameter = first topic to be deleted */
	ppos = 0;
	while(1) {
		nppos = parse(commandline, ppos, " ", p1, STRINGLEN, 0);
		if(nppos == ppos || nppos < 0) {
			break;
		}
		ppos = nppos;
		which = atoi(p1);
		if(!isnumbw(which, OER_LIMIT_TOPICS_MIN, OER_LIMIT_TOPICS_MAX)) {
			continue;
		}
		t = this->topics;
		amount = 1;
		while((t != NULL) && (amount != which)) {
			amount++;
			t = t->next;
		}
		if(t == NULL) {
			continue;
		}
		t->deleted = 1;
		snprintf(outstring, WRITE_BUFFER_LENGTH, "deleted Topic #%d: %s", which, t->topic);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, outstring);
	}
	t = this->topics;
	while(t != NULL) {
		t2 = t->next;
		if(t->deleted) {
			if(t->prev == NULL) {
				/* first */
				if(t->next == NULL) {
					/* only one */
					this->topics = NULL;
				} else {
					t->next->prev = NULL;
					this->topics = t->next;
				}
			} else if(t->next == NULL) {
				/* last */
				t->prev->next = NULL;
			} else {
				/* between 2 or more */
				t->prev->next = t->next;
				t->next->prev = t->prev;
			}
			free(t->setby);
			free(t->topic);
			free(t);
			this->topic_change = 1;
		}
		t = t2;
	}
}

void delalltopics(struct channel *this)
{
	struct topic *t;
	struct topic *t2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "delalltopics(\"%s\")\n", this->name);
#endif
	t = this->topics;
	while(t != NULL) {
		t2 = t->next;
		free(t->setby);
		free(t->topic);
		free(t);
		t = t2;
	}
	this->topics = NULL;
}

void swaptopic(struct channel *this, int x, int y)
{
	int amount;
	struct topic *t;
	struct topic *tx;
	struct topic *ty;
	char *ptr;
	time_t at;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "swaptopic(\"%s\", %d, %d)\n", this->name, x, y);
#endif
	if(!isnumbw(x, OER_LIMIT_TOPICS_MIN, OER_LIMIT_TOPICS_MAX)) {
		return;
	}
	if(!isnumbw(y, OER_LIMIT_TOPICS_MIN, OER_LIMIT_TOPICS_MAX)) {
		return;
	}
	t = this->topics;
	amount = 1;
	tx = NULL;
	ty = NULL;
	while((t != NULL)) {
		if(amount == x) {
			tx = t;
		}
		if(amount == y) {
			ty = t;
		}
		amount++;
		t = t->next;
	}
	if(tx == NULL || ty == NULL) {
		return;
	}
	ptr = tx->topic;
	tx->topic = ty->topic;
	ty->topic = ptr;
	ptr = tx->setby;
	tx->setby = ty->setby;
	ty->setby = ptr;
	at = tx->at;
	tx->at = ty->at;
	ty->at = at;
	this->topic_change = 1;
}

void parsectcp(char *nick, char *userhost, int ctcp, char *message)
{
	int proceed;
	int isuser;
	struct channel *this;
#ifdef HAVE_UNAME
	struct utsname utsname;
#endif
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "parsectcp(\"%s\", \"%s\", %d, \"%s\")\n", nick, userhost, ctcp, message);
#endif
	isuser = 1;
	if(!isopa(userhost)) {
		proceed = 0;
		isuser = 0;
		if(index(mystate->flags, (int)'f') != NULL) {
			proceed = 1;
		}
		for(this = mystate->channels; this != NULL; this = this->next) {
			if(haschanflags(this, "U") && !isatleastop(this, nick, userhost)) {
				continue;
			}
			if(isatleastopnow(this, nick, userhost)) {
				proceed = 1;
				isuser = 1;
			}
			if(haschanflags(this, "F") && isfriend(this, nick, userhost)) {
				proceed = 1;
				isuser = 1;
			}
		}
		if(!proceed) {
			return;
		}
	}
	switch(ctcp) {
	case OER_WHICHCTCP_FINGER:
		snprintf(timed_str, WRITE_BUFFER_LENGTH, "Notice %s :%cFINGER (%s@%s) Idle 0 seconds%c", nick, 1, mystate->user, mystate->host, 1);
		timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CTCP, timed_str);
		break;
	case OER_WHICHCTCP_PING:
		snprintf(timed_str, WRITE_BUFFER_LENGTH, "Notice %s :%s", nick, message);
		timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CTCP, timed_str);
		break;
	case OER_WHICHCTCP_USERINFO:
		snprintf(timed_str, WRITE_BUFFER_LENGTH, "Notice %s :%cUSERINFO oer IRC bot%s%c", nick, 1, (index(mystate->flags, (int)'q') != NULL) ? " (Q friendly)" : "", 1);
		timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CTCP, timed_str);
		break;
	case OER_WHICHCTCP_VERSION:
#ifdef HAVE_UNAME
		/* show only to users on bot */
		if(isuser && !uname(&utsname)) {
			snprintf(timed_str, WRITE_BUFFER_LENGTH, "Notice %s :%cVERSION %s, %s running on %s %s %s [%lu]%c", nick, 1, OER_VERSION, OER_COPYRIGHT1, utsname.sysname, utsname.release, utsname.machine, mystate->now - mystate->startup, 1);
		} else {
			snprintf(timed_str, WRITE_BUFFER_LENGTH, "Notice %s :%cVERSION %s, %s [%lu]%c", nick, 1, OER_VERSION, OER_COPYRIGHT1, mystate->now - mystate->startup, 1);
		}
#else
		snprintf(timed_str, WRITE_BUFFER_LENGTH, "Notice %s :%cVERSION %s, %s [%lu]%c", nick, 1, OER_VERSION, OER_COPYRIGHT1, mystate->now - mystate->startup, 1);
#endif
		timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CTCP, timed_str);
		break;
	}
}

int whichctcp(char *message)
{
	int i;
	int k;
	int length;
	char stringbuffer[HUGESTRINGLEN + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "whichctcp(\"%s\")\n", message);
#endif
	/* We start from the 2nd char (first is ^A) */
	for(i = 1, k = 0, length = strlen(message); i < length && k < HUGESTRINGLEN; i++) {
		if(message[i] == 1) {
			break;
		}
		stringbuffer[k++] = message[i];
	}
	if(k >= HUGESTRINGLEN) {
		return OER_WHICHCTCP_INVALID;
	}
	stringbuffer[k] = '\0';
	if(!strncasecmp(stringbuffer, "ACTION", 6)) {
		return OER_WHICHCTCP_ACTION;
	}
	if(!strncasecmp(stringbuffer, "FINGER", 6)) {
		return OER_WHICHCTCP_FINGER;
	}
	if(!strncasecmp(stringbuffer, "PING", 4)) {
		return OER_WHICHCTCP_PING;
	}
	if(!strncasecmp(stringbuffer, "USERINFO", 8)) {
		return OER_WHICHCTCP_USERINFO;
	}
	if(!strncasecmp(stringbuffer, "VERSION", 7)) {
		return OER_WHICHCTCP_VERSION;
	}
	return OER_WHICHCTCP_INVALID;
}

int whichcommand(char *command, int params)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "whichcommand(\"%s\", %d)\n", command, params);
#endif
	if(!strcasecmp(command, "action")) {
		if(params >= 1) {
			return OER_WHICHCOMMAND_ACTION;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "add")) {
		if(params >= 1) {
			return OER_WHICHCOMMAND_ADD;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "b")) {
		if(params >= 1) {
			return OER_WHICHCOMMAND_BAN;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "ban")) {
		if(params >= 1) {
			return OER_WHICHCOMMAND_BAN;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "bankick")) {
		if(params >= 1) {
			return OER_WHICHCOMMAND_BANKICK;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "bk")) {
		if(params >= 1) {
			return OER_WHICHCOMMAND_BANKICK;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "bstats")) {
		if(params == 0) {
			return OER_WHICHCOMMAND_BSTATS;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "cb")) {
		if(params == 0) {
			return OER_WHICHCOMMAND_CLEARBANS;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "cc")) {
		if(params == 1) {
			return OER_WHICHCOMMAND_CLONECHANNEL;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "ci")) {
		if(params == 0) {
			return OER_WHICHCOMMAND_CHANINFO;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "chaninfo")) {
		if(params == 0) {
			return OER_WHICHCOMMAND_CHANINFO;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "clearbans")) {
		if(params == 0) {
			return OER_WHICHCOMMAND_CLEARBANS;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "clonechannel")) {
		if(params == 1) {
			return OER_WHICHCOMMAND_CLONECHANNEL;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "cycle")) {
		if(params == 0) {
			return OER_WHICHCOMMAND_CYCLE;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "date")) {
		if(params == 0) {
			return OER_WHICHCOMMAND_DATE;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "del")) {
		if(params >= 1) {
			return OER_WHICHCOMMAND_DEL;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "devoice")) {
		if(params >= 0) {
			return OER_WHICHCOMMAND_DEVOICE;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "dop")) {
		if(params >= 0) {
			return OER_WHICHCOMMAND_DOP;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "edit")) {
		if(params >= 1) {
			return OER_WHICHCOMMAND_EDIT;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "ext")) {
		if(params >= 1) {
			return OER_WHICHCOMMAND_EXT;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "flush")) {
		if(params == 0) {
			return OER_WHICHCOMMAND_FLUSH;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "help")) {
		if(params == 0) {
			return OER_WHICHCOMMAND_HELP;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "info")) {
		if(params == 0) {
			return OER_WHICHCOMMAND_INFO;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "invite")) {
		if(params == 0) {
			return OER_WHICHCOMMAND_INVITE;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "jump")) {
		if(params >= 0) {
			return OER_WHICHCOMMAND_JUMP;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "k")) {
		if(params >= 1) {
			return OER_WHICHCOMMAND_KICK;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "kick")) {
		if(params >= 1) {
			return OER_WHICHCOMMAND_KICK;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "last")) {
		if(params == 0 || params == 1 || params == 2) {
			return OER_WHICHCOMMAND_LAST;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "list")) {
		if(params >= 1) {
			return OER_WHICHCOMMAND_LIST;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "lock")) {
		if(params >= 0) {
			return OER_WHICHCOMMAND_LOCK;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "locku")) {
		if(params >= 0) {
			return OER_WHICHCOMMAND_LOCKU;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "logoff")) {
		if(params >= 1) {
			return OER_WHICHCOMMAND_LOGOFF;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "logon")) {
		/* params == 1 when admin logon */
		if(params == 1 || params == 2) {
			return OER_WHICHCOMMAND_LOGON;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "mm")) {
		if(params >= 1) {
			return OER_WHICHCOMMAND_MASSMESSAGE;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "massmessage")) {
		if(params >= 1) {
			return OER_WHICHCOMMAND_MASSMESSAGE;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "m")) {
		if(params >= 1) {
			return OER_WHICHCOMMAND_MODE;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "mode")) {
		if(params >= 1) {
			return OER_WHICHCOMMAND_MODE;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "nstats")) {
		if(params == 1) {
			return OER_WHICHCOMMAND_NSTATS;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "op")) {
		if(params >= 0) {
			return OER_WHICHCOMMAND_OP;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "qs")) {
		if(params == 0) {
			return OER_WHICHCOMMAND_QUEUESTATS;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "queuestats")) {
		if(params == 0) {
			return OER_WHICHCOMMAND_QUEUESTATS;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "quit")) {
		if(params >= 0) {
			return OER_WHICHCOMMAND_QUIT;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "quote")) {
		if(params == 1) {
			return OER_WHICHCOMMAND_QUOTE;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "r")) {
		if(params == 1) {
			return OER_WHICHCOMMAND_RESET;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "raw")) {
		if(params >= 1) {
			return OER_WHICHCOMMAND_RAW;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "reset")) {
		if(params == 1) {
			return OER_WHICHCOMMAND_RESET;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "rk")) {
		if(params == 0) {
			return OER_WHICHCOMMAND_RANDOM_KICK;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "rbk")) {
		if(params == 0) {
			return OER_WHICHCOMMAND_RANDOM_BANKICK;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "save")) {
		if(params == 0) {
			return OER_WHICHCOMMAND_SAVE;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "say")) {
		if(params >= 1) {
			return OER_WHICHCOMMAND_SAY;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "seen")) {
		if(params == 0 || params == 1 || params == 2) {
			return OER_WHICHCOMMAND_SEEN;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "sstats")) {
		if(params == 0) {
			return OER_WHICHCOMMAND_SSTATS;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "sync")) {
		if(params == 0) {
			return OER_WHICHCOMMAND_SYNC;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "syncall")) {
		if(params == 0) {
			return OER_WHICHCOMMAND_SYNCALL;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "t")) {
		if(params >= 1) {
			return OER_WHICHCOMMAND_TOPIC_SET;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "topic")) {
		if(params >= 1) {
			return OER_WHICHCOMMAND_TOPIC_SET;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "ta")) {
		if(params >= 1) {
			return OER_WHICHCOMMAND_TOPIC_ADD;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "td")) {
		if(params >= 1) {
			return OER_WHICHCOMMAND_TOPIC_DEL;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "te")) {
		if(params >= 2) {
			return OER_WHICHCOMMAND_TOPIC_EDIT;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "ti")) {
		if(params >= 2) {
			return OER_WHICHCOMMAND_TOPIC_INS;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "tl")) {
		if(params == 0) {
			return OER_WHICHCOMMAND_TOPIC_LIST;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "tg")) {
		if(params == 0) {
			return OER_WHICHCOMMAND_TOPIC_GET;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "tr")) {
		if(params == 0) {
			return OER_WHICHCOMMAND_TOPIC_REFRESH;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "ts")) {
		if(params == 2) {
			return OER_WHICHCOMMAND_TOPIC_SWAP;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "ub")) {
		if(params >= 1) {
			return OER_WHICHCOMMAND_UNBAN;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "unban")) {
		if(params >= 1) {
			return OER_WHICHCOMMAND_UNBAN;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "unlock")) {
		if(params == 0) {
			return OER_WHICHCOMMAND_UNLOCK;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "uptime")) {
		if(params == 0) {
			return OER_WHICHCOMMAND_UPTIME;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "uc")) {
		if(params == 2 || params == 3) {
			return OER_WHICHCOMMAND_USERCOPY;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "usercopy")) {
		if(params == 2 || params == 3) {
			return OER_WHICHCOMMAND_USERCOPY;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "v")) {
		if(params >= 0) {
			return OER_WHICHCOMMAND_VOICE;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "voice")) {
		if(params >= 0) {
			return OER_WHICHCOMMAND_VOICE;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	if(!strcasecmp(command, "wall")) {
		if(params > 1) {
			return OER_WHICHCOMMAND_WALL;
		}
		return OER_WHICHCOMMAND_INVALID;
	}
	return OER_WHICHCOMMAND_INVALID;
}

struct maskstruct *addnewadminmask(char *handle, char *mask)
{
	struct botuser *admin;
	struct maskstruct *ms;
	struct maskstruct *ms2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "addnewadminmask(\"%s\", \"%s\")\n", handle, mask);
#endif
	for(admin = mystate->admins; admin != NULL; admin = admin->next) {
		if(!strcasecmp(admin->handle, handle)) {
			break;
		}
	}
	if(admin == NULL) {
		return NULL;
	}
	ms = admin->firstmask;
	ms2 = ms;
	while(ms != NULL) {
		ms2 = ms;
		if(!strcasecmp(ms->mask, mask)) {
			return NULL;
		}
		ms = ms2->next;
	}
	if((ms = (struct maskstruct *) malloc(sizeof(struct maskstruct))) == NULL) {
		return NULL;
	}
	if((ms->mask = (char *) malloc(strlen(mask) + 1)) == NULL) {
		return NULL;
	}
	strcpy(ms->mask, mask);
	ms->next = NULL;
	ms->prev = NULL;
	if(admin->firstmask == NULL) {
		/* first mask, special case */
		admin->firstmask = ms;
		return ms;
	}
	/* >=1 masks, normal processing */
	ms2->next = ms;
	ms->prev = ms2;
	return ms;
}

int deladmin(char *handle)
{
	struct botuser *admin;
        struct maskstruct *ms;
        struct maskstruct *ms2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "deladmin(\"%s\")\n", handle);
#endif
	for(admin = mystate->admins; admin != NULL; admin = admin->next) {
		if(!strcasecmp(admin->handle, handle)) {
			break;
		}
	}
	if(admin == NULL) {
		return 0;
	}
	/* check if admin can be deleted */
	if(!delopa(admin->handle)) {
		return 0;
	}
	if(index(admin->options, (int)'d') != NULL) {
		/* dyn admin, remove all autheds */
		logoff(NULL, admin->handle);
	}
	/* first free all masks */
	ms = admin->firstmask;
	while(ms != NULL) {
		ms2 = ms->next;
		free(ms->mask);
		free(ms);
		ms = ms2;
	}
	if(admin->prev == NULL) {
                /* first */
                if(admin->next == NULL) {
                        /* only one */
			mystate->admins = NULL;
                } else {
                        admin->next->prev = NULL;
                        mystate->admins = admin->next;
                }
        } else if(admin->next == NULL) {
                /* last */
                admin->prev->next = NULL;
        } else {
                /* between 2 or more */
                admin->prev->next = admin->next;
                admin->next->prev = admin->prev;
        }
	free(admin->handle);
	free(admin->options);
	free(admin->password);
	free(admin);
	return 1;
}

int deladminmask(char *handle, char *mask)
{
	struct botuser *admin;
	struct maskstruct *ms;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "deladminmask(\"%s\", \"%s\")\n", handle, mask);
#endif
	for(admin = mystate->admins; admin != NULL; admin = admin->next) {
		if(!strcasecmp(admin->handle, handle)) {
			break;
		}
	}
	if(admin == NULL) {
		return 0;
	}
	/* find & remove mask */
	for(ms = admin->firstmask; ms != NULL; ms = ms->next) {
		if(!strcasecmp(ms->mask, mask)) {
			break;
		}
	}
	if(ms == NULL) {
		return 0;
	}
	if(ms->prev == NULL) {
                /* first */
                if(ms->next == NULL) {
                        /* only */
                        admin->firstmask = NULL;
                } else {
                        ms->next->prev = NULL;
                        admin->firstmask = ms->next;
                }
        } else if(ms->next == NULL) {
                /* last */
                ms->prev->next = NULL;
        } else {
                /* between 2 or more */
                ms->prev->next = ms->next;
                ms->next->prev = ms->prev;
        }
	free(ms->mask);
	free(ms);
	return 1;
}

int deluser(struct channel *this, char *handle)
{
	struct botuser *user;
	struct maskstruct *ms;
	struct maskstruct *ms2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "deluser(\"%s\")\n", handle);
#endif
	for(user = this->users; user != NULL; user = user->next) {
		if(!strcasecmp(user->handle, handle)) {
			break;
		}
	}
	if(user == NULL) {
		return 0;
	}
	/* first free all masks */
	ms = user->firstmask;
	while(ms != NULL) {
		ms2 = ms->next;
		free(ms->mask);
		free(ms);
		ms = ms2;
	}
	if(user->prev == NULL) {
                /* first */
                if(user->next == NULL) {
                        /* only one */
                        this->users = NULL;
                } else {
                        user->next->prev = NULL;
                        this->users = user->next;
                }
        } else if(user->next == NULL) {
                /* last */
                user->prev->next = NULL;
        } else {
                /* between 2 or more */
                user->prev->next = user->next;
                user->next->prev = user->prev;
        }
	free(user->password);
	free(user->handle);
	free(user->options);
	free(user);
	return 1;
}

int delusermasks(struct channel *this, char *handle, char *mask, int force)
{
	int deleted;
	struct botuser *user;
	struct maskstruct *ms;
	struct maskstruct *ms2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "delusermasks(\"%s\", \"%s\", \"%s\", %d)\n", this->name, handle, mask, force);
#endif
	deleted = 0;
	for(user = this->users; user != NULL; user = user->next) {
		if(force || !strcasecmp(user->handle, handle)) {
			ms = user->firstmask;
			while(ms != NULL) {
				ms2 = ms->next;
				if(force || !strcasecmp(ms->mask, mask)) {
					if(ms->prev == NULL) {
						/* first mask */
						if(ms->next == NULL) {
							/* only mask */
							user->firstmask = NULL;
						} else {
							ms->next->prev = NULL;
							user->firstmask = ms->next;
						}
					} else if(ms->next == NULL) {
						/* last mask */
						ms->prev->next = NULL;
					} else {
						/* between 2 or more masks */
						ms->prev->next = ms->next;
						ms->next->prev = ms->prev;
					}
					free(ms->mask);
					free(ms);
					deleted++;
				}
				ms = ms2;
			}
		}
	}
	return deleted;
}

int deladverts(struct channel *this, char *to, int force)
{
	int deleted;
	struct advert *a;
	struct advert *a2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "deladverts(\"%s\", \"%s\", %d)\n", this->name, to, force);
#endif
	/* check for static type adverts & valid userhost */
	if(!force && strcasecmp(to, "admins") && strcasecmp(to, "ops") && strcasecmp(to, "other") && strcasecmp(to, "normals") && strcasecmp(to, "all") && !isvaliduserhost(to)) {
		return 0;
	}
	a = this->adverts;
	deleted = 0;
	while(a != NULL) {
		a2 = a->next;
		if(force || !strcmp(a->to, to)) {
                        if(a->prev == NULL) {
                                /* first */
                                if(a->next == NULL) {
                                        /* only one */
                                        this->adverts = NULL;
                                } else {
                                        a->next->prev = NULL;
                                        this->adverts = a->next;
                                }
                        } else if(a->next == NULL) {
                                /* last */
                                a->prev->next = NULL;
                        } else {
                                /* between 2 or more */
                                a->prev->next = a->next;
                                a->next->prev = a->prev;
                        }
			free(a->to);
			free(a->message);
			free(a);
			deleted++;
		}
		a = a2;
	}
	return deleted;
}

int deltrusted(char *host)
{
	struct maskstruct *ms;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "deltrusted(\"%s\")\n", host);
#endif
	/* find & remove host */
	for(ms = mystate->trusted; ms != NULL; ms = ms->next) {
		if(!strcasecmp(ms->mask, host)) {
			break;
		}
	}
	if(ms == NULL) {
		return 0;
	}
	if(ms->prev == NULL) {
                /* first */
                if(ms->next == NULL) {
                        /* only */
                        mystate->trusted = NULL;
                } else {
                        ms->next->prev = NULL;
                        mystate->trusted = ms->next;
                }
        } else if(ms->next == NULL) {
                /* last */
                ms->prev->next = NULL;
        } else {
                /* between 2 or more */
                ms->prev->next = ms->next;
                ms->next->prev = ms->prev;
        }
        free(ms->mask);
        free(ms);
        return 1;
}

int delservice(char *host)
{
	struct maskstruct *ms;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "delservice(\"%s\")\n", host);
#endif
	/* find & remove host */
	for(ms = mystate->services; ms != NULL; ms = ms->next) {
		if(!strcasecmp(ms->mask, host)) {
			break;
		}
	}
	if(ms == NULL) {
		return 0;
	}
	if(ms->prev == NULL) {
                /* first */
                if(ms->next == NULL) {
                        /* only */
                        mystate->services = NULL;
                } else {
                        ms->next->prev = NULL;
                        mystate->services = ms->next;
                }
        } else if(ms->next == NULL) {
                /* last */
                ms->prev->next = NULL;
        } else {
                /* between 2 or more */
                ms->prev->next = ms->next;
                ms->next->prev = ms->prev;
        }
        free(ms->mask);
        free(ms);
        return 1;
}

int delscript(char *filename)
{
	struct script *s;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "delscript(\"%s\")\n", filename);
#endif
	/* find & remove script */
	for(s = mystate->scripts; s != NULL; s = s->next) {
		if(!strcasecmp(s->filename, filename)) {
			break;
		}
	}
	if(s == NULL) {
		return 0;
	}
	oerperl_deletescript(s);
	if(s->prev == NULL) {
		/* first */
		if(s->next == NULL) {
			/* only */
			mystate->scripts = NULL;
		} else {
			s->next->prev = NULL;
			mystate->scripts = s->next;
		}
	} else if(s->next == NULL) {
		/* last */
		s->prev->next = NULL;
	} else {
		/* between 2 or more */
		s->prev->next = s->next;
		s->next->prev = s->prev;
	}
	free(s);
	return 1;
}

int delchannel(struct channel *this)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "delchannel(\"%s\")\n", this->name);
#endif
	if(this->joined) {
		/* leave the channel */
		snprintf(timed_str, WRITE_BUFFER_LENGTH, "PART %s", this->name);
		timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CHANNEL_HANDLING, timed_str);
	}
	/* do the normal init */
	initchannel(this);
	/* the following differ from normal initchannel() */
	delalltopics(this);
	initpermbans(this);
	initnickbks(this);
	initwordbks(this);
	initusers(this);
	cleanautheds(this, 1);
	deladverts(this, "all", 1);
	if(this->prev == NULL) {
                /* first */
                if(this->next == NULL) {
                        /* only */
                        mystate->channels = NULL;
                } else {
                        this->next->prev = NULL;
                        mystate->channels = this->next;
                }
        } else if(this->next == NULL) {
                /* last */
                this->prev->next = NULL;
        } else {
                /* between 2 or more */
                this->prev->next = this->next;
                this->next->prev = this->prev;
        }
	free(this);
	return 1;
}

struct maskstruct *editmask(struct maskstruct *first, char *from, char *to)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "editmask(\"%s\", \"%s\", \"%s\")\n", first->mask, from, to);
#endif
	while(first != NULL) {
		if(!strcasecmp(first->mask, from)) {
			break;
		}
		first = first->next;
	}
	if(first == NULL) {
		return NULL;
	}
	free(first->mask);
	if((first->mask = (char *) malloc(strlen(to) + 1)) == NULL) {
		return NULL;
	}
	strcpy(first->mask, to);
	return first;
}

struct maskstruct *addnewusermask(struct channel *this, char *handle, char *mask)
{
	struct botuser *user;
	struct maskstruct *ms;
	struct maskstruct *ms2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "addnewusermask(\"%s\", \"%s\", \"%s\")\n", this->name, handle, mask);
#endif
	for(user = this->users; user != NULL; user = user->next) {
		if(!strcasecmp(user->handle, handle)) {
			break;
		}
	}
	if(user == NULL) {
		return NULL;
	}
	ms = user->firstmask;
	ms2 = ms;
	while(ms != NULL) {
		ms2 = ms;
		if(!strcasecmp(ms->mask, mask)) {
			return NULL;
		}
		ms = ms2->next;
	}
	if((ms = (struct maskstruct *) malloc(sizeof(struct maskstruct))) == NULL) {
		return NULL;
	}
	if((ms->mask = (char *) malloc(strlen(mask) + 1)) == NULL) {
		return NULL;
	}
	strcpy(ms->mask, mask);
	ms->next = NULL;
	ms->prev = NULL;
	if(user->firstmask == NULL) {
		/* first mask, special case */
		user->firstmask = ms;
		return ms;
	}
	/* >=1 masks, normal processing */
	ms2->next = ms;
	ms->prev = ms2;
	return ms;
}

struct advert *addnewadvert(struct channel *this, char *to, char *message)
{
	struct advert *a;
	struct advert *a2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "addnewadvert(\"%s\", \"%s\", \"%s\")\n", this->name, to, message);
#endif
	/* check for static type adverts & valid userhost */
	if(strcasecmp(to, "admins") && strcasecmp(to, "ops") && strcasecmp(to, "other") && strcasecmp(to, "normals") && strcasecmp(to, "all") && !isvaliduserhost(to)) {
		return 0;
	}
	a = this->adverts;
	a2 = a;
	while(a != NULL) {
		a2 = a;
		if(!strcmp(a->to, to) && !strcasecmp(a->message, message)) {
			return NULL;
		}
		a = a2->next;
	}
	if((a = (struct advert *) malloc(sizeof(struct advert))) == NULL) {
		return NULL;
	}
	if((a->to = (char *) malloc(strlen(to) + 1)) == NULL) {
		return NULL;
	}
	strcpy(a->to, to);
	if((a->message = (char *) malloc(strlen(message) + 1)) == NULL) {
		return NULL;
	}
	strcpy(a->message, message);
	a->next = NULL;
	a->prev = NULL;
	if(this->adverts == NULL) {
		/* first advert, special case */
		this->adverts = a;
		return a;
	}
	/* >=1 adverts, normal processing (append) */
	a2->next = a;
	a->prev = a2;
	return a;
}

struct maskstruct *addnewpermban(struct channel *this, char *mask, char *nick, char *reason)
{
	struct maskstruct *ms;
	struct maskstruct *ms2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "addnewpermban(\"%s\", \"%s\", \"%s\", \"%s\")\n", this->name, mask, (nick) ? nick : "(null)", (reason) ? reason : "(null)");
#endif
	ms = this->permbans;
	ms2 = ms;
	while(ms != NULL) {
		ms2 = ms;
		if(!strcasecmp(ms->mask, mask)) {
			return NULL;
		}
		ms = ms2->next;
	}
	if((ms = (struct maskstruct *) malloc(sizeof(struct maskstruct))) == NULL) {
		return NULL;
	}
	if((ms->mask = (char *) malloc(strlen(mask) + 1)) == NULL) {
		return NULL;
	}
	strcpy(ms->mask, mask);
	ms->next = NULL;
	ms->prev = NULL;
	ms->optstring1 = NULL;
	ms->optstring2 = NULL;
	if(nick != NULL) {
		if((ms->optstring1 = (char *) malloc(strlen(nick) + 1)) == NULL) {
			return NULL;
		}
		strcpy(ms->optstring1, nick);
	}
	if(reason != NULL) {
		if((ms->optstring2 = (char *) malloc(strlen(reason) + 1)) == NULL) {
			return NULL;
		}
		strcpy(ms->optstring2, reason);
	}
	if(this->permbans == NULL) {
		/* first permban, special case */
		this->permbans = ms;
		return ms;
	}
	/* >=1 permbans, normal processing */
	ms2->next = ms;
	ms->prev = ms2;
	return ms;
}

struct maskstruct *addnewnickbk(struct channel *this, char *mask, char *nick, char *reason)
{
	struct maskstruct *ms;
	struct maskstruct *ms2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "addnewnickbk(\"%s\", \"%s\", \"%s\", \"%s\")\n", this->name, mask, (nick == NULL) ? "(null)" : nick, (reason == NULL) ? "(null)" : reason);
#endif
	ms = this->nickbks;
	ms2 = ms;
	while(ms != NULL) {
		ms2 = ms;
		if(!strcasecmp(ms->mask, mask)) {
			return NULL;
		}
		ms = ms2->next;
	}
	if((ms = (struct maskstruct *) malloc(sizeof(struct maskstruct))) == NULL) {
		return NULL;
	}
	if((ms->mask = (char *) malloc(strlen(mask) + 1)) == NULL) {
		return NULL;
	}
	strcpy(ms->mask, mask);
	ms->next = NULL;
	ms->prev = NULL;
	ms->optstring1 = NULL;
	ms->optstring2 = NULL;
	if(nick != NULL) {
		if((ms->optstring1 = (char *) malloc(strlen(nick) + 1)) == NULL) {
			return NULL;
		}
		strcpy(ms->optstring1, nick);
	}
	if(reason != NULL) {
		if((ms->optstring2 = (char *) malloc(strlen(reason) + 1)) == NULL) {
			return NULL;
		}
		strcpy(ms->optstring2, reason);
	}
	if(this->nickbks == NULL) {
		/* first nickbk, special case */
		this->nickbks = ms;
		return ms;
	}
	/* >=1 nickbks, normal processing */
	ms2->next = ms;
	ms->prev = ms2;
	return ms;
}

struct maskstruct *addnewwordbk(struct channel *this, char *mask, char *nick, char *reason)
{
	struct maskstruct *ms;
	struct maskstruct *ms2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "addnewwordbk(\"%s\", \"%s\", \"%s\", \"%s\")\n", this->name, mask, (nick == NULL) ? "(null)" : nick, (reason == NULL) ? "(null)" : reason);
#endif
	ms = this->wordbks;
	ms2 = ms;
	while(ms != NULL) {
		ms2 = ms;
		if(!strcasecmp(ms->mask, mask)) {
			return NULL;
		}
		ms = ms2->next;
	}
	if((ms = (struct maskstruct *) malloc(sizeof(struct maskstruct))) == NULL) {
		return NULL;
	}
	if((ms->mask = (char *) malloc(strlen(mask) + 1)) == NULL) {
		return NULL;
	}
	strcpy(ms->mask, mask);
	ms->next = NULL;
	ms->prev = NULL;
	ms->optstring1 = NULL;
	ms->optstring2 = NULL;
	if(nick != NULL) {
		if((ms->optstring1 = (char *) malloc(strlen(nick) + 1)) == NULL) {
			return NULL;
		}
		strcpy(ms->optstring1, nick);
	}
	if(reason != NULL) {
		if((ms->optstring2 = (char *) malloc(strlen(reason) + 1)) == NULL) {
			return NULL;
		}
		strcpy(ms->optstring2, reason);
	}
	if(this->wordbks == NULL) {
		/* first wordbk, special case */
		this->wordbks = ms;
		return ms;
	}
	/* >=1 wordbks, normal processing */
	ms2->next = ms;
	ms->prev = ms2;
	return ms;
}

struct maskstruct *addnewtrusted(char *host)
{
	struct maskstruct *ms;
	struct maskstruct *ms2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "addnewtrusted(\"%s\")\n", host);
#endif
	ms = mystate->trusted;
	ms2 = ms;
	while(ms != NULL) {
		ms2 = ms;
		if(!strcasecmp(ms->mask, host)) {
			return NULL;
		}
		ms = ms2->next;
	}
	if((ms = (struct maskstruct *) malloc(sizeof(struct maskstruct))) == NULL) {
		return NULL;
	}
	if((ms->mask = (char *) malloc(strlen(host) + 1)) == NULL) {
		return NULL;
	}
	strcpy(ms->mask, host);
	ms->optstring1 = NULL;
	ms->optstring2 = NULL;
	ms->next = NULL;
	ms->prev = NULL;
	if(mystate->trusted == NULL) {
		/* first trusted, special case */
		mystate->trusted = ms;
		return ms;
	}
	/* >=1 trusted, normal processing */
	ms2->next = ms;
	ms->prev = ms2;
	return ms;
}

struct script *addnewscript(char *filename)
{
	struct script *s1;
	struct script *s2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "addnewscript(\"%s\")\n", filename);
#endif
	s1 = mystate->scripts;
	s2 = s1;
	while(s1 != NULL) {
		s2 = s1;
		if(!strcasecmp(s1->filename, filename)) {
			return NULL;
		}
		s1 = s2->next;
	}
	if((s1 = (struct script *) malloc(sizeof(struct script))) == NULL) {
		return NULL;
	}
	strncpy(s1->filename, filename, BIGSTRINGLEN);
	s1->eventhandlers = NULL;
	s1->commandhandlers = NULL;
	s1->timers = NULL;
	s1->startup_callback = &PL_sv_undef;
	s1->shutdown_callback = &PL_sv_undef;
	if(!oerperl_loadscript(s1)) {
		free(s1);
		return NULL;
	}
	s1->prev = NULL;
	s1->next = NULL;
	/* first one is a special case */
	if(mystate->scripts == NULL) {
		mystate->scripts = s1;
		return s1;
	}
	/* >=1 scripts, normal case */
	s2->next = s1;
	s1->prev = s2;
	return s1;
}

struct eventhandler *addneweventhandler(struct script *script, char *eventtype, SV *callback)
{
	struct eventhandler *h;
	struct eventhandler *h2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "addneweventhandler(\"%s\", \"%s\")\n", eventtype, SvPV_nolen(callback));
#endif
	h = script->eventhandlers;
	h2 = h;
	while(h != NULL) {
		h2 = h;
		h = h2->next;
	}
	if((h = (struct eventhandler *) malloc(sizeof(struct eventhandler))) == NULL) {
		return NULL;
	}
	memset(h->eventtype, 0, MICROSTRINGLEN + 1);
	strncpy(h->eventtype, eventtype, MICROSTRINGLEN);
	h->callback = callback;
	SvREFCNT_inc(h->callback);
	h->prev = NULL;
	h->next = NULL;
	/* first one is a special case */
	if(script->eventhandlers == NULL) {
		script->eventhandlers = h;
		return h;
	}
	/* >=1 eventhandlers, normal case */
	h2->next = h;
	h->prev = h2;
	return h;
}

struct commandhandler *addnewcommandhandler(struct script *script, char *command, SV *callback)
{
	struct commandhandler *c;
	struct commandhandler *c2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "addnewcommandhandler(\"%s\", \"%s\")\n", command, SvPV_nolen(callback));
#endif
	c = script->commandhandlers;
	c2 = c;
	while(c != NULL) {
		c2 = c;
		c = c2->next;
	}
	if((c = (struct commandhandler *) malloc(sizeof(struct commandhandler))) == NULL) {
		return NULL;
	}
	memset(c->command, 0, TINYSTRINGLEN + 1);
	strncpy(c->command, command, TINYSTRINGLEN);
	c->callback = callback;
	SvREFCNT_inc(c->callback);
	c->prev = NULL;
	c->next = NULL;
	/* first one is a special case */
	if(script->commandhandlers == NULL) {
		script->commandhandlers = c;
		return c;
	}
	/* >=1 commandhandlers, normal case */
	c2->next = c;
	c->prev = c2;
	return c;
}

struct scripttimer *addnewscripttimer(struct script *script, time_t interval, SV *callback)
{
	struct scripttimer *s;
	struct scripttimer *s2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "addnewscripttimer(%ld, \"%s\")\n", interval, SvPV_nolen(callback));
#endif
	s = script->timers;
	s2 = s;
	while(s != NULL) {
		s2 = s;
		s = s2->next;
	}
	if((s = (struct scripttimer *) malloc(sizeof(struct scripttimer))) == NULL) {
		return NULL;
	}
	s->interval = interval;
	s->lastrun = 0;
	s->callback = callback;
	SvREFCNT_inc(s->callback);
	s->prev = NULL;
	s->next = NULL;
	/* first one is a special case */
	if(script->timers == NULL) {
		script->timers = s;
		return s;
	}
	/* >=1 timers, normal case */
	s2->next = s;
	s->prev = s2;
	return s;
}

struct maskstruct *addnewservice(char *host)
{
	struct maskstruct *ms;
	struct maskstruct *ms2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "addnewservice(\"%s\")\n", host);
#endif
	ms = mystate->services;
	ms2 = ms;
	while(ms != NULL) {
		ms2 = ms;
		if(!strcasecmp(ms->mask, host)) {
			return NULL;
		}
		ms = ms2->next;
	}
	if((ms = (struct maskstruct *) malloc(sizeof(struct maskstruct))) == NULL) {
		return NULL;
	}
	if((ms->mask = (char *) malloc(strlen(host) + 1)) == NULL) {
		return NULL;
	}
	strcpy(ms->mask, host);
	ms->optstring1 = NULL;
	ms->optstring2 = NULL;
	ms->next = NULL;
	ms->prev = NULL;
	if(mystate->services == NULL) {
		/* first service, special case */
		mystate->services = ms;
		return ms;
	}
	/* >=1 services, normal processing */
	ms2->next = ms;
	ms->prev = ms2;
	return ms;
}

struct botuser *addbotuser(char *handle, char *options)
{
	struct botuser *botuser;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "addbotuser(\"%s\", \"%s\")\n", handle, options);
#endif
	if((botuser = (struct botuser *) malloc(sizeof(struct botuser))) == NULL) {
		return NULL;
	}
	botuser->password = NULL;
	botuser->firstmask = NULL;
	botuser->prev = NULL;
	botuser->next = NULL;
	if((botuser->handle = (char *) malloc(strlen(handle) + 1)) == NULL) {
		return NULL;
	}
	strcpy(botuser->handle, handle);
	if((botuser->options = (char *) malloc(strlen(options) + 1)) == NULL) {
		return NULL;
	}
	strcpy(botuser->options, options);
	return botuser;
}

struct botuser *addnewuser(struct channel *this, char *handle, char *options)
{
	struct botuser *bu;
	struct botuser *bu2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "addnewuser(\"%s\", \"%s\", \"%s\")\n", this->name, handle, options);
#endif
	bu = this->users;
	bu2 = bu;
	while(bu != NULL) {
		bu2 = bu;
		if(!strcasecmp(bu->handle, handle)) {
			return NULL;
		}
		bu = bu2->next;
	}
	if((bu = addbotuser(handle, options)) == NULL) {
		return NULL;
	}
	if(this->users == NULL) {
		this->users = bu;
		return bu;
	}
	/* >=1 users, normal processing */
	bu2->next = bu;
	bu->prev = bu2;
	return bu;
}

struct botuser *addnewadmin(char *handle, char *options)
{
	struct botuser *admin;
	struct botuser *admin2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "addnewadmin(\"%s\", \"%s\")\n", handle, options);
#endif
	admin = mystate->admins;
	admin2 = admin;
	while(admin != NULL) {
		admin2 = admin;
		if(!strcasecmp(admin->handle, handle)) {
			return NULL;
		}
		admin = admin2->next;
	}
	if((admin = addbotuser(handle, options)) == NULL) {
		return NULL;
	}
	if(mystate->admins == NULL) {
		mystate->admins = admin;
		return admin;
	}
	/* >1 admins, normal processing */
	admin2->next = admin;
	admin->prev = admin2;
	return admin;
}

struct maskstruct *addnewkickreason(char *reason)
{
	struct maskstruct *ms;
	struct maskstruct *ms2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "addnewkickreason(\"%s\")\n", reason);
#endif
	ms = mystate->kickreasons;
	ms2 = ms;
	while(ms != NULL) {
		ms2 = ms;
		if(!strcasecmp(ms->mask, reason)) {
			return NULL;
		}
		ms = ms2->next;
	}
	if((ms = (struct maskstruct *) malloc(sizeof(struct maskstruct))) == NULL) {
		return NULL;
	}
	if((ms->mask = (char *) malloc(strlen(reason) + 1)) == NULL) {
		return NULL;
	}
	strcpy(ms->mask, reason);
	ms->optstring1 = NULL;
	ms->optstring2 = NULL;
	ms->next = NULL;
	ms->prev = NULL;
	mystate->kickrs++;
	if(mystate->kickreasons == NULL) {
		/* first kickreason, special case */
		mystate->kickreasons = ms;
		return ms;
	}
	/* >=1 kick reasons, normal processing */
	ms2->next = ms;
	ms->prev = ms2;
	return ms;
}

struct server *addnewserver(char *serverhost, int serverport, int servermodes, int pingfrequency, int protected_ircops, int linenoise, char *password)
{
	struct server *s;
	struct server *s2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "addnewserver(\"%s\", %d, %d, %d, %d, %d, \"%s\"\n", serverhost, serverport, servermodes, pingfrequency, protected_ircops, linenoise, (password == NULL) ? "(null)" : password);
#endif
	s = mystate->servers;
	s2 = s;
	while(s != NULL) {
		s2 = s;
		if(!strcasecmp(s->serverhost, serverhost) && s->serverport == serverport && s->servermodes == servermodes && s->pingfrequency == pingfrequency && s->protected_ircops == protected_ircops && s->linenoise == linenoise) {
			/* server already exists */
			return NULL;
		}
		s = s2->next;
	}
	if((s = (struct server *) malloc(sizeof(struct server))) == NULL) {
		return NULL;
	}
	strncpy(s->serverhost, serverhost, HOSTLEN);
	memset(s->serverhost_r, 0, HOSTLEN + 1);
	s->serverport = serverport;
	s->servermodes = servermodes;
	s->pingfrequency = pingfrequency;
	s->protected_ircops = protected_ircops;
	s->used = 0;
	s->connected = 0;
	s->stoned = 0;
	s->linenoise = linenoise;
	s->rx = 0;
	s->tx = 0;
	s->linkup = 0;
	memset(s->password, 0, TINYSTRINGLEN + 1);
	if(password != NULL) {
		strncpy(s->password, password, TINYSTRINGLEN);
	}
	memset(&s->registered, 0, sizeof(struct registered));
	s->lastping = 0;
	s->prev = NULL;
	s->next = NULL;
	if(mystate->servers == NULL) {
		/* first server, special case */
		mystate->servers = s;
		return s;
	}
	/* >=1 servers, normal case */
	s2->next = s;
	s->prev = s2;
	return s;
}

struct channel *addnewchannel(char *name)
{
	struct channel *this;
	struct channel *this2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "addnewchannel(\"%s\")\n", name);
#endif
	this = mystate->channels;
	this2 = this;
	while(this != NULL) {
		this2 = this;
		if(!strcasecmp(this->name, name)) {
			return NULL;
		}
		this = this2->next;
	}
	if((this = (struct channel *) malloc(sizeof(struct channel))) == NULL) {
		return NULL;
	}
	strncpy(this->name, name, CHANLEN);
	memset(this->mode, 0, CHANLEN + 1);
	memset(this->key, 0, CHANLEN + 1);
	memset(this->limit, 0, CHANLEN + 1);
	this->joined = 0;
	this->joining = 0;
	this->setchanmode = 0;
	this->chankeyisset = 0;
	this->chanlimitisset = 0;
	this->rejoin_at = 0;
	this->last_quote = 0;
	this->last_advert = 0;
	this->join_ts = 0;
	this->nickcount = 0;
	this->i_am_op = 0;
	this->bantype = OER_BAN_TYPE_HOST;
	this->allhostsknown = 0;
	this->whoquery = 0;
	this->requestop_now = 1;
	this->requestop = mystate->now + 300;
	this->seencounter = 0;
	this->lastcounter = 0;
	memset(this->chanflags, 0, FLAGLEN + 1);
	this->last = NULL;
	this->seen = NULL;
	this->nicks = NULL;
	this->pubmsgs = NULL;
	this->nickchanges = NULL;
	this->parts = NULL;
	this->joins = NULL;
	memset(&this->locked, 0, sizeof(struct locked));
	memset(&this->floodvars, 0, sizeof(struct floodvars));
	/* set the flood defaults, run-time changable */
	this->floodvars.repeat_expire = PUBMSG_FLOOD_REPEAT_EXPIRE;
	this->floodvars.repeat_limit = PUBMSG_FLOOD_REPEAT;
	this->floodvars.interval = PUBMSG_FLOOD_INTERVAL;
	this->floodvars.lines = PUBMSG_FLOOD_LINES;
	this->floodvars.chars = PUBMSG_FLOOD_CHARS;
	this->floodvars.nickflood_expire = NICKFLOOD_EXPIRE;
	this->floodvars.nickflood_changes = NICKFLOOD_CHANGES;
	/* .. */
	memset(&this->banvars, 0, sizeof(struct banvars));
	/* set the ban time defaults, run-time changable */
	this->banvars.auto_rejoin = OER_BANVARS_AUTO_REJOIN;
	this->banvars.part_rejoin = OER_BANVARS_PART_REJOIN;
	this->banvars.public_flood = OER_BANVARS_PUBLIC_FLOOD;
	this->banvars.public_flood_repeat = OER_BANVARS_PUBLIC_FLOOD_REPEAT;
	this->banvars.bad_word = OER_BANVARS_BAD_WORD;
	this->banvars.bad_nick = OER_BANVARS_BAD_NICK;
	this->banvars.normal_ban = OER_BANVARS_NORMAL_BAN;
	/* .. */
	this->permbans = NULL;
	this->nickbks = NULL;
	this->wordbks = NULL;
	this->users = NULL;
	this->autheds = NULL;
	this->topic_change = 0;
	this->topic_list = 0;
	this->synced = 0;
	this->topics = NULL;
	this->adverts = NULL;
	this->mmodes = NULL;
	this->channelbans = NULL;
	this->prev = NULL;
	this->next = NULL;
	/* first one is a special case */
	if(mystate->channels == NULL) {
		mystate->channels = this;
		return this;
	}
	/* >=1 channels, normal case */
	this2->next = this;
	this->prev = this2;
	return this;
}

struct channel *getchptr(char *channel)
{
        struct channel *this;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "getchptr(\"%s\")\n", channel);
#endif
        for(this = mystate->channels; this != NULL; this = this->next) {
                if(!strcasecmp(this->name, channel)) {
                        return this;
                }
        }
        return NULL;
}

int loadall(void)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "loadall()\n");
#endif
	/* we only require loadconf() and loadadmins() to succeed */
	if(!loadconf()) {
#ifdef OER_DEBUG
                oer_debug(OER_DEBUG_INFO, "loadall->loadconf() failed\n");
#endif
		return 0;
        }
        if(!loadadmins()) {
#ifdef OER_DEBUG
                oer_debug(OER_DEBUG_INFO, "loadall->loadadmins() failed\n");
#endif
		return 0;
        }
        if(!loadchannels()) {
#ifdef OER_DEBUG
                oer_debug(OER_DEBUG_INFO, "loadall->loadchannels() failed\n");
#endif
        }
        if(!loadtrusted()) {
#ifdef OER_DEBUG
                oer_debug(OER_DEBUG_INFO, "loadall->loadtrusted() failed\n");
#endif
        }
        if(!loadservices()) {
#ifdef OER_DEBUG
                oer_debug(OER_DEBUG_INFO, "loadall->loadservices() failed\n");
#endif
        }
        if(!loadkickreasons()) {
#ifdef OER_DEBUG
                oer_debug(OER_DEBUG_INFO, "loadall->loadkickreasons() failed\n");
#endif
        }
        if(!loadchaninfo()) {
#ifdef OER_DEBUG
                oer_debug(OER_DEBUG_INFO, "loadall->loadchaninfo() failed\n");
#endif
        }
        if(!loadscripts()) {
#ifdef OER_DEBUG
                oer_debug(OER_DEBUG_INFO, "loadall->loadscripts() failed\n");
#endif
        }
	return 1;
}

int loadconf(void)
{
	FILE *fp;
        char filename[STRINGLEN + 1];
        char stringbuffer[BIGSTRINGLEN + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "loadconf()\n");
#endif
	snprintf(filename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".conf");
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "loadconf->loading %s\n", filename);
#endif
	if((fp = fopen(filename, "r")) == NULL) {
                return 0;
        }
        while(fgets(stringbuffer, BIGSTRINGLEN, fp)) {
		striplf(stringbuffer);
		if(emptyline(stringbuffer)) {
			continue;
		}
		if(iscomment(stringbuffer)) {
			continue;
		}
                if(!parseconf(stringbuffer)) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_WARNING, "loadconf->non-parsable line: '%s'\n", stringbuffer);
#endif
                }
        }
        fclose(fp);
        return 1;
}

int saveall(void)
{	
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "saveall()\n");
#endif
	if(!saveconf()) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "saveall->saveconf() failed\n");
#endif
		return 0;
	}
	if(!savechannels()) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "saveall->savechannels() failed\n");
#endif
		return 0;
	}
	if(!savetrusted()) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "saveall->savetrusted() failed\n");
#endif
		return 0;
	}
	if(!saveservices()) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "saveall->saveservices() failed\n");
#endif
		return 0;
	}
	if(!saveadmins()) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "saveall->saveadmins() failed\n");
#endif
		return 0;
	}
	if(!savekickreasons()) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "saveall->savekickreasons() failed\n");
#endif
		return 0;
	}
	if(!savechaninfo()) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "saveall->savechaninfo() failed\n");
#endif
		return 0;
	}
	if(!savescripts()) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "saveall->savescripts() failed\n");
#endif
		return 0;
	}
	return 1;
}

int saveconf(void)
{
	FILE *fp;
	char *ptr;
        char filename[STRINGLEN + 1];
        char newfilename[STRINGLEN + 1];
        char bfilename[STRINGLEN + 1];
        char stringbuffer[BIGSTRINGLEN + 1];
	char prefixes[TINYSTRINGLEN + 1];
	char tmpuser[USERLEN + 1];
	struct server *server;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "saveconf()\n");
#endif
	snprintf(filename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".conf.saving");
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "saveconf->saving to %s\n", filename);
#endif
	if((fp = fopen(filename, "w")) == NULL) {
                return 0;
        }
	if(strcasecmp(mystate->nick, mystate->getnick)) {
		snprintf(stringbuffer, BIGSTRINGLEN, "nick::%s\n", mystate->getnick);
	} else {
		snprintf(stringbuffer, BIGSTRINGLEN, "nick::%s\n", mystate->nick);
	}
	if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_ERROR, "saveconf->fputs() failed: %s\n", strerror(errno));
#endif
		fclose(fp);
		return 0;
	}
        snprintf(stringbuffer, BIGSTRINGLEN, "altnick::%s\n", mystate->altnick);
	if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_ERROR, "saveconf->fputs() failed: %s\n", strerror(errno));
#endif
		fclose(fp);
		return 0;
	}
	strncpy(prefixes, CONNECTION_STATE_PREFIXES, TINYSTRINGLEN);
	strncpy(tmpuser, mystate->user, USERLEN);
	ptr = tmpuser;
	while(ptr && (index(prefixes, *ptr) != NULL)) {
		/* strip the prefixes added by the server */
		ptr++;
	}
        snprintf(stringbuffer, BIGSTRINGLEN, "user::%s\n", ptr);
	if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_ERROR, "saveconf->fputs() failed: %s\n", strerror(errno));
#endif
		fclose(fp);
		return 0;
	}
        snprintf(stringbuffer, BIGSTRINGLEN, "usermode::%s\n", mystate->mode);
	if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_ERROR, "saveconf->fputs() failed: %s\n", strerror(errno));
#endif
		fclose(fp);
		return 0;
	}
        snprintf(stringbuffer, BIGSTRINGLEN, "flags::%s\n", mystate->flags);
	if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_ERROR, "saveconf->fputs() failed: %s\n", strerror(errno));
#endif
		fclose(fp);
		return 0;
	}
        snprintf(stringbuffer, BIGSTRINGLEN, "prefix::%s\n", mystate->prefix);
	if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_ERROR, "saveconf->fputs() failed: %s\n", strerror(errno));
#endif
		fclose(fp);
		return 0;
	}
        snprintf(stringbuffer, BIGSTRINGLEN, "realname::%s\n", mystate->realname);
	if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_ERROR, "saveconf->fputs() failed: %s\n", strerror(errno));
#endif
		fclose(fp);
		return 0;
	}
        if(strlen(mystate->vhost)) {
                snprintf(stringbuffer, BIGSTRINGLEN, "vhost::%s\n", mystate->vhost);
		if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_ERROR, "saveconf->fputs() failed: %s\n", strerror(errno));
#endif
			fclose(fp);
			return 0;
		}
        }
        for(server = mystate->servers; server != NULL; server = server->next) {
                snprintf(stringbuffer, BIGSTRINGLEN, "server::%s::%d::%d::%d::%d::%d::%s\n", server->serverhost, server->serverport, server->servermodes, server->pingfrequency, server->protected_ircops, server->linenoise, (strlen(server->password)) ? server->password : "");
		if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_ERROR, "saveconf->fputs() failed: %s\n", strerror(errno));
#endif
			fclose(fp);
			return 0;
		}
        }
        if(mystate->qauth.hasauth) {
                snprintf(stringbuffer, BIGSTRINGLEN, "qauth::%s::%s::%s\n", mystate->qauth.q, mystate->qauth.name, mystate->qauth.password);
		if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_ERROR, "saveconf->fputs() failed: %s\n", strerror(errno));
#endif
			fclose(fp);
			return 0;
		}
        }
        if(strlen(mystate->proxysetup)) {
                snprintf(stringbuffer, BIGSTRINGLEN, "proxysetup::%s\n", mystate->proxysetup);
		if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_ERROR, "saveconf->fputs() failed: %s\n", strerror(errno));
#endif
			fclose(fp);
			return 0;
		}
        }
        if(fclose(fp) == EOF) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_ERROR, "saveconf->fclose() failed: %s\n", strerror(errno));
#endif
		return 0;
	}
	snprintf(newfilename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".conf");
	if(index(mystate->flags, (int)'b') != NULL && (getfilesize(newfilename) > 0)) {
		/* keep old version */
		snprintf(bfilename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".conf.bak");
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "saveconf->keeping old %s, renaming to %s\n", newfilename, bfilename);
#endif
		if(rename(newfilename, bfilename) < 0) {
			return 0;
		}
	}
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_INFO, "saveconf->renaming %s to %s\n", filename, newfilename);
#endif
        if(rename(filename, newfilename) < 0) {
                return 0;
        }
        return 1;
}

int parseconf(char *paramline)
{
	int ppos;
	int ping;
	int linenoise;
	char p1[STRINGLEN + 1];
	char p2[STRINGLEN + 1];
	char p3[STRINGLEN + 1];
	char p4[STRINGLEN + 1];
	char p5[STRINGLEN + 1];
	char p6[STRINGLEN + 1];
	char p7[STRINGLEN + 1];
	char p8[STRINGLEN + 1];
	char *pwptr;
	struct server *server;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "parseconf(\"%s\")\n", paramline);
#endif
	ppos = 0;
	if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p1, STRINGLEN, 1)) < 0) {
		return 0;
	}
	if(!strcasecmp(p1, "nick")) {
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p2, STRINGLEN, 1)) < 0) {
			return 0;
		}
		strncpy(mystate->nick, p2, NICKLEN);
		strncpy(mystate->getnick, p2, NICKLEN);
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "parseconf->my nick is %s\n", mystate->nick);
#endif
		return 1;
	}
	if(!strcasecmp(p1, "altnick")) {
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p2, STRINGLEN, 1)) < 0) {
			return 0;
		}
		strncpy(mystate->altnick, p2, NICKLEN);
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "parseconf->my alt nick is %s\n", mystate->altnick);
#endif
		return 1;
	}
	if(!strcasecmp(p1, "realname")) {
		if(!strlen(paramline + ppos)) {
			return 0;
		}
		strncpy(mystate->realname, paramline + ppos, STRINGLEN);
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "parseconf->my IRC REALNAME is %s\n", mystate->realname);
#endif
		return 1;
	}
	if(!strcasecmp(p1, "user")) {
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p2, STRINGLEN, 1)) < 0) {
			return 0;
		}
		strncpy(mystate->user, p2, USERLEN);
		mystate->customuser = 1;
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "parseconf->my user is %s\n", mystate->user);
#endif
		return 1;
	}
	if(!strcasecmp(p1, "usermode")) {
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p2, STRINGLEN, 1)) < 0) {
			return 0;
		}
		strncpy(mystate->mode, p2, CHANLEN);
		mystate->newmode = 1;
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "parseconf->my usermode is %s\n", mystate->mode);
#endif
		return 1;
	}
	if(!strcasecmp(p1, "flags")) {
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p2, STRINGLEN, 1)) < 0) {
			return 0;
		}
		strncpy(mystate->flags, p2, FLAGLEN);
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "parseconf->my flags are %s\n", mystate->flags);
#endif
		return 1;
	}
	if(!strcasecmp(p1, "prefix")) {
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p2, STRINGLEN, 1)) < 0) {
			return 0;
		}
		if(index(p2, (int)'"')) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_FATAL, "parseconf->invalid prefix %s\n", p2);
#endif
			return 0;
		}
		strncpy(mystate->prefix, p2, MICROSTRINGLEN);
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "parseconf->my prefix is %s\n", mystate->prefix);
#endif
		return 1;
	}
	if(!strcasecmp(p1, "vhost")) {
                if(!isvalidhost(paramline + ppos)) {
                        return 0;
                }
                strncpy(mystate->vhost, paramline + ppos, HOSTLEN);
#ifdef OER_DEBUG
                oer_debug(OER_DEBUG_INFO, "parseconf->using virtual host %s\n", mystate->vhost);
#endif
                return 1;
        }
	if(!strcasecmp(p1, "server")) {
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p2, STRINGLEN, 1)) < 0) {
			return 0;
		}
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p3, STRINGLEN, 1)) < 0) {
			return 0;
		}
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p4, STRINGLEN, 1)) < 0) {
			return 0;
		}
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p5, STRINGLEN, 1)) < 0) {
			return 0;
		}
		ping = atoi(p5);
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p6, STRINGLEN, 1)) < 0) {
			return 0;
		}
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p7, STRINGLEN, 1)) < 0) {
			return 0;
		}
		linenoise = atoi(p7);
		if(linenoise != 0 && (linenoise < OER_LINENOISE_INTERVAL_MIN || linenoise > ping)) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_WARNING, "parseconf->invalid linenoise value %d, disabled\n", linenoise);
#endif
			linenoise = 0;
		}
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p8, STRINGLEN, 1)) < 0) {
			return 0;
		}
		pwptr = (strlen(p8)) ? p8 : NULL;
		if((server = addnewserver(p2, atoi(p3), atoi(p4), ping, atoi(p6), linenoise, pwptr)) == NULL) {
			return 0;
		}
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "parseconf->added IRC server host: %s port: %d servermodes: %d ping frequency: %d protected ircops: %d linenoise: %d password: %s\n", server->serverhost, server->serverport, server->servermodes, server->pingfrequency, server->protected_ircops, server->linenoise, (pwptr == NULL) ? "(null)" : pwptr);
#endif
		return 1;
	}
	if(!strcasecmp(p1, "qauth")) {
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p2, STRINGLEN, 1)) < 0) {
			return 0;
		}
		strncpy(mystate->qauth.q, p2, USERHOSTLEN);
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p3, STRINGLEN, 1)) < 0) {
			return 0;
		}
		strncpy(mystate->qauth.name, p3, NICKLEN);
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p4, STRINGLEN, 1)) < 0) {
			return 0;
		}
		strncpy(mystate->qauth.password, p4, TINYSTRINGLEN);
		mystate->qauth.hasauth = 1;
		mystate->qauth.authed = 0;
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "parseconf->got qauth information for %s\n", mystate->qauth.q);
#endif
		return 1;
	}
	if(!strcasecmp(p1, "proxysetup")) {
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p2, STRINGLEN, 1)) < 0) {
			return 0;
		}
		strncpy(mystate->proxysetup, p2, STRINGLEN);
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "parseconf->got proxysetup information\n");
#endif
		return 1;
	}
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_WARNING, "parseconf->unknown line: '%s'\n", paramline);
#endif
	return 1;
}

int loadchannels(void)
{
	FILE *fp;
        char filename[STRINGLEN + 1];
        char stringbuffer[BIGSTRINGLEN + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "loadchannels()\n");
#endif
	snprintf(filename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".channels");
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "loadchannels->loading %s\n", filename);
#endif
	if((fp = fopen(filename, "r")) == NULL) {
                return 0;
        }
	while(fgets(stringbuffer, BIGSTRINGLEN, fp)) {
                striplf(stringbuffer);
                if(emptyline(stringbuffer)) {
                        continue;
                }
                if(iscomment(stringbuffer)) {
                        continue;
                }
                if(!parsechannels(stringbuffer)) {
#ifdef OER_DEBUG
                        oer_debug(OER_DEBUG_WARNING, "loadchannels->non-parsable line: '%s'\n", stringbuffer);
#endif
                }
        }
        fclose(fp);
        return 1;
}

int savechannels(void)
{
	FILE *fp;
        char filename[STRINGLEN + 1];
        char newfilename[STRINGLEN + 1];
	char bfilename[STRINGLEN + 1];
        char stringbuffer[BIGSTRINGLEN + 1];
	struct channel *this;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "savechannels()\n");
#endif
	snprintf(filename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".channels.saving");
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "savechannels->saving to %s\n", filename);
#endif
	if((fp = fopen(filename, "w")) == NULL) {
                return 0;
        }
	for(this = mystate->channels; this != NULL; this = this->next) {
		snprintf(stringbuffer, BIGSTRINGLEN, "channel::%s::%s::%s::%s::%s\n", this->name, (strlen(this->key)) ? this->key : "", this->mode, this->chanflags, (strlen(this->limit))  ? this->limit : "");
                if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_ERROR, "savechannels->fputs() failed: %s\n", strerror(errno));
#endif
			fclose(fp);
			return 0;
		}
        }
        if(fclose(fp) == EOF) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_ERROR, "savechannels->fclose() failed: %s\n", strerror(errno));
#endif
		return 0;
	}
	snprintf(newfilename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".channels");
	if(index(mystate->flags, (int)'b') != NULL && (getfilesize(newfilename) > 0)) {
		/* keep old version */
		snprintf(bfilename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".channels.bak");
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "savechannels->keeping old %s, renaming to %s\n", newfilename, bfilename);
#endif
		if(rename(newfilename, bfilename) < 0) {
			return 0;
		}
	}
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_INFO, "savechannels->renaming %s to %s\n", filename, newfilename);
#endif
        if(rename(filename, newfilename) < 0) {
                return 0;
        }
        return 1;
}

int parsechannels(char *paramline)
{
	int ppos;
	char p1[STRINGLEN + 1];
	char p2[STRINGLEN + 1];
	char p3[STRINGLEN + 1];
	char p4[STRINGLEN + 1];
	char p5[STRINGLEN + 1];
	char p6[STRINGLEN + 1];
	struct channel *this;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "parsechannels(\"%s\")\n", paramline);
#endif
	ppos = 0;
	if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p1, STRINGLEN, 1)) < 0) {
		return 0;
	}
	if(!strcasecmp(p1, "channel")) {
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p2, STRINGLEN, 1)) < 0) {
			return 0;
		}
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p3, STRINGLEN, 1)) < 0) {
			return 0;
		}
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p4, STRINGLEN, 1)) < 0) {
			return 0;
		}
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p5, STRINGLEN, 1)) < 0) {
			return 0;
		}
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p6, STRINGLEN, 1)) < 0) {
			return 0;
		}
		if(!isvalidchannel(p2)) {
			return 0;
		}
		if((this = addnewchannel(p2)) == NULL) {
			return 0;
		}
		if(strlen(p3) > 0) {
			setchankey(this, p3);
		}
		if(strlen(p6) > 0) {
			setchanlimit(this, p6);
		}
		strncpy(this->mode, p4, CHANLEN);
		this->setchanmode = 1;
		sortstring(p5);
		strncpy(this->chanflags, p5, FLAGLEN);
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "parsechannels->added channel %s with key %s modes %s flags %s and limit %s\n", this->name, this->key, this->mode, this->chanflags, this->limit);
#endif
		return 1;
	}
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_WARNING, "parsechannels->unknown line: '%s'\n", paramline);
#endif
        return 1;
}

int loadtrusted(void)
{
	FILE *fp;
        char filename[STRINGLEN + 1];
        char stringbuffer[BIGSTRINGLEN + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "loadtrusted()\n");
#endif
	snprintf(filename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".trusted");
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "loadtrusted->loading %s\n", filename);
#endif
	if((fp = fopen(filename, "r")) == NULL) {
                return 0;
        }
	while(fgets(stringbuffer, BIGSTRINGLEN, fp)) {
                striplf(stringbuffer);
                if(emptyline(stringbuffer)) {
                        continue;
                }
                if(iscomment(stringbuffer)) {
                        continue;
                }
                if(!parsetrusted(stringbuffer)) {
#ifdef OER_DEBUG
                        oer_debug(OER_DEBUG_WARNING, "loadtrusted->non-parsable line: '%s'\n", stringbuffer);
#endif 
                }
        }
        fclose(fp);
        return 1;
}

int savetrusted(void)
{
	FILE *fp;
        char filename[STRINGLEN + 1];
        char newfilename[STRINGLEN + 1];
	char bfilename[STRINGLEN + 1];
        char stringbuffer[BIGSTRINGLEN + 1];
	struct maskstruct *ms;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "savetrusted()\n");
#endif
	snprintf(filename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".trusted.saving");
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "savetrusted->saving to %s\n", filename);
#endif
	if((fp = fopen(filename, "w")) == NULL) {
                return 0;
        }
	for(ms = mystate->trusted; ms != NULL; ms = ms->next) {
                snprintf(stringbuffer, BIGSTRINGLEN, "trusted::%s\n", ms->mask);
                if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_ERROR, "savetrusted->fputs() failed: %s\n", strerror(errno));
#endif
			fclose(fp);
			return 0;
		}
        }
        if(fclose(fp) == EOF) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_ERROR, "savetrusted->fclose() failed: %s\n", strerror(errno));
#endif
		return 0;
	}
	snprintf(newfilename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".trusted");
	if(index(mystate->flags, (int)'b') != NULL && (getfilesize(newfilename) > 0)) {
		/* keep old version */
		snprintf(bfilename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".trusted.bak");
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "savetrusted->keeping old %s, renaming to %s\n", newfilename, bfilename);
#endif
		if(rename(newfilename, bfilename) < 0) {
			return 0;
		}
	}
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_INFO, "savetrusted->renaming %s to %s\n", filename, newfilename);
#endif
        if(rename(filename, newfilename) < 0) {
                return 0;
        }
        return 1;
}

int parsetrusted(char *paramline)
{
	int ppos;
	char p1[STRINGLEN + 1];
	char p2[STRINGLEN + 1];
	struct maskstruct *ms;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "parsetrusted(\"%s\")\n", paramline);
#endif
	ppos = 0;
	if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p1, STRINGLEN, 1)) < 0) {
		return 0;
	}
	if(!strcasecmp(p1, "trusted")) {
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p2, STRINGLEN, 1)) < 0) {
			return 0;
		}
		if((ms = addnewtrusted(p2)) == NULL) {
			return 0;
		}
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "parsetrusted->added trusted host %s\n", ms->mask);
#endif
		return 1;
	}
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_WARNING, "parsetrusted->unknown line: '%s'\n", paramline);
#endif
        return 1;
}

int loadservices(void)
{
	FILE *fp;
        char filename[STRINGLEN + 1];
        char stringbuffer[BIGSTRINGLEN + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "loadservices()\n");
#endif
	snprintf(filename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".services");
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "loadservices->loading %s\n", filename);
#endif
	if((fp = fopen(filename, "r")) == NULL) {
                return 0;
        }
	while(fgets(stringbuffer, BIGSTRINGLEN, fp)) {
                striplf(stringbuffer);
                if(emptyline(stringbuffer)) {
                        continue;
                }
                if(iscomment(stringbuffer)) {
                        continue;
                }
                if(!parseservices(stringbuffer)) {
#ifdef OER_DEBUG
                        oer_debug(OER_DEBUG_WARNING, "loadservices->non-parsable line: '%s'\n", stringbuffer);
#endif
                }
        }
        fclose(fp);
        return 1;
}

int loadscripts(void)
{
	FILE *fp;
        char filename[STRINGLEN + 1];
        char stringbuffer[BIGSTRINGLEN + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "loadscripts()\n");
#endif
	snprintf(filename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".scripts");
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "loadscripts->loading %s\n", filename);
#endif
	if((fp = fopen(filename, "r")) == NULL) {
                return 0;
        }
	while(fgets(stringbuffer, BIGSTRINGLEN, fp)) {
                striplf(stringbuffer);
                if(emptyline(stringbuffer)) {
                        continue;
                }
                if(iscomment(stringbuffer)) {
                        continue;
                }
                if(!parsescripts(stringbuffer)) {
#ifdef OER_DEBUG
                        oer_debug(OER_DEBUG_WARNING, "loadscripts->non-parsable line: '%s'\n", stringbuffer);
#endif
                }
        }
        fclose(fp);
        return 1;
}

int saveservices(void)
{
	FILE *fp;
        char filename[STRINGLEN + 1];
        char newfilename[STRINGLEN + 1];
	char bfilename[STRINGLEN + 1];
        char stringbuffer[BIGSTRINGLEN + 1];
	struct maskstruct *ms;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "saveservices()\n");
#endif
	snprintf(filename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".services.saving");
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "saveservices->saving to %s\n", filename);
#endif
	if((fp = fopen(filename, "w")) == NULL) {
                return 0;
        }
	for(ms = mystate->services; ms != NULL; ms = ms->next) {
                snprintf(stringbuffer, BIGSTRINGLEN, "service::%s\n", ms->mask);
                if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_ERROR, "saveservices->fputs() failed: %s\n", strerror(errno));
#endif
			fclose(fp);
			return 0;
		}
        }
        if(fclose(fp) == EOF) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_ERROR, "saveservices->fclose() failed: %s\n", strerror(errno));
#endif
		return 0;
	}
	snprintf(newfilename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".services");
	if(index(mystate->flags, (int)'b') != NULL && (getfilesize(newfilename) > 0)) {
		/* keep old version */
		snprintf(bfilename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".services.bak");
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "saveservices->keeping old %s, renaming to %s\n", newfilename, bfilename);
#endif
		if(rename(newfilename, bfilename) < 0) {
			return 0;
		}
	}
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_INFO, "saveservices->renaming %s to %s\n", filename, newfilename);
#endif
        if(rename(filename, newfilename) < 0) {
                return 0;
        }
        return 1;
}

int parseservices(char *paramline)
{
	int ppos;
	char p1[STRINGLEN + 1];
	char p2[STRINGLEN + 1];
	struct maskstruct *ms;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "parseservices(\"%s\")\n", paramline);
#endif
	ppos = 0;
	if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p1, STRINGLEN, 1)) < 0) {
		return 0;
	}
	if(!strcasecmp(p1, "service")) {
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p2, STRINGLEN, 1)) < 0) {
			return 0;
		}
		if((ms = addnewservice(p2)) == NULL) {
			return 0;
		}
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "parseservices->added service %s\n", ms->mask);
#endif
		return 1;
	}
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_WARNING, "parseservices->unknown line: '%s'\n", paramline);
#endif
        return 1;
}

int savescripts(void)
{
	FILE *fp;
	char filename[STRINGLEN + 1];
	char newfilename[STRINGLEN + 1];
	char bfilename[STRINGLEN + 1];
	char stringbuffer[BIGSTRINGLEN + 1];
	struct script *s;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "savescripts()\n");
#endif
	snprintf(filename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".scripts.saving");
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "savescripts->saving to %s\n", filename);
#endif
	if((fp = fopen(filename, "w")) == NULL) {
		return 0;
	}
	for(s = mystate->scripts; s != NULL; s = s->next) {
		snprintf(stringbuffer, BIGSTRINGLEN, "script::%s\n", s->filename);
		if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_ERROR, "savescripts->fputs() failed: %s\n", strerror(errno));
#endif
			fclose(fp);
			return 0;
		}
	}
	if(fclose(fp) == EOF) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_ERROR, "savescripts->fclose() failed: %s\n", strerror(errno));
#endif
		return 0;
	}
	snprintf(newfilename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".scripts");
	if(index(mystate->flags, (int)'b') != NULL && (getfilesize(newfilename) > 0)) {
		/* keep old version */
		snprintf(bfilename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".scripts.bak");
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "savescripts->keeping old %s, renaming to %s\n", newfilename, bfilename);
#endif
		if(rename(newfilename, bfilename) < 0) {
			return 0;
		}
	}
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "savescripts->renaming %s to %s\n", filename, newfilename);
#endif
	if(rename(filename, newfilename) < 0) {
		return 0;
	}
	return 1;
}

int parsescripts(char *paramline)
{
	int ppos;
	char p1[STRINGLEN + 1];
	char p2[STRINGLEN + 1];
	struct script *s;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "parsescripts(\"%s\")\n", paramline);
#endif
	ppos = 0;
	if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p1, STRINGLEN, 1)) < 0) {
		return 0;
	}
	if(!strcasecmp(p1, "script")) {
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p2, STRINGLEN, 1)) < 0) {
			return 0;
		}
		if((s = addnewscript(p2)) == NULL) {
			return 0;
		}
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "parsescripts->added script %s\n", s->filename);
#endif
		return 1;
	}
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_WARNING, "parsescripts->unknown line: '%s'\n", paramline);
#endif
	return 1;
}

int loadadmins(void)
{
	FILE *fp;
        char filename[STRINGLEN + 1];
        char stringbuffer[BIGSTRINGLEN + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "loadadmins()\n");
#endif
	snprintf(filename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".admins");
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "loadadmins->loading %s\n", filename);
#endif
	if((fp = fopen(filename, "r")) == NULL) {
                return 0;
        }
	while(fgets(stringbuffer, BIGSTRINGLEN, fp)) {
                striplf(stringbuffer);
                if(emptyline(stringbuffer)) {
                        continue;
                }
                if(iscomment(stringbuffer)) {
                        continue;
                }
                if(!parseadmins(stringbuffer)) {
#ifdef OER_DEBUG
                        oer_debug(OER_DEBUG_WARNING, "loadadmins->non-parsable line: '%s'\n", stringbuffer);
#endif
                }
        }
        fclose(fp);
        return 1;
}

int saveadmins(void)
{
	FILE *fp;
        char filename[STRINGLEN + 1];
        char newfilename[STRINGLEN + 1];
	char bfilename[STRINGLEN + 1];
        char stringbuffer[BIGSTRINGLEN + 1];
	struct botuser *admin;
	struct maskstruct *ms;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "saveadmins()\n");
#endif
	snprintf(filename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".admins.saving");
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "saveadmins->saving to %s\n", filename);
#endif
	if((fp = fopen(filename, "w")) == NULL) {
                return 0;
        }
	for(admin = mystate->admins; admin != NULL; admin = admin->next) {
		/* don't save the internal dynamic admin */
		if(!strcasecmp(admin->handle, OER_INTERNAL_DYNAMIC_ADMIN)) {
			continue;
		}
                snprintf(stringbuffer, BIGSTRINGLEN, "admin::%s::%s\n", admin->handle, admin->options);
                if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_ERROR, "saveadmins->fputs() failed: %s\n", strerror(errno));
#endif
			fclose(fp);
			return 0;
		}
		if(index(admin->options,(int)'d') != NULL && admin->password != NULL) {
			snprintf(stringbuffer, BIGSTRINGLEN, "password::%s::%s\n", admin->handle, admin->password);
			if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
				oer_debug(OER_DEBUG_ERROR, "saveadmins->fputs() failed: %s\n", strerror(errno));
#endif
				fclose(fp);
				return 0;
			}
		}
                for(ms = admin->firstmask; ms != NULL; ms = ms->next) {
                        snprintf(stringbuffer, BIGSTRINGLEN, "mask::%s::%s\n", admin->handle, ms->mask);
                        if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
				oer_debug(OER_DEBUG_ERROR, "saveadmins->fputs() failed: %s\n", strerror(errno));
#endif
				fclose(fp);
				return 0;
			}
                }
        }
        if(fclose(fp) == EOF) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_ERROR, "saveadmins->fclose() failed: %s\n", strerror(errno));
#endif
		return 0;
	}
	snprintf(newfilename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".admins");
	if(index(mystate->flags, (int)'b') != NULL && (getfilesize(newfilename) > 0)) {
		/* keep old version */
		snprintf(bfilename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".admins.bak");
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "saveadmins->keeping old %s, renaming to %s\n", newfilename, bfilename);
#endif
		if(rename(newfilename, bfilename) < 0) {
			return 0;
		}
	}
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_INFO, "saveadmins->renaming %s to %s\n", filename, newfilename);
#endif
        if(rename(filename, newfilename) < 0) {
                return 0;
        }
        return 1;
}

int parseadmins(char *paramline)
{
	int ppos;
	char p1[STRINGLEN + 1];
	char p2[STRINGLEN + 1];
	char p3[STRINGLEN + 1];
	struct maskstruct *ms;
	struct botuser *admin;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "parseadmins(\"%s\")\n", paramline);
#endif
	ppos = 0;
	if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p1, STRINGLEN, 1)) < 0) {
		return 0;
	}
	if(!strcasecmp(p1, "admin")) {
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p2, STRINGLEN, 1)) < 0) {
			return 0;
		}
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p3, STRINGLEN, 1)) < 0) {
			return 0;
		}
		if((admin = addnewadmin(p2, p3)) == NULL) {
			return 0;
		}
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "parseadmins->added admin %s with options %s\n", admin->handle, admin->options);
#endif
		return 1;
	}
	if(!strcasecmp(p1, "mask")) {
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p2, STRINGLEN, 1)) < 0) {
			return 0;
		}
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p3, STRINGLEN, 1)) < 0) {
			return 0;
		}
		if((ms = addnewadminmask(p2, p3)) == NULL) {
			return 0;
		}
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "parseadmins->added mask %s to admin %s\n", p3, p2);
#endif
		return 1;
	}
	if(!strcasecmp(p1, "password")) {
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p2, STRINGLEN, 1)) < 0) {
			return 0;
		}
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p3, STRINGLEN, 1)) < 0) {
			return 0;
		}
		if(!setpassword(NULL, p2, p3)) {
			return 0;
		}
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "parseadmins->set password for admin %s\n", p2);
#endif
		return 1;
	}
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_WARNING, "parseadmins->unknown line: '%s'\n", paramline);
#endif
        return 1;
}

int loadkickreasons(void)
{
	FILE *fp;
        char filename[STRINGLEN + 1];
        char stringbuffer[BIGSTRINGLEN + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "loadkickreasons()\n");
#endif
	snprintf(filename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".kickreasons");
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "loadkickreasons->loading %s\n", filename);
#endif
	if((fp = fopen(filename, "r")) == NULL) {
                return 0;
        }
	while(fgets(stringbuffer, BIGSTRINGLEN, fp)) {
                striplf(stringbuffer);
                if(emptyline(stringbuffer)) {
                        continue;
                }
                if(iscomment(stringbuffer)) {
                        continue;
                }
                if(!parsekickreasons(stringbuffer)) {
#ifdef OER_DEBUG
                        oer_debug(OER_DEBUG_WARNING, "loadkickreasons->non-parsable line: '%s'\n", stringbuffer);
#endif
                }
        }
        fclose(fp);
        return 1;
}

int savekickreasons(void)
{
	FILE *fp;
        char filename[STRINGLEN + 1];
        char newfilename[STRINGLEN + 1];
	char bfilename[STRINGLEN + 1];
        char stringbuffer[BIGSTRINGLEN + 1];
	struct maskstruct *ms;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "savekickreasons()\n");
#endif
	snprintf(filename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".kickreasons.saving");
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "savekickreasons->saving to %s\n", filename);
#endif
	if((fp = fopen(filename, "w")) == NULL) {
                return 0;
        }
	for(ms = mystate->kickreasons; ms != NULL; ms = ms->next) {
                snprintf(stringbuffer, BIGSTRINGLEN, "kickreason::%s\n", ms->mask);
                if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_ERROR, "savekickreasons->fputs() failed: %s\n", strerror(errno));
#endif
			fclose(fp);
			return 0;
		}
        }
        if(fclose(fp) == EOF) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_ERROR, "savekickreasons->fclose() failed: %s\n", strerror(errno));
#endif
		return 0;
	}
	snprintf(newfilename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".kickreasons");
	if(index(mystate->flags, (int)'b') != NULL && (getfilesize(newfilename) > 0)) {
		/* keep old version */
		snprintf(bfilename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".kickreasons.bak");
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "savekickreasons->keeping old %s, renaming to %s\n", newfilename, bfilename);
#endif
		if(rename(newfilename, bfilename) < 0) {
			return 0;
		}
	}
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_INFO, "savekickreasons->renaming %s to %s\n", filename, newfilename);
#endif
        if(rename(filename, newfilename) < 0) {
                return 0;
        }
        return 1;
}

int parsekickreasons(char *paramline)
{
	int ppos;
	char p1[STRINGLEN + 1];
	char p2[STRINGLEN + 1];
	struct maskstruct *ms;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "parsekickreasons(\"%s\")\n", paramline);
#endif
	ppos = 0;
	if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p1, STRINGLEN, 1)) < 0) {
		return 0;
	}
	if(!strcasecmp(p1, "kickreason")) {
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p2, STRINGLEN, 1)) < 0) {
			return 0;
		}
		if((ms = addnewkickreason(p2)) == NULL) {
			return 0;
		}
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "parsekickreasons->added a new kick reason %s\n", ms->mask);
#endif
		return 1;
	}
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_WARNING, "parsekickreasons->unknown line: '%s'\n", paramline);
#endif
        return 1;
}

int loadchaninfo(void)
{
	FILE *fp;
        char filename[STRINGLEN + 1];
        char stringbuffer[BIGSTRINGLEN + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "loadchaninfo()\n");
#endif
	snprintf(filename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".chaninfo");
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "loadchaninfo->loading %s\n", filename);
#endif
	if((fp = fopen(filename, "r")) == NULL) {
                return 0;
        }
	while(fgets(stringbuffer, BIGSTRINGLEN, fp)) {
                striplf(stringbuffer);
                if(emptyline(stringbuffer)) {
                        continue;
                }
                if(iscomment(stringbuffer)) {
                        continue;
                }
                if(!parsechaninfo(stringbuffer)) {
#ifdef OER_DEBUG
                        oer_debug(OER_DEBUG_WARNING, "loadchaninfo->non-parsable line: '%s'\n", stringbuffer);
#endif
                }
        }
        fclose(fp);
        return 1;
}

int savechaninfo(void)
{
	FILE *fp;
        char filename[STRINGLEN + 1];
        char newfilename[STRINGLEN + 1];
	char bfilename[STRINGLEN + 1];
        char stringbuffer[BIGSTRINGLEN + 1];
	struct channel *this;
	struct botuser *user;
	struct maskstruct *ms;
	struct advert *a;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "savechaninfo()\n");
#endif
	snprintf(filename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".chaninfo.saving");
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "savechaninfo->saving to %s\n", filename);
#endif
	if((fp = fopen(filename, "w")) == NULL) {
                return 0;
        }
	for(this = mystate->channels; this != NULL; this = this->next) {
                for(user = this->users; user != NULL; user = user->next) {
                        snprintf(stringbuffer, BIGSTRINGLEN, "chaninfo::%s::user::%s::%s\n", this->name, user->handle, user->options);
                        if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
				oer_debug(OER_DEBUG_ERROR, "savechaninfo->fputs() failed: %s\n", strerror(errno));
#endif
				fclose(fp);
				return 0;
			}
                        if(index(user->options, (int)'d') != NULL && user->password != NULL) {
                                snprintf(stringbuffer, BIGSTRINGLEN, "chaninfo::%s::password::%s::%s\n", this->name, user->handle, user->password);
                                if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
					oer_debug(OER_DEBUG_ERROR, "savechaninfo->fputs() failed: %s\n", strerror(errno));
#endif
					fclose(fp);
					return 0;
				}
                        }
                        for(ms = user->firstmask; ms != NULL; ms = ms->next) {
                                snprintf(stringbuffer, BIGSTRINGLEN, "chaninfo::%s::usermask::%s::%s\n", this->name, user->handle, ms->mask);
                                if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
					oer_debug(OER_DEBUG_ERROR, "savechaninfo->fputs() failed: %s\n", strerror(errno));
#endif
					fclose(fp);
					return 0;
				}
                        }
                }
                for(ms = this->wordbks; ms != NULL; ms = ms->next) {
                        snprintf(stringbuffer, BIGSTRINGLEN, "chaninfo::%s::wordbk::%s::%s::%s\n", this->name, ms->mask, (ms->optstring1 == NULL) ? "" : ms->optstring1, (ms->optstring2 == NULL) ? "" : ms->optstring2);
                        if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
				oer_debug(OER_DEBUG_ERROR, "savechaninfo->fputs() failed: %s\n", strerror(errno));
#endif
				fclose(fp);
				return 0;
			}
                }
                for(ms = this->nickbks; ms != NULL; ms = ms->next) {
                        snprintf(stringbuffer, BIGSTRINGLEN, "chaninfo::%s::nickbk::%s::%s::%s\n", this->name, ms->mask, (ms->optstring1 == NULL) ? "" : ms->optstring1, (ms->optstring2 == NULL) ? "" : ms->optstring2);
                        if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
				oer_debug(OER_DEBUG_ERROR, "savechaninfo->fputs() failed: %s\n", strerror(errno));
#endif
				fclose(fp);
				return 0;
			}
                }
                for(ms = this->permbans; ms != NULL; ms = ms->next) {
                        snprintf(stringbuffer, BIGSTRINGLEN, "chaninfo::%s::permban::%s::%s::%s\n", this->name, ms->mask, ms->optstring1, ms->optstring2);
                        if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
				oer_debug(OER_DEBUG_ERROR, "savechaninfo->fputs() failed: %s\n", strerror(errno));
#endif
				fclose(fp);
				return 0;
			}
                }
                for(a = this->adverts; a != NULL; a = a->next) {
                        snprintf(stringbuffer, BIGSTRINGLEN, "chaninfo::%s::advert::%s::%s\n", this->name, a->to, a->message);
                        if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
				oer_debug(OER_DEBUG_ERROR, "savechaninfo->fputs() failed: %s\n", strerror(errno));
#endif
				fclose(fp);
				return 0;
			}
                }
		snprintf(stringbuffer, BIGSTRINGLEN, "chaninfo::%s::floodvars::%d::%d::%d::%d::%d::%d::%d\n", this->name, this->floodvars.repeat_expire, this->floodvars.repeat_limit, this->floodvars.interval, this->floodvars.lines, this->floodvars.chars, this->floodvars.nickflood_expire, this->floodvars.nickflood_changes);
		if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_ERROR, "savechaninfo->fputs() failed: %s\n", strerror(errno));
#endif
			fclose(fp);
			return 0;
		}
		snprintf(stringbuffer, BIGSTRINGLEN, "chaninfo::%s::banvars::%d::%d::%d::%d::%d::%d::%d\n", this->name, this->banvars.auto_rejoin, this->banvars.part_rejoin, this->banvars.public_flood, this->banvars.public_flood_repeat, this->banvars.bad_word, this->banvars.bad_nick, this->banvars.normal_ban);
		if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_ERROR, "savechaninfo->fputs() failed: %s\n", strerror(errno));
#endif
			fclose(fp);
			return 0;
		}
		snprintf(stringbuffer, BIGSTRINGLEN, "chaninfo::%s::bantype::%d\n", this->name, this->bantype);
		if(fputs(stringbuffer, fp) == EOF) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_ERROR, "savechaninfo->fputs() failed: %s\n", strerror(errno));
#endif
			fclose(fp);
			return 0;
		}
        }
        if(fclose(fp) == EOF) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_ERROR, "savechaninfo->fclose() failed: %s\n", strerror(errno));
#endif
		return 0;
	}
	snprintf(newfilename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".chaninfo");
	if(index(mystate->flags, (int)'b') != NULL && (getfilesize(newfilename) > 0)) {
		/* keep old version */
		snprintf(bfilename, STRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".chaninfo.bak");
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "savechaninfo->keeping old %s, renaming to %s\n", newfilename, bfilename);
#endif
		if(rename(newfilename, bfilename) < 0) {
			return 0;
		}
	}
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_INFO, "savechaninfo->renaming %s to %s\n", filename, newfilename);
#endif
        if(rename(filename, newfilename) < 0) {
                return 0;
        }
        return 1;
}

int parsechaninfo(char *paramline)
{
	int ppos;
	int bantype;
	int amount;
	char *nptr;
	char p0[STRINGLEN + 1];
	char p1[STRINGLEN + 1];
	char p2[STRINGLEN + 1];
	char p3[STRINGLEN + 1];
	char p4[STRINGLEN + 1];
	char p5[STRINGLEN + 1];
	char p6[STRINGLEN + 1];
	char p7[STRINGLEN + 1];
	char p8[STRINGLEN + 1];
	char p9[STRINGLEN + 1];
	struct maskstruct *ms;
	struct botuser *user;
	struct channel *this;
	struct advert *a;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "parsechaninfo(\"%s\")\n", paramline);
#endif
	ppos = 0;
	if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p0, STRINGLEN, 1)) < 0) {
		return 0;
	}
	if(!strcasecmp(p0, "chaninfo")) {
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p1, STRINGLEN, 1)) < 0) {
			return 0;
		}
		if((this = getchptr(p1)) == NULL) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_WARNING, "parsechaninfo->unknown channel %s\n", p1);
#endif
			return 0;
		}
		if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p2, STRINGLEN, 1)) < 0) {
			return 0;
		}
		if(!strcasecmp(p2, "user")) {
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p3, STRINGLEN, 1)) < 0) {
				return 0;
			}
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p4, STRINGLEN, 1)) < 0) {
				return 0;
			}
			if((user = addnewuser(this, p3, p4)) == NULL) {
				return 0;
			}
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_INFO, "parsechaninfo->added user %s to channel %s with options %s\n", user->handle, this->name, user->options);
#endif
			return 1;
		}
		if(!strcasecmp(p2, "password")) {
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p3, STRINGLEN, 1)) < 0) {
				return 0;
			}
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p4, STRINGLEN, 1)) < 0) {
				return 0;
			}
			if(!setpassword(this, p3, p4)) {
#ifdef OER_DEBUG
				oer_debug(OER_DEBUG_INFO, "parsechaninfo->failed to add password %s to %s dyn user %s\n", p4, this->name, p3);
#endif
				return 0;
			}
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_INFO, "parsechaninfo->added password %s to %s dyn user %s\n", p4, this->name, p3);
#endif
			return 1;
		}
		if(!strcasecmp(p2, "usermask")) {
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p3, STRINGLEN, 1)) < 0) {
				return 0;
			}
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p4, STRINGLEN, 1)) < 0) {
				return 0;
			}
			if((ms = addnewusermask(this, p3, p4)) == NULL) {
				return 0;
			}
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_INFO, "parsechaninfo->added mask %s to %s user %s\n", p4, p1, p3);
#endif
			return 1;
		}
		if(!strcasecmp(p2, "wordbk")) {
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p3, STRINGLEN, 1)) < 0) {
				return 0;
			}
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p4, STRINGLEN, 1)) < 0) {
				return 0;
			}
			nptr = paramline + ppos;
			if((ms = addnewwordbk(this, p3, strlen(p4) ? p4 : NULL, strlen(nptr) ? nptr : NULL)) == NULL) {
				return 0;
			}
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_INFO, "parsechaninfo->added banned word %s to channel %s\n", ms->mask, this->name);
#endif
			return 1;
		}
		if(!strcasecmp(p2, "nickbk")) {
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p3, STRINGLEN, 1)) < 0) {
				return 0;
			}
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p4, STRINGLEN, 1)) < 0) {
				return 0;
			}
			nptr = paramline + ppos;
			if((ms = addnewnickbk(this, p3, strlen(p4) ? p4 : NULL, strlen(nptr) ? nptr : NULL)) == NULL) {
				return 0;
			}
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_INFO, "parsechaninfo->added banned nick %s to channel %s\n", ms->mask, this->name);
#endif
			return 1;
		}
		if(!strcasecmp(p2, "permban")) {
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p3, STRINGLEN, 1)) < 0) {
				return 0;
			}
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p4, STRINGLEN, 1)) < 0) {
				return 0;
			}
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p5, STRINGLEN, 1)) < 0) {
				return 0;
			}
			amount = permbancount(this);
			if(amount >= OER_PERMBANS) {
#ifdef OER_DEBUG
				oer_debug(OER_DEBUG_INFO, "parsechaninfo->I won't add permban %s to channel %s (max. %d)", p3, this->name, OER_PERMBANS);
#endif
				return 0;
			}
			addnewpermban(this, p3, strlen(p4) ? p4 : NULL, strlen(p5) ? p5 : NULL);
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_INFO, "parsechaninfo->added permban %s to channel %s\n", p3, p1);
#endif
			return 1;
		}
		if(!strcasecmp(p2, "advert")) {
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p3, STRINGLEN, 1)) < 0) {
				return 0;
			}
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p4, STRINGLEN, 1)) < 0) {
				return 0;
			}
			if((a = addnewadvert(this, p3, p4)) == NULL) {
				return 0;
			}
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_INFO, "parsechaninfo->added advert \"%s\" (to: %s) to channel %s\n", a->message, a->to, p1);
#endif
			return 1;
		}
		if(!strcasecmp(p2, "floodvars")) {
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p3, STRINGLEN, 1)) < 0) {
				return 0;
			}
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p4, STRINGLEN, 1)) < 0) {
				return 0;
			}
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p5, STRINGLEN, 1)) < 0) {
				return 0;
			}
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p6, STRINGLEN, 1)) < 0) {
				return 0;
			}
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p7, STRINGLEN, 1)) < 0) {
				return 0;
			}
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p8, STRINGLEN, 1)) < 0) {
				return 0;
			}
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p9, STRINGLEN, 1)) < 0) {
				return 0;
			}
			this->floodvars.repeat_expire = (strlen(p3)) ? atoi(p3) : this->floodvars.repeat_expire;
			this->floodvars.repeat_limit = (strlen(p4)) ? atoi(p4) : this->floodvars.repeat_limit;
			this->floodvars.interval = (strlen(p5)) ? atoi(p5) : this->floodvars.interval;
			this->floodvars.interval = (this->floodvars.interval > 0) ? this->floodvars.interval : 1;
			this->floodvars.lines = (strlen(p6)) ? atoi(p6) : this->floodvars.lines;
			this->floodvars.lines = (this->floodvars.lines > 0) ? this->floodvars.lines : 1;
			this->floodvars.chars = (strlen(p7)) ? atoi(p7) : this->floodvars.chars;
			this->floodvars.nickflood_expire = (strlen(p8)) ? atoi(p8) : this->floodvars.nickflood_expire;
			this->floodvars.nickflood_changes = (strlen(p9)) ? atoi(p9) : this->floodvars.nickflood_changes;
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_INFO, "parsechaninfo->channel %s floodvars set to %d %d %d %d %d %d %d\n", p1, this->floodvars.repeat_expire, this->floodvars.repeat_limit, this->floodvars.interval, this->floodvars.lines, this->floodvars.chars, this->floodvars.nickflood_expire, this->floodvars.nickflood_changes);
#endif
			return 1;
		}
		if(!strcasecmp(p2, "banvars")) {
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p3, STRINGLEN, 1)) < 0) {
				return 0;
			}
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p4, STRINGLEN, 1)) < 0) {
				return 0;
			}
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p5, STRINGLEN, 1)) < 0) {
				return 0;
			}
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p6, STRINGLEN, 1)) < 0) {
				return 0;
			}
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p7, STRINGLEN, 1)) < 0) {
				return 0;
			}
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p8, STRINGLEN, 1)) < 0) {
				return 0;
			}
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p9, STRINGLEN, 1)) < 0) {
				return 0;
			}
			this->banvars.auto_rejoin = atoi(p3);
			this->banvars.part_rejoin = atoi(p4);
			this->banvars.public_flood = atoi(p5);
			this->banvars.public_flood_repeat = atoi(p6);
			this->banvars.bad_word = atoi(p7);
			this->banvars.bad_nick = atoi(p8);
			this->banvars.normal_ban = atoi(p9);
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_INFO, "parsechaninfo->channel %s banvars set to %d %d %d %d %d %d %d\n", p1, this->banvars.auto_rejoin, this->banvars.part_rejoin, this->banvars.public_flood, this->banvars.public_flood_repeat, this->banvars.bad_word, this->banvars.bad_nick, this->banvars.normal_ban);
#endif
			return 1;
		}
		if(!strcasecmp(p2, "bantype")) {
			if((ppos = parse(paramline, ppos, OER_DEFAULT_DELIM, p3, STRINGLEN, 1)) < 0) {
				return 0;
			}
			bantype = atoi(p3);
			if(bantype < OER_BAN_TYPE_HOST || bantype >= OER_BAN_TYPE_INVALID) {
				return 0;
			}
			this->bantype = bantype;
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_INFO, "parsechaninfo->%s has bantype %d\n", this->name, this->bantype);
#endif
			return 1;
		}
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_FATAL, "parsechaninfo->bad channel parameter %s\n", p2);
#endif
		return 0;
	}
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_WARNING, "parsechaninfo->unknown line: '%s'\n", paramline);
#endif
        return 1;
}

int deltaflags(struct channel *this, char *deltaflags, char polar)
{
	int pos;
	char *ptr;
        char newflags[FLAGLEN + 1];
        char newflags2[FLAGLEN + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "deltaflags(\"%s\", \"%s\", \"%c\")\n", (this == NULL) ? "(null)" : this->name, deltaflags, polar);
#endif
	ptr = mystate->flags;
	if(this != NULL) {
		ptr = this->chanflags;
	}
	/* find out what to add or remove */
	memset(newflags, 0, FLAGLEN + 1);
	for(pos = strlen(deltaflags) - 1; pos >= 0; pos--) {
		if(index(ptr, (int)deltaflags[pos]) != NULL) {
			if(polar == '-') {
				strncat(newflags, deltaflags + pos, 1);
			}
		} else if(polar == '+') {
			strncat(newflags, deltaflags + pos, 1);
		}
	}
	if(!strlen(newflags)) {
		return 0;
	}
	memset(newflags2, 0, FLAGLEN + 1);
	for(pos = strlen(ptr) - 1; pos >= 0; pos--) {
		if(index(newflags, (int)ptr[pos]) != NULL) {
			if(polar == '-') {
				continue;
			}
		}
		strncat(newflags2, ptr + pos, 1);
	}
	if(polar == '+') {
		strncat(newflags2, newflags, FLAGLEN - strlen(newflags2));
	}
	sortstring(newflags2);
	strncpy(ptr, newflags2, FLAGLEN);
	return 1;
}

int burstjoins(struct channel *this)
{
	int i;
	time_t delta;
	struct join *j;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "burstjoins(\"%s\")\n", this->name);
#endif
	for(j = this->joins, i = 0; j != NULL; j = j->next) {
		delta = mystate->now - j->at;
		if(delta <= OER_BURST_JOINS_LIMIT) {
			i++;
		}
		if(i >= OER_BURST_JOINS) {
			return 1;
		}
	}
	return 0;
}

int partcount(struct channel *this, char *nick, char *userhost)
{
	int count;
	struct part *p;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "partcount(\"%s\", \"%s\", \"%s\")\n", this->name, nick, userhost);
#endif
	for(count = 0, p = this->parts; p != NULL; p = p->next) {
		if(p->valid && (mystate->now < (p->at + OER_PART_EXPIRE))) {
			if(issamenickandhost(p->nick, nick, p->userhost, userhost)) {
				count++;
			}
		}
	}
	return count;
}

void resetparts(struct channel *this, char *nick, char *userhost)
{
	struct part *p;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "resetparts(\"%s\", \"%s\", \"%s\")\n", this->name, nick, userhost);
#endif
	for(p = this->parts; p != NULL; p = p->next) {
		if(issamenickandhost(p->nick, nick, p->userhost, userhost)) {
			p->valid = 0;
		}
	}
}

time_t lastoff(struct channel *this, char *nick, char *userhost)
{
	time_t delta;
	struct part *p;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "lastoff(\"%s\", \"%s\", \"%s\")\n", this->name, nick, userhost);
#endif
	p = this->parts;
	while(p != NULL) {
		if(issamenickandhost(p->nick, nick, p->userhost, userhost)) {
			delta = (mystate->now - p->at);
			return delta;
		}
		p = p->next;
	}
	return OER_AUTO_REJOIN_TIME + 1;
}

int getjoincount(struct channel *this, char *nick)
{
	int joins;
	struct join *j;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "getjoincount(\"%s\", \"%s\")\n", this->name, nick);
#endif
	joins = 0;
	j = this->joins;
	while(j != NULL) {
		if(!strcasecmp(j->nick, nick)) {
			joins++;
		}
		j = j->next;
	}
	return joins;
}

int isq(char *nick)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "isq(\"%s\")\n", nick);
#endif
	if((index(mystate->flags, (int)'q') != NULL) && !strcasecmp(nick, Q_NICK)) {
                return 1;
        }
	return 0;
}

int isservice(char *host)
{
	struct maskstruct *ms;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "isservice(\"%s\")\n", host);
#endif
        if(index(mystate->flags, (int)'s') == NULL) {
		return 0;
	}
	for(ms = mystate->services; ms != NULL; ms = ms->next) {
                if(wild_match(ms->mask, host)) {
			return 1;
		}
	}
	return 0;
}

int istrusted(char *host)
{
	struct maskstruct *ms;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "istrusted(\"%s\")\n", host);
#endif
	for(ms = mystate->trusted; ms != NULL; ms = ms->next) {
                if(wild_match(ms->mask, host)) {
			return 1;
		}
	}
	return 0;
}

int isme(char *nick)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "isme(\"%s\")\n", nick);
#endif
	if(!strcasecmp(mystate->nick, nick)) {
		return 1;
	}
	return 0;
}

int ispermban(struct channel *this, char *mask)
{
	char thisban[STRINGLEN + 1];
	char thatban[STRINGLEN + 1];
	char *bptr1;
	char *bptr2;
	struct maskstruct *pb;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "ispermban(\"%s\", \"%s\")\n", this->name, mask);
#endif
	if(completeban(mask, thisban, STRINGLEN) < 0) {
		bptr1 = mask;
	} else {
		bptr1 = thisban;
	}
	pb = this->permbans;
	while(pb != NULL) {
		if(completeban(pb->mask, thatban, STRINGLEN) < 0) {
			bptr2 = pb->mask;
		} else {
			bptr2 = thatban;
		}
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_NOISE, "ispermban->comparing %s to %s\n", bptr2, bptr1);
#endif
		if(!strcasecmp(bptr2, bptr1)) {
			return 1;
		}
		pb = pb->next;
	}
	return 0;
}

int permbancount(struct channel *this)
{
	int amount;
	struct maskstruct *pb;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "permbancount(\"%s\")\n", this->name);
#endif
	for(amount = 0, pb = this->permbans; pb != NULL; amount++, pb = pb->next)
		;;
	return amount;
}

int topiccount(struct channel *this)
{
	int amount;
	struct topic *t;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "topiccount(\"%s\")\n", this->name);
#endif
	for(amount = 0, t = this->topics; t != NULL; amount++, t = t->next)
		;;
	return amount;
}

char *getkickreason(char *reason)
{
	int counter;
	int nthkr;
	struct maskstruct *kr;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "getkickreason()\n");
#endif
	counter = 0;
	kr = mystate->kickreasons;
	nthkr = getrandom(mystate->kickrs) - 1;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "getkickreason->returning kickreason %d/%d\n", nthkr, mystate->kickrs);
#endif
	while(kr != NULL && counter < nthkr) {
		counter++;
		kr = kr->next;
        }
	if(kr == NULL) {
		return NULL;
	}
	strncpy(reason, kr->mask, STRINGLEN);
	return reason;
}

int isop(struct channel *this, char *userhost)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "isop(\"%s\", \"%s\")\n", (this == NULL) ? "(null)" : this->name, (userhost == NULL) ? "(null)" : userhost);
#endif
	if(this == NULL || userhost == NULL) {
		return 0;
	}
	/* check for static +o user */
	if(hasuserflags(this, userhost, "o", "d")) {
		return 1;
	}
	/* check for dynamic +o (logoned) user */
	if(hasuserflags(this, userhost, "do", "") && isvalidlogon(this, userhost)) {
		return 1;
	}
	return 0;
}

int isvoice(struct channel *this, char *userhost)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "isvoice(\"%s\", \"%s\")\n", (this == NULL) ? "(null)" : this->name, (userhost == NULL) ? "(null)" : userhost);
#endif
	if(this == NULL || userhost == NULL) {
		return 0;
	}
	/* check for static +v user */
	if(hasuserflags(this, userhost, "v", "d")) {
		return 1;
	}
	/* check for dynamic +v (logoned) user */
	if(hasuserflags(this, userhost, "dv", "") && isvalidlogon(this, userhost)) {
		return 1;
	}
	return 0;
}

int isadmin(struct channel *this, char *userhost)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "isadmin(\"%s\", \"%s\")\n", (this == NULL) ? "(null)" : this->name, (userhost == NULL) ? "(null)" : userhost);
#endif
	if(userhost == NULL) {
		return 0;
	}
	/* check for admin */
	if(isopa(userhost)) {
		return 1;
	}
	/* check for static +a user */
	if(hasuserflags(this, userhost, "a", "d")) {
		return 1;
	}
	/* check for dynamic +a (logoned) user */
	if(hasuserflags(this, userhost, "ad", "") && isvalidlogon(this, userhost)) {
		return 1;
	}
	return 0;
}

int isopa(char *userhost)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "isopa(\"%s\")\n", (userhost == NULL) ? "(null)" : userhost);
#endif
	if(userhost == NULL) {
		return 0;
	}
	/* first check static admins */
	if(hasuserflags(NULL, userhost, "", "d")) {
		return 1;
	}
	/* then check dynamic admins */
	if(hasuserflags(NULL, userhost, "d", "") && isvalidlogon(NULL, userhost)) {
		return 1;
	}
	return 0;
}

int isvalidlogon(struct channel *this, char *userhost)
{
	struct authed *a;
	struct botuser *user;
	struct maskstruct *ms;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "isvalidlogon(\"%s\", \"%s\")\n", (this == NULL) ? "(null)" : this->name, (userhost == NULL) ? "(null)" : userhost);
#endif
	if(userhost == NULL) {
		return 0;
	}
	/* go through all authed hostmasks that are valid */
	for(a = (this == NULL) ? mystate->autheds : this->autheds; a != NULL; a = a->next) {
		if(this != NULL) {
			/* no-expire is checked only for channels */
			if(!haschanflags(this, "x") && (mystate->now > (a->at + OER_LOGON_TIMEOUT))) {
				continue;
			}
		} else if(mystate->now > (a->at + OER_LOGON_TIMEOUT)) {
			continue;
		}
		/* check if the auth matches */
		if(strcasecmp(a->userhost, userhost)) {
			continue;
		}
		/* get handle */
		for(user = (this == NULL) ? mystate->admins : this->users; user != NULL; user = user->next) {
			if(!strcasecmp(user->handle, a->handle)) {
				break;
			}
		}
		if(user == NULL) {
			return 0;
		}
		/* check if any of the hosts in the user field match */
		for(ms = user->firstmask; ms != NULL; ms = ms->next) {
			if(wild_match(ms->mask, userhost)) {
				return 1;
			}
		}
	}
	/* no authed for this mask */
	return 0;
}

int isatleastopnow(struct channel *this, char *nick, char *userhost)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "isatleastopnow(\"%s\", \"%s\", \"%s\")\n", (this == NULL) ? "(null)" : this->name, nick, (userhost == NULL) ? "(null)" : userhost);
#endif
	if(isq(nick) || isadmin(this, userhost) || isop(this, userhost) || isopnow(this, nick)) {
		return 1;
	}
	return 0;
}

int isatleastop(struct channel *this, char *nick, char *userhost)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "isatleastop(\"%s\", \"%s\", \"%s\")\n", this->name, nick, userhost);
#endif
	if(isq(nick) || isadmin(this, userhost) || isop(this, userhost)) {
		return 1;
	}
	return 0;
}

int isallowedop(struct channel *this, char *nick)
{
	struct chanuser *cu;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "isallowedop(\"%s\", \"%s\")\n", this->name, nick);
#endif
	for(cu = this->nicks; cu != NULL; cu = cu->next) {
		if(!strcasecmp(cu->nick, nick)) {
			break;
		}
	}
	if(cu == NULL) {
		return 0;
	}
	if(isq(nick)) {
                return 1;
        }
	/* first check for admin */
	if(isadmin(this, cu->userhost)) {
		return 1;
	}
	/* then user */
	if(isop(this, cu->userhost)) {
		return 1;
	}
	/* only dyn left, is dyn user with o flag? */
	if(!hasuserflags(this, cu->userhost, "do", "")) {
		return 0;
	}
	if(!haschanflags(this, "P")) {
		return 1;
	}
	/* paranoid channel, check for valid logon */
	if(isvalidlogon(this, cu->userhost)) {
		return 1;
	}
	return 0;
}

int delopa(char *handle)
{
	struct botuser *admin;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "delopa(\"%s\")\n", handle);
#endif
	for(admin = mystate->admins; admin != NULL; admin = admin->next) {
		if(!strcasecmp(admin->handle, handle)) {
			break;
		}
	}
	if(admin == NULL) {
		return 0;
	}
	if(index(admin->options, (int)'n') != NULL) {
		/* we won't delete admins with "n" flag */
		return 0;
	}
	return 1;
}

int isonchan(struct channel *this, char *nick)
{
	struct chanuser *cu;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "isonchan(\"%s\", \"%s\")\n", this->name, nick);
#endif
	for(cu = this->nicks; cu != NULL; cu = cu->next) {
		if(!strcasecmp(cu->nick, nick)) {
			return 1;
		}
	}
	return 0;
}

int issameuser(struct channel *this, char *handle, char *userhost)
{
	struct botuser *user;
	struct maskstruct *ms;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "issameuser(\"%s\", \"%s\", \"%s\")\n", (this == NULL) ? "(null)" : this->name, handle, userhost);
#endif
	if(this == NULL) {
		return 0;
	}
	for(user = this->users; user != NULL; user = user->next) {
		if(!strcasecmp(user->handle, handle)) {
			break;
		}
	}
	if(user == NULL) {
		return 0;
	}
	for(ms = user->firstmask; ms != NULL; ms = ms->next) {
		if(wild_match(ms->mask, userhost)) {
			return 1;
		}
	}
	return 0;
}

int isopnow(struct channel *this, char *nick)
{
	struct chanuser *cu;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "isopnow(\"%s\", \"%s\")\n", (this == NULL) ? "(null)" : this->name, nick);
#endif
	if(this == NULL) {
		return 0;
	}
	for(cu = this->nicks; cu != NULL; cu = cu->next) {
		if(!strcasecmp(cu->nick, nick)) {
			return cu->chanop;
		}
	}
	return 0;
}

void banuser(struct channel *this, time_t when, char *nick)
{
	struct chanuser *cu;
	char ban[USERHOSTLEN + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "banuser(\"%s\", %lu, \"%s\")\n", this->name, when, nick);
#endif
	for(cu = this->nicks; cu != NULL; cu = cu->next) {
		if(!strcasecmp(cu->nick, nick)) {
			if(cu->userhost == NULL) {
				return;
			}
			strncpy(ban, cu->userhost, USERHOSTLEN);
			if(safeban(this, ban, cu->nick) == NULL) {
				return;
			}
			if(this->banvars.normal_ban > 0) {
				mmode_new(this, when, "+b", ban);
				mmode_new(this, when + this->banvars.normal_ban, "-b", ban);
			}
			return;
		}
	}
}

void unbanuser(struct channel *this, time_t when, char *nick)
{
	struct chanuser *cu;
	char ban[USERHOSTLEN + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "unbanuser(\"%s\", %lu, \"%s\")\n", this->name, when, nick);
#endif
	for(cu = this->nicks; cu != NULL; cu = cu->next) {
		if(!strcasecmp(cu->nick, nick)) {
			if(cu->userhost == NULL) {
				return;
			}
			strncpy(ban, cu->userhost, USERHOSTLEN);
			if(safeban(this, ban, cu->nick) == NULL) {
				return;
			}
			mmode_new(this, when, "-b", ban);
			return;
		}
	}
}

void editcommand(struct channel *this, char *to, int tochan, char *nick, char *userhost, char *commandline)
{
	int proceed;
	int ppos;
	int bantype;
	int tostatic;
	int last_stat;
	int seen_stat;
	int notactivated;
	int cleana;
        char p1[STRINGLEN + 1];
        char p2[STRINGLEN + 1];
        char p3[STRINGLEN + 1];
        char p4[STRINGLEN + 1];
        char p5[STRINGLEN + 1];
        char p6[STRINGLEN + 1];
        char p7[STRINGLEN + 1];
        char p8[STRINGLEN + 1];
	char stringbuffer[BIGSTRINGLEN + 1];
	char cryptsalt[3];
	char *crypted;
	struct botuser *admin;
	struct botuser *user;
	struct server *server;
        struct server newserver;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "editcommand(\"%s\", \"%s\", %d, \"%s\", \"%s\", \"%s\")\n", (this == NULL) ? "(null)" : this->name, to, tochan, nick, userhost, commandline);
#endif
	/* command line parsing begins here, first parameter = the command */
	ppos = 0;
        if((ppos = parse(commandline, ppos, " ", p1, STRINGLEN, 0)) < 0) {
                return;
        }
	if(!strcasecmp(p1, "flags")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) != 1) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		validateflags(p2, p3, OER_FLAGS_TYPE_GLOBAL);
		strncpy(p2, p3, STRINGLEN);
		if(!strlen(p2)) {
			return;
		}
		/* check if we have to change some behaviour */
		if(index(p2, (int)'l')) {
			if(index(mystate->flags, (int)'l') == NULL) {
				if(strlen(mystate->altloggingdir)) {
					snprintf(stringbuffer, WRITE_BUFFER_LENGTH, "oer-%d.raw", getpid());
				} else {
					snprintf(stringbuffer, WRITE_BUFFER_LENGTH, "%s/oer-%d.raw", mystate->altloggingdir, getpid());
				}
				if((mystate->logfp = fopen(stringbuffer, "a+")) == NULL) {
#ifdef OER_DEBUG
					oer_debug(OER_DEBUG_INFO, "editcommand->couldn't open logfile %s: %s\n", stringbuffer, strerror(errno));
#endif
				} else {
#ifdef OER_DEBUG
					oer_debug(OER_DEBUG_INFO, "editcommand->logfile is %s\n", stringbuffer);
#endif
				}
			}
		} else if(index(mystate->flags, (int)'l') != NULL) {
			if(fclose(mystate->logfp) == EOF) {
#ifdef OER_DEBUG
				oer_debug(OER_DEBUG_ERROR, "editcommand->fclose() failed: %s\n", strerror(errno));
#endif
			}
			mystate->logfp = NULL;
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_INFO, "editcommand->logging terminated by request\n");
#endif
		}
		sortstring(p2);
		strncpy(mystate->flags, p2, FLAGLEN);
		snprintf(stringbuffer, BIGSTRINGLEN, "my flags are now %s", mystate->flags);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "floodvars")) {
		if(this == NULL) {
			return;
		}
		if(!isadmin(this, userhost)) {
			return;
		}
		if(numofparams(commandline) != 7) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}	
		if(!isnumbw(atoi(p2), OER_LIMIT_FLOODVARS_REPEAT_EXPIRE_MIN, OER_LIMIT_FLOODVARS_REPEAT_EXPIRE_MAX)) {
                        return;
                }
		if((ppos = parse(commandline, ppos, " ", p3, STRINGLEN, 0)) < 0) {
			return;
		}
		if(!isnumbw(atoi(p3), OER_LIMIT_FLOODVARS_REPEAT_LIMIT_MIN, OER_LIMIT_FLOODVARS_REPEAT_LIMIT_MAX)) {
                        return;
                }
		if((ppos = parse(commandline, ppos, " ", p4, STRINGLEN, 0)) < 0) {
			return;
		}
		if(!isnumbw(atoi(p4), OER_LIMIT_FLOODVARS_INTERVAL_MIN, OER_LIMIT_FLOODVARS_INTERVAL_MAX)) {
                        return;
                }
		if((ppos = parse(commandline, ppos, " ", p5, STRINGLEN, 0)) < 0) {
			return;
		}
		if(!isnumbw(atoi(p5), OER_LIMIT_FLOODVARS_LINES_MIN, OER_LIMIT_FLOODVARS_LINES_MAX)) {
                        return;
                }
		if((ppos = parse(commandline, ppos, " ", p6, STRINGLEN, 0)) < 0) {
			return;
		}
		if(!isnumbw(atoi(p6), OER_LIMIT_FLOODVARS_CHARS_MIN, OER_LIMIT_FLOODVARS_CHARS_MAX)) {
                        return;
                }
		if((ppos = parse(commandline, ppos, " ", p7, STRINGLEN, 0)) < 0) {
			return;
		}
		if(!isnumbw(atoi(p7), OER_LIMIT_FLOODVARS_NICKFLOOD_EXPIRE_MIN, OER_LIMIT_FLOODVARS_NICKFLOOD_EXPIRE_MAX)) {
                        return;
                }
		if((ppos = parse(commandline, ppos, " ", p8, STRINGLEN, 0)) < 0) {
			return;
		}
		if(!isnumbw(atoi(p8), OER_LIMIT_FLOODVARS_NICKFLOOD_LIMIT_MIN, OER_LIMIT_FLOODVARS_NICKFLOOD_LIMIT_MAX)) {
                        return;
                }
		this->floodvars.repeat_expire = atoi(p2);
		this->floodvars.repeat_limit = atoi(p3);
		this->floodvars.interval = atoi(p4);
		this->floodvars.lines = atoi(p5);
		this->floodvars.chars = atoi(p6);
		this->floodvars.nickflood_expire = atoi(p7);
		this->floodvars.nickflood_changes = atoi(p8);
		snprintf(stringbuffer, BIGSTRINGLEN, "%s floodvars are now %d %d %d %d %d %d %d", this->name, this->floodvars.repeat_expire, this->floodvars.repeat_limit, this->floodvars.interval, this->floodvars.lines, this->floodvars.chars, this->floodvars.nickflood_expire, this->floodvars.nickflood_changes);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "banvars")) {
		if(this == NULL) {
			return;
		}
		if(!isadmin(this, userhost)) {
			return;
		}
		if(numofparams(commandline) != 7) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		if(!isnumbw(atoi(p2), OER_LIMIT_BANVARS_AUTOREJOIN_MIN, OER_LIMIT_BANVARS_AUTOREJOIN_MAX)) {
                        return;
                }
		if((ppos = parse(commandline, ppos, " ", p3, STRINGLEN, 0)) < 0) {
			return;
		}
		if(!isnumbw(atoi(p3), OER_LIMIT_BANVARS_PARTREJOIN_MIN, OER_LIMIT_BANVARS_PARTREJOIN_MAX)) {
                        return;
                }
		if((ppos = parse(commandline, ppos, " ", p4, STRINGLEN, 0)) < 0) {
			return;
		}
		if(!isnumbw(atoi(p4), OER_LIMIT_BANVARS_PUBLICFLOOD_MIN, OER_LIMIT_BANVARS_PUBLICFLOOD_MAX)) {
                        return;
                }
		if((ppos = parse(commandline, ppos, " ", p5, STRINGLEN, 0)) < 0) {
			return;
		}
		if(!isnumbw(atoi(p5), OER_LIMIT_BANVARS_PUBLICFLOODREPEAT_MIN, OER_LIMIT_BANVARS_PUBLICFLOODREPEAT_MAX)) {
                        return;
                }
		if((ppos = parse(commandline, ppos, " ", p6, STRINGLEN, 0)) < 0) {
			return;
		}
		if(!isnumbw(atoi(p6), OER_LIMIT_BANVARS_BADWORD_MIN, OER_LIMIT_BANVARS_BADWORD_MAX)) {
                        return;
                }
		if((ppos = parse(commandline, ppos, " ", p7, STRINGLEN, 0)) < 0) {
			return;
		}
		if(!isnumbw(atoi(p7), OER_LIMIT_BANVARS_BADNICK_MIN, OER_LIMIT_BANVARS_BADNICK_MAX)) {
                        return;
                }
		if((ppos = parse(commandline, ppos, " ", p8, STRINGLEN, 0)) < 0) {
			return;
		}
		if(!isnumbw(atoi(p8), OER_LIMIT_BANVARS_NORMALBAN_MIN, OER_LIMIT_BANVARS_NORMALBAN_MAX)) {
                        return;
                }
		this->banvars.auto_rejoin = atoi(p2);
		this->banvars.part_rejoin = atoi(p3);
		this->banvars.public_flood = atoi(p4);
		this->banvars.public_flood_repeat = atoi(p5);
		this->banvars.bad_word = atoi(p6);
		this->banvars.bad_nick = atoi(p7);
		this->banvars.normal_ban = atoi(p8);
		snprintf(stringbuffer, BIGSTRINGLEN, "%s banvars are now %d %d %d %d %d %d %d", this->name, this->banvars.auto_rejoin, this->banvars.part_rejoin, this->banvars.public_flood, this->banvars.public_flood_repeat, this->banvars.bad_word, this->banvars.bad_nick, this->banvars.normal_ban);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "bantype")) {
		if(this == NULL) {
			return;
		}
		if(!isadmin(this, userhost)) {
			return;
		}
		if(numofparams(commandline) != 1) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		bantype = atoi(p2);
		if(bantype < OER_BAN_TYPE_HOST || bantype >= OER_BAN_TYPE_INVALID) {
			return;
		}
		this->bantype = bantype;
		snprintf(stringbuffer, BIGSTRINGLEN, "%s ban type is now %d", this->name, this->bantype);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "chanflags")) {
		if(this == NULL) {
			return;
		}
		if(!isadmin(this, userhost)) {
			return;
		}
		if(numofparams(commandline) != 1) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		validateflags(p2, p3, OER_FLAGS_TYPE_CHANNEL);
		strncpy(p2, p3, STRINGLEN);
		if(!strlen(p2)) {
			return;
		}
		if(index(p2, (int)'x') == NULL && index(this->chanflags, (int)'x') != NULL) {
			cleana = 1;
		} else {
			cleana = 0;
		}
		last_stat = -1;
		seen_stat = -1;
		if(index(p2, (int)'L') != NULL && index(this->chanflags, (int)'L') == NULL) {
			last_stat = 1;
		}
		if(index(p2, (int)'S') != NULL && index(this->chanflags, (int)'S') == NULL) {
			seen_stat = 1;
		}
		if(index(p2, (int)'L') == NULL && index(this->chanflags, (int)'L') != NULL) {
			last_stat = 0;
		}
		if(index(p2, (int)'S') == NULL && index(this->chanflags, (int)'S') != NULL) {
			seen_stat = 0;
		}
		sortstring(p2);
		strncpy(this->chanflags, p2, FLAGLEN);
		snprintf(stringbuffer, BIGSTRINGLEN, "%s chanflags are now %s", this->name, this->chanflags);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		if(last_stat == 1) {
			if(this->last == NULL) {
				if(strlen(mystate->altloggingdir)) {
					snprintf(p3, STRINGLEN, "%s/last-%s", mystate->altloggingdir, this->name);
				} else {
					snprintf(p3, STRINGLEN, "last-%s", this->name);
				}
				if((this->last = fopen(p3, "a")) == NULL) {
#ifdef OER_DEBUG
					oer_debug(OER_DEBUG_INFO, "editcommand->failed to open %s: %s\n", p3, strerror(errno));
#endif
				} else {
					snprintf(stringbuffer, BIGSTRINGLEN, "%s has been opened for logging", p3);
					sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
				}
			}
		}
		if(seen_stat == 1) {
			if(this->seen == NULL) {
				if(strlen(mystate->altloggingdir)) {
					snprintf(p3, STRINGLEN, "%s/seen-%s", mystate->altloggingdir, this->name);
				} else {
					snprintf(p3, STRINGLEN, "seen-%s", this->name);
				}
				if((this->seen = fopen(p3, "a")) == NULL) {
#ifdef OER_DEBUG
					oer_debug(OER_DEBUG_INFO, "editcommand->failed to open %s: %s\n", p3, strerror(errno));
#endif
				} else {
					snprintf(stringbuffer, BIGSTRINGLEN, "%s has been opened for logging", p3);
					sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
				}

			}
		}
		if(last_stat == 0) {
			if(this->last != NULL) {
				if(fclose(this->last) == EOF) {
#ifdef OER_DEBUG
					oer_debug(OER_DEBUG_ERROR, "editcommand->fclose() failed: %s\n", strerror(errno));
#endif
				}
				this->last = NULL;
				snprintf(p3, STRINGLEN, "last-%s", this->name);
				snprintf(stringbuffer, BIGSTRINGLEN, "%s has been closed, you will have to delete it manually", p3);
				sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
			}
		}
		if(seen_stat == 0) {
			if(this->seen != NULL) {
				if(fclose(this->seen) == EOF) {
#ifdef OER_DEBUG
					oer_debug(OER_DEBUG_ERROR, "editcommand->fclose() failed: %s\n", strerror(errno));
#endif
				}
				this->seen = NULL;
				snprintf(p3, STRINGLEN, "seen-%s", this->name);
				snprintf(stringbuffer, BIGSTRINGLEN, "%s has been closed, you will have to delete it manually", p3);
				sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
			}
		}
		if(this->allhostsknown && this->i_am_op) {
			channelsync(this);
		}
		if(cleana) {
			cleanautheds(this, 0);
		}
		return;
	}
	if(!strcasecmp(p1, "chanmode")) {
		if(this == NULL) {
			return;
		}
		if(!isadmin(this, userhost)) {
			return;
		}
		if(numofparams(commandline) != 1) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		strncpy(this->mode, p2, CHANLEN);
		this->setchanmode = 1;
		snprintf(stringbuffer, BIGSTRINGLEN, "%s chanmodes are now %s", this->name, this->mode);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "chankey")) {
		if(this == NULL) {
			return;
		}
		if(!isadmin(this, userhost)) {
			return;
		}
		if(numofparams(commandline) != 1) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		if(!strcasecmp(this->key, p2)) {
			/* key hasn't changed */
			return;
		}
                if(getmodepol(this->mode, 'k') != '-') {
			/* remove old one first, force mmodes2timeds() */
			delchankey(this);
			mmodes2timeds();
			setchankey(this, p2);
			snprintf(stringbuffer, BIGSTRINGLEN, "%s channel key is now %s", this->name, this->key);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
	if(!strcasecmp(p1, "chanlimit")) {
		if(this == NULL) {
			return;
		}
		if(!isadmin(this, userhost)) {
			return;
		}
		if(numofparams(commandline) != 1) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		if(!isnumbw(atol(p2), OER_LIMIT_CHANNEL_CHANLIMIT_MIN, OER_LIMIT_CHANNEL_CHANLIMIT_MAX)) {
                        return;
                }
		setchanlimit(this, p2);
		if(strlen(this->limit)) {
			snprintf(stringbuffer, BIGSTRINGLEN, "%s channel limit is now %s", this->name, this->limit);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
	if(!strcasecmp(p1, "nick")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) != 1) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		strncpy(mystate->getnick, p2, NICKLEN);
		mystate->newnick = 1;
		mystate->use_altnick = 0;
		snprintf(stringbuffer, BIGSTRINGLEN, "attempting to change my nick to %s", mystate->getnick);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "altnick")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) != 1) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		strncpy(mystate->altnick, p2, NICKLEN);
		snprintf(stringbuffer, BIGSTRINGLEN, "my alternative nick is now %s", mystate->altnick);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "prefix")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) != 1) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		if(index(p2, (int)'"')) {
			return;
		}
		strncpy(mystate->prefix, p2, MICROSTRINGLEN);
		snprintf(stringbuffer, BIGSTRINGLEN, "my command prefix is now %s", mystate->prefix);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "realname")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) < 1) {
			return;
		}
		strncpy(mystate->realname, commandline + ppos, STRINGLEN);
		snprintf(stringbuffer, BIGSTRINGLEN, "my IRC REALNAME is now %s", mystate->realname);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "usermode")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) != 1) {
			return;
		}
		strncpy(mystate->mode, commandline + ppos, CHANLEN);
		snprintf(stringbuffer, BIGSTRINGLEN, "my usermodes are now %s", mystate->mode);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "user")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) != 1) {
			return;
		}
		strncpy(mystate->user, commandline + ppos, USERLEN);
		snprintf(stringbuffer, BIGSTRINGLEN, "my user ID (ident) is now %s", mystate->user);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "vhost")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) != 1) {
			return;
		}
		if(!isvalidhost(commandline + ppos)) {
			return;
		}
		strncpy(mystate->vhost, commandline + ppos, HOSTLEN);
		snprintf(stringbuffer, BIGSTRINGLEN, "my virtual host is now %s", mystate->vhost);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "password")) {
		if(tochan) {
			return;
		}
		if(numofparams(commandline) != 2) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p3, STRINGLEN, 0)) < 0) {
			return;
		}
		if(wild_match("*@*", p3)) {
			/* do not allow hostmask like passwords */
			return;
		}
		if(this == NULL && !isopa(userhost)) {
			return;
		}
		for(user = (this == NULL) ? mystate->admins : this ->users; user != NULL; user = user->next) {
			if(!strcasecmp(user->handle, p2)) {
				break;
			}
		}
		if(user == NULL) {
			return;
		}
		if(this == NULL) {
			/* admin password */
			proceed = 1;
		} else {
			proceed = (isopa(userhost)) ? 1 : 0;
			if(index(user->options, (int)'a') != NULL) {
				/* the user has +a flag */
				if(isadmin(this, userhost) && issameuser(this, p2, userhost)) {
					proceed = 1;
				}
			} else {
				if(isadmin(this, userhost)) {
					proceed = 1;
				}
				if(isop(this, userhost) && issameuser(this, p2, userhost)) {
					proceed = 1;
				}
			}
		}
		if(!proceed) {
			return;
		}
		cryptsalt[0] = salt_chars[getrandom(strlen(salt_chars)) - 1];
		cryptsalt[1] = cryptsalt[0];
		cryptsalt[2] = '\0';
		crypted = crypt(p3, cryptsalt);
		if(!setpassword(this, p2, crypted)) {
			return;
		}
		if(this == NULL) {
			snprintf(stringbuffer, BIGSTRINGLEN, "password for admin %s is now %s", p2, p3);
		} else {
			snprintf(stringbuffer, BIGSTRINGLEN, "password for %s user %s is now %s", this->name, p2, p3);
		}
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "options")) {
		if(numofparams(commandline) != 3) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p3, STRINGLEN, 0)) < 0) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p4, STRINGLEN, 0)) < 0) {
			return;
		}
		if(!strcasecmp(p2, "admin")) {
			if(!isopa(userhost)) {
				return;
			}
			for(admin = mystate->admins; admin != NULL; admin = admin->next) {
				if(!strcasecmp(admin->handle, p3)) {
					break;
				}
			}
			if(admin == NULL) {
				return;
			}
			/* we won't allow static <-> dyn */
			if(((index(admin->options, (int)'d') == NULL) && (index(p4, (int)'d') != NULL)) || ((index(admin->options, (int)'d') != NULL) && (index(p4, (int)'d') == NULL))) {
				snprintf(stringbuffer, BIGSTRINGLEN, "your aren't allowed to change a static admin into a dynamic one or visa versa, use add admin");
				sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
				return;
			}
			/* only allow +n admins to add other +n admins */
			if((index(p4, (int)'n') != NULL) && !hasadminflags(userhost, "n")) {
#ifdef OER_DEBUG
				oer_debug(OER_DEBUG_INFO, "editcommand->%s!%s attempted to make %s a protected admin\n", nick, userhost, p3);
#endif
				return;
			}
			/* we won't allow removal of the "n" flag for admins */
			if((index(admin->options, (int)'n') != NULL) && (index(p4, (int)'n') == NULL)) {
#ifdef OER_DEBUG
				oer_debug(OER_DEBUG_INFO, "editcommand->%s!%s attempted to unprotect protected admin %s\n", nick, userhost, p3);
#endif
				return;
			}
			validateflags(p4, p5, OER_FLAGS_TYPE_ADMIN);
			strncpy(p4, p5, STRINGLEN);
			if(!strlen(p4)) {
				return;
			}
			free(admin->options);
			if((admin->options = malloc(strlen(p4) + 1)) == NULL) {
				return;
			}
			strcpy(admin->options, p4);
			snprintf(stringbuffer, BIGSTRINGLEN, "options for admin %s are now %s", admin->handle, admin->options);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
			return;
		}
		if(!strcasecmp(p2, "user")) {
			if(this == NULL) {
				return;
			}
			for(user = this->users; user != NULL; user = user->next) {
				if(!strcasecmp(user->handle, p3)) {
					break;
				}
			}
			if(user == NULL) {
				return;
			}
			proceed = (isopa(userhost)) ? 1 : 0;
			if(index(user->options, (int)'a') != NULL) {
				/* target is a channel admin, has "a" flag */
				if(isadmin(this, userhost) && issameuser(this, p3, userhost)) {
					/* same user, no change if channel admin is inactive */
					if(index(user->options, (int)'!') == NULL) {
						proceed = 1;
					}
				}
			} else if(isadmin(this, userhost) && (index(p4, (int)'a') == NULL)) {
				/* normal user, do not allow setting of "a" flag */
				proceed = 1;
			}
			if(!proceed) {
				return;
			}
			tostatic = 0;
			if(index(user->options, (int)'d') != NULL && index(p4, (int)'d') == NULL) {
				tostatic = 1;
			}
			validateflags(p4, p5, OER_FLAGS_TYPE_USER);
			strncpy(p4, p5, STRINGLEN);
			if(!strlen(p4)) {
				return;
			}
			free(user->options);
			if((user->options = malloc(strlen(p4) + 1)) == NULL) {
				return;
			}
			strcpy(user->options, p4);
			if(tostatic) {
				logoff(this, user->handle);
				if(user->password) {
					free(user->password);
					user->password = NULL;
				}
			}
			snprintf(stringbuffer, BIGSTRINGLEN, "options for user %s are now %s", user->handle, user->options);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
			if(this->i_am_op) {
				syncvoices(this);
				syncops(this);
			}
			return;
		}
		return;
	}
	if(!strcasecmp(p1, "mask")) {
		if(numofparams(commandline) != 4) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p3, STRINGLEN, 0)) < 0) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p4, STRINGLEN, 0)) < 0) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p5, STRINGLEN, 0)) < 0) {
			return;
		}
		if(!strcasecmp(p2, "admin")) {
			if(!isopa(userhost)) {
				return;
			}
			for(admin = mystate->admins; admin != NULL; admin = admin->next) {
				if(!strcasecmp(admin->handle, p3)) {
					break;
				}
			}
			if(admin == NULL) {
				return;
			}
			if(editmask(admin->firstmask, p4, p5) == NULL) {
				return;
			}
			snprintf(stringbuffer, BIGSTRINGLEN, "changed mask for admin %s from %s to %s", p3, p4, p5);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
			return;
		}
		if(!strcasecmp(p2, "user")) {
			if(this == NULL) {
				return;
			}
			for(user = this->users; user != NULL; user = user->next) {
				if(!strcasecmp(user->handle, p3)) {
					break;
				}
			}
			if(user == NULL) {
				return;
			}
			proceed = (isopa(userhost)) ? 1 : 0;
			if(index(user->options, (int)'a') != NULL) {
				/* the user has +a flag */
				if(isadmin(this, userhost) && issameuser(this, p3, userhost)) {
					proceed = 1;
				}
			} else {
				/* a normal user */
				if(isadmin(this, userhost)) {
					proceed = 1;
				}
				if(isop(this, userhost) && issameuser(this, p3, userhost)) {
					proceed = 1;
				}
				if(haschanflags(this, "M") && !isadmin(this, userhost)) {
					proceed = 0;
				}
				if(index(user->options, (int)'m') != NULL && !isadmin(this, userhost)) {
					proceed = 0;
				}
			}
			if(!proceed) {
				return;
			}
			if(editmask(user->firstmask, p4, p5) == NULL) {
				return;
			}
			snprintf(stringbuffer, BIGSTRINGLEN, "changed mask for %s channel user %s from %s to %s", this->name, p3, p4, p5);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
			if(this->i_am_op) {
				syncvoices(this);
				syncops(this);
			}
			return;
		}
		return;
	}
	if(!strcasecmp(p1, "qauth")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) != 3) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p3, STRINGLEN, 0)) < 0) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p4, STRINGLEN, 0)) < 0) {
			return;
		}
		strncpy(mystate->qauth.q, p2, USERHOSTLEN);
		strncpy(mystate->qauth.name, p3, NICKLEN);
		strncpy(mystate->qauth.password, p4, TINYSTRINGLEN);
		mystate->qauth.hasauth = 1;
		mystate->qauth.isonline = 0;
		mystate->qauth.lastquery = 0;
		mystate->qauth.authed = 0;
		snprintf(stringbuffer, BIGSTRINGLEN, "got qauth information for %s", mystate->qauth.q);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "proxysetup")) {
		if(!isopa(userhost)) {
			return;
		}
		if(!strlen(commandline + ppos)) {
			return;
		}
		strncpy(mystate->proxysetup, commandline + ppos, STRINGLEN);
		snprintf(stringbuffer, BIGSTRINGLEN, "got proxysetup information");
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "server")) {
		if(!isopa(userhost)) {
                        return;
                }
                if(numofparams(commandline) < 2 || numofparams(commandline) > 9) {
                        return;
                }
		server = mystate->current_server;
                strncpy(newserver.serverhost, server->serverhost, HOSTLEN);
                newserver.serverport = server->serverport;
                newserver.servermodes = server->servermodes;
                newserver.pingfrequency = server->pingfrequency;
                newserver.protected_ircops = server->protected_ircops;
                newserver.linenoise = server->linenoise;
                strncpy(newserver.password, server->password, TINYSTRINGLEN);
                notactivated = 0;
		/* hostname */
                if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 1)) < 0) {
                        return;
                }
                if(!isvalidhost(p2)) {
                        return;
                }
                if(strcasecmp(p2, server->serverhost)) {
                        strncpy(newserver.serverhost, p2, HOSTLEN);
                        notactivated = 1;
                }
		/* port */
                if((ppos = parse(commandline, ppos, " ", p3, STRINGLEN, 1)) < 0) {
                        return;
                }
		newserver.serverport = atoi(p3);
                if(!isnumbw(newserver.serverport, OER_LIMIT_SERVER_PORT_MIN, OER_LIMIT_SERVER_PORT_MAX)) {
                        return;
                }
		if(newserver.serverport != server->serverport) {
			notactivated = 1;
		}
                /* modes (optional) */
                if((ppos = parse(commandline, ppos, " ", p4, STRINGLEN, 1)) < 0) {
                        return;
                }
		if(strlen(p4)) {
			newserver.servermodes = atoi(p4);
			if(!isnumbw(newserver.servermodes, OER_LIMIT_SERVER_MODES_MIN, OER_LIMIT_SERVER_MODES_MAX)) {
				return;
			}
		}
		/* ping frequency (optional) */
                if((ppos = parse(commandline, ppos, " ", p5, STRINGLEN, 1)) < 0) {
                        return;
                }
		if(strlen(p5)) {
			newserver.pingfrequency = atoi(p5);
			if(!isnumbw(newserver.pingfrequency, OER_LIMIT_SERVER_PING_MIN, OER_LIMIT_SERVER_PING_MAX)) {
				return;
			}
		}
                /* protected IRC ops (optional) */
                if((ppos = parse(commandline, ppos, " ", p6, STRINGLEN, 1)) < 0) {
                        return;
                }
		if(strlen(p6)) {
			newserver.protected_ircops = atoi(p6);
			if(!isnumbw(newserver.protected_ircops, OER_LIMIT_SERVER_PROTOPER_MIN, OER_LIMIT_SERVER_PROTOPER_MAX)) {
				return;
			}
		}
		/* line noise (optional) */
                if((ppos = parse(commandline, ppos, " ", p7, STRINGLEN, 1)) < 0) {
                        return;
                }
		if(strlen(p7)) {
			newserver.linenoise = atoi(p7);
			if(!isnumbw(newserver.linenoise, OER_LINENOISE_INTERVAL_MIN, server->pingfrequency)) {
				return;
			}
		}
                /* server password (optional) */
                if((ppos = parse(commandline, ppos, " ", p8, STRINGLEN, 1)) < 0) {
                        return;
                }
                if(strlen(p8) && strcasecmp(p8, server->password)) {
                        strncpy(newserver.password, p8, TINYSTRINGLEN);
                        notactivated = 1;
                }
                strncpy(server->serverhost, newserver.serverhost, HOSTLEN);
                server->serverport = newserver.serverport;
                server->servermodes = newserver.servermodes;
                server->pingfrequency = newserver.pingfrequency;
                server->protected_ircops = newserver.protected_ircops;
                server->linenoise = newserver.linenoise;
                strncpy(server->password, newserver.password, TINYSTRINGLEN);
		snprintf(stringbuffer, BIGSTRINGLEN, "current server settings: %s %d %d %d %d %d %s", server->serverhost, server->serverport, server->servermodes, server->pingfrequency, server->protected_ircops, server->linenoise, (strlen(server->password) && isopa(userhost)) ? server->password : "(null)");
                sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
                if(notactivated) {
                        snprintf(stringbuffer, BIGSTRINGLEN, "some settings won't take effect before reconnecting to the server");
                        sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
                }
		return;
	}
}

void delcommand(struct channel *this, char *to, int tochan, char *nick, char *userhost, char *commandline)
{
	int proceed;
	int port;
	int modes;
	int ping;
	int opers;
	int noise;
	int ppos;
	int nppos;
	int last_stat;
	int seen_stat;
	int cleana;
        char p1[STRINGLEN + 1];
        char p2[STRINGLEN + 1];
        char p3[STRINGLEN + 1];
        char p4[STRINGLEN + 1];
        char p5[STRINGLEN + 1];
        char p6[STRINGLEN + 1];
        char p7[STRINGLEN + 1];
	char stringbuffer[BIGSTRINGLEN + 1];
	struct botuser *user;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "delcommand(\"%s\", \"%s\", %d, \"%s\", \"%s\", \"%s\")\n", (this == NULL) ? "(null)" : this->name, to, tochan, nick, userhost, commandline);
#endif
	/* command line parsing begins here, first parameter = the command */
	ppos = 0;
        if((ppos = parse(commandline, ppos, " ", p1, STRINGLEN, 0)) < 0) {
                return;
        }
	if(!strcasecmp(p1, "admin")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) < 1) {
			return;
		}
		while(1) {
                        nppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0);
                        if(nppos == ppos || nppos < 0) {
                                break;
                        }
			ppos = nppos;
			if(!deladmin(p2)) {
#ifdef OER_DEBUG
				oer_debug(OER_DEBUG_INFO, "delcommand->%s!%s attempted to delete protected admin %s\n", nick, userhost, p2);
				
#endif
				continue;
			}
			snprintf(stringbuffer, BIGSTRINGLEN, "deleted admin %s", p2);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
	if(!strcasecmp(p1, "chankey")) {
		if(this == NULL) {
			return;
		}
		if(!isadmin(this, userhost)) {
			return;
		}
		if((numofparams(commandline) != 0)) {
			return;
		}
		delchankey(this);
		snprintf(stringbuffer, BIGSTRINGLEN, "removed channel key from %s", this->name);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "chanlimit")) {
		if(this == NULL) {
			return;
		}
		if(!isadmin(this, userhost)) {
			return;
		}
		if((numofparams(commandline) != 0)) {
			return;
		}
		if(delchanlimit(this)) {
			snprintf(stringbuffer, BIGSTRINGLEN, "deleted channel limit from %s", this->name);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
	if(!strcasecmp(p1, "chanmode")) {
		if(this == NULL) {
			return;
		}
		if(!isadmin(this, userhost)) {
			return;
		}
		if((numofparams(commandline) != 0)) {
			return;
		}
		if(delchanmode(this)) {
			snprintf(stringbuffer, BIGSTRINGLEN, "deleted %s channel mode", this->name);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
	if(!strcasecmp(p1, "channel")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) < 1) {
			return;
		}
		while(1) {
                        nppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0);
                        if(nppos == ppos || nppos < 0) {
                                break;
                        }
			ppos = nppos;
			if((this = getchptr(p2)) == NULL) {
				continue;
			}
			last_stat = -1;
			seen_stat = -1;
			if(index(this->chanflags, (int)'L') != NULL) {
				last_stat = 0;
			}
			if(index(this->chanflags, (int)'S') != NULL) {
				seen_stat = 0;
			}
			if(last_stat == 0) {
				if(this->last != NULL) {
					if(fclose(this->last) == EOF) {
#ifdef OER_DEBUG
						oer_debug(OER_DEBUG_ERROR, "delcommand->fclose() failed: %s\n", strerror(errno));
#endif
					}
					this->last = NULL;
					snprintf(p3, STRINGLEN, "last-%s", this->name);
					snprintf(stringbuffer, BIGSTRINGLEN, "%s has been closed, you will have to delete it manually", p3);
					sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
				}
			}
			if(seen_stat == 0) {
				if(this->seen != NULL) {
					if(fclose(this->seen) == EOF) {
#ifdef OER_DEBUG
						oer_debug(OER_DEBUG_ERROR, "delcommand->fclose() failed: %s\n", strerror(errno));
#endif
					}
					this->seen = NULL;
					snprintf(p3, STRINGLEN, "seen-%s", this->name);
					snprintf(stringbuffer, BIGSTRINGLEN, "%s has been closed, you will have to delete it manually", p3);
					sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
				}
			}
			strncpy(p3, to, STRINGLEN);
			if(!delchannel(this)) {
				return;
			}
			snprintf(stringbuffer, BIGSTRINGLEN, "deleted channel %s", p2);
			sendreply(p3, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		/* force save here since there is no way to get
		   back .chaninfo for a channel that was deleted */
		savechannels();
		savechaninfo();
		return;
	}
	if(!strcasecmp(p1, "chanflags")) {
		if(this == NULL) {
			return;
		}
		if(!isadmin(this, userhost)) {
			return;
		}
		if(numofparams(commandline) != 1) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		validateflags(p2, p3, OER_FLAGS_TYPE_CHANNEL);
		strncpy(p2, p3, STRINGLEN);
		if(!strlen(p2)) {
			return;
		}
		if(index(p2, (int)'x') != NULL) {
			cleana = 1;
		} else {
			cleana = 0;
		}
		last_stat = -1;
		seen_stat = -1;
		if(index(p2, (int)'L') != NULL && index(this->chanflags, (int)'L') != NULL) {
			last_stat = 0;
		}
		if(index(p2, (int)'S') != NULL && index(this->chanflags, (int)'S') != NULL) {
			seen_stat = 0;
		}
		if(!deltaflags(this, p2, '-')) {
			return;
		}
		snprintf(stringbuffer, BIGSTRINGLEN, "%s chanflags are now %s", this->name, this->chanflags);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		if(last_stat == 0) {
			if(this->last != NULL) {
				if(fclose(this->last) == EOF) {
#ifdef OER_DEBUG
					oer_debug(OER_DEBUG_ERROR, "delcommand->fclose() failed: %s\n", strerror(errno));
#endif
				}
				this->last = NULL;
				snprintf(p3, STRINGLEN, "last-%s", this->name);
				snprintf(stringbuffer, BIGSTRINGLEN, "%s has been closed, you will have to delete it manually", p3);
				sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
			}
		}
		if(seen_stat == 0) {
			if(this->seen != NULL) {
				if(fclose(this->seen) == EOF) {
#ifdef OER_DEBUG
					oer_debug(OER_DEBUG_ERROR, "delcommand->fclose() failed: %s\n", strerror(errno));
#endif
				}
				this->seen = NULL;
				snprintf(p3, STRINGLEN, "seen-%s", this->name);
				snprintf(stringbuffer, BIGSTRINGLEN, "%s has been closed, you will have to delete it manually", p3);
				sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
			}
		}
		if(this->allhostsknown && this->i_am_op) {
			channelsync(this);
		}
		if(cleana) {
			cleanautheds(this, 0);
		}
		return;
	}
	if(!strcasecmp(p1, "user")) {
		if(this == NULL) {
			return;
		}
		if(numofparams(commandline) < 1) {
			return;
		}
		while(1) {
			nppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0);
			if(nppos == ppos || nppos < 0) {
				break;
			}
			ppos = nppos;
			for(user = this->users; user != NULL; user = user->next) {
				if(!strcasecmp(user->handle, p2)) {
					break;
				}
			}
			if(user == NULL) {
				continue;
			}
			proceed = (isopa(userhost)) ? 1 : 0;
			if(index(user->options, (int)'a') == NULL) {
				/* a normal user */
				if(isadmin(this, userhost)) {
					proceed = 1;
				}
			}
			if(!proceed) {
				continue;
			}
                        if(index(user->options, (int)'d') != NULL) {
				/* dyn user, remove all autheds */
				logoff(this, p2);
			}
			if(!deluser(this, p2)) {
				continue;
			}
			snprintf(stringbuffer, BIGSTRINGLEN, "deleted %s channel user %s", this->name, p2);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		if(this->i_am_op) {
			syncops(this);
		}
		return;
	}
	if(!strcasecmp(p1, "mask")) {
		if(numofparams(commandline) < 3) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p3, STRINGLEN, 0)) < 0) {
			return;
		}
		if(!strcasecmp(p2, "admin")) {
			if(!isopa(userhost)) {
				return;
			}
			while(1) {
				nppos = parse(commandline, ppos, " ", p4, STRINGLEN, 0);
				if(nppos == ppos || nppos < 0) {
					break;
				}
				ppos = nppos;
				if(!deladminmask(p3, p4)) {
					continue;
				}
				snprintf(stringbuffer, BIGSTRINGLEN, "deleted mask %s from admin %s", p4, p3);
				sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
			}
			return;
		}
		if(!strcasecmp(p2, "user")) {
			if(this == NULL) {
				return;
			}
			for(user = this->users; user != NULL; user = user->next) {
				if(!strcasecmp(user->handle, p3)) {
					break;
				}
			}
			if(user == NULL) {
				return;
			}
			proceed = (isopa(userhost)) ? 1 : 0;
			if(index(user->options, (int)'a') != NULL) {
				/* the user has +a flag */
				if(isadmin(this, userhost) && issameuser(this, p3, userhost)) {
					proceed = 1;
				}
			} else {
				/* a normal user */
				if(isadmin(this, userhost)) {
					proceed = 1;
				}
				if(isop(this, userhost) && issameuser(this, p3, userhost)) {
					proceed = 1;
				}
				if(haschanflags(this, "M") && !isadmin(this, userhost)) {
					proceed = 0;
				}
				if(index(user->options, (int)'m') != NULL && !isadmin(this, userhost)) {
					proceed = 0;
				}
			}
			if(!proceed) {
				return;
			}
			while(1) {
				nppos = parse(commandline, ppos, " ", p4, STRINGLEN, 0);
				if(nppos == ppos || nppos < 0) {
					break;
				}
				ppos = nppos;
				if(!delusermasks(this, p3, p4, 0)) {
					continue;
				}
				snprintf(stringbuffer, BIGSTRINGLEN, "deleted mask %s from %s user %s", p4, this->name, p3);
				sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
			}
			if(this->i_am_op) {
				syncops(this);
			}
			return;
		}
		return;
	}
	if(!strcasecmp(p1, "permban")) {
		if(this == NULL) {
			return;
		}
		/* check auth for this specific channel,
		   checkauth() does a global authority check (all channels) */
                if(haschanflags(this, "U") && !isatleastop(this, nick, userhost)) {
                        return;
                }
                if(!isatleastopnow(this, nick, userhost)) {
                        return;
                }
		if(numofparams(commandline) < 1) {
			return;
		}
		while(1) {
                        nppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0);
                        if(nppos == ppos || nppos < 0) {
                                break;
                        }
			ppos = nppos;
			if(!delpermbans(this, p2, 0)) {
				continue;
			}
			if(this->i_am_op) {
				mmode_new(this, mystate->now, "-b", p2);
			}
			snprintf(stringbuffer, BIGSTRINGLEN, "removed permban %s from channel %s", p2, this->name);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		if(this->i_am_op) {
			syncbans(this);
		}
		return;
	}
	if(!strcasecmp(p1, "adverts")) {
		if(this == NULL) {
			return;
		}
		/* check auth for this specific channel,
		   checkauth() does a global authority check (all channels) */
                if(haschanflags(this, "U") && !isatleastop(this, nick, userhost)) {
                        return;
                }
                if(!isatleastopnow(this, nick, userhost)) {
                        return;
                }
		if(numofparams(commandline) < 1) {
			return;
		}
		while(1) {
                        nppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0);
                        if(nppos == ppos || nppos < 0) {
                                break;
                        }
			ppos = nppos;
			if(!strcasecmp(p2, "admins") && !isadmin(this, userhost)) {
				continue;
			}
			if(!deladverts(this, p2, 0)) {
				continue;
			}
			snprintf(stringbuffer, BIGSTRINGLEN, "removed all adverts directed to \"%s\" from channel %s", p2, this->name);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
	if(!strcasecmp(p1, "nickbk")) {
		if(this == NULL) {
			return;
		}
		/* check auth for this specific channel,
		   checkauth() does a global authority check (all channels) */
                if(haschanflags(this, "U") && !isatleastop(this, nick, userhost)) {
                        return;
                }
                if(!isatleastopnow(this, nick, userhost)) {
                        return;
                }
		if(numofparams(commandline) < 1) {
			return;
		}
		while(1) {
                        nppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0);
                        if(nppos == ppos || nppos < 0) {
                                break;
                        }
			ppos = nppos;
			if(!delnickbks(this, p2, 0)) {
				continue;
			}
			snprintf(stringbuffer, BIGSTRINGLEN, "removed nickbk %s from channel %s", p2, this->name);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
	if(!strcasecmp(p1, "wordbk")) {
		if(this == NULL) {
			return;
		}
		/* check auth for this specific channel,
		   checkauth() does a global authority check (all channels) */
                if(haschanflags(this, "U") && !isatleastop(this, nick, userhost)) {
                        return;
                }
                if(!isatleastopnow(this, nick, userhost)) {
                        return;
                }
		if(numofparams(commandline) < 1) {
			return;
		}
		while(1) {
                        nppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0);
                        if(nppos == ppos || nppos < 0) {
                                break;
                        }
			ppos = nppos;
			if(!delwordbks(this, p2, 0)) {
				continue;
			}
			snprintf(stringbuffer, BIGSTRINGLEN, "removed wordbk %s from channel %s", p2, this->name);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
	if(!strcasecmp(p1, "kickreason")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) < 1) {
			return;
		}
		if(delkickreason(commandline + ppos)) {
			snprintf(stringbuffer, BIGSTRINGLEN, "removed kickreason: %s", commandline + ppos);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
	if(!strcasecmp(p1, "trusted")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) < 1) {
			return;
		}
		while(1) {
                        nppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0);
                        if(nppos == ppos || nppos < 0) {
                                break;
                        }
			ppos = nppos;
			if(!deltrusted(p2)) {
				continue;
			}
			snprintf(stringbuffer, BIGSTRINGLEN, "removed trusted host %s", p2);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
	if(!strcasecmp(p1, "service")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) < 1) {
			return;
		}
		while(1) {
                        nppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0);
                        if(nppos == ppos || nppos < 0) {
                                break;
                        }
			ppos = nppos;
			if(!delservice(p2)) {
				continue;
			}
			snprintf(stringbuffer, BIGSTRINGLEN, "removed service %s", p2);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
	if(!strcasecmp(p1, "server")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) != 6) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p3, STRINGLEN, 0)) < 0) {
			return;
		}
		if(!strlen(p3)) {
			return;
		}
		port = atoi(p3);
		if((ppos = parse(commandline, ppos, " ", p4, STRINGLEN, 0)) < 0) {
			return;
		}
		modes = (strlen(p4) > 0) ? atoi(p4) : 6;
		if((ppos = parse(commandline, ppos, " ", p5, STRINGLEN, 0)) < 0) {
			return;
		}
		ping = (strlen(p5) > 0) ? atoi(p5) : 90;
		if((ppos = parse(commandline, ppos, " ", p6, STRINGLEN, 0)) < 0) {
			return;
		}
		opers = (strlen(p6) > 0) ? atoi(p6) : 1;
		if((ppos = parse(commandline, ppos, " ", p7, STRINGLEN, 0)) < 0) {
			return;
		}
		noise = (strlen(p7) > 0) ? atoi(p7) : 1;
		if(!delserver(p2, port, modes, ping, opers, noise)) {
			return;
		}
                snprintf(stringbuffer, BIGSTRINGLEN, "removed IRC server: %s %d %d %d %d %d", p2, port, modes, ping, opers, noise);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "flags")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) < 1) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		validateflags(p2, p3, OER_FLAGS_TYPE_GLOBAL);
		strncpy(p2, p3, STRINGLEN);
		if(!strlen(p2)) {
			return;
		}
		/* check if we have to change some behaviour */
		if(index(p2, (int)'l')) {
			if(index(mystate->flags, (int)'l') != NULL) {
				if(fclose(mystate->logfp) == EOF) {
#ifdef OER_DEBUG
					oer_debug(OER_DEBUG_ERROR, "delcommand->fclose() failed: %s\n", strerror(errno));
#endif
				}
				mystate->logfp = NULL;
#ifdef OER_DEBUG
				oer_debug(OER_DEBUG_INFO, "delcommand->logging terminated by request\n");
#endif
			}
		}
		if(!deltaflags(NULL, p2, '-')) {
			return;
		}
		snprintf(stringbuffer, BIGSTRINGLEN, "my flags are now %s", mystate->flags);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "qauth")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) != 0) {
			return;
		}
		snprintf(stringbuffer, BIGSTRINGLEN, "removed qauth information for %s", mystate->qauth.q);
		memset(&mystate->qauth, 0, sizeof(struct qauth));
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "proxysetup")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) != 0) {
			return;
		}
		memset(mystate->proxysetup, 0, STRINGLEN + 1);
		snprintf(stringbuffer, BIGSTRINGLEN, "removed proxysetup information");
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "vhost")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) != 0) {
			return;
		}
		memset(mystate->vhost, 0, HOSTLEN + 1);
		snprintf(stringbuffer, BIGSTRINGLEN, "removed vhost");
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "script")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) < 1) {
			return;
		}
		while(1) {
			nppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0);
			if(nppos == ppos || nppos < 0) {
				break;
			}
			ppos = nppos;
			if(!delscript(p2)) {
				continue;
			}
			snprintf(stringbuffer, BIGSTRINGLEN, "removed script %s", p2);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
}

void sendadverts(struct channel *this, char *nick, char *userhost)
{
	int send_normals;
	int send_ops;
	int send_admins;
	int send_other;
	int proceed;
	struct advert *a;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "sendadverts(\"%s\", \"%s\", \"%s\")\n", this->name, nick, userhost);
#endif
	send_normals = 1;
	send_ops = 0;
	send_admins = 0;
	send_other = 0;
	if(isadmin(this, userhost)) {
		send_normals = 0;
		send_ops = 1;
		send_admins = 1;
	}
	if(isop(this, userhost)) {
		send_normals = 0;
		send_ops = 1;
	}
	if(hasuserflags(this, userhost, "d", "") || hasuserflags(this, userhost, "f", "")) {
		send_normals = 0;
		send_other = 1;
	}
	for(a = this->adverts; a != NULL; a = a->next) {
		proceed = 0;
		/* check for static types */
		if(!strcmp(a->to, "normals") && send_normals) {
			proceed = 1;
		}
		if(!strcmp(a->to, "ops") && send_ops) {
			proceed = 1;
		}
		if(!strcmp(a->to, "admins") && send_admins) {
			proceed = 1;
		}
		if(!strcmp(a->to, "other") && send_other) {
			proceed = 1;
		}
		if(!strcmp(a->to, "all")) {
			proceed = 1;
		}
		/* then check user@host */
		if(wild_match(a->to, userhost)) {
			proceed = 1;
		}
		if(proceed) {
			sendreply(nick, 0, 0, OER_TIMED_PRIORITY_ADVERT, a->message);
		}
	}
}

void addcommand(struct channel *this, char *to, int tochan, char *nick, char *userhost, char *commandline)
{
	int amount;
	int port;
	int modes;
	int ping;
	int opers;
	int noise;
	int proceed;
	int ppos;
	int nppos;
	int last_stat;
	int seen_stat;
	char *nptr;
        char p1[STRINGLEN + 1];
        char p2[STRINGLEN + 1];
        char p3[STRINGLEN + 1];
        char p4[STRINGLEN + 1];
        char p5[STRINGLEN + 1];
        char p6[STRINGLEN + 1];
        char p7[STRINGLEN + 1];
        char p8[STRINGLEN + 1];
	char stringbuffer[BIGSTRINGLEN + 1];
	char *pwptr;
	struct botuser *user;
	struct maskstruct *ms;
	struct server *server;
	struct advert *a;
	struct script *s;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "addcommand(\"%s\", \"%s\", %d, \"%s\", \"%s\", \"%s\")\n", (this == NULL) ? "(null)" : this->name, to, tochan, nick, userhost, commandline);
#endif
	/* command line parsing begins here, first parameter = the command */
	ppos = 0;
        if((ppos = parse(commandline, ppos, " ", p1, STRINGLEN, 0)) < 0) {
                return;
        }
	if(!strcasecmp(p1, "channel")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) < 1) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p3, STRINGLEN, 0)) < 0) {
			return;
		}
		if(!isvalidchannel(p2)) {
			return;
		}
		if((this = addnewchannel(p2)) == NULL) {
			return;
		}
		snprintf(stringbuffer, BIGSTRINGLEN, "added new channel %s", this->name);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		if(strlen(p3)) {
			snprintf(stringbuffer, BIGSTRINGLEN, "chanflags %s", p3);
			editcommand(this, to, tochan, nick, userhost, stringbuffer);
		}
		return;
	}
	if(!strcasecmp(p1, "chanflags")) {
		if(this == NULL) {
			return;
		}
		if(!isadmin(this, userhost)) {
			return;
		}
		if(numofparams(commandline) != 1) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		validateflags(p2, p3, OER_FLAGS_TYPE_CHANNEL);
		strncpy(p2, p3, STRINGLEN);
		if(!strlen(p2)) {
			return;
		}
		last_stat = -1;
		seen_stat = -1;
		if(index(p2, (int)'L') != NULL && index(this->chanflags, (int)'L') == NULL) {
			last_stat = 1;
		}
		if(index(p2, (int)'S') != NULL && index(this->chanflags, (int)'S') == NULL) {
			seen_stat = 1;
		}
		if(!deltaflags(this, p2, '+')) {
			return;
		}
		snprintf(stringbuffer, BIGSTRINGLEN, "%s chanflags are now %s", this->name, this->chanflags);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		if(last_stat == 1) {
			if(this->last == NULL) {
				if(strlen(mystate->altloggingdir)) {
					snprintf(p3, STRINGLEN, "%s/last-%s", mystate->altloggingdir, this->name);
				} else {
					snprintf(p3, STRINGLEN, "last-%s", this->name);
				}
				if((this->last = fopen(p3, "a")) == NULL) {
#ifdef OER_DEBUG
					oer_debug(OER_DEBUG_INFO, "addcommand->failed to open %s: %s\n", p3, strerror(errno));
#endif
				} else {
					snprintf(stringbuffer, BIGSTRINGLEN, "%s has been opened for logging", p3);
					sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
				}
			}
		}
		if(seen_stat == 1) {
			if(this->seen == NULL) {
				if(strlen(mystate->altloggingdir)) {
					snprintf(p3, STRINGLEN, "%s/seen-%s", mystate->altloggingdir, this->name);
				} else {
					snprintf(p3, STRINGLEN, "seen-%s", this->name);
				}
				if((this->seen = fopen(p3, "a")) == NULL) {
#ifdef OER_DEBUG
					oer_debug(OER_DEBUG_INFO, "addcommand->failed to open %s: %s\n", p3, strerror(errno));
#endif
				} else {
					snprintf(stringbuffer, BIGSTRINGLEN, "%s has been opened for logging", p3);
					sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
				}
			}
		}
		if(this->allhostsknown && this->i_am_op) {
			channelsync(this);
		}
		return;
	}
	if(!strcasecmp(p1, "server")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) < 2 || numofparams(commandline) > 9) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		if(!isvalidhost(p2)) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p3, STRINGLEN, 0)) < 0) {
			return;
		}
		port = (strlen(p3) > 0) ? atoi(p3) : 6667;
		if(!isnumbw(port, OER_LIMIT_SERVER_PORT_MIN, OER_LIMIT_SERVER_PORT_MAX)) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p4, STRINGLEN, 0)) < 0) {
			return;
		}
		modes = (strlen(p4) > 0) ? atoi(p4) : 4;
		if(!isnumbw(modes, OER_LIMIT_SERVER_MODES_MIN, OER_LIMIT_SERVER_MODES_MAX)) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p5, STRINGLEN, 0)) < 0) {
			return;
		}
		ping = (strlen(p5) > 0) ? atoi(p5) : 90;
		if(!isnumbw(ping, OER_LIMIT_SERVER_PING_MIN, OER_LIMIT_SERVER_PING_MAX)) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p6, STRINGLEN, 0)) < 0) {
			return;
		}
		opers = (strlen(p6) > 0) ? atoi(p6) : 0;
		if(!isnumbw(opers, OER_LIMIT_SERVER_PROTOPER_MIN, OER_LIMIT_SERVER_PROTOPER_MAX)) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p7, STRINGLEN, 0)) < 0) {
			return;
		}
		noise = (strlen(p7) > 0) ? atoi(p7) : 0;
                if(noise && !isnumbw(noise, OER_LINENOISE_INTERVAL_MIN, ping)) {
                        return;
                }
		if((ppos = parse(commandline, ppos, " ", p8, STRINGLEN, 0)) < 0) {
			return;
		}
		pwptr = (strlen(p8)) ? p8 : NULL;
		if((server = addnewserver(p2, port, modes, ping, opers, noise, pwptr)) == NULL) {
			return;
		}
                snprintf(stringbuffer, BIGSTRINGLEN, "added new IRC server: %s %d %d %d %d %d %s", server->serverhost, server->serverport, server->servermodes, server->pingfrequency, server->protected_ircops, server->linenoise, (pwptr == NULL) ? "(null)" : pwptr);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "admin")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) < 1 || numofparams(commandline) > 2) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p3, STRINGLEN, 0)) < 0) {
			return;
		}
		/* only allow +n admins to add other +n admins */
		if((index(p3, (int)'n') != NULL) && !hasadminflags(userhost, "n")) {
			return;
		}
		validateflags(p3, p4, OER_FLAGS_TYPE_ADMIN);
		strncpy(p3, p4, STRINGLEN);
		if((user = addnewadmin(p2, p3)) == NULL) {
			return;
		}
		snprintf(stringbuffer, BIGSTRINGLEN, "added admin %s as %s", p2, p3);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "user")) {
		if(this == NULL) {
			return;
		}
		if(!isadmin(this, userhost)) {
			return;
		}
                if(numofparams(commandline) < 1 || numofparams(commandline) > 2) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p3, STRINGLEN, 0)) < 0) {
			return;
		}
		if(index(p3, (int)'a') != NULL) {
			/* only admins can add +a users */
			if(!isopa(userhost)) {
				return;
			}
		}
		validateflags(p3, p4, OER_FLAGS_TYPE_USER);
		strncpy(p3, p4, STRINGLEN);
		if((user = addnewuser(this, p2, p3)) == NULL) {
			return;
		}
		snprintf(stringbuffer, BIGSTRINGLEN, "added user %s to channel %s as %s", p2, this->name, p3);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "mask")) {
		if(numofparams(commandline) < 3) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p3, STRINGLEN, 0)) < 0) {
			return;
		}
		if(!strcasecmp(p2, "admin")) {
			if(!isopa(userhost)) {
				return;
			}
			while(1) {
				nppos = parse(commandline, ppos, " ", p4, STRINGLEN, 0);
				if(nppos == ppos || nppos < 0) {
					break;
				}
				ppos = nppos;
				if((ms = addnewadminmask(p3, p4)) == NULL) {
					continue;
				}
				snprintf(stringbuffer, BIGSTRINGLEN, "added mask %s to admin %s", p4, p3);
				sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
			}
			return;
		}
		if(!strcasecmp(p2, "user")) {
			if(this == NULL) {
				return;
			}
			for(user = this->users; user != NULL; user = user->next) {
				if(!strcasecmp(user->handle, p3)) {
					break;
				}
			}
			if(user == NULL) {
				return;
			}
			proceed = (isopa(userhost)) ? 1 : 0;
			if(index(user->options, (int)'a') != NULL) {
				/* the user has +a flag */
				if(isadmin(this, userhost) && issameuser(this, p3, userhost)) {
					proceed = 1;
				}
			} else {
				/* a normal user */
				if(isadmin(this, userhost)) {
					proceed = 1;
				}
				if(isop(this, userhost) && issameuser(this, p3, userhost)) {
					proceed = 1;
				}
				if(haschanflags(this, "M") && !isadmin(this, userhost)) {
					proceed = 0;
				}
				if(index(user->options, (int)'m') != NULL && !isadmin(this, userhost)) {
					proceed = 0;
				}
			}
			if(!proceed) {
				return;
			}
			while(1) {
				nppos = parse(commandline, ppos, " ", p4, STRINGLEN, 0);
				if(nppos == ppos || nppos < 0) {
					break;
				}
				ppos = nppos;
				if((ms = addnewusermask(this, p3, p4)) == NULL) {
					continue;
				}
				snprintf(stringbuffer, BIGSTRINGLEN, "added mask %s to %s user %s", p4, this->name, p3);
				sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
			}
			if(this->i_am_op) {
				syncvoices(this);
				syncops(this);
			}
			return;
		}
		return;
	}
	if(!strcasecmp(p1, "permban")) {
		if(this == NULL) {
			return;
		}
		/* check auth for this specific channel,
		   checkauth() does a global authority check (all channels) */
                if(haschanflags(this, "U") && !isatleastop(this, nick, userhost)) {
                        return;
                }
                if(!isatleastopnow(this, nick, userhost)) {
                        return;
                }
		if(numofparams(commandline) < 1) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		nptr = commandline + ppos;
		amount = permbancount(this);
		if(amount >= OER_PERMBANS) {
			snprintf(stringbuffer, BIGSTRINGLEN, "I won't add permban %s to channel %s (max. %d)", p2, this->name, OER_PERMBANS);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
			return;
		}
		if((ms = addnewpermban(this, p2, nick, strlen(nptr) ? nptr : NULL)) == NULL) {
			return;
		}
		if(this->i_am_op) {
			mmode_new(this, mystate->now, "+b", ms->mask);
		}
		snprintf(stringbuffer, BIGSTRINGLEN, "added new permban %s to channel %s", ms->mask, this->name);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		if(this->i_am_op) {
			syncbans(this);
		}
		return;
	}
	if(!strcasecmp(p1, "nickbk")) {
		if(this == NULL) {
			return;
		}
		/* check auth for this specific channel,
		   checkauth() does a global authority check (all channels) */
                if(haschanflags(this, "U") && !isatleastop(this, nick, userhost)) {
                        return;
                }
                if(!isatleastopnow(this, nick, userhost)) {
                        return;
                }
		if(numofparams(commandline) < 1) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		nptr = commandline + ppos;
		if((ms = addnewnickbk(this, p2, nick, strlen(nptr) ? nptr : NULL)) == NULL) {
			return;
		}
		if(this->i_am_op) {
			syncnickbks(this);
		}
		snprintf(stringbuffer, BIGSTRINGLEN, "added nick %s as ban & kick for channel %s", ms->mask, this->name);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "wordbk")) {
		if(this == NULL) {
			return;
		}
		/* check auth for this specific channel,
		   checkauth() does a global authority check (all channels) */
                if(haschanflags(this, "U") && !isatleastop(this, nick, userhost)) {
                        return;
                }
                if(!isatleastopnow(this, nick, userhost)) {
                        return;
                }
		if(numofparams(commandline) < 1) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		nptr = commandline + ppos;
		if((ms = addnewwordbk(this, p2, nick, strlen(nptr) ? nptr : NULL)) == NULL) {
  			return;
		}
		snprintf(stringbuffer, BIGSTRINGLEN, "added word %s as ban & kick for channel %s", ms->mask, this->name);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "kickreason")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) < 1) {
			return;
		}
		if(strlen(commandline + ppos) > 0) {
			/* check that the kickreason doesn't exist already */
			for(ms = mystate->kickreasons; ms != NULL; ms = ms->next) {
				if(!strcasecmp(ms->mask, commandline + ppos)) {
					snprintf(stringbuffer, BIGSTRINGLEN, "kickreason %s already exists", commandline + ppos);
					sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
					return;
				}
			}
			if((ms = addnewkickreason(commandline + ppos)) == NULL) {
				return;
			}
			snprintf(stringbuffer, BIGSTRINGLEN, "added new kickreason: %s", ms->mask);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
	if(!strcasecmp(p1, "advert")) {
		if(this == NULL) {
			return;
		}
		/* check auth for this specific channel,
		   checkauth() does a global authority check (all channels) */
                if(haschanflags(this, "U") && !isatleastop(this, nick, userhost)) {
                        return;
                }
                if(!isatleastopnow(this, nick, userhost)) {
                        return;
                }
		if(numofparams(commandline) < 2) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		if(!strcasecmp(p2, "admins") && !isadmin(this, userhost)) {
			return;
		}
		if((a = addnewadvert(this, p2, commandline + ppos)) == NULL) {
			return;
		}
		snprintf(stringbuffer, BIGSTRINGLEN, "added new advert \"%s\" (to: %s) to channel %s", a->message, a->to, this->name);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "trusted")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) < 1) {
			return;
		}
		while(1) {
                        nppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0);
                        if(nppos == ppos || nppos < 0) {
                                break;
                        }
			ppos = nppos;
			if(!isvalidhost(p2)) {
				continue;
			}
			if((ms = addnewtrusted(p2)) == NULL) {
				continue;
			}
			snprintf(stringbuffer, BIGSTRINGLEN, "added new trusted host %s", ms->mask);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
	if(!strcasecmp(p1, "service")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) < 1) {
			return;
		}
		while(1) {
                        nppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0);
                        if(nppos == ppos || nppos < 0) {
                                break;
                        }
			ppos = nppos;
			if(!isvalidhost(p2)) {
				continue;
			}
			if((ms = addnewservice(p2)) == NULL) {
				continue;
			}
			snprintf(stringbuffer, BIGSTRINGLEN, "added new service %s", ms->mask);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
	if(!strcasecmp(p1, "flags")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) < 1) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		validateflags(p2, p3, OER_FLAGS_TYPE_GLOBAL);
		strncpy(p2, p3, STRINGLEN);
		if(!strlen(p2)) {
			return;
		}
		/* check if we have to change some behaviour */
		if(index(p2, (int)'l')) {
			if(index(mystate->flags, (int)'l') == NULL) {
				if(strlen(mystate->altloggingdir)) {
					snprintf(stringbuffer, WRITE_BUFFER_LENGTH, "%s/oer-%d.raw", mystate->altloggingdir, getpid());
				} else {
					snprintf(stringbuffer, WRITE_BUFFER_LENGTH, "oer-%d.raw", getpid());
				}
				if((mystate->logfp = fopen(stringbuffer, "a+")) == NULL) {
#ifdef OER_DEBUG
					oer_debug(OER_DEBUG_INFO, "addcommand->couldn't open logfile %s: %s\n", stringbuffer, strerror(errno));
#endif
				} else {
#ifdef OER_DEBUG
					oer_debug(OER_DEBUG_INFO, "addcommand->logfile is %s\n", stringbuffer);
#endif
				}
			}
		}
		if(!deltaflags(NULL, p2, '+')) {
			return;
		}
		snprintf(stringbuffer, BIGSTRINGLEN, "my flags are now %s", mystate->flags);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "script")) {
		if(!isopa(userhost)) {
			return;
		}
		if(numofparams(commandline) < 1) {
			return;
		}
		while(1) {
			nppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0);
			if(nppos == ppos || nppos < 0) {
				break;
			}
			ppos = nppos;
			if((s = addnewscript(p2)) == NULL) {
				continue;
			}
			snprintf(stringbuffer, BIGSTRINGLEN, "added new script %s", s->filename);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
}

void listcommand(struct channel *this, char *to, int tochan, char *nick, char *userhost, char *commandline)
{
	int lines;
	int startpos;
	int curpos;
	int more;
	int amount;
	int ppos;
	int proceed;
	int suppressed;
        char p1[STRINGLEN + 1];
        char p2[STRINGLEN + 1];
        char p3[STRINGLEN + 1];
	char outstring[STRINGLEN + 1];
	char stringbuffer[HUGESTRINGLEN + 1];
	char stringbuffer2[HUGESTRINGLEN + 1];
        char allflags[FLAGLEN + 1];
	char ts[TINYSTRINGLEN + 1];
	struct server *server;
	struct botuser *admin;
	struct botuser *user;
	struct maskstruct *ms;
	struct chanuser *cu;
	struct authed *a;
	struct advert *adv;
	struct channel *that;
	struct channelban *cb;
	struct script *s;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "listcommand(\"%s\", \"%s\", %d, \"%s\", \"%s\", \"%s\")\n", (this == NULL) ? "(null)" : this->name, to, tochan, nick, userhost, commandline);
#endif
	ppos = 0;
        if((ppos = parse(commandline, ppos, " ", p1, STRINGLEN, 0)) < 0) {
                return;
        }
	if(!strcasecmp(p1, "nick")) {
		snprintf(stringbuffer, HUGESTRINGLEN, "my nick is %s", mystate->nick);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "altnick")) {
		snprintf(stringbuffer, HUGESTRINGLEN, "my alternative nick is %s", mystate->altnick);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "user")) {
		snprintf(stringbuffer, HUGESTRINGLEN, "my user ID (ident) is %s", mystate->user);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "flags")) {
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
                        return;
                }
		sortstring(mystate->flags);
                if(!strlen(p2)) {
                        /* non verbose */
			snprintf(stringbuffer, HUGESTRINGLEN, "my flags are %s", mystate->flags);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
                        return;
                }
                if(strcasecmp(p2, "verbose")) {
                        return;
                }
		memset(outstring, 0, STRINGLEN + 1);
		memset(stringbuffer, 0, HUGESTRINGLEN + 1);
		strncpy(allflags, OER_FLAGS_GLOBAL, FLAGLEN);
		for(curpos = 0, amount = strlen(allflags); curpos < amount; curpos++) {
                        if((strlen(stringbuffer) + 10) > BIGSTRINGLEN) {
				/* strip leading space if any */
				if(strlen(stringbuffer)) {
					stringbuffer[strlen(stringbuffer) - 1] = '\0';
				}
				snprintf(stringbuffer2, HUGESTRINGLEN, "my flags are [%s] %s", outstring, stringbuffer);
				sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer2);
				memset(outstring, 0, STRINGLEN + 1);
				memset(stringbuffer, 0, HUGESTRINGLEN + 1);
			}
			switch(allflags[curpos]) {
			case 'b':
				strncat(outstring, (index(mystate->flags, (int)'b') != NULL) ? "b" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (index(mystate->flags, (int)'b') != NULL) ? "b/backupconfs=1 " : "b/backupconfs=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'd':
				strncat(outstring, (index(mystate->flags, (int)'d') != NULL) ? "d" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (index(mystate->flags, (int)'d') != NULL) ? "d/intdynadmin=1 " : "d/intdynadmin=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'f':
				strncat(outstring, (index(mystate->flags, (int)'f') != NULL) ? "f" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (index(mystate->flags, (int)'f') != NULL) ? "f/friendlyctcp=1 " : "f/friendlyctcp=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'g':
				strncat(outstring, (index(mystate->flags, (int)'g') != NULL) ? "g" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (index(mystate->flags, (int)'g') != NULL) ? "g/getandkeepnick=1 " : "g/getandkeepnick=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'l':
				strncat(outstring, (index(mystate->flags, (int)'l') != NULL) ? "l" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (index(mystate->flags, (int)'l') != NULL) ? "l/lograw=1 " : "l/lograw=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'm':
				strncat(outstring, (index(mystate->flags, (int)'m') != NULL) ? "m" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (index(mystate->flags, (int)'m') != NULL) ? "m/msgmode=1 " : "m/msgmode=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'p':
				strncat(outstring, (index(mystate->flags, (int)'p') != NULL) ? "p" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (index(mystate->flags, (int)'p') != NULL) ? "p/pingdisabled=1 " : "p/pingdisabled=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'q':
				strncat(outstring, (index(mystate->flags, (int)'q') != NULL) ? "q" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (index(mystate->flags, (int)'q') != NULL) ? "q/qfriendly=1 " : "q/qfriendly=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 's':
				strncat(outstring, (index(mystate->flags, (int)'s') != NULL) ? "s" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (index(mystate->flags, (int)'s') != NULL) ? "s/services=1 " : "s/services=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			}
		}
		if(strlen(stringbuffer) || strlen(outstring)) {
			/* strip leading space if any */
			if(strlen(stringbuffer)) {
				stringbuffer[strlen(stringbuffer) - 1] = '\0';
			}
			snprintf(stringbuffer2, HUGESTRINGLEN, "my flags are [%s] %s", outstring, stringbuffer);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer2);
		}
		return;
	}
	if(!strcasecmp(p1, "prefix")) {
		snprintf(stringbuffer, HUGESTRINGLEN, "my command prefix is %s", mystate->prefix);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "usermode")) {
		snprintf(stringbuffer, HUGESTRINGLEN, "my usermodes are %s", mystate->mode);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "realname")) {
		snprintf(stringbuffer, HUGESTRINGLEN, "my IRC REALNAME is %s", mystate->realname);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "vhost")) {
		snprintf(stringbuffer, HUGESTRINGLEN, "my virtual host is %s", mystate->vhost);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "servers")) {
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		startpos =  (strlen(p2) > 0) ? atoi(p2) : 1;
		if(!isnumbw(startpos, OER_LIMIT_LISTCOMMAND_STARTPOS_MIN, OER_LIMIT_LISTCOMMAND_STARTPOS_MAX)) {
                        return;
                }
		for(server = mystate->servers, lines = 0, curpos = 1, more = 0; server != NULL; server = server->next, curpos++) {
			if(lines < OER_MAX_LIST_LINES) {
				if(curpos >= startpos) {
					snprintf(stringbuffer, HUGESTRINGLEN, "%s %d %d %d %d %d %s%s", server->serverhost, server->serverport, server->servermodes, server->pingfrequency, server->protected_ircops, server->linenoise, (strlen(server->password) && isopa(userhost)) ? server->password : "(null)", (server == mystate->current_server) ? " (current)" : "");
					sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
					lines++;
				}
			} else {
				more = 1;
			}
		}
		if(more) {
			snprintf(stringbuffer, HUGESTRINGLEN, "%d servers, listing limited to %d lines", curpos - 1, OER_MAX_LIST_LINES);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
	if(!strcasecmp(p1, "adverts")) {
		if(this == NULL) {
			return;
		}
		if(haschanflags(this, "U") && !isatleastop(this, nick, userhost)) {
			return;
		}
		if(!isatleastopnow(this, nick, userhost)) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		startpos =  (strlen(p2) > 0) ? atoi(p2) : 1;
		if(!isnumbw(startpos, OER_LIMIT_LISTCOMMAND_STARTPOS_MIN, OER_LIMIT_LISTCOMMAND_STARTPOS_MAX)) {
                        return;
                }
		for(adv = this->adverts, lines = 0, curpos = 1, more = 0; adv != NULL; adv = adv->next, curpos++) {
			if(!strcmp(adv->to, "admins") && !isadmin(this, userhost)) {
				continue;
			}
			if(wild_match(adv->to, "*@*")) {
				if(!isadmin(this, userhost) && !wild_match(adv->to, userhost)) {
					continue;
				}
			}
			if(tochan) {
				/* if the command is sent to a channel, check that the channel
				   doesn't have any users that shouldn't see the adverts */
				proceed = 1;
				for(cu = this->nicks; cu != NULL; cu = cu->next) {
					if(!strcasecmp(adv->to, "admins") && !isadmin(this, cu->userhost)) {
						proceed = 0;
						break;
					}
					if(!strcasecmp(adv->to, "ops") && !isatleastop(this, cu->nick, cu->userhost)) {
						proceed = 0;
						break;
					}
					if(wild_match(adv->to, "*@*")) {
						if(!isadmin(this, cu->userhost) && !wild_match(adv->to, cu->userhost)) {
							proceed = 0;
							break;
						}
					}
				}
				if(!proceed) {
					continue;
				}
			}
			if(lines < OER_MAX_LIST_LINES) {
				if(curpos >= startpos) {
					snprintf(stringbuffer, HUGESTRINGLEN, "%s (to: %s)", adv->message, adv->to);
					sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
					lines++;
				}
			} else {
				more = 1;
			}
		}
		if(more) {
			snprintf(stringbuffer, HUGESTRINGLEN, "%d adverts, listing limited to %d lines", curpos - 1, OER_MAX_LIST_LINES);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
	if(!strcasecmp(p1, "trusted")) {
		if(!isopa(userhost)) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		startpos =  (strlen(p2) > 0) ? atoi(p2) : 1;
		if(!isnumbw(startpos, OER_LIMIT_LISTCOMMAND_STARTPOS_MIN, OER_LIMIT_LISTCOMMAND_STARTPOS_MAX)) {
                        return;
                }
		for(ms = mystate->trusted, lines = 0, curpos = 1, more = 0; ms != NULL; ms = ms->next, curpos++) {
			if(lines < OER_MAX_LIST_LINES) {
				if(curpos >= startpos) {
					snprintf(stringbuffer, HUGESTRINGLEN, "trusted: %s", ms->mask);
					sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
					lines++;
				}
			} else {
				more = 1;
			}
		}
		if(more) {
			snprintf(stringbuffer, HUGESTRINGLEN, "%d trusted hosts, listing limited to %d lines", curpos - 1, OER_MAX_LIST_LINES);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
	if(!strcasecmp(p1, "services")) {
		if(!isopa(userhost)) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		startpos =  (strlen(p2) > 0) ? atoi(p2) : 1;
		if(!isnumbw(startpos, OER_LIMIT_LISTCOMMAND_STARTPOS_MIN, OER_LIMIT_LISTCOMMAND_STARTPOS_MAX)) {
                        return;
                }
		for(ms = mystate->services, lines = 0, curpos = 1, more = 0; ms != NULL; ms = ms->next, curpos++) {
			if(lines < OER_MAX_LIST_LINES) {
				if(curpos >= startpos) {
					snprintf(stringbuffer, HUGESTRINGLEN, "service: %s", ms->mask);
					sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
					lines++;
				}
			} else {
				more = 1;
			}
		}
		if(more) {
			snprintf(stringbuffer, HUGESTRINGLEN, "%d services, listing limited to %d lines", curpos - 1, OER_MAX_LIST_LINES);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
	if(!strcasecmp(p1, "admins")) {
		if(numofparams(commandline) >= 1) {
			if(!isopa(userhost)) {
				return;
			}
			if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
				return;
			}
			if((ppos = parse(commandline, ppos, " ", p3, STRINGLEN, 0)) < 0) {
				return;
			}
			for(admin = mystate->admins; admin != NULL; admin = admin->next) {
				if(!strcasecmp(admin->handle, p2)) {
					break;
				}
			}
			if(admin == NULL) {
				return;
			}
			if(strlen(p3) > 0) {
				startpos = atoi(p3);
			} else {
				startpos = 1;
			}
			if(!isnumbw(startpos, OER_LIMIT_LISTCOMMAND_STARTPOS_MIN, OER_LIMIT_LISTCOMMAND_STARTPOS_MAX)) {
				return;
			}
			if(startpos == 1) {
				/* only send header info if no startpos */
				snprintf(stringbuffer, HUGESTRINGLEN, "options for admin %s: %s", admin->handle, admin->options);
				sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
			}
			for(ms = admin->firstmask, lines = 0, curpos = 1, more = 0; ms != NULL; ms = ms->next, curpos++) {
				if(lines < OER_MAX_LIST_LINES) {
					if(curpos >= startpos) {
						snprintf(stringbuffer, HUGESTRINGLEN, "hostmask for admin %s: %s", admin->handle, ms->mask);
						sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
						lines++;
					}
				} else {
					more = 1;
				}
			}
			if(more) {
				snprintf(stringbuffer, HUGESTRINGLEN, "%d hostmasks in total, listing limited to %d lines", curpos - 1, OER_MAX_LIST_LINES);
				sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
			}
			return;
		}
		/* no parameters, list all admins */
		memset(stringbuffer, 0, HUGESTRINGLEN + 1);
		strncpy(stringbuffer, "admins: ", HUGESTRINGLEN);
		admin = mystate->admins;
		while(admin != NULL) {
			if(strlen(stringbuffer) + strlen(admin->handle) > HUGESTRINGLEN) {
				sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
				memset(stringbuffer, 0, HUGESTRINGLEN + 1);
				strncpy(stringbuffer, "admins: ", HUGESTRINGLEN);
			}
			strncat(stringbuffer, admin->handle, HUGESTRINGLEN - strlen(stringbuffer));
			strncat(stringbuffer, " ", HUGESTRINGLEN - strlen(stringbuffer));
			admin = admin->next;
		}
		if(strlen(stringbuffer) > 0) {
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
	if(!strcasecmp(p1, "kickreasons")) {
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		startpos =  (strlen(p2) > 0) ? atoi(p2) : 1;
		if(!isnumbw(startpos, OER_LIMIT_LISTCOMMAND_STARTPOS_MIN, OER_LIMIT_LISTCOMMAND_STARTPOS_MAX)) {
                        return;
                }
		for(ms = mystate->kickreasons, lines = 0, curpos = 1, more = 0; ms != NULL; ms = ms->next, curpos++) {
			if(lines < OER_MAX_LIST_LINES) {
				if(curpos >= startpos) {
					snprintf(stringbuffer, HUGESTRINGLEN, "kickreason: %s", ms->mask);
					sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
					lines++;
				}
			} else {
				more = 1;
			}
		}
		if(more) {
			snprintf(stringbuffer, HUGESTRINGLEN, "%d kickreasons, listing limited to %d lines", curpos - 1, OER_MAX_LIST_LINES);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
	if(!strcasecmp(p1, "channels")) {
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		if(strlen(p2) > 0) {
			startpos = atoi(p2);
			startpos = (startpos >= 1) ? startpos : 1;
		} else {
			startpos = 1;
		}
		if(!isnumbw(startpos, OER_LIMIT_LISTCOMMAND_STARTPOS_MIN, OER_LIMIT_LISTCOMMAND_STARTPOS_MAX)) {
                        return;
                }
		for(that = mystate->channels, lines = 0, curpos = 1, more = 0, suppressed = 0; that != NULL; that = that->next, curpos++) {
			if(curpos < startpos) {
				continue;
			}
			proceed = 1;
			if(tochan) {
				if(strcasecmp(to, that->name)) {
					/* was a channel command -> don't show keyed channels,
					   secret or hidden channels */
					if(strlen(that->key) || getmodepol(that->mode, 's') == '+' || getmodepol(that->mode, 's') == '-') {
						proceed = 0;
					}
				}
			} else {
				/* show keyed, secret and hidden channels only if user is
				   allowed to see them */
				if(strlen(that->key) || getmodepol(that->mode, 's') == '+' || getmodepol(that->mode, 's') == '-') {
					if(haschanflags(that, "U") && !isatleastop(that, nick, userhost)) {
						proceed = 0;
					}
					if(!isatleastopnow(that, nick, userhost)) {
						proceed = 0;
					}
				}
			}
			if(!proceed) {
				suppressed++;
				continue;
			}
			if(lines < OER_MAX_LIST_LINES) {
				if(strlen(that->key)) {
					snprintf(stringbuffer, HUGESTRINGLEN, "%s %s (%d %s) [key: %s]", (that->i_am_op) ? "@" : " ", that->name, that->nickcount, (that->nickcount == 1) ? "user" : "users", that->key);
				} else {
					snprintf(stringbuffer, HUGESTRINGLEN, "%s %s (%d %s)", (that->i_am_op) ? "@" : " ", that->name, that->nickcount, (that->nickcount == 1) ? "user" : "users");
				}
				sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
				lines++;
			} else {
				more = 1;
			}
		}
		if(more) {
			if(suppressed) {
				snprintf(stringbuffer, HUGESTRINGLEN, "%d channels listed (%d suppressed), listing limited to %d lines", lines, suppressed, OER_MAX_LIST_LINES);
			} else {
				snprintf(stringbuffer, HUGESTRINGLEN, "%d channels listed, listing limited to %d lines", lines, OER_MAX_LIST_LINES);
			}
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		} else if(suppressed) {
			snprintf(stringbuffer, HUGESTRINGLEN, "%d channels listed (%d suppressed)", lines, suppressed);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
	if(!strcasecmp(p1, "chankey")) { 
		if(this == NULL) {
			return;
		}
                if(haschanflags(this, "U") && !isatleastop(this, nick, userhost)) {
                        return;
                }
		if(!isatleastopnow(this, nick, userhost)) {
			return;
		}
		snprintf(stringbuffer, HUGESTRINGLEN, "%s channel key is %s", this->name, this->key);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "chanlimit")) { 
		if(this == NULL) {
			return;
		}
                if(haschanflags(this, "U") && !isatleastop(this, nick, userhost)) {
                        return;
                }
		if(!isatleastopnow(this, nick, userhost)) {
			return;
		}
		snprintf(stringbuffer, HUGESTRINGLEN, "channel limit is %s", this->limit);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "chanmode")) {
		if(this == NULL) {
			return;
		}
                if(haschanflags(this, "U") && !isatleastop(this, nick, userhost)) {
                        return;
                }
		if(!isatleastopnow(this, nick, userhost)) {
			return;
		}
		snprintf(stringbuffer, HUGESTRINGLEN, "channel modes are %s", this->mode);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "chanflags")) {
		if(this == NULL) {
			return;
		}
                if(haschanflags(this, "U") && !isatleastop(this, nick, userhost)) {
                        return;
                }
		if(!isatleastopnow(this, nick, userhost)) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
                        return;
                }
		sortstring(this->chanflags);
                if(!strlen(p2)) {
                        /* non verbose */
                        snprintf(outstring, STRINGLEN, "channel flags for %s are %s", this->name, this->chanflags);
                        sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, outstring);
                        return;
                }
                if(strcasecmp(p2, "verbose")) {
                        return;
                }
		memset(outstring, 0, STRINGLEN + 1);
		memset(stringbuffer, 0, HUGESTRINGLEN + 1);
		strncpy(allflags, OER_FLAGS_CHANNEL, FLAGLEN);
		for(curpos = 0, amount = strlen(allflags); curpos < amount; curpos++) {
                        if((strlen(stringbuffer) + 10) > BIGSTRINGLEN) {
				/* strip leading space if any */
				if(strlen(stringbuffer)) {
					stringbuffer[strlen(stringbuffer) - 1] = '\0';
				}
				snprintf(stringbuffer2, HUGESTRINGLEN, "channel flags for %s are [%s] %s", this->name, outstring, stringbuffer);
				sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer2);
				memset(outstring, 0, STRINGLEN + 1);
				memset(stringbuffer, 0, HUGESTRINGLEN + 1);
			}
			switch(allflags[curpos]) {
			case '!':
				strncat(outstring, (haschanflags(this, "!")) ? "!" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "!")) ? "!/inactive=1 " : "!/inactive=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'a':
				strncat(outstring, (haschanflags(this, "a")) ? "a" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "a")) ? "a/ansi_prot=1 " : "a/ansi_prot=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'A':
				strncat(outstring, (haschanflags(this, "A")) ? "A" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "A")) ? "A/adverts=1 " : "A/adverts=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'b':
				strncat(outstring, (haschanflags(this, "b")) ? "b" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "b")) ? "b/ban_prot=1 " : "b/ban_prot=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'c':
				strncat(outstring, (haschanflags(this, "c")) ? "c" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "c")) ? "c/noactions=1 " : "c/noactions=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'D':
				strncat(outstring, (haschanflags(this, "D")) ? "D" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "D")) ? "D/de-op_protect=1 " : "D/de-op_protect=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'e':
				strncat(outstring, (haschanflags(this, "e")) ? "e" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "e")) ? "e/perm_bans=1 " : "e/perm_bans=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'f':
				strncat(outstring, (haschanflags(this, "f")) ? "f" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "f")) ? "f/pubmsg_floodp=1 " : "f/pubmsg_floodp=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'F':
				strncat(outstring, (haschanflags(this, "F")) ? "F" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "F")) ? "F/friends=1 " : "F/friends=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'G':
				strncat(outstring, (haschanflags(this, "G")) ? "G" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "G")) ? "G/allflood=1 " : "G/allflood=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'k':
				strncat(outstring, (haschanflags(this, "k")) ? "k" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "k")) ? "k/nickflood=1 " : "k/nickflood=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'l':
				strncat(outstring, (haschanflags(this, "l")) ? "l" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "l")) ? "l/lock_chan=1 " : "l/lock_chan=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'L':
				strncat(outstring, (haschanflags(this, "L")) ? "L" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "L")) ? "L/last=1 " : "L/last=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'm':
				strncat(outstring, (haschanflags(this, "m")) ? "m" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "m")) ? "m/mass_prot=1 " : "m/mass_prot=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'M':
				strncat(outstring, (haschanflags(this, "M")) ? "M" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "M")) ? "M/restrictedmasks=1 " : "M/restrictedmasks=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'n':
				strncat(outstring, (haschanflags(this, "n")) ? "n" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "n")) ? "n/nick_bk=1 " : "n/nick_bk=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'N':
				strncat(outstring, (haschanflags(this, "N")) ? "N" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "N")) ? "N/nodynops=1 " : "N/nodynops=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'o':
				strncat(outstring, (haschanflags(this, "o")) ? "o" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "o")) ? "o/auto_op=1 " : "o/auto_op=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'O':
				strncat(outstring, (haschanflags(this, "O")) ? "O" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "O")) ? "O/auto_op_admins=1 " : "O/auto_op_admins=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'p':
				strncat(outstring, (haschanflags(this, "p")) ? "p" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "p")) ? "p/postnj_check=1 " : "p/postnj_check=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'P':
				strncat(outstring, (haschanflags(this, "P")) ? "P" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "P")) ? "P/paranoid=1 " : "P/paranoid=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'q':
				strncat(outstring, (haschanflags(this, "q")) ? "q" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "q")) ? "q/quote=1 " : "q/quote=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'r':
				strncat(outstring, (haschanflags(this, "r")) ? "r" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "r")) ? "r/autorejoin_kb=1 " : "r/autorejoin_kb=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'R':
				strncat(outstring, (haschanflags(this, "R")) ? "R" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "R")) ? "R/requestop=1 " : "R/requestop=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 's':
				strncat(outstring, (haschanflags(this, "s")) ? "s" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "s")) ? "s/serveall=1 " : "s/serveall=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'S':
				strncat(outstring, (haschanflags(this, "S")) ? "S" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "S")) ? "S/seen=1 " : "S/seen=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'u':
				strncat(outstring, (haschanflags(this, "u")) ? "u" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "u")) ? "u/users_op=1 " : "u/users_op=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'U':
				strncat(outstring, (haschanflags(this, "U")) ? "U" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "U")) ? "U/botusersonly=1 " : "U/botusersonly=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 't':
				strncat(outstring, (haschanflags(this, "t")) ? "t" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "t")) ? "t/topiclock=1 " : "t/topiclock=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'T':
				strncat(outstring, (haschanflags(this, "T")) ? "T" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "T")) ? "T/notopics=1 " : "T/notopics=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'v':
				strncat(outstring, (haschanflags(this, "v")) ? "v" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "v")) ? "v/auto_voice=1 " : "v/auto_voice=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'V':
				strncat(outstring, (haschanflags(this, "V")) ? "V" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "V")) ? "V/auto_voice_admins=1 " : "V/auto_voice_admins=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'w':
				strncat(outstring, (haschanflags(this, "w")) ? "w" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "w")) ? "w/word_bk=1 " : "w/word_bk=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'x':
				strncat(outstring, (haschanflags(this, "x")) ? "x" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "x")) ? "x/noauthexpire=1 " : "x/noauthexpire=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			case 'X':
				strncat(outstring, (haschanflags(this, "X")) ? "X" : "", STRINGLEN - strlen(outstring));
				strncat(stringbuffer, (haschanflags(this, "X")) ? "X/nofun=1 " : "X/nofun=0 ", HUGESTRINGLEN - strlen(stringbuffer));
				break;
			}
		}
		if(strlen(stringbuffer) || strlen(outstring)) {
			/* strip leading space if any */
			if(strlen(stringbuffer)) {
				stringbuffer[strlen(stringbuffer) - 1] = '\0';
			}
			snprintf(stringbuffer2, HUGESTRINGLEN, "channel flags for %s are [%s] %s", this->name, outstring, stringbuffer);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer2);
		}
		return;
	}
	if(!strcasecmp(p1, "users")) {
		if(this == NULL) {
			return;
		}
                if(haschanflags(this, "U") && !isatleastop(this, nick, userhost)) {
                        return;
                }
		if(!isatleastopnow(this, nick, userhost)) {
			return;
		}
		if(numofparams(commandline) >= 1) {
			if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
				return;
			}
			if((ppos = parse(commandline, ppos, " ", p3, STRINGLEN, 0)) < 0) {
				return;
			}
			for(user = this->users; user != NULL; user = user->next) {
				if(!strcasecmp(user->handle, p2)) {
					break;
				}
			}
			if(user == NULL) {
				return;
			}
			proceed = (isopa(userhost)) ? 1 : 0;
			if(index(user->options, (int)'a') != NULL) {
				if(isadmin(this, userhost)) {
					proceed = 1;
				}
			} else {
				proceed = 1;
			}
			if(!proceed) {
				return;
			}
			if(strlen(p3) > 0) {
				startpos = atoi(p3);
			} else {
				startpos = 1;
			}
			if(!isnumbw(startpos, OER_LIMIT_LISTCOMMAND_STARTPOS_MIN, OER_LIMIT_LISTCOMMAND_STARTPOS_MAX)) {
				return;
			}
			if(startpos == 1) {
				/* only send header info if no startpos */
				snprintf(stringbuffer, HUGESTRINGLEN, "options for user %s: %s", user->handle, user->options);
				sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
			}
			for(ms = user->firstmask, lines = 0, curpos = 1, more = 0; ms != NULL; ms = ms->next, curpos++) {
				if(lines < OER_MAX_LIST_LINES) {
					if(curpos >= startpos) {
						snprintf(stringbuffer, HUGESTRINGLEN, "hostmask for user %s: %s", user->handle, ms->mask);
						sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
						lines++;
					}
				} else {
					more = 1;
				}
			}
			if(more) {
				snprintf(stringbuffer, HUGESTRINGLEN, "%d hostmasks in total, listing limited to %d lines", curpos - 1, OER_MAX_LIST_LINES);
				sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
			}
			return;
		}
                /* no parameters, list all bot users */
		memset(stringbuffer, 0, HUGESTRINGLEN + 1);
		snprintf(stringbuffer, HUGESTRINGLEN, "%s users: ", this->name);
		for(user = this->users; user != NULL; user = user->next) {
			if(strlen(stringbuffer) + strlen(user->handle) > HUGESTRINGLEN) {
				sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
				memset(stringbuffer, 0, HUGESTRINGLEN + 1);
				snprintf(stringbuffer, HUGESTRINGLEN, "%s users: ", this->name);
			}
			if(index(user->options, (int)'!') != NULL) {
				strncat(stringbuffer, "!", HUGESTRINGLEN - strlen(stringbuffer));
			}
			if(index(user->options, (int)'a') != NULL) {
				strncat(stringbuffer, "*", HUGESTRINGLEN - strlen(stringbuffer));
			}
			strncat(stringbuffer, user->handle, HUGESTRINGLEN - strlen(stringbuffer));
			strncat(stringbuffer, " ", HUGESTRINGLEN - strlen(stringbuffer));
		}
		if(strlen(stringbuffer) > 0) {
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
	if(!strcasecmp(p1, "nicks")) {
		if(this == NULL) {
			return;
		}
                if(haschanflags(this, "U") && !isatleastop(this, nick, userhost)) {
                        return;
                }
		if(!isatleastopnow(this, nick, userhost)) {
			return;
		}
		if(numofparams(commandline) >= 1) {
			if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
				return;
			}
			for(cu = this->nicks; cu != NULL; cu = cu->next) {
				if(!strcasecmp(cu->nick, p2)) {
					break;
				}
			}
			if(cu == NULL) {
				return;
			}
			snprintf(stringbuffer, HUGESTRINGLEN, "%s!%s (ircop=%d chanop=%d voice=%d friend=%d)", cu->nick, (cu->userhost == NULL) ? "(null)" : cu->userhost, cu->ircop, cu->chanop, cu->voice, cu->friend);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
			return;
		}
                /* no parameters, list all channel nicks */
		amount = 0;
		memset(stringbuffer, 0, HUGESTRINGLEN + 1);
		cu = this->nicks;
		while(cu != NULL) {
			if((strlen(stringbuffer) + CHANLEN + NICKLEN + 16) > HUGESTRINGLEN) {
				/* send the thing, empty string */
				snprintf(stringbuffer2, HUGESTRINGLEN, "nicks on %s [%d]:", this->name, amount);
				strncat(stringbuffer2, stringbuffer, HUGESTRINGLEN - strlen(stringbuffer2));
				sendreply(to, tochan, 0, OER_TIMED_PRIORITY_FLOOD, stringbuffer2);
				amount = 0;
				memset(stringbuffer, 0, HUGESTRINGLEN + 1);
			}
			strncat(stringbuffer, " ", HUGESTRINGLEN - strlen(stringbuffer));
			if(cu->ircop) {
				strncat(stringbuffer, "*", HUGESTRINGLEN - strlen(stringbuffer));
			}
			if(cu->voice) {
				strncat(stringbuffer, "+", HUGESTRINGLEN - strlen(stringbuffer));
			}
			if(cu->chanop) {
				strncat(stringbuffer, "@", HUGESTRINGLEN - strlen(stringbuffer));
			}
			strncat(stringbuffer, cu->nick, HUGESTRINGLEN - strlen(stringbuffer));
			amount++;
			cu = cu->next;
		}
		if(amount > 0) {
			snprintf(stringbuffer2, HUGESTRINGLEN, "nicks on %s [%d]:", this->name, amount);
			strncat(stringbuffer2, stringbuffer, HUGESTRINGLEN - strlen(stringbuffer2));
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_FLOOD, stringbuffer2);
		}
		return;
	}
	if(!strcasecmp(p1, "wordbks")) {
		if(this == NULL) {
			return;
		}
                if(haschanflags(this, "U") && !isatleastop(this, nick, userhost)) {
                        return;
                }
		if(!isatleastopnow(this, nick, userhost)) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		startpos =  (strlen(p2) > 0) ? atoi(p2) : 1;
		if(!isnumbw(startpos, OER_LIMIT_LISTCOMMAND_STARTPOS_MIN, OER_LIMIT_LISTCOMMAND_STARTPOS_MAX)) {
                        return;
                }
		for(ms = this->wordbks, lines = 0, curpos = 1, more = 0; ms != NULL; ms = ms->next, curpos++) {
			if(lines < OER_MAX_LIST_LINES) {
				if(curpos >= startpos) {
					snprintf(stringbuffer, HUGESTRINGLEN, "%s (reason: %s, set by: %s)", ms->mask, (ms->optstring2 == NULL) ? OER_UNKNOWN : ms->optstring2, (ms->optstring1 == NULL) ? OER_UNKNOWN : ms->optstring1);
					sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
					lines++;
				}
			} else {
				more = 1;
			}
		}
		if(more) {
			snprintf(stringbuffer, HUGESTRINGLEN, "%d %s wordbks, listing limited to %d lines", curpos - 1, this->name, OER_MAX_LIST_LINES);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
	if(!strcasecmp(p1, "nickbks")) {
		if(this == NULL) {
			return;
		}
                if(haschanflags(this, "U") && !isatleastop(this, nick, userhost)) {
                        return;
                }
		if(!isatleastopnow(this, nick, userhost)) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		startpos =  (strlen(p2) > 0) ? atoi(p2) : 1;
		if(!isnumbw(startpos, OER_LIMIT_LISTCOMMAND_STARTPOS_MIN, OER_LIMIT_LISTCOMMAND_STARTPOS_MAX)) {
                        return;
                }
		for(ms = this->nickbks, lines = 0, curpos = 1, more = 0; ms != NULL; ms = ms->next, curpos++) {
			if(lines < OER_MAX_LIST_LINES) {
				if(curpos >= startpos) {
					snprintf(stringbuffer, HUGESTRINGLEN, "%s (reason: %s, set by: %s)", ms->mask, (ms->optstring2 == NULL) ? OER_UNKNOWN : ms->optstring2, (ms->optstring1 == NULL) ? OER_UNKNOWN : ms->optstring1);
					sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
					lines++;
				}
			} else {
				more = 1;
			}
		}
		if(more) {
			snprintf(stringbuffer, HUGESTRINGLEN, "%d %s nickbks, listing limited to %d lines", curpos - 1, this->name, OER_MAX_LIST_LINES);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
	if(!strcasecmp(p1, "permbans")) {
		if(this == NULL) {
			return;
		}
                if(haschanflags(this, "U") && !isatleastop(this, nick, userhost)) {
                        return;
                }
		if(!isatleastopnow(this, nick, userhost)) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		startpos =  (strlen(p2) > 0) ? atoi(p2) : 1;
		if(!isnumbw(startpos, OER_LIMIT_LISTCOMMAND_STARTPOS_MIN, OER_LIMIT_LISTCOMMAND_STARTPOS_MAX)) {
                        return;
                }
		for(ms = this->permbans, lines = 0, curpos = 1, more = 0; ms != NULL; ms = ms->next, curpos++) {
			if(lines < OER_MAX_LIST_LINES) {
				if(curpos >= startpos) {
					snprintf(stringbuffer, HUGESTRINGLEN, "%s (reason: %s, set by: %s)", ms->mask, (ms->optstring2 == NULL) ? OER_UNKNOWN : ms->optstring2, (ms->optstring1 == NULL) ? OER_UNKNOWN : ms->optstring1);
					sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
					lines++;
				}
			} else {
				more = 1;
			}
		}
		if(more) {
			snprintf(stringbuffer, HUGESTRINGLEN, "%d %s permbans, listing limited to %d lines", curpos - 1, this->name, OER_MAX_LIST_LINES);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
	if(!strcasecmp(p1, "floodvars")) {
		if(this == NULL) {
			return;
		}
                if(haschanflags(this, "U") && !isatleastop(this, nick, userhost)) {
                        return;
                }
		if(!isatleastopnow(this, nick, userhost)) {
			return;
		}
		snprintf(stringbuffer, HUGESTRINGLEN, "%s floodvars are %d %d %d %d %d %d %d", this->name, this->floodvars.repeat_expire, this->floodvars.repeat_limit, this->floodvars.interval, this->floodvars.lines, this->floodvars.chars, this->floodvars.nickflood_expire, this->floodvars.nickflood_changes);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
	}
	if(!strcasecmp(p1, "banvars")) {
		if(this == NULL) {
			return;
		}
                if(haschanflags(this, "U") && !isatleastop(this, nick, userhost)) {
                        return;
                }
		if(!isatleastopnow(this, nick, userhost)) {
			return;
		}
		snprintf(stringbuffer, HUGESTRINGLEN, "%s banvars are %d %d %d %d %d %d %d", this->name, this->banvars.auto_rejoin, this->banvars.part_rejoin, this->banvars.public_flood, this->banvars.public_flood_repeat, this->banvars.bad_word, this->banvars.bad_nick, this->banvars.normal_ban);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
	}
	if(!strcasecmp(p1, "bantype")) {
		if(this == NULL) {
			return;
		}
                if(haschanflags(this, "U") && !isatleastop(this, nick, userhost)) {
                        return;
                }
		if(!isatleastopnow(this, nick, userhost)) {
			return;
		}
		snprintf(stringbuffer, HUGESTRINGLEN, "%s ban type is %d", this->name, this->bantype);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
	}
	if(!strcasecmp(p1, "autheds")) {
                if(haschanflags(this, "U") && !isatleastop(this, nick, userhost)) {
                        return;
                }
		if(!isatleastopnow(this, nick, userhost)) {
			return;
		}
		if(numofparams(commandline) >= 1) {
			if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
				return;
			}
			if((ppos = parse(commandline, ppos, " ", p3, STRINGLEN, 0)) < 0) {
				return;
			}
			if(this == NULL && !isopa(userhost)) {
				return;
			}
			startpos =  (strlen(p3) > 0) ? atoi(p3) : 1;
			if(!isnumbw(startpos, OER_LIMIT_LISTCOMMAND_STARTPOS_MIN, OER_LIMIT_LISTCOMMAND_STARTPOS_MAX)) {
				return;
			}
			proceed = (isadmin(this, userhost)) ? 1 : 0;
			if(issameuser(this, p2, userhost)) {
				proceed = 1;
			}
			if(!proceed) {
				return;
			}
			for(a  = (this == NULL) ? mystate->autheds : this->autheds, lines = 0, curpos = 1, more = 0; a != NULL; a = a->next, curpos++) {
				if(this != NULL) {
					if(!haschanflags(this, "x") && (mystate->now > (a->at + OER_LOGON_TIMEOUT))) {
						continue;
					}
				} else if(mystate->now > (a->at + OER_LOGON_TIMEOUT)) {
					continue;
				}
				if(strcasecmp(a->handle, p2)) {
					continue;
				}
				if(lines < OER_MAX_LIST_LINES) {
					if(curpos >= startpos) {
						snprintf(stringbuffer, HUGESTRINGLEN, "%s is authed from %s", a->handle, a->userhost);
						sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
						lines++;
					}
				} else {
					more = 1;
				}
			}
			if(more) {
				snprintf(stringbuffer, HUGESTRINGLEN, "%d hostmasks, listing limited to %d lines", curpos - 1, OER_MAX_LIST_LINES);
				sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
			}
			return;
		}
                /* no parameters, list all autheds for channel */
		amount = 0;
		memset(stringbuffer, 0, HUGESTRINGLEN + 1);
		if(this == NULL && !isopa(userhost)) {
			return;
		}
		for(a = (this == NULL) ? mystate->autheds : this->autheds; a != NULL; a = a->next) {
			if((strlen(stringbuffer) + CHANLEN + NICKLEN + 16) > HUGESTRINGLEN) {
				/* send the thing, empty string */
				if(this == NULL) {
					snprintf(stringbuffer2, HUGESTRINGLEN, "authed admins [%d]:", amount);
				} else {
					snprintf(stringbuffer2, HUGESTRINGLEN, "autheds on %s [%d]:", this->name, amount);
				}
				strncat(stringbuffer2, stringbuffer, HUGESTRINGLEN - strlen(stringbuffer2));
				sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer2);
				amount = 0;
				memset(stringbuffer, 0, HUGESTRINGLEN + 1);
			}
			strncat(stringbuffer, " ", HUGESTRINGLEN - strlen(stringbuffer));
			if(this != NULL) {
				if(!haschanflags(this, "x") && (mystate->now > (a->at + OER_LOGON_TIMEOUT))) {
					strncat(stringbuffer, "!", HUGESTRINGLEN - strlen(stringbuffer));
				}
			} else if(mystate->now > (a->at + OER_LOGON_TIMEOUT)) {
				strncat(stringbuffer, "!", HUGESTRINGLEN - strlen(stringbuffer));
			}
			strncat(stringbuffer, a->handle, HUGESTRINGLEN - strlen(stringbuffer));
			amount++;
		}
		if(amount > 0) {
			if(this == NULL) {
				snprintf(stringbuffer2, HUGESTRINGLEN, "authed admins [%d]:", amount);
			} else {
				snprintf(stringbuffer2, HUGESTRINGLEN, "autheds on %s [%d]:", this->name, amount);
			}
			strncat(stringbuffer2, stringbuffer, HUGESTRINGLEN - strlen(stringbuffer2));
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer2);
		}
		return;
	}
	if(!strcasecmp(p1, "handles")) {
		if(this == NULL) {
			return;
		}
                if(haschanflags(this, "U") && !isatleastop(this, nick, userhost)) {
                        return;
                }
		if(!isatleastopnow(this, nick, userhost)) {
			return;
		}
		if(numofparams(commandline) != 1) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		for(cu = this->nicks; cu != NULL; cu = cu->next) {
			if(!strcasecmp(cu->nick, p2)) {
				break;
			}
		}
		if(cu == NULL || cu->userhost == NULL) {
			return;
		}
		/* check global admins */
		amount = 0;
		memset(stringbuffer, 0, HUGESTRINGLEN + 1);
		snprintf(stringbuffer, HUGESTRINGLEN, "admin handles matching %s: ", cu->nick);
		for(user = mystate->admins; user != NULL; user = user->next) {
			for(ms = user->firstmask; ms != NULL; ms = ms->next) {
				if(wild_match(ms->mask, cu->userhost)) {
					/* this one matches, continue with next user */
					if(strlen(stringbuffer) + strlen(user->handle) > HUGESTRINGLEN) {
						sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
						memset(stringbuffer, 0, HUGESTRINGLEN + 1);
						snprintf(stringbuffer, HUGESTRINGLEN, "admin handles matching %s: ", cu->nick);
					}
					strncat(stringbuffer, user->handle, HUGESTRINGLEN - strlen(stringbuffer));
					strncat(stringbuffer, " ", HUGESTRINGLEN - strlen(stringbuffer));
					amount++;
					break;
				}
			}
		}
		if((amount > 0) && (strlen(stringbuffer) > 0)) {
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		/* check channel handles */
		amount = 0;
		memset(stringbuffer, 0, HUGESTRINGLEN + 1);
		snprintf(stringbuffer, HUGESTRINGLEN, "%s handles matching %s: ", this->name, cu->nick);
		for(user = this->users; user != NULL; user = user->next) {
			for(ms = user->firstmask; ms != NULL; ms = ms->next) {
				if(wild_match(ms->mask, cu->userhost)) {
					/* this one matches, continue with next user */
					if(strlen(stringbuffer) + strlen(user->handle) > HUGESTRINGLEN) {
						sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
						memset(stringbuffer, 0, HUGESTRINGLEN + 1);
						snprintf(stringbuffer, HUGESTRINGLEN, "%s handles matching %s: ", this->name, cu->nick);
					}
					strncat(stringbuffer, user->handle, HUGESTRINGLEN - strlen(stringbuffer));
					strncat(stringbuffer, " ", HUGESTRINGLEN - strlen(stringbuffer));
					amount++;
					break;
				}
			}
		}
		if((amount > 0) && (strlen(stringbuffer) > 0)) {
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
	if(!strcasecmp(p1, "channelbans")) {
		if(this == NULL) {
			return;
		}
                if(haschanflags(this, "U") && !isatleastop(this, nick, userhost)) {
                        return;
                }
		if(!isatleastopnow(this, nick, userhost)) {
			return;
		}
		if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
			return;
		}
		startpos =  (strlen(p2) > 0) ? atoi(p2) : 1;
		if(!isnumbw(startpos, OER_LIMIT_LISTCOMMAND_STARTPOS_MIN, OER_LIMIT_LISTCOMMAND_STARTPOS_MAX)) {
                        return;
                }
		for(cb = this->channelbans, lines = 0, curpos = 1, more = 0; cb != NULL; cb = cb->next, curpos++) {
			if(lines < OER_MAX_LIST_LINES) {
				if(curpos >= startpos) {
					if(cb->setby && cb->at) {
						strncpy(ts, ctime(&cb->at), TINYSTRINGLEN);
						striplf(ts);
						tzset();
						snprintf(stringbuffer, HUGESTRINGLEN, "%s (set by %s at %s %s %s)", cb->ban, cb->setby, ts, tzname[0], tzname[1]);
					} else {
						snprintf(stringbuffer, HUGESTRINGLEN, "%s", cb->ban);
					}
					sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
					lines++;
				}
			} else {
				more = 1;
			}
		}
		if(more) {
			snprintf(stringbuffer, HUGESTRINGLEN, "%d %s channelbans, listing limited to %d lines", curpos - 1, this->name, OER_MAX_LIST_LINES);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		}
		return;
	}
    if(!strcasecmp(p1, "scripts")) {
        if(this == NULL) {
            return;
        }
        if(haschanflags(this, "U") && !isatleastop(this, nick, userhost)) {
            return;
        }
        if(!isatleastopnow(this, nick, userhost)) {
            return;
        }
        if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
            return;
        }
        startpos =  (strlen(p2) > 0) ? atoi(p2) : 1;
        if(!isnumbw(startpos, OER_LIMIT_LISTCOMMAND_STARTPOS_MIN, OER_LIMIT_LISTCOMMAND_STARTPOS_MAX)) {
            return;
        }
        for(s = mystate->scripts, lines = 0, curpos = 1, more = 0; s != NULL; s = s->next, curpos++) {
            if(lines < OER_MAX_LIST_LINES) {
                if(curpos >= startpos) {
                    snprintf(stringbuffer, HUGESTRINGLEN, "script: %s", s->filename);
                    sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
                    lines++;
                }
            } else {
                more = 1;
            }
        }
        if(more) {
            snprintf(stringbuffer, HUGESTRINGLEN, "%d scripts, listing limited to %d lines", curpos - 1, OER_MAX_LIST_LINES);
            sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
        }
        return;
    }
}

void logoffcommand(struct channel *this, char *to, int tochan, char *nick, char *userhost, char *commandline)
{
	int ppos;
	int nppos;
	int syncadmins;
	int syncusers;
        char p1[STRINGLEN + 1];
	char stringbuffer[HUGESTRINGLEN + 1];
        struct botuser *user;
	struct maskstruct *ms;
	struct channel *that;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "logoffcommand(\"%s\", \"%s\", %d, \"%s\", \"%s\", \"%s\")\n", (this == NULL) ? "(null)" : this->name, to, tochan, nick, userhost, commandline);
#endif
	ppos = 0;
	syncadmins = 0;
	syncusers = 0;
	while(1) {
		nppos = parse(commandline, ppos, " ", p1, STRINGLEN, 0);
		if(nppos == ppos || nppos < 0) {
			break;
		}
		ppos = nppos;
                for(user = (this == NULL) ? mystate->admins : this->users; user != NULL; user = user->next) {
                        if(!strcasecmp(user->handle, p1)) {
				break;
			}
                }
		if(user == NULL) {
			continue;
		}
		if(index(user->options, (int)'d') == NULL) {
#ifdef OER_DEBUG
                        oer_debug(OER_DEBUG_INFO, "logoffcommand->handle %s has no \"d\" flag\n", user->handle);
#endif
                        continue;
                }
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "logoffcommand->logoff for handle %s\n", user->handle);
#endif
		if(this == NULL) {
			/* global admin logoff */
			if(!isopa(userhost)) {
				continue;
			}
			/* check mask */
			for(ms = user->firstmask; ms != NULL; ms = ms->next) {
				if(wild_match(ms->mask, userhost)) {
					break;
				}
			}
			if(ms == NULL) {
#ifdef OER_DEBUG
				oer_debug(OER_DEBUG_INFO, "logoffcommand->hostmask %s is invalid for handle %s\n", userhost, user->handle);
#endif
				continue;
			}
			if(!logoff(this, user->handle)) {
#ifdef OER_DEBUG
				oer_debug(OER_DEBUG_INFO, "logoffcommand->logoff failed for handle %s\n", p1);
#endif
				continue;
			}
			snprintf(stringbuffer, HUGESTRINGLEN, "admin %s logged off", user->handle);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
			syncadmins = 1;
		} else {
			/* normal/forced logoff */
			if(!isadmin(this, userhost)) {
				if(!issameuser(this, user->handle, userhost)) {
#ifdef OER_DEBUG
					oer_debug(OER_DEBUG_INFO, "logoffcommand->invalid logoff attempt for handle %s\n", user->handle);
#endif
					continue;
				}
			}
			if(!logoff(this, user->handle)) {
#ifdef OER_DEBUG
				oer_debug(OER_DEBUG_INFO, "logoffcommand->logoff failed for handle %s\n", user->handle);
#endif
				continue;
			}
			snprintf(stringbuffer, HUGESTRINGLEN, "%s user %s successfully logged off", this->name, user->handle);
			sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
			syncusers = 1;
		}
	}
	if(syncadmins) {
		/* sync all channels, this is necessary because of
		   "O" and "V" chanflags */
		for(that = mystate->channels; that != NULL; that = that->next) {
			if(haschanflags(that, "V")) {
				syncvoices(that);
			}
			if(haschanflags(that, "O")) {
				syncops(that);
			}
		}
	}
	if(syncusers) {
		if(haschanflags(this, "v")) {
			syncvoices(this);
		}
		if(haschanflags(this, "o")) {
			syncops(this);
		}
	}
}

void logoncommand(struct channel *this, char *to, int tochan, char *nick, char *userhost, char *commandline)
{
	int admin;
	int ppos;
        char p1[STRINGLEN + 1];
        char p2[STRINGLEN + 1];
	char stringbuffer[HUGESTRINGLEN + 1];
        struct botuser *user;
	struct maskstruct *ms;
	struct channel *that;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "logoncommand(\"%s\", \"%s\", %d, \"%s\", \"%s\", \"%s\")\n", (this == NULL) ? "(null)" : this->name, to, tochan, nick, userhost, commandline);
#endif
	/* we always require 2 parameters, the 2nd one is either hostmask or pw */
        if(numofparams(commandline) != 1) {
                return;
        }
	ppos = 0;
	/* p1 is the handle */
        if((ppos = parse(commandline, ppos, " ", p1, STRINGLEN, 0)) < 0) {
                return;
        }
	/* p2 is either the hostmask (forced logon) or the password */
        if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
                return;
        }
	for(user = (this == NULL) ? mystate->admins : this->users; user != NULL; user = user->next) {
		if(!strcasecmp(user->handle, p1)) {
			break;
		}
	}
	if(user == NULL) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "logoncommand->invalid handle %s\n", p1);
#endif
		return;
	}
	if(index(user->options, (int)'d') == NULL) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "logoncommand->handle %s has no \"d\" flag\n", user->handle);
#endif
		return;
	}
	if(this == NULL) {
		/* admin logon, admins can only logon themselves */
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "logoncommand->admin logon for handle %s\n", user->handle);
#endif
		/* check if the caller has one of the masks of the handle being logoned */
		for(ms = user->firstmask; ms != NULL; ms = ms->next) {
			if(wild_match(ms->mask, userhost)) {
				break;
			}
		}
		if(ms == NULL) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_INFO, "logoncommand->hostmask %s is invalid for handle %s\n", userhost, user->handle);
#endif
			return;
		}
		if(!logon(NULL, user, userhost, p2, 0)) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_INFO, "logoncommand->admin logon failed for handle %s password %s\n", user->handle, p2);
#endif
			return;
		}
		/* sync all channels, this is necessary because of
		   "O" and "V" chanflags */
		for(that = mystate->channels; that != NULL; that = that->next) {
			if(haschanflags(that, "V")) {
				syncvoices(that);
			}
			if(haschanflags(that, "O")) {
				syncops(that);
			}
		}
		snprintf(stringbuffer, HUGESTRINGLEN, "admin %s successfully logged on", user->handle);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	/* is the person requesting the logon a global/channel admin? */
	admin = (isadmin(this, userhost)) ? 1 : 0;
	if((tochan && !admin) || (tochan && admin && !wild_match("*@*", p2))) {
		/* don't allow anything but forced logons to be sent to the channel */
		return;
	}
	/* check whether it is a forced logon or a user wanting to auth */
	if(admin && wild_match("*@*", p2)) {
		/* clearly a forced logon */
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "logoncommand->forced logon for handle %s\n", user->handle);
#endif
		if(!issameuser(this, user->handle, p2)) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_INFO, "logoncommand->hostmask %s is invalid for handle %s\n", p2, user->handle);
#endif
			return;
		}
		if(!logon(this, user, p2, NULL, 1)) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_INFO, "logoncommand->forced logon failed for handle %s\n", user->handle);
#endif
			return;
		}
	} else {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "logoncommand->logon for handle %s\n", user->handle);
#endif
		if(!issameuser(this, user->handle, userhost)) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_INFO, "logoncommand->hostmask %s is invalid for handle %s\n", userhost, user->handle);
#endif
			return;
		}
		if(!logon(this, user, userhost, p2, 0)) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_INFO, "logoncommand->logon failed for handle %s password %s\n", user->handle, p2);
#endif
			return;
		}
	}
	snprintf(stringbuffer, HUGESTRINGLEN, "%s user %s successfully logged on", this->name, user->handle);
	sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
	/* we don't always know who is affected by the logon */
	syncvoices(this);
	syncops(this);
	/* check if user has "f" flag, set friend (if not on channel, nothing happens) */
	if(haschanflags(this, "F") && hasuserflags(this, userhost, "f", "")) {
		changeuser(this, nick, -1, -1, -1, 1);
	}
}

int logoff(struct channel *this, char *handle)
{
	int logoffs;
	struct authed *a;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "logoff(\"%s\", \"%s\")\n", (this == NULL) ? "(null)" : this->name, handle);
#endif
	for(a = (this == NULL) ? mystate->autheds : this->autheds, logoffs = 0; a != NULL; a = a->next) {
		if(strcasecmp(a->handle, handle)) {
			continue;
		}
		logoffs++;
		a->at = 0;
		a->forced_logoff = 1;
	}
	if(logoffs) {
		cleanautheds(this, 0);
		return 1;
	} else {
		return 0;
	}
}

int logon(struct channel *this, struct botuser *user, char *userhost, char *password, int admin)
{
	char cryptsalt[3];
	char *crypted;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "logon(\"%s\", \"%s\", \"%s\", \"%s\", %d)\n", (this == NULL) ? "(null)" : this->name, user->handle, userhost, (password == NULL) ? "(null)" : password, admin);
#endif
	if((this == NULL && user->password == NULL) || (this != NULL && !admin && user->password == NULL)) {
		/* the admin/user has no password */
		return 0;
	}
	if(!admin) {
		cryptsalt[0] = user->password[0];
		cryptsalt[1] = user->password[1];
		cryptsalt[2] = '\0';
		crypted = crypt(password, cryptsalt);
		if(strcasecmp(user->password, crypted)) {
			return 0;
		}
	}
	if(addnewauthed(this, mystate->now, user->handle, userhost) == NULL) {
		return 0;
	}
	cleanautheds(this, 0);
	return 1;
}

int setpassword(struct channel *this, char *handle, char *password)
{
	struct botuser *user;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "setpassword(\"%s\", \"%s\", \"%s\")\n", (this == NULL) ? "(null)" : this->name, handle, password);
#endif
	for(user = (this == NULL) ? mystate->admins : this->users; user != NULL; user = user->next) {
		if(!strcasecmp(user->handle, handle)) {
			break;
		}
	}
	if(user == NULL) {
		return 0;
	}
	if(index(user->options, (int)'d') == NULL) {
		return 0;
	}
	free(user->password);
	if((user->password = (char *) malloc(strlen(password) + 1)) == NULL) {
		return 0;
	}
	strcpy(user->password, password);
	return 1;
}

void sendreply(char *target, int tochan, int delay, int prio, char *message)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "sendreply(\"%s\", %d, %d, %d, \"%s\")\n", target, tochan, delay, prio, message);
#endif
	if(tochan) {
		snprintf(timed_str, WRITE_BUFFER_LENGTH, "PRIVMSG %s :%s", target, message);
	} else {
		if(index(mystate->flags, (int)'m') != NULL) {
			snprintf(timed_str, WRITE_BUFFER_LENGTH, "PRIVMSG %s :%s", target, message);
		} else {
			snprintf(timed_str, WRITE_BUFFER_LENGTH, "NOTICE %s :%s", target, message);
		}
	}
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "sendreply(%lu)->%s\n", delay, timed_str);
#endif
	if(prio != OER_TIMED_PRIORITY_NORMAL) {
		timed_new(NULL, mystate->now + delay, OER_TIMED_TYPE_NORMAL, prio, timed_str);
	} else {
		timed_new(NULL, mystate->now + delay, OER_TIMED_TYPE_NORMAL, (tochan) ? OER_TIMED_PRIORITY_PRIVMSG : OER_TIMED_PRIORITY_NOTICE, timed_str);
	}
}

void sendwall(char *target, struct channel *this, int delay, char *message)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "sendwall(\"%s\", \"%s\", %d, \"%s\")\n", target, this->name, delay, message);
#endif
	if(index(mystate->flags, (int)'m') != NULL) {
		snprintf(timed_str, WRITE_BUFFER_LENGTH, "PRIVMSG %s :%s", target, message);
	} else {
		snprintf(timed_str, WRITE_BUFFER_LENGTH, "NOTICE %s :%s", target, message);
	}
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "sendwall(%lu)->%s\n", delay, timed_str);
#endif
	timed_new(this, mystate->now + delay, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_WALL, timed_str);
}

void sendchannelnotice(struct channel *this, int delay, char *message)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "sendchannelnotice(\"%s\", %d, \"%s\")\n", this->name, delay, message);
#endif
	snprintf(timed_str, WRITE_BUFFER_LENGTH, "NOTICE %s :%s", this->name, message);
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "sendchannelnotice->%s\n", timed_str);
#endif
        timed_new(NULL, mystate->now + delay, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_NOTICE, timed_str);
}

void kickuser(struct channel *this, time_t when, char *nick, char *reason)
{
	struct chanuser *cu;
	char kr[STRINGLEN + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "kickuser(\"%s\", %lu, \"%s\", \"%s\")\n", this->name, when, nick, reason);
#endif
	for(cu = this->nicks; cu != NULL; cu = cu->next) {
		if(!strcasecmp(cu->nick, nick)) {
			break;
		}
	}
	if(cu == NULL) {
		return;
	}
	if(cu->userhost == NULL) {
		return;
	}
	if(reason != NULL) {
		striplf(reason);
		snprintf(timed_str, WRITE_BUFFER_LENGTH, "KICK %s %s :%s", this->name, cu->nick, reason);
	} else {
		if(getkickreason(kr) != NULL) {
			snprintf(timed_str, WRITE_BUFFER_LENGTH, "KICK %s %s :%s", this->name, cu->nick, kr);
		} else {
			snprintf(timed_str, WRITE_BUFFER_LENGTH, "KICK %s %s :%s", this->name, cu->nick, mystate->nick);
		}
	}
        timed_new(NULL, when, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CHANNEL_PROTECTION, timed_str);
}

int isvoicenow(struct channel *this, char *nick)
{
	struct chanuser *cu;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "isvoicenow(\"%s\", \"%s\")\n", this->name, nick);
#endif
	for(cu = this->nicks; cu != NULL; cu = cu->next) {
		if(!strcasecmp(cu->nick, nick)) {
			return cu->voice;
		}
	}
	return 0;
}

int nickchange(struct channel *this, char *nick, char *newnick)
{
	struct chanuser *cu;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "nickchange(\"%s\", \"%s\", \"%s\")\n", this->name, nick, newnick);
#endif
	for(cu = this->nicks; cu != NULL; cu = cu->next) {
		if(!strcasecmp(cu->nick, nick)) {
			free(cu->nick);
			if((cu->nick = (char *) malloc(strlen(newnick) + 1)) == NULL) {
				return 0;
			}
			strcpy(cu->nick, newnick);
			return 1;
		}
	}
	return 0;
}

void cleartobeks(struct channel *this)
{
	struct chanuser *cu;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "cleartobeks(\"%s\")\n", this->name);
#endif
	for(cu = this->nicks; cu != NULL; cu = cu->next) {
		cu->tobek = 0;
	}
}

unsigned int getrandom(unsigned int ceiling)
{
        unsigned int rdelta;
        unsigned int nrand;
        struct timeval tv;
	/* returns a integer between 1..ceiling */
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "getrandom(%u)\n", ceiling);
#endif
        gettimeofday(&tv, 0);
        srand(tv.tv_usec);
        nrand = rand();
        ceiling = (ceiling > 0) ? ceiling : 1;
        rdelta = (nrand / (RAND_MAX / ceiling)) + 1;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "getrandom->returning %u\n", rdelta);
#endif
        return rdelta;
}

struct chanuser *getrandomuser(struct channel *this)
{
	int nthuser;
	int counter;
	struct chanuser *cu;
	struct chanuser *cu2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "getrandomuser(\"%s\")\n", this->name);
#endif
	if(!this->joined) {
		return NULL;
	}
	nthuser = getrandom(this->nickcount) - 1;
	cu = this->nicks;
	counter = 0;
	while(cu != NULL && counter < nthuser) {
		counter++;
		cu = cu->next;
	}
	if(cu == NULL) {
		return NULL;
	}
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "getrandomuser->trying nick %s (position %d)\n", cu->nick, nthuser);
#endif

	for(cu2 = cu; cu != NULL; cu = cu->prev) {
		if(cu->tobek) {
			/* already being kicked */
			continue;
		}
		if(haschanflags(this, "U") && isatleastop(this, cu->nick, cu->userhost)) {
			continue;
		}
		if(isatleastop(this, cu->nick, cu->userhost)) {
			continue;
		}
		if(!haschanflags(this, "N") && isopnow(this, cu->nick)) {
			continue;
		}
		if(haschanflags(this, "F") && isfriend(this, cu->nick, cu->userhost)) {
			continue;
		}
		if(cu->ircop && mystate->current_server->protected_ircops) {
			continue;
		}
		cu->tobek = 1;
		return cu;
	}
	for(cu = cu2; cu != NULL; cu = cu->next) {
		if(cu->tobek) {
			/* already being kicked */
			continue;
		}
		if(haschanflags(this, "U") && isatleastop(this, cu->nick, cu->userhost)) {
			continue;
		}
		if(isatleastop(this, cu->nick, cu->userhost)) {
			continue;
		}
		if(!haschanflags(this, "N") && isopnow(this, cu->nick)) {
			continue;
		}
		if(haschanflags(this, "F") && isfriend(this, cu->nick, cu->userhost)) {
			continue;
		}
		if(cu->ircop && mystate->current_server->protected_ircops) {
			continue;
		}
		cu->tobek = 1;
		return cu;
	}
	return NULL;
}

char *getfirsthostless(struct channel *this, char *nick)
{
        struct chanuser *cu;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "getfirsthostless(\"%s\")\n", this->name);
#endif
	if(!this->joined) {
		return NULL;
	}
        for(cu = this->nicks; cu != NULL; cu = cu->next) {
                if(cu->userhost == NULL && !cu->hostquery) {
                        break;
                }
        }
        if(cu == NULL) {
                return NULL;
        }
        strncpy(nick, cu->nick, NICKLEN);
        return nick;
}

void sethostquerystatus(char *nick, int status)
{
	struct chanuser *cu;
	struct channel *this;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "sethostquerystatus(\"%s\", %d)\n", nick, status);
#endif
        for(this = mystate->channels; this != NULL; this = this->next) {
		for(cu = this->nicks; cu != NULL; cu = cu->next) {
			if(!strcasecmp(cu->nick, nick)) {
				cu->hostquery = status;
				break;
			}
		}
	}
}

void setuserhost(char *nick, char *userhost)
{
	struct chanuser *cu;
	struct channel *this;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "setuserhost(\"%s\", \"%s\")\n", nick, userhost);
#endif
        for(this = mystate->channels; this != NULL; this = this->next) {
		if(!this->joined) {
			continue;
		}
		for(cu = this->nicks; cu != NULL; cu = cu->next) {
			if(!strcasecmp(cu->nick, nick)) {
				free(cu->userhost);
				/* we'll need sometimes to reset the host to
				   force a USERHOST query */
				if(userhost == NULL) {
					cu->userhost = NULL;
					break;
				}
				if((cu->userhost = (char *) malloc(strlen(userhost) + 1)) == NULL) {
					return;
				}
				strcpy(cu->userhost, userhost);
				break;
			}
		}
	}
}

int ishostless(char *nick)
{
	struct chanuser *cu;
	struct channel *this;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "ishostless(\"%s\")\n", nick);
#endif
        for(this = mystate->channels; this != NULL; this = this->next) {
		if(!this->joined) {
			continue;
		}
		for(cu = this->nicks; cu != NULL; cu = cu->next) {
			if(!strcasecmp(cu->nick, nick)) {
				if(cu->userhost == NULL) {
					return 1;
				} else {
					return 0;
				}
			}
		}
	}
	return -1;
}

struct maskstruct *isnickbk(struct channel *this, char *nick)
{
	struct maskstruct *ms;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "isnickbk(\"%s\", \"%s\")\n", this->name, nick);
#endif
	for(ms = this->nickbks; ms != NULL; ms = ms->next) {
#ifdef OER_DEBUG
                oer_debug(OER_DEBUG_NOISE, "isnickbk->matching %s to %s\n", ms->mask, nick);
#endif
		if(wild_match(ms->mask, nick)) {
			return ms;
		}
	}
	return NULL;
}

struct maskstruct *iswordbk(struct channel *this, char *message)
{
	int ppos;
	int nppos;
	struct maskstruct *ms;
	char outstring[WRITE_BUFFER_LENGTH + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "iswordbk(\"%s\", \"%s\")\n", this->name, message);
#endif
	ppos = 0;
	nppos = 0;
	while(1) {
		nppos = parse(message, ppos, " ", outstring, WRITE_BUFFER_LENGTH, 0);
		if(nppos == ppos || nppos < 0) {
			break;
		}
		ppos = nppos;
		for(ms = this->wordbks; ms != NULL; ms = ms->next) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_NOISE, "iswordbk->matching %s to %s\n", ms->mask, outstring);
#endif
			/* test word for pattern or exact match */
			if(wild_match(ms->mask, outstring)) {
				return ms;
			}
		}
	}
	return NULL;
}

int isflood(struct channel *this, char *nick, char *userhost, char *message)
{
	time_t diff;
        int perint100;
        int allowed100;
        int weight;
        int bonus;
        int penalty;
        int lines;
        int chars;
        int linecount;
        int charcount;
        struct pubmsg *pm;
        struct chanuser *cu;
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_NOISE, "isflood(\"%s\", \"%s\", \"%s\", \"%s\")\n", this->name, nick, userhost, message);
#endif
        /* get nick structure */
        if((cu = getcuptr(this, nick)) == NULL) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_WARNING, "isflood->nick structure not found!\n");
#endif
                return OER_PUBMSG_FLOOD_NONE;
        }
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_INFO, "isflood->before isflood_last %lu isflood_lines %d isflood_chars %d\n", cu->isflood_last, cu->isflood_lines, cu->isflood_chars);
#endif
	if(!cu->isflood_last || !cu->isflood_lines || !cu->isflood_chars) {
                cu->isflood_last = mystate->now;
                cu->isflood_lines = 1;
                cu->isflood_chars = strlen(message);
        } else {
                /* diff is the time in seconds between the previous and this message */
                diff = mystate->now - cu->isflood_last;
                perint100 = (this->floodvars.lines * 100) / this->floodvars.interval;
#ifdef OER_DEBUG
                oer_debug(OER_DEBUG_NOISE, "isflood->diff is %lu\n", diff);
                oer_debug(OER_DEBUG_NOISE, "isflood->perint100 is %d\n", perint100);
#endif
                /* allowed */
                allowed100 = (diff) ? diff * perint100 : 0;
                bonus = (allowed100 >> 1) / 100;
                penalty = (diff) ? 0 : (100 / perint100) >> 2;
#ifdef OER_DEBUG
                oer_debug(OER_DEBUG_NOISE, "isflood->allowed100 is %d\n", allowed100);
                oer_debug(OER_DEBUG_NOISE, "isflood->bonus is %d\n", bonus);
                oer_debug(OER_DEBUG_NOISE, "isflood->penalty is %d\n", penalty);
#endif
                if(diff) {
                        lines = cu->isflood_lines - bonus + 1;
                } else {
                        lines = cu->isflood_lines + penalty + 1;
                }
                chars = lines * (this->floodvars.chars / this->floodvars.lines);
                /* the isflood should never get below these */
                cu->isflood_lines = (lines >= 1) ? lines : 1;
                cu->isflood_chars = (chars >= (this->floodvars.chars / this->floodvars.lines)) ? chars : (this->floodvars.chars / this->floodvars.lines);
                cu->isflood_last = mystate->now;
        }
	/* check repeat and double check lines/chars */
        for(pm = this->pubmsgs, weight = 0, linecount = 1, charcount = strlen(message); pm != NULL; pm = pm->next) {
                if(!strcasecmp(pm->nick, nick) && !strcasecmp(pm->userhost, userhost)) {
                        if(mystate->now > (pm->at + this->floodvars.repeat_expire)) {
                                /* expired, skip */
                                continue;
                        }
                        linecount++;
                        charcount += strlen(pm->message);
                        if(issimilarstring(pm->message, message)) {
                                weight++;
                        }
                }
        }
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_INFO, "isflood->repeat weight is %d linecount is %d charcount is %d\n", weight, linecount, charcount);
#endif
        if(weight >= this->floodvars.repeat_limit - 1) {
                return OER_PUBMSG_FLOOD_REPEAT;
        }
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_NOISE, "isflood->after isflood_last %lu isflood_lines %d isflood_chars %d\n", cu->isflood_last, cu->isflood_lines, cu->isflood_chars);
#endif
        if(cu->isflood_lines > this->floodvars.lines ||
           cu->isflood_chars > this->floodvars.chars ||
           linecount > this->floodvars.lines ||
           charcount > this->floodvars.chars) {
                return OER_PUBMSG_FLOOD_NORMAL;
        }
        return OER_PUBMSG_FLOOD_NONE;
}

int haschanflags(struct channel *this, char *flags)
{
	int i;
	int len;
	int got_flags;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "haschanflags(\"%s\", \"%s\")\n", (this == NULL) ? "(null)" : this->name, flags);
#endif
	if(this == NULL) {
		return 0;
	}
	/* check if the channel has all the required flags set... */
	for(i = 0, got_flags = 0, len = strlen(flags); i < len; i++) {
		if(index(this->chanflags, (int)flags[i]) != NULL) {
			got_flags++;
		}
	}
	if(got_flags == len) {
		return 1;
	}
	return 0;
}

int hasadminflags(char *userhost, char *flags)
{
	int i;
	int len;
	int got_flags;
	struct botuser *admin;
	struct maskstruct *ms;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "hasadminflags(\"%s\", \"%s\")\n", (userhost == NULL) ? "(null)" : userhost, flags);
#endif
	if(userhost == NULL) {
		return 0;
	}
	/* go through all admins */
	for(admin = mystate->admins; admin != NULL; admin = admin->next) {
		/* check if the admin has all the required flags set... */
		for(i = 0, got_flags = 0, len = strlen(flags); i < len; i++) {
			if(index(admin->options, (int)flags[i]) != NULL) {
				got_flags++;
			}
		}
		if(got_flags != len) {
			/* nope, skip to next user */
			continue;
		}
		/* the admin has the right flags, what about hostmask? */
		for(ms = admin->firstmask; ms != NULL; ms = ms->next) {
			if(wild_match(ms->mask, userhost)) {
				return 1;
			}
		}
	}
	return 0;
}

int hasuserflags(struct channel *this, char *userhost, char *flags, char *notflags)
{
	int i;
	int len;
	int got_flags;
	struct botuser *user;
	struct maskstruct *ms;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "hasuserflags(\"%s\", \"%s\", \"%s\", \"%s\")\n", (this == NULL) ? "(null)" : this->name, (userhost == NULL) ? "(null)" : userhost, flags, notflags);
#endif
	if(userhost == NULL) {
		return 0;
	}
	for(user = (this == NULL) ? mystate->admins : this->users; user != NULL; user = user->next) {
		if(index(user->options, (int)'!') != NULL) {
			/* user is inactive */
			continue;
		}
		/* check if the user has all the required flags set... */
		for(i = 0, got_flags = 0, len = strlen(flags); i < len; i++) {
			if(index(user->options, (int)flags[i]) != NULL) {
				got_flags++;
			}
		}
#ifdef OER_DEBUG
                oer_debug(OER_DEBUG_NOISE, "hasuserflags->got_flags is %d and len is %d\n", got_flags, len);
#endif
		if(got_flags != len) {
			continue;
		}
		/* check that the user hasn't got any of the non-allowed flags set */
		for(i = 0, got_flags = 0, len = strlen(notflags); i < len; i++) {
			if(index(user->options, (int)notflags[i]) != NULL) {
				got_flags++;
			}
		}
#ifdef OER_DEBUG
                oer_debug(OER_DEBUG_NOISE, "hasuserflags->got_flags is %d and len is %d\n", got_flags, len);
#endif
		if(got_flags) {
			continue;
		}
		/* the user has the right flags, what about hostmask? */
		for(ms = user->firstmask; ms != NULL; ms = ms->next) {
			if(wild_match(ms->mask, userhost)) {
				return 1;
			}
		}
        }
	return 0;
}

void isjoined(struct channel *this)
{
	char stringbuffer[STRINGLEN + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "isjoined(\"%s\")\n", this->name);
#endif
	this->join_ts = mystate->now;
	this->joined = 1;
	this->joining = 0;
	/* make a WHO query to get all userhosts & flags for
	   nicks on channel */
	snprintf(timed_str, WRITE_BUFFER_LENGTH, "WHO %s", this->name);
        timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CHANNEL_HANDLING, timed_str);
	if(haschanflags(this, "L") && this->last == NULL) {
		if(strlen(mystate->altloggingdir)) {
			snprintf(stringbuffer, STRINGLEN, "%s/last-%s", mystate->altloggingdir, this->name);
		} else {
			snprintf(stringbuffer, STRINGLEN, "last-%s", this->name);
		}
		if((this->last = fopen(stringbuffer, "a")) == NULL) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_INFO, "isjoined->failed to open %s: %s\n", stringbuffer, strerror(errno));
#endif
		}
	}
	if(haschanflags(this, "S") && this->seen == NULL) {
		if(strlen(mystate->altloggingdir)) {
			snprintf(stringbuffer, STRINGLEN, "%s/seen-%s", mystate->altloggingdir, this->name);
		} else {
			snprintf(stringbuffer, STRINGLEN, "seen-%s", this->name);
		}
		if((this->seen = fopen(stringbuffer, "a")) == NULL) {
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_INFO, "isjoined->failed to open %s: %s\n", stringbuffer, strerror(errno));
#endif
		}
	}
}

void setchankey(struct channel *this, char *key)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "setchankey(\"%s\", \"%s\")\n", this->name, key);
#endif
	strncpy(this->key, key, CHANLEN);
	this->setchanmode = 1;
	this->chankeyisset = 0;
}

void setchanlimit(struct channel *this, char *limit)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "setchanlimit(\"%s\", \"%s\")\n", this->name, limit);
#endif
	strncpy(this->limit, limit, CHANLEN);
	this->setchanmode = 1;
	this->chanlimitisset = 0;
}

int delchankey(struct channel *this)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "delchankey(\"%s\")\n", this->name);
#endif
	if(strlen(this->key)) {
		mmode_new(this, mystate->now, "-k", this->key);
		memset(this->key, 0, CHANLEN + 1);
	}
	return 1;
}

int delchanlimit(struct channel *this)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "delchanlimit(\"%s\")\n", this->name);
#endif
	mmode_new(this, mystate->now, "-l", NULL);
	memset(this->limit, 0, CHANLEN + 1);
	return 1;
}

int delchanmode(struct channel *this)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "delchanmode(\"%s\")\n", this->name);
#endif
	if(!strlen(this->mode)) {
		return 0;
	}
	memset(this->mode, 0, CHANLEN + 1);
	return 1;
}

char *safeban(struct channel *this, char *safemask, char *nick)
{
	int got_ban;
	int counter;
	int type;
	int is_trusted;
	char user[USERHOSTLEN + 1];
	char host[USERHOSTLEN + 1];
	char ban_mask[USERHOSTLEN + 1];
	char my_mask[USERHOSTLEN + 1];
	char *ptr;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "safeban(\"%s\", \"%s\")\n", this->name, nick);
#endif
	/* safemask contains user@host */
	/* strip user & host */
	strncpy(user, safemask, USERHOSTLEN);
	ptr = index(user, '@');
	memset(ptr, 0, USERHOSTLEN - strlen(user) + 1);
	ptr = index(safemask, '@');
	strncpy(host, ptr + 1, USERHOSTLEN);
	snprintf(my_mask, USERHOSTLEN, "%s!%s@%s", mystate->nick, mystate->user, mystate->host);
	type = this->bantype;
	for(counter = 0, got_ban = 0; !got_ban && counter < OER_BAN_TYPE_INVALID; counter++) {
		is_trusted = 0;
		switch(type) {
		case OER_BAN_TYPE_HOST:
			/* no domain bans for trusteds */
			if(istrusted(host)) {
				is_trusted = 1;
			}
			snprintf(ban_mask, USERHOSTLEN, "*!*@%s", host);
			type = OER_BAN_TYPE_USER_HOST;
			break;
		case OER_BAN_TYPE_USER:
			snprintf(ban_mask, USERHOSTLEN, "*!%s@*", user);
			type = OER_BAN_TYPE_USER_HOST;
			break;
		case OER_BAN_TYPE_USER_HOST:
			snprintf(ban_mask, USERHOSTLEN, "*!%s@%s", user, host);
			type = OER_BAN_TYPE_NICK_USER_HOST;
			break;
		case OER_BAN_TYPE_NICK:
			snprintf(ban_mask, USERHOSTLEN, "%s!*@*", nick);
			type = OER_BAN_TYPE_NICK_HOST;
			break;
		case OER_BAN_TYPE_NICK_HOST:
			snprintf(ban_mask, USERHOSTLEN, "%s!*@%s", nick, host);
			type = OER_BAN_TYPE_NICK_USER;
			break;	
		case OER_BAN_TYPE_NICK_USER:
			snprintf(ban_mask, USERHOSTLEN, "%s!%s@*", nick, user);
			type = OER_BAN_TYPE_NICK_USER_HOST;
			break;
		case OER_BAN_TYPE_NICK_USER_HOST:
			snprintf(ban_mask, USERHOSTLEN, "%s!%s@%s", nick, user, host);
			break;
		default:
			return NULL;
		}
		if(is_trusted) {
			continue;
		}
		if(haschanflags(this, "b")) {
                        /* check that ban is safe */
                        if(!wild_match(ban_mask, my_mask)) {
                                got_ban = 1;
                        }
                } else {
                        got_ban = 1;
                }
	}
	if(!got_ban) {
		return NULL;
	}
	strncpy(safemask, ban_mask, USERHOSTLEN);
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "safeban->returning %s\n", safemask);
#endif
	return safemask;
}

int nthmode(char *modeline, int index)
{
	int i;
	int n;
	int length;
	int modetype;
	char polar;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "nthmode(\"%s\", %d)\n", modeline, index);
#endif
	for(i = 0, n = 0, polar = 'x', modetype = OER_NTHMODE_UNKNOWN, length = strlen(modeline); i < length; i++) {
		switch(modeline[i]) {
		case '+':
			polar = '+';
			continue;
			break;
		case '-':
			polar = '-';
			continue;
			break;
		case 'o':
			modetype = (polar == '+') ? OER_NTHMODE_OP : OER_NTHMODE_DOP;
			break;
		case 'v':
			modetype = (polar == '+') ? OER_NTHMODE_VOICE : OER_NTHMODE_DVOICE;
			break;
		case 'b':
			modetype = (polar == '+') ? OER_NTHMODE_BAN : OER_NTHMODE_UNBAN;
			break;
		case 'p':
		case 's':
		case 'i':
		case 't':
		case 'n':
		case 'm':
		case 'c': /* QuakeNet extension */
		case 'C': /* QuakeNet extension */
			modetype = OER_NTHMODE_CHANMODE_WITHOUT_PARAMS;
			break;
		case 'l':
			if(polar == '+') {
				modetype = OER_NTHMODE_CHANMODE_WITH_PARAMS;
			} else if(polar == '-') {
				modetype = OER_NTHMODE_CHANMODE_WITHOUT_PARAMS;
			}
			break;
		case 'k':
			modetype = OER_NTHMODE_CHANMODE_WITH_PARAMS;
			break;
		default:
			modetype = OER_NTHMODE_UNKNOWN;
		}
		if(n == index) {
			return modetype;
		}
		n++;
	}
	return OER_NTHMODE_UNKNOWN;
}

void getnthmode(char *modeline, int index, char *to)
{
	int i;
	int n;
	int length;
	char polar;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "getnthmode(\"%s\", %d)\n", modeline, index);
#endif
	memset(to, 0, CHANLEN + 1);
	length = strlen(modeline);
	for(i = 0, n = 0, polar = 'x'; i < length; i++) {
		switch(modeline[i]) {
		case '+':
		case '-':
			polar = modeline[i];
			break;
                default:
			n++;
			if(n == index) {
				snprintf(to, CHANLEN, "%c%c", polar, modeline[i]);
				return;
			}
			break;
		}
	}
}

struct ls_wrapstruct *addnew_ls(struct ls_wrapstruct *first, int show_amount, time_t at, char *nick, char *userhost, char *message)
{
	int amount;
	struct ls_wrapstruct *ls;
	struct ls_wrapstruct *ls2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "addnew_ls(\"%s\", %d, %lu, \"%s\", \"%s\", \"%s\")\n", (first == NULL) ? "(null)" : first->str3, show_amount, at, nick, userhost, message);
#endif
	ls = first;
	ls2 = ls;
	amount = 0;
	while(ls != NULL) {
		ls2 = ls;
		amount++;
		ls = ls2->next;
	}
	/* add a new entry to the list */
	if((ls = (struct ls_wrapstruct *) malloc(sizeof(struct ls_wrapstruct))) == NULL) {
		return NULL;
	}
	if((ls->str1 = (char *) malloc(strlen(nick) + 1)) == NULL) {
		return NULL;
	}
	strcpy(ls->str1, nick);
	if((ls->str2 = (char *) malloc(strlen(userhost) + 1)) == NULL) {
		return NULL;
	}
	strcpy(ls->str2, userhost);
	if(message != NULL) {
		if((ls->str3 = (char *) malloc(strlen(message) + 1)) == NULL) {
			return NULL;
		}
		strcpy(ls->str3, message);
	} else {
		ls->str3 = NULL;
	}
	ls->at = at;
	ls->prev = NULL;
	ls->next = NULL;
	/* the first one is a special case */
	if(first == NULL) {
		return ls;
	}
	/* >= 1 ls */
	ls->prev = ls2;
	ls2->next = ls;
	if(amount >= show_amount) {
		/* strip first one */
		first->next->prev = NULL;
		ls2 = first->next;
		free(first->str1);
		free(first->str2);
		free(first->str3);
		free(first);
		first = ls2;
	}
	return first;
}

struct pubmsg *addnewpubmsg(struct channel *this, time_t at, char *nick, char *userhost, char *message)
{
	int amount;
	int allowed;
	struct pubmsg *pm;
	struct pubmsg *pm2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "addnewpubmsg(\"%s\", %lu, \"%s\", \"%s\", \"%s\")\n", this->name, at, nick, userhost, message);
#endif
	/* add a new entry to the list */
	if((pm = (struct pubmsg *) malloc(sizeof(struct pubmsg))) == NULL) {
		return NULL;
	}
	if((pm->nick = (char *) malloc(strlen(nick) + 1)) == NULL) {
		return NULL;
	}
	strcpy(pm->nick, nick);
	if((pm->userhost = (char *) malloc(strlen(userhost) + 1)) == NULL) {
		return NULL;
	}
	strcpy(pm->userhost, userhost);
	if((pm->message = (char *) malloc(strlen(message) + 1)) == NULL) {
		return NULL;
	}
	strcpy(pm->message, message);
	pm->at = at;
	pm->prev = NULL;
	pm->next = NULL;
	if(this->pubmsgs == NULL) {
		/* the first one is a special case */
		this->pubmsgs = pm;
	} else {
		/* we got at least 1 pubmsg in the list */
		this->pubmsgs->prev = pm;
		pm->next = this->pubmsgs;
		this->pubmsgs = pm;
	}
	/* there is always at least 1 pubmsg */
	for(pm2 = this->pubmsgs, amount = 1; pm2->next != NULL; pm2 = pm2->next, amount++)
		;
	if(this->floodvars.lines > (PUBMSG_FLOOD_LINES * 10)) {
		/* maximum storage for public messages */
		allowed = PUBMSG_FLOOD_LINES * 10;
	} else {
		/* minimum is PUBMSG_FLOOD_LINES */
		allowed = (this->floodvars.lines > PUBMSG_FLOOD_LINES) ? this->floodvars.lines : PUBMSG_FLOOD_LINES;
	}
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "addnewpubmsg->there are %d pubmsgs (max. is %d)\n", amount, allowed);
#endif
	if(amount > allowed) {
		/* list full, strip last one */
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_NOISE, "addnewpubmsg->stripping last pubmsg\n");
#endif
		pm2->prev->next = NULL;
		free(pm2->message);
		free(pm2->userhost);
		free(pm2->nick);
		free(pm2);
	}
	return pm;
}

struct nickchange *addnewnickchange(struct channel *this, char *userhost)
{
	int amount;
	struct nickchange *nc1;
	struct nickchange *nc2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "addnewnickchange(\"%s\", \"%s\")\n", this->name, userhost);
#endif
	/* add a new entry to the list */
	if((nc1 = (struct nickchange *) malloc(sizeof(struct nickchange))) == NULL) {
		return NULL;
	}
	if((nc1->userhost = (char *) malloc(strlen(userhost) + 1)) == NULL) {
		return NULL;
	}
	strcpy(nc1->userhost, userhost);
	nc1->at = mystate->now;
	nc1->prev = NULL;
	nc1->next = NULL;
	/* the first one is a special case */
	if(this->nickchanges == NULL) {
		this->nickchanges = nc1;
		return nc1;
	}
	/* we got at least 1 nickchange in the list */
	this->nickchanges->prev = nc1;
	nc1->next = this->nickchanges;
	this->nickchanges = nc1;
	for(nc2 = this->nickchanges, amount = 0; nc2->next != NULL; nc2 = nc2->next, amount++) {
	}
	if(amount >= OER_NICKCHANGES) {
		/* list full, strip last one */
		nc2->prev->next = NULL;
		free(nc2->userhost);
		free(nc2);
	}
	return nc1;
}

struct part *addnewpart(struct channel *this, time_t at, char *nick, char *userhost)
{
	int amount;
	struct part *p;
	struct part *p2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "addnewpart(\"%s\", %lu, \"%s\", \"%s\")\n", this->name, at, nick, userhost);
#endif
	if(userhost == NULL) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "addnewpart->hostmask = NULL, not adding part for user\n");
#endif
		return NULL;
	}
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "addnewpart->adding part from %s at %lu for %s!%s\n", this->name, at, nick, userhost);
#endif
	/* add a new entry to the list */
	if((p = (struct part *) malloc(sizeof(struct part))) == NULL) {
		return NULL;
	}
	if((p->nick = (char *) malloc(strlen(nick) + 1)) == NULL) {
		return NULL;
	}
	strcpy(p->nick, nick);
	if((p->userhost = (char *) malloc(strlen(userhost) + 1)) == NULL) {
		return NULL;
	}
	strcpy(p->userhost, userhost);
	p->at = at;
	p->valid = 1;
	p->prev = NULL;
	p->next = NULL;
	/* the first one is a special case */
	if(this->parts == NULL) {
		this->parts = p;
		return p;
	}
	/* we got at least 1 part in the list */
	this->parts->prev = p;
	p->next = this->parts;
	this->parts = p;
	for(p2 = this->parts, amount = 0; p2->next != NULL; p2 = p2->next, amount++) {
	}
	if(amount >= OER_PARTS) {
		/* list full, strip last one */
		p2->prev->next = NULL;
		free(p2->nick);
		free(p2->userhost);
		free(p2);
	}
	return p;
}

struct join *addnewjoin(struct channel *this, time_t at, char *nick, char *userhost)
{
	int amount;
	struct join *j;
	struct join *j2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "addnewjoin(\"%s\", %lu, \"%s\", \"%s\")\n", this->name, at, nick, userhost);
#endif
	/* add a new entry to the list */
	if((j = (struct join *) malloc(sizeof(struct join))) == NULL) {
		return NULL;
	}
	if((j->nick = (char *) malloc(strlen(nick) + 1)) == NULL) {
		return NULL;
	}
	strcpy(j->nick, nick);
	if((j->userhost = (char *) malloc(strlen(userhost) + 1)) == NULL) {
		return NULL;
	}
	strcpy(j->userhost, userhost);
	j->at = at;
	j->prev = NULL;
	j->next = NULL;
	/* the first one is a special case */
	if(this->joins == NULL) {
		this->joins = j;
		return j;
	}
	/* we got at least 1 join in the list */
	this->joins->prev = j;
	j->next = this->joins;
	this->joins = j;
	for(j2 = this->joins, amount = 0; j2->next != NULL; j2 = j2->next, amount++) {
	}
	if(amount >= OER_JOINS) {
		/* list full, strip last one */
		j2->prev->next = NULL;
		free(j2->nick);
		free(j2->userhost);
		free(j2);
	}
	return j;
}

struct chanuser *getcuptr(struct channel *this, char *nick)
{
        struct chanuser *cu;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "getcuptr(\"%s\", \"%s\")\n", this->name, nick);
#endif
	for(cu = this->nicks; cu != NULL; cu = cu->next) {
                if(!strcmp(cu->nick, nick)) {
			return cu;
                }
        }
	return NULL;
}

struct chanuser *userjoined(struct channel *this, char *nick, char *userhost, int ircop, int chanop, int voice, int friend)
{
	struct chanuser *cu;
	struct chanuser *cu2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "userjoined(\"%s\", \"%s\", \"%s\", %d, %d, %d, %d)\n", this->name, nick, (userhost == NULL) ? "(null)" : userhost, ircop, chanop, voice, friend);
#endif
	if((cu = (struct chanuser *) malloc(sizeof(struct chanuser))) == NULL) {
		return NULL;
	}
	cu->hostquery = 0;
	cu->ircop = ircop;
	cu->chanop = chanop;
	cu->voice = voice;
	cu->tobek = 0;
	cu->friend = friend;
	cu->isflood_last = 0;
	cu->isflood_lines = 0;
	cu->isflood_chars = 0;
	cu->prev = NULL;
	cu->next = NULL;
	if((cu->nick = (char *) malloc(strlen(nick) + 1)) == NULL) {
		return NULL;
	}
	strcpy(cu->nick, nick);
	cu->userhost = NULL;
	if(userhost != NULL) {
		if((cu->userhost = (char *) malloc(strlen(userhost) + 1)) == NULL) {
			return NULL;
		}
		strcpy(cu->userhost, userhost);
	}
	/* if user is logoned and has "f" flag */
	if(haschanflags(this, "F") && isfriend(this, nick, userhost)) {
		cu->friend = 1;
	}
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "userjoined->added join to %s from %s!%s (%d %d %d %d)\n", this->name, cu->nick, (cu->userhost == NULL) ? "(null)" : cu->userhost, cu->ircop, cu->chanop, cu->voice, cu->friend);
#endif
	/* as always, the empty case is special */
	if(this->nicks == NULL) {
		this->nicks = cu;
		this->nickcount++;
		return cu;
	}
	/* there is no need to check for existance, append */
	for(cu2 = this->nicks; cu2->next != NULL; cu2 = cu2->next) {
	}
	cu2->next = cu;
	cu->prev = cu2;
	this->nickcount++;
	return cu;
}

int userleft(struct channel *this, char *nick, char *userhost)
{
	struct chanuser *cu;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "userleft(\"%s\", \"%s\", \"%s\")\n", this->name, nick, userhost);
#endif
	for(cu = this->nicks; cu != NULL; cu = cu->next) {
		if(!strcasecmp(cu->nick, nick)) {
			break;
		}
	}
	if(cu == NULL) {
		/* a ghost user left?!?! */
		return 0;
	}
	if(cu->prev == NULL) {
                /* first */
                if(cu->next == NULL) {
                        /* only */
                        this->nicks = NULL;
                } else {
                        cu->next->prev = NULL;
                        this->nicks = cu->next;
                }
        } else if(cu->next == NULL) {
                /* last */
                cu->prev->next = NULL;
        } else {
                /* between 2 or more */
                cu->prev->next = cu->next;
                cu->next->prev = cu->prev;
        }
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "userleft->removed from %s %s!%s\n", this->name, cu->nick, (cu->userhost == NULL) ? "(null)" : cu->userhost);
#endif
	free(cu->userhost);
	free(cu->nick);
	free(cu);
	this->nickcount--;
	return 1;
}

int changeuser(struct channel *this, char *nick, int ircop, int chanop, int voice, int friend)
{
	struct chanuser *cu;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "changeuser(\"%s\", \"%s\", %d, %d, %d, %d)\n", this->name, nick, ircop, chanop, voice, friend);
#endif
	for(cu = this->nicks; cu != NULL; cu = cu->next) {
		if(!strcasecmp(cu->nick, nick)) {
			break;
		}
	}
	if(cu == NULL) {
		return 0;
	}
	cu->ircop = (ircop == -1) ? cu->ircop : ircop;
	cu->chanop = (chanop == -1) ? cu->chanop : chanop;
	cu->voice = (voice == -1) ? cu->voice : voice;
	cu->friend = (friend == -1) ? cu->friend : friend;
	return 1;
}

void changetobek(struct channel *this, char *nick, int tobek)
{
	struct chanuser *cu;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "changetobek(\"%s\", \"%s\", %d)\n", this->name, nick, tobek);
#endif
	for(cu = this->nicks; cu != NULL; cu = cu->next) {
		if(!strcasecmp(cu->nick, nick)) {
			cu->tobek = tobek;
			return;
		}
	}
}

int gettobek(struct channel *this, char *nick)
{
	struct chanuser *cu;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "gettobek(\"%s\", \"%s\")\n", this->name, nick);
#endif
	for(cu = this->nicks; cu != NULL; cu = cu->next) {
		if(!strcasecmp(cu->nick, nick)) {
			return cu->tobek;
		}
	}
	return 0;
}

void freepubmsguser(struct channel *this, char *nick, char *userhost)
{
	struct pubmsg *pm;
	struct pubmsg *pm2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "freepubmsguser(\"%s\", \"%s\", \"%s\")\n", this->name, nick, userhost);
#endif
	pm = this->pubmsgs;
	while(pm != NULL) {
		pm2 = pm->next;
		if(!strcasecmp(pm->nick, nick) && !strcasecmp(pm->userhost, userhost)) {
			if(pm->prev == NULL) {
				/* first */
				if(pm->next == NULL) {
					/* only */
					this->pubmsgs = NULL;
				} else {
					pm->next->prev = NULL;
					this->pubmsgs = pm->next;
				}
			} else if(pm->next == NULL) {
				/* last */
				pm->prev->next = NULL;
			} else {
				/* between 2 or more */
				pm->prev->next = pm->next;
				pm->next->prev = pm->prev;
			}
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_INFO, "freepubmsguser->removing: %lu %s %s %s\n", pm->at, pm->nick, pm->userhost, pm->message);
#endif
			free(pm->message);
			free(pm->nick);
			free(pm->userhost);
			free(pm);
		}
		pm = pm2;
	}
}

void freenickchanges(struct channel *this, char *userhost)
{
	struct nickchange *nc1;
	struct nickchange *nc2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "freenickchanges(\"%s\", \"%s\")\n", this->name, userhost);
#endif
	nc1 = this->nickchanges;
	while(nc1 != NULL) {
		nc2 = nc1->next;
		if(!strcasecmp(nc1->userhost, userhost)) {
			if(nc1->prev == NULL) {
				/* first */
				if(nc1->next == NULL) {
					/* only */
					this->nickchanges = NULL;
				} else {
					nc1->next->prev = NULL;
					this->nickchanges = nc1->next;
				}
			} else if(nc1->next == NULL) {
				/* last */
				nc1->prev->next = NULL;
			} else {
				/* between 2 or more */
				nc1->prev->next = nc1->next;
				nc1->next->prev = nc1->prev;
			}
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_INFO, "freenickchanges->removing: %lu %s\n", nc1->at, nc1->userhost);
#endif
			free(nc1->userhost);
			free(nc1);
		}
		nc1 = nc2;
	}
}

void lockchan(struct channel *this, char *reason, int auto_unlock, char *nick, char *userhost)
{
	char stringbuffer[STRINGLEN + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "lockchan(\"%s\", \"%s\", %d, \"%s\", \"%s\")\n", this->name, reason, auto_unlock, nick, userhost);
#endif
	this->locked.locked = 1;
	this->locked.unlocked = 0;
	this->locked.auto_unlock = auto_unlock;
	memset(this->locked.reason, 0, STRINGLEN + 1);
	if(strlen(reason)) {
		strncpy(this->locked.reason, reason, STRINGLEN);
	}
	strncpy(this->locked.nick, nick, NICKLEN);
	strncpy(this->locked.host, userhost, USERHOSTLEN);
	mmode_new(this, mystate->now, "+i", NULL);
	/* make sure +i will come before the first kick */
	mmodes2timeds();
	snprintf(stringbuffer, STRINGLEN, "channel %s is locked by %s!%s", this->name, this->locked.nick, this->locked.host);
        snprintf(timed_str, WRITE_BUFFER_LENGTH, "NOTICE %s :%s", this->name, stringbuffer);
        timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CHANNEL_PROTECTION, timed_str);
}

void unlockchan(struct channel *this, char *nick, char *userhost)
{
	char stringbuffer[STRINGLEN + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "unlockchan(\"%s\", \"%s\", \"%s\")\n", this->name, nick, userhost);
#endif
	this->locked.unlocked = 1;
	/* this has to be a timed because of the way mmodes & timeds
	   are processed in the mainloop (we want the -i to appear
           after all normal users have been kicked out, not before) */
	mmode_new(this, mystate->now, "-i", NULL);
	snprintf(stringbuffer, STRINGLEN, "channel %s was unlocked by %s!%s", this->name, nick, userhost);
        snprintf(timed_str, WRITE_BUFFER_LENGTH, "NOTICE %s :%s", this->name, stringbuffer);
        timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CHANNEL_PROTECTION, timed_str);
	cleartobeks(this);
}

void wall(struct channel *this, char *nick, char *userhost, char *commandline)
{
	int ppos;
        char p1[STRINGLEN + 1];
	char stringbuffer[HUGESTRINGLEN + 1];
	struct chanuser *cu;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "wall(\"%s\", \"%s\", \"%s\", \"%s\")\n", this->name, nick, userhost, commandline);
#endif
	ppos = 0;
        if((ppos = parse(commandline, ppos, " ", p1, STRINGLEN, 0)) < 0) {
                return;
        }
	if(!strlen(p1) || !strlen(commandline + ppos)) {
		return;
	}
	if(!strcasecmp(p1, "admins")) {
		if(!isadmin(this, userhost)) {
			return;
		}
		for(cu = this->nicks; cu != NULL; cu = cu->next) {
			if(!isme(cu->nick) && isadmin(this, cu->userhost)) {
				snprintf(stringbuffer, HUGESTRINGLEN, "notice to %s admins from %s: %s", this->name, nick, commandline + ppos);
				sendwall(cu->nick, this, 0, stringbuffer);
			}
		}
		return;
	}
	if(!strcasecmp(p1, "channel")) {
		snprintf(stringbuffer, HUGESTRINGLEN, "notice to %s from %s: %s", this->name, nick, commandline + ppos);
		sendchannelnotice(this, 0, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "normals")) {
		for(cu = this->nicks; cu != NULL; cu = cu->next) {
			if(!isme(cu->nick) && !cu->chanop) {
				snprintf(stringbuffer, HUGESTRINGLEN, "notice to non-opped %s users from %s: %s", this->name, nick, commandline + ppos);
				sendwall(cu->nick, this, 0, stringbuffer);
			}
		}
		return;
	}
	if(!strcasecmp(p1, "ops")) {
		for(cu = this->nicks; cu != NULL; cu = cu->next) {
			if(!isme(cu->nick) && cu->chanop) {
				snprintf(stringbuffer, HUGESTRINGLEN, "notice to %s channel operators from %s: %s", this->name, nick, commandline + ppos);
				sendwall(cu->nick, this, 0, stringbuffer);
			}
		}
		return;
	}
	if(!strcasecmp(p1, "users")) {
		for(cu = this->nicks; cu != NULL; cu = cu->next) {
			if(!isme(cu->nick) && (isadmin(this, cu->userhost) || isop(this, cu->userhost))) {
				snprintf(stringbuffer, HUGESTRINGLEN, "notice to %s bot users from %s: %s", this->name, nick, commandline + ppos);
				sendwall(cu->nick, this, 0, stringbuffer);
			}
		}
		return;
	}
}

struct authed *addnewauthed(struct channel *this, time_t at, char *handle, char *userhost)
{
	struct authed *a;
	struct authed *a2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "addnewauthed(\"%s\", %lu, \"%s\", \"%s\")\n", (this == NULL) ? "(null)" : this->name, at, handle, userhost);
#endif
	/* first check that there is no active authed for the same handle & mask */
	for(a = (this == NULL) ? mystate->autheds : this->autheds; a != NULL; a = a->next) {
		if(haschanflags(this, "x") && !strcasecmp(a->handle, handle) && !strcasecmp(a->userhost, userhost)) {
			return NULL;
		} else if((mystate->now < (a->at + OER_LOGON_TIMEOUT)) && !strcasecmp(a->handle, handle) && !strcasecmp(a->userhost, userhost)) {
			return NULL;
		}
	}
	/* add a new entry to the list */
	if((a = (struct authed *) malloc(sizeof(struct authed))) == NULL) {
		return NULL;
	}
	a->at = at;
	a->forced_logoff = 0;
	if((a->userhost = (char *) malloc(strlen(userhost) + 1)) == NULL) {
		return NULL;
	}
	strcpy(a->userhost, userhost);
	if((a->handle = (char *) malloc(strlen(handle) + 1)) == NULL) {
		return NULL;
	}
	strcpy(a->handle, handle);
	a->prev = NULL;
	a->next = NULL;
	if(this == NULL) {
		/* admin authed, the first one is a special case */
		if(mystate->autheds == NULL) {
			mystate->autheds = a;
			return a;
		}
		/* we got at least 1 authed in the list, append */
		for(a2 = mystate->autheds; a2->next != NULL; a2 = a2->next) {
			;
		}
	} else {
		/* user authed, the first one is a special case */
		if(this->autheds == NULL) {
			this->autheds = a;
			return a;
		}
		/* we got at least 1 authed in the list, append */
		for(a2 = this->autheds; a2->next != NULL; a2 = a2->next) {
			;
		}
	}
	a2->next = a;
	a->prev = a2;
	return a;
}

int cleanautheds(struct channel *this, int force)
{
	int deleted;
	struct authed *a;
	struct authed *a2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "cleanautheds(\"%s\", %d)\n", (this == NULL) ? "(null)" : this->name, force);
#endif
	deleted = 0;
	if(this == NULL) {
		/* scan global autheds */
                for(a = mystate->autheds; a != NULL; a = a2) {
                        a2 = a->next;
                        if(!force) {
                                if(!a->forced_logoff && (mystate->now < (a->at + OER_LOGON_TIMEOUT))) {
                                        continue;
                                }
                        }
			if(a->prev == NULL) {
				/* first */
				if(a->next == NULL) {
					/* only */
					mystate->autheds = NULL;
				} else {
					a->next->prev = NULL;
					mystate->autheds = a->next;
				}
			} else if(a->next == NULL) {
				/* last */
				a->prev->next = NULL;
			} else {
				/* between 2 or more */
				a->prev->next = a->next;
				a->next->prev = a->prev;
			}
                        free(a->userhost);
                        free(a->handle);
                        free(a);
			deleted++;
                }
        } else {
		/* scan channel autheds */
                for(a = this->autheds; a != NULL; a = a2) {
                        a2 = a->next;
                        if(!force) {
                                if(!a->forced_logoff && (haschanflags(this, "x") || (mystate->now < (a->at + OER_LOGON_TIMEOUT)))) {
                                        continue;
                                }
                        }
			if(a->prev == NULL) {
				/* first */
				if(a->next == NULL) {
					/* only */
					this->autheds = NULL;
				} else {
					a->next->prev = NULL;
					this->autheds = a->next;
				}
			} else if(a->next == NULL) {
				/* last */
				a->prev->next = NULL;
			} else {
				/* between 2 or more */
				a->prev->next = a->next;
				a->next->prev = a->prev;
			}
                        free(a->userhost);
                        free(a->handle);
                        free(a);
			deleted++;
                }
	}
	return deleted;
}

struct timed *timed_new(struct channel *this, time_t at, int type, int prio, char *command)
{
	int rc;
	struct timed *t1;
	struct timed *t2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "timed_new(\"%s\", %lu, %d, %d, \"%s\")\n", (this == NULL) ? "(null)" : this->name, at, type, prio, command);
#endif
	if((t1 = malloc(sizeof(struct timed))) == NULL) {
		return NULL;
	}
	/* channel can be NULL */
	t1->channel = NULL;
	if(this != NULL) {
		if((t1->channel = malloc(strlen(this->name) + 1)) == NULL) {
			return NULL;
		}
		strcpy(t1->channel, this->name);
	}
	if((t1->command = malloc(strlen(command) + 1)) == NULL) {
		return NULL;
	}
	strcpy(t1->command, command);
	t1->at = at;
	t1->type = type;
	t1->prio = prio;
	t1->prev = NULL;
	t1->next = NULL;
	if(mystate->timeds == NULL) {
		/* empty list */
		mystate->timeds = t1;
		return t1;
	}
	t2 = mystate->timeds;
	while(1) {
		rc = timed_cmp(t1, t2);
		if(rc == 1) {
			break;
		}
		if(t2->next == NULL) {
			break;
		}
		t2 = t2->next;
	}
	if(t2->prev == NULL) {
		/* first timed in list */
		if(rc == 1) {
			t2->prev = t1;
			t1->next = t2;
			mystate->timeds = t1;
			return t1;
		}
		t2->next = t1;
		t1->prev = t2;
		return t1;
	}
	if(t2->next == NULL) {
		/* last timed in list */
		if(rc == 1) {
			t1->next = t2;
			t1->prev = t2->prev;
			t2->prev->next = t1;
			t2->prev = t1;
			return t1;
		}
		t2->next = t1;
		t1->prev = t2;
		return t1;
	}
	/* between 2 or more timeds */
	if(rc == 1) {
		t1->next = t2;
		t1->prev = t2->prev;
		t2->prev->next = t1;
		t2->prev = t1;
		return t1;
	}
	t1->next = t2->next;
	t1->prev = t2;
	t2->next->prev = t1;
	t2->next = t1;
	return t1;
}

void timed_del(struct timed *t1)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "timed_del(%x)\n", t1);
#endif
	if(t1->prev == NULL) {
		/* first */
		if(t1->next == NULL) {
			/* only one */
			mystate->timeds = NULL;
		} else {
			t1->next->prev = NULL;
			mystate->timeds = t1->next;
		}
	} else if(t1->next == NULL) {
		/* last */
		t1->prev->next = NULL;
	} else {
		/* between 2 or more */
		t1->prev->next = t1->next;
		t1->next->prev = t1->prev;
	}
	free(t1->channel);
	free(t1->command);
	free(t1);
	return;
}

int timed_cmp(struct timed *t1, struct timed *t2)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "timed_cmp(%x, %x)\n", t1, t2);
#endif
        /* sort timeds based on priority, then time */
        if(t1->prio == t2->prio) {
                /* same priority */
                if(t1->at == t2->at) {
                        /* same time */
                        return 0;
                }
                if(t1->at > t2->at) {
                        /* t1 has bigger time */
                        return -1;
                }
                return 1;
        }
        if(t1->prio > t2->prio) {
                /* t1 has higher priority */
                return 1;
        }
        /* t1 has smaller priority */
        return -1;
}

struct mmode *mmode_new(struct channel *this, time_t at, char *command, char *target)
{
	int rc;
	struct mmode *m1;
	struct mmode *m2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "mmode_new(\"%s\", %lu, \"%s\", \"%s\")\n", this->name, at, command, (target == NULL) ? "(null)" : target);
	for(m1 = this->mmodes; m1 != NULL; m1 = m1->next) {
		oer_debug(OER_DEBUG_NOISE, "mmode_new->found mmode command %s at %lu target %s\n", m1->command, m1->at, m1->target);
	}
#endif
	if((m1 = malloc(sizeof(struct mmode))) == NULL) {
		return NULL;
	}
	if((m1->command = malloc(strlen(command) + 1)) == NULL) {
		return NULL;
	}
	strcpy(m1->command, command);
	/* target can be NULL, because of chanmodes for ex. */
	m1->target = NULL;
	if(target) {
		if((m1->target = malloc(strlen(target) + 1)) == NULL) {
			return NULL;
		}
		strcpy(m1->target, target);
	}
	m1->at = at;
	m1->prev = NULL;
	m1->next = NULL;
	if(this->mmodes == NULL) {
		/* empty list */
		this->mmodes = m1;
		return m1;
	}
	m2 = this->mmodes;
        while(1) {
		if(m1->at > m2->at) {
			rc = -1;
		} else if(m1->at < m2->at) {
			rc = 1;
		} else {
			rc = 0;
		}
                if(rc == 1) {
                        break;
                }
                if(m2->next == NULL) {
                        break;
                }
                m2 = m2->next;
        }
        if(m2->prev == NULL) {
                /* first mmode in list */
                if(rc == 1) {
                        m2->prev = m1;
                        m1->next = m2;
                        this->mmodes = m1;
                        return m1;
                }
                m2->next = m1;
                m1->prev = m2;
                return m1;
        }
	if(m2->next == NULL) {
                /* last mmode in list */
                if(rc == 1) {
                        m1->next = m2;
			m1->prev = m2->prev;
                        m2->prev->next = m1;
                        m2->prev = m1;
                        return m1;
                }
                m2->next = m1;
                m1->prev = m2;
                return m1;
        }
        /* between 2 or more mmodes */
        if(rc == 1) {
                m1->next = m2;
		m1->prev = m2->prev;
                m2->prev->next = m1;
                m2->prev = m1;
                return m1;
        }
	m1->next = m2->next;
        m1->prev = m2;
        m2->next->prev = m1;
        m2->next = m1;
        return m1;
}

void mmode_del(struct channel *this, struct mmode *m1)
{
#ifdef OER_DEBUG
	struct mmode *m;
#endif
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "mmode_del(\"%s\", \"%s\", \"%s\")\n", this->name, m1->command, m1->target);
	for(m = this->mmodes; m != NULL; m = m->next) {
		oer_debug(OER_DEBUG_NOISE, "mmode_del->found mmode command %s target %s\n", m->command, m->target);
	}
#endif
	if(m1->prev == NULL) {
		/* first */
		if(m1->next == NULL) {
			/* only one */
			this->mmodes = NULL;
		} else {
			m1->next->prev = NULL;
			this->mmodes = m1->next;
		}
	} else if(m1->next == NULL) {
		/* last mmode */
		m1->prev->next = NULL;
	} else {
		/* between 2 or more */
		m1->prev->next = m1->next;
		m1->next->prev = m1->prev;
	}
	free(m1->command);
	free(m1->target);
	free(m1);
	return;
}

void mmodes2timeds(void)
{
	int modes;
	time_t earliest;
	struct mmode *m1;
	struct mmode *m2;
	char command[CHANLEN + 1];
        char target[WRITE_BUFFER_LENGTH + 1];
	struct channel *this;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "mmodes2timeds()\n");
#endif
	for(this = mystate->channels; this != NULL; this = this->next) {
		if(this->mmodes == NULL) {
			continue;
		}
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_NOISE, "mmodes2timeds->channel is %s\n", this->name);
		for(m1 = this->mmodes; m1 != NULL; m1 = m1->next) {
			oer_debug(OER_DEBUG_NOISE, "mmodes2timeds->found mmode channel %s command %s target %s\n", this->name, m1->command, m1->target);
		}
#endif
		memset(command, 0, CHANLEN + 1);
		memset(target, 0, WRITE_BUFFER_LENGTH + 1);
		modes = 0;
		m1 = this->mmodes;
		earliest = mystate->now;
		while(m1 != NULL) {
			m2 = m1->next;
#ifdef OER_DEBUG
			oer_debug(OER_DEBUG_NOISE, "mmodes2timeds->found mode %s target %s\n", m1->command, (m1->target == NULL) ? "(null)" : m1->target);
#endif
			if(m1->at <= mystate->now) {
				earliest = (m1->at < earliest) ? m1->at : earliest;
				strncat(command, m1->command, CHANLEN - strlen(command));
				if(m1->target) {
					strncat(target, m1->target, WRITE_BUFFER_LENGTH - strlen(target));
					strncat(target, " ", WRITE_BUFFER_LENGTH - strlen(target));
				}
				modes++;
				mmode_del(this, m1);
			}
			m1 = m2;
			if(modes >= mystate->current_server->servermodes) {
				snprintf(timed_str, WRITE_BUFFER_LENGTH, "MODE %s %s %s", this->name, command, target);
				timed_new(NULL, earliest, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CHANNEL_PROTECTION, timed_str);
				memset(command, 0, CHANLEN + 1);
				memset(target, 0, WRITE_BUFFER_LENGTH + 1);
				modes = 0;
			}
		}
		if(modes) {
			snprintf(timed_str, WRITE_BUFFER_LENGTH, "MODE %s %s %s", this->name, command, target);
			timed_new(NULL, earliest, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CHANNEL_PROTECTION, timed_str);
		}
	}
}

int clonechannel(char *ref, char *commandline)
{
	int ppos;
	char new[STRINGLEN + 1];
	struct channel *that;
	struct channel *this;
	struct maskstruct *ms;
	struct botuser *user;
	struct topic *topic;
	struct advert *advert;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "clonechannel(\"%s\", \"%s\")\n", ref, commandline);
#endif
	ppos = 0;
        if((ppos = parse(commandline, ppos, " ", new, STRINGLEN, 0)) < 0) {
                return 0;
        }
	if(!strcasecmp(ref, new)) {
		return 0;
	}
	/* does the new channel already exist? */
        if(getchptr(new) != NULL) {
                return 0;
        }
	if((that = getchptr(ref)) == NULL) {
                return 0;
        }
	if(!isvalidchannel(new)) {
		return 0;
	}
	if((this = addnewchannel(new)) == NULL) {
		return 0;
	}
	/* copy data from reference */
	strcpy(this->mode, that->mode);
	this->setchanmode = 1;
	this->chankeyisset = 0;
	this->chanlimitisset = 0;
	if(strlen(that->key)) {
		strcpy(this->key, that->key);
	}
	strcpy(this->chanflags, that->chanflags);
	if(strlen(that->limit)) {
		strcpy(this->limit, that->limit);
	}
	this->bantype = that->bantype;
	/* the logs will be opened in isjoined() */
	this->last = NULL;
	this->seen = NULL;
	memcpy(&this->floodvars, &that->floodvars, sizeof(struct floodvars));
	memcpy(&this->banvars, &that->banvars, sizeof(struct banvars));
	/* copy permbans */
	for(ms = that->permbans; ms != NULL; ms = ms->next) {
		if(addnewpermban(this, ms->mask, ms->optstring1, ms->optstring2) == NULL) {
			delchannel(this);
			return 0;
		}
	}
	/* copy nickbks */
	for(ms = that->nickbks; ms != NULL; ms = ms->next) {
		if(addnewnickbk(this, ms->mask, ms->optstring1, ms->optstring2) == NULL) {
			delchannel(this);
			return 0;
		}
	}
	/* copy wordbks */
	for(ms = that->wordbks; ms != NULL; ms = ms->next) {
		if(addnewwordbk(this, ms->mask, ms->optstring1, ms->optstring2) == NULL) {
			delchannel(this);
			return 0;
		}
	}
	/* copy users, their masks and optionally passwords */
	for(user = that->users; user != NULL; user = user->next) {
		if(addnewuser(this, user->handle, user->options) == NULL) {
			delchannel(this);
			return 0;
		}
		for(ms = user->firstmask; ms != NULL; ms = ms->next) {
			if(addnewusermask(this, user->handle, ms->mask) == NULL) {
				delchannel(this);
				return 0;
			}
		}
		if(user->password != NULL) {
                        if(!setpassword(this, user->handle, user->password)) {
				delchannel(this);
				return 0;
			}
		}
	}
	/* copy topics */
	for(topic = that->topics; topic != NULL; topic = topic->next) {
		if(addnewtopic(this, "clonechannel()", topic->topic) == NULL) {
			delchannel(this);
			return 0;
		}
	}
	/* copy adverts */
	for(advert = that->adverts; advert != NULL; advert = advert->next) {
		if(addnewadvert(this, advert->to, advert->message) == NULL) {
			delchannel(this);
			return 0;
		}
	}
	return 1;
}

int isfriend(struct channel *this, char *nick, char *userhost)
{
        struct chanuser *cu;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "isfriend(\"%s\", \"%s\", \"%s\")\n", this->name, nick, (userhost == NULL) ? "(null)" : userhost);
#endif
	/* first check user records (it's possible the user has logoned
           but isn't on channel yet */
	if(hasuserflags(this, userhost, "f", "d")) {
		return 1;
	}
	/* then check if user is logoned and has "f" flag */
	if(hasuserflags(this, userhost, "df", "") && isvalidlogon(this, userhost)) {
		return 1;
	}
	/* then check the channel info */
        for(cu = this->nicks; cu != NULL; cu = cu->next) {
		if(!strcasecmp(cu->nick, nick)) {
			return cu->friend;
		}
	}
	return 0;
}

int isvalidchannel(char *name)
{
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "isvalidchannel(\"%s\")\n", name);
#endif
	if(strlen(name) > 200) {
		return 0;
	}
	if(name[0] != '&' && name[0] != '#' && name[0] != '+' && name[0] != '!') {
		return 0;
	}
	if(index(name, ' ') != NULL) {
		return 0;
	}
	if(index(name, 7) != NULL) {
		return 0;
	}
	if(index(name, ',') != NULL) {
		return 0;
	}
	return 1;
}

int noexpiredlogons(struct channel *this)
{
	struct authed *a;

	for(a = this->autheds; a != NULL; a = a->next) {
		if(!haschanflags(this, "x") && (mystate->now > (a->at + OER_LOGON_TIMEOUT))) {
			return 0;
		}
	}
	return 1;
}

long getfilesize(char *filename)
{
	long size;
	FILE *fp;

        if((fp = fopen(filename, "r")) == NULL) {
		return -1;
	}
	if(fseek(fp, 0, SEEK_END) < 0) {
		return -1;
	}
	size = ftell(fp);
	fclose(fp);
	return size;
}

int checkforaction(struct channel *this, char *nick, char *userhost, int ctcp)
{
        char ban[USERHOSTLEN + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "checkforaction(\"%s\", \"%s\", \"%s\", %d)\n", this->name, nick, userhost, ctcp);
#endif
        if(!haschanflags(this, "c")) {
                return 0;
        }
	if(isq(nick)) {
                return 0;
        }
        if(isadmin(this, userhost)) {
                return 0;
        }
        if(isop(this, userhost)) {
                return 0;
        }
        if(!haschanflags(this, "N") && isopnow(this, nick)) {
                return 0;
        }
        if(haschanflags(this, "F") && isfriend(this, nick, userhost)) {
		return 0;
	}
	if(ctcp != OER_WHICHCTCP_ACTION) {
		return 0;
	}
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_INFO, "checkforaction->disallowed action detected on %s from %s\n", this->name, nick);
#endif
        strncpy(ban, userhost, USERHOSTLEN);
        if(safeban(this, ban, nick) == NULL) {
                return 0;
        }
	if(this->banvars.normal_ban > 0) {
		mmode_new(this, mystate->now - 1, "+b", ban);
	}
	snprintf(timed_str, WRITE_BUFFER_LENGTH, "KICK %s %s :disallowed ACTION", this->name, nick);
        timed_new(NULL, mystate->now, OER_TIMED_TYPE_NORMAL, OER_TIMED_PRIORITY_CHANNEL_PROTECTION, timed_str);
	if(this->banvars.normal_ban > 0) {
		mmode_new(this, mystate->now + this->banvars.normal_ban, "-b", ban);
	}
        return 1;
}

char getmodepol(char *modeline, char mode)
{
	int i;
	int length;
	char polar;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_NOISE, "getmodepol(\"%s\", \"%c\")\n", modeline, mode);
#endif
	length = strlen(modeline);
	for(i = 0, polar = 'x'; i < length; i++) {
		switch(modeline[i]) {
		case '+':
		case '-':
			polar = modeline[i];
			break;
                default:
			if(modeline[i] == mode) {
				return polar;
			}
			break;
		}
	}
	return 'x';
}

struct botuser *usercopy(struct channel *this, char *handle, struct channel *that, char *newhandle)
{
	char *ptr;
	struct botuser *user;
	struct botuser *user2;
	struct maskstruct *ms;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "usercopy(\"%s\", \"%s\", \"%s\", \"%s\")\n", this->name, handle, that->name, (newhandle == NULL) ? "(null)" : newhandle);
#endif
	/* first check that the source exists */
	for(user = this->users; user != NULL; user = user->next) {
		if(!strcasecmp(user->handle, handle)) {
			break;
		}
	}
	if(user == NULL) {
		return NULL;
	}
	ptr = (newhandle == NULL) ? handle : newhandle;
	/* add user without options */
	if((user2 = addnewuser(that, ptr, "")) == NULL) {
		return NULL;
	}
	/* copy masks */
	for(ms = user->firstmask; ms != NULL; ms = ms->next) {
		if(addnewusermask(that, ptr, ms->mask) == NULL) {
			deluser(that, ptr);
			return NULL;
		}
	}
	return user2;
}

void validateflags(char *flags, char *to, int type)
{
	int length;
	int i;
	int k;
	char outstring[STRINGLEN + 1];

	memset(to, 0, STRINGLEN + 1);
        memset(outstring, 0, STRINGLEN + 1);
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_NOISE, "validateflags(\"%s\", \"%s\", %d)\n", flags, to, type);
#endif
	switch(type) {
	case OER_FLAGS_TYPE_GLOBAL:
		strncpy(outstring, OER_FLAGS_GLOBAL, STRINGLEN);
		break;
	case OER_FLAGS_TYPE_CHANNEL:
		strncpy(outstring, OER_FLAGS_CHANNEL, STRINGLEN);
		break;
	case OER_FLAGS_TYPE_ADMIN:
		strncpy(outstring, OER_FLAGS_ADMIN, STRINGLEN);
		break;
	case OER_FLAGS_TYPE_USER:
		strncpy(outstring, OER_FLAGS_USER, STRINGLEN);
		break;
	}
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_NOISE, "validateflags->outstring is %s\n", outstring);
#endif
	to[0] = '\0';
	for(i = 0, k = 0, length = strlen(flags); i < length; i++) {
		if(index(outstring, (int)flags[i]) != NULL && index(to, (int)flags[i]) == NULL) {
			to[k++] = flags[i];
		}
	}
	to[k] = '\0';
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_NOISE, "validateflags->returning %s\n", to);
#endif
}

struct channelban *addnewchannelban(struct channel *this, char *ban, char *setby, time_t at)
{
	struct channelban *cb;
	struct channelban *cb2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "addnewchannelban(\"%s\", \"%s\", \"%s\", %lu)\n", this->name, ban, setby, at);
#endif
	cb = this->channelbans;
	cb2 = cb;
	while(cb != NULL) {
		cb2 = cb;
		if(!strcasecmp(cb->ban, ban)) {
			return NULL;
		}
		cb = cb2->next;
	}
	if((cb = (struct channelban *) malloc(sizeof(struct channelban))) == NULL) {
		return NULL;
	}
	if((cb->ban = (char *) malloc(strlen(ban) + 1)) == NULL) {
		return NULL;
	}
	strcpy(cb->ban, ban);
	if(strlen(setby)) {
		if((cb->setby = (char *) malloc(strlen(setby) + 1)) == NULL) {
			return NULL;
		}
		strcpy(cb->setby, setby);
	} else {
		cb->setby = NULL;
	}
	cb->at = at;
	cb->next = NULL;
	cb->prev = NULL;
	if(this->channelbans == NULL) {
		/* first channelban, special case */
		this->channelbans = cb;
		return cb;
	}
	/* >=1 channelbans, normal processing */
	cb2->next = cb;
	cb->prev = cb2;
	return cb;
}

int delchannelban(struct channel *this, char *ban)
{
	struct channelban *cb;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "delchannelban(\"%s\", \"%s\")\n", this->name, ban);
#endif
	/* find & remove ban */
	for(cb = this->channelbans; cb != NULL; cb = cb->next) {
		if(!strcasecmp(cb->ban, ban)) {
			break;
		}
	}
	if(cb == NULL) {
		return 0;
	}
	if(cb->prev == NULL) {
                /* first */
                if(cb->next == NULL) {
                        /* only */
                        this->channelbans = NULL;
                } else {
                        cb->next->prev = NULL;
                        this->channelbans = cb->next;
                }
        } else if(cb->next == NULL) {
                /* last */
                cb->prev->next = NULL;
        } else {
                /* between 2 or more */
                cb->prev->next = cb->next;
                cb->next->prev = cb->prev;
        }
	free(cb->setby);
	free(cb->ban);
	free(cb);
	return 1;
}

void resetfloodcounters(struct channel *this, char *nick)
{
	struct chanuser *cu;

	/* get nick structure */
	if((cu = getcuptr(this, nick)) == NULL) {
		return;
	}
	cu->isflood_last = 0;
	cu->isflood_lines = 0;
	cu->isflood_chars = 0;
}

int ischanopless(struct channel *this)
{
        struct chanuser *cu;
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_NOISE, "ischanopless(\"%s\")\n", (this == NULL) ? "(null)" : this->name);
#endif
        if(this == NULL) {
                return 0;
        }
        for(cu = this->nicks; cu != NULL; cu = cu->next) {
                if(cu->chanop) {
                        return 0;
                }
        }
        return 1;
}

int joininprogress(void)
{
	struct channel *this;

	for(this = mystate->channels; this != NULL; this = this->next) {
		if(this->joining) {
			return 1;
		}
	}
	return 0;
}

void extcommand(struct channel *this, char *to, int tochan, char *nick, char *userhost, char *commandline)
{
	int ppos;
	int lines;
	int more;
        char p1[STRINGLEN + 1];
        char p2[STRINGLEN + 1];
        char p3[STRINGLEN + 1];
        char p4[STRINGLEN + 1];
        char p5[STRINGLEN + 1];
        char q1[STRINGLEN + 1];
        char q2[STRINGLEN + 1];
        char q3[STRINGLEN + 1];
        char q4[STRINGLEN + 1];
        char q5[STRINGLEN + 1];
	char outstring[TXT_BUFFER + 1];
	char stringbuffer[HUGESTRINGLEN + 1];
	FILE *fp;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "extcommand(\"%s\", \"%s\", %d, \"%s\", \"%s\", \"%s\")\n", (this == NULL) ? "(null)" : this->name, to, tochan, nick, userhost, commandline);
#endif
	/* command line parsing begins here, first parameter = the command */
	ppos = 0;
        if((ppos = parse(commandline, ppos, " ", p1, STRINGLEN, 0)) < 0) {
                return;
        }
        if((ppos = parse(commandline, ppos, " ", p2, STRINGLEN, 0)) < 0) {
                return;
        }
        if((ppos = parse(commandline, ppos, " ", p3, STRINGLEN, 0)) < 0) {
                return;
        }
        if((ppos = parse(commandline, ppos, " ", p4, STRINGLEN, 0)) < 0) {
                return;
        }
        if((ppos = parse(commandline, ppos, " ", p5, STRINGLEN, 0)) < 0) {
                return;
        }
	if(!strlen(p1)) {
		/* first argument is required */
		return;
	}
	snprintf(stringbuffer, HUGESTRINGLEN, "%s/%s%s", mystate->basedir, mystate->progname, ".ext");
	if((fp = fopen(stringbuffer, "r")) == NULL) {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "extcommand->failed to open %s: %s\n", stringbuffer, strerror(errno));
#endif
		return;
	}
	lines = 0;
	more = 0;
	while(fgets(outstring, TXT_BUFFER, fp) && !more) {
		striplf(outstring);
		if(emptyline(outstring)) {
			continue;
		}
		if(iscomment(outstring)) {
			continue;
		}
		ppos = 0;
		if((ppos = parse(outstring, ppos, OER_DEFAULT_DELIM, q1, STRINGLEN, 1)) < 0) {
			return;
		}
		if((ppos = parse(outstring, ppos, OER_DEFAULT_DELIM, q2, STRINGLEN, 1)) < 0) {
			return;
		}
		if((ppos = parse(outstring, ppos, OER_DEFAULT_DELIM, q3, STRINGLEN, 1)) < 0) {
			return;
		}
		if((ppos = parse(outstring, ppos, OER_DEFAULT_DELIM, q4, STRINGLEN, 1)) < 0) {
			return;
		}
		if((ppos = parse(outstring, ppos, OER_DEFAULT_DELIM, q5, STRINGLEN, 1)) < 0) {
			return;
		}
		if(!strcasecmp(p1, q1) && !strcasecmp(p2, q2) && !strcasecmp(p3, q3) && !strcasecmp(p4, q4) && !strcasecmp(p5, q5)) {
			if(lines < OER_MAX_LIST_LINES) {
				snprintf(stringbuffer, HUGESTRINGLEN, "\"%s\"", outstring + ppos);
				sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
				lines++;
			} else {
				more = 1;
			}
		}
	}
	fclose(fp);
	if(more) {
		snprintf(stringbuffer, HUGESTRINGLEN, "listing limited to %d lines", OER_MAX_LIST_LINES);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
	}
}

void resetcommand(struct channel *this, char *to, int tochan, char *nick, char *userhost, char *commandline)
{
	int ppos;
	int deleted;
        char p1[STRINGLEN + 1];
	char stringbuffer[HUGESTRINGLEN + 1];
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "resetcommand(\"%s\", \"%s\", %d, \"%s\", \"%s\", \"%s\")\n", (this == NULL) ? "(null)" : this->name, to, tochan, nick, userhost, commandline);
#endif
	ppos = 0;
        if((ppos = parse(commandline, ppos, " ", p1, STRINGLEN, 0)) < 0) {
                return;
        }
	if(!strcasecmp(p1, "adverts")) {
		if(!(deleted = deladverts(this, "", 1))) {
			return;
		}
		snprintf(stringbuffer, HUGESTRINGLEN, "deleted %d %s from %s", deleted, (deleted == 1) ? "advert" : "adverts", this->name);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "autheds")) {
		if(!isopa(userhost)) {
			return;
		}
		if(!(deleted = cleanautheds(this, 1))) {
			return;
		}
		snprintf(stringbuffer, HUGESTRINGLEN, "deleted %d %s from %s", deleted, (deleted == 1) ? "authed" : "autheds", this->name);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "masks")) {
		if(!isopa(userhost)) {
			return;
		}
		if(!(deleted = delusermasks(this, "", "", 1))) {
			return;
		}
		snprintf(stringbuffer, HUGESTRINGLEN, "deleted %d %s from %s", deleted, (deleted == 1) ? "mask" : "masks", this->name);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "nickbks")) {
		if(!(deleted = delnickbks(this, "", 1))) {
			return;
		}
		snprintf(stringbuffer, HUGESTRINGLEN, "deleted %d %s from %s", deleted, (deleted == 1) ? "nickbk" : "nickbks", this->name);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "permbans")) {
		if(!(deleted = delpermbans(this, "", 1))) {
			return;
		}
		snprintf(stringbuffer, HUGESTRINGLEN, "deleted %d %s from %s", deleted, (deleted == 1) ? "permban" : "permbans", this->name);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
	if(!strcasecmp(p1, "wordbks")) {
		if(!(deleted = delwordbks(this, "", 1))) {
			return;
		}
		snprintf(stringbuffer, HUGESTRINGLEN, "deleted %d %s from %s", deleted, (deleted == 1) ? "wordbk" : "wordbks", this->name);
		sendreply(to, tochan, 0, OER_TIMED_PRIORITY_NORMAL, stringbuffer);
		return;
	}
}

void initmmodesfornick(struct channel *this, char *nick)
{
	struct mmode *m1;
	struct mmode *m2;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "initmmodesfornick(\"%s\", \"%s\")\n", this->name, nick);
#endif
	m1 = this->mmodes;
	while(m1 != NULL) {
		m2 = m1->next;
		if(!strcasecmp(m1->target, nick)) {
			mmode_del(this, m1);
		}
		m1 = m2;
	}
	if(!getmmodecount(this)) {
		this->mmodes = NULL;
	}
}

void changemmodesfornick(struct channel *this, char *nick, char *to)
{
	struct mmode *m;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "changemmodesfornick(\"%s\", \"%s\", \"%s\")\n", this->name, nick, to);
#endif
	for(m = this->mmodes; m != NULL; m = m->next) {
		if(!strcasecmp(m->target, nick)) {
			free(m->target);
			m->target = malloc(strlen(to) + 1);
			strcpy(m->target, to);
		}
	}
}

int getmmodecount(struct channel *this)
{
	int count;
	struct mmode *m;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "getmmodecount(\"%s\")\n", this->name);
#endif
	for(count = 0, m = this->mmodes; m != NULL; m = m->next) {
		count++;
	}
	return count;
}

int gettimedcount()
{
	int count;
	struct timed *t;
#ifdef OER_DEBUG
	oer_debug(OER_DEBUG_INFO, "gettimedcount()\n");
#endif
	for(count = 0, t = mystate->timeds; t != NULL; t = t->next) {
		count++;
	}
	return count;
}
