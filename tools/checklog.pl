#!/usr/bin/perl

# usage: checklog.pl <last or seen> <filename>

# checks oer's last/seen logs for bad rows

use POSIX qw (strftime);

$args = $#ARGV + 1;
if($args != 2) {
    die "insufficient number of arguments.\n";
}
$arg1 = @ARGV[0];
$arg2 = @ARGV[1];
if($arg1 ne "last" && $arg1 ne "seen") {
    die "required argument missing.\n";
}
open (fd, "$arg2") || die "couldn't open $arg2 for reading\n";
$linecount = 0;
while (<fd>) {
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
    }
    if($corrupt == 1) {
        print "$_";
        print "\tis corrupt ($linecount)\n";
    }
    $linecount++;
}
close (fd);
