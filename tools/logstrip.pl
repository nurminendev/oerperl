#!/usr/bin/perl

# usage: logstrip.pl <last or seen> <filename> <timestamp>

# extracts last/seen rows beginning from <timestamp>, result
# is stored in <filename>.<timestamp>

use POSIX qw (strftime);

$args = $#ARGV + 1;
if ($args != 3) {
    print "insufficient number of arguments.\n";
    exit;
}
$arg1 = @ARGV[0];
$arg2 = @ARGV[1];
$arg3 = @ARGV[2];
if($arg1 ne "last" && $arg1 ne "seen") {
    die "required argument missing.\n";
}
open (fd, "$arg2") || die "couldn't open $arg2 for reading\n";
$_ = $arg2;
$fname = "$arg2.$arg3";
open (fdout, ">$fname") || die "couldn't open $fname for writing\n";
$start_ts = localtime ($arg3);
printf ("stripping %s, starting from %s\n", $arg2, $start_ts);
$last_ts = $start_ts;
$linecount = 0;
while (<fd>) {
    chop;
    $corrupt = 1;
    if($arg1 eq "last") {
        if(/^(\d*)::(\S*)::(\S*)::(.*)$/) {
            $corrupt = 0;
            $keep = $_;
            $_ = $4;
            if(/(.*)::(.*)/) {
                $corrupt = 1;
            }
            $_ = $keep;
        }
	if($corrupt == 0) {
	    ($timestamp, $nick, $host, $message) = split(/::/, $_, 4);
# the ~0 has to be here for invalid timestamps
	    if($timestamp >= $arg3 && $timestamp < ~0) {
		printf fdout ("%s::%s::%s::%s\n", $timestamp, $nick, $host, $message);
		$linecount++;
	    }
	}
    }
    if($arg1 eq "seen") {
        if(/^(\d*)::(\S*)::(\S*)$/) {
            $corrupt = 0;
            $keep = $_;
            $_ = $3;
            if(/(.*)::(.*)/) {
                $corrupt = 1;
            }
            $_ = $keep;
        }
	if($corrupt == 0) {
	    ($timestamp, $nick, $host) = split(/::/, $_, 3);
# the ~0 has to be here for invalid timestamps
	    if($timestamp >= $arg3 && $timestamp < ~0) {
		printf fdout ("%s::%s::%s\n", $timestamp, $nick, $host);
		$linecount++;
	    }
	}
    }
}
if($linecount == 0) {
    print "no valid lines, sure you have the right input file?\n";
    close (fdout);
    unlink ($fname);
    exit;
}
$end_ts = localtime ($timestamp);
printf ("last log entry %s\n", $end_ts);
close (fdout);
close (fd);
