#! /usr/bin/env python2.7

"""
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

"""
 constants
"""
ETH_OVERHEAD = 14
IP_OVERHEAD = 20
TCP_OVERHEAD = 32
UDP_OVERHEAD = 8
PRP_OVERHEAD = 6

"""
 global variables
"""
mtu = int(1500)
transmission_type = 'UDP'
prp_enabled = False
current_framesize = 64
sizetype = 'MIN'
target = '127.0.0.1'
targetmac = 'ff:ff:ff:ff:ff:ff'
interface = 'eth0'
count_frames = int(1)
unlimited_count = False
datafile = ''
srvmode = False
old_stdout = ''
portnumber = 52015
logfile = ''

"""
 Import scapy and other modules
 Check if run by root and exit if not
"""

try:
    import os, socket, sys, getopt, uuid, logging

    from socket import error as socket_error
    import errno

    logging.getLogger("scapy.runtime").setLevel(logging.ERROR)

    from scapy.all import *
except ImportError:
    print("Use python 2.7 or install scapy http://www.secdev.org/projects/scapy")
    sys.exit(0)

if not os.geteuid()==0:
    sys.exit("Only root can run this script")

"""
 functions
"""

def printheader():
    sys.stdout.write('#######################################\n shck - generate and send network load\n#######################################\n')

printheader()


"""
 function definitions
"""
def addpayload( element, payloadcontent ):
    if payloadcontent != '':
        element.payload=payloadcontent

def getpayloadfromfile( datafile ):
    data = ''
    with open(datafile, "rb") as f:
        byte = f.read(1)
        while (byte != "" and sys.getsizeof(data) <= 1500):
            # Do stuff with byte.
            data += byte
            if not byte:
                break
            byte = f.read(1)

    return data

def generateeth( data ):
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
    global current_framesize

    if transmission_type == 'ETH':
        size = int(current_framesize)-int(ETH_OVERHEAD)
    elif transmission_type == 'TCP':
        size = int(current_framesize)-int(IP_OVERHEAD)-int(TCP_OVERHEAD)-int(ETH_OVERHEAD)
    elif transmission_type == 'UDP':
        size = int(current_framesize)-int(IP_OVERHEAD)-int(UDP_OVERHEAD)-int(ETH_OVERHEAD)

    if prp_enabled == True:
        size -= PRP_OVERHEAD

    if size <= 0:
        return ''
    else:
        return data[0:int(size)]

def generate_package( data ):
    if transmission_type == 'ETH':
        if not ':' in str(target):
            logfile.close()
            sys.stdout = old_stdout
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
            logfile.close()
            sys.stdout = old_stdout
            sys.stdout.write('need an IP-address as destination when sending ' + str(transmission_type) + '-packets')
            sys.exit()
        return cutpayload(data)
    else:
        logfile.close()
        sys.stdout = old_stdout
        sys.stdout.write('\nERROR: Wrong transmission_type in generate_package()\n')
        sys.exit()

    
def sendpacketout( packet, data ):
    try:
        countend = int(count_frames)
            
        if transmission_type == 'ETH':
            #s = conf.L2socket(iface=interface)
            s = socket.socket(socket.PF_PACKET, socket.SOCK_RAW)
            s.bind((interface,0x2015))
    
        elif transmission_type == 'UDP':
            s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            s.connect((target, portnumber))
    
        elif transmission_type == 'TCP':
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.connect((target, portnumber))
    
        ss = StreamSocket(s)
    
        if sizetype == 'RANDOM':
            global current_framesize
            count = 0
            randframesizefile = open("random_framesizes.txt", "r")
            line = randframesizefile.readline()
            while (line or int(count)<int(count_frames)):
                if (int(count)==int(count_frames) and unlimited_count == False):
                    break
                if int(count)%99==0:
                    randframesizefile.seek(0)

                current_framesize = int(randframesizefile.readline())
                if int(current_framesize) > int(mtu):
                    current_framesize = int(mtu)
                packet = generate_package(data)

                bytecount = ss.send(Raw(packet))
                if (unlimited_count == False) and (int(bytecount) > int(0)):
                    count += int(1)
            randframesizefile.close()
        else:

            count = 0
            while int(count) < int(countend):
                bytecount = ss.send(Raw(packet))
                if int(bytecount) > int(0):
                    count += int(1)
        
            while unlimited_count == True:
                ss.send(Raw(packet))
        
        s.close()
    except socket_error as serr:
        if serr.errno != errno.ECONNREFUSED:
            raise serr
        else:
            logfile.close()
            sys.stdout = old_stdout
            print('\nERROR (Connection refused by target):\nshck needs to run in Server-Mode (-S) with -t ' + transmission_type + ' on the target host!\nTry again after starting shck in Server-Mode on the target.')
            sys.exit()

def sendpacket( data ):
    import time
    global old_stdout
    global logfile
    old_stdout = sys.stdout
    logfile = open('logfile.log', 'w')
    sys.stdout = logfile
    print('-shck------LOG-' + time.strftime("%Y%m%d-%H%M%S") + '-------')

    global current_framesize

    if sizetype == 'MIN':
        current_framesize = int(64)
        packet = generate_package(data)
        sendpacketout(packet, data)
    elif sizetype == 'MAX':
        current_framesize = int(mtu)
        packet = generate_package(data)
        sendpacketout(packet, data)
    elif sizetype == 'RANDOM':
        packet = generate_package(data)
        sendpacketout(packet, data)
            
    print('-------------------------------------------------')
    logfile.close()
    sys.stdout = old_stdout

def server():
    HOST = ''
    PORT = portnumber
    if transmission_type == 'TCP':
        while 1:
            try:
                s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
                s.bind((HOST, PORT))
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
        s.bind((HOST, PORT))
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
    #print sys.argv[0] + ' -s SIZETYPE -t TRANSMISSION_TYPE -m MTU -d DESTINATION -f FILE -i INTERFACE -n COUNT [-P]\n-s SIZETYPE: MIN | RANDOM | MAX\n-t TRANSMISSION_TYPE: ETH | TCP | UDP (default)\n-f FILE: File you want to send as payload (will only send the first x bytes of it)\n-d DESTINATION: MAC OR IP-destination-address (MAC if -t ETH, IP if -t TCP|UDP)\n-m MTU: set MTU (default 1500)\n-I INTERFACE (for example eth0): is needed when -t ETH\n-n COUNT: How many frames/packets you want to send (default 1)\n-P enable PRP (subtracts 6 bytes from payload (RCT))'

"""
 main
"""
def main(argv):

    try:
        opts, args = getopt.getopt(argv,"s:t:d:f:m:i:n:PSh")
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
            if (arg == 'MIN' or arg == 'RANDOM' or arg == 'MAX'):
                sizetype = arg
            else:
                print('No valid sizetype selected (MIN, RANDOM, MAX)')
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
        elif opt in ("-S"):
            global srvmode
            srvmode = True

    if (datafile == '' and srvmode == False):
        printhelp()
        sys.exit()
    elif (srvmode == True):
        print('PID of shck: ' + str(os.getpid()) + '\nRunning in SERVER MODE' + '\n-TRANSMISSION_TYPE: ' + str(transmission_type) + '\n\nListening...')
        server()

    else:
        print('PID of shck: ' + str(os.getpid()) + '\nLoad characteristics:\nDestination: ' + str(target) + '\n-SIZETYPE: ' + str(sizetype) + '\n-TRANSMISSION_TYPE: ' + str(transmission_type) + '\n-PRP-mode enabled: ' + str(prp_enabled) + '\n-Interface: ' + str(interface) + '\n-MTU: ' + str(mtu) + '\n-FILE: ' + str(datafile))
        if (unlimited_count == True):
            print('-Frame / Packet count: unlimited (Stop by pressing Ctrl+C)')
        else:
            print('-Frame / Packet count: ' + str(count_frames))
        print('\n\nSending load...')

        payload=getpayloadfromfile( datafile )
        sendpacket( payload )

        print('\nDone\nshck is finished\n')
    sys.exit()

try:
    main(sys.argv[1:])
except (KeyboardInterrupt, SystemExit):
    sys.stdout.write("\n######################################\n Goodbye!\n this traffic generation was brought to you by shck \n######################################\n")
    sys.exit()
except:
    raise
