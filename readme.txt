Open Syobon Action
------------------
by Mathew Velasquez

Open Syobon Action is a cross-platform port of the unforgiving Japanese parody of
Super Mario Bros..

Please report any bugs on the Sourceforge page
(http://sourceforge.net/projects/opensyobon/).

Requires:
SDL, SDL_image, SDL_mixer, SDL_ttf, SDL_gfx

Controls:
Z, Up, Semicolon, Button 1 (Joystick*) - Jump
Left, Right - Movement
Down - Enter warp pipe
0-9 (On menu) - Level Select

*Gamepad/Joystick Notes:
Your gamepad or joystick may not work by default with Open Syobon Action. Check
joyconfig.h in the src directory for more information.

Command Line Parameters:
-nosound	Disables Sound

Unimplemented (Todo):
*Font thickness (Really not important)

--Changelog--
Open Syobon Action RC 2:
*First Windows release!
*Dvorak keyboard support
*Error messages more detailed and directed to stdout
*Compiled code optimized for speed
*Better initialization and de-initialization code, so you can be confident
 that all resources are freed.
*Optional sound: The game will no longer crash if there is no audio device
 available, and all sound can be disabled with the -nosound parameter
*Minor bugfixes here and there

Open Syobon Action RC 1:
*Fixed incorrect "box" rendering (used to draw green pipes)
*Fixed random crashing at certain parts (thanks to mekuso for the bug report)
*Volumes now appropriately altered
*Gamepad support added
*Now uses original indexed PNGs, with original filenames (capital PNG)
*Minor tweaks and optimizations

Open Syobon Action v0.9:
*Fixed incorrect character rendering, uses font very similar to the original game
 (sazanami-gothic.ttf rather than msgothic.ttf)
*Changed encoding of the original readme and source code to UTF-8, so they can
 be viewed correctly with most software
*Changed DOS newlines to Unix newlines in source and readme
*Source code has been cleaned up with "indent", removed a few obsolete comments

Open Syobon Action v0.8:
*First release
