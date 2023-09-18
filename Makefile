.PHONY : all clean

all :
	$(MAKE) -C common
	$(MAKE) -C platform
	$(MAKE) -C game

clean :
	$(MAKE) -C common clean
	$(MAKE) -C platform clean
	$(MAKE) -C game clean
	$(RM) -r bin/
