# F1_telem_API
make f1 udp telemetry accessible over http

# Requirements
Poco (Net/JSON/Foundation/Util)

#Usage
##Config
there is a standard config file called `f1_telem_API.properties` in the git repository that contains all the 
supported options as well as their default values. The config file has to be called `f1_telem_API.properties`
and has to be placed in the same directory as the executable or else the program wont find the file.

The config options are: 

### logger.file_name
this option specifies the log file relative to the executables base directory. if not specified then the
logger will just log to the console and not log to a file at all

### UDP.port
the port the udp client listens to for f1 2018 telemetry data.

### UDP.address
the network address that the udp client listens to for f1 2018 telemetry data.

### UDP.max_post_packet_threads
the maximum amount of threads for post packet operations. this is currently only calculating the timing 
difference between drivers but may expand in the future

### HTTP.port
the port that the HTTP server will listen to for connections

## HTTP endpoint
The HTTP server currently waits for GET requests to the `/f1telem` endpoint.
this endpoint is where you get the f1 game packets returned in JSON format. 

You can find full details about the packets at 
https://forums.codemasters.com/topic/30601-f1-2018-udp-specification/

The different packets are: 
##
###Header
this is the header that accompanies every UDP packet and will always be the last one that was received. 
contains information like what index in the arrays the player is at and the current session timestamp.

all the other packets also contain the header that last accompanied that packet.

###CarSetups
this is the packet where the setups of all the cars are described. this is things like front/rear wing downforce
levels as well as ballast and fuel load that the car left parc ferme with. 

###CarStatus
this packet details the status of all the cars in the race. it includes things like damage and
the current marshal flag for the car

###CarTelemetry
this is the telemetry packet for the cars. this includes things like speed, throttle application and drs status

###EventData
this literally just includes the event string code. get details about this code from the link above

###LapData
this packet gives lap data for all the cars. things like the sector times, the previous lap time and the car position.

###MotionData
this gives motion information about the cars like its velocity in the three dimensions as well as its position
on track and its G-forces.

###Participants
this packet contains information about all the participants in the race like their name
and if they are AI controlled 

###SessionData
this packet contains information about the current session like for example the track temperature and the
status of all the marshal posts

###timing
time difference between the drivers is not something the f1 game send itself but is something this program provides.

##
these packets can all be requested from the endpoint using the `req` parameter set to the packets name.
one can request multiple packets in the same request by setting the parameter multiple times like so:

``req=Header&req=CarSetups&req=CarStatus``

requesting the same packet multiple times in the same request will result in the packet only being returned once
