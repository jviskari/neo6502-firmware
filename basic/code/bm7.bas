10 t1 = time()
11 print "Start"
12 dim m(5)
20 k=0
30 k=k+1
40 LET a=k/2*3+4-5
45 gosub 700
46 for l = 1 to 5
47 m(l) = a
48 next
50 IF k<5000 THEN GOTO 30
60 print "End"
70 print (time()-t1)/100/5:end
700 return
