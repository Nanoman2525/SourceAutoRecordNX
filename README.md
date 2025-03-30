# SourceAutoRecord - NX

This is a WIP effort to port [SourceAutoRecord](https://github.com/p2sr/SourceAutoRecord) to the Nintendo Switch version of Portal 2 (v1.0.3).

# Compiling

1. Have [devkitPro](https://devkitpro.org/wiki/Getting_Started) installed for your current platform you will be working on.
2. Run the MakeFile using `make`.

# Installing

1. Have [nxtool](https://github.com/dazjo/nxtool) compiled and ready to use.
2. Take the generated `.nro` binary file and feed it as as argument when using nxtool.
3. Find the hash of your plugin from the output and save it for a later step.
4. Hex edit Portal 2's `romfs/.nrr/launcher_main.nrr` file and change the byte at the address of `00000344` from `15` to `16`.
5. Scroll down until you find the start of the `00` byte repitition that lasts to the end of the file.
6. Starting at the first `00` _overwrite_ the bytes with your plugin's hash from earlier.
7. Install your plugin's file in this directory: `romfs/nro/(your plugin).nro`.
8. You should now be able to load the plugin through the `plugin_load` console command.

# Notes

It's a bit tedious, but you will need to do every step of the installation if you wish to make multiple builds. The modification of `launcher_main.nrr` is needed in order for the plugin to be whitelisted as a valid executable to load on the device.
