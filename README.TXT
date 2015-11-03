EVaRT SDK Example README File
-----------------------------

The EVaRTSDKExample program (with source code) is a console application demonstrating the most common
features of the EVaRT SDK. Simply run the executable, and follow the prompts:

1. Specify host name. The host name is the name of the computer on which EVaRT is running. This can be
the network name of the computer like, "TrackingComputer", or an IP address such as, "10.1.1.199".

2. Specify number of frames. Enter the number of frames to collect from EVaRT. The program will not
exit until it has received the specified number of frames. To kill the program early, press Ctrl-C.

3. The next series of prompts asks which types of data you want to receive from EVaRT, and the name
of the file to write the data to.

4. The final prompt asks you if you want to specify the trial name. This is equivalent to setting the
filename in the "Motion Capture"->"Output" panel in EVaRT. If you choose to specify the trial name, 
EVaRT must be in "Motion Capture" mode, either live or running a .vc file. Additionally, at least one
output must be checked in the "Motion Capture"->"Output" panel. The trial name should include a name,
number, and suffix, such as "remote_trial_1.vc1". The output files will be placed on the computer running
EVaRT, in the current project directory. After specifying the trial name, the program will ask EVaRT to
start recording (equivalent to pressing the "Record" button). After the specified number of frames have
been received, the program will ask EVaRT to stop recording (equivalent to pressing the "Stop" button).

5. Once the specified number of frames have been received, the program writes the results to the file(s) that
you specified.


NOTE: The zip file containing the example program also includes the most recent EVaRT SDK header files and
libraries, so these do not need to be downloaded separately. The example program statically links against
the EVaRT SDK library, but the project settings could be changed to use the macRTcom.lib import library which
would then use the macRTcom.dll at run-time.