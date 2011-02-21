CONFIGURATION README FOR NONE GROUP BASED EXPERIMENT

PART 1: MAIN CONFIGURATION FILES
There are three main configuration files:
1. calib.txt: Same as other program written by Jeff.
2. config-experiment.txt: The configuration settings for the experiment mode
3. config-practice.txt: The configuration settings for the practice mode

PART 1.1: Format of configuration files
The config-experiment.txt and config-practice.txt follow the same format as follows:
Line 1: the coordinates for the 3D view of the object
Line 2: the coordinates for the 2D view of the object
Use the two coordinates, we can set the position of the object showing on the screen

Line 3: Minimal duration for each trial in seconds. It is used to prevent the subject
to click so fast to finish the trial.

Line 4: Number of trials per section
Line 5: The number of times that each trial will appear in one section

Line 6: Number of available textures, suppose we denote it by ts
Line 7: A line of title of how to describe each texture. We need to have it in the 
configuration file, and there should be no other lines between line 6 and line 7
Line 8 ~ (8 + ts - 1): ts lines for describing the textures
For each line, it has three components
the first one is the user-defined name(contain no space) for the texture; 
the second one is and the texture's type, 'T' means it is from a texture file,
and 'C' means it is a color;
if the texture type is 'T', the third one is the relative path to the program
plus the filename of the texture; if the texture type is 'C', the third one 
has three integers from 0 to 255, which indicating R, G and B
e.g.
t1 T ./texture/stone.bmp 
c1 C 255 255 255
the user defined texture name will be used in constraint description

Line 8 + ts: Number of conditions in this condition group. Let us denote
it by cs.

Then follows the description of conditions

PART 2: Format of condition description
Then follows the description of cs conditions. To describe each condition, 
it has to follow the format below:

Line 1: The display mode. There are two modes:
        C: Show the continuous motion of the object 
           in 3D while rotating it
        D: Show the discrete motion of the object 
           in 3D while rotating it
        
If you choose the D mode, you need to have an extra line following the Line 1
to set the number of seconds the object keep displaying, and the number of seconds
the object disappeared.


Line 2: Set it as:
1 Cylinder

Detailed explanation
Object to be used. The first column is the number of object to be used in this
condition denoting by os, and then following os columns are the names of the object to
be used. There is only one object supported at the moment, which is Cylinder.

Line 3: Set first column to 1, set the second column to 3, then the third column is
the texture name which to be used for the top of the cylinder, the forth column is
the one for the bottom of the cylinder, the fifth is the one for the side of the
cylinder. All texture names are set in the config-experiment.txt and config-practice.txt.
e.g.:
1 3 t6 t6 t7

Detailed explanation:
Number of texture groups TG, and TG lines of texture list follows:
Texture list(start with a number indicating the number of items in the list, and 
then use user-defined texture name which are separated by space. The order 
of names in the list will affect the final result. e.g. if you need all 
three faces of the cylinder use the same texture t1, then it should be
3 t1 t1 t1)
When generating a condition using specified description, it will randomly
choose a texture list from the texture group.

Line 4: Quantity of conditions: The number of times you want this condition appeared in 
the condition group. Usually set it to 1.

Line 5:  Pitch Range (Slant)
Line 6:  Yaw Range
Line 7:  Roll Range (tilt)
Line 8:  Height Range 
Line 9:  Initial Z Aspect Ratio Range 
Line 10: Rotation Speed Range (It will not take effect if the display mode is D, but we 
still need this line to maintain the format)
Line 11: Max Rotation Degree Range 
Line 12 ~ END: Object Specific Parameters     

For the Cylinder, the object specific parameter is Raduis Range.
There are two types of range. The first one is defined by the 
lower bound and upper bound of the range, and the value is randomly
choosing inside the two bounds; the second one is defined by a 
set of numbers, the value can be only choosing from the set.

Range format 1 (range is defined by a lower bound and a upper bound):
"R" "Lower Bound" "Upper Bound"
e.g.:
R 0.8 1.2

Range format 2 (range is defined by a set of possible values):
"S" "number of values in the set" "value 1" "value 2" ... 
e.g.:
S 3 0.9 1.0 1.1

Object Specific Parameters are in the order of the objects appeared in
the "Object Names". Each object can have multiple parameters, and it can
also have none parameters. e.g. Cylinder has only one parameter for radius
range
