package com.as.mateusz.SmartFlowerPotScaner;

import android.content.Context;
import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

/**
 * Created by matt on 10.09.2017.
 */


public class ListViewBeaconAdapter extends ArrayAdapter<SmartFlowerPotBeacon> {

    public ListViewBeaconAdapter(Context context) {
        super(context, 0);
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        if (convertView == null) {
            convertView = LayoutInflater.from(getContext())
                    .inflate( R.layout.item_beacon_list, parent, false);
        }

        SmartFlowerPotBeacon beacon = getItem(position);

        TextView nameView    = (TextView) convertView.findViewById(R.id.text_name);
        TextView soliView    = (TextView) convertView.findViewById(R.id.text_soli);
        TextView lightView   = (TextView) convertView.findViewById(R.id.text_light);
        TextView tempNrfView = (TextView) convertView.findViewById(R.id.text_tempNrf);
        TextView tempView    = (TextView) convertView.findViewById(R.id.text_temp);
        TextView rssiView    = (TextView) convertView.findViewById(R.id.text_rssi);
        TextView batteryView = (TextView) convertView.findViewById(R.id.text_battery);
        TextView addressView = (TextView) convertView.findViewById(R.id.text_address);
        TextView counterView = (TextView) convertView.findViewById(R.id.text_counter);

        nameView.setText( beacon.getName() );
        soliView.setText( String.format("%d%%", beacon.getSoli() ) );
        lightView.setText( String.format("%d%%", beacon.getLight() ) );
        tempNrfView.setText( String.format("%3.1f °C", beacon.getNrfTemp() ) );
        tempView.setText( String.format("%3.1f °C", beacon.getTemp() ) );
        rssiView.setText( String.format("RSSI: %d dBm", beacon.getSignalRssi()) );
        batteryView.setText( String.format("Battery: %d%%", beacon.getBatteryPercent()) );
        addressView.setText( beacon.getAddress());
        counterView.setText( "Counter: " + beacon.getCounter() );

        nameView.setTextColor( getTemperatureColor( beacon.getSoli() ) ) ;

        return convertView;
    }

    private int getTemperatureColor(float temperature) {
        //Color range from 15 - 40 degC
        float clipped = Math.max(15f, Math.min(40f, temperature));

        float scaled = ((40f - clipped) / 25f) * 255f;
        int blue = Math.round(scaled);
        int red = 0;// 255 - blue;
        int green = 255 - blue;

        return Color.rgb(red, green, blue);
    }
}
