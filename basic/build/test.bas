code = alloc(32)

poke code,$EA
poke code+1,$88:poke code+2,$D0:poke code+3,$FD
poke code+4,$CA:poke code+5,$D0:poke code+6,$FA
poke code+7,$3A:poke code+8,$D0:poke code+9,$F7
poke code+10,$60

count = 2
a = 50:x = 0:y = 0

start = time()
for i = 1 to count:print i:sys code:next
tExec = (time()-start) / 100
innerCycles = 1280 * 256
tExec = tExec / count / a
print tExec;" secs per inner 2 loops"
cps = innerCycles / tExec
print cps/1000/1000;" Mhz clock"