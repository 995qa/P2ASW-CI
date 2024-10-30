Alien Swarm's snd_rebuildaudiocache command is not aware of sounds in VPKs.
This means if you run the command, it will wipe all base Portal 2 sounds from the cache and they will fail to play.

There are two workarounds for this:
1. Use snd_updateaudiocache exclusively, which only adds new sounds to the cache, never removes existing ones.
2. If you must run a full rebuild, you will need to extract Portal 2's sounds from the VPKs into the respective sound/ folders before running the command.

Once the sounds are cached, the game can play them out of VPKs with no issue, so you do not need do bundle unpacked copies of every Portal 2 sound with mods or anything of the sort. This is only necessary during the process of building the cache itself.