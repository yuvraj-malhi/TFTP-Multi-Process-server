# TFTP Server (Multi process)
### Description
TFTP stands for **Trivial File Transfer Protocol**. This is an aplication level protocol initially developed in the late 90s to be a simple file transfer protocol as suggested by the name. The basic task of TFTP is similar to any other File Transfer Protocol: transfer files from one device on a network to another. This file may be a _JPEG, PDF, EXE, ZIP, TXT_, or any other file.

Here are some features that make TFTP useful and noteworthy:    
  1. __Lockstep protocol__    - It is fault tolerant and can correct mistakes committed on the network   
  2. __Light weight__         - Hence, it does not require huge client or server computational power   
  3. __Uses UDP (fast)__ - A unique exception in an otherwise TCP dominated protcol environment  

To read more on the details TFTP, visit *http://www.ietf.org/rfc/rfc1350.txt*. This documentation contains the details of the algorithm that is implemnted in all the latest TFTP clients and servers.

How does this project help TFTP?        
In a world of exceptionally complex codes that cannot be understood, there is a need for simplicity.

Keeping this in mind, this TFTP server has the following properties:    
  1. __Concurrent__     - This server can handle multiple clients at the same time without any hastle.       
  2. __Muti process__   - A multi process server that can handle multiple clients while maintaining good speed.    
  3. __Simple API__     - Easily understandable code that uses variables, functions and loops in a justified manner.     
  4. __Fast__          - Functions that currently take hundereds of lines have been optimised to do the same task in very few lines.    
  5. __Intuitive__      - Variable & function names are very intuitive so as to eliminate confusion.    
  6. __Optional Verbose__ - The code has an option whether to print all the details of transfer or not.     


### Concurrency
This is a multiprocess server that ceates a new child process or a new thread (multi-threaded concurrency) for each new client. For example, In the below picture, say if a new "Client C" were to connect, then another server child process (or thread) would be created.

![1](https://user-images.githubusercontent.com/76866159/107152058-e21f3a00-698b-11eb-9cd9-61489dbdca16.png)

Advantage - **Speed**. For upto a limited number of clients (depending on CPU's specifications), the speed of multi process servers is extremely good.   
Disadvantage - As the number of child processes (or threads) increase, the server becomes **proportinately slow.**  



## Usage
### Initialize Server
Run the following commands to get started: 

```sudo ifconfig``` &emsp;&emsp;&emsp;&emsp;&emsp;&emsp; -  &emsp; *Check ther server IP. Mine is ```10.0.2.12```*        
``` sudo git clone https://github.com/yuvrajmalhi/TFTP-Multi-Process-server.git```           
```cd TFTP-Single-Process-server/```           
```cp <file location> ./``` &emsp;&emsp;&nbsp; -  &emsp; *Copy the file to server location. I have chosen 'TT.deb' for demonstration*          
```gcc tftp.c -o server```           
```ls```  &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&nbsp; - &emsp;  *Confirm that ther 'server' executable and file are present*            
```sudo ./server 69```         

[Note: if you are a networking geek, you may change the 69 to any other port number, but keep in mind that all TFTP clients connect to port 69 by default]

On the prompt, the program asks if you want to print everything that goes on in the file transfer or just the basics.
I will recomment choosing no or 'n' as this information will be irrelevant to all other than networking folks.

Here's how:
![Server_Multiproc2](https://user-images.githubusercontent.com/76866159/134908495-c01bff9b-8caf-4d62-b4a1-dc5f4680e512.gif)

Voila! Server is all setup.



## Client
### Usage
Run the following commands to get started: 

``` sudo apt-get install tftp``` &emsp;&emsp;&emsp;&emsp;&emsp;&emsp; -  &emsp; *Install tftp client*        
``` sudo tftp```           
``` connect 10.0.2.12``` &emsp;&emsp;&emsp;&emsp;&emsp;&emsp; -  &emsp; *Connect to server's IP*        
``` get TT.deb ``` &emsp;&emsp;&emsp;&emsp;&emsp;&emsp; -  &emsp; *Get the file 'TT.deb' from server*  

Follow here:
![Client_Setup2](https://user-images.githubusercontent.com/76866159/134916244-d4c5edb9-fccd-45bc-a611-7361cbcca732.gif)


As soon as the transfer ends, a prompt is shown telling you the size and time of file transfer.
Morover, on the **Server Side** the speed is also shown in mbps.

In this case, 500.4 Mb was transferred in 12.0 seconds. Transfer speed = **42 Mb/s**.

### Concurrent Transfer
To demonstrate concurrency, **3** clients will receive file "TT.deb" top 3 clients parallely.
![Concurrent_transfer](https://user-images.githubusercontent.com/76866159/134914441-9707c9a5-d39f-4c99-9519-830c08da8915.gif)

As the number of clients triple (from 1 to 3), the speed approximately reduces to a third (42 Mb/s to 18 Mb/s).

_**Conclusion: In multi-process servers, the speed is excellent. But as anticipated earlier, the speed reduces to a third as the number of clients triple.
Hence, this is a good method if there is an upper limit on the number of clients.**_

[Note: Go see my single process server at *https://github.com/yuvrajmalhi/TFTP-Single-Process-server* ]

Do share if you liked my work. Thanks!

:smile:
