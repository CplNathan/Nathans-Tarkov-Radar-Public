#pragma once
#include <mutex>
#include <condition_variable>
#include "sio_client.h"

class connection_listener
{
    sio::client &handler;

public:
    std::mutex _connectedMutex;
    std::condition_variable_any _connectedCond;

    bool connect_finish = false;
    bool connect_failed = false;

    bool connect_authenticated = false;

    bool connect_active = false;

    int connattempts = 0;

    connection_listener(sio::client &h)
        : handler(h)
    {
    }

    void on_connected()
    {
        // Called when the socket is opened for any reason.
        connect_finish = true;
        connect_active = true;

        _connectedCond.notify_one();
    }

    void on_authenticated(bool success)
    {
        connect_authenticated = success;

        if (!success)
        {
            connect_failed = true;
            connect_active = false;
        }

        _connectedCond.notify_one();
    }

    void on_close(const sio::client::close_reason &reason)
    {
        // Called when the socket is closed greacefully due to a disconnect request.
        connect_active = false;
    }

    void on_reconnect_required()
    {
        // Called when the socket is closed greacefully due to a disconnect request.
        connect_active = false;
    }

    void on_fail()
    {
        // Called when the connection is closed and there are no more reconnect attempts left.
        connect_failed = true;
        connect_active = false;

        exit(0);
    }

    void on_reconnecting()
    {
        // Called when a reconnection attempt is made.
        connect_active = false;

        printf("\rAttempting reconnect: #%i\n", ++connattempts);
        fflush(stdout);
    }
};