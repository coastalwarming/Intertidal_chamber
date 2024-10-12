#0- Libraries
library(readxl)
library("dplyr")                
library(stringr)

#1- Set working directory to script path: 
setwd(dirname(rstudioapi::getActiveDocumentContext()$path))

#2- Check if Profile_raw_instructions.xlsx is on same path:
if(file.exists("Profile_raw_instructions.xlsx")){
  print("Confirmed Profile_raw_instructions.xlsx presence.")
}else{
    print(" Profile_raw_instructions.xlsx not detected!")
  }

#3- Check if Profile_raw_instructions.csv is on same path:
Profile_raw_instructions <- read_excel("Profile_raw_instructions.xlsx", 
                                            col_types = c("date", "numeric", "numeric", 
                                                                    "numeric", "numeric"))

#Test profile lines:
for(i in 1:nrow(Profile_raw_instructions)){
  #Test timestamp:
  if(!between(as.numeric(
    as.POSIXct(Profile_raw_instructions$Timestamp[i])),
    1704067200, 2524608000)){
    print(paste0("Timestamp at Line ",i," is not between 01/01/2024 and 01/01/2050, is that correct?"))
  }
  
  #Test temperature:
  if(!between(
    Profile_raw_instructions$Temperature[i],
    0, 99.9)){
    print(paste0("Temperature at Line ",i," must be between 0.0 and 99.9 ºC."))
  }
  
  #Test Tide:
  if(!(Profile_raw_instructions$Tide[i]!=0  || 
       Profile_raw_instructions$Tide[i]!=1)){
    print(paste0("Tide at Line ",i," must be 0 (Low tide) or 1 ( High tide)."))
  }
  
  #Test PAR:
  if(!(Profile_raw_instructions$PAR[1]%%1==0) ||
     !between(Profile_raw_instructions$PAR[1],0, 260)){
    print(paste0("Light at Line ",i," cannot have decimals and must be between 0 and 260 umol m-2 s-1."))
  }
  
  #Test Water exchange:
  if(!(Profile_raw_instructions$Water_change[i]!=0  || 
       Profile_raw_instructions$Water_change[i]!=1)){
    print(paste0("Water change at Line ",i," must be 0 (No water change ) or 1 (water change)."))
  }
}

#4- Build profile and produce Profile.txt:
for(i in 1:nrow(Profile_raw_instructions)){
  if(i==1){
    step<-1
    Profile <-c()
  }
    Profile[step]<-paste0(
      as.numeric(
        as.POSIXct(Profile_raw_instructions$Timestamp[i])),
      "-",
      str_pad(ifelse(round(Profile_raw_instructions$Temperature[i]- floor(Profile_raw_instructions$Temperature[i]), digits = 1)==1, floor(Profile_raw_instructions$Temperature[i])+1, floor(Profile_raw_instructions$Temperature[i])),2, pad = "0"),
      ifelse(round(Profile_raw_instructions$Temperature[i]-floor(Profile_raw_instructions$Temperature[i]), digits = 1)==1,0,round(Profile_raw_instructions$Temperature[i]-floor(Profile_raw_instructions$Temperature[i]), digits = 1)*10),
      Profile_raw_instructions$Tide[i],
      ifelse(Profile_raw_instructions$PAR[i]==260,"100",
             ifelse(Profile_raw_instructions$PAR[i]==0,"000",
                    str_pad(round((Profile_raw_instructions$PAR[i]-23.182)/2.5562,0), 3, pad = "0"))),
      Profile_raw_instructions$Water_change[i]
    )
    step<- step+1
  
}

#5-Check chars length, should be 19 per line:
for(i in 1:(length(Profile))){
  if(i==1)Errors=0
  if(nchar(Profile[i]) != 19){
    Errors=Errors+1
    write.table(Profile_2, file = paste0(LED_col_name,"Profile.txt"), sep = "\t",  col.names=FALSE, row.names = FALSE, quote = FALSE)
  }
  if(i==length(Profile) && Errors==0){
    write.table(Profile, file = "Profile.txt", sep = "\t",  
                           col.names=FALSE, row.names = FALSE, quote = FALSE)
    cat("\nProfile.txt file produced.\nPlace this file in the Intertidal chamber's SD card to start experiment.")    
    }

}
