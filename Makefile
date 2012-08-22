all: compile

compile:
	$(MAKE) -C lib/ all
	$(MAKE) -C Utils/ all
	$(MAKE) -C Tests/ all

clean:
	$(MAKE) -C lib/ clean
	$(MAKE) -C Utils/ clean
	$(MAKE) -C Tests/ clean
