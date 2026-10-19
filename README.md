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
./build/SystemMonitor
```

**Windows**

MSVC builds into a configuration subdirectory, so run:

```bash
./build/Release/SystemMonitor.exe
```

From the Windows Command Prompt or PowerShell, use backslashes instead:

```
build\Release\SystemMonitor.exe
```

### Options

| Option | Default | Description |
| --- | --- | --- |
| `--interval <ms>` | `1000` | Time between refreshes, in milliseconds. Must be a positive integer. |
| `--top <N>` | `10` | Number of process rows to display. Must be a positive integer. |
| `--sort cpu\|mem` | `cpu` | Sort processes by CPU% or resident memory, both descending. |
| `-h`, `--help` | | Show usage. |

```bash
./build/SystemMonitor --interval 2000 --top 5 --sort mem
```

Press `Ctrl+C` to stop. The program refreshes until interrupted.

### Output

The first snapshot is a baseline only. CPU% values need two samples, so the first
report appears after one interval. Unavailable values print as `--`:

- Overall CPU% is unavailable when the counters cannot produce a valid delta.
- Process CPU% is unavailable for newly started processes and for processes without
  a matching baseline.
- Process RSS (`VmRSS`) is unavailable for processes that do not report it, such as
  kernel threads.

CPU% for a process can exceed 100% when it uses multiple cores. Overall CPU% is
measured against total system capacity.
