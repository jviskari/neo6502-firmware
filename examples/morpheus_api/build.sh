
ca65 crt0.s
ar65 a neo6502.lib crt0.o
cc65 -Oris --cpu 65c02 -T hello.c
cl65 -t none -C neo6502.cfg -Oris --cpu 65c02 -m hello.map \
     -o hello.neo hello.s neo6502.lib

# launch emulator
#test -f hello.neo && ../../bin/neo hello.neo@800 cold
