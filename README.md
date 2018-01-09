# RelayMCU

Code for STC15F104W MCU which was installed in cheap ESP-01 relay module marked "LCTECH".
I bought few such modules from ebay but they had broken MCU firmware and not works. 
UART interaction code I got from STC15F100 docs
http://www.stcmicro.com/datasheet/STC15F100-en.pdf

# To compile this code under linux run: 

  sdcc -mmcs51 --iram-size 128 --xram-size 0 --code-size 4096  --nooverlay --noinduction --verbose --debug -V --std-sdcc89 --model-small   "relay.c"

# To upload code into MCU 

  stcgal -p /dev/ttyUSB1 -b 1200 -D -t 11059 relay.ihx

