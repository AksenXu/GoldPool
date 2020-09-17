SERVER_BIN=broadcast_server
CLIENT_BIN=broadcast_client

CFLAGS=	-I../Android2Linux/log/  \
		-I../Android2Linux/log/liblog \
		-I../utils/inc \
		-DHAVE_SYS_UIO_H
CommonSrc = BREpoll.cpp ConnProcessor.cpp BRType.cpp BRUtil.cpp
ServerSrc = BRServer.cpp
ClientSrc = BRClient.cpp

LDFLAGS = -llog -lpthread

bin:
	g++ -o $(SERVER_BIN) $(CommonSrc) $(ServerSrc) $(LDFLAGS) $(CFLAGS)
	g++ -o $(CLIENT_BIN) $(CommonSrc) $(ClientSrc) $(LDFLAGS) $(CFLAGS)

clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN) *.o