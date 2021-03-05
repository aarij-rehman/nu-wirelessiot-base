// SSID and password go here
#define SSID "APT P2"
#define key "nestor123"

// Function for connecting the ESP to the internet
void esp_init();

// Function for getting the ping from the network
void get_ping();

// Function for getting the RSSI from the network
void get_rssi();

// Function for running a speed test 
void get_speed_test();

// Function to send commands to the ESP
void esp_send(char* command); 

// Function which receives a message from the ESP and puts it 
// on the buffer 
void esp_receive(char* buf)