clearscreen.
print "Display temperature".

SET gui TO GUI(200).
set gui:x to +200.
set gui:y to -70.

LOCAL label IS gui:ADDLABEL("").
LOCAL label2 IS gui:ADDLABEL("").
SET label:STYLE:ALIGN TO "CENTER".
SET label2:STYLE:ALIGN TO "CENTER".

gui:SHOW().
UNTIL FALSE
{
    set temp to BODY:ATM:ALTITUDETEMPERATURE(altitude).
    //set label:text to "Outside-Temp: "+ ROUND (temp-273.15, 1)+" C".
    //set label:text to "Outside-Temp: "+ ROUND ((temp-273.15)*9/5+32, 1)+" F".
    //set label:text to "Outside-Temp: "+ ROUND (temp-273.15, 1)+" C  Outside-Temp: "+ ROUND ((temp-273.15)*9/5+32, 1)+" F".
	set label:text to "Outside-Temp: "+ ROUND (temp-273.15, 1)+" C".
	set label2:text to "Speed: "+ ROUND(velocity:surface:mag*3.6,1)+" km/h   ".
    WAIT (1).
}
gui:HIDE().