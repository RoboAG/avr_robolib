#!/usr/bin/env python3

###############################################################################
#                                                                             #
# bin/data.py                                                                 #
# ===========                                                                 #
#                                                                             #
# Version: 1.4.0                                                              #
# Date   : 11.02.22                                                           #
# Author : Peter Weissig                                                      #
#                                                                             #
# For help or bug report please visit:                                        #
#   https://github.com/RoboAG/avr_robolib                                     #
###############################################################################

import sys
import serial
import curses
import datetime
import time

# check for arguments
if (len(sys.argv) < 3):
    print("not enough arguments - usage:")
    print("python " + sys.argv[0] + " <serial port> <baudrate>")
    sys.exit()

comport = serial.Serial(port=sys.argv[1], baudrate=int(sys.argv[2]),
  timeout = None, parity = serial.PARITY_NONE,
  stopbits = serial.STOPBITS_TWO, xonxoff = False, bytesize = serial.EIGHTBITS,
  rtscts = False, dsrdtr = False)

signal_rts = False
signal_dtr = False
signal_cts = False
signal_dsr = False
comport.setRTS(signal_rts)
comport.setDTR(signal_dtr)

# flush possible data
comport.flushInput()

# save all communication in a file
now = datetime.datetime.now().strftime("%Y_%m_%d_%H%M")
f = open(now + ".txt","w")
# -1=nothing; 0=input; 1=output; 2=other
file_lastdata = -1

#init screen
stdscr = curses.initscr()
curses.noecho()
curses.cbreak()
stdscr.nodelay(1)

screen_max = stdscr.getmaxyx()
pos_in_y  = 2
pos_in_x  = 4
pos_out_y = (screen_max[0] // 2) + 2
pos_out_x = 4

caption = " Receiving from \"" + sys.argv[1] + "\" "
if (len(caption) < screen_max[1]):
    pos_x = 0
    while (pos_x < (screen_max[1] - len(caption)) // 2):
        stdscr.addstr(0,pos_x,"=")
        pos_x = pos_x + 1

    stdscr.addstr(0,pos_x,str(caption))
    pos_x = pos_x + len(caption)

    while (pos_x < screen_max[1]):
        stdscr.addstr(0,pos_x,"=")
        pos_x = pos_x + 1

caption = " Transmitting to \"" + sys.argv[1] + "\" "
if (len(caption) < screen_max[1]):
    pos_x = 0
    while (pos_x < (screen_max[1] - len(caption)) // 2):
        stdscr.addstr(screen_max[0] // 2,pos_x,"=")
        pos_x = pos_x + 1

    stdscr.addstr(screen_max[0] // 2,pos_x,str(caption))
    pos_x = pos_x + len(caption)

    while (pos_x < screen_max[1]):
        stdscr.addstr(screen_max[0] // 2,pos_x,"=")
        pos_x = pos_x + 1

stdscr.addstr(pos_in_y ,0,">>>")
stdscr.addstr(pos_out_y,0,"<<<")
stdscr.addstr(pos_out_y,pos_out_x,"")

stdscr.refresh()

skip_special_character = False
skip_ascii_13 = False
skip_ascii_10 = False
counter = 0

# endless loop
# Stop with STRG-C!
while 1:
    try:
        # receive data from comport
        input_count = comport.inWaiting()
        if (input_count > 0):
            input_data = comport.read()

            # filter characters
            out_str = ""
            for current_byte in input_data:
                new_skip_ascii_10 = False
                new_skip_ascii_13 = False
                if (current_byte == 13):
                    if (not skip_ascii_13):
                        out_str = out_str + chr(10)
                        new_skip_ascii_10 = True

                elif (current_byte == 10):
                    if (not skip_ascii_10):
                        out_str = out_str + chr(current_byte)
                        new_skip_ascii_13 = True

                elif (current_byte < 32):
                    out_str = out_str + '.'

                elif (current_byte > 255):
                    out_str = out_str + '.'

                else :
                    out_str = out_str + chr(current_byte)

                skip_ascii_10 = new_skip_ascii_10
                skip_ascii_13 = new_skip_ascii_13

            # print characters to screen
            for current_char in out_str:
                if (ord(current_char) == 10):
                    pos_in_x = screen_max[1]
                else:
                    stdscr.addstr(pos_in_y,pos_in_x, str(current_char))

                # next character is one position to the left
                pos_in_x = pos_in_x + 1

                # check for line jump
                if (pos_in_x >= screen_max[1]):
                    pos_in_x = 4
                    stdscr.addstr(pos_in_y,0, "    ")

                    # jump to next line
                    pos_in_y = pos_in_y + 1
                    # check for wrap around
                    if (pos_in_y >= (screen_max[0] // 2) - 1):
                        pos_in_y = 2

                    # clear the following line
                    pos_y = pos_in_y + 1
                    if (pos_y >= (screen_max[0] // 2) - 1):
                        pos_y = 2
                    pos_x = 0
                    while (pos_x < screen_max[1]):
                        stdscr.addstr(pos_y,pos_x, " ")
                        pos_x = pos_x + 1

                    stdscr.addstr(pos_in_y,0, ">>> ")

            stdscr.addstr(pos_out_y,pos_out_x, "")
            stdscr.refresh()

            # save characters in file
            for current_char in out_str:
                if (file_lastdata != 0):
                    if (file_lastdata != -1):
                        f.write("\n")
                    f.write("[IN ] ")
                    file_lastdata = 0

                f.write(current_char)
                if (ord(current_char) == 10):
                    f.write("      ")
            f.flush()

        # read data from input
        c = stdscr.getch()
        c = int(c)

        # filter characters
        if (((c == 10) or (c >= 32)) and
          (skip_special_character == False)):

            # send data
            comport.write([c])
            if (c == 13):
                # dummy
                j = 1

            elif (c == 10):
                pos_out_x = screen_max[1]

            elif (c < 32):
                stdscr.addstr(pos_out_y,pos_out_x, ".")
                pos_out_x = pos_out_x + 1

            else :
                stdscr.addstr(pos_out_y,pos_out_x, str(chr(c)))
                pos_out_x = pos_out_x + 1

            # check for line jump
            if (pos_out_x >= screen_max[1]):
                pos_out_x = 4
                stdscr.addstr(pos_out_y,0, "    ")

                # jump to next line
                pos_out_y = pos_out_y + 1
                # check for wrap around
                if (pos_out_y >= screen_max[0] - 1):
                    pos_out_y = screen_max[0] // 2 + 2

                # clear the following line
                pos_y = pos_out_y + 1
                if (pos_y         >= screen_max[0] - 1):
                    pos_y = screen_max[0] // 2 + 2
                pos_x = 0
                while (pos_x < screen_max[1]):
                    stdscr.addstr(pos_y,pos_x, " ")
                    pos_x = pos_x + 1

                stdscr.addstr(pos_out_y,0, "<<< ")

            stdscr.refresh()

            if (file_lastdata != 1):
                if (file_lastdata != -1):
                    f.write("\n")
                f.write("[OUT] ")
                file_lastdata = 1
            f.write(str(chr(c)))
            if (c == 10):
                f.write("      ")
            f.flush()
        elif (c < 0):
           skip_special_character = False
        elif (c == 27):
           skip_special_character = True
        elif (c == 4):
            signal_dtr = not signal_dtr
            comport.setDTR(signal_dtr)

            if (signal_dtr):
                stdscr.addstr(screen_max[0] // 2,screen_max[1] - 4, " DTR")
            else:
                stdscr.addstr(screen_max[0] // 2,screen_max[1] - 4, " dtr")
            stdscr.addstr(pos_out_y,pos_out_x, "")
            stdscr.refresh()

            if (file_lastdata != -1):
                f.write("\n")
            f.write("[DTR] ")
            file_lastdata = 2
            f.write(str(signal_dtr))
            f.flush()
        elif (c == 18):
            signal_rts = not signal_rts
            comport.setRTS(signal_rts)
            if (signal_rts):
                stdscr.addstr(screen_max[0] // 2,0, "RTS ")
            else:
                stdscr.addstr(screen_max[0] // 2,0, "rts ")
            stdscr.addstr(pos_out_y,pos_out_x, "")
            stdscr.refresh()

            if (file_lastdata != -1):
                f.write("\n")
            f.write("[RTS] ")
            file_lastdata = 2
            f.write(str(signal_rts))
            f.flush()

        current_signal_cts = comport.getCTS()
        if (current_signal_cts != signal_cts):
            signal_cts = current_signal_cts

            if (signal_cts):
                stdscr.addstr(0,0, "CTS ")
            else:
                stdscr.addstr(0,0, "cts ")
            stdscr.addstr(pos_out_y,pos_out_x, "")
            stdscr.refresh()

            if (file_lastdata != -1):
                f.write("\n")
            f.write("[CTS] ")
            file_lastdata = 2
            f.write(str(signal_cts))
            f.flush()

        current_signal_dsr = comport.getDSR()
        if (current_signal_dsr != signal_dsr):
            signal_dsr = current_signal_dsr

            if (signal_dsr):
                stdscr.addstr(0,screen_max[1] - 4, " DSR")
            else:
                stdscr.addstr(0,screen_max[1] - 4, " dsr")
            stdscr.addstr(pos_out_y,pos_out_x, "")
            stdscr.refresh()

            if (file_lastdata != -1):
                f.write("\n")
            f.write("[DSR] ")
            file_lastdata = 2
            f.write(str(signal_dsr))
            f.flush()

    except:
        curses.nocbreak(); curses.echo()
        curses.endwin()

        f.close()

        comport.close()

        sys.exit()
