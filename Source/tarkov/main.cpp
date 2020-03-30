#include "Includes.h"
#include "GameSpecific/Tarkov/Tarkov.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

FILE* dfile;

void LoadGame(char* ProcessName, char* ModuleName, BaseGame* GameSpecific)
{
	FILE* out = stdout;
	pid_t pid;
#if (LMODE() == MODE_EXTERNAL())
	FILE* pipe = popen("pidof qemu-system-x86_64", "r");
	fscanf(pipe, "%d", &pid);
	pclose(pipe);
#else
	out = fopen("/tmp/testr.txt", "w");
	pid = getpid();
#endif
	fprintf(out, "Using Mode: %s\n", TOSTRING(LMODE));

	dfile = out;

	try {
		WinContext ctx(pid);
		ctx.processList.Refresh();

		fprintf(dfile, "\nInitializing Game Offsets\n");
		bool ProcessFound, ModuleFound;

		for (auto& i : ctx.processList) {
			if (!strcasecmp(ProcessName, i.proc.name)) {
				fprintf(out, "Game:\tLooping process %lx:%s\n", i.proc.dirBase, i.proc.name);
				ProcessFound = true;

				//fprintf(out, "\tExports:\n");
				for (auto& o : i.modules) {
					fprintf(out, "\t%.8lx\t%.8lx\t%lx\t%s\n", o.info.baseAddress, o.info.entryPoint, o.info.sizeOfModule, o.info.name);
					if (!strcmp(ModuleName, o.info.name)) {
						ModuleFound = true;

						GameSpecific->EntryPointModule(&i, o.info.baseAddress);
						return;
					}
				}
			}
		}

		if (ProcessFound == false)
			fprintf(dfile, "Error finding process: %s\n", ProcessName);

		if (ModuleFound == false)
			fprintf(dfile, "Error finding module: %s\n", ModuleName);

	} catch (VMException& e) {
		fprintf(out, "Initialization error: %d\n", e.value);
	}


	fclose(out);
}

// This is probably not thread safe if injected internally to the VM due to my many usages of sleep().
__attribute__((constructor))
static void init()
{
	while (true)
	{
		sleep(1);

		BaseGame* Game;

		Game = new TarkovGame();
		// NOTICE: I have since merged a new version of vmread however I have not updated the process strings that it looks for, so this may be out of date. 30/03/20
		LoadGame((char*)"EscapeFromTark", (char*)"UnityPlayer.dll", Game); // 'EscapeFromTark' is due to a character limitation on the way we currently obtain the process name through vmread.

		delete Game;
	}
}

int main()
{
	return 0;
}
