#ifndef NETWORK_H
#define NETWORK_H
/**
 * @brief Configures wifi ap 
 */
void network_setup();
/**
 * @brief initializes all links and starts server
 */
void network_initServer();
/**
 * @brief stops the wifi, used in startup sequence 
 */
void network_stop();
/**
 * @brief closes webserver, turns off wifi
 * 
 */
void network_poweroff();

#endif