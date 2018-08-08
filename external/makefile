all: lib/liboled96.a include/oled96.h


lib/liboled96.a: oled_96/liboled96.a
	mkdir -p lib
	cp -r $< $@

include/oled96.h: oled_96/oled96.h
	mkdir -p include
	cp -r $< $@

oled_96/liboled96.a: oled_96/makefile
	$(MAKE) CC=${CC} ARCH=${ARCH} -C oled_96

oled_96/makefile:
	git clone git@github.com:yekovalyov/oled_96.git

clean:
	$(MAKE) -C oled_96 clean