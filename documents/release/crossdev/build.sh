#
#		Where executables are
#
BINDIR=../../../bin
#
#		Convert source.bsc to a tokenised program
#
python3 $BINDIR/makebasic.zip $1.bsc -o$1.bas
#
#		Run it in the emulator. Loads program.bas in to 'page' (where program code goes)
#		Then warm starts the interpreter. You can use exec to auto run it.
#
$BINDIR/neo	$1.bas@page warm
