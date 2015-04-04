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
import os, sys, getopt

if not os.geteuid()==0:
    sys.exit("Only root can run this script")

try:
    from scapy.all import *
except ImportError:
    print("Use python 2.7 or install scapy http://www.secdev.org/projects/scapy")
    sys.exit(0)

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

def generatetcp( target, data ):
    ip=ippacket(target)
    tcp=tcpfragment(2014,2015)
    data=cutpayload(data)
    addpayload(tcp, data)
    return ip/tcp

def generateudp( target, data ):
    ip=ippacket(target)
    udp=udpdatagram(2014, 2015)
    data=cutpayload(data)
    addpayload(udp, data)
    return ip/udp

def cutpayload( data ):
    if transmission_type == 'ETH':
        size = current_framesize
    elif transmission_type == 'TCP':
        size = current_framesize-IP_OVERHEAD-TCP_OVERHEAD
    elif transmission_type == 'UDP':
        size = current_framesize-IP_OVERHEAD-UDP_OVERHEAD

    if prp_enabled == True:
        size -= PRP_OVERHEAD

    size -= ETH_OVERHEAD
    return data[0:size]

def generate_package( data ):
    if transmission_type == 'ETH':
        print('WIP')
    elif transmission_type == 'TCP':
        packet = generatetcp( target, data )
    elif transmission_type == 'UDP':
        packet = generateudp( target, data )

    return packet
    

def sendpacket( data ):
    global current_framesize

    if sizetype == 'MIN':
        current_framesize = 64
        packet = generate_package(data)
        srloop(packet)
        
    elif sizetype == 'RANDOM':
        with open("random_framesizes.txt", "r") as ins:
            for line in ins:
                current_framesize = int(line)
                packet = generate_package(data)
                sr(packet)

    elif sizetype == 'MAX':
        current_framesize = 1500
        packet = generate_package(data)
        srloop(packet)

def printhelp():
    print sys.argv[0] + ' -s SIZETYPE -t TRANSMISSION_TYPE -m MTU -d DESTINATION -f FILE [-P]\n-s SIZETYPE: MIN | RANDOM | MAX\n-t TRANSMISSION_TYPE: ETH | TCP | UDP (default)\n-f FILE: File you want to send as payload (will only send the first x bytes of it)\n-d DESTINATION: IP-destination-address\n-m MTU: set MTU (default 1500)\n-P enable PRP (subtracts 6 bytes from payload (RCT))'

"""
 main
"""
def main(argv):

    """
        default values
    """

    try:
        opts, args = getopt.getopt(argv,"s:t:d:f:m:Ph")
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
            datafile = arg
        elif opt in ("-d"):
            global target
            target = arg
        elif opt in ("-m"):
            global mtu
            mtu = arg
        elif opt in ("-P"):
            global prp_enabled
            prp_enabled = True

    payload=getpayloadfromfile( datafile )

    if transmission_type == 'ETH':
        sys.exit(1)

    sendpacket( payload )



main(sys.argv[1:])
