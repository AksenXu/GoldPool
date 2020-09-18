SERVER_BIN=broadcast_server
CLIENT_BIN=broadcast_client
BR_LIB=libbroadcast

CFLAGS=	-I../Android2Linux/log/  \
		-I../Android2Linux/log/liblog \
		-I../utils/inc \
		-DHAVE_SYS_UIO_H

CommonSrc = BREpoll.cpp ConnProcessor.cpp BRType.cpp BRUtil.cpp BRServer.cpp BRClient.cpp
ServerSrc = BRServerTest.cpp
ClientSrc = BRClientTest.cpp

LDFLAGS = -L. -llog -lpthread 

bin:
	g++ -fPIC -shared -o $(BR_LIB).so $(CommonSrc) $(LDFLAGS) $(CFLAGS)
	g++ -o $(SERVER_BIN) $(CommonSrc) $(ServerSrc) $(LDFLAGS) $(CFLAGS) -lbroadcast
	g++ -o $(CLIENT_BIN) $(CommonSrc) $(ClientSrc) $(LDFLAGS) $(CFLAGS) -lbroadcast

clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN) *.o *.so