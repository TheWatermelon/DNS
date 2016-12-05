# DNS
A DNS implementation in C

Server side : 

make clean && make
while true; do
	ncat -ulvp 4567 -e ./main;
done


