Version History and Changelog {#changelog}
=============================

1.9 (2009-08-17)
----------------

### New features ###

* implemented animation management
* new sprites : rotated sign, cloud block, sand block and slime block
* new enemy : spikeball ( graphics from yoshis_fan )
* new ice_kill, iceball, mushroom_blue sound ( from evans.jahja )
* furball, flyon, gee, krush, turtle and turtle boss moving state handling updated
* gee, krush, turtle and turtle boss movement handling updated
* implemented ability to reset the options
* implemented path rewind move type
* added metal spike and desert block static enemy to the editor
* added spanish translation ( thanks lightbuster )
* 3 level updates ( from sauer2 )
* updated level yuki2000 ( thanks konstantin )
* updated scaley ( from levelengine )
* added spikeball to world 3 and 4 ( from guest_xd )
* added editor mouse auto hide to the options
* spika and running shell both get hit on collision
* leaving level settings with a different music filename plays the new music
* entering level settings does not fade out the music anymore
* updated translations
* SHIFT + CTRL + A does now deselect everything
* shell does not die anymore if it hits a box from below
* fire resistant and ice resistance options added to static enemy
* updated mouse collision handling
* updated hud time calculation
* updated opengl version warning
* updated font

### Fixes ###

* fixed linked turtle shell does not handle collisions
* fixed path position can not be edited if it is a floating point value
* fixed massivetype change is set to not valid object types if multiple objects are selected
* updated french translation ( from thebosssdu68 and sst )
* fixed loading world with no waypoints crashes
* fixed active text box does not handle direction keys
* fixed crash when changing flyon, eato or thromp image directory
* fixed music is not looped in certain event situations
* fixed dying because of the fixed camera velocity in custom level mode results in a endless loop
* removed high frequency noise from death_box (thanks evans.jahja)
* fixed ghost maryo with ice power throws fireballs
* fixed world without objects can not be entered
* fixed changing flyon direction in the editor does not update velocity
* fixed shell y velocity was not reset when hitting a massive object on top/bottom
* fixed gee with no fly distance always flies
* fixed flyon can not get hit if frozen and gee only from top
* preload more images
* updated german translation
* update string to float conversion function to work correctly
* fixed semi massive text box does not activate
* disabling the global effect clears the active particles
* use new boost filesystem path handling functions
* fixed lvl_4 camera limit ( thanks guest_xd )
* updated ghost window_1 settings ( thanks yoshis_fan )
* plugged rare memory leaks
* 2 small level updates
* some level fixes ( thanks guest_xd )
* many cleanups
* removed fuzzy and unused translation text

1.8 (2009-04-08)
----------------

### New features ###

* implemented underground furball boss
* new sprites : slime particle, overworld sand_1 hills ( thanks youngheart80 ), overworld sand_1 bone ( thanks sauer2 ), big snow hill ( from youngheart80 and updated from me ), snow hills background, snow_1 window ( from thebosssdu68 )
* sprite updates : more_hills, beanstalk ( from sauer2 ), rope_1_hor ( from sauer2 ), fir_1 ( thanks sauer2 ), lightblue_1 cloud, dirt, smoke, smoke_black, green_1 kplant, recreated green_1 hedge wild_medium ( from sauer2 and me ), recreated old yoshi_1 slider and moved it to jungle_1 ( from sauer2 and updated from me ), recreated brown slider, recreated logo, background images, re-exported desert_1 ground in high resolution ( thanks fabianius ), recreated green_1 hedge big_1, green_1 hedge medium_1, green_1 hedge small_2, green_2 tendril, wood block 1 ( thanks fabianius ), signs, green grass ( thanks fabianius ), mushroom block ( thanks yoshis_fan ), brown box, moon, fireplant
* implemented path object and the ability for level exit, static enemy and moving platform to connect with it
* implemented moving platform circle and path type
* updated almost every level
* updated collision creation, checking, handling and validation !
* many level updates and fixes ( thanks to sauer2 )
* updated game_tutorial level ( from guest_xd, yoshis_fan and me )
* completely new gui graphics ( from fabianius and updated from me )
* implemented detection and scaling if image is greater than the maximum supported texture size
* massive sourcecode cleanup with many small optimizations and changes
* menu background changed to a level ( default is menu_green_1 )
* new levels : ita_2 ( from italian ), lake_shore_paradise level ( from yoshis_fan and small update from me )
* small world 3 updates
* world 4 level updates ( from guest_xd and me )
* use better and faster scale function in texture creation
* updated full screen effects
* new sounds : red goldpiece, pickup item, turtle stand up, empty box and fireball repelled
* recreated sounds : rokko activate, rokko hit, fireball explode, wall hit and fireball explosion sound
* fireplant and jstar can now be placed in a level
* updated world editor menu
* updated editor selected object drawing
* fade ghost mode in and out
* implemented line stipple pattern in the renderer
* implemented level exit blink movement option ( from simpletoon )
* many smaller updates and performance optimizations
* updated handling of the player active item release
* implemented rect request scaling
* implemented rokko up and down direction
* updated and added background image settings
* added fireplant particle animation
* added particle animation if player jumps against a massive object
* updated and fixed other particle animations
* implemented joystick hat handling ( thanks lig15 )
* allow all render request types to use rotation
* merged flippa_3 levels and some nice updates
* added nsis installer translations
* implemented possibility to handle a send collision instantly
* implemented collision handling with passive objects
* added greek translation ( thanks medigeek )
* added polish translation ( thanks rajish )
* added turkish translation ( thanks yusuf aydin )
* added more performance timer

### Fixes ###

* changed default action key from left ctrl to a
* changed default jump key from left alt to s
* fixed player change size function did not check the opposite direction
* updated moving ground and platform collision handling
* fixed entering level exit does not release the active object correctly
* fixed on a very high framerate per second maryo could get killed if on a massive moving platform
* fixed goldpiece animation draw and update is using the wrong images ( for years :o )
* fixed particle animation image is not centered
* fixed if player is in ghost mode all powerups move to the itembox and the bonusbox always creates the better item
* fixed a segfault on sprite destructor
* optimize box collision check function
* collision handling updates
* limit fps on loading screens or vsync will slow down the loading
* updated editor help screen
* fixed image settings rotation z of 180 does not change the x position
* fixed background image velocity is not framerate independent
* merged object collision type and array type
* fixed custom level could reset state after entering the menu
* updated custom level mode handling
* merged array type passive and front passive
* fixed crash if flyon or thromp image dir is set without a trailing slash
* updated particle emitter item drawing
* updated particle animation drawing
* fixed level music is not played if entering from menu
* fixed gee rotation is sometimes not updated
* fixed text box is not shown completely on the left or right level border
* fixed box did not always use the disabled image
* updated box useable count handling
* fixed disabled combobox button is not drawn
* fixed player active item as shell could collide with objects on top when player is moving
* fixed objects can still collide with a moving platform if it falls below the level
* optimized powerups and goldpiece collision handling
* changed generic enemy got hit animation
* update krush collision rect
* fixed player handling with the moving platform when jumping from it
* optimized plastic pipe connection collision rect
* fixed enemies don't ignore ghost objects
* fixed debug rects could be drawn behind objects
* auto update level to new slider graphics on load ( if the slider sprite used middle tiles it does now overlap and you need to update the level but not for the moving platform as it auto sizes )
* fixed player particle animation is not on the correct position when ducked and jumping against an object
* fixed editor crash if text or question box was opened twice
* allow player to walk on static enemies when on downgrade invincibility
* updated rokko, turtle boss, thromp and poison mushroom particle animation
* fixed thromp collides with almost everything
* turtle and turtle boss as shell only stand up if nothing is blocking it
* do not cache images without the width and height set in the image settings as there is currently no support to get the old and real image size. the scaled down (cached) image size was used which is wrong
* fixed level sprite manager z position always increases
* renamed jpiranha to flyon
* fixed game over particle animation is not drawn
* fixed time display did recreate the image every frame
* updated credits menu animation
* updated editor gui window handling
* optimize mouse copy function
* update txt file parser and make it more robust
* updated turtle and turtle boss
* added entered number validation for all editor object settings
* savegames now restore the custom level mode
* fixed falling from a halfmassive ground object does not check if other objects now block
* fixed if object or player is moved by another object the ground is not checked
* updated player movement check if colliding with blocking objects
* fixed level background image constant velocity changes the start position
* fixed on ground check sends collision even if it failed
* updated object detection if climbing
* updated detection if on climbable object
* draw moving platform always if active object
* fixed destroyed object collisions are handled
* fixed player active item can collide with objects it should not as it moved one the wrong frame
* added checks if audio was initialized as requested
* set audio menu tooltip text with gettext
* changed audio hertz option from 44800 to 48000
* fixed thromp speed can be set to negative values
* fixed thromp stops after collision with fire and iceball
* fixed moving platform does not shake
* fixed falling platform image rotation is framerate dependant
* fixed star does not turn around on the level edges
* fixed entering level with fading out music could play the default music
* fixed collision handling and points text memory leak
* many stl updates and iterator handling fixes
* run in place patch for linux ( from simpletoon )
* fixed fireball fire particle animation is framerate dependent
* add option to enable debug build on linux
* fixed menu alpha rect is drawn in front of buttons
* fixed rope_1_hor and beanstalk image settings
* many cegui event updates
* updated options menu
* renamed texture/geometry detail to texture/geometry quality
* options menu text is now translatable
* changed level engine format to integer
* updated fonts
* removed old green ground from stephan levels ( thanks sauer2 )
* fixed controls.html style ( thanks fabianius )
* lowered the default background image scrolling speed
* moved some txt author files to image settings data
* editor item drawing now overwrites scale directions
* updated mountain trials ( from bowserjr )
* updated nsis installer
* preload fire animation
* updated template and german translation
* optimized renderer drawing a bit
* fixed get pixel function

### Removals ###

* removed old yoshi_1 ground
* removed very old pipes
* remove old and unused sprites

1.7 (2008-12-24)
----------------

### New features ###

* new sprites : tree_shaped_2 ( from sauer2 ), jumping piranha ( from sauer2 and me ), snow ground ( from youngheart80, bowserjr and updated from me ), ice balloon tree, candy_cane ( from sauer2 and me ), fir_1 ( from youngheart80 ), ice block 3, candy_cane_2 ( from youngheart80 ), snowman_1 ( from youngheart80 )
* new levels : test_pipes ( from robwood ), sauer2_12 ( from sauer2 ), sauer2_13 ( from sauer2 ), sauer2_13end ( from sauer2 ), sauer2_14 ( from sauer2 ), neverland ( from sauer2 ), furball_hills ( from sauer2 ), flan ( from jasonwoof ), space ( from ae ), cave_1 ( from youngheart80 ), jungle_2 ( from youngheart80 ), jungle_3 ( from youngheart80 ), test_moving_platform_1 ( from italian )
* world line layer copy function implemented
* implemented listbox search buffer for typing in the name directly in menu start
* new translations : russian ( from andrey_sm ), chinese traditional ( from dl7und ), french ( from tester and dylou )
* sprite updates : green_hills_2, balloon tree, jungle_1 tree 2, underground ground, underground cain, brick block 2_1, ice block 1, red and yellow door
* many level updates ( from sauer2 and me )
* implemented level entry beam type
* implemented moving platform up and left direction
* implemented editor ability to set the furball, turtle, rokko, turtle boss, jpiranha, eato, thromp, gee, krush and moving platform direction
* allow most enemies to be used as ground objects
* implemented semi massive box which is only touchable in the activation direction
* main menu ground images are now loaded from game pixmaps
* completely downgrade walking enemies if below ground
* display image filename in editor if it is not found and save it again
* changed menu handling if entered from level or world and display the active level or world
* display levels in the start menu which are in the game and user directory with a gradient color
* player throws two times the fire/ice-balls if in star mode
* added clear_night to world 2
* added option if editor should show item images and an option for their size
* added translation template

### Fixes ###

* fixed moving platform did not move player/enemy if colliding on the left or right !
* fixed moving platform did not hit player/enemy if colliding on top or below with the object on ground !
* fixed player can not get out of a small corridor when sliding ducked into it !
* fixed thromp did not move/hit player/enemy if colliding on the left, right or bottom
* fixed background image tiling for height does not work correctly
* fixed falling moving platform does not check correctly if below ground
* fixed editor selects destroyed objects and when copied it crashes rarely
* fixed halfmassive moving platform stopped moving if it could not move the object upwards
* fixed deleting the ground object of a moving object in the editor crashes when exiting
* do not allow level limits below the game screen resolution
* updated rokko death animation
* fixed turtle boss did not play dying animation if below ground
* optimized thromp collision validation
* fixed fire/ice ball colliding with player was handled as full collision
* fixed box is not drawn initially in editor if set to invisible
* fixed moving platform position is wrong if image has a collision rect
* fixed moving platform could move objects into the opposite direction
* fixed overworld way image settings
* fixed jungle_1 ground tiling
* fixed deleting waypoint on world editor crashes
* fixed turtle shell could activate text box
* fixed crash if thromp image directory setting is changed in the editor
* fixed starting a world does not reset the player data and current progress
* optimized fire/ice-ball throwing
* level entry and exit player animation optimized
* fixed player is ducked or position changes sometimes if coming out of a level entry with massive objects near it
* updated world 3 layer lines and changed music to land_2
* fixed ghost box is not called ghost box in editor
* fixed rare crash with warp level entry
* fixed some gettext translations for CEGUI are not utf8
* fixed destroyed waypoint handles draw and update
* moved many txt author files to image settings data
* fixed selected objects in editor mode with different massive types could be drawn on same z position
* fix some drawing validation checks
* made rokko smoke particles smaller
* fixed uninstaller always removed the music directory
* fixed nsis uninstaller did not remove translations directory
* fixed game options menu tabcontrol name is audio
* updated fonts

### Removals ###

* removed very old documents

1.6 (2008-09-29)
----------------

### New features ###

* basic translation support without support for some cegui objects, levels, images and credits text yet
* added german translation
* mac support patch ( from auria )
* implemented geometry and texture detail setting
* implemented clipboard text handling for copy, cut and paste
* updated options menu
* plastic_1 ground recreated
* sprites updated : ghost mushroom, arrow image, beanstalk_2 ( from youngheart80 ), green climbing plant ( from youngheart80 ), green_2 hedges, furball, spika, green_1 and light_blue_1 hills
* implemented background image type for all directions !
* implemented background image position and constant velocity
* image cache resize is now done in software and only caches images who need to
* updated ambient wind sound and removed the old one ( from consonance )
* new ambient sounds ( from consonance )
* updated and fixed audio engine
* install VC 2005 SP1 runtime (vcredist_x86.exe) on windows (this removes the need for the Microsoft.VC80.CRT directory)
* implement continuous ambient sound
* added heightsticks for the level editor ( from BowserJr and small update from me )
* pipe connection blocks in blue, green and red
* new furball turn around image
* handle page up/down and home/end keys in the world/level listbox
* new level sauer2_nolin ( from sauer2 ) and ita_1 ( from italian )
* updated levels ( some updates from sauer2 )
* text box can be set invisible
* updated command line handling and added world and debug arguments
* changed gui slider style from vertical to horizontal
* new big maryo and power jump sounds
* add particle emitter iteration interval and quota limits
* added a debug printf macro

### Fixes ###

* get supported resolutions from SDL and display the good ones in green and bad ones in red
* fixed destroyed objects got saved
* fixed invisible state was ignored on box, level entry, level exit and particle emitter
* fixed destroyed state was ignored on sprite and ambient sound
* copying a text box also copies the text
* configure.ac updated : added check for header files and check for gettext library
* ducking small maryo should not be bigger than big maryo ducking ( thanks bowserjr )
* fixed deleting ground object of an moving object crashed if exiting editor
* updated overworld and level input system
* fixed pressing left and right defaults to wrong key
* fade ambient sound out if camera gets out of range
* fixed an editor item menu endless loop because std::string::size_type is not used ( thanks nyhm )
* updated editor object settings tooltips
* move some txt author files to image settings data
* updated float_to_string function (with a function from stringencoders)
* renamed rex to krush
* updated level loading
* stop playing ambient sounds if a new one is set
* fixed gee fly_distance_counter was framerate dependent
* fixed could not cancel set joystick button dialog
* changed shooting to be time interval based
* rokko, thromp, gee and jpiranha always show the distance rect if the object settings are active
* fixed moving platform height is not set if using a non default image
* fixed world 4 first line is not always detected
* fixed crash if creating cache failed
* updates for cegui 0.6 support
* fixed thromp did collide with massive objects if not moving
* fixed frozen thromp is still used as ground object if hit
* fixed furball turn around image is set if colliding with an object on top or bottom
* fixed rokko death animation smoke z position
* movingsprite col_move performance improved
* updated the fixed colorbox effect
* updated rokko and now shows the distance if in editor mode
* fixed ostringstream openmode was wrong in float_to_string
* fixed power jump is always set if ducking
* updated furball and turtle collision rect
* fixed green turtle color is displayed wrong in editor
* fixed spika killed turtle boss on contact so it now only causes a state change
* updated fonts
* updated many tooltip help messages
* removed the global cegui namespace
* renamed all header include check defines
* changed naming of pipe connections
* correct direction rotation for gee
* only convert to a new software image if needed
* pixel removed in underground ground ( from sauer2 )
* changed banzai bill to rokko in history
* added check if the the window icon does exist
* reset player animation if walking against a massive object
* fixed endless loop if entering a not existing level with level entry set

### Removals ###

* removed old and unused image settings
* removed some very old and unused images

1.5 (2008-05-03)
----------------

* new game icon ( very important !!!!1!oneone )
* added world 4
* extended world 2
* new sprite snowflake_1 particle ( from youngheart80 )
* new sprites block toy ball, static enemy blocks and desert thromp ( from frostbringer )
* new sprite star_2 and windtrail_1 ( updated from me ) particle ( from bowserjr )
* new sprites ghost_hills_1 ( updated from me ), overworld bridge_1, cactus 3 and 4, overworld stone 2, ghost light, stone_2 set ( updated from me ) and jungle plants ( updated from me ) ( from sauer2 )
* new sprites small_green_ballhills_1 and big hill 3 ( fluxy )
* updated sprite for eato ( from helios )
* updated sprite for mushrooms ( from youngheart80 )
* updated sprites for tendril, thromp, rokko ( updated from me ) and gee ( from frostbringer )
* updated sprites for stalagtites background, ice/screw block, small hedge 1 and 2 and rokko ( fluxy )
* gumba is replaced with the new furball ( from helios )
* rex is replaced with the new krunch ( small update from me ) ( from helios )
* optimized and updated plentiful images
* added many levels ( from sauer2 )
* implemented ambient sounds ( from simpletoon with many updates and some fixes from me )
* plentiful level and world updates and adjusted difficult
* implemented renderer circle drawing
* implemented performance timers ( visible with CTRL + P )
* static enemy image and rotation speed can be changed
* optimized moving platform
* moving platform slows down near the end position
* added touch_time to new moving_platform
* merged falling platform with moving platform
* implemented delayed moving platform ( from simpletoon )
* moving platform data is now saved in savegames
* implemented author for image settings
* overworld camera now handles level limits
* small hud update
* stephan and other old levels brought back ( from nemo )
* thromp can use different image directories
* thromp uses different image when active
* audio hz is now applied immediately
* keyboard/joystick shortcut handling and gui update
* implemented keyboard scroll speed and the gui
* implemented joystick horizontal and vertical axis gui
* cache can now be recreated in game
* implemented texture reloading from file
* on resolution change cache and textures are reloaded from file ( displayed with the loading screen )
* software texture reloading saves and restores format and bpp
* loading screen updated ( shows a progress bar )
* updated gui skin
* image cache can be disabled via config.xml
* implemented sprite image rotation can affect the collision rect ( fixed eato collision rect )
* updated credits and credit screen particle animation
* updated level settings gui
* implemented particle emitter as editor object ( from simpletoon and many changes from me )
* added collision rect to all blocks and boxes
* new world 4 music bonus_1.ogg ( from vencabot_teppoo )
* power jump if ducking for some time ( from rolo with changes and particle animation from me )
* allow negative global effect constant rotation z
* added start rotation particle animation support and fixed negative constant rotation
* particle animation gravity patch ( from simpletoon )
* implemented editor object settings row ( from simpletoon and updated from me )
* eato image directory can be set
* implemented level entries can be entered in a different level ( from simpletoon and many changes from me )
* fixed transparent color of many images is wrong
* much cleaner way of defining DATA_DIR ( from nyhm )
* fixed overworld layer lines file crashes game if empty
* enabled anti-aliasing for points
* updated pipe connector image settings
* updated menu ground image and removed menu_quit ground image
* fixed crash if started with unknown argument
* fixed massive moving platform didn't always pick objects up
* fixed new music isn't played on certain game mode changes
* fixed world editor waypoint settings default start access isn't set
* fixed overworld hud isn't updated if editor enabled
* fixed editor mouse object position text didn't display editor/start position
* fixed player sometimes falls through the moving platform
* fixed overworld camera stutters 1 pixel
* optimized turtle and eato collision rect
* fixed the editor load function doesn't do anything
* fixed player could fall through vertical level limit if hitting the horizontal level limit at the same time
* fixed turtle boss can be hit with a shell when linked to the player
* fixed static enemy rotation is not framerate independent
* small rendering optimizations
* update man page and add .desktop file
* fixed screenshot is saved with the alpha channel
* moved todo-code.txt to the wiki http://www.secretmaryo.org/wiki/index.php?title=Todo_Code
* fixed moving platform drops player off when falling
* fixed overworld camera used smart camera offset
* fixed audio options buttons did affect the wrong item
* fixed camera position if player changed size
* fixed camera y centering didn't use bottom position
* fixed cAudio :: PlaySound volume is used even if invalid
* catch initialization exceptions
* updated makefile
* fixed thromp distance rectangle didn't use image size
* fixed thromp distance rectangle size is displayed shorter
* fixed thromp distance rectangle is collision checked in thromb
* fixed thromp position isn't updated correctly if reached start position after activation
* updated preferences handling audio and video
* fixed global effect/particle animation emitter rect x/y normal position could be overwritten
* fixed global effect emitter rect x/y start position is not used
* fixed image settings x rotation 180° did nothing
* fixed crash if exiting game with activated editor object
* fixed goldpiece and moving platform update handling
* fixed level camera position is not set back on level settings exit
* fixed editor object settings are positioned using the game position not the start position
* fixed entering custom level did not resets progress
* fixed global effect particle animation image is loaded before final screen initialization
* particle animation updates
* local config is preferred over the the user data dir if available
* fixed renderer vertical gradient drawing
* fixed joystick name is not saved as xml string
* disable rendering while inactive ( from rolosworld and small update from me )
* fixed fullscreen is not changed if it is the only change
* fixed camera horizontal and vertical offset is not set to preferences and is set in options from the wrong camera
* preferences loading uses a fake cegui system and renderer for the xml parser so a fake videomode is not needed anymore see the new startup direct video initialization
* preload 2 more sounds
* fixed entering world from a world did not play new music
* rokko smoke behind rokko
* fixed dying with active fire/ice balls could crash
* fixed crash if object selected in editor when disabling editor
* script for running optipng on the data files ( from nyhm )
* moved some images and handle it in level loader
* updated camera moving to new position when entered level entry
* if level exit destination level is empty use the same level
* fixed text box text can be not completely visible
* particles are now scaled from the center
* fixed directly loaded gl_surface images ingame did not delete the opengl texture because it could think the texture is still in use by wrongly checking the managed images
* fixed immense memory leak when caching images caused by not destroying the png write and info struct
* fixed slowly moving to camera is not completely smooth
* fixed camera moving to new level entry did not keep global effect particles on screen

1.4 (2007-12-23)
----------------

* implemented image cache which saves the resolution scaled images in the user directory for faster loading
* new perfect jungle plant and grass sprites ( from needcoffee )
* new saw sprite ( from inky )
* added climbable vine explanation text box to lvl_2
* added ability to save images to png
* screenshots are now saved as png
* joystick event handling update
* level updates
* start menu and loading screen updated
* updated credits
* sprites now set the rect and col_rect width and height
* implemented gl_surface int_x and int_y scaled from sprite
* updated rex and gumba death animation
* fixed mouse did set internal button state of events processed by cegui (editor settings crash)
* fixed itembox fireplant image is not displayed
* fixed spinbox editor settings are not positioned initially
* fixed world compass shows an "O" instead of an "E" for east
* removed empty pixmaps/extra folder
* fixed some joystick menu options are handled from the wrong id
* player moving state is now saved in savegames (fixes saving when climbing)
* optimized turtle shell in player hands position
* fixed editor item image int_x and int_y aren't scaled
* fixed player could collide with objects when dead

1.3 (2007-12-01)
----------------

* savegame support for spinbox, all powerups and enemies with their state
* new big and small pipe sprites with collision rects
* new overworld maryo sprites (thanks Helios)
* implemented ground type patch (thanks segfault) and updated it (added earth, ice, stone and plastic type + affects running smoke and walking animation )
* applied patch for different joystick axis support ( thanks Toad King )
* level/world updates
* new sprites : green_2 hill, green_1 and light_blue_1 hill, green ground 3, default stone block 
* turtle boss got his own hit sound
* implemented skeleton level manager
* level sprite manager is now handled over the level class
* fixed crash when invalid filename used with file_exists()
* fixed image settings rotation with an asynchron resolution
* fixed GL_Surface width and height are not the final values
* updated level settings screen (background gradient is previewed and auto scales to the selected background image)
* fixed editor_mouse_auto_hide doesn't show the mouse again
* fixed changing preferences ingame resets the sound/music setting
* some speed optimizations
* more precise mouse position
* correctly rendered cegui fonts
* optimized mouse scrolling
* fixed resolutions below 800x600 didn't work correctly
* fixed ground_3 right_bottom and right_top tile collision rect
* updated fonts
* updated preferences setting names
* updated image settings rotation handling
* fixed cegui.log file is in an invalid directory for Linux
* fixed position rotations aren't precise
* optimized sprite debug array color handling
* fixed sprite Set_PosY with new_startpos set did set the startposx
* editor item list items got a black shadow
* video initialization checks if resolution and bits per pixel are supported, if not falls back to supported settings
- removed remake levels

1.2 (2007-10-18)
----------------

* new jungle sprites ( from youngheart80 )
* added new/edit/delete to level start menu
* new level : mountain trials ( from BowserJr )
* many level updates
* big night_sky level update
* implemented fixed horizontal level scrolling
* implemented random powerup bonus box
* audio hz is now selectable in the options
* added editor special object and unknown image
* image settings now handle images on base image settings correctly
* added waypoint type to editor settings
* added delete function to level editor
* new sound if level up from 100 goldpieces
* remastered fireball, fireplant, goldpiece_1, mushroom and star_kill sound
* overworld : fixed next direction walking line was set using the current waypoint id and not the detected front line origin id
* fixed editor item menu images lagged
* fixed rendering with no_camera not set did change the position permanently
* fixed new maryo type was only set if animation was drawn
* settings without effect are now grayed out on the bonus box editor object settings
* fixed user worlds didn't override game worlds in overworld manager
* user worlds are now green in the start menu list
* use correct location for the cegui log
* fixed many memory leaks
* fixed editor got active while unloading on loading of a different game mode
* fixed start menu gui was not unloaded before changing game mode
* fixed overworld editor layer drawing was set for each world individually
* fixed overworld next waypoint detection/activation used the old waypoint number method
* fixed walking from waypoint "lvl_5" to the next waypoint didn't work on first run

1.1 (2007-09-21)
----------------

* implemented user/game directory seperation for savegames, screenshots, levels and worlds
* player graphics now in high quality
* implemented player graphics collision rect
* implemented overworld function "new"
* new sprite wood door
* updated sprites : doors, signs and ice mushroom ( from youngheart80 )
* overworld layer lines are now followed until waypoint found to check if it's accessible
* overworld : only check with a forced layer line origin id if start line found
* updated player animations
* level updates
* new ice mushroom effect
* ice gumba has ice_resistance
* new level allen_1 and eatomania and updated many levels
* overworld maryo now looks *different* and optimized the image settings
* statictext can now handle multiple lines of text
* fixed active fire/ice ball counter is not updated if leaving level
* fixed level settings didn't set direction range
* fixed image copying from settings based file didn't base settings
* make sure the display resolution is restored if tsc exits abnormally (nyhm)
* fixed world editor save box text was "Save Level ?"
* optimized some jungle_1 ground graphics
* fixed some invalid music getting played bugs
* fixed player ducking did check for out of level
* fixed level loading on start menu always erased after and the "." in the level name
* moving platform collision detection update
* fixed ice_resistance wasn't applied correctly
* a created level is not anymore instantly saved
* fixed send collision allowed object which is not existent in the object manager
* fixed first found overworld is the default
* fixed player ChangeSize checked greater out of position and not back to original if new position isn't valid
* fixed player ChangeSize always used given position even if detected as invalid
* fixed player Draw_Animation didn't end with the new type

1.0 (2007-07-28)
----------------

* implemented screen fade in effect
* new CEGUI skin
* new particle animations
* implemented text box
* implemented keyboard input handling in start menu
* new jungle ground tileset with halfmassive and plain sets
* updated yellow box
* updated gumba and menu ground graphics
* new jump and eato sound
* implemented level entries for pipe warping in a level
* entering the menu from a custom level selects the active level
* added mushroom platform shaft_bottom and shaft_double_riffle
* new green_1 plants graphics ( from Paddy )
* new level jr_cave ( from BowserJr )
* entering a pipe now centers the position and rotates the player
* spika now jumps if hit from box
* added game over animation
* added joypad exit button to ingame configuration
* level joystick input events are now handled directly
* implemented sprite based 'can be ground' validation
* player enemy collisions from top are handled
* updated climbing
* fixed ball could get set as ground object and crashes the game
* updated green_hills_2
* many level updates
* level/world loading keyboard shortcut needs CTRL
* fixed maryo couldn't walk on thromp
* fixed star maryo walking
* updated mushroom platform, balloon tree
* fixed spin and bonusbox didn't use last animation image
* fixed player collides with shell if invincible
* fixed jpiranha and gee collides with player if invincible
* fixed player didn't release ghost box ground object if ghost powerup ended
* fixed enemies didn't update correctly if frozen
* fixed player did set turtle shell running if frozen when released from holding
* fixed turtle boss didn't throw more fireballs if downgrades happened
* fixed goldpiece could set ground object
* fixed editor menu scrollbar is visible if item got selected
* fixed item collides with ball
* fixed object settings are visible in level settings screen
* fixed camera update is done twice
* fixed camera update lags behind a frame
* fixed ducking in front of halfmassive causes ducked-stay loop
* fixed box powerups didn't set as spawned on creation
* fixed box star powerup is not spawned on top
* fixed already used sound resource id is ignored
* obsolete objects are now shown in the editor in red color
* fixed gui boxes didn't send key up events to CEGUI
* fixed ghost lamp used a wrong editor tag
* level_dir preference is ignored if the directory doesn't exist
* updated big item effect
* fixed some fading effects were limited to low fps
* fixed dying doesn't reset active item
* fixed instant destroyed object deletion could cause collision invalidation or even crash. the array position is now replaced if a new object is added.
* fixed player is visible if entered pipe
* fixed rokko vertical activation range is not checked
* changed player stay, walk and run moving state handling
* fixed itembox sound is played on savegame loading
* fixed static and debug text box width was not set to fit content
* switched to the GPL v.3 license
* documentation updates

0.99.7 (2007-06-15)
-------------------

* many new sounds ( most from Cameron )
* new Gumba sprites ( from Pipgirl )
* new Turtle Shell sprites ( from maYO )
* many moving and falling platform updates
* added green_hills_2 ( from ufa and some small improvements from me )
* added sand_hill, blue_hills_1 and blue_waterhills_1 background level background
* new levels ( from Martimor )
* added ice_1 particle sprite
* added xpm icon ( from nyhm )
* fixed world 2 and 3 layer lines
* collision detection updated
* centered editor max distance rects
* fixed gee didn't check for max distance if moving down
* updated fonts
* updated Fire Flower, Star, Moon, Goldpiece, Mushroom, default_1/lightblue_1 cloud, Arrow, Eato, Hud, Spika and Saw sprites
* updated green_junglehills background
* many level updates
* fixed font_very_small is not destroyed ( thanks InsaneBoarder234 )
* fixed image settings loader didn't load base image settings
* optimized desert_dunes_1 and low_sand background
* manually loaded levels don't enter the overworld now
* image settings now detect if the image is already assigned when an image settings base image is given
* fixed wrong climbing image was used
* fixed mipmapping settings were not saved on texture reloading
* fixed late camera position update caused tearing on some drawing request
* fixed editor window could not be activated
* fixed some editor items are not rotated
* updated Fire and Goldpiece static animation
* small animation updates

0.99.6.1 (2007-04-19)
---------------------

* fixed GCC compiling

0.99.6 (2007-04-18)
-------------------

* big gee update
* new image loading scaled fitting to the current resolution with optimized settings loading and mipmap support 
* implemented player fast run
* moving platform now supports massive type 
* implemented Overworld Editor layer lines support
* 10 new light yellow clouds ( from Pollilla86 ) 
* new and updated levels
* new flower and hud itembox image 
* implemented GUI boxes 
* overworld editor can now create waypoints 
* shell with active playercounter doesn't block the player anymore 
* player can now hit gee if invincible
* updated gumba 
* fixed throwing animation can go out of level rect 
* updated credits screen 
* updated moon 
* updated hud gold image 
* ducking stops if lost ground object 
* updated menu bottom image 
* fixed spinbox copying didn't set usable count and invisible type 
* fixed editor player mouse selection checking was behind other objects 
* fixed level player start position could change 
* fixed throwing animation is drawn while ducking 
* updated overworld sprites 
* fixed ball and items collides with ghost boxes 
* fixed box animation changes position 
* updated castle windows 
* updated star and clock graphics 
* fixed mouse is hidden if entered menu in editor mode 
* fixed new level dialog always created a new level 
* fixed freeze state is drawn in editor 
* gradient drawing is now done via hardware/OpenGL 
* screen is light grey if ghost maryo 
* enemy collision handling updated 

0.99.5 (2007-02-28)
-------------------

* New Unified Editor with Overworld Support using tags and dynamic Menus and Items
* Added Image Scanning for the Editor based on new image settings parameters
* Added Level List showing all levels available
* Implemented New Maryo Graphics with Throwing and Holding Animations ( from dteck )
* Added Snow and Ghost Maryo ( from dteck )
* New Sprites : Default Cloud and Hills
* New and Updated Effects
* New and Updated Sounds for Rex, jPiranha and Turtle Boss
* Image Settings name, editor_tags and type added
* Credits Screen is now shown if the Overworld is finished
* Added Static Enemy Saw
* Added Laying Mushrooms
* Default Joystick is now selectable
* Editor can now copy multiple Objects using their size
* Overworld detection is now dynamic
* Joystick updates and fixes
* Turtle Boss updates
* Savegame and Preferences loading XML exceptions are now catched and don't crash the game anymore
* updated lvl_1 and added lvl_1_sub_1
* updated new level creation
* updated star maryo walking
* fixed global effect Z Position was never saved/loaded
* optimized global effect
* updated makefiles
* fixed saving in Overworld also subtracts 3000 points
* updated menu design

0.99.4 (2007-01-04)
-------------------

* New Savegame XML format with Overworld Progress and Level Object Data
* Walking between Overworlds saves the progress made
* New Enemy : Turtle Boss
* Added Falling Platform
* New Overworld 3 Levels and Updates ( from Weirdnose )
* Leveleditor Help Screen available with F1 ( from Weirdnose )
* New Sprites : Snow Hills Background ( from maYO ), BonusBox, SpinBox, Yellow Box, Power Box
* Leveleditor displays the Massivestate now as Color on the Item Menu and Selection Rect
* BonusBox can now be Invisible and Empty ( from Weirdnose )
* BonusBox Editor Settings added
* Added BonusBox "Power" Animation Type
* Added BonusBox Poison Mushroom
* Added Gee and jPiranha editor distance rects
* Leveleditor Copy Buffer is now saved between levels
* Maryo now animates on Downgrade
* Faster Editor scrolling when Shift is pressed ( from Weirdnose )
* optimized Player state change animations
* fixed moving_platform interrupted jumping if moving faster upwards
* fixed Leveleditor quit doesn't clear active object and causes crash on reload
* fixed beam Levelexit checked direction
* fixed Overworld saving could save in the last Level instead
* fixed crash if a Level was loaded and exited before an Overworld
* fixed Joystick and Mouse keys were sometimes not cleared correctly
* fixed Level small player falling image hast no white half-circle on the hat :) ( thanks to A Person )
* Linux fixes
* Level updates
* many other fixes and updates

0.99.3 (2006-11-11)
-------------------

* New XML Level format with the file ending .tsclvl
* New Enemy : Green Turtle and Spika 
* New Sprites : Desert Background ( thanks CDvd )
* New Box : Mushroom only
* New Overworld Selection Menu
* New Menu Design
* New Extra Overworld ( from Weirdnose )
* Leveleditor activateable Dialogs moved to CEGUI
* New Game Icon
* Eato left/right/down support
* Warp levelexit left/right/up support
* overall big collision detection speedup
* optimized Turtle Shell
* optimized Player star mode and item holding
* optimized Player walking collision handling
* optimized overall Enemy collision handling
* optimized Player &lt;-&gt; Enemy collision handling
* fixed Leveleditor rotated Enemies
* many other fixes and updates

0.99.2 (2006-09-14)
-------------------

* New Sprites : New Green ground tileset
* Falling Goldpieces
* Camera Limits
* Background Images Z position
* Many Levels migrated to the new default Green Tileset ( thanks Frostbringer )
* fixed Climbing crash
* fixed many collision detection problems
* fixed Rokko is drawn behind passive
* fixed Ghost enemies
* fixed Moving Platforms
* fixed saving of Multiple Background Images
* many other fixes

0.99.1 (2006-08-08)
-------------------

* Joystick Button Configuration
* Smart Camera Speed Configuration
* Added Vertical and Horizontal Moving Platform
* Added Many Editor Object Settings
* Added new Particle Animations
* Maryo Stop Sound
* New Sprites : Desert Tileset, Ballon Tree, New Mushroom, Beanstalk, Tendril, Grass, Screw Block, Ice Block, Brick Blocks, Brick Ground Tileset, Metal Pipe Connector, Mushroom Platform, Cain, Rope and Overworld Objects
* optimized drawing with Z positions
* updated Level load dialog
* optimized Gee and Eato
* updated Rokko
* updated Cloud
* optimized climbing
* fixed Leveleditor didn't delete front passive objects
* other fixes

0.99 (2006-07-17)
-----------------

* New Level Settings Design
* New Level Global Effects features
* New Level Settings : Level Author, Level Version and Background Image Speed
* New Level Editor Multiple Sprite Selection
* New Overworld : world_2
* New Enemies : Eato and Gee
* New Sounds
* New Fonts and better Font shadows
* New Sand and Grass Background
* New Smart Camera
* Overworld supports background color, music and entering another overworld from a waypoint
* New Rain Global Effect sprite
* New Sprites : Pipe Connection Blocks, Metal and extra Blocks, Castle Windows, Clouds, Sphinx, Hedges and Trees
* Updated Level Editor copy and fast copy Buffer
* Updated Effects
* Updated type collision detection
* Updated Levels
* Goldpieces are drawn correctly
* fixed rare Mushroom, Turtle Shell and Rokko wrong collision detection
* fixed leveleditor goldpiece creation

0.98.1 (2006-06-14)
-------------------

* More item effects
* New Goldpiece and Goomba Graphics
* New Savegame System
* New Enemy : Thwomp
* Global Level Effects
* Updated Overworld Waypoint collision handling
* Fixed Fullscreen double buffer flicker
* Fixed Dialog drawing
* Optimized box activation handling
* Optimized sublevel levelexit

0.98 (2006-05-17)
-----------------

* OpenGL Graphics Engine
* Updated Sound Engine
* Image File Settings
* Updated Main Menu
* Sound and Music Volume
* Star Item
* Overworld Selector
* Collision detection can handle multiple directions, objects and types
* More Enemies can be placed in more directions
* New Leveleditor Settings
* Leveleditor shows Box types
* Multiple Background Images
* Alot new and updated Sprites
* Particle Effects
* Front Passive Sprites
* Many Level updates
* Added Overworld Levels
* Alot bugfixes and changes

0.97 (2005-07-04)
-----------------

* Maryo can now climb
* Jumping piranha con now jump in all directions
* Keyrepeat enabled in editboxes
* Support for basic background images
* New background image ( from MaYO )
* New Menu Music Theme( from LoXodonte )
* New Overworld Maryo images ( from Enzakun )
* New Linux compilation stuff ( from Boder )
* New Levelexit system with new types
* Moon Animation updated
* Collision detection updates
* Enemy collision detection updated
* Player collision detection updates
* Better Player positioning when changing the size
* Levels updated
* Leveleditor updated
* Many other updates and fixes

0.96 (2005-05-09)
-----------------

* Added Fireball Animations
* Added 2 new Overworld Levels ( from pasol )
* Added Background Gradient support ( thanks Ricardo )
* New Graphics : Turtle, Spinbox, Plastic tileset, jungle tiles, Clouds redone, climbing plant head, other small changes
* Fixed Savegame loading deaths
* Fixed wrong collision handling with Rex
* Leveleditor updated
* Overworld saving fixed ( doesn't cost points anymore )
* Overworld collision handling fixed and updated
* Overworld Waypoint drawing updated
* Updated the background images support ( BETA )
* Fixed the Black Fading Effect ( thanks Ricardo )
* Fixed the Video Apply crash
* Updated the Itembox Item handling
* Updated the Savegame Menu
* Camera movement updated
* Levels updated

0.95 (2005-03-28)
-----------------

* Some Documents are now in HTML
* Completely New Graphics, Music and Sounds
* Command-line features added
* Many Levels optimized
* Updated the Audio Engine
* Updated the Input Handler
* Updated the Itembox
* Updated the Menu
* Updated the Overworld
* Updated the ducking routine
* Updated the Box collision routines
* Leveleditor updates
* Leveleditor : The first 2 created Objects could not change their state
- Removed Extra Level Commands - The settings are already available in the Leveleditor


0.94.1 (2004-12-04)
-------------------

* New internal Level parser - Should be more robust
* Overworld : Live display position corrected
* Many level error messages used wrong data
* The Itembox item won't be activated anymore if maryo dies
* Fixed a rare Audio deinitialisation bug
* Fixed if you loaded a Level in the Menu the game crashes after completion
* Some Main-Menu code optimised
* The Savegame text will now only be Auto-deleted if the Slot was empty
* The Turtle Collision sound is only played if the colliding Object is visible on the Screen

0.94 (2004-11-30)
-----------------

* Leveleditor : When CTRL is pressed with the right Mouse button every intersecting object will now be deleted
* Leveleditor : New Yoshi world and Ghost Sprites
* Leveleditor : Level settings menu
* Leveleditor : Spacer between Main menu buttons
* Leveleditor : fixed a possible level saving error
* Leveleditor : wrong default Object types changed
* The default Messagebox now allows more characters
* Maximal Fireballs set to two
* The HUD could be drawn two times
* Playing many sounds at once could crash the game
* If Maryo dies or exits the level every object should now be drawn correctly
* The stored item gets automatically activated now
* The savegame description won't get automatically deleted anymore
* Many Levels improved
* Overworld walking was ignored if the next level direction was left
* More intelligent level saving and loading

0.93.2 (2004-10-14)
-------------------

* Fixed a jumping Piranha collision detection bug
* Fixed rex animation when colliding with a shell
* Fixed collision detection between enemies and items

0.93.1 (2004-10-10)
-------------------

* New improved Collision detection changes
* Leveleditor : the Goldpieces weren't saved
* Fixed some turtle collision detection bugs
* Leveleditor : fixed the wrong saving of "/"

0.93 (2004-10-09)
-----------------

* New Enemy : Rex
* New Item : Moon ( 3 Level UP )
* Improved the collision detection for Mushrooms and the Turtle

0.92.2 (not released)
---------------------

* New Overworld Levels ( markoff_01 and fluxy_1 )
* The Pointinfo Text will get more yellow the higher the points you got
* Fixed rare Turtle collision bugs
* The animation when Maryo dies in higher areas takes now the correct time
* Fixed : If a save was loaded with an earlier progress or the game gots a reset and you won an earlier level than before you cannot enter the next level
* Fixed if maryo dies with 0 lives the statustext displayed -1 lives
* Fixed many Mushroom Collisions
* Fixed jumping piranha only comes out if the player is near
* Fixed Mushroom &lt;-&gt; ActiveObjects Collision detection
* Leveleditor : Fixed if the mousecursor moved an Object over a Leveleditor Menu the Menu got activated
* Improved the Collision detection
* Fixed an rokko crash
* Optimised the Turtle &lt;-&gt; Player Collision detection
* Optimised the Active Object movement

0.92.1 (2004-09-14)
-------------------

* Leveleditor : New Main Menu's
* Leveleditor : has now the ability to scroll with the mouse ( with the middle mouse key )
* Fixed fastcopy with Enemystopper
* Many Levels Improved
* The camera is now set correctly back when leaving the Leveleditor
* Improved the Enemy &lt;-&gt; Enemy Collision detection
* Jumping Pranha and Rokko are now drawn correctly if maryo dies
* Unified Framerate correction for better non-Windows support
* Improved the complete Collision detection

0.92 (2004-08-22)
-----------------

* Added the Itembox
* Saving the Game costs now 3000 points
* Leveleditor : New Green_1 and Pipe ground Tilesets
* Linux Support and maybe other operating systems are now supported as well
* Leveleditor : implemented the Home and End keys
* Leveleditor : shows now camera limit lines 
* Addedfunction for loading unknown levels with the l key
* New Enemy &lt;-&gt; Enemy Collision detection added
* Fixed Shell Collisions with Maryo on specific situations
* Little speedup
* Fixed a Game-exit bug
* Fixed Rokko stops moving if colliding with Enemies or Player
* Fixed Enemies stops moving if colliding with Rokko ( OO' )
* Halfmassive Objects are now drawn properly
* Menu alignment is now correct in both Resolutions
* Fixed long Level loading times could cause wrong movement
* Fixed Error messages when loading a Level
* Fixed : some Enemies were sometimes not drawn on collisions
* The Screen effects should now run on every system with the equal time
* Enemies are now walking through halfmassive Objects
* Fixed a rare Goomba crash bug
* Advanced Halfmassive support for Enemies and other Objects
* Fixed a Savegame bug
* Menu : Fixed Music don't stops playing after setting off

0.91.5 (2004-06-17)
-------------------

* Rewritten the detection Routine to check if Maryo is stuck
* Fixed an bug when an Savegame points to an non existent Level
* Rokko is now drawn over Massive Sprites
* Leveleditor : Enemystopper are now selected correctly
* Savegame saving works now correctly with unknown Levels
* Active Sprites are now drawn properly
- no console window in release builds

0.91.4 (2004-05-23)
-------------------

* Maryo can now kill Enemys if he is invincible
* Fixed Window Quitting 
* Fixed the Cloud - Enemy Collisions if Maryo is on this Cloud
* Improved Level 2
* Fixed the Player - Cloud ground detection
* Leveleditor : Fixed Enemystopper - Mouse Collision Checking
* Fixed the Enemy drawing in the no update mode
* Leveleditor : Fixed Player Start position was not set

0.91.3 (2004-05-09)
-------------------

* Fixed the Fireball-Enemy collision detection again
* Fixed the 1024x768 Resolution Bugs ( still slow and Unsupported )
* Leveleditor Menu Option New Level : fixed and changed

0.91.2 (not released)
---------------------

* Savegames now support Unicode names
* Improved and optimized the new tiles in Level 1, 2, 3, 4, 5 and 6
* Fixed many Savegame crash bugs
* replaced lvl_1 with yuki_1
* Enemystoppers are now drawn properly in the Leveleditor
* Jumping Piranhas are now drawn properly in the game
* Fixed/Changed Maryo's ducking
* Fixed Leveleditor saves over an existing wrong Level
* Fixed Music does not stop playing when disabled
* Many little speed improvements and better Enemy and Active out of range detection

0.91.1 (not released)
---------------------

* added a new level editor menu sprite option : ground objects
* improved and optimized level 1, 3 and 6
* maryo can not fire a fireball anymore if ducked
* fixed an overworld player direction bug if in the last available waypoint
- removed some unneded images

0.91 (2004-04-27)
-----------------

* music now fades in and out
* added and updated the history, keyboard control and extra level commands
* fixed some wrong levels objects
* fixed music doesn't play
* fixed invalid music commands in level 5 and 6
* fixed mushroom is drawn after passive objects

0.90 (2004-04-26)
-----------------

* added turtle shell collision with every powerup and goldbox
* added overworld
* added animations
* added 1-up mushroom
* added new framerate correction which is a lot smoother
* added higher quality music files in ogg vorbis
* many images optimized
* many ingame level editor enhancements
* many more features !
* realigned point system
* every level updated
* updated collision checking
* many bugs fixed
- removed level music volume setting

0.811 (2004-01-03)
------------------

* added two new sounds
* red goomba now gives 50 points
* red turtle points changed
* sound file normalized
* some collision fixes
* images optimized : goldpiece, hud sprites, stones, fireball, star, fire maryo, palm, kplant, lmario1, wall
* some levels updated

0.810 (2004-01-01)
------------------

* new enemy : red goomba
* graphical text fixes and changes
* rare rokko collision problem fixed
* level editor fixes
* level limit changed from 16 to 32
* jumping from enemy changed
* player fixes
* level editor now draws dead enemies properly
* rokko images
* other fixes/changes

0.89 (2003-12-31)
-----------------

* turtle changes and fixes
* fireballs are jumping and got a better animation
* some game over fixes
* point text changes
* small speedups
* debugmode changes
* every box is now moving upwards if hit

0.88 (2003-12-24)
-----------------

* some stones can now kill enemys and kick objects if maryo jumps from below
* bouncing Goldpiece fixes
* level possible fixes
* enemy fixes
* turtle changes and fixes
* maryo changes
* mousecoursor collision is now pixel precise
* mushroom changes
* rokko game crash fixed
* other fixes/changes

0.87 (not released)
-------------------

* level editor rectangle boxes now shaded and fixed
* fireplant fixes
* fireball fixes
* player possible fixes
* rokko fixes and will is now properly drawn
* if you get a live because you have 100 Goldpieces a text will appear
* little maryo physics changes and fixes
* bouncing goldpiece fixes

0.86 (2003-12-22)
-----------------

* if maryo gets points a small point text will appear
* shells now play sounds and you get coins if they kill enemies
* rokko is not drawn anymore if you die

0.85 (not released)
-------------------

* fixed a shell collison bug
* level editor : rectangle boxes for better object selection

0.84 (2003-12-21)
-----------------

* spinbox spins only for 5 seconds
* noxes are now drawn after passive objects
* mushrooms are not colliding anymore with an enemystopper
* fireballs are drawn properly over an enemystopper

0.83 (2003-12-18)
-----------------

* history text added
* different music is played in the official levels
* fixed text with shadow
* audio options added and updated
* options updated
* controls options fixed

0.82 (2003-12-15)
-----------------

* updated in many different parts
* hud changes and speedups
* fixed changing screen resolution
- removed 640x480 resolution support

0.81 (2003-12-15)
-----------------

* maryo jump sound will not anymore played if he hits an enemy
* rokko drawing fixes

0.80 (not released)
-------------------

* new enemy : rokko
* piranhas are not jumping out anymore if maryo is in front

0.78.1 (2003-12-14)
-------------------

* camera changes
* small maryo changes

0.78 (2003-12-13)
-----------------

* fixed camera bugs
* FPS correction changed
* many collision fixes and better player stuck position testing

0.77 (2003-12-13)
-----------------

* music can now be disabled ingame with F11
* game keys description added
* level editor object moving is now more precise
* fireballs don't collide anymore with enemystoppers
* many enemy collision updates !
* camera up/down handling changed
* level editor delete bug fixed
* some levels fixed and updated
* goldpieces are now drawn properly again in the level editor
* level editor : spawned objects are not saved
* level editor : some fixes

0.76 (2003-12-11)
-----------------

* some new sounds
* level editor objects updated
* physics modified and some fixes
* piranha images optimized
* something else ... o.0

0.75 (2003-12-10)
-----------------

* camera modified
* jumping piranha modified
* all levels updated
* if maryo enters the next level the size is kept
* physics modified
* level music will be played again if entered from menu
* Piranha jump start is randomized
* level editor : piranhas can now be copied an properly selected
* fireballs are now changing enemies into goldpieces
* goldpieces are drawn properly

0.74 (2003-12-10)
-----------------

* new images
* new enemy : jumping piranha

0.73 (2003-12-09)
-----------------

* possible maryo bug fixed
* jumping modifications
* physic changes and fixes
* joypad jumping fixed

0.72 (2003-12-07)
-----------------

* player doesn't fall below the ground if in godmode
* object physics improved

0.71 (not released)
-------------------

* mouse is now drawn ingame
* major level editor bugs fixed
* level editor updates
* player can't jump anymore if he's ducked
* main menu updated
* player tries to recover from some stuck positions

0.70 (2003-12-02)
-----------------

* new audio engine
* new sounds : mushroom, fireball, door, size change, level up
* main menu music added
* music command is now in a Level avilable
* implemented camera mod for levels
* implemented background color for levels
* settings are saved
* added missing duck animation
* player jump and move optimized
* collision detection with enemies optimized
* audio output optimised
* fireballs are now faster and automatically destroyed if far away
* moving objects in the level editor optimized
* many objects are now drawn in the correct order
* fixed some savegame bugs
* shells now ignore enemystoppers
* player running optimized if in big or fire type
* fireplant got new images and moves
* turtle shells now move correctly
* new sprites
* fixed joystick bugs
* player now needs to stand on the ground if entering the door
* level editor can now copy every object
* level editor positionig with the mouse is more precise
* fixed level editor bugs
* performance improvements
* savegame description dialog optimized
* fixed changing resolution causes menu errors
* fixed menu bugs
* level editor updated
* music is not played again while dying

0.64 (2003-11-19)
-----------------

* menu is now accessible with a joystick
* implemented video options
* sprites optimized ( goomba, grass, hedges, maryo, plants, mushrooms, flower, shells )
* joystick Support optimized

0.62 (2003-11-18)
-----------------

* implemented  joystick Support
* savegame names now supprt upper case

0.60 (2003-11-15)
-----------------

* implemented savegame support
* savegames now show the save time
* implemented level editor sprite copy &amp; paste function
* position display in the level editor
* audio menu selection bug fixed
* level editor tile selection key changed from 0 to F1
* the game does not quit anymore if you press esc ingame or in the level editor
* fixed screenshot bug
* fixed savegame loading bug
* very small optimizations in lvl 1 ^^
* if you die the level does not get reloaded (makes it harder !)
* savegames are now saved with the exact player position
* sprites optimized ( clouds, maryo, turtle, pipes, goldpieces, yoshi, grey stone, sign )
* exact object movement in the level editor
* enemystopper was sometimes not shown in the level editor
* audio quality improved
* if you select an object in the level editor menu it is placed at the mouse pointer position
* level editor menu extended
* fixed the savegame description could change when saving
* level editor maryo position bug fixed
* level editor bug fixed with some newly created objects from the menu which got moved afterwards

0.53 (2003-02-28)
-----------------

* linux version
* implemented fireballs
* new levels
* less values for jump power / speed
* new font

0.51 (2003-02-24)
-----------------

* added SDL_ttf libary
* implemented main menu
* fixed falling through the ground

0.50 (2003-02-22)
-----------------

* added command line switches for resolution change (--res=[resolution])
* added command line switch for no fullscreen (--nofullscreen)
* reduced the size of each image by factor 0.7, now better overview
* modified the control feelings, especially for jump behavior
* bugfixes
* changed levels so that they fit into the new image sizes

0.49 (2003-02-17)
-----------------

* bugfixes, there were some problems with the boxes and with big
  maryo

0.48 (2003-02-16)
-----------------

* implemented mushrooms
* implemented fireplants
* player can get powerups
* new level
* new jumping controls

0.47 (not released)
-------------------

* implemented gold box
* implemented bonus box
* implemented spin box

0.46 (2003-02-13)
-----------------

* performance improvements
* fixed halfmassive objects have problems with enemies
* bugfixes

0.45 (2003-02-10)
-----------------

* implemented halfmassive support
* fixed duck support
* halfmassives sprites behave like the clouds

0.44 (2003-02-09)
-----------------

* hud status bar background
* implemented level exit object
* lvl_list.txt in data/levels
* bugfixes

0.43 (2003-02-04)
-----------------

* implemented hud status bar

0.42 (2003-02-04)
-----------------

* player has lives and gold
* player gets 100 gold and one live
* gold and life are displayed
* some bugfixes
* level editor bugfixes
* game does not quit anymore when player dies
* started dynamic memory allocation and more

0.41 (2003-02-02)
-----------------

* level editor extended

0.40 (2003-02-01)
-----------------

- added level editor

0.32 (2003-01-31)
----------------

* sound support enabled
* debug display of x/y coordinates for level designers
* extended level a bit

0.31 (2003-01-30)
-----------------

* performance improvements (images are now only loaded once)
* extended level a bit
* show an error message when an error occurs in the first level

0.30 (2003-01-30)
-----------------

* points and time display
* bugfixes

0.29.6 (2001-01-28)
-------------------

* bugfixes
* performance improvements
* smaller changes

0.29.5 (2003-01-28)
-------------------

* new enemy : red turtle
* bugfixes

0.29 (2003-01-27)
-----------------

* new clouds on which you can jump and they move
* many new images
* new level
* smoother graphics, looks great
* many bugfixes

0.28 (2003-01-24)
-----------------

* implemented hitting goomba

0.27 (2003-01-23)
-----------------

* start of history
* new enemy : goomba
* new images
* bugfixes

0.00 (2003-01-01)
-----------------

* TSC was registered on the 2003.1.1 on Sourceforge :)
