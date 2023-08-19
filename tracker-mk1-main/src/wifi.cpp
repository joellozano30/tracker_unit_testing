// #include "wifi.h"

// t_wifiAp* wifiSearchForWiFi(uint8_t *_mac);

// uint8_t wifiFound;
// t_wifiAp wifi[WIFISCAN_MAX_AP];

// static const char ssidFiltered[WIFISCAN_SSIDFILTERLEN][16]= {
//   "android",
//   "phone",
//   "samsung",
//   "huawei",
//   "tp-dis" 
// };

// void wifiInitWithoutFS(void)
// {
//   uint32_t wifiStartTime = millis();
//   const char* ssid = "BANET";
//   const char* password = "4271051500";

//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, password);
//   while(WiFi.status() != WL_CONNECTED && ((millis() - wifiStartTime) < WIFI_TIMEOUT))
//   {
//     delay(500);
//     #ifdef WIFI_DEBUG
//       Serial.print(".");
//     #endif
//   }

//   #ifdef WIFI_DEBUG
//   Serial.println("");
//   Serial.println("WiFi connected");
//   Serial.println("IP address: ");
//   Serial.println(WiFi.localIP());
//   #endif
// }

// void wifiStartScan(uint32_t timeoutMs, uint8_t maxAp, boolean filtered)
// {
//   WiFi.mode(WIFI_STA);

//   bool scanHidden = (filtered)?false: true;

//   uint32_t start = millis();

//   if(maxAp > WIFISCAN_MAX_AP)
//     maxAp = WIFISCAN_MAX_AP;
  
//   wifiFound = 0;

//   while((millis() - start) < timeoutMs && wifiFound < maxAp)
//   {
//     int n = WiFi.scanNetworks(false, scanHidden);
//     for(int i=0; i < n; i++)
//     {
//       if(filtered && wifiFiltering(i)) 
//         continue;
//       wifiAddWifi(WiFi.BSSID(i), WiFi.RSSI(i), false);
//     }
//   }

//   WiFi.mode(WIFI_OFF);
//   delay(1);
// }

// bool wifiFiltering(int index)
// {
//   // Test for multicast, locally administered and full of FF
//   uint8_t *mac = WiFi.BSSID(index);
//   uint8_t firstMacByte = mac[0];
//   if((firstMacByte & 0x03) != 0)
//     return true;
  
//   // Test for full of 0
//   if(firstMacByte == 0)
//   {
//     int i=0;
//     while (i < 6)
//     {
//       if(mac[i] != 0)
//         break;
//       i++;
//     }
//     if (i == 6)
//       return true;
//   }
//   yield();
  
//   // test if network is hidden
//   if(WiFi.isHidden(index))
//     return true;
  
//   // test SSID
//   String ssid = WiFi.SSID(index);
//   ssid.toLowerCase();
//   for(int i=0; i < WIFISCAN_SSIDFILTERLEN; i++)
//   {
//     if(ssid.indexOf(ssidFiltered[i]) > 0)
//       return true;
//   }

//   return false;
// }

// int wifiGetFirstAndSecondBestAPs(uint8_t *mac1, uint8_t *mac2)
// {
//   switch(wifiFound)
//   {
//     case 0:
//       return 0;
//     case 1:
//       for (int k=0; k < 6; k++)
//         mac1[k] = wifi[0].mac[k];
//       return 1;
//     default:
//       int best1 = 0;
//       int8_t rss1 = -128;
//       int best2 = 0;
//       int8_t rss2 = -128;

//       for(int i=0; i < wifiFound; i++)
//       {
//         if(wifi[i].rssi >= rss1)
//         {
//           best2 = best1;
//           rss2 = rss1;
//           best1 = i;
//           rss1 = wifi[i].rssi;
//         } 
//         else if(wifi[i].rssi >= rss2)
//         {
//           best2 = i;
//           rss2  = wifi[i].rssi;
//         }
//         for(int k=0; k < 6; k++)
//           mac1[k] = wifi[best1].mac[k];
//         for(int k=0; k < 6; k++)
//           mac2[k] = wifi[best2].mac[k];
//       }
//       return 2;
//   }
// }

// void wifiAddWifi(uint8_t *_mac, int32_t _rssi, bool unicastOnly)
// {
//   // filter the multicast addresses
//   if(unicastOnly && (_mac[0] & 0x01) == 0x01)
//     return;
  
//   t_wifiAp * entry = wifiSearchForWiFi(_mac);

//   if(entry == NULL && wifiFound < WIFISCAN_MAX_AP)
//   {
//     for(int j=0; j < 6; j++)
//       wifi[wifiFound].mac[j] = _mac[j];
    
//     wifi[wifiFound].rssi = (int8_t)((_rssi < -128)?-128:_rssi);
//     wifiFound++;
//   }
//   else
//   {
//     // update the RSSI if better
//     if(_rssi > entry->rssi)
//     {
//       entry->rssi = (int8_t)((_rssi<-128)?-128:_rssi);
//     }
//   }
// }

// t_wifiAp* wifiSearchForWiFi(uint8_t * _mac)
// {
//   for(int i=0; i < wifiFound; i++)
//   {
//     uint8_t c = 0;
//     while(c < 6 && wifi[i].mac[c] == _mac[c])
//       c++;
//     if(c == 6)
//       return &wifi[i];
//   }
//   return NULL;
// }

// bool wifiCompareMacAddresses(uint8_t *mac1, uint8_t *mac2, structMacPair *saved_macs)
// {
//   if((memcmp(mac1, saved_macs->mac1, 6) != 0) && (memcmp(mac1, saved_macs->mac2, 6) != 0))
//     return false;
//   if((memcmp(mac2, saved_macs->mac1, 6) != 0) && (memcmp(mac2, saved_macs->mac2, 6) != 0))
//     return false;
  
//   // getting here means that both mac addresses are the same ones as the previously saved macs
//   return true;
// }

// void wifiPrintMac(uint8_t *mac)
// {
//   for(int i=0; i < 5; i++)
//   {
//     Serial.print(mac[i], HEX);
//     Serial.print(":");
//   }
//   Serial.println(mac[5], HEX);
// }