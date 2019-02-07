#!/usr/bin/env python

###############################################################################
#                                                                             #
# bin/data.py                                                                 #
# ===========                                                                 #
#                                                                             #
# Version: 1.3.3                                                              #
# Date   : 07.02.19                                                           #
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
    print "not enough arguments - usage:"
    print "python " + sys.argv[0] + " <serial port> <baudrate>"
    sys.exit()

comport = serial.Serial(port=sys.argv[1], baudrate=long(sys.argv[2]),
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
f = file(now + ".txt","w")
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
pos_out_y = (screen_max[0] / 2) + 2
pos_out_x = 4

tmp2 = " Receiving from \"" + sys.argv[1] + "\" "
if (len(tmp2) < screen_max[1]):
    pos_str = 0
    while (pos_str < (screen_max[1] - len(tmp2)) / 2):
        stdscr.addstr(0,pos_str,"=")
        pos_str = pos_str + 1

    stdscr.addstr(0,pos_str,str(tmp2))
    pos_str = pos_str + len(tmp2)

    while (pos_str < screen_max[1]):
        stdscr.addstr(0,pos_str,"=")
        pos_str = pos_str + 1

tmp2 = " Transmitting to \"" + sys.argv[1] + "\" "
if (len(tmp2) < screen_max[1]):
    pos_str = 0
    while (pos_str < (screen_max[1] - len(tmp2)) / 2):
        stdscr.addstr(screen_max[0] / 2,pos_str,"=")
        pos_str = pos_str + 1

    stdscr.addstr(screen_max[0] / 2,pos_str,str(tmp2))
    pos_str = pos_str + len(tmp2)

    while (pos_str < screen_max[1]):
        stdscr.addstr(screen_max[0] / 2,pos_str,"=")
        pos_str = pos_str + 1

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
        tmp = str(comport.read(comport.inWaiting()))

        #tmp = ""
        tmp2 = ""
        if (len(tmp) > 0):
            # filter characters
            pos_str = 0
            while (pos_str < len(tmp)):
                new_skip_ascii_10 = False
                new_skip_ascii_13 = False
                if (ord(tmp[pos_str]) == 13):
                    if (not skip_ascii_13):
                        tmp2 = tmp2 + chr(10)
                        new_skip_ascii_10 = True

                elif (ord(tmp[pos_str]) == 10):
                    if (not skip_ascii_10):
                        tmp2 = tmp2 + tmp[pos_str]
                        new_skip_ascii_13 = True

                elif (ord(tmp[pos_str]) < 32):
                    tmp2 = tmp2 + '.'

                elif (ord(tmp[pos_str]) > 255):
                    tmp2 = tmp2 + '.'

                else :
                    tmp2 = tmp2 + tmp[pos_str]

                pos_str = pos_str + 1
                skip_ascii_10 = new_skip_ascii_10
                skip_ascii_13 = new_skip_ascii_13

            # print characters to screen
            pos_str = 0
            while (pos_str < len(tmp2)):
                if (ord(tmp2[pos_str]) == 10):
                    pos_in_x = screen_max[1]
                else:
                    stdscr.addstr(pos_in_y,pos_in_x, str(tmp2[pos_str]))

                # next character is one position to the left
                pos_in_x = pos_in_x + 1

                # check for line jump
                if (pos_in_x >= screen_max[1]):
                    pos_in_x = 4
                    stdscr.addstr(pos_in_y,0, "    ")

                    # jump to next line
                    pos_in_y = pos_in_y + 1
                    # check for wrap around
                    if (pos_in_y >= (screen_max[0] / 2) - 1):
                        pos_in_y = 2

                    # clear the following line
                    pos_y = pos_in_y + 1
                    if (pos_y         >= (screen_max[0] / 2) - 1):
                        pos_y = 2
                    pos_x = 0
                    while (pos_x < screen_max[1]):
                        stdscr.addstr(pos_y,pos_x, " ")
                        pos_x = pos_x + 1

                    stdscr.addstr(pos_in_y,0, ">>> ")

                pos_str = pos_str + 1

            stdscr.addstr(pos_out_y,pos_out_x, "")
            stdscr.refresh()

            # save characters in file
            pos_str = 0
            while (pos_str < len(tmp2)):
                if (file_lastdata != 0):
                    if (file_lastdata != -1):
                        f.write("\n")
                    f.write("[IN ] ")
                    file_lastdata = 0

                f.write(tmp2[pos_str])
                if (ord(tmp2[pos_str]) == 10):
                    f.write("      ")

                pos_str = pos_str + 1
            f.flush()

        # read data from input
        c = stdscr.getch()
        c = int(c)

        # filter characters
        if (((c == 10) or (c >= 32)) and
          (skip_special_character == False)):

            # send data
            comport.write(str(chr(c)))
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
                    pos_out_y = screen_max[0] / 2 + 2

                # clear the following line
                pos_y = pos_out_y + 1
                if (pos_y         >= screen_max[0] - 1):
                    pos_y = screen_max[0] / 2 + 2
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
                stdscr.addstr(screen_max[0] / 2,screen_max[1] - 4, " DTR")
            else:
                stdscr.addstr(screen_max[0] / 2,screen_max[1] - 4, " dtr")
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
                stdscr.addstr(screen_max[0] / 2,0, "RTS ")
            else:
                stdscr.addstr(screen_max[0] / 2,0, "rts ")
            stdscr.addstr(pos_out_y,pos_out_x, "")
            stdscr.refresh()

            if (file_lastdata != -1):
                f.write("\n")
            f.write("[RTS] ")
            file_lastdata = 2
            f.write(str(signal_rts))
            f.flush()

        tmp = comport.getCTS()
        if (tmp != signal_cts):
            signal_cts = tmp

            if (tmp):
                stdscr.addstr(0,0, "CTS ")
            else:
                stdscr.addstr(0,0, "cts ")
            stdscr.addstr(pos_out_y,pos_out_x, "")
            stdscr.refresh()

            if (file_lastdata != -1):
                f.write("\n")
            f.write("[CTS] ")
            file_lastdata = 2
            f.write(str(tmp))
            f.flush()

        tmp = comport.getDSR()
        if (tmp != signal_dsr):
            signal_dsr = tmp

            if (tmp):
                stdscr.addstr(0,screen_max[1] - 4, " DSR")
            else:
                stdscr.addstr(0,screen_max[1] - 4, " dsr")
            stdscr.addstr(pos_out_y,pos_out_x, "")
            stdscr.refresh()

            if (file_lastdata != -1):
                f.write("\n")
            f.write("[DSR] ")
            file_lastdata = 2
            f.write(str(tmp))
            f.flush()

    except:
        curses.nocbreak(); curses.echo()
        curses.endwin()

        f.close()

        comport.close()

        sys.exit()
