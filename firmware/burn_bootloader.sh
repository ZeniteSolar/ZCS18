
avrdude avrdude.conf -v -patmega328pb -cusbasp -Pusb -e -Ulock:w:0xFF:m -Uefuse:w:0xFD:m -Uhfuse:w:0xDE:m -Ulfuse:w:0xFF:m 

