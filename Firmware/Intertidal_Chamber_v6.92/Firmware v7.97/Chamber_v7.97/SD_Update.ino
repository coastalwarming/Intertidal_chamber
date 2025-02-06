

void updateFromFS(fs::FS &fs) {

  listDir(SD_MMC, "/", 0);

  File updateBin = fs.open("/update.bin");
  Serial.print("updateBin:");
  Serial.println(updateBin);

  if (updateBin) {
    if (updateBin.isDirectory()) {
      Serial.println("Error, update.bin is not a file");
      updateBin.close();
      return;
    }

    size_t updateSize = updateBin.size();
    Serial.print("updateBin.size():");
    Serial.println(updateBin.size());

 
    if (updateSize > 0) {
      Serial.println("Try to start update");


      tft.fillScreen(BLACK);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      drawCentreString("Detected new Firmware.", 240, 140, 22, 3);
      drawCentreString("Updating...", 240, 180, 11, 3);

      performUpdate(updateBin, updateSize);
    }
    else {
      Serial.println("Error, file is empty");
    }

    updateBin.close();

    // whe finished remove the binary from sd card to indicate end of the process
    fs.remove("/update.bin");
    ESP.restart();
  }
  else {
    Serial.println("Could not load update.bin from sd root");
  }
}

void performUpdate(Stream &updateSource, size_t updateSize) {
  if (Update.begin(updateSize)) {
    size_t written = Update.writeStream(updateSource);
    if (written == updateSize) {
      Serial.println("Written : " + String(written) + " successfully");
    }
    else {
      Serial.println("Written only : " + String(written) + "/" + String(updateSize) + ". Retry?");
    }
    if (Update.end()) {
      Serial.println("OTA done!");
      if (Update.isFinished()) {
        Serial.println("Update successfully completed. Rebooting.");

        tft.fillScreen(BLACK);
        tft.setTextColor(GREEN);
        tft.setTextSize(3);
        drawCentreString("Firmware Updated.", 240, 140, 17, 3);
        drawCentreString("Rebooting.", 240, 180, 10, 3);
        delay(3000);



      }
      else {
        Serial.println("Update not finished? Something went wrong!");
      }
    }
    else {
      Serial.println("Error Occurred. Error #: " + String(Update.getError()));
    }

  }
  else
  {
    Serial.println("Not enough space to begin OTA");
  }
}
