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
TCP_OVERHEAD = 20
UDP_OVERHEAD = 8
PRP_OVERHEAD = 6

"""
 global variables
"""
mtu = 1500
transmission_type = 'UDP'
prp_enabled = False
current_framesize = 64
sizetype = 'MIN'
target = '127.0.0.1'
targetmac = 'ff:ff:ff:ff:ff:ff'
interface = 'eth0'
count_frames = 1
datafile = ''

"""
 functions
"""

def printheader():
    print("#######################################\n shck - generate and send network load\n#######################################\n")

printheader()

"""
 Import scapy and other modules
 Check if run by root and exit if not
"""

try:
    import os, sys, getopt, uuid, logging
    logging.getLogger("scapy.runtime").setLevel(logging.ERROR)
    from scapy.all import *
except ImportError:
    print("Use python 2.7 or install scapy http://www.secdev.org/projects/scapy")
    sys.exit(0)

if not os.geteuid()==0:
    sys.exit("Only root can run this script")

"""
 function definitions
"""
def ippacket( ipdst ):
    ip=IP(dst=ipdst)
    return ip

def udpdatagram( sport, dport ):
    udp=UDP(sport=sport, dport=dport)
    return udp

def tcpfragment( sport, dport ):
    tcp=TCP(sport=sport, dport=dport)
    return tcp

def addpayload( element, payloadcontent ):
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
    src=hex(uuid.getnode())
    srcmac = str(src[2])
    srcmac += str(src[3]) 
    srcmac += ':' + str(src[4]) 
    srcmac += str(src[5]) 
    srcmac += ':' + str(src[6]) 
    srcmac += str(src[7]) 
    srcmac += ':' + str(src[8]) 
    srcmac += str(src[9])
    srcmac += ':' + str(src[10]) 
    srcmac += str(src[11]) 
    srcmac += ':' + str(src[12]) 
    srcmac += str(src[13])
    eth=Ether(dst=target,src=srcmac,type=0x2015)
    data=cutpayload(data)
    return eth/data

def generatetcp( data ):
    ip=ippacket(target)
    tcp=tcpfragment(2014,2015)
    data=cutpayload(data)
    addpayload(tcp, data)
    return ip/tcp

def generateudp( data ):
    ip=ippacket(target)
    udp=udpdatagram(2014, 2015)
    data=cutpayload(data)
    addpayload(udp, data)
    return ip/udp

def cutpayload( data ):
    if transmission_type == 'ETH':
        size = current_framesize
    elif transmission_type == 'TCP':
        size = current_framesize-IP_OVERHEAD-TCP_OVERHEAD-ETH_OVERHEAD
    elif transmission_type == 'UDP':
        size = current_framesize-IP_OVERHEAD-UDP_OVERHEAD-ETH_OVERHEAD

    if prp_enabled == True:
        size -= PRP_OVERHEAD

    return data[0:size]

def generate_package( data ):
    if transmission_type == 'ETH':
        if not ':' in target:
            print('need a MAC-address as destination when sending Ethernet frames')
            sys.exit()
        else:
            packet = generateeth( data )
    elif transmission_type == 'TCP':
        if not '.' in target:
            print('need an IP-address as destination when sending TCP-packets')
            sys.exit()
        else:
            packet = generatetcp( data )
    elif transmission_type == 'UDP':
        if not '.' in target:
            print('need an IP-address as destination when sending UDP-packets')
            sys.exit()
        else:
            packet = generateudp( data )

    return packet
    
def sendpacketout( packet ):
    if sizetype == 'RANDOM':
        countend = 1
    else:
        countend = count_frames
        
    if transmission_type == 'ETH':
        for x in range(0, int(countend)):
            sendp(packet, iface=interface)
    elif transmission_type == 'TCP':
        for x in range(0, int(countend)):
            sr(packet, iface=interface)
    elif transmission_type == 'UDP':
        for x in range(0, int(countend)):
            sr(packet, iface=interface)

def sendpacket( data ):
    import time
    old_stdout = sys.stdout
    sys.stdout = open('log/shck-' + time.strftime("%Y%m%d-%H%M%S") + '.log', 'w')
    print('-shck------LOG-' + time.strftime("%Y%m%d-%H%M%S") + '-------')

    global current_framesize

    if sizetype == 'MIN':
        current_framesize = 64
        packet = generate_package(data)
        sendpacketout(packet)
    elif sizetype == 'MAX':
        current_framesize = mtu
        packet = generate_package(data)
        sendpacketout(packet)
    elif sizetype == 'RANDOM':
        count = 0
        randframesizefile = open("random_framesizes.txt", "r")
        line = randframesizefile.readline()
        while (line and int(count)<int(count_frames)):
            current_framesize = int(randframesizefile.readline())
            if int(current_framesize) > int(mtu):
                current_framesize = int(mtu)
            packet = generate_package(data)
            sendpacketout(packet)
            count += 1
            
    print('-------------------------------------------------')
    sys.stdout = old_stdout

def printhelp():
    from subprocess import call
    call(["less", "README"])
    #print sys.argv[0] + ' -s SIZETYPE -t TRANSMISSION_TYPE -m MTU -d DESTINATION -f FILE -i INTERFACE -n COUNT [-P]\n-s SIZETYPE: MIN | RANDOM | MAX\n-t TRANSMISSION_TYPE: ETH | TCP | UDP (default)\n-f FILE: File you want to send as payload (will only send the first x bytes of it)\n-d DESTINATION: MAC OR IP-destination-address (MAC if -t ETH, IP if -t TCP|UDP)\n-m MTU: set MTU (default 1500)\n-I INTERFACE (for example eth0): is needed when -t ETH\n-n COUNT: How many frames/packets you want to send (default 1)\n-P enable PRP (subtracts 6 bytes from payload (RCT))'

"""
 main
"""
def main(argv):

    try:
        opts, args = getopt.getopt(argv,"s:t:d:f:m:i:n:Ph")
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
            count_frames = arg
        elif opt in ("-P"):
            global prp_enabled
            prp_enabled = True

    if datafile == '':
        printhelp()
        sys.exit()

    print('Load characteristics:\nDestination: ' + str(target) + '\n-SIZETYPE: ' + str(sizetype) + '\n-TRANSMISSION_TYPE: ' + str(transmission_type) + '\n-PRP-mode enabled: ' + str(prp_enabled) + '\n-Interface: ' + str(interface) + '\n-MTU: ' + str(mtu) + '\n-FILE: ' + str(datafile) + '\n-Frame / Packet count: ' + str(count_frames) + '\n\nSending load...')

    payload=getpayloadfromfile( datafile )
    sendpacket( payload )

main(sys.argv[1:])
