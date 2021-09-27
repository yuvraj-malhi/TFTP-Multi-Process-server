# TFTP Server (Multi process)
## Description
TFTP stands for **Trivial File Transfer Protocol**. This is an aplication level protocol initially developed in the late 90s to be a simple file transfer protocol as suggested by the name. The basic task of TFTP is similar to any other File Transfer Protocol: transfer files from one device on a network to another. This file may be a _JPEG, PDF, EXE, ZIP, TXT_, or any other file; the work remains the same. 

Here are some features that make TFTP useful and noteworthy:    
  1. __Lockstep protocol__    - It is fault tolerant and can correct mistakes committed on the network   
  2. __Light weight__         - Hence, it does not require huge client or server computational power   
  3. __Uses UDP, hence fast__ - A unique exception in an otherwise TCP dominated protcol environment  

To read more on the details TFTP, visit *http://www.ietf.org/rfc/rfc1350.txt*. This documentation contains the details of the algorithm that is implemnted in all the latest TFTP clients and servers.

TFTP's importance today can be seen by the simple fact that port 69 is reserved for it by IANA. Only ports 1-1024 are reserved for the most common and useful applications like HTTP, SSH etc.

How does this project help TFTP?
In a world of exceptionally complex codes that can only be used directly and not really understood, there is a need for simplicity in code, implementation, and usage.

Keeping this in mind, this TFTP server has the following properties:    
  1. __Concurrent__     - This server can handle multiple clients at the same time without any hastle    
  2. __Muti process__   - A multi process server that can handle multiple clients with great speed.   
  3. __Simplicity__     - Easily understandable code that uses variables, functions and loops in a justified manner     
  4. __Speed__          - Functions that currently take hundereds of lines have been optimised to do the same task in very few lines    
  5. __Intuitive__      - Variable & function names are made to be very intuitive so as to eliminate confusion   
  6. __Verbose output__ - The code has an option whether you want to print all the details of transfer, or simply just the speed.


### Concurrency
This is a concurrent server because it creates a new process or each new client that wants to interact with the server.
A multiprocess server would create a new child process or a new thread (in the case for multi-threaded concurrency) for each new client. 

In the below picture, say if a new "Client C" were to connect, then another server child process (or thread) would be created.

![1](https://user-images.githubusercontent.com/76866159/107152058-e21f3a00-698b-11eb-9cd9-61489dbdca16.png)

Advantage - **Speed**. For upto a limited number of clients (this number depends on your CPU's core specifications), the speed of multi process servers outperforms all.  
Disadvantage - As the number of child processes (or threads) increase, the server becomes **exponentially slow.**  



## Usage

### Server Side

#### STEP I - Note Server IP
Run  to find the server IP.

![image](https://user-images.githubusercontent.com/76866159/134906479-7b241fd5-b8ac-4ae2-91cc-c172ef893668.png)
As shown, in my case the IP is ```10.0.2.12``` 


#### STEP II - Initialize server
Run the following commands to get started: 

```sudo ifconfig``` - 
``` sudo git clone https://github.com/yuvrajmalhi/TFTP-Multi-Process-server.git```     
```cd TFTP-Single-Process-server/```    
```cp <file location> ./``` 
```gcc tftp.c -o server```      
```ls```     
```sudo ./server 69```        

[Note: if you are a networking geek, you may change the 69 to any other port number, but keep in mind that all TFTP clients connect to port 69 by default]

On the prompt, the program asks if you want to print everything that goes on in the file transfer or just the basics.
I will recomment choosing no or 'n' as this information will be irrelevant to all other than networking folks.

Here's how:
![Server_Multiproc](https://user-images.githubusercontent.com/76866159/134904153-60cb5b49-6125-463c-98d4-ba3faf17c838.gif)

Voila! Server is all setup and is listening.



### Client side
Client machine can be any machine on the same network (wifi or ethernet or hotspot) as the server. 
For the sake of demonstration, the client may be on the server machine also. That's what I will do.

#### STEP I
Kali users may skip this step.
For all others, check if you have a TFTP client. To to this, run the command:
```sudo tftp```


If an error is shown then you have to install it on your system. So run the command:
``` sudo apt install tftp```

After installing, run the command:
``` sudo tftp```


![image](https://user-images.githubusercontent.com/76866159/106705722-7d26b580-6614-11eb-8adb-bf54083e01de.png)


#### STEP II
Connect to ther server using the server's IP that we noted in Step I of Server side Usage
> As the picture, in my case the IP is 192.168.1.106

So to connect to the server IP, run the command:
``` connect 192.168.1.106```

Replace 192.168.1.106 with the server IP in you case as it may not be the same.

![image](https://user-images.githubusercontent.com/76866159/106705874-c2e37e00-6614-11eb-9967-d6b9df9e0c79.png)


#### STEP III
Now, we get one of the files that had been stores in the server in Step III of server's usage.
> I have copied the debian file "TT.deb"

``` get TT.deb```
Replace TT.deb6 with the filename that you had stored.

#### Wait and watch!
The transfer will start.

![image](https://user-images.githubusercontent.com/76866159/106706325-882e1580-6615-11eb-9a51-11c4f6250dea.png)


As soon as the transfer ends, a prompt is shown telling you the time.
Morover, on the **Server Side** the speed is also shown.

![image](https://user-images.githubusercontent.com/76866159/106706394-a136c680-6615-11eb-984c-36c3c5bc4ba4.png)


## Demonstration of concurrency
In this section **3** concurrent processes are made to transfer a file "TT.deb" top 3 clients concurrently.

![image](https://user-images.githubusercontent.com/76866159/106706632-fc68b900-6615-11eb-96ba-3d5cb3e2a9da.png)

Together, I am going to press ENTER on all the clients.

![image](https://user-images.githubusercontent.com/76866159/106706715-186c5a80-6616-11eb-93c8-e1719d785e2b.png)

As seen, all three first start and then end one after another. The code works beautifully. 


_**Important conclusion, in this methodology, the speed is excellent. But as anticipated earlier, the speed reduces to a third as the number of clients triple.
Hence, this is a good method if there is an upper limit on the number of clients that you have. Else, using a single process server would be better.**_

[Note: Go see my single process server at *https://github.com/yuvrajmalhi/TFTP-Single-Process-server* ]

Do share if you liked my work. Thanks!

:smile:
