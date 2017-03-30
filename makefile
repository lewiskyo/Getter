all: Getter So
.PHONY : all

Getter:
	make -f MakeGetter
So:
	make -f MakeSo

.PHONY : clean
clean : 
	make -f MakeGetter clean
	make -f MakeSo clean
