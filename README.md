# Shop Vac Controller

An Arduino Nano-based project to provide automatic control for a small workshop vacuum by either sensing tool use, or by a keyfob style remote control.

![schematic](images/schematic.jpg)

Four LEDs display the status of the controller and its trigger source. It should be noted that the "Vac On" LED flashes if a startup or shutdown delay is in progress.

 Power control for the vacuum is provided by a 20A solid state relay and the remote control by a small keyfob and receiver ( I used a [**fob/receiver** from Amazon](https://www.amazon.co.uk/gp/product/B08SHQ749R/)).

Power-tool on/off sensing is provided by a small circuit by [**0033mer**](https://www.youtube.com/user/0033mer) using a cheap current transformer  I found on [**YouTube**](https://www.youtube.com/watch?v=gvBVxQGS_OU&t=280s).

Both the *sensitivity* of the current sensor and the turn-off delay for the vacuum control relay are adjustable by preset potentiometers.

Whilst the remote has no turn-on delay, the vacuum turns on 3s after the power tool is started (this setting may be adjusted in the Arduino code). This delay helps to reduce the excessive surge current created if both the power tool and the vacuum are turned on together.

The turn-off delay is the same for both the power tool and the remote fob, and can easily be adjusted between 3s and 30s with a preset potentiometer, this allows time for the wood chips and sawdust to be cleared from the vacuum hose.

It should be noted that if the vacuum is initiated by the mains-powered tool turning on, it can only be turned off by stopping the tool, and, in a similar way, if initiated by the remote fob it can only be turned off by the remote fob.

The vacuum power circuit should include a suitable fuse.
