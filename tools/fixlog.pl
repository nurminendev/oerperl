#!/usr/bin/perl

# usage: fixlog.pl <last or seen> <filename>

# fixes oer's last/seen logs, result stored in <filename>.fixed

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
$fname = "$arg2.fixed";
open (fdout, ">$fname") || die "couldn't open $fname for writing\n";
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
    } else {
        printf fdout ($_);
    }
    $linecount++;
}
close (fdout);
close (fd);
