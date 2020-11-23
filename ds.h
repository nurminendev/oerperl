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

/* global prototype definitions */
extern char *safeban(struct channel *, char *, char *);
extern char *getfirsthostless(struct channel *, char *);
extern char *getkickreason(char *);
extern char getmodepol(char *, char);
extern int partcount(struct channel *, char *, char *);
extern int loadall(void);
extern int loadconf(void);
extern int loadchannels(void);
extern int loadtrusted(void);
extern int loadservices(void);
extern int loadscripts(void);
extern int loadadmins(void);
extern int loadkickreasons(void);
extern int loadchaninfo(void);
extern int saveall(void);
extern int saveconf(void);
extern int savechannels(void);
extern int savetrusted(void);
extern int saveservices(void);
extern int savescripts(void);
extern int saveadmins(void);
extern int savekickreasons(void);
extern int savechaninfo(void);
extern int parseconf(char *);
extern int parsechannels(char *);
extern int parsetrusted(char *);
extern int parseservices(char *);
extern int parsescripts(char *);
extern int parseadmins(char *);
extern int parsekickreasons(char *);
extern int parsechaninfo(char *);
extern int burstjoins(struct channel *);
extern int isq(char *);
extern int isservice(char *);
extern int istrusted(char *);
extern int isme(char *);
extern int isop(struct channel *, char *);
extern int isvoice(struct channel *, char *);
extern int isvalidlogon(struct channel *, char *);
extern int isopa(char *);
extern int ishostless(char *);
extern int delopa(char *);
extern int isatleastopnow(struct channel *, char *, char *);
extern int isatleastop(struct channel *, char *, char *);
extern int isallowedop(struct channel *, char *);
extern int isadmin(struct channel *, char *);
extern int isonchan(struct channel *, char *);
extern int issameuser(struct channel *, char *, char *);
extern int isopnow(struct channel *, char *);
extern int nickchange(struct channel *, char *, char *);
extern int isflood(struct channel *, char *, char *, char *);
extern int isvoicenow(struct channel *, char *);
extern int haschanflags(struct channel *, char*);
extern int hasadminflags(char *, char *);
extern int ispermban(struct channel *, char *);
extern int permbancount(struct channel *);
extern int topiccount(char *);
extern int nthmode(char *, int);
extern int userleft(struct channel *, char *, char *);
extern int changeuser(struct channel *, char *, int, int, int, int);
extern int whichctcp(char *);
extern int whichcommand(char *, int);
extern int gettobek(struct channel *, char *);
extern int checkforansi(struct channel *, char *, char *, char *);
extern int checkforflood(struct channel *, char *, char *, char *);
extern int checkfornickflood(struct channel *, char *, char *);
extern int checkforbadword(struct channel *, char *, char *, char *);
extern int checkforbadnick(struct channel *, char *, char *);
extern int checkforautorejoin(struct channel *, char *, char *);
extern int checkforpartrejoin(struct channel *, char *, char *);
extern int checkforaction(struct channel *, char *, char *, int);
extern int delchankey(struct channel *);
extern int delchanlimit(struct channel *);
extern int delpermbans(struct channel *, char *, int);
extern int delnickbks(struct channel *, char *, int);
extern int delwordbks(struct channel *, char *, int);
extern int uptime(time_t *, time_t *);
extern int deladmin(char *);
extern int deladminmask(char *, char *);
extern int deluser(struct channel *, char *);
extern int delusermasks(struct channel *, char *, char *, int);
extern int deltrusted(char *);
extern int delservice(char *);
extern int delscript(char *);
extern int delchannel(char *);
extern int delkickreason(char *);
extern int getondiskmsgcount(struct channel *, char *);
extern int getondiskjoincount(struct channel *, char *);
extern int getrandommsg(struct channel *, char *, int, int, char *, int);
extern int getjoincount(struct channel *, char *);
extern int delserver(char *, int, int, int, int, int);
extern int deladverts(struct channel *, char *, int);
extern int setpassword(struct channel *, char *, char *);
extern int logoff(struct channel *, char *);
extern int logon(struct channel *, struct botuser *, char *, char *, int);
extern int timed_cmp(struct timed *, struct timed *);
extern int clonechannel(char *, char *);
extern int isfriend(struct channel *, char *, char *);
extern int deltaflags(struct channel *, char *, char);
extern int isvalidchannel(char *);
extern int hasuserflags(struct channel *, char *, char *, char *);
extern int noexpiredlogons(struct channel *);
extern int delchannelban(struct channel *, char *);
extern int ischanopless(struct channel *);
extern int joininprogress(void);
extern int getmmodecount(struct channel *);
extern int gettimedcount(void);
extern int cleanautheds(struct channel *, int);
extern unsigned int getrandom(unsigned int);
extern long getfilesize(char *);
extern struct authed *addnewauthed(struct channel *, time_t, struct botuser *, char *);
extern struct botuser *addbotuser(char *, char *);
extern struct botuser *addnewadmin(char *, char *);
extern struct botuser *addnewuser(struct channel *, char *, char *);
extern struct botuser *usercopy(struct channel *, char *, struct channel *, char *);
extern struct channel *addnewchannel(char *);
extern struct channel *getchptr(char *);
extern struct channelban *addnewchannelban(struct channel *, char *, char *, time_t);
extern struct chanuser *userjoined(struct channel *, char *, char *, int, int, int, int);
extern struct chanuser *getcuptr(struct channel *, char *);
extern struct state *emptystate(void);
extern struct server *addnewserver(char *, int, int, int, int, int, char *);
extern struct server *getserver(void);
extern struct advert *addnewadvert(struct channel *, char *, char *);
extern struct maskstruct *addnewmask(char *);
extern struct maskstruct *addnewpermban(struct channel *, char *, char *, char *);
extern struct maskstruct *addnewnickbk(struct channel *, char *, char *, char *);
extern struct maskstruct *addnewwordbk(struct channel *, char *, char *, char *);
extern struct maskstruct *addnewtrusted(char *);
extern struct maskstruct *addnewservice(char *);
extern struct script *addnewscript(char *);
extern struct eventhandler *addneweventhandler(struct script *, char *, SV *);
extern struct commandhandler *addnewcommandhandler(struct script *, char *, SV *);
extern struct scripttimer *addnewscripttimer(struct script *, time_t, SV *);
extern struct maskstruct *addnewkickreason(char *);
extern struct maskstruct *addnewusermask(struct channel *, char *, char *);
extern struct maskstruct *addnewadminmask(char *, char *);
extern struct maskstruct *editmask(struct maskstruct *, char *, char *);
extern struct maskstruct *isnickbk(struct channel *, char *);
extern struct maskstruct *iswordbk(struct channel *, char *);
extern struct pubmsg *addnewpubmsg(struct channel *, time_t, char *, char *, char *);
extern struct nickchange *addnewnickchange(struct channel *, char *);
extern struct part *addnewpart(struct channel *, time_t, char *, char *);
extern struct join *addnewjoin(struct channel *, time_t, char *, char *);
extern struct topic *addnewtopic(struct channel *, char *, char *);
extern struct topic *insertnewtopic(struct channel *, char *, char *, int);
extern struct topic *edittopic(struct channel *, char *, char *, int);
extern struct chanuser *getrandomuser(struct channel *);
extern struct ls_wrapstruct *addnew_ls(struct ls_wrapstruct *, int, time_t, char *, char *, char *);
extern struct timed *timed_new(struct channel *, time_t, int, int, char *);
extern struct mmode *mmode_new(struct channel *, time_t, char *, char *);
extern time_t lastoff(struct channel *, char *, char *);
extern void sendadverts(struct channel *, char *, char *);
extern void resetparts(struct channel *, char *, char *);
extern void initpubmsgs(struct channel *);
extern void freepubmsguser(char *, char *, char *);
extern void freenickchanges(struct channel *, char *);
extern void lockchan(struct channel *, char *, int, char *, char *);
extern void unlockchan(struct channel *, char *, char *);
extern void initchannel(struct channel *);
extern void initall(void);
extern void initnicks(struct channel *);
extern void initmmodes(struct channel *);
extern void initmmodesfornick(struct channel *, char *);
extern void changemmodesfornick(struct channel *, char *, char *);
extern void initparts(struct channel *);
extern void initjoins(struct channel *);
extern void initnickbks(struct channel *);
extern void initwordbks(struct channel *);
extern void initpermbans(struct channel *);
extern void initchannelbans(struct channel *);
extern void initusers(struct channel *);
extern void delalltopics(struct channel *);
extern void swaptopic(struct channel *, int, int);
extern void gettopic(struct channel *, char *);
extern void listtopic(struct channel *, char *, int);
extern void setuserhost(char *, char *);
extern void sethostquerystatus(char *, int);
extern void banuser(struct channel *, time_t, char *);
extern void unbanuser(struct channel *, time_t, char *);
extern void kickuser(struct channel *, time_t, char *, char *);
extern void sendreply(char *, int, int, int, char *);
extern void sendwall(char *, struct channel *, int, char *);
extern void sendchannelnotice(struct channel *, int, char *);
extern void addcommand(struct channel *, char *, int, char *, char *, char *);
extern void delcommand(struct channel *, char *, int, char *, char *, char *);
extern void editcommand(struct channel *, char *, int, char *, char *, char *);
extern void extcommand(struct channel *, char *, int, char *, char *, char *);
extern void listcommand(struct channel *, char *, int, char *, char *, char *);
extern void logoffcommand(struct channel *, char *, int, char *, char *, char *);
extern void logoncommand(struct channel *, char *, int, char *, char *, char *);
extern void isjoined(struct channel *);
extern void changetobek(struct channel *, char *, int);
extern void syncvoices(struct channel *);
extern void syncops(struct channel *);
extern void syncbans(struct channel *);
extern void syncnickbks(struct channel *);
extern void syncuserhosts(void);
extern void settopic(struct channel *);
extern void processenv(void);
extern void processnetjoin(struct channel *);
extern void mmodes2timeds(void);
extern void processlock(struct channel *);
extern void joinchannel(struct channel *);
extern void updatelast(struct channel *, char *, char *, char *);
extern void updateseen(struct channel *, char *, char *);
extern void showlast(struct channel *, char *, int, char *);
extern void showseen(struct channel *, char *, int, char *);
extern void setchanmode(struct channel *);
extern void setchankey(struct channel *, char *);
extern void setchanlimit(struct channel *, char *);
extern void channelsync(struct channel *);
extern void cleartobek(struct channel *);
extern void getnthmode(char *, int, char *);
extern void massmessage(char *, char *);
extern void flushall(void);
extern void quit(void);
extern void parsectcp(char *, char *, int, char *);
extern void nstats(struct channel *, char *, int, char *);
extern void wall(struct channel *, char *, char *, char *);
extern void listchanusers(void);
extern void timed_del(struct timed *);
extern void mmode_del(struct channel *, struct mmode *);
extern void validateflags(char *, char *, int);
extern void resetfloodcounters(struct channel *, char *);
extern void resetcommand(struct channel *, char *, int, char *, char *, char *);
extern void deltopic(struct channel *, char *, int, char *, char *, char *);
