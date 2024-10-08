
void _callback(FtpOperation ftpOperation, unsigned int freeSpace, unsigned int totalSpace, bool Profile_received, bool Settings_received) {
  Serial.print(">>>>>>>>>>>>>>> _callback " );
  Serial.print(ftpOperation);

  Serial.print(">>>>>>>>>>>>>>> _callback:Profile_received " );
  Serial.println(Profile_received);

   Serial.print(">>>>>>>>>>>>>>> _callback:Settings_received " );
  Serial.println(Settings_received);
  
  Global_Profile_received = Profile_received;
 Global_Settings_received = Settings_received;
  
  /* FTP_CONNECT,
     FTP_DISCONNECT,
     FTP_FREE_SPACE_CHANGE
  */
  //Serial.print(" ");
  //Serial.print(freeSpace);
  //Serial.print(" ");
  //Serial.println(totalSpace);

  // freeSpace : totalSpace = x : 360

  if (ftpOperation == FTP_CONNECT) Serial.println(F("CONNECTED"));
  if (ftpOperation == FTP_DISCONNECT) Serial.println(F("DISCONNECTED"));
}

void _transferCallback(FtpTransferOperation ftpOperation, const char* name, unsigned int transferredSize) {
  Serial.print(">>>>>>>>>>>>>>> _transferCallback " );
  Serial.println(ftpOperation);
  /* FTP_UPLOAD_START = 0,
     FTP_UPLOAD = 1,

     FTP_DOWNLOAD_START = 2,
     FTP_DOWNLOAD = 3,

     FTP_TRANSFER_STOP = 4,
     FTP_DOWNLOAD_STOP = 4,
     FTP_UPLOAD_STOP = 4,

     FTP_TRANSFER_ERROR = 5,
     FTP_DOWNLOAD_ERROR = 5,
     FTP_UPLOAD_ERROR = 5
  */
  //Serial.print(" ");
  //Serial.print(name);
  //Serial.print(" ");
  //Serial.println(transferredSize);
}
