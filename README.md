# RelayMCU

Code for STC15F104W MCU which was installed in cheap ESP-01 relay module marked "LCTECH".

![LCTECH ESP-01 based Module with STC15F100W MCU connected to USB-UART](?raw=true "Relay with STC15F104W MCU driven by ESP-01")

I bought few such modules from ebay but they had broken MCU firmware and not works. It must accept on UART commands (hex): A0 01 01 A2 for Relay ON and A0 01 00 A1 for Relay OFF

UART interaction code I got from STC15F100 docs
http://www.stcmicro.com/datasheet/STC15F100-en.pdf

# To compile this code under linux run: 

  sdcc -mmcs51 --iram-size 128 --xram-size 0 --code-size 4096  --nooverlay --noinduction --verbose --debug -V --std-sdcc89 --model-small   "relay.c"

# To upload code into MCU connect module to USB UART as shown on picture run this
 
  stcgal -p /dev/ttyUSB1 -b 1200 -D -t 11059 relay.ihx
  
  disconnect +5V (red wire on image) and connect it again for start flashing. 
  

