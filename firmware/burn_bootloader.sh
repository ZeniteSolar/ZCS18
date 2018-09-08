
avrdude -Cavrdude.conf -v -patmega328pb -cusbasp -Pusb -Uflash:w:optiboot_m328pb.hex:i -Ulock:w:0xCF:m
