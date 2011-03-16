OBJs = hserver.o Mutex.o MutexInstance.o HTTPmessage.o HTTPrequest.o ssbuf.o Socket.o Config.o Thread.o HTTPServer.o

httpserver: $(OBJs)
	g++ -g -lnsl -D_REENTRANT -lpthread -lsocket -o httpserver $(OBJs)

hserver.o: hserver.C
	g++ -c -Wall -g hserver.C

HTTPServer.o: HTTPServer.C HTTPServer.H Thread.H Types.H Runnable.H Config.C Config.H
	g++ -c -Wall -g HTTPServer.C
	
MutexInstance.o: MutexInstance.H MutexInstance.C Mutex.H Mutex.C
	g++ -c -Wall -g MutexInstance.C
Mutex.o: Mutex.C Mutex.H
	g++ -c -Wall -g Mutex.C
	
HTTPmessage.o: HTTPmessage.C HTTPmessage.H
	g++ -c -Wall -g HTTPmessage.C

HTTPrequest.o: HTTPrequest.C HTTPrequest.H
	g++ -c -Wall -g HTTPrequest.C

ssbuf.o: ssbuf.C ssbuf.H
	g++ -c -Wall -g ssbuf.C

Config.o: Config.C Config.H
	g++ -c -Wall -g Config.C

Socket.o: Socket.C Socket.H
	g++ -c -Wall -g Socket.C

Thread.o: Thread.C Thread.H Types.H Runnable.H
	g++ -Wall -g -c Thread.C -o Thread.o
clean:
	rm -f $(OBJs) core
	echo cleaning completed
