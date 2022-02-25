# TCP-socket-programming-in-C-
# UDP-Socket-Programming-in-C-
Untuk compile dan run listener :  - gcc -o tserver tserver.c
                                  - ./tserver
Untuk compile dan run talker   :  - gcc -o tclient tclient.c
                                  - ./tclient [IP] [Pesan yang akan dikirim]
                                  - contoh : .tclient 192.168.1.1 "Hallo"
Buka aplikasi wireshark, untuk menganalisa paket yang mampu diterima tserver.
