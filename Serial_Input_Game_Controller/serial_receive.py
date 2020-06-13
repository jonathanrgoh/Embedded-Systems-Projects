'''
Converts serial input from the ESP32 into keypresses for the arrow keys. 

Uses Python 3
To run: python3 serial_receive.py 

Required dependencies:
  serial:    pip3 install pyserial   | source: https://github.com/pyserial/pyserial
  pyautogui: pip3 install pyautogui  | source: https://github.com/asweigart/pyautogui

Notes on using PyAutoGui: 
  Key names can be found here: https://pyautogui.readthedocs.io/en/latest/keyboard.html

  Make sure your computer settings allow the command line program to control your computer

  To debug commnication logic, I recommend commenting out the 4 press() statements so your 
  computer doesn't register keyboard input and do weird stuff. 
'''

import sys
import serial
from pyautogui import press, hotkey

# pyautogui.PAUSE = 0 # uncomment if keypresses are too slow

encoding = 'utf-8'
ser = serial.Serial()
ser.baudrate = 115200
ser.port = 'COM4'   # check which port the ESP32 is using on your machine

def graceful_exit():
	print("Closing serial connection...")
	ser.close()
	print("quitting...")
	sys.exit()

def main():
	print("Starting serial connection...")
	ser.open()
	print("Serial connection started successfully!")

	while True:
		try: 
			received_str = ser.read(1).decode(encoding)
			# print(received_str)
			if received_str == "-":
				print("-")
			elif received_str == "u":
				press("up")
				print("up")
			elif received_str == "d":
				press("down")
				print("down")
			elif received_str == "l":
				press("left")
				print("left")
			elif received_str == "r":
				press("right")
				print("right")
			elif received_str == "p":
				press("space")
				print("space")
			elif received_str == "q":
				graceful_exit()

		except KeyboardInterrupt: # Ctrl-C detected
			graceful_exit()

	graceful_exit()

if __name__ == "__main__":
	main()


