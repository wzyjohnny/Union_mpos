#!/usr/local/bin/perl
use CGI qw(:all);

use XML::Simple; 
use Data::Dumper;

my $cmd = "dir /s";

system($cmd);

my $config = $ARGV[0];
$config ="config";
my $backconfig = $config;
my $savefile;

print "\n配置文件名",$config;

if(!($config =~ /\.xml/))
	{
    	print"\r\n未输入后缀";
    	$config = $config.".xml";
	}
#open the file


my $file = 'config.xml';
my $xs1 = XML::Simple->new();

my $doc = $xs1->XMLin($file);
print Dumper($doc);
foreach my $key (keys (%{$doc->{Make}})){
   print $doc->{Make}->{$key}->{'cmd'} . ' (' . $key . ') ';
   print $doc->{Make}->{$key}->{'key'}->final . "\n";
}

#foreach $line <CONFIG_FILE>
#	print $line;
#read file line  finde the
#fine the 'ENTERY'

#find the 'cmd'

#find the main file to file the  app  version

#run  the  cmd

#read the bin and get make time&date&version

#packet  bin

#rename the NLP, XXX_date_time_version.nlp

#print header;
