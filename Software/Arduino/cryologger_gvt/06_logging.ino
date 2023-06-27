// -----------------------------------------------------------------------------
// Logging mode 0 : File creation
// -----------------------------------------------------------------------------
#if LOGGING_MODE == 0
// Create debugging log file
void createDebugFile()
{
  // Debug log file name
  sprintf(debugFileName, "%s_%d_debug.csv", ID, UNIT);
  
  // Create debug log file
  // O_CREAT - Create the file if it does not exist
  // O_APPEND - Seek to the end of the file prior to each write
  // O_WRITE - Open the file for writing
  if (!debugFile.open(debugFileName, O_CREAT | O_APPEND | O_WRITE))
  {
    DEBUG_PRINTLN("Warning - Failed to create debug file.");
    return;
  }
  else
  {
    DEBUG_PRINT("Info - Created "); DEBUG_PRINTLN(debugFileName);
  }

  // Write header to file
  debugFile.println("datetime,battery,online_microsd,online_gnss,online_log_gnss,online_log_debug,"
                    "timer_battery,timer_microsd,timer_gnss,timer_sync_rtc,timer_log_gnss,timer_log_debug,"
                    "rtc_sync_flag,rtc_drift,bytes_written,max_buffer_bytes,wdt_counter_max,"
                    "write_fail_counter,sync_fail_counter,close_fail_counter,debug_counter");

  // Sync the debug file
  if (!debugFile.sync())
  {
    DEBUG_PRINTLN("Warning - Failed to sync debug file.");
  }

  // Update the file create timestamp
  updateFileCreate(&debugFile);

  // Close log file
  if (!debugFile.close())
  {
    DEBUG_PRINTLN("Warning - Failed to close debug file.");
  }
}
// -----------------------------------------------------------------------------
// Logging mode 1 : File creation
// -----------------------------------------------------------------------------
// Log translated data (ex: Latitude, longitude, HDOP, etc.)
#elif LOGGING_MODE == 1
// Create debugging log file
void createDebugFile()
{
  // Debug log file name
  sprintf(debugFileName, "%s_%d_dataLog.csv", ID, UNIT);
  
  // Create debug log file
  // O_CREAT - Create the file if it does not exist
  // O_APPEND - Seek to the end of the file prior to each write
  // O_WRITE - Open the file for writing
  if (!debugFile.open(debugFileName, O_CREAT | O_APPEND | O_WRITE))
  {
    DEBUG_PRINTLN("Warning - Failed to create debug file.");
    return;
  }
  else
  {
    DEBUG_PRINT("Info - Created "); DEBUG_PRINTLN(debugFileName);
  }

  // Write header to file
  debugFile.println("datetime,battery,year,month,day,hour,minutes,seconds,latitude,longitude,AltitudeEllipsoid,AltitudeMSL,HDOP,FIX");

  // Sync the debug file
  if (!debugFile.sync())
  {
    DEBUG_PRINTLN("Warning - Failed to sync debug file.");
  }

  // Update the file create timestamp
  updateFileCreate(&debugFile);

  // Close log file
  if (!debugFile.close())
  {
    DEBUG_PRINTLN("Warning - Failed to close debug file.");
  }
}
#endif
// -----------------------------------------------------------------------------
// Logging mode 1 : Data Log
// -----------------------------------------------------------------------------
#if LOGGING_MODE == 1
// Log debugging information
void logDebug()
{
  // Start loop timer
  unsigned long loopStartTime = millis();

  // Increment debug counter
  debugCounter++;

  // Open debug file for writing
  if (!debugFile.open(debugFileName, O_APPEND | O_WRITE))
  {
    DEBUG_PRINTLN("Warning - Failed to open debug file.");
    online.logDebug = false; // Set flag
    return;
  }
  else
  {
    DEBUG_PRINT("Info - Opened "); DEBUG_PRINTLN(debugFileName);
    online.logDebug = true; // Set flag
  }

  // Create datetime string
  char dateTime[30];
  sprintf(dateTime, "20%02d-%02d-%02d %02d:%02d:%02d",
          rtc.year, rtc.month, rtc.dayOfMonth,
          rtc.hour, rtc.minute, rtc.seconds);

  // Log debugging information
  debugFile.print(dateTime);                debugFile.print(",");
  debugFile.print(readVoltage());           debugFile.print(",");
  debugFile.print(gnssData.rtcYear);        debugFile.print(",");
  debugFile.print(gnssData.rtcMonth);       debugFile.print(",");
  debugFile.print(gnssData.rtcDay);         debugFile.print(",");
  debugFile.print(gnssData.rtcHour);        debugFile.print(",");
  debugFile.print(gnssData.rtcMinutes);     debugFile.print(",");
  debugFile.print(gnssData.rtcSeconds);     debugFile.print(",");
  debugFile.print(gnssData.latitudeGPS);    debugFile.print(",");
  debugFile.print(gnssData.longitudeGPS);   debugFile.print(",");
  debugFile.print(gnssData.altEllipsoid);   debugFile.print(",");
  debugFile.print(gnssData.altMSL);         debugFile.print(",");
  debugFile.print(gnssData.hdopGPS);        debugFile.print(",");
  debugFile.println(gnssData.nbrSat);       debugFile.print(",");

  // Sync the debug file
  if (!debugFile.sync())
  {
    DEBUG_PRINTLN("Warning - Failed to sync debug file.");
    syncFailCounter++; // Count number of failed file syncs
  }

  // Update file access timestamps
  updateFileAccess(&debugFile);

  // Close the debug file
  if (!debugFile.close())
  {
    DEBUG_PRINTLN("Warning - Failed to close debug file.");
    closeFailCounter++; // Count number of failed file closes
  }
                   
  // Print debugging information
  DEBUG_PRINT(dateTime);          DEBUG_PRINT(",");
  DEBUG_PRINT(readVoltage());     DEBUG_PRINT(",");
  DEBUG_PRINT(online.microSd);    DEBUG_PRINT(",");
  DEBUG_PRINT(online.gnss);       DEBUG_PRINT(",");
  DEBUG_PRINT(online.logGnss);    DEBUG_PRINT(",");
  DEBUG_PRINT(online.logDebug);   DEBUG_PRINT(",");
  DEBUG_PRINT(timer.voltage);     DEBUG_PRINT(",");
  DEBUG_PRINT(timer.microSd);     DEBUG_PRINT(",");
  DEBUG_PRINT(timer.gnss);        DEBUG_PRINT(",");
  DEBUG_PRINT(timer.syncRtc);     DEBUG_PRINT(",");
  DEBUG_PRINT(timer.logGnss);     DEBUG_PRINT(",");
  DEBUG_PRINT(timer.logDebug);    DEBUG_PRINT(",");
  DEBUG_PRINT(rtcSyncFlag);       DEBUG_PRINT(",");
  DEBUG_PRINT(rtcDrift);          DEBUG_PRINT(",");
  DEBUG_PRINT(bytesWritten);      DEBUG_PRINT(",");
  DEBUG_PRINT(maxBufferBytes);    DEBUG_PRINT(",");
  DEBUG_PRINT(wdtCounterMax);     DEBUG_PRINT(",");
  DEBUG_PRINT(writeFailCounter);  DEBUG_PRINT(",");
  DEBUG_PRINT(syncFailCounter);   DEBUG_PRINT(",");
  DEBUG_PRINT(closeFailCounter);  DEBUG_PRINT(",");
  DEBUG_PRINTLN(debugCounter);
  
  // Stop the loop timer
  timer.logDebug = millis() - loopStartTime;
}
// -----------------------------------------------------------------------------
// Logging mode 0 : Data Log
// -----------------------------------------------------------------------------
#elif LOGGING_MODE == 0
// Log debugging information
void logDebug()
{
  // Start loop timer
  unsigned long loopStartTime = millis();

  // Increment debug counter
  debugCounter++;

  // Open debug file for writing
  if (!debugFile.open(debugFileName, O_APPEND | O_WRITE))
  {
    DEBUG_PRINTLN("Warning - Failed to open debug file.");
    online.logDebug = false; // Set flag
    return;
  }
  else
  {
    DEBUG_PRINT("Info - Opened "); DEBUG_PRINTLN(debugFileName);
    online.logDebug = true; // Set flag
  }

  // Create datetime string
  char dateTime[30];
  sprintf(dateTime, "20%02d-%02d-%02d %02d:%02d:%02d",
          rtc.year, rtc.month, rtc.dayOfMonth,
          rtc.hour, rtc.minute, rtc.seconds);

  // Log debugging information
  debugFile.print(dateTime);            debugFile.print(",");
  debugFile.print(readVoltage());       debugFile.print(",");
  debugFile.print(online.microSd);      debugFile.print(",");
  debugFile.print(online.gnss);         debugFile.print(",");
  debugFile.print(online.logGnss);      debugFile.print(",");
  debugFile.print(online.logDebug);     debugFile.print(",");
  debugFile.print(timer.voltage);       debugFile.print(",");
  debugFile.print(timer.microSd);       debugFile.print(",");
  debugFile.print(timer.gnss);          debugFile.print(",");
  debugFile.print(timer.syncRtc);       debugFile.print(",");
  debugFile.print(timer.logGnss);       debugFile.print(",");
  debugFile.print(timer.logDebug);      debugFile.print(",");
  debugFile.print(rtcSyncFlag);         debugFile.print(",");
  debugFile.print(rtcDrift);            debugFile.print(",");
  debugFile.print(bytesWritten);        debugFile.print(",");
  debugFile.print(maxBufferBytes);      debugFile.print(",");
  debugFile.print(wdtCounterMax);       debugFile.print(",");
  debugFile.print(writeFailCounter);    debugFile.print(",");
  debugFile.print(syncFailCounter);     debugFile.print(",");
  debugFile.print(closeFailCounter);    debugFile.print(",");
  debugFile.println(debugCounter);

  // Sync the debug file
  if (!debugFile.sync())
  {
    DEBUG_PRINTLN("Warning - Failed to sync debug file.");
    syncFailCounter++; // Count number of failed file syncs
  }

  // Update file access timestamps
  updateFileAccess(&debugFile);

  // Close the debug file
  if (!debugFile.close())
  {
    DEBUG_PRINTLN("Warning - Failed to close debug file.");
    closeFailCounter++; // Count number of failed file closes
  }
                   
  // Print debugging information
  DEBUG_PRINT(dateTime);          DEBUG_PRINT(",");
  DEBUG_PRINT(readVoltage());     DEBUG_PRINT(",");
  DEBUG_PRINT(online.microSd);    DEBUG_PRINT(",");
  DEBUG_PRINT(online.gnss);       DEBUG_PRINT(",");
  DEBUG_PRINT(online.logGnss);    DEBUG_PRINT(",");
  DEBUG_PRINT(online.logDebug);   DEBUG_PRINT(",");
  DEBUG_PRINT(timer.voltage);     DEBUG_PRINT(",");
  DEBUG_PRINT(timer.microSd);     DEBUG_PRINT(",");
  DEBUG_PRINT(timer.gnss);        DEBUG_PRINT(",");
  DEBUG_PRINT(timer.syncRtc);     DEBUG_PRINT(",");
  DEBUG_PRINT(timer.logGnss);     DEBUG_PRINT(",");
  DEBUG_PRINT(timer.logDebug);    DEBUG_PRINT(",");
  DEBUG_PRINT(rtcSyncFlag);       DEBUG_PRINT(",");
  DEBUG_PRINT(rtcDrift);          DEBUG_PRINT(",");
  DEBUG_PRINT(bytesWritten);      DEBUG_PRINT(",");
  DEBUG_PRINT(maxBufferBytes);    DEBUG_PRINT(",");
  DEBUG_PRINT(wdtCounterMax);     DEBUG_PRINT(",");
  DEBUG_PRINT(writeFailCounter);  DEBUG_PRINT(",");
  DEBUG_PRINT(syncFailCounter);   DEBUG_PRINT(",");
  DEBUG_PRINT(closeFailCounter);  DEBUG_PRINT(",");
  DEBUG_PRINTLN(debugCounter);
  
  // Stop the loop timer
  timer.logDebug = millis() - loopStartTime;
}
#endif
