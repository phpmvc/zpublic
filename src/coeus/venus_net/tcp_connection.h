#ifndef __TCP_CONNECTION_H__
#define __TCP_CONNECTION_H__

#include "common.h"
#include "basic_stream.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketReactor.h"
#include "network_message.h"
#include "message_block_packetization.h"

class MessageQueue;
class TcpConnection : public Poco::Net::TCPServerConnection
{
    static const int MAX_RECV_LEN = 1024 * 4;

public:
    TcpConnection(const Poco::Net::StreamSocket& socket, MessageQueue& messageQueue, uint32 sequence);
    virtual ~TcpConnection();
    void run();

public:
	void sendMessage(int16 opcode, const byte* buff, size_t size);
	void sendMessage(uint16 opcode, NetworkMessage& message);
    inline uint32 sequence() const { return _sequence; }

private:
    enum ShutdownReason
    {
        SR_GRACEFUL_SHUTDOWN,
        SR_EXCEPTION
    };

    void sendMessage(const BasicStreamPtr& packet);
    bool onReadable();
    void onShutdown(const ShutdownReason& reason);
    void finishedPacketCallback(BasicStreamPtr& packet);

private:
    byte* _buffer;
    Poco::Net::StreamSocket& _socket;
    MessageBlockPacketization _blockPacketization;
    MessageQueue& _messageQueue;
    uint32 _sequence;
    mutable Poco::FastMutex _mutex;
};

#endif // !__TCP_CONNECTION_H__
