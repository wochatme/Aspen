#!/usr/bin/perl
use strict;
use warnings;

if($#ARGV != 0) {
    print "Usage: p.pl filename\n";
    exit;
}

my $filename = $ARGV[0];
open(TXTFILE, '<', $filename)
or die "Could not open file '$filename' $!";

my $line;

while (<TXTFILE>) {
    chomp;
    $line = $_;
    $line =~ s/^\s+//;
    $line =~ s/\s+$//;

    print "idx = ($line & 0xFFFF);\t\t\t\tmsg_name[idx] = (char*)\"$line\";\n";
	
}

exit
