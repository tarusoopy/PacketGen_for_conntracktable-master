UDPPacketGen_for_conntracktable
=================================================

UDP Packet Generator for many conntrack table.  
It can generate about 5000 packet/sec.

## make
````
gcc udppacketgen.c -o udppacketgen
````

## USAGE
````
udppacketgen [-i] --saddr_start <IP> [--saddr_end <IP>] [--sport_start <port>] [--sport_num <num>] --daddr_start <IP> [--daddr_end <IP>] [--dport_start <port>] [--dport_num <num>]
               -i : show acccess message (slow)
    --saddr_start : Start Address of source
    --saddr_end   : End Address of source (default : same saddr_start)
    --sport_start : Port number of source (default : 10000)
    --sport_num   : Use Number of source Port (default : 1)
    --daddr_start : Start Address of dource
    --daddr_end   : End Address of dource (default : same daddr_start)
    --dport_start : Port number of dource (default : 10000)
    --dport_num   : Use Number of dource Port (default : 1)
````

1. まず自身に使用するsource IPをすべて設定しておく。
2. 通信先にもdest IPをすべて設定しとく。
3. 設定したsource IPの範囲, source portの範囲, dest IPの範囲, dest portの範囲を指定してコマンド実行

## Sample
````
$ time ./udppacketgen  --saddr_start 192.168.101.60 --daddr_start 192.168.101.142 --dport_num 30000
finished : send 30000 packets

real    0m0.314s
user    0m0.012s
sys     0m0.299s
````

source IP : 192.168.101.60 1IP  
source port : 10000 1port (default値)  
dest IP : 192.168.101.142 1IP  
dest port : 10000 (default値) - 40000

で全30000パケットを送出

````
$ ./udppacketgen -i --saddr_start 10.1.0.10 --saddr_end 10.1.0.12 \
--sport_start 10000 --sport_num 2 \
--daddr_start 10.2.0.10 --daddr_end 10.2.0.13 \
--dport_start 20000 --dport_num 3
src 10.1.0.10:10000, dst 10.2.0.10:20000
src 10.1.0.10:10000, dst 10.2.0.10:20001
src 10.1.0.10:10000, dst 10.2.0.10:20002
src 10.1.0.10:10001, dst 10.2.0.10:20000
src 10.1.0.10:10001, dst 10.2.0.10:20001
src 10.1.0.10:10001, dst 10.2.0.10:20002
src 10.1.0.10:10000, dst 10.2.0.11:20000
src 10.1.0.10:10000, dst 10.2.0.11:20001
src 10.1.0.10:10000, dst 10.2.0.11:20002
src 10.1.0.10:10001, dst 10.2.0.11:20000
src 10.1.0.10:10001, dst 10.2.0.11:20001
src 10.1.0.10:10001, dst 10.2.0.11:20002
src 10.1.0.10:10000, dst 10.2.0.12:20000
src 10.1.0.10:10000, dst 10.2.0.12:20001
src 10.1.0.10:10000, dst 10.2.0.12:20002
src 10.1.0.10:10001, dst 10.2.0.12:20000
src 10.1.0.10:10001, dst 10.2.0.12:20001
src 10.1.0.10:10001, dst 10.2.0.12:20002
src 10.1.0.10:10000, dst 10.2.0.13:20000
src 10.1.0.10:10000, dst 10.2.0.13:20001
src 10.1.0.10:10000, dst 10.2.0.13:20002
src 10.1.0.10:10001, dst 10.2.0.13:20000
src 10.1.0.10:10001, dst 10.2.0.13:20001
src 10.1.0.10:10001, dst 10.2.0.13:20002
src 10.1.0.11:10000, dst 10.2.0.10:20000
src 10.1.0.11:10000, dst 10.2.0.10:20001
src 10.1.0.11:10000, dst 10.2.0.10:20002
src 10.1.0.11:10001, dst 10.2.0.10:20000
src 10.1.0.11:10001, dst 10.2.0.10:20001
src 10.1.0.11:10001, dst 10.2.0.10:20002
src 10.1.0.11:10000, dst 10.2.0.11:20000
src 10.1.0.11:10000, dst 10.2.0.11:20001
src 10.1.0.11:10000, dst 10.2.0.11:20002
src 10.1.0.11:10001, dst 10.2.0.11:20000
src 10.1.0.11:10001, dst 10.2.0.11:20001
src 10.1.0.11:10001, dst 10.2.0.11:20002
src 10.1.0.11:10000, dst 10.2.0.12:20000
src 10.1.0.11:10000, dst 10.2.0.12:20001
src 10.1.0.11:10000, dst 10.2.0.12:20002
src 10.1.0.11:10001, dst 10.2.0.12:20000
src 10.1.0.11:10001, dst 10.2.0.12:20001
src 10.1.0.11:10001, dst 10.2.0.12:20002
src 10.1.0.11:10000, dst 10.2.0.13:20000
src 10.1.0.11:10000, dst 10.2.0.13:20001
src 10.1.0.11:10000, dst 10.2.0.13:20002
src 10.1.0.11:10001, dst 10.2.0.13:20000
src 10.1.0.11:10001, dst 10.2.0.13:20001
src 10.1.0.11:10001, dst 10.2.0.13:20002
src 10.1.0.12:10000, dst 10.2.0.10:20000
src 10.1.0.12:10000, dst 10.2.0.10:20001
src 10.1.0.12:10000, dst 10.2.0.10:20002
src 10.1.0.12:10001, dst 10.2.0.10:20000
src 10.1.0.12:10001, dst 10.2.0.10:20001
src 10.1.0.12:10001, dst 10.2.0.10:20002
src 10.1.0.12:10000, dst 10.2.0.11:20000
src 10.1.0.12:10000, dst 10.2.0.11:20001
src 10.1.0.12:10000, dst 10.2.0.11:20002
src 10.1.0.12:10001, dst 10.2.0.11:20000
src 10.1.0.12:10001, dst 10.2.0.11:20001
src 10.1.0.12:10001, dst 10.2.0.11:20002
src 10.1.0.12:10000, dst 10.2.0.12:20000
src 10.1.0.12:10000, dst 10.2.0.12:20001
src 10.1.0.12:10000, dst 10.2.0.12:20002
src 10.1.0.12:10001, dst 10.2.0.12:20000
src 10.1.0.12:10001, dst 10.2.0.12:20001
src 10.1.0.12:10001, dst 10.2.0.12:20002
src 10.1.0.12:10000, dst 10.2.0.13:20000
src 10.1.0.12:10000, dst 10.2.0.13:20001
src 10.1.0.12:10000, dst 10.2.0.13:20002
src 10.1.0.12:10001, dst 10.2.0.13:20000
src 10.1.0.12:10001, dst 10.2.0.13:20001
src 10.1.0.12:10001, dst 10.2.0.13:20002
finished : send 72 packets
````
source IP : 10.1.0.10 - 10.1.0.12 (3IP)  
source port : 10000 - 10001 ( 2ports )   
dest IP : 10.1.0.10 10.1.0.13 ( 3IPs )  
dest port : 20000 - 20002 (3 ports)  
の計72パケットを送出

## TODO
自身にIPがなかった時の処理とかのエラー処理はあまり考慮していない
