# Ameba Arduino SDK (ambpro2_arduino)


## 1. About HUB 8735 Ameba Arduino

> The HUB 8735 Arduino SDK is fork from https://github.com/ambiot/ `Arduino SDK Development` `AmebaPro2 Family`
> The version mapping is following below.

|HUB 8735 Version      		| ambiot Version       |Remarks                         					|
|:--------------------------|:---------------------|:---------------------------------------------------|
|`4.0.1-build20230110` 		|`4.0.1-build20230110` | Modify pin define for HUB 8735 board 				|
|`4.0.1-build20230217` 		|`4.0.2-build20230206` | ***feature addition*** and ***bug fix***			| 
|`4.0.2-build20230220` 		|`4.0.2-build20230216` | ***feature addition*** and ***bug fix***			|
|`4.0.3-build20230322` 		|`4.0.2-build20230220` | ***feature addition*** and ***bug fix***			|
|`4.0.3-build20230510` 		|`4.0.3-build20230414` | ***4.0.3 Release***	and ***bug fix***    		|
|`4.0.4-build20230601` 		|`4.0.3-build20230414` | ***4.0.3 Release***	and ***update tools***		|
|`4.0.4-build20230615` 		|`4.0.3-build20230414` | ***4.0.3 Release***	and ***update tools***		|
|`4.0.5-Release-20230727` 	|`4.0.4-1`             | ***4.0.4 Release***								|
|`4.0.6-Release-20230922` 	|`4.0.4-1`             | ***feature addition***	Add line Notify image		|
|`4.0.7-Release-20231220` 	|`4.0.5`               | ***Add NN for Audio classification and support portable***|
|`4.0.8-Release-20240115` 	|`4.0.5`               | ***Add new board "HUB 8735 ultra"***				|
|`4.0.9-Release-20240125` 	|`4.0.5`               | ***bug fix***				                        |
|`4.0.10-Release` 	        |`4.0.6`               | ***bug fix*** and ***update tools 1.2.14***		|
|~~`4.0.11-Release`~~ 	    |~~`4.0.7`~~           | ~~***feature addition*** and ***update tools 1.3.4***~~| 
|`4.0.12-Release` 	        |`4.0.7`               | ***bug fix*** and ***update tools 1.3.5*** and ***remove 4.0.11*** |
|`4.0.13-Release` 	        |`4.0.7`               | ***feature addition*** |
|`4.0.14-Release` 	        |`4.0.8`               | ***bug fix*** and ***update tools 1.4.0*** |
|`4.0.15-Release` 	        |`4.0.9-build20250325` | ***feature addition*** and ***update tools 1.4.1*** |
</br>

## 2. How to add HUB 8735 Ameba Arduino package to `Arduino IDE`:

> You can refer to this link to get started: https://www.amebaiot.com/en/amebad-amb82-mini-arduino-getting-started/
> , or follow the steps below.

1. **Add Ameba Arduino SDK link to *Arduino IDE Additional Boards Manager***

    > Arduino IDE 1.6.5 and above versions support third party hardware so please make sure to use the latest Arduino IDE for better experience.

    Therefore, you need to add Ameba Arduino SDK link in 
    
    **"File" -> "Preferences" -> "Additional Boards Manager URLs:"**

    Copy and paste the following link into the field and click "OK",

    https://github.com/ideashatch/HUB-8735/raw/main/amebapro2_arduino/Arduino_package/ideasHatch.json

    > We also suggest to enable "Show verbose output" options on "compilation" and "upload" in Preference for easier debugging.
    
    </br>

2. **Install Ameba board in *"Board Manager"***

    Open **"Tools" -> "Board" -> "Board Manager"**, wait for it to update additional hardware configurations, then type **"HUB 8735"** or **"ideashatch"** in the search bar, you will see Realtek Ameba in the list.

    Press **"Install"** to start the installation.
    
    </br>

3. **Select your Ameba model in *"Tools" -> "Board" -> "AmebaPro2 ARM (32-bits) Boards - ideasHatch"***

    > Make sure you select the correct model for your board, otherwise your program might not work properly 

    Now you are **ready** to develop and upload firmware onto Ameba.

    For more information, please refer to https://www.amebaiot.com/en/ameba-arduino-summary/
    
    </br>
## 3. License:

> The overall project is licensed under the MIT License. See the [LICENSE](https://github.com/ideashatch/HUB-8735/blob/main/amebapro2_arduino/LICENSE.txt) file for details.
