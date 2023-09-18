.PHONY : all clean

all :
	$(MAKE) -C platform
	$(MAKE) -C game

clean :
	$(MAKE) -C platform clean
	$(MAKE) -C game clean
	$(RM) -r bin/
