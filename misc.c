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

/* prototype definitions */
int emptyline(char *);
int iscomment(char *);
int isserver(char *);
int isvaliduserhost(char *);
int isvalidhost(char *);
int isvalidipv4host(char *);
int isvalidipv6host(char *);
int isansi(char *);
int isaction(char *);
int isctcp(char *);
int wordcount(char *);
int parse(char *, int, char *, char *, int, int);
int issamenickandhost(char *, char *, char *, char *);
int issimilarstring(char *, char *);
int isnumbw(int, int, int);
int completeban(char *, char *, int);
int countchars(char *, char);
int countnchars(char *, char);
int parseprotocolmessage(char *, char *, int, char *, int, char *, int, char *, int);
unsigned int longestword(char *);
unsigned int isgoodquote(char *);
char *mybasename(char *);
void striplf(char *);
void stripchars(char *, char *);
void stripcntrl(char *);
void stripansi(char *);
void stripmirc(char *);
void safequote(char *);
void oer_debug(int, const char *, ...);
void secondstostring(time_t, char *, int);
void sortstring(char *);
void filteroutchanmodes(char *, char *, char *, int);

#ifndef HAVE_SNPRINTF
int snprintf(char *, size_t, const char *, ...);
#endif

#ifndef HAVE_SNPRINTF
int snprintf(char *str, size_t size, const char *format, ...)
{
	int slen;
        va_list ap;
	char *ptr;

	/* what would be a "safe" size, beats me */
	if((ptr = malloc(TXT_BUFFER * 10)) == NULL) {
		return -1;
	}
	va_start(ap, format);
	vsprintf(ptr, format, ap);
	va_end(ap);
	strncpy(str, ptr, size);
	if(strlen(ptr) > size) {
		free(ptr);
		return -1;
	}
	slen = strlen(ptr);
	free(ptr);
	return slen;
}
#endif

void oer_debug(int level, const char *fmt, ...)
{
        va_list ap;

	if(do_tempoutput) {
		va_start(ap, fmt);
		vfprintf(stdout, fmt, ap);
		va_end(ap);
		return;
	}
	if(!do_console && !do_output) {
		return;
	}
        if(level <= do_debug) {
                va_start(ap, fmt);
                if(do_output) {
                        vfprintf(mystate->logto, fmt, ap);
			if(mystate->outputcounter >= OER_FORCE_FLUSH) {
				/* we cant output a possible failure */
				fflush(mystate->logto);
				mystate->outputcounter = 0;
			}
		}
		if(do_console) {
                        vfprintf(stdout, fmt, ap);
                }
                va_end(ap);
        }
}

int wordcount(char *line)
{
	int ppos;
	int nppos;
	int words;
	char word[WRITE_BUFFER_LENGTH + 1];

	ppos = 0;
	words = 0;
	while(1) {
		nppos = parse(line, ppos, " ", word, WRITE_BUFFER_LENGTH, 0);
		if(nppos == ppos || nppos < 0) {
			break;
		}
		ppos = nppos;
		words++;
	}
	return words;
}

int isansi(char *line)
{
        int i;
        int length;
	char stringbuffer[WRITE_BUFFER_LENGTH + 1];

	strncpy(stringbuffer, line, WRITE_BUFFER_LENGTH);
	stripansi(stringbuffer);
	for(i = 0, length = strlen(stringbuffer); i < length; i++) {
                if(iscntrl((int)stringbuffer[i])) {
			return 1;
		}
        }
	return 0;
}

int isctcp(char *line)
{
	int i;
	int length;

	/* The first char has to be ^A */
	if(line[0] != 1) {
		return 0;
	}
	for(i = 1, length = strlen(line); i < length; i++) {
		if(line[i] == 1) {
			return 1;
		}
	}
	return 0;
}

int isaction(char *line)
{
	char chars[STRINGLEN + 1];

	snprintf(chars, STRINGLEN, "%cACTION", 1);
	if(!strncasecmp(line, chars, strlen(chars))) {
		return 1;
	} else {
		return 0;
	}
}

void stripcntrl(char *source)
{
	int i;
	int k;
	int length;
        char stringbuffer[WRITE_BUFFER_LENGTH + 1];
	
	for(i = 0, k = 0, length = strlen(source); i < length && k < WRITE_BUFFER_LENGTH; i++) {
		if(!iscntrl((int)source[i])) {
			stringbuffer[k++] = source[i];
		}
	}
	stringbuffer[k] = 0;
	/* the outstring is at max. equal length to the
	   original source string */
	strcpy(source, stringbuffer);
}

void stripmirc(char *source)
{
	int i;
	int k;
	int iscolor;
	int incolor;
	int length;
        char stringbuffer[WRITE_BUFFER_LENGTH + 1];
	
	for(i = 0, k = 0, iscolor = 0, incolor = 0, length = strlen(source); i < length && k < WRITE_BUFFER_LENGTH; i++) {
		if(source[i] == 3) {
			iscolor = (iscolor) ? 0 : 1;
			incolor = (incolor) ? 0 : 1;
			continue;
		}
		if(iscolor && incolor && (isdigit((int)source[i]) || source[i] == ',')) {
			continue;
		}
		incolor = 0;
		stringbuffer[k++] = source[i];
	}
	stringbuffer[k] = 0;
	/* the outstring is at max. equal length to the
	   original source string */
	strcpy(source, stringbuffer);
}

void stripchars(char *source, char *what)
{
	int i;
	int j;
	int k;
	int length;
	int length2;
        char stringbuffer[WRITE_BUFFER_LENGTH + 1];

	for(i = 0, length = strlen(what); i < length; i++) {
		for(k = 0, j = 0, length2 = strlen(source); k < length2; k++) {
			if(source[k] != what[i]) {
				stringbuffer[j++] = source[k];
			}
		}
		/* the outstring is at max. equal length to the
		   original source string */
		stringbuffer[j] = 0;
		strcpy(source, stringbuffer);
	}
}

void safequote(char *source)
{
	int j;
	int k;
	int length;
        char stringbuffer[WRITE_BUFFER_LENGTH + 1];

	for(k = 0, j = 0, length = strlen(source); k < length; k++) {
		if(source[k] == ':') {
			if((k + 1 < length) && source[k + 1] == ':') {
				/* skip to next sequent char */
				k++;
			}
		}
		stringbuffer[j++] = source[k];
	}
	/* the outstring is at max. equal length to the
	   original source string */
	stringbuffer[j] = 0;
	strcpy(source, stringbuffer);
}

void stripansi(char *source)
{
	char chars[STRINGLEN + 1];

	/* strip ^A which is for ctcp actions */
	chars[0] = 1;
	/* strip ^B which is normal bold */
	chars[1] = 2;
	/* strip ^G which is bell */
	chars[2] = 7;
	/* strip ^V which is normal reverse */
	chars[3] = 22;
	/* strip ^_ which is normal underline */
	chars[4] = 31;
	/* strip ^O which is normal ansi toggle */
	chars[5] = 15;
	/* eol */
	chars[6] = 0;
	stripchars(source, chars);
}

int emptyline(char *line)
{
        int length;
        int i;

        for(i = 0, length = strlen(line); i < length; i++) {
                if(!isspace((int)line[i])) {
                        return 0;
		}
	}
        return 1;
}

int iscomment(char *line)
{
        int length;
        int i;

        for(i = 0, length = strlen(line); i < length && isspace((int)line[i]); i++) {
		;
	}
	if(line[i] == '#') {
		return 1;
	}
        return 0;
}

void striplf(char *line)
{
        int i;

        for(i = strlen(line) - 1; isspace((int)line[i]) && i >= 0; i--) {
                line[i] = '\0';
	}
}

int isserver(char *line)
{
	int i;
	int length;
	int dots;

	for(i = 0, length = strlen(line), dots = 0; i < length; i++) {
		if(line[i] == '.') {
			dots++;
		}
	}
	if(wordcount(line) >= 2 && dots >= 2) {
		return 1;
	} else {
		return 0;
	}
}

int isvaliduserhost(char *line)
{
	int c;
	int c2;
	char *ptr;
	char *ptr2;

        if((ptr = index(line, '@')) == NULL) {
		return 0;
	}
	ptr2 = ptr + strlen(line);
	c = *(ptr - 1);
	c2 = *(ptr + 1);
	if(ptr == line || ptr == ptr2 || !isgraph(c) || !isgraph(c2)) {
		return 0;
	}
	return 1;
}

int parse(char *from, int pos, char *delim, char *to, int to_length, int fixed)
{
        int i;
        int j;
        int k;
        int len;
	int found_delim;

        memset(to, 0, to_length + 1);
	i = pos;
	k = 0;
	j = strlen(delim);
	len = strlen(from);
	found_delim = 0;
	while(i < len && k < to_length) {
		/* loop until delim found, respecting 
		   length constraints */
		if(fixed && !strncasecmp(from + i, delim, j)) {
			found_delim = 1;
			break;
		}
		if(!fixed && (from[i] == delim[0])) {
			found_delim = 1;
			break;
		}
                to[k++] = from[i++];
	}
	if(found_delim) {
		/* advance until next non-delim character */
		if(fixed) {
			i += j;
		} else {
			while((from[i] == delim[0]) && (i < len)) {
				i++;
			}
		}
	}
        if(k >= to_length) {
		/* insufficient buffer space */
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_WARNING, "parse->destination buffer too small\n");
#endif
                return -1;
        }
        to[k] = '\0';
	return i;
}

int parseprotocolmessage(char *serveroutput, char *prefix, int prefix_len, char *command, int command_len, char *params, int params_len, char *trailing, int trailing_len)
{
        int i;
        int k;
        int len;

	i = 0;
        len = strlen(serveroutput);
        /* skip ':' if exists */
	if((i < len) && (serveroutput[i] == ':')) {
		i++;
	}
        k = 0;
        memset(prefix, 0, prefix_len);
        /* parse the prefix */
        while((i < len) && (k < prefix_len) && (serveroutput[i] != ' ')) {
                prefix[k++] = serveroutput[i++];
        }
        prefix[k] = '\0';
        /* skip following SPACE(s) */
        while((i < len) && (serveroutput[i] == ' ')) {
                i++;
        }
        k = 0;
        memset(command, 0, command_len);
        /* parse the command */
        while((i < len) && (k < command_len) && (serveroutput[i] != ' ')) {
                command[k++] = serveroutput[i++];
        }
        command[k] = '\0';
        /* skip following SPACE(s) */
        while((i < len) && (serveroutput[i] == ' ')) {
                i++;
        }
        /* skip ':' if exists */
	if((i < len) && (serveroutput[i] == ':')) {
		i++;
	}
	k = 0;
        memset(params, 0, params_len);
        /* parse the params */
        while((i < len) && (k < params_len) && (serveroutput[i] != ' ')) {
                params[k++] = serveroutput[i++];
        }
        params[k] = '\0';
        /* skip following SPACE(s) */
        while((i < len) && (serveroutput[i] == ' ')) {
                i++;
        }
	k = 0;
	memset(trailing, 0, trailing_len);
        /* if there is ':' -> end of message goes to trailing */
	if((i < len) && (serveroutput[i] == ':')) {
		i++;
		/* parse the trailing */
		while((i < len) && (k < trailing_len)) {
			trailing[k++] = serveroutput[i++];
		}
		trailing[k] = '\0';
	} else {
		/* trailing gets only middle part */
		while((i < len) && (k < trailing_len) && (serveroutput[i] != ' ')) {
			trailing[k++] = serveroutput[i++];
		}
		trailing[k] = '\0';
		/* skip following SPACE(s) */
		while((i < len) && (serveroutput[i] == ' ')) {
			i++;
		}
		/* skip ':' if exists */
		if((i < len) && (serveroutput[i] == ':')) {
			i++;
		}
	}
        return i;
}

void secondstostring(time_t seconds, char *string, int maxlen)
{
	int fields;
        time_t safe;
        time_t days;
        time_t hours;
        time_t minutes;
        char stringbuffer[STRINGLEN + 1];

        safe = seconds;
        days = seconds / 3600 / 24;
        seconds -= days * (3600 * 24);
        hours = seconds / 3600;
        seconds -= hours * (3600);
        minutes = seconds / 60;
        seconds -= minutes * 60;
	memset(string, 0, maxlen);
	fields = 0;
        if(days > 0) {
                snprintf(string, maxlen, "%lu %s ", days, (days == 1) ? "day" : "days");
		fields++;
		fields++;
        }
        if(hours > 0) {
                snprintf(stringbuffer, STRINGLEN, "%lu %s ", hours, (hours == 1) ? "hour" : "hours");
                strncat(string, stringbuffer, maxlen - strlen(string));
		fields++;
        }
        if(minutes > 0) {
                snprintf(stringbuffer, STRINGLEN, "%lu %s ", minutes, (minutes == 1) ? "minute" : "minutes");
                strncat(string, stringbuffer, maxlen - strlen(string));
		fields++;
        }
        if(seconds > 0) {
		if(fields > 0) {
			snprintf(stringbuffer, STRINGLEN, "and %lu %s", seconds, (seconds == 1) ? "second" : "seconds");
		} else {
			snprintf(stringbuffer, STRINGLEN, "%lu %s", seconds, (seconds == 1) ? "second" : "seconds");
		}
                strncat(string, stringbuffer, maxlen - strlen(string));
		fields++;
        }
}

int issamenickandhost(char *n1, char *n2, char *h1, char *h2)
{
        if(n1 == NULL || n2 == NULL || h1 == NULL || h2 == NULL) {
		return 0;
	}
	if(!strcasecmp(n1, n2) && !strcasecmp(n1, n2) && !strcasecmp(h1, h2)) {
		return 1;
	}
	return 0;
}

void sortstring(char *string)
{
	int times;
	int pos;
	int u1;
	int u2;
	int c1;
	int c2;
	int c;

	for(times = strlen(string) - 1; times >= 0; times--) {
		for(pos = strlen(string) - 1; pos > 0; pos--) {
			u1 = 0;
			u2 = 0;
			c1 = string[pos - 1];
			c2 = string[pos];
			if(isupper(c1)) {
				u1 = 1;
				c1 = tolower(c1);
			}
			if(isupper(c2)) {
				u2 = 1;
				c2 = tolower(c2);
			}
			if(c1 < c2) {
				continue;
			}
			if(c1 == c2 && u1) {
				continue;
			}
			if(u1) {
				c1 = toupper(c1);
			}
			if(u2) {
				c2 = toupper(c2);
			}
			c = c2;
			string[pos] = c1;
			string[pos - 1] = c;
		}
	}
}

unsigned int longestword(char *quote)
{
        int len;
        int pos;
        int max;
        int wlen;

        len = strlen(quote);
        pos = 0;
        max = 0;
        while(pos < len) {
                while(pos < len && isspace((int)quote[pos])) {
                        pos++;
                }
                wlen = 0;
                while(pos < len && !isspace((int)quote[pos])) {
                        wlen++;
                        pos++;
                }
                if(wlen > max) {
                        max = wlen;
                }
        }
        return max;
}

unsigned int isgoodquote(char *quote)
{
        /* we want a string longer than OER_QUOTE_MIN_LENGTH */
        if(strlen(quote) < OER_QUOTE_MIN_LENGTH) {
                return 0;
        }
        /* we want a string shorter than OER_QUOTE_MAX_LENGTH */
        if(strlen(quote) > OER_QUOTE_MAX_LENGTH) {
                return 0;
        }
        /* we want the string to contain at least 2 words */
        if(wordcount(quote) < 3) {
                return 0;
        }
        /* no word of the string should be longer than OER_QUOTE_MAX_WORD_LENGTH */
        if(longestword(quote) > OER_QUOTE_MAX_WORD_LENGTH) {
                return 0;
        }
        /* no strings with (c) in them */
        if(strstr(quote, "(c)")) {
                return 0;
        }
        /* no strings with http:// in them */
		if(strstr(quote, "http://")) {
			return 0;
		}
		return 1;
}

int issimilarstring(char *s1, char *s2)
{
        int s1len;
        int s2len;
        int fract;
        /* first do a compare for exact match */
        if(!strcasecmp(s1, s2) || !strcasecmp(s2, s1)) {
                return 1;
        }
        s1len = strlen(s1);
        s2len = strlen(s2);
        if(s1len < 8 || s2len < 8) {
                return 0;
        }
        /* then do a compare of s1 and s2 beginning
           at pos 0 + len/8 and ending at pos len - len/8 */
        if(s1len > s2len) {
                fract = s2len >> 3;
                if(!strncasecmp(s1 + fract, s2 + fract, s1len - (fract << 1))) {
                        return 1;
                }
        } else {
                fract = s1len >> 3;
                if(!strncasecmp(s2 + fract, s1 + fract, s2len - (fract << 1))) {
                        return 1;
                }
        }
        return 0;
}

int isnumbw(int number, int low, int high)
{
	if(number >= low && number <= high) {
		return 1;
	} else {
#ifdef OER_DEBUG
		oer_debug(OER_DEBUG_INFO, "isnumbw->%d is not >= %d and <= %d\n", number, low, high);
#endif
		return 0;
	}
}

int isvalidhost(char *line)
{
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_INFO, "isvalidhost->%s\n", line);
#endif
        /* check for IPv4 or IPv6 hostname */
        if(!isvalidipv4host(line) && !isvalidipv6host(line)) {
                return 0;
        }
        return 1;
}

int isvalidipv4host(char *line)
{
        char *ptr;
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_INFO, "isvalidipv4host->%s\n", line);
#endif
        /* we require at least X.Y */
        if(strlen(line) < 3 || (ptr = index(line, '.')) == NULL) {
                return 0;
        }
        /* we want both X and Y to be printable */
        if(!isgraph((int)*(ptr - 1)) || !isgraph((int)*(ptr + 1))) {
                return 0;
        }
        /* the domain nor the TLD separator can't be the
           first nor the last character of the hostname */
        if(ptr == line || ptr == (line + (strlen(line) - 1))) {
                return 0;
        }
        return 1;
}

int isvalidipv6host(char *line)
{
#ifdef OER_DEBUG
        oer_debug(OER_DEBUG_INFO, "isvalidipv6host->%s\n", line);
#endif
        /* we require at least ::X */
        if(countchars(line, ':') < 2 || countnchars(line, ':') < 1) {
                return 0;
        }
        return 1;
}

int completeban(char *from, char *to, int tlength)
{
	int rc;
	char *ptr1;
	char *ptr2;
	char *ptr3;

	ptr1 = index(from, (int)'!');
	ptr2 = index(from, (int)'@');
	ptr3 = index(from, (int)'.');
	if(ptr1 == NULL && ptr2 == NULL) {
		/* nick ban */
		rc = snprintf(to, tlength, "%s!*@*", from);
	} else if(ptr1 == NULL && ptr2 != NULL) {
		/* ident@host */
		rc = snprintf(to, tlength, "*!%s", from);
	} else if(ptr1 == NULL && ptr2 == NULL && ptr3 != NULL) {
		/* host.xxx */
		rc = snprintf(to, tlength, "*!*@%s", from);
	} else if(ptr1 != NULL && ptr2 == NULL){
		/* nick!ident */
		rc = snprintf(to, tlength, "%s@*", from);
	} else {
		/* dont know */
		rc = snprintf(to, tlength, "%s", from);
	}
	return rc;
}

char *mybasename(char *ptr)
{
	int length;
	int counter;

	length = strlen(ptr);
	counter = length - 1;
	while(counter >= 0 && ptr[counter] != '/') {
		counter--;
	}
	if(counter >= 0) {
		return ptr + counter + 1;
	} else {
		return NULL;
	}
}

int countchars(char *line, char ch)
{
        int length;
        int counter;
        int matchcount;

        for(length = strlen(line), counter = 0, matchcount = 0; counter < length; counter++) {
                if(line[counter] == ch) {
                        matchcount++;
                }
        }
        return matchcount;
}

int countnchars(char *line, char ch)
{
        int length;
        int counter;
        int matchcount;

        for(length = strlen(line), counter = 0, matchcount = 0; counter < length; counter++) {
                if(line[counter] != ch) {
                        matchcount++;
                }
        }
        return matchcount;
}

void filteroutchanmodes(char *from, char *what, char *to, int to_length)
{
	char polar;
	char polaractive;
        int length;
        int i;
        int k;

        for(polar = 'x', polaractive = 'x', length = strlen(from), i = 0, k = 0; i < length && k < to_length; i++) {
		if(from[i] == '+' || from[i] == '-') {
			polar = from[i];
			if(polaractive != polar) {
				to[k++] = polar;
				polaractive = polar;
			}
		} else if(index(what, (int)from[i]) == NULL) {
			to[k++] = from[i];
		}
        }
	to[k] = '\0';
	/* strip possible polar at the end of the string */
	for(k = strlen(to) - 1; (k >= 0) && (to[k] == '+' || to[k] == '-'); k--) {
		to[k] = '\0';
	}
}
