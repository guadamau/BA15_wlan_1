#! /usr/bin/env python2.7

"""shck - network generator script
==============================================================================
 BA15_wlan_1
=============================================================================

*****************************************************************************
  Software:   shck Python Script
  Authors:    guadamau@students.zhaw.ch | leibupro@students.zhaw.ch
  Date:       03.2015

  Purpose:    Generate and send specific network load

  Desc:       Creates specific load described in the german thesis
              "Ermittlung der Performance von Netzwerkfunktionen am Beispiel
              von PRP" by Mauro Guadagnini and Prosper Leibundgut.
             
              Uses scapy (http://www.secdev.org/projects/scapy/index.html)

*****************************************************************************
"""

#
#  constants
#
ETH_OVERHEAD = 14
IP_OVERHEAD = 20
TCP_OVERHEAD = 32
UDP_OVERHEAD = 8
PRP_OVERHEAD = 6

#
# global variables
#
sizetype = 'MIN'                    # set default sizetype to MIN
payload_size = 46               # set default framesize to 46 bytes (MIN sizetype)

target = '127.0.0.1'                # set default IP-taget to localhost
interface = 'eth0'                  # set default network interface to eth0
mtu = int(1500)                     # set default mtu to 1500 bytes
                                    # (Maximum frame size decrement by 6 bytes)
transmission_type = 'UDP'           # set default transmission type to UDP
prp_enabled = False                 # disable PRP-support 

count_frames = int(1)               # send 1 frame by default
unlimited_count = False             # don't send unlimited count of frames per default

srvmode = False                     # start per default in client mode

portnumber_tcp = int(52015)             # set default portnumber for TCP to 52015
portnumber_udp = int(52014)             # set default portnumber for UDP to 52015

datafile = ''                       # global variable for datafile (payload of packets)

#
# Import scapy and other modules
# Check if run by root and exit if not
#
try:
    import os, socket, sys, getopt, uuid

    from socket import error as socket_error
    import errno

    from scapy.all import *

except ImportError:
    print("Use python 2.7 or install scapy http://www.secdev.org/projects/scapy")
    sys.exit(0)

if not os.geteuid()==0:
    sys.exit("ERROR:\nOnly root can run this script\nUse sudo or run as root")

#
# functions
#
def printheader():
    sys.stdout.write('#######################################\n shck - generate and send network load\n#######################################\n')

printheader()


#
# function definitions
#
def addpayload( element, payloadcontent ):
    """Adds data to payload of a specific element
    """
    if payloadcontent != '':
        element.payload=payloadcontent

def getpayloadfromfile( datafile ):
    """returns the first 1500 bytes of a specific file
    """
    data = ''
    with open(datafile, "rb") as f:
        byte = f.read(1)
        while (byte != "" and sys.getsizeof(data) <= 3000):
            # Do stuff with byte.
            data += byte
            if not byte:
                break
            byte = f.read(1)

    return data

def generateeth( data ):
    """Generates an ethernet frame (Type value is 0x2015)

    Parses the MAC-address of the NIC and writes it as source
    into the ethernet frame.

    Writes target MAC-address (needs to have the format "XX:XX:XX:XX:XX:XX") 
    as destination into the ethernet frame.

    Adds prepared payload (by cutpayload(data)) in the needed size to the frame.
    """
    src=str(hex(uuid.getnode()))
    srcmac = ''
    if str(src[1])=='x':
        src = src[2:]
        srcmac += '00:'
    srcmac += str(src[0])
    srcmac += str(src[1]) 
    srcmac += ':' + str(src[2]) 
    srcmac += str(src[3]) 
    srcmac += ':' + str(src[4]) 
    srcmac += str(src[5]) 
    srcmac += ':' + str(src[6]) 
    srcmac += str(src[7])
    srcmac += ':' + str(src[8]) 
    srcmac += str(src[9]) 
    eth=Ether(dst=target,src=srcmac,type=0x2015)
    data=cutpayload(data)
    addpayload(eth, data)
    return eth

def cutpayload( data ):
    """Returns the first x bytes from the content of the argument variable

    The size of the return value matches the payload of an ethernet frame or
    TCP- or UDP-packet so that the final packet/frame will have the needed size.
    """
    global payload_size

    if transmission_type == 'ETH':
        size = int(payload_size)
    elif transmission_type == 'TCP':
        size = int(payload_size)-int(IP_OVERHEAD)-int(TCP_OVERHEAD)-int(ETH_OVERHEAD)
    elif transmission_type == 'UDP':
        size = int(payload_size)-int(IP_OVERHEAD)-int(UDP_OVERHEAD)

    if size <= 0:
        return ''
    else:
        return data[0:int(size)]

def calcsizeofrandompayload(current_randomsize):
    if transmission_type == 'ETH':
        size = int(current_randomsize)
    elif transmission_type == 'TCP':
        size = int(current_randomsize)-int(IP_OVERHEAD)-int(TCP_OVERHEAD)-int(ETH_OVERHEAD)
    elif transmission_type == 'UDP':
        size = int(current_randomsize)-int(IP_OVERHEAD)-int(UDP_OVERHEAD)-int(ETH_OVERHEAD)
    
    if size <= 0:
        return 0
    else:
        return int(size)

def file_len(fname):
    with open("custom_framesizes.txt", "r") as f:
        for i, l in enumerate(f):
            pass
    return i + 1

def generate_package( data ):
    """returns the final content that needs to be send by the sockets.

    The TCP- and UDP-socket wrap all needed headers around the return value,
    whereas the RAW-socket needs to have the complete ethernet frame.

    Also checks if the destination argument matches 
    the specified transmission type (MAC for Ethernet / IP for TCP and UDP)
    """
    if transmission_type == 'ETH':
        if not ':' in str(target):
            sys.stdout.write('need a MAC-address as destination when sending Ethernet frames')
            sys.exit()
        else:
            packet = generateeth( data )
            return str(packet)
    elif (transmission_type == 'TCP' or transmission_type == 'UDP'):
        if (sizetype == 'MIN' and transmission_type == 'TCP'):
        # Empty TCP-packet is 66 bytes
        # Cant send an empty packet -> Payload 1 Byte
            return "\0"
        if not '.' in str(target):
            sys.stdout.write('need an IP-address as destination when sending ' + str(transmission_type) + '-packets')
            sys.exit()
        return cutpayload(data)
    else:
        sys.stdout.write('\nERROR: Wrong transmission_type in generate_package()\n')
        sys.exit()

    
def sendpacketout( packet, data ):
    """Creates the needed socket and sends the prepared data x times (specified per -n parameter) over it

    For the CUSTOM-sizetype, the function reads a prepared textfile which contains a specific
    size in bytes per line, prepares and sends the frame/packet per line.
    """
    try:
        countend = int(count_frames)
            
        if transmission_type == 'ETH':
            s = socket.socket(socket.PF_PACKET, socket.SOCK_RAW)
            s.bind((interface,0x2015))
    
        elif transmission_type == 'UDP':
            s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            s.connect((target, portnumber_udp))
    
        elif transmission_type == 'TCP':
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.connect((target, portnumber_tcp))
    
        ss = StreamSocket(s)
        sendpacketonsocket = ss.send
    
        if sizetype == 'CUSTOM':
            global payload_size
            count = 0
            packet = generate_package(data)
            packet = str(packet)
            if unlimited_count == False:
                frametogen = count_frames
            else:
                frametogen = 1000000

            filelength = file_len("custom_framesizes.txt")
            with open("custom_framesizes.txt", "r") as randomsizes:
                head = [next(randomsizes) for x in xrange(0,filelength)]
            cursor = 0
            while (count < count_frames):

                if (count==count_frames and unlimited_count == False):
                    break

                if (int(cursor)%frametogen==0 or int(cursor)%(len(head))==0):
                    cursor = 0

                if int(head[cursor]) > (mtu+ETH_OVERHEAD):
                    head[cursor] = (mtu+ETH_OVERHEAD)
                packettosend = Raw(packet[0:calcsizeofrandompayload(head[cursor])])
                packettosendlength = len(packettosend)
                bytecount = sendpacketonsocket(packettosend)
                if (unlimited_count == False) and (bytecount == packettosendlength):
                    count += 1
                    cursor += 1
                elif (unlimited_count == True) and (bytecount == packettosendlength):
                    cursor += 1
        else:
            count = 0
            packet = Raw(packet)
            while count < countend:
                bytecount = sendpacketonsocket(packet)
                if int(bytecount) == len(packet):
                    count += 1
        
            while unlimited_count == True:
                ss.send(packet)
        
        s.close()
    except socket_error as serr:
        if serr.errno != errno.ECONNREFUSED:
            raise serr
        else:
            print('\nERROR (Connection refused by target):\nshck needs to run in Server-Mode (-S) with -t ' + transmission_type + '\nand on the same port (-p PORTNUMBER) on the target host!\nTry again after starting shck in Server-Mode on the target.')
            sys.exit()

def sendpacket( data ):
    """starts the sending-method (sendpacketout())
    """
    import time

    global payload_size

    if sizetype == 'MIN':
        payload_size = int(46)
        packet = generate_package(data)
        sendpacketout(packet, data)
    elif sizetype == 'MAX':
        payload_size = int(mtu)
        if prp_enabled == True and mtu == 1500:
            payload_size -= PRP_OVERHEAD
        packet = generate_package(data)
        sendpacketout(packet, data)
    elif sizetype == 'CUSTOM':
        payload_size = int(mtu)
        if prp_enabled == True:
            payload_size -= PRP_OVERHEAD
        packet = generate_package(data)
        sendpacketout(packet, data)

def server():
    """server-mode: listens on the specified portnumber (TCP or UDP)
    """
    HOST = ''
    PORT_UDP = portnumber_udp
    PORT_TCP = portnumber_tcp
    if transmission_type == 'TCP':
        while 1:
            try:
                s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
                s.bind((HOST, PORT_TCP))
                s.listen(1)
                conn, addr = s.accept()
                print('\nConnected by ' + str(addr))
                count = 0
                while 1:
                    data = conn.recv(4096)
                    if not data: break
                    count += 1
                    print ('\rRecieved messages: ' + str(count) + ''),
                    #conn.close()
                    s.close()
            except socket_error as serr:
                if serr.errno !=  errno.ECONNRESET:
                    raise serr
                else:
                    print('\nConnection reset by peer, ready for new connection...')
    elif transmission_type == 'UDP':
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.bind((HOST, PORT_UDP))
        count = 0
        while 1:
            count += int(1)
            data, addr = s.recvfrom(4096)
            print ('\rRecieved messages: ' + str(count) + ''),
    else:
        print('\nINVALID TRANSMISSION_TYPE! Choose TCP or UDP!\n')
    

def printhelp():
    from subprocess import call
    call(["less", "README"])

#
# main
#
def main(argv):
    """Parse arguments
    """

    try:
        opts, args = getopt.getopt(argv,"s:t:d:f:m:i:n:p:PSh")
    except getopt.GetoptError as err:
        print(err)
        printhelp()
        sys.exit(2)

    for opt, arg in opts:
        if opt == '-h':
            printhelp()
            sys.exit()
        elif opt in ("-s"):
            global sizetype
            if (arg == 'MIN' or arg == 'CUSTOM' or arg == 'MAX'):
                sizetype = arg
            else:
                print('No valid sizetype selected (MIN, CUSTOM, MAX)')
                sys.exit()
        elif opt in ("-t"):
            global transmission_type
            if (arg == 'ETH' or arg == 'TCP' or arg == 'UDP'):
                transmission_type = arg
            else:
                print('No valid transmission type selected (ETH, TCP, UDP)')
                sys.exit()
        elif opt in ("-f"):
            global datafile
            datafile = arg
        elif opt in ("-d"):
            global target
            target = arg
        elif opt in ("-m"):
            global mtu
            mtu = int(arg)
        elif opt in ("-i"):
            global interface
            interface = arg
        elif opt in ("-n"):
            global count_frames
            global unlimited_count
            if int(arg) == 0:
                unlimited_count = True
            else:
                count_frames = int(arg)
        elif opt in ("-P"):
            global prp_enabled
            prp_enabled = True
        elif opt in ("-p"):
            global portnumber_udp
            global portnumber_tcp
            portnumber_udp = int(arg)
            portnumber_tcp = int(arg)
        elif opt in ("-S"):
            global srvmode
            srvmode = True

    if (datafile == '' and srvmode == False):   # display help if invalid parameters are set
        printhelp()
        sys.exit()
    elif (srvmode == True):
        print('PID of shck: ' + str(os.getpid()) + '\nRunning in SERVER MODE' + '\n-TRANSMISSION_TYPE: ' + str(transmission_type) + '\n\nListening...')
        server()                                # start server-mode

    else:
        print('PID of shck: ' + str(os.getpid()) + '\nLoad characteristics:\nDestination: ' + str(target) + '\n-SIZETYPE: ' + str(sizetype) + '\n-TRANSMISSION_TYPE: ' + str(transmission_type) + '\n-PRP-mode enabled: ' + str(prp_enabled) + '\n-Interface: ' + str(interface) + '\n-FILE: ' + str(datafile))
        if (unlimited_count == True):
            print('-Frame / Packet count: unlimited (Stop by pressing Ctrl+C)')
        else:
            print('-Frame / Packet count: ' + str(count_frames))
        print('\n\nSending load...')

        payload=getpayloadfromfile( datafile )  # get payload from file
        sendpacket( payload )                   # send frames with specified payload

        print('\nDone\nshck is finished\n')
    sys.exit()

#
# start main and catch KeyboardInterrupt and sys.exit()
#
try:
    main(sys.argv[1:])
except (KeyboardInterrupt, SystemExit):
    sys.stdout.write("\n######################################\n Goodbye!\n this traffic generation was brought to you by shck \n######################################\n")
    sys.exit()
except:
    raise
