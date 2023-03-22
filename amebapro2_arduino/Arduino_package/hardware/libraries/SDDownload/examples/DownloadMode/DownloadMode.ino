/*
 This sketch is change version to SD download mode for AT command version.
 */

#include "HUB8735FatFS.h"

char filename[] = "hub8735.bin";

HUB8735FatFS fs;

void setup() {
    char path[128];
    bool b_result;
    b_result = fs.begin();
    
    if (b_result)
    {
	    sprintf(path, "%s%s", fs.getRootPath(), filename);
	    File file = fs.open(path);	
	    int size = file.available();	
	    printf("==== size = %d ====\r\n", size);
	    file.close();
	    printf("==== content ====\r\n");
	    fs.sd_upgrade();
	    printf("====   end   ====\r\n");
	
	    fs.end();
	}
	else
		printf("====   sd init failed   ====\r\n");
}

void loop() {
    delay(1000);
}
