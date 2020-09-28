# unix_final_project
Copilation command with neccery links:
gcc myFileSystemMonitor.c -rdynamic -finstrument-functions -lpthread -lrt -lcli -o a.out

Example for running program command with neccery argument:  
./a.out -d /home/tomer/Documents -i 127.0.0.1

The program will support telnet in localhost port 8000 witch you can run with command:
telnet localhost 8000
Telnet connection deatails:
Name:fred 
Password:nerk
Telnet support the command: backtrace as required

The program will run client socket as required 
Listening on the ip you will provied when run the program with the argument -i, on port 10000
It will informe the server with inotify dir information as required
You can run server with netcat command:
netcat -l -u -p 10000

The program will listen with inotify (libcli) to the directory witch you will provide to the program with the argument -d

The program support apache webserver and update the index.html with inotify dir information as required
update webserver fonction declared in inotify_thread.c

