clearscreen.
print "Display temperature".

SET gui TO GUI(200).

LOCAL label IS gui:ADDLABEL("").
SET label:STYLE:ALIGN TO "CENTER".

gui:SHOW().
UNTIL FALSE
{
    set temp to BODY:ATM:ALTITUDETEMPERATURE(altitude).
    set label:text to "Outside-Temp: "+ ROUND (temp-273.15, 1)+" C".
    //set label:text to "Outside-Temp: "+ ROUND ((temp-273.15)*9/5+32, 1)+" F".
    //set label:text to "Outside-Temp: "+ ROUND (temp-273.15, 1)+" C  Outside-Temp: "+ ROUND ((temp-273.15)*9/5+32, 1)+" F".
    WAIT (1).
}
gui:HIDE().