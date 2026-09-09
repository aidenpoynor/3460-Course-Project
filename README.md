# System Monitor

## Project Topic

**System Observability Monitor** — a cross-platform utility that collects and reports
system-level metrics (CPU, memory, and process activity) so a user can observe the
health of the machine it runs on.

## Team Members

| Full Name | A-Number | USU Email | Role |
| --- | --- | --- | --- |
| Justin Shupe | A02318460 | A02318460@usu.edu | **Team Leader** |
| Aiden Poynor | A02401376 | A02401376@usu.edu | Member |

**Team Leader:** Justin Shupe (A02318460, A02318460@usu.edu)

## Build Instructions

The project uses CMake and is built with the provided `build.sh` script, which detects the
host operating system and runs the appropriate CMake commands.

**Build (Linux and Windows Bash)**

From the root of the repository:

```bash
./build.sh
```

If the script is not executable, run `chmod +x build.sh` first.

Build output is placed in the `build/` directory. Re-running `./build.sh` rebuilds in place.

## Run Instructions

Run instructions differ by platform because of where each toolchain places the executable.

**Linux**

```bash
./build/HelloWorld
```

**Windows**

MSVC builds into a configuration subdirectory, so run:

```bash
./build/Release/HelloWorld.exe
```

From the Windows Command Prompt or PowerShell, use backslashes instead:

```
build\Release\HelloWorld.exe
```
