#pragma once

#include <argp.h>

const char *argp_program_version =
    "1.0";
const char *argp_program_bug_address =
    "https://github.com/CplNathan/Nathans-Tarkov-Radar-Public";

static char doc[] =
    "Tarkov DMA Reader - https://github.com/CplNathan/Nathans-Tarkov-Radar-Public\n"
    "VMRead - https://github.com/Heep042/vmread"
    "\v"
    "./TarkovInjector <socket url> <socket passcode> -s";

static char args_doc[] =
    "http://127.0.0.1:8080\n boop -s";

static struct argp_option options[] = {
    {"retrysocket", 's', 0, OPTION_ARG_OPTIONAL, "Auto retry socket connection.", 1},
    {0}};

struct arguments
{
    char *args[2];
    bool retrysocket;

    int retrysocketmax;

    arguments()
    {
        this->retrysocketmax = 1000;
    }
};

struct arguments arguments;

static error_t parse_opt(int key, char *arg, struct argp_state *state);

static struct argp argp = {options, parse_opt, args_doc, doc};