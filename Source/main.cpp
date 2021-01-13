#include "main.h"

#include "sio_client.h"
#include "connectionlistener.h"
#include "relayabstract.h"

#include <thread>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "Tarkov/Tarkov.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

template <class T>
bool LoadGame(char *ProcessName, char *ModuleName, RelayAbstract *Relay)
{
    pid_t pid;
#if (LMODE() == MODE_EXTERNAL())
    FILE *pipe = popen("pidof qemu-system-x86_64", "r");
    fscanf(pipe, "%d", &pid);
    pclose(pipe);
#else
    pid = getppid();
#endif
    std::string Errors;
    bool ProcessFound, ModuleFound;

    try
    {
        WinContext ctx(pid);
        ctx.processList.Refresh();

        for (auto &i : ctx.processList)
        {
            if (!strcasecmp(ProcessName, i.proc.name))
            {
                ProcessFound = true;
                for (auto &o : i.modules)
                {
                    if (!strcmp(ModuleName, o.info.name))
                    {
                        Relay->UpdateVMStatus("");
                        ModuleFound = true;
                        T(&i, o.info.baseAddress, Relay);
                    }
                }
            }
        }

        if (!ProcessFound)
        {
            char buff[100];
            snprintf(buff, sizeof(buff), " Error finding process: %s", ProcessName);
            Errors += buff;
        }

        if (!ModuleFound)
        {
            char buff[100];
            snprintf(buff, sizeof(buff), " Error finding module: %s", ModuleName);
            Errors += buff;
        }
    }
    catch (VMException &e)
    {
        char buff[100];
        snprintf(buff, sizeof(buff), " Initialization error: %d", e.value);
        Errors += buff;
    }

    Relay->UpdateVMStatus(Errors);

    return ProcessFound && ModuleFound;
}

static void init()
{
    sio::client h; // TODO Authentication option (for token)
    sio::socket::ptr current_socket;

    int retry = arguments.retrysocket ? arguments.retrysocketmax : -1;
    connection_listener l(h);

    h.set_open_listener(std::bind(&connection_listener::on_connected, &l));
    h.set_close_listener(std::bind(&connection_listener::on_close, &l, std::placeholders::_1));
    h.set_fail_listener(std::bind(&connection_listener::on_fail, &l));

    h.set_reconnect_attempts(retry);
    h.set_reconnecting_listener(std::bind(&connection_listener::on_reconnecting, &l));
    h.set_reconnect_listener(std::bind(&connection_listener::on_reconnect_required, &l));

    h.connect(arguments.args[0]);

    current_socket = h.socket();

    RelayAbstract *Relay = new RelayAbstract(current_socket, &l);

    std::thread([Relay] {
        while (true)
        {
            usleep(100000);
            LoadGame<TarkovGame>((char *)"EscapeFromTarkov.exe", (char *)"UnityPlayer.dll", Relay);
        }
    }).detach();

    if (!l.connect_finish)
    {
        std::unique_lock<std::mutex> lk(l._connectedMutex);
        l._connectedCond.wait(lk);
    }

    Relay->Authenticate(arguments.args[1]);

    if (!l.connect_authenticated)
    {
        std::unique_lock<std::mutex> lk(l._connectedMutex);
        l._connectedCond.wait(lk);
    }

    static int pos = 0;
    char cursor[4] = {'/', '-', '\\', '|'};
    auto printStatus = [&]() {
        usleep(100000);
        std::string VMStatus = Relay->ReadVMStatus();
        bool gameReady = Relay->ReadGameReady();
        printf("\rSocket Status: %s %c - VM Status:%s - Game Status: %s %c",
               l.connect_active ? (l.connect_authenticated ? "Online Auth" : "Online No-Auth") : l.connect_finish && !l.connect_authenticated ? "Offline No-Auth" : "Offline No-Connect",
               l.connect_active && l.connect_authenticated ? cursor[pos] : cursor[0],
               VMStatus == "" ? " Online" : VMStatus.c_str(),
               gameReady ? "Online" : "Offline",
               gameReady ? cursor[pos] : cursor[0]);
        fflush(stdout);
        pos = (pos + 1) % 4;
    };

    while (!l.connect_failed && l.connect_authenticated)
    {
        usleep(100000);
        printStatus();
    };

    printStatus();
    printf("\nConnection has failed and maximum retries have been reached, quiting.\n");

    h.clear_socket_listeners();
    h.clear_con_listeners();

    current_socket->off_all();
    current_socket->close();
    h.sync_close();
}

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = static_cast<struct arguments *>(state->input);

    switch (key)
    {
    case 's':
        arguments->retrysocket = true;
        break;

    case ARGP_KEY_ARG:
        if (state->arg_num >= 2)
            /* Too many arguments. */
            argp_usage(state);

        arguments->args[state->arg_num] = arg;
        break;

    case ARGP_KEY_END:
        if (state->arg_num < 2)
            /* Not enough arguments. */
            argp_usage(state);
        break;

    default:
        return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

__attribute__((constructor)) int main(int argc, char **argv)
{
    error_t argsparse = argp_parse(&argp, argc, argv, 0, 0, &arguments);

    if (argsparse == 0) // 0 = No Error
    {
        init();
    }

    return 0;
}
