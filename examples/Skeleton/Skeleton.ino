/*
   Sketch skeleton for Arduino Manager for iPad / iPhone / Mac

   A simple test program to show the Arduino Manager
   features.

   Author: Fabrizio Boco - fabboco@gmail.com

   Version: 1.0

   06/02/2021

   All rights reserved

*/

/*

   AMController libraries, example sketches (“The Software”) and the related documentation (“The Documentation”) are supplied to you
   by the Author in consideration of your agreement to the following terms, and your use or installation of The Software and the use of The Documentation
   constitutes acceptance of these terms.
   If you do not agree with these terms, please do not use or install The Software.
   The Author grants you a personal, non-exclusive license, under author's copyrights in this original software, to use The Software.
   Except as expressly stated in this notice, no other rights or licenses, express or implied, are granted by the Author, including but not limited to any
   patent rights that may be infringed by your derivative works or by other works in which The Software may be incorporated.
   The Software and the Documentation are provided by the Author on an "AS IS" basis.  THE AUTHOR MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT
   LIMITATION THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, REGARDING THE SOFTWARE OR ITS USE AND OPERATION
   ALONE OR IN COMBINATION WITH YOUR PRODUCTS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR CONSEQUENTIAL DAMAGES (INCLUDING,
   BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE,
   REPRODUCTION AND MODIFICATION OF THE SOFTWARE AND OR OF THE DOCUMENTATION, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
   STRICT LIABILITY OR OTHERWISE, EVEN IF THE AUTHOR HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
#include <AM_WINC1500.h>

#define WINC_CS     8
#define WINC_IRQ    7
#define WINC_RST    4
#define SD_SELECT   5

/*

   WINC1500 Library configuration

*/
char ssid[]    = "myssid";      //  your network SSID (name)
char pass[]    = "mypassword";  // your network password

IPAddress ip(x, y, t, z);
IPAddress dns(x, y, t, z);
IPAddress gateway(x, y, t, z);
IPAddress subnet(x, y, t, z);

Adafruit_WINC1500Server server(port);

int status = WL_IDLE_STATUS;

/**

   Other initializations

*/


/*

   Prototypes of IOSControllerWINC1500’s callbacks

*/
void doWork();
void doSync();
void processIncomingMessages(char *variable, char *value);
void processOutgoingMessages();
void processAlarms(char *variable);
void deviceConnected();
void deviceDisconnected();
bool checkTwitter(char *variable, char *value);


Adafruit_WINC1500 WiFi(WINC_CS, WINC_IRQ, WINC_RST);

/*

   IOSController Library initialization

*/
#ifdef ALARMS_SUPPORT
  AMController amController(&server, &doWork, &doSync, &processIncomingMessages, &processOutgoingMessages, &processAlarms, &deviceConnected, &deviceDisconnected);
#else
  AMController amController(&server, &doWork, &doSync, &processIncomingMessages, &processOutgoingMessages, &deviceConnected, &deviceDisconnected);
#endif

void setup() {

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Start");

  // Check for the presence of the shield
  Serial.print("WINC1500: ");
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("NOT PRESENT");
    return; // don't continue
  }
  Serial.println("DETECTED");
  
  // attempt to connect to Wifi network
  WiFi.config(ip, dns, gateway, subnet);

  while (status != WL_CONNECTED) {

    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network
    status = WiFi.begin(ssid, pass);

    // wait 2 seconds for connection:
    delay(2000);
  }

  // print your WiFi shield's IP address

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);


  /**
     Other initializations
  */

  Serial.println("Ready");
}

/**

   Standard loop function

*/
void loop() {
  amController.loop(20);
}

/**

  This function is called periodically and its equivalent to the standard loop() function

*/
void doWork() {
  //Serial.println("doWork");
}


/**


  This function is called when the iOS device connects to Arduino and needs to initialize some widgets like switches and knobs

*/
void doSync () {
  //Serial.println("doSync");
}

/**


  This function is called for each message received from the iOS device

*/
void processIncomingMessages(char *variable, char *value) {
	//Serial.println("processIncomingMessages");
}

/**


  This function is periodically called to send messages to the iOS device

*/
void processOutgoingMessages() {
	//Serial.println("processOutgoingMessages");
}

/**


  This function is called for each fired alarm to process it

*/
void processAlarms(char *alarm) {
  //Serial.print("Alarm: "); Serial.println(alarm);
}

/**


  This function is called when the iOS device connects

*/
void deviceConnected () {

  //Serial.println("Device connected");
}

/**


  This function is called when the iOS device disconnects

*/
void deviceDisconnected () {
  //Serial.println("Device disconnected");
}

/**

  Auxiliary functions

*/
