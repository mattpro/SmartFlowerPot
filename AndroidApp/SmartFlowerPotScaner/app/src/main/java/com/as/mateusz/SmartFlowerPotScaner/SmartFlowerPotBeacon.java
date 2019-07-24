package com.as.mateusz.SmartFlowerPotScaner;

import android.annotation.TargetApi;
import android.bluetooth.le.ScanResult;
import android.os.Build;
import android.util.Log;

/**
 * Created by Mateusz on 2016-12-01.
 */
public class SmartFlowerPotBeacon {
    private static final String TAG = "SmartFlowerPot";
    private String FName;
    private String FAddress;
    private float  FTemp;
    private float  FNrfTemp;
    private int    FLight;
    private int    FSoli;
    private int    FBatteryPercent;
    private int    FSignalRSSI;
    private int    FCounter;
    private byte[] beaconData;

    @TargetApi(Build.VERSION_CODES.LOLLIPOP)
    public SmartFlowerPotBeacon(ScanResult result) {
        FSignalRSSI = result.getRssi();
        FAddress = result.getDevice().getAddress();
        FName = result.getDevice().getName();
        if ( FName == null ) {
            FName = "Smart FP " + FAddress;
        }

        Log.i(TAG, "Name: " + FName );

        beaconData = result.getScanRecord().getBytes();

        if ( beaconData != null ) {
            FTemp           = parseTemp(beaconData);
            FNrfTemp        = parseNrfTemp(beaconData);
            FLight          = parseLight(beaconData);
            FSoli           = parseSoli(beaconData);
            FBatteryPercent = parseBatteryPercent(beaconData);

        }
        else {
            FTemp           = 0;
            FNrfTemp        = 0;
            FLight          = 0;
            FSoli           = 0;
            FBatteryPercent = 0;
        }
    }

    private float parseTemp( byte[] data ) {
        float temp = 0.f ;
        short tempShort;

        tempShort = (short) ( (((short)data[13] & 0xFF ) << 8 ) | (((short)data[14] & 0xFF ) ) );
        temp = tempShort / 10.f ;
        Log.i(TAG, "Temp = " + temp );
        return temp;
    }

    private float parseNrfTemp( byte[] data ) {
        float tempNrf;
        short tempNrfShort;

        tempNrfShort = (short) ( (((short)data[15] & 0xFF ) << 8 ) | (((short)data[16] & 0xFF ) ) );
        tempNrf = tempNrfShort / 10.f ;
        Log.i(TAG, "TempNrf = " + tempNrf );
        return tempNrf;
    }

    private int parseLight( byte[] data ) {
        int light;

        light = (int)data[20] & 0xFF;
        Log.i(TAG, "Light = " + light );
        return light;
    }

    private int parseSoli( byte[] data ) {
        int soli;

        soli = (int)data[19] & 0xFF;
        Log.i(TAG, "Light = " + soli );
        return soli;
    }

    private int parseBatteryPercent( byte[] data ) {
        int batteryPercent;

        batteryPercent = (int)data[21] & 0xFF;
        Log.i(TAG, "Light = " + batteryPercent );
        return batteryPercent;
    }

    public boolean checkSFP() {
        int check;
        boolean test;

        check = ( (int)beaconData[22] & 0xFF ) | ( ( (int)beaconData[23] & 0xFF ) << 8 ) | ( ( (int)beaconData[24] & 0xFF ) << 16 );
        if ( check == 0x9AAAEB ) test = true;
        else                     test = false;

        return test;
    }


    public void update(byte[] data) {
        FTemp           = parseTemp(data);
        FNrfTemp        = parseNrfTemp(data);
        FLight          = parseLight(data);
        FSoli           = parseSoli(data);
        FBatteryPercent = parseBatteryPercent(data);
    }

    public String getName() { return FName; }

    public String getAddress() { return FAddress; }

    public int getSignalRssi() { return FSignalRSSI; }

    public float getTemp() { return FTemp; }

    public float getNrfTemp() { return FNrfTemp; }

    public int getLight() { return FLight; }

    public int getSoli() { return FSoli; }

    public int getBatteryPercent() { return FBatteryPercent; }

    public int getCounter() { return FCounter; }

    public void incCounter() { FCounter++; }

    final protected static char[] hexArray = "0123456789ABCDEF".toCharArray();
    public static String bytesToHex(byte[] bytes) {
        char[] hexChars = new char[bytes.length * 2];
        for ( int j = 0; j < bytes.length; j++ ) {
            int v = bytes[j] & 0xFF;
            hexChars[j * 2] = hexArray[v >>> 4];
            hexChars[j * 2 + 1] = hexArray[v & 0x0F];
        }
        return new String(hexChars);
    }

}

