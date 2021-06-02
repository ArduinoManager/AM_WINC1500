/*
 *
 * AMController libraries, example sketches (“The Software”) and the related documentation (“The Documentation”) are supplied to you 
 * by the Author in consideration of your agreement to the following terms, and your use or installation of The Software and the use of The Documentation 
 * constitutes acceptance of these terms.  
 * If you do not agree with these terms, please do not use or install The Software.
 * The Author grants you a personal, non-exclusive license, under author's copyrights in this original software, to use The Software. 
 * Except as expressly stated in this notice, no other rights or licenses, express or implied, are granted by the Author, including but not limited to any 
 * patent rights that may be infringed by your derivative works or by other works in which The Software may be incorporated.
 * The Software and the Documentation are provided by the Author on an "AS IS" basis.  THE AUTHOR MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT 
 * LIMITATION THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, REGARDING THE SOFTWARE OR ITS USE AND OPERATION 
 * ALONE OR IN COMBINATION WITH YOUR PRODUCTS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, 
 * REPRODUCTION AND MODIFICATION OF THE SOFTWARE AND OR OF THE DOCUMENTATION, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE), 
 * STRICT LIABILITY OR OTHERWISE, EVEN IF THE AUTHOR HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Author: Fabrizio Boco - fabboco@gmail.com
 *
 * Version: 1.0.0
 *
 * All rights reserved
 *
 */
#ifndef AM_WINC1500_H
#define AM_WINC1500_H

#define ALARMS_SUPPORT    			// uncomment to enable support for Alarm Widget
#define SD_SUPPORT        			// uncomment to enable support for SD Widget 
#define SDLOGGEDATAGRAPH_SUPPORT    // uncomment to enable support for Logged Data Widget
//#define DEBUG           			// uncomment to enable debugging - You should not need it !


#ifdef ARDUINO_SAMD_ZERO
#error THIS LIBRARY IS NOT FOR ARDUINO ZERO!!!! FOR ZERO, PLEASE USE IOSControllerWINC1500ForZero.
#endif

#if defined(ARDUINO_AVR_UNO)
#warning ALARMS WIDGETS, SD WIDGETS AND LOGGED DATA GRAPH WIDGETS NOT SUPPORTED ON UNO
	#undef ALARMS_SUPPORT
	#undef SD_SUPPORT
	#undef SDLOGGEDATAGRAPH_SUPPORT
#endif

#include <Adafruit_WINC1500.h>

#if defined(SD_SUPPORT) || defined(SDLOGGEDATAGRAPH_SUPPORT)
	#include <SD.h>
#endif 

#if defined(ALARMS_SUPPORT)

#include <Adafruit_WINC1500Udp.h>

typedef struct  {
	char 			id[12];  // First character of id is always A
	unsigned long 	time;
	bool			repeat;
} alarm;


#define MAX_ALARMS  					5			// Maximum number of Alarms Widgets
#define ALARM_CHECK_INTERVAL	60			// [s]

#endif


#define VARIABLELEN 			14
#define VALUELEN 					14

class AMController {

private:
char											_variable[VARIABLELEN+1];
char 	   									_value[VALUELEN+1];
bool	   									_var;
int       								_idx;
Adafruit_WINC1500Server 	*_server;
Adafruit_WINC1500Client  	*_pClient;
bool											_initialized;

#ifdef ALARMS_SUPPORT
#if !defined(ARDUINO_AVR_UNO)
String 										_alarmFile;
#endif
Adafruit_WINC1500UDP			_udp;
IPAddress 								_timeServerAddress;  // NTP Server Address
bool											_sendNtpRequest;
byte 											_packetBuffer[48]; 	// buffer to hold incoming and outgoing packets 
unsigned long   					_lastAlarmCheck;
unsigned long							_startTime;
#endif


/**
Pointer to the function where to put code in place of loop()
**/
void (*_doWork)(void); 

/*
Pointer to the function where Switches, Knobs and Leds are syncronized
*/
void (*_doSync)();

/*
Pointer to the function where incoming messages are processed
*
* variable
*
* value
*
*/
void (*_processIncomingMessages)(char *variable, char *value);

/*
Pointer to the function where outgoing messages are processed
*
*/
void (*_processOutgoingMessages)(void);

#ifdef ALARMS_SUPPORT
/*
Pointer to the function where alerts are processed
*
*/
void (*_processAlarms)(char *alarm);
#endif

/*
Pointer to the function called when a device connects to Arduino
*
*/
void (*_deviceConnected)(void);

/*
Pointer to the function called when a device disconnects from Arduino
*
*/
void (*_deviceDisconnected)(void);

void readVariable(void);

#ifdef ALARMS_SUPPORT

void sendNTPpacket(IPAddress& address, Adafruit_WINC1500UDP udp);
void breakTime(unsigned long time, int *seconds, int *minutes, int *hours, int *Wday, long *Year, int *Month, int *Day);

void syncTime();
void readTime();

void inizializeAlarms();
void checkAndFireAlarms();
void createUpdateAlarm(char *id, unsigned long time, bool repeat);
void removeAlarm(char *id);	

#endif

public:

#if defined(ALARMS_SUPPORT)

	AMController(Adafruit_WINC1500Server *server, 
								  void (*doWork)(void), 
								  void (*doSync)(), 
								  void (*processIncomingMessages)(char *variable, char *value),
								  void (*processOutgoingMessages)(void),
#if defined(ALARMS_SUPPORT) 								  
								  void (*processAlarms)(char *alarm),
#endif								  
								  void (*deviceConnected)(void),
								  void (*deviceDisconnected)(void));
#endif


	AMController(Adafruit_WINC1500Server *server, 
								  void (*doWork)(void), 
								  void (*doSync)(), 
								  void (*processIncomingMessages)(char *variable, char *value),
								  void (*processOutgoingMessages)(void),
								  void (*deviceConnected)(void),
								  void (*deviceDisconnected)(void));
								  
	void loop();
	void loop(unsigned long delay);
	void writeMessage(const char *variable, int value);
	void writeMessage(const char *variable, float value);
	void writeTripleMessage(const char *variable, float vX, float vY, float vZ);
	void writeTxtMessage(const char *variable, const char *value);
	
	void log(const char *msg);
	void log(int msg);

	void logLn(const char *msg);
	void logLn(int msg);
	void logLn(long msg);
	void logLn(unsigned long msg);

	void temporaryDigitalWrite(uint8_t pin, uint8_t value, unsigned long ms);
	
	void setNTPServerAddress(IPAddress address);	
	
#ifdef ALARMS_SUPPORT 	
	unsigned long now();
#ifdef DEBUG
	void dumpAlarms();
  	void printTime(unsigned long time);
#endif 
#endif 

#ifdef SDLOGGEDATAGRAPH_SUPPORT

	void sdLogLabels(const char *variable, const char *label1);
	void sdLogLabels(const char *variable, const char *label1, const char *label2);
	void sdLogLabels(const char *variable, const char *label1, const char *label2, const char *label3);
	void sdLogLabels(const char *variable, const char *label1, const char *label2, const char *label3, const char *label4);
	void sdLogLabels(const char *variable, const char *label1, const char *label2, const char *label3, const char *label4, const char *label5);

	void sdLog(const char *variable, unsigned long time, float v1);
	void sdLog(const char *variable, unsigned long time, float v1, float v2);
	void sdLog(const char *variable, unsigned long time, float v1, float v2, float v3);
	void sdLog(const char *variable, unsigned long time, float v1, float v2, float v3, float v4);
	void sdLog(const char *variable, unsigned long time, float v1, float v2, float v3, float v4, float v5);
	
	void sdSendLogData(const char *variable);

	void sdPurgeLogData(const char *variable);
#endif	

};
#endif

