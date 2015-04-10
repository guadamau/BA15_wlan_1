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
def ippacket( ipdst ):
    ip=IP(dst=ipdst)
    return ip

def udpdatagram( sport, dport ):
    udp=UDP(sport=sport, dport=dport)
    return udp

def tcpfragment( sport, dport ):
    tcp=TCP(sport=sport, dport=dport)
    tcp.flags="PA"
    return tcp

def addpayload( element, payloadcontent ):
    #element.payload=payloadcontent
    print('addpayload() disabled')

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

def generatetcp( data ):
    ip=ippacket(target)
    tcp=tcpfragment(2014,portnumber)
    data=cutpayload(data)
    addpayload(tcp, data)
    return ip/tcp/data

def generateudp( data ):
    ip=ippacket(target)
    udp=udpdatagram(2014,portnumber)
    data=cutpayload(data)
    addpayload(udp, data)
    return ip/udp

def cutpayload( data ):
    global current_framesize

    if transmission_type == 'ETH':
        size = current_framesize-ETH_OVERHEAD
    elif transmission_type == 'TCP':
        size = current_framesize-IP_OVERHEAD-TCP_OVERHEAD-ETH_OVERHEAD
    elif transmission_type == 'UDP':
        size = current_framesize-IP_OVERHEAD-UDP_OVERHEAD-ETH_OVERHEAD
    size = int(22)

    if prp_enabled == True:
        size -= PRP_OVERHEAD

    return data[0:(size)]

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

    packet.len = int(current_framesize)

    return packet
    
def sendpacketout( packet, data ):
    try:
        countend = int(count_frames)
            
        if transmission_type == 'ETH':
            s = conf.L2socket(iface=interface)
    
        elif transmission_type == 'UDP':
            s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            s.connect((target, portnumber))
    
        elif transmission_type == 'TCP':
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.connect((target, portnumber))
    
        """
            ip=ippacket(target)
            tcpsyn=tcpfragment(3014,portnumber)
            tcpsyn.flags="S"
            tcpsynack=ss.sr1(ip/tcpsyn)
    
            tcpack=tcpfragment(2014,portnumber)
            tcpack.flags="A"
            tcpack.seq=tcpsynack[TCP].ack + 1
            tcpack.ack=tcpsynack[TCP].seq + 1
            ss.send(ip/tcpack)
        """
        ss = StreamSocket(s)
    
        if sizetype == 'RANDOM':
            global current_framesize
            count = 0
            randframesizefile = open("random_framesizes.txt", "r")
            line = randframesizefile.readline()
            while (line or int(count)<int(count_frames)):
                if int(count)==int(count_frames):
                    break
                if int(count)%99==0:
                    randframesizefile.seek(0)
                current_framesize = int(randframesizefile.readline())
                if int(current_framesize) > int(mtu):
                    current_framesize = int(mtu)
                packet = generate_package(data)
                ss.send(packet)
                if unlimited_count == False:
                    count += int(1)
            randframesizefile.close()
        else:
            for x in range(0, int(countend)):
                ss.send(packet)
        
            while unlimited_count == True:
                ss.send(packet)
        
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
        current_framesize = 64
        packet = generate_package(data)
        sendpacketout(packet, data)
    elif sizetype == 'MAX':
        current_framesize = mtu
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
                s.bind((HOST, PORT))
                s.listen(1)
                conn, addr = s.accept()
                print('\nConnected by ' + str(addr))
                count = 0
                while 1:
                    data = conn.recv(1024)
                    if not data: break
                    count += 1
                    print ('\rRecieved messages: ' + str(count) + ''),
                    conn.close()
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
            data, addr = s.recvfrom(1024)
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
        print('PID of shck: ' + str(os.getpid()) + '\nLoad characteristics:\nDestination: ' + str(target) + '\n-SIZETYPE: ' + str(sizetype) + '\n-TRANSMISSION_TYPE: ' + str(transmission_type) + '\n-PRP-mode enabled: ' + str(prp_enabled) + '\n-Interface: ' + str(interface) + '\n-MTU: ' + str(mtu) + '\n-FILE: ' + str(datafile) + '\n-Frame / Packet count: ' + str(count_frames) + '\n\nSending load...')

        payload=getpayloadfromfile( datafile )
        sendpacket( payload )

        print('\nDone\nshck is finished\n')
    sys.exit()

try:
    main(sys.argv[1:])
except (KeyboardInterrupt, SystemExit):
    sys.stdout.write("\n######################################\n Goodbye!\n Avoid the Gates of Hell. Use Linux. \n######################################\n")
    sys.exit()
except:
    raise
