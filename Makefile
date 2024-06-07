build: ep1.exe

ep2d.exe: EP2.c
	gcc EP2.c -o ep2.exe

clean:
	del *.exe