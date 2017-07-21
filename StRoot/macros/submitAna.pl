#! /usr/bin/perl
use File::Basename;
use Cwd;
use Env;
my $macro = "readPicoDst.C";
my $mSplit  = @ARGV[0];
my $Production = "";
my $Prod = "P15ic";
my $STAR_LEVEL = "new";
my $File = "*.root";
my $NEvents = 1000000;
my $Dir = Cwd::cwd();
my $submit = "/scheduler/submit"; 
$Dir =~ s/$submit//;
print "my Dir: $Dir\n";

my %ARG = (files => '*.root',
	FilesPerJob => '40',
	);
while (@ARGV) {
    $_ = shift @ARGV;
    if ($_ =~ /=/) { my($key, $val) = /([^=]*)=(.*)/; $ARG{$key} = $val;}
}
while (my($key, $val) = each %ARG) {
    print  "$key=$val\n"; 
}

my $Simu = $Dir."/".$ARG{files};
my $FilesPerJob = $ARG{FilesPerJob};
my $OutDir = $Dir."/scheduler/output";
$glb = $Simu;
print "Macro : $macro\n";
print "glb=$glb\n";
my @Files = glob "$glb";
print "#Files : $#Files+1\n";
print "outDir : $OutDir\n";
print "FilesPerJob = $FilesPerJob\n";

my $submit = "submitAna_".$mEnergy."\.xml";
open XML,">$submit";
print XML '<?xml version="1.0" encoding="utf-8" ?> 
<job name="DiEAna" maxFilesPerProcess="1" filesPerHour="10" simulateSubmission="false" fileListSyntax="paths">
<command>csh -x $INPUTFILE0 </command>';
print XML "\n";
my $stdout = '<stdout URL="file:'.$Dir.'/scheduler/stdout/stdout_$JOBID.log" />'; 
my $stderr = '<stderr URL="file:'.$Dir.'/scheduler/stderr/stderr_$JOBID.log" />'; 
print XML "$stdout\n";
print XML "$stderr\n";
my $ctr = 0;
my $listctr = 0;
my $LISTNAME;
my $OutName ;
my $log     ;
my $csh     ;
foreach my $file (@Files){
    chomp $file;
    print "$file\n";
    if($ctr == 0 ) {
	$LISTNAME = $Dir."/scheduler/submit/job_$listctr.list";
	$OutName  = $OutDir."/job_$listctr.root";
	$log      = $OutDir."/job_$listctr.log";
	$csh      = $Dir."/scheduler/submit/job_$listctr.csh";
	print "Create list $LISTNAME !!\n";
	open LIST, ">$LISTNAME";
	$listctr += 1;
    }

    print LIST "$file\n";
    $ctr += 1;

    if($ctr >= $FilesPerJob) {
	open CSH ,">$csh";
	my $cmdline = "root.exe -b -l \'$macro(\"$LISTNAME\",\"$OutName\")\'>& $log";
	print "$cmdline\n";
	print CSH "#!/bin/tcsh \n";
	print CSH "starver $STAR_LEVEL\n";
	print CSH "cd $Dir\n";
	print CSH "if( ! -d $OutDir ) then\n";
	print CSH "mkdir -p $OutDir\n";
	print CSH "endif\n";
	print CSH "if( -e $log) then\n";
	print CSH "rm -f $log\n";
	print CSH "endif\n";
	print CSH "$cmdline\n";
	print XML "<input URL=\"file:$csh\"/>\n";
	print RUNNUM "@ss[0],";
	$ctr = 0;
    }
}
# write the last list
open CSH ,">$csh";
my $cmdline = "root.exe -b -l \'$macro(\"$LISTNAME\",\"$OutName\")\'>& $log";
print "$cmdline\n";
print CSH "#!/bin/tcsh \n";
print CSH "starver $STAR_LEVEL\n";
print CSH "cd $Dir\n";
print CSH "if( ! -d $OutDir ) then\n";
print CSH "mkdir -p $OutDir\n";
print CSH "endif\n";
print CSH "if( -e $log) then\n";
print CSH "rm -f $log\n";
print CSH "endif\n";
print CSH "$cmdline\n";
print XML "<input URL=\"file:$csh\"/>\n";
print RUNNUM "@ss[0],";

print XML "</job>";
print RUNNUM "};";
