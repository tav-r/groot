# GROOT (Glbic hooking userland ROOTkit)
This is a simple userland `LD_PRELOAD` rootkit. I wrote it as a personal POC. It is not really interesting because:
1. Using `LD_PRELOAD` is not really the way to go if you are serious about writing a rootkit
2. There are many much more advanced rootkits with exactly the same approach.

**groot** currently doesn't have any useful funcitonality. It only hies some files (you can configure the paths to hide in `config.h`) from tools that use `readdir` and  hides the `LD_PRELOAD` environment variable.
