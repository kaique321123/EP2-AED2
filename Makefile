debug: ep2d.exe

ep2d.exe: ep2def.c
	gcc ep2def.c -o ep2d.exe -g

clean:
	del *.exe