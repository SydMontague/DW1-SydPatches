# Syd's DW Patches

This is an attempt to re-implement and patch Digimon World 1 C++ in an ship of theseus style approach, where we re-implement an inject more and more parts of the game.

The main goal is to fix bugs and allow more modding of the game, rather than being a full mod on its own.

Special shoutout goes to Vice04, whose work directly and indirectly contributed to some of the patches.

## How to use

This project is not ready for public consumption, you'll have to build it from scratch yourself.

The project comes with a [devcontainer](https://containers.dev/) that contains the necessary tools and compiler. 
Tools like VS Code should be able to run and open it.

Once you've setup the devcontainer, run the `extract.sh <pathToDW1ROM>`/`extract.bat <pathToDW1ROM>` from within the project, to extract the US Version of the original game to use a base files.
After that you can run `rebuild.sh` from within the devcontainer and wait for it to build a `DigimonWorldPatched.bin/cue` that contains the patched version.

# Contact

* Discord: SydMontague, or in either the [Digimon Modding Community](https://discord.gg/cb5AuxU6su) or [Digimon Discord Community](https://discord.gg/0VODO3ww0zghqOCO)
* directly on GitHub
* E-Mail: sydmontague@web.de
* Reddit: [/u/Sydmontague](https://reddit.com/u/sydmontague)