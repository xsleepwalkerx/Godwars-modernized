#include <time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdarg.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "merc.h"

extern 		int		control, port;

bool    write_to_descriptor     args( ( int desc, char *txt, int length ) );
void attempt_copyover( void );
void crash_notice( int );

/* Edited by Aristoi (just removed the comment) */
void init_signals( void )
{
  pfile = FALSE;
//  pfile_name = str_dup("");

    signal(SIGPFILE,	crash_notice);
    signal(SIGFPE,	crash_notice);
    signal(SIGINT,	crash_notice);
    signal(SIGHUP,	crash_notice);
    signal(SIGSEGV,	crash_notice);
    signal(SIGTERM,	crash_notice);
    signal(SIGIO,	crash_notice);
    signal(SIGPIPE,	crash_notice);
    signal(SIGTRAP,	crash_notice);
return;
}
/* end aristoi */

void crash_notice( int sig )
{
 FILE *fp;

 fp =fopen("crash.txt", "a");

/* Original fprintf(fp, "Crash at %s system time.\n", mud_time); */
fprintf(fp, "Crash at %s", (char *) ctime( &current_time ) );
 fprintf(fp, "Last pfile was: %s..%s\n", pfile_name, pfile ? "loaded properly" : "BUGGED");
 fprintf(fp, "Last command: %s <BY> %s <%s>\n\n\n", last_user,last_command,command_finished ? 
"finished" : "BUGGED");
fflush(fp);
fclose(fp);
{
 DESCRIPTOR_DATA *d;
 DESCRIPTOR_DATA *d_next;

/* Attempt graceful copyover on crash for most signals.
 * Skip for SIGINT (clean shutdown) or SIGPFILE (pfile bug in loading). */
if (sig != SIGINT && sig != SIGPFILE)
{
    attempt_copyover();
    /* If attempt_copyover() returns, execl() failed — fall through to save+abort */
}

for (d = descriptor_list; d; d = d->next)
    if (d->character && d->connected >= CON_PLAYING) do_wake(d->character,
"");
for (d = descriptor_list; d; d = d->next)
    if (d->character && d->connected >= CON_PLAYING) do_call(d->character,
"all");
for (d = descriptor_list; d; d = d->next)
    if (d->character && d->connected >= CON_PLAYING) do_save(d->character,
"");

 for (d = descriptor_list; d; d = d_next)
 {
  d_next = d->next;

  if (d->character)
     stc("The Ethereal Mists is about to crash. Hold on tight.. \n\r", d->character);
  else
     write_to_descriptor(d->descriptor, "We are about to crash. Hold on..", 0);

if (pfile)
{sprintf(log_buf, "%s has a bugged pfile. He crashed the mud..\n\r",
	pfile_name);
 write_to_descriptor(d->descriptor, log_buf, 0);}

/*if (d->character)
{do_wake(d->character,"");do_call(d->character,"all");do_save(d->character,"");}*/

  close_socket(d);
} }
abort();
}

void attempt_copyover(void)
{
 DESCRIPTOR_DATA *d;
 FILE *fp;
 char buf[50], buf2[50];
	fp = fopen (COPYOVER_FILE, "w");
	if (!fp)
		return;

log_string("CRASH COPYOVER initiated..");
for (d = descriptor_list; d; d = d->next)
{
 CHAR_DATA *ch = d->original ? d->original : d->character;
 /* Null-check BEFORE using ch to avoid double-fault */
 if (!ch)
 {
     write_to_descriptor(d->descriptor, "We are crashing. Please reconnect.\n\r", 0);
     close_socket(d);
     continue;
 }
 save_char_obj(ch);
 fprintf(fp, "%d %s %s\n", d->descriptor, ch->name, d->host);
 write_to_descriptor(d->descriptor,
     "\n\r *** CRASH COPYOVER *** Attempting to restore your session...\n\r", 0);
}
        fprintf (fp, "-1\n");
	fflush(fp);
        fclose (fp);

sprintf(buf, "%d", port);
sprintf(buf2, "%d", control); 

execl(EXE_FILE, "merc", buf, "copyover", buf2, (char *) NULL);

for (d = descriptor_list; d; d = d->next)
 write_to_descriptor(d->descriptor, "COPYOVER FAILED! Attempting NORMAL boot!\n\r", 0);

return;
}

void crash_data( int sig )
{
 FILE *fp;

 fp =fopen("crash.txt", "a");

/* Original fprintf(fp, "Crash at %s system time.\n", mud_time); */
fprintf(fp, "Crash at %s", (char *) ctime( &current_time ) );
 fprintf(fp, "Last pfile was: %s..%s\n", pfile_name, pfile ? "loaded properly" : "BUGGED");
 fprintf(fp, "Last command: %s <BY> %s <%s>\n\n\n", last_user,last_command,command_finished ?
"finished" : "DATA");
fflush(fp);
fclose(fp);
}

