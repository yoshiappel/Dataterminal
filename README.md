# Dataterminal [CMD]

Dataterminal is a custom cli that executes different usefull actions based on commands inside a Json file.

Commands like
- "yt" -> "https://www.youtube.com"
- "gh" -> "https://github.com"
- "phasmo" "steam://run/739630"

# Json setup
- Edit the Json file to add/delete commands.
- First assign the shortcut (key) for example "yt".
- Then assign the type so it knows if it is a Url or Steam game.
- And to finish up assign the value for Url just copypaste the Url in there, for steam game search the gameId on [SteamDB](https://steamdb.info/) and paste it in.
- You don't have to recompile.

# Project Setup
- Download this project.
- Change this line "CommandData data = loadJsonCommands("YOURLOCATION/commands.json");" to the location of the commands.json.
- Compile and run inside you terminal.
- Alternatively you could set a path in your system enviroments and just type the name in CMD then and it would also work.
