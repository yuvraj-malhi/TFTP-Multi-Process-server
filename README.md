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

![image](https://user-images.githubusercontent.com/76866159/106448570-f43f3b00-64a8-11eb-9c48-04cb430ed682.png)   

In the above picture, say if a new "Client C" were to connect, then another server child process (or thread) would be created.

Advantage - **Speed**. For upto a limited number of clients (this number depends on your CPU's core specifications), the speed of multi process servers outperforms all.  
Disadvantage - As the number of child processes (or threads) increase, the server becomes **exponentially slow.**  

![image](https://user-images.githubusercontent.com/76866159/106449420-f6ee6000-64a9-11eb-9cb8-44e45a52c106.png)


## Usage

### Server Side

#### STEP I
Before the actual setup, you need to note down the server computer's IP address so that it may be used later on. 
To do this, run the command:
``` sudo ifconfig ```

![image](https://user-images.githubusercontent.com/76866159/106704965-ffae7580-6612-11eb-882f-fcb0543f951f.png)

As the picture, in my case the IP is ```192.168.1.106``` 

[Note: The typical IP of a machine starts with either 192.... or 172.... or 10....]

#### STEP II
On the machine where you want the server, create a new empty folder and download (or copy-paste) "tftp.c".
        OR
Simply run the command: 
``` sudo git clone https://github.com/yuvrajmalhi/TFTP-Multi-Process-server.git ```

![image](https://user-images.githubusercontent.com/76866159/106705063-37b5b880-6613-11eb-999f-e8d2a3d3f38d.png)

 and then run the command:   
 ``` cd TFTP-Single-Process-server/ ```
 
 Here you may check that two files may have been created.
 
![image](https://user-images.githubusercontent.com/76866159/106705114-561bb400-6613-11eb-96b5-819297cfcb65.png)

#### STEP III
Now, copy all the files into the current folder that you may want to transfer to the clients later.
Use the command:

```cp <file location> ./```

Replace ```<file location>``` with the actual location of the file. 


I have copied the debian file "TT.deb"

![image](https://user-images.githubusercontent.com/76866159/106705228-94b16e80-6613-11eb-9f66-ee623dab95f8.png)


#### STEP IV
Now, to compile the program, run the command:   
 ``` gcc tftp.c ```
 
A new file "a.out" may have been created.

![image](https://user-images.githubusercontent.com/76866159/106705290-aeeb4c80-6613-11eb-8a19-c99a7059a92a.png)

#### STEP V
Now, to run the program, run the command:   
 ``` sudo ./a.out 69```
 
![image](https://user-images.githubusercontent.com/76866159/106705354-ce827500-6613-11eb-8245-4ab8cc826e20.png)

[Note: if you are a networking geek, you may change the 69 to any other port number, but keep in mind that all TFTP clients connect to port 69 by default]

On the prompt, the program asks if you want to print everything that goes on in the file transfer or just the basics.
I will recomment choosing no or 'n' as this information will be irrelevant to all other than networking folks.

![image](https://user-images.githubusercontent.com/76866159/106705400-e1954500-6613-11eb-8e83-f32d8dd65f8a.png)

Voila! Server is all setup and is listening.
Now all requests made from the client are shown here, along with the client's IP address, and the filname it has requested.

![image](https://user-images.githubusercontent.com/76866159/106455677-85ff7600-64b2-11eb-8741-56ad29a73e30.png)


### Client side
Client machine can be any machine on the same network (wifi or ethernet or hotspot) as the server. 
For the sake of demonstration, the client may be on the server machine also. That's what I will do.

#### STEP I
Kali users may skip this step.
For all others, check if you have a TFTP client. To to this, run the command:
```sudo tftp```


If an error is shown then you have to install it on your system. So run the command:
``` sudo apt install tftp```

![image](https://user-images.githubusercontent.com/76866159/106454080-3fa91780-64b0-11eb-8ced-88f51cdfa583.png)

After installing, run the command:
``` sudo tftp```

![image](https://user-images.githubusercontent.com/76866159/106454231-6f581f80-64b0-11eb-918a-864f5c99c5f3.png)

#### STEP II
Connect to ther server using the server's IP that we noted in Step I of Server side Usage
> As the picture, in my case the IP is 192.168.1.106

So to connect to the server IP, run the command:
``` connect 192.168.1.106```

Replace 192.168.1.106 with the server IP in you case as it may not be the same.

#### STEP III
Now, we get one of the files that had been stores in the server in Step III of server's usage.
> I have copied the debian file "TT.deb"

``` get TT.deb```
Replace TT.deb6 with the filename that you had stored.

#### Wait and watch!
The transfer will start.

![image](https://user-images.githubusercontent.com/76866159/106455184-dfb37080-64b1-11eb-9991-c4763e5b00a9.png)


As soon as the transfer ends, a prompt is shown telling you the time.
Morover, on the **Server Side** the speed is also shown.

![image](https://user-images.githubusercontent.com/76866159/106455302-05407a00-64b2-11eb-9092-7ce44c9e4e12.png)


## Demonstration of concurrency
In this section **3** concurrent processes are made to transfer a file "TT.deb" top 3 clients concurrently.

![image](https://user-images.githubusercontent.com/76866159/106456536-a419a600-64b3-11eb-93e7-9c7c97deffed.png)

Together, I am going to press ENTER on all the clients.

![image](https://user-images.githubusercontent.com/76866159/106456674-d75c3500-64b3-11eb-9ee6-14e831237e4c.png)

As seen, all three first start and then end one after another. The code works beautifully.

### Timeout
This program also implements timeouts. That is, if a client does not respond in 5 seconds due to inactivity, network issues or anything else, then that client's transfer is closed, thereby saving the servers resources.

_**Important conclusion, in this methodology, the speed performance with multiple clients is BETTER than single client.
Traditionally as the number of clients double, the speed halves, but here the speed does not go dow; only increases.**_

Do share if you liked my work. Thanks!

:smile:
