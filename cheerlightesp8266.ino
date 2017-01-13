#include <ESP8266WiFi.h>
#define ON LOW
#define OFF HIGH

const int redPin = D2;
const int greenPin = D3;
const int bluePin = D4;

const char ssid[]     = "xxxx";
const char password[] = "xxxx";

const char host[] = "api.thingspeak.com";
static unsigned long Lastcolour = 0;

void setup()
{
  
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  Serial.begin(115200);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void loop()
{
  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/channels/1417/field/2/last.txt";

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  delay(100);
  // Read all the lines of the reply from server and scan for hex colour
  while (client.available()) {
    unsigned long colour;
    String line = client.readStringUntil('\n');
    // Example line: #808182
    if ((line[0] == '#') && (line.length() == 8)) {
      int r, g, b;
      colour = strtoul(line.c_str() + 1, NULL, 16);
      Serial.println(colour, HEX);
      // Update LEDs only when colour changes
      if (colour != Lastcolour) {
        Lastcolour = colour;

        if (colour == 0x008000) { digitalWrite(redPin, OFF); digitalWrite(greenPin, ON); digitalWrite(bluePin, OFF);  } // green
        if (colour == 0xFF0000) { digitalWrite(redPin, ON); digitalWrite(greenPin, OFF); digitalWrite(bluePin, OFF);  } // red
        if (colour == 0x0000FF) { digitalWrite(redPin, OFF); digitalWrite(greenPin, OFF); digitalWrite(bluePin, ON);  }  // blue
        if (colour == 0x00FFFF)  { digitalWrite(redPin, OFF); digitalWrite(greenPin, ON); digitalWrite(bluePin, ON);  } // cyan
        if (colour == 0xFFFFFF) { digitalWrite(redPin, ON); digitalWrite(greenPin, ON); digitalWrite(bluePin, ON); }// white
        if (colour == 0xFF00FF) { digitalWrite(redPin, ON); digitalWrite(greenPin, OFF); digitalWrite(bluePin, ON); } // magenta
        if (colour == 0xFFFF00) { digitalWrite(redPin, OFF); digitalWrite(greenPin, ON); digitalWrite(bluePin, ON); } // yellow
        if (colour == 0x800080) { digitalWrite(redPin, ON); digitalWrite(greenPin, OFF); digitalWrite(bluePin, ON); }// purple
        if (colour == 0xFFA500) { digitalWrite(redPin, ON); digitalWrite(greenPin, ON); digitalWrite(bluePin, OFF); } // orange
        if (colour == 0xFFC0CB) { digitalWrite(redPin, ON); digitalWrite(greenPin, OFF); digitalWrite(bluePin, OFF); } // pink
        if (colour == 0xFDF5E6) { digitalWrite(redPin, ON); digitalWrite(greenPin, ON); digitalWrite(bluePin, ON); } // warmwhite

      }

    }
  }

  // ThingSpeak does not allow updates more often than every 15 seconds.
  delay(15000);
}


